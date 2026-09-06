# 使用 `journalctl` 按 systemd 服务和日志级别筛选日志

## 1. systemd 服务的日志在哪里

在使用 `systemd` 的 Linux 系统中，服务不一定自己写入 `/var/log/...` 之类的日志文件。

服务输出到：

* 标准输出（stdout）
* 标准错误（stderr）

的内容通常会被 **systemd journal（系统日志日志库）** 收集。

查询这些日志的主要命令是：

```bash
journalctl
```

journal 是整个系统共享的、带索引的日志存储，因此查询日志时通常不需要寻找具体日志文件，而是根据日志的结构化字段进行过滤。

---

## 2. 按 systemd Unit 查看日志：`-u`

每条 journal 日志都会记录它来自哪个 systemd unit。

例如查看：

```text
course-app.service
```

的日志：

```bash
journalctl -u course-app.service
```

其中：

```text
-u
```

表示：

> 只显示指定 unit 的日志。

如果不使用 `-u`：

```bash
journalctl
```

会显示整个系统中大量服务和系统组件的日志。

因此排查某个服务时，通常首先执行：

```bash
journalctl -u <service>
```

例如：

```bash
journalctl -u nginx.service
```

---

## 3. 按日志严重程度过滤：`-p`

journal 中的日志不仅包含文本，还包含 **priority（优先级/严重程度）**。

常见等级从严重到轻微依次为：

| 数值 | 名称        | 含义      |
| -: | --------- | ------- |
|  0 | `emerg`   | 系统不可用   |
|  1 | `alert`   | 必须立即处理  |
|  2 | `crit`    | 严重错误    |
|  3 | `err`     | 错误      |
|  4 | `warning` | 警告      |
|  5 | `notice`  | 需要注意的信息 |
|  6 | `info`    | 普通信息    |
|  7 | `debug`   | 调试信息    |

例如：

```bash
journalctl -u course-app.service -p err
```

表示：

> 查看 `course-app.service` 中严重程度为 `err` 或更严重的日志。

也就是说，`-p err` 实际包含：

```text
emerg
alert
crit
err
```

而不是“只显示恰好为 `err` 的日志”。

---

## 4. 为什么不要用 `grep error`

一种比较脆弱的方法是：

```bash
journalctl -u course-app.service | grep error
```

这种方式只是搜索日志文本中是否出现字符串 `error`。

例如下面这条完全正常的信息：

```text
no errors found
```

也会匹配：

```bash
grep error
```

但它实际上并不是错误。

相比之下：

```bash
journalctl -u course-app.service -p err
```

检查的是日志本身记录的 **priority 字段**，因此更加可靠。

核心区别是：

```text
grep        → 根据日志文本匹配
journalctl -p → 根据结构化日志字段筛选
```

---

## 5. `--no-pager`：直接输出结果

默认情况下，`journalctl` 可能把输出交给 `less` 等 pager：

```bash
journalctl -u course-app.service
```

适合人在终端里阅读，但不适合：

* 重定向到文件
* shell 脚本
* 自动化处理

此时可以使用：

```bash
--no-pager
```

例如：

```bash
journalctl -u course-app.service --no-pager
```

表示直接把内容写到标准输出。

---

## 6. `-o cat`：只输出服务真正写出的消息

默认 `journalctl` 会给每条日志加上额外信息，例如：

```text
Sep 07 09:12:31 server course-app[1234]: Database connection failed
```

这里除了真正的日志：

```text
Database connection failed
```

还包含：

* 时间
* 主机名
* 进程或 unit 信息

如果只希望得到服务本身输出的消息，可以使用：

```bash
-o cat
```

例如：

```bash
journalctl -u course-app.service -o cat
```

输出可能变成：

```text
Database connection failed
```

因此当日志需要写入文件，并且要求文件中只保存真正的消息内容时，`-o cat` 非常有用。

---

## 7. 常见组合

### 查看某个服务的全部日志

```bash
journalctl -u course-app.service
```

适合排查问题的第一步，因为可以先看到完整上下文。

### 只查看错误及更严重的日志

```bash
journalctl -u course-app.service -p err
```

### 只查看错误消息本身

```bash
journalctl \
  -u course-app.service \
  -p err \
  --no-pager \
  -o cat
```

各参数的职责分别是：

```text
-u course-app.service   → 选择哪个服务
-p err                  → 选择什么严重程度
--no-pager              → 直接输出，不打开 pager
-o cat                  → 只显示日志消息正文
```

可以把它们理解成两个不同层次：

```text
选择日志：
-u
-p

控制显示格式：
--no-pager
-o cat
```

---

## 8. 将筛选结果写入文件

Shell 的：

```bash
>
```

可以把命令的标准输出写入文件。

例如：

```bash
journalctl -u course-app.service -p err --no-pager -o cat > error.log
```

执行过程可以理解为：

```text
journalctl
    ↓
选择 course-app.service
    ↓
只保留 err 及以上级别
    ↓
只输出消息正文
    ↓
写入 error.log
```

需要注意：

```bash
>
```

会覆盖目标文件原有内容。

如果希望追加：

```bash
>>
```

不过当任务要求“提取当前查询结果到一个文件”时，通常应使用 `>`。

---

## 9. 推荐的日志排查顺序

排查某个 systemd 服务时，一个实用流程是：

```bash
journalctl -u <service>
```

先查看完整日志和上下文。

确认问题之后，再逐步缩小范围：

```bash
journalctl -u <service> -p err
```

最后根据需要调整输出格式：

```bash
journalctl -u <service> -p err --no-pager -o cat
```

这种方式比一开始就进行字符串搜索更容易发现真正的问题。

---

# 问题解答：提取 `course-app.service` 的错误消息

## 问题

`course-app.service` 已经运行并向 systemd journal 写入了多条日志，其中只有一条是真正的错误。

要求：

1. 先查看该服务的完整 journal；
2. 找出 priority 为 `err` 或更严重的日志；
3. 只保存服务真正输出的消息，不保留时间、主机名等前缀；
4. 将结果写入：

```text
/home/learner/course-service-error
```

---

## 操作步骤

### 第一步：查看完整日志

```bash
journalctl -u course-app.service
```

这样可以先看到服务记录的所有日志，并理解后续 priority 过滤掉了哪些内容。

---

### 第二步：提取错误并写入文件

```bash
journalctl -u course-app.service -p err --no-pager -o cat > /home/learner/course-service-error
```

参数含义：

```text
-u course-app.service
```

只查看 `course-app.service`。

```text
-p err
```

只保留 `err` 以及比它更严重的日志。

```text
--no-pager
```

直接输出结果，不启动 pager。

```text
-o cat
```

删除 `journalctl` 添加的时间、主机名、unit 等前缀，只保留服务输出的消息正文。

```text
> /home/learner/course-service-error
```

把最终结果写入指定文件。

---

## 验证结果

可以查看生成的文件：

```bash
cat /home/learner/course-service-error
```

如果该服务确实只有一条 `err` 或更严重的日志，那么文件中应该只包含这一条错误消息。

> 如果当前用户没有读取 system journal 的权限，可以在 `journalctl` 前使用 `sudo`；是否需要 `sudo` 取决于系统的 journal 权限配置。
