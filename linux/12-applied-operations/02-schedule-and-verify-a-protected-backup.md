# 使用 Cron 安全地以 root 身份执行每日备份

## 目标

配置一个每天凌晨 **02:00** 自动运行的备份任务：

* 备份源目录：`/srv/course-data`
* 备份脚本：`/usr/local/sbin/course-backup`
* 备份文件：`/var/backups/course-data.tar.gz`
* Cron 配置：`/etc/cron.d/course-backup`
* 恢复测试目录：`/home/learner/restore-check`
* 最终验证：能够从备份中恢复并读取 `README`

最终状态应为：

| 对象                                   | Owner  |    权限 | 用途           |
| ------------------------------------ | ------ | ----: | ------------ |
| `/usr/local/sbin/course-backup`      | `root` | `750` | root 执行的备份脚本 |
| `/etc/cron.d/course-backup`          | `root` | `644` | Cron 调度配置    |
| `/var/backups/course-data.tar.gz`    | —      |     — | 实际生成的压缩备份    |
| `/home/learner/restore-check/README` | —      |     — | 从备份中恢复出的验证文件 |

---

## 1. 为什么 root 定时执行的脚本必须严格保护

如果 Cron 以 `root` 身份执行：

```text
/usr/local/sbin/course-backup
```

那么谁能够修改这个脚本，实际上就能够让 `root` 执行任意命令。

例如，如果普通用户能够往脚本中加入：

```bash
rm -rf /some/important/path
```

下一次 Cron 执行时，这条命令就会拥有 root 权限。

因此：

> **root 自动执行的脚本必须由 root 所有，并禁止普通用户修改。**

适合的权限是：

```bash
750
```

含义：

|  数字 | 身份          | 权限    |
| --: | ----------- | ----- |
| `7` | owner（root） | `rwx` |
| `5` | group       | `r-x` |
| `0` | others      | `---` |

即：

```text
rwxr-x---
```

普通用户不属于对应组时甚至不能直接执行该脚本。

这不是配置错误，而是安全设计。

因此手动测试时通常需要：

```bash
sudo /usr/local/sbin/course-backup
```

---

## 2. 创建备份脚本

脚本需要把：

```text
/srv/course-data
```

压缩成：

```text
/var/backups/course-data.tar.gz
```

可以创建：

```bash
sudo tee /usr/local/sbin/course-backup > /dev/null <<'EOF'
#!/bin/bash
set -euo pipefail

tar -czf /var/backups/course-data.tar.gz -C /srv/course-data .
EOF
```

脚本核心命令是：

```bash
tar -czf /var/backups/course-data.tar.gz -C /srv/course-data .
```

### 参数解释

```text
tar
 │
 ├─ -c   创建 archive（create）
 ├─ -z   使用 gzip 压缩
 ├─ -f   后面指定 archive 文件名
 │
 ├─ /var/backups/course-data.tar.gz
 │      输出文件
 │
 ├─ -C /srv/course-data
 │      先进入这个目录
 │
 └─ .
        归档该目录中的内容
```

这里使用：

```bash
-C /srv/course-data .
```

而不是直接：

```bash
tar -czf ... /srv/course-data
```

是为了让 archive 中保存的是：

```text
./README
./...
```

而不是把完整目录层级包含进去。

这样以后恢复到：

```text
/home/learner/restore-check
```

时可以直接得到：

```text
/home/learner/restore-check/README
```

---

## 3. 设置脚本 ownership 和权限

设置 owner：

```bash
sudo chown root:root /usr/local/sbin/course-backup
```

设置权限：

```bash
sudo chmod 750 /usr/local/sbin/course-backup
```

可以检查：

```bash
ls -l /usr/local/sbin/course-backup
```

结果应类似：

```text
-rwxr-x--- 1 root root ... /usr/local/sbin/course-backup
```

也可以使用：

```bash
stat -c '%U %G %a %n' /usr/local/sbin/course-backup
```

预期：

```text
root root 750 /usr/local/sbin/course-backup
```

---

