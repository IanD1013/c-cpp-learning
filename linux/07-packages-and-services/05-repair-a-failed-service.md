# systemd 服务启动失败：用 `systemctl status` 与 `journalctl` 定位并修复问题

## 核心思路

当一个 systemd 服务无法启动时，不要反复执行 `restart` 碰运气。正确流程是：

```text
查看状态
    ↓
查看日志
    ↓
根据错误检查实际文件
    ↓
修复根因
    ↓
必要时 daemon-reload
    ↓
重新启动
    ↓
再次验证
```

以 `course-reporter.service` 为例：

```bash
systemctl status course-reporter --no-pager
journalctl -u course-reporter --no-pager
```

这两个命令通常已经足够回答：

1. 服务为什么失败？
2. systemd 尝试执行什么？
3. 是程序自己退出，还是 systemd 根本无法启动它？

---

## 1. 先查看服务状态

```bash
systemctl status course-reporter --no-pager
```

重点关注：

```text
Loaded: ...
Active: failed (...)
Process: ...
Main PID: ...
```

其中最重要的是 `Active:` 和失败结果。

例如：

```text
Active: failed (Result: exit-code)
```

说明服务进程**已经被启动，但随后以非零退出码结束**。

这与“程序根本没有成功执行”是不同类型的问题。

### 为什么失败类型很重要

systemd 启动失败大致可以分为两类：

| 类型      | 含义                     | 常见原因                   |
| ------- | ---------------------- | ---------------------- |
| 程序启动后退出 | systemd 成功执行程序，但程序返回错误 | 配置错误、依赖缺失、程序内部异常       |
| 程序无法执行  | systemd 连程序都无法正常启动     | 文件不存在、路径错误、没有执行权限、权限问题 |

因此：

> 不要只看到 `failed` 就开始修改配置，要先判断失败发生在哪一个阶段。

---

## 2. 再查看该服务的 journal

状态页只提供摘要，更完整的信息通常在 journal 中：

```bash
journalctl -u course-reporter --no-pager
```

其中：

* `-u course-reporter`：只查看这个 systemd unit 的日志；
* `--no-pager`：直接输出到终端，不进入分页器。

日志可能直接指出：

```text
Failed to execute ...
Permission denied
```

或者：

```text
No such file or directory
```

这类信息通常比单纯的：

```text
Active: failed
```

更接近真正的根因。

### `status` 与 `journalctl` 的分工

可以把它们理解成：

```text
systemctl status
    → “这个服务现在是什么状态？”

journalctl -u
    → “它启动过程中到底发生了什么？”
```

排查 systemd 服务失败时，两者应该配合使用。

---

## 3. 根据日志找到 systemd 实际执行的程序

systemd 服务通常通过 unit 文件中的 `ExecStart=` 指定程序，例如：

```ini
[Service]
ExecStart=/usr/local/bin/course-reporter
```

如果日志显示问题出在这个程序，就应该检查磁盘上的实际文件，而不是继续重启服务：

```bash
ls -l /usr/local/bin/course-reporter
```

例如正常的系统可执行程序可能显示：

```text
-rwxr-xr-x 1 root root 12345 Sep 7 09:00 /usr/local/bin/course-reporter
```

这里包含几个重要信息：

```text
-rwxr-xr-x
root root
```

---

## 4. 理解 `ls -l` 中的执行权限

例如：

```text
-rwxr-xr-x
```

可以拆成：

```text
-  rwx  r-x  r-x
   │     │     │
   │     │     └── 其他用户
   │     └──────── 所属组
   └────────────── 文件所有者
```

其中：

* `r`：read，读取；
* `w`：write，写入；
* `x`：execute，执行。

对于普通系统命令，常见权限是：

```text
755
```

也就是：

```text
rwxr-xr-x
```

可以设置为：

```bash
sudo chmod 755 /usr/local/bin/course-reporter
```

或者更直接地添加执行权限：

```bash
sudo chmod +x /usr/local/bin/course-reporter
```

如果要求文件继续由 `root` 拥有，则可以检查：

```bash
ls -l /usr/local/bin/course-reporter
```

确认类似：

```text
-rwxr-xr-x 1 root root ...
```

如果所有者被错误修改，可以恢复：

```bash
sudo chown root:root /usr/local/bin/course-reporter
```

---

## 5. 修复根因，而不是反复重启

错误的排查方式：

```bash
sudo systemctl restart course-reporter
sudo systemctl restart course-reporter
sudo systemctl restart course-reporter
```

如果：

```text
/usr/local/bin/course-reporter
```

没有执行权限，那么每次启动都会因为同一个原因失败。

正确顺序是：

```bash
systemctl status course-reporter --no-pager
journalctl -u course-reporter --no-pager

ls -l /path/to/program

# 根据实际错误修复
sudo chmod 755 /path/to/program

sudo systemctl restart course-reporter

systemctl status course-reporter --no-pager
```

核心原则：

> **先读错误 → 修复错误指出的具体问题 → 再重新启动。**

---

## 6. 什么时候需要 `systemctl daemon-reload`

这是排查 systemd 时非常重要的区别。

### 情况一：修改了 unit 文件

例如修改：

```text
/etc/systemd/system/course-reporter.service
```

把：

```ini
ExecStart=/old/path/course-reporter
```

改成：

```ini
ExecStart=/new/path/course-reporter
```

因为 systemd 会缓存已经读取的 unit 配置，所以修改之后需要：

```bash
sudo systemctl daemon-reload
```

然后再：

```bash
sudo systemctl restart course-reporter
```

完整流程：

