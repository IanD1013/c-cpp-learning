# Linux 中让后台任务脱离终端继续运行：`nohup`、重定向与 `$!`

## 1. `&` 只是后台运行，不等于脱离 Shell

在 Bash 中：

```bash
some-command &
```

末尾的 `&` 会让命令进入**后台（background）**运行，因此 Shell 会立即返回提示符。

但这个进程仍然和当前 Shell / 终端会话有关。

如果 SSH 断开、终端关闭或登录 Shell 退出，系统通常会向相关进程发送 **SIGHUP（hangup signal，挂断信号）**。很多程序收到 SIGHUP 后会退出。

因此：

```bash
long-running-command &
```

适合“后台执行”，但不适合需要在退出登录之后继续运行的长时间任务。

---

## 2. `nohup`：忽略终端挂断

`nohup` 是 **no hangup** 的缩写。

基本形式：

```bash
nohup command
```

它会让命令忽略 SIGHUP，从而能够在启动它的终端或 Shell 消失后继续运行。

实际使用时通常同时加 `&`：

```bash
nohup command &
```

两部分的作用不同：

| 写法                | 作用                 |
| ----------------- | ------------------ |
| `command &`       | 后台运行，但仍可能随终端退出     |
| `nohup command`   | 忽略 SIGHUP，但仍在前台运行  |
| `nohup command &` | 后台运行，并且能在终端退出后继续运行 |

因此长时间任务常见写法是：

```bash
nohup long-running-command &
```

---

## 3. 为什么要处理标准输出和错误输出

进程通常有三个标准文件描述符：

| 文件描述符 | 名称          | 默认位置 |
| ----: | ----------- | ---- |
|   `0` | stdin，标准输入  | 终端   |
|   `1` | stdout，标准输出 | 终端   |
|   `2` | stderr，标准错误 | 终端   |

长期运行的后台任务不应该依赖一个即将消失的终端，因此通常需要把 stdout 和 stderr 保存到日志文件。

如果不指定输出位置，`nohup` 通常会尝试把终端输出写入：

```text
nohup.out
```

比起产生大量难以区分的 `nohup.out`，更好的方式是为任务明确指定日志文件。

---

## 4. `> file`：重定向标准输出

例如：

```bash
nohup command > output.log
```

这里：

```bash
> output.log
```

等价于：

```bash
1> output.log
```

意思是：

> 把文件描述符 `1`（stdout）写入 `output.log`。

但是 stderr，也就是文件描述符 `2`，还没有被重定向。

---

## 5. `2>&1`：让 stderr 跟随 stdout

常见写法：

```bash
command > output.log 2>&1
```

可以分两步理解。

第一步：

```bash
> output.log
```

让 stdout 指向：

```text
output.log
```

第二步：

```bash
2>&1
```

意思是：

> 让文件描述符 `2` 指向文件描述符 `1` **当前指向的位置**。

此时 stdout 已经指向 `output.log`，所以 stderr 也会进入同一个文件。

最终效果：

```text
stdout ─┐
        ├──> output.log
stderr ─┘
```

---

## 6. 为什么重定向的顺序很重要

正确：

```bash
command > output.log 2>&1
```

执行顺序：

1. stdout → `output.log`
2. stderr → stdout 当前的位置
3. 因此二者都进入 `output.log`

而：

```bash
command 2>&1 > output.log
```

含义不同。

Shell 会先执行：

```bash
2>&1
```

此时 stdout 仍然指向终端，因此 stderr 被复制到终端。

然后才执行：

```bash
> output.log
```

只把 stdout 改为文件。

最终可能变成：

```text
stdout ───> output.log
stderr ───> terminal
```

因此常用顺序应是：

```bash
> file 2>&1
```

关键点：

> `2>&1` 不是“把 stderr 永久绑定到 stdout”，而是复制 stdout **执行到这一刻的目标位置**。

---

## 7. `$!`：最近一个后台进程的 PID

例如：

```bash
sleep 100 &
```

Bash 会把刚启动的后台进程 PID 保存到特殊变量：

```bash
$!
```

因此：

```bash
sleep 100 &
echo $!
```

可能得到：

```text
24731
```

这个数字就是该后台进程的 PID。

可以把 PID 保存到文件：