## 4. 创建 `/etc/cron.d` 定时任务

创建：

```bash
sudo tee /etc/cron.d/course-backup > /dev/null <<'EOF'
0 2 * * * root /usr/local/sbin/course-backup
EOF
```

这里的 Cron 格式是：

```text
分钟 小时 日 月 星期 用户 命令
 │    │   │  │   │    │    │
 0    2   *  *   *   root  /usr/local/sbin/course-backup
```

所以：

```cron
0 2 * * * root /usr/local/sbin/course-backup
```

表示：

> 每天凌晨 **02:00**，以 `root` 用户身份运行 `/usr/local/sbin/course-backup`。

---

## 5. `/etc/cron.d` 和普通 crontab 的重要区别

`/etc/cron.d/*` 中需要写 **用户字段**：

```cron
0 2 * * * root /usr/local/sbin/course-backup
```

其中：

```text
root
```

明确表示命令以谁的身份执行。

而用户通过：

```bash
crontab -e
```

编辑自己的 crontab 时，不写这个用户字段。

例如用户 crontab 中通常是：

```cron
0 2 * * * /path/to/script
```

因此不要把两种格式混淆。

---

## 6. 为什么 Cron 文件使用 `644`

设置：

```bash
sudo chown root:root /etc/cron.d/course-backup
sudo chmod 644 /etc/cron.d/course-backup
```

检查：

```bash
stat -c '%U %G %a %n' /etc/cron.d/course-backup
```

预期：

```text
root root 644 /etc/cron.d/course-backup
```

`644` 表示：

```text
rw-r--r--
```

也就是：

* root：可读、可写
* group：只读
* others：只读

普通用户不能修改 Cron 配置，但管理员和排障人员可以读取它。

### 为什么脚本是 `750`，Cron 文件却是 `644`

两者面临的风险不同：

### 可执行脚本

```text
/usr/local/sbin/course-backup
```

会被 root 执行，因此要限制访问：

```text
750
```

### Cron 配置

```text
/etc/cron.d/course-backup
```

最重要的是不能被普通用户修改，但允许读取有助于检查系统配置，因此：

```text
644
```

核心原则是：

> **所有能控制 root 将执行什么内容的文件，都不能让普通用户拥有写权限。**

---

## 7. 不要等到凌晨 02:00 才测试

配置 Cron 后，不能只等第二天看有没有文件。

需要分别验证两个问题：

1. 备份命令能否成功执行？
2. 生成的 archive 能否真正恢复？

Cron 只负责：

> “什么时候执行命令。”

它不能证明：

> “命令产生的备份是有效的。”

---

## 8. 手动运行备份

直接执行：

```bash
sudo /usr/local/sbin/course-backup
```

然后确认 archive 存在：

```bash
ls -l /var/backups/course-data.tar.gz
```

或者：

```bash
test -f /var/backups/course-data.tar.gz && echo "backup exists"
```

预期：

```text
backup exists
```

---

## 9. 检查 archive 内容

可以先查看压缩包内部：

```bash
tar -tzf /var/backups/course-data.tar.gz
```

参数：

| 参数   | 含义              |
| ---- | --------------- |
| `-t` | 列出 archive 内容   |
| `-z` | archive 使用 gzip |
| `-f` | 指定 archive 文件   |

应该能够看到类似：

```text
./
./README
...
```

这证明文件被放进了 archive，但还不能完全证明能够恢复。

---

## 10. 真正执行恢复测试

创建测试目录：

```bash
mkdir -p /home/learner/restore-check
```

将 archive 解压到这里：

```bash
tar -xzf /var/backups/course-data.tar.gz \
    -C /home/learner/restore-check
```

参数中的：

```bash
-x
```

表示：

> extract，即从 archive 中提取文件。

因此：

```bash
tar -xzf backup.tar.gz
```

可以简单记成：

> **解压一个 `.tar.gz` archive。**

---

## 11. 验证恢复后的文件

检查：

```bash
ls -l /home/learner/restore-check/README
```

然后实际读取：

```bash
cat /home/learner/restore-check/README
```