```bash
sudo vim /etc/systemd/system/course-reporter.service

sudo systemctl daemon-reload
sudo systemctl restart course-reporter
```

---

### 情况二：只修改了 unit 引用的程序

假设 unit 文件一直是：

```ini
ExecStart=/usr/local/bin/course-reporter
```

只是这个文件原本没有执行权限：

```text
-rw-r--r--
```

然后修改为：

```bash
sudo chmod 755 /usr/local/bin/course-reporter
```

此时：

```bash
systemctl daemon-reload
```

**不需要执行。**

直接：

```bash
sudo systemctl restart course-reporter
```

即可。

原因是：

> systemd 缓存的是 **unit 配置**，而不是 `ExecStart=` 指向的可执行文件本身。

每次启动服务时，systemd 都会重新访问该程序。

---

## 7. 判断是否需要 `daemon-reload`

可以记住这条规则：

| 修改内容                                | 是否需要 `daemon-reload` |
| ----------------------------------- | -------------------: |
| `.service` unit 文件                  |                 ✅ 需要 |
| `ExecStart=`                        |                 ✅ 需要 |
| `Environment=` 等 unit 配置            |                 ✅ 需要 |
| `/usr/local/bin/course-reporter` 内容 |                ❌ 不需要 |
| 程序的执行权限                             |                ❌ 不需要 |
| 程序的所有者                              |                ❌ 不需要 |
| 程序读取的普通配置文件                         |             通常 ❌ 不需要 |

一句话记忆：

> **改 systemd 的配置，要 reload；改 systemd 启动的东西，不用 reload。**

---

## 8. 一个典型的权限故障示例

假设 unit 中有：

```ini
[Service]
ExecStart=/usr/local/bin/course-reporter
```

检查状态：

```bash
systemctl status course-reporter --no-pager
```

显示失败。

进一步查看日志：

```bash
journalctl -u course-reporter --no-pager
```

发现类似：

```text
Failed to execute /usr/local/bin/course-reporter: Permission denied
```

检查文件：

```bash
ls -l /usr/local/bin/course-reporter
```

发现：

```text
-rw-r--r-- 1 root root ...
```

问题在于没有 `x` 执行权限。

修复：

```bash
sudo chmod 755 /usr/local/bin/course-reporter
```

再次检查：

```bash
ls -l /usr/local/bin/course-reporter
```

应类似：

```text
-rwxr-xr-x 1 root root ...
```

因为没有修改 `.service` 文件，所以不需要：

```bash
systemctl daemon-reload
```

直接：

```bash
sudo systemctl restart course-reporter
```

最后验证：

```bash
systemctl status course-reporter --no-pager
```

正常情况下应看到：

```text
Active: active (running)
```

---

## 9. 常见误区

### 误区一：服务失败后首先反复 `restart`

`restart` 不会修复：

* 路径错误；
* 文件不存在；
* 缺少执行权限；
* 配置错误；
* 程序内部异常。

如果根因没有改变，重新启动通常只会制造一条时间更新的相同错误。

---

### 误区二：所有修改后都执行 `daemon-reload`

`daemon-reload` 不是“systemd 出问题时刷新一下”的万能命令。

它的作用是：

> 让 systemd 重新读取 unit 文件。

如果只是执行：

```bash
chmod +x /usr/local/bin/course-reporter
```

则没有必要 `daemon-reload`。

---

### 误区三：只看 `systemctl status`

`status` 非常适合快速诊断，但通常只包含部分日志。

需要完整错误信息时继续执行：

```bash
journalctl -u course-reporter --no-pager
```

尤其是：

```text
Permission denied
No such file or directory
Failed to execute
```

等信息，往往能够直接确定下一步检查什么。

---

### 误区四：看到 `failed` 就认为程序本身有 bug

服务启动失败可能发生在程序真正运行之前。

例如：

```text
ExecStart=/usr/local/bin/course-reporter
```

但文件没有执行权限，那么应用代码可能**一行都没有运行**。

因此必须区分：

```text
systemd 无法执行程序
```

和：

```text
程序成功启动后自己报错退出
```

---

## 10. 推荐的 systemd 服务故障排查模板

以后遇到类似问题，可以按照下面的顺序操作：

```bash
# 1. 查看状态
systemctl status <service> --no-pager

# 2. 查看该服务日志
journalctl -u <service> --no-pager

# 3. 根据错误检查相关文件
ls -l <path>

# 4. 修复真正的问题
sudo chmod ...
sudo chown ...
# 或修改其他有问题的配置

# 5. 只有修改 unit 文件时才需要
sudo systemctl daemon-reload

# 6. 重新启动
sudo systemctl restart <service>

# 7. 验证结果
systemctl status <service> --no-pager
```

例如：

```bash
systemctl status course-reporter --no-pager
journalctl -u course-reporter --no-pager

ls -l /usr/local/bin/course-reporter

sudo chmod 755 /usr/local/bin/course-reporter
sudo chown root:root /usr/local/bin/course-reporter

sudo systemctl restart course-reporter

systemctl status course-reporter --no-pager
```

如果最终看到：

```text
Active: active (running)
```

说明服务已经正常运行。

---

## 核心总结

systemd 服务无法启动时，最重要的是建立固定的诊断顺序：

```text
systemctl status
        ↓
journalctl -u
        ↓
找到具体错误
        ↓
检查错误涉及的文件或配置
        ↓
修复根因
        ↓
restart
        ↓
重新 status 验证
```

特别记住两个原则：

1. **不要在根因没有改变时反复重启服务。**
2. **只有修改 systemd unit 文件时才需要 `systemctl daemon-reload`；修改 unit 引用的可执行程序、权限或所有者通常不需要 reload。**