```bash
sleep 100 &
echo $! > sleep.pid
```

以后就可以通过 PID 找回进程：

```bash
cat sleep.pid
```

例如检查它：

```bash
ps -p "$(cat sleep.pid)"
```

或者向它发送正常终止信号：

```bash
kill "$(cat sleep.pid)"
```

---

## 8. 为什么 `$!` 应该立即保存

`$!` 只表示：

> 当前 Shell 最近一次启动的后台命令的 PID。

例如：

```bash
command-a &
command-b &
echo $!
```

此时 `$!` 是 `command-b` 的 PID，而不是 `command-a` 的。

因此启动重要后台任务时，最好马上保存：

```bash
nohup command > task.log 2>&1 &
echo $! > task.pid
```

也可以写成一行：

```bash
nohup command > task.log 2>&1 & echo $! > task.pid
```

执行过程是：

1. `nohup` 让任务忽略 SIGHUP；
2. `> task.log` 把 stdout 写入日志；
3. `2>&1` 让 stderr 也进入同一个日志；
4. `&` 让任务进入后台；
5. `$!` 得到刚刚启动的后台任务 PID；
6. `echo ... > task.pid` 保存 PID。

---

## 9. 一个典型的长期后台任务模式

```bash
nohup /path/to/program > /path/to/program.log 2>&1 &
echo $! > /path/to/program.pid
```

可以把它理解成：

```text
程序
 │
 ├── 忽略终端退出 ────── nohup
 │
 ├── stdout ────────────┐
 │                      ├── program.log
 ├── stderr ────────────┘
 │
 └── PID ───────────────── program.pid
```

日志文件用于回答：

> 程序运行得怎么样？

PID 文件用于回答：

> 这个程序现在是哪一个进程？

这是 Unix/Linux 服务和长期任务中非常常见的设计。

---

## 10. `nohup` 与真正的服务管理器

`nohup` 适合：

* 临时运行长时间脚本；
* SSH 断开后仍需要继续运行的命令；
* 一次性导出、数据处理、构建等任务。

但它不是完整的服务管理方案。

对于长期运行的正式服务，通常更适合使用：

* `systemd`
* Docker / Kubernetes
* Supervisor 等进程管理器

因为这些工具还能提供：

* 自动重启；
* 开机启动；
* 日志管理；
* 状态检查；
* 依赖关系管理。

可以简单记成：

```text
&        → 后台运行
nohup    → 终端退出后继续运行
systemd  → 正式管理长期服务
```

---

# 启动 `course-export-worker` 并保存日志和 PID

## 问题

启动：

```text
/usr/local/bin/course-export-worker
```

要求：

* Shell 退出后进程继续运行；
* stdout 和 stderr 都写入：

```text
/home/learner/course-export.log
```

* PID 写入：

```text
/home/learner/course-export.pid
```

## 命令

```bash
nohup /usr/local/bin/course-export-worker > /home/learner/course-export.log 2>&1 & echo $! > /home/learner/course-export.pid
```

## 命令拆解

```bash
nohup /usr/local/bin/course-export-worker
```

让程序忽略 SIGHUP，因此当前 Shell 或终端退出后仍可继续运行。

```bash
> /home/learner/course-export.log
```

把 stdout 写入日志文件。

```bash
2>&1
```

让 stderr 也写入 stdout 当前指向的位置，因此错误和普通输出都会进入：

```text
/home/learner/course-export.log
```

```bash
&
```

让程序在后台运行并立即返回 Shell 提示符。

最后：

```bash
echo $! > /home/learner/course-export.pid
```

`$!` 是刚刚启动的后台进程 PID，将它保存到 PID 文件。

最终形成：

```text
/usr/local/bin/course-export-worker
        │
        ├── stdout ──┐
        │            ├──> /home/learner/course-export.log
        ├── stderr ──┘
        │
        └── PID ────────> /home/learner/course-export.pid
```

之后可以查看保存的 PID：

```bash
cat /home/learner/course-export.pid
```

查看日志：

```bash
cat /home/learner/course-export.log
```

或实时观察日志：

```bash
tail -f /home/learner/course-export.log
```

核心命令是：

```bash
nohup /usr/local/bin/course-export-worker > /home/learner/course-export.log 2>&1 & echo $! > /home/learner/course-export.pid
```