内容应该和原始文件一致：

```bash
cat /srv/course-data/README
```

还可以直接比较：

```bash
cmp /srv/course-data/README \
    /home/learner/restore-check/README
```

如果两个文件完全相同，`cmp` 不输出任何内容，并返回成功状态。

也可以：

```bash
cmp /srv/course-data/README \
    /home/learner/restore-check/README \
    && echo "restore verified"
```

成功时：

```text
restore verified
```

---

## 12. 为什么“恢复测试”比“备份文件存在”更重要

只检查：

```bash
ls /var/backups/course-data.tar.gz
```

只能证明：

> 某个文件被创建了。

它不能证明：

* archive 没有损坏；
* 需要的文件真的在里面；
* 文件路径正确；
* archive 可以解压；
* 数据可以真正恢复。

更可靠的验证链是：

```text
运行备份
    ↓
archive 存在
    ↓
解压到独立目录
    ↓
找到 README
    ↓
读取或比较 README 内容
```

因此：

> **一个不能成功恢复的备份，实际上不能算可靠的备份。**

---

## 13. 完整操作流程

```bash
# 1. 创建备份脚本
sudo tee /usr/local/sbin/course-backup > /dev/null <<'EOF'
#!/bin/bash
set -euo pipefail

tar -czf /var/backups/course-data.tar.gz -C /srv/course-data .
EOF

# 2. 设置脚本 owner 和权限
sudo chown root:root /usr/local/sbin/course-backup
sudo chmod 750 /usr/local/sbin/course-backup

# 3. 创建 Cron 配置
sudo tee /etc/cron.d/course-backup > /dev/null <<'EOF'
0 2 * * * root /usr/local/sbin/course-backup
EOF

# 4. 设置 Cron 文件 owner 和权限
sudo chown root:root /etc/cron.d/course-backup
sudo chmod 644 /etc/cron.d/course-backup

# 5. 立即手动执行备份
sudo /usr/local/sbin/course-backup

# 6. 确认 archive 存在
ls -l /var/backups/course-data.tar.gz

# 7. 查看 archive 内容
tar -tzf /var/backups/course-data.tar.gz

# 8. 创建恢复测试目录
mkdir -p /home/learner/restore-check

# 9. 恢复备份
tar -xzf /var/backups/course-data.tar.gz \
    -C /home/learner/restore-check

# 10. 阅读恢复出的 README
cat /home/learner/restore-check/README

# 11. 验证恢复文件与源文件一致
cmp /srv/course-data/README \
    /home/learner/restore-check/README \
    && echo "restore verified"
```

---

## 14. 最终检查

### 检查备份脚本

```bash
stat -c '%U %G %a %n' /usr/local/sbin/course-backup
```

应该是：

```text
root root 750 /usr/local/sbin/course-backup
```

### 检查 Cron 文件

```bash
stat -c '%U %G %a %n' /etc/cron.d/course-backup
```

应该是：

```text
root root 644 /etc/cron.d/course-backup
```

### 检查备份

```bash
test -f /var/backups/course-data.tar.gz && echo "OK"
```

### 检查恢复结果

```bash
test -f /home/learner/restore-check/README && echo "OK"
```

### 检查内容

```bash
cat /home/learner/restore-check/README
```

---

## 核心知识点

1. **root 自动执行的脚本是高权限入口**，必须防止普通用户修改。
2. `/usr/local/sbin/course-backup` 使用：

   ```text
   root:root 750
   ```
3. `/etc/cron.d/course-backup` 使用：

   ```text
   root:root 644
   ```
4. `/etc/cron.d` 的 Cron 格式包含额外的 **用户字段**：

   ```cron
   0 2 * * * root /usr/local/sbin/course-backup
   ```
5. `tar -czf` 用于创建 gzip 压缩 archive，`tar -xzf` 用于恢复。
6. 安装定时任务后，应立即手动执行脚本，不需要等到定时时间。
7. **备份文件存在不等于备份可靠。真正的验证是成功恢复并读取其中的数据。**
