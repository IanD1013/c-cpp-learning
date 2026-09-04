# Linux 进程、`ps` 与 `/proc`

## 1. 什么是进程

在 Linux 中，一个正在运行的程序实例称为**进程（process）**。

内核会为每个进程分配一个唯一编号：

* **PID（Process ID）**：进程 ID
* **PPID（Parent Process ID）**：创建该进程的父进程 ID

例如：

```text
PID    PPID
2451   1023
```

表示：

* 当前进程 PID 是 `2451`
* 它由 PID 为 `1023` 的进程创建

PID 是操作系统识别进程的重要方式。查看进程、发送信号、终止进程以及分析日志时，通常都会用到 PID。

---

## 2. 使用 `ps` 查看进程

`ps`（process status）用于查看当前系统中的进程。

一个很实用的形式是：

```bash
ps -e -o pid,ppid,state,comm
```

其中：

```text
-e
```

表示显示系统中的**所有进程**，而不只是当前终端关联的进程。

```text
-o pid,ppid,state,comm
```

表示自定义输出字段，只显示：

| 字段      | 含义        |
| ------- | --------- |
| `pid`   | 当前进程的 PID |
| `ppid`  | 父进程 PID   |
| `state` | 当前进程状态    |
| `comm`  | 可执行程序名称   |

输出可能类似：

```text
    PID    PPID S COMMAND
      1       0 S systemd
    821       1 S sshd
   2451     821 R worker
```

相比直接运行：

```bash
ps
```

使用 `-o` 可以只查看真正关心的信息，更适合脚本处理和故障排查。

---

## 3. 进程状态 `state`

`state` 通常是一个字母，用于表示进程当前处于什么状态。

### `R`：Running / Runnable

```text
R
```

表示进程：

* 正在 CPU 上执行，或者
* 已经准备好执行，正在等待 CPU 调度

因此 `R` 并不一定意味着此刻真的占用了 CPU。

---

### `S`：Sleeping

```text
S
```

表示进程正在等待某个事件，例如：

* 网络数据到达
* 用户输入
* 定时器到期
* 磁盘或其他 I/O
* 其他进程的结果

这是非常正常的状态。

> 一个健康的 Linux 系统中，大部分进程通常处于 `S`，而不是 `R`。

程序通常不会一直计算，而是大量时间都在等待某件事情发生。

---

### `Z`：Zombie

```text
Z
```

表示**僵尸进程（zombie process）**。

它已经执行结束，但父进程还没有读取它的退出状态。

基本过程是：

```text
子进程退出
   ↓
内核保存退出状态
   ↓
父进程调用 wait()/waitpid()
   ↓
退出信息被回收
```

如果父进程迟迟没有执行最后一步，子进程会暂时保持 `Z` 状态。

---

## 4. `ps` 的信息从哪里来

`ps` 并不是凭空知道进程信息。

Linux 内核通过：

```text
/proc
```

提供大量运行时信息。

系统中的每个进程通常都有一个对应目录：

```text
/proc/<pid>/
```

例如 PID 为 `2451`：

```text
/proc/2451/
```

其中包含该进程的各种信息。

### 查看进程状态

```bash
cat /proc/2451/status
```

其中可以看到：

```text
Name:
State:
Pid:
PPid:
```

等信息。

---

### 查看启动命令

```text
/proc/2451/cmdline
```

例如：

```bash
cat /proc/2451/cmdline
```

需要注意，`cmdline` 中的参数实际上使用 **NUL 字符**分隔，因此直接 `cat` 时显示效果可能不像普通文本。

可以使用：

```bash
tr '\0' ' ' < /proc/2451/cmdline
```

将参数之间的 NUL 转换为空格。

---

## 5. 为什么理解 `/proc` 很重要

可以把关系理解成：

```text
Linux 内核
   ↓
/proc
   ↓
ps
   ↓
格式化后的进程信息
```

例如：

```bash
ps -e -o pid,ppid,state,comm
```

本质上是在读取内核暴露的进程信息，然后把它整理成容易阅读的表格。

因此 `ps` 更像是一个**进程信息查看与格式化工具**，而 `/proc` 是更接近底层的数据接口。

---

## 6. 使用 `head` 限制输出

系统中可能存在几百甚至几千个进程：

```bash
ps -e -o pid,ppid,state,comm
```

输出可能远远超过一个屏幕。

可以使用：

```bash
ps -e -o pid,ppid,state,comm | head
```

默认只显示前 10 行。

如果只想看前 5 行：

```bash
ps -e -o pid,ppid,state,comm | head -5
```

也可以写成更明确的形式：

```bash
head -n 5
```

这里：

```text
|
```

是管道（pipe），表示：

```text
ps 的输出
    ↓
成为 head 的输入
    ↓
只保留前几行
```

`head` 只是帮助阅读，不会改变 `ps` 本身如何获取进程信息。

---

## 7. 过滤某个特定进程

如果只想找名为 `course-worker` 的进程，可以先输出所有进程：

```bash
ps -e -o pid,ppid,state,comm
```

再使用 `grep` 过滤：

```bash
ps -e -o pid,ppid,state,comm | grep 'course-worker'
```

管道的数据流是：

```text
ps
 │
 │ 所有进程
 ▼
grep
 │
 │ 只保留匹配 course-worker 的行
 ▼
终端
```

如果后续操作要求**完全保留 `ps` 原本的空格和排版**，使用 `grep` 很合适，因为它直接选择整行，而不是重新拼接各字段。

---

## 8. 输出重定向到文件

Shell 中：

```text
>
```

表示把标准输出写入文件。

例如：

```bash
echo "hello" > output.txt
```

执行后：

```text
output.txt
```

内容为：

```text
hello
```

如果文件已经存在，`>` 会覆盖原内容。

因此：

```bash
command > file
```

可以理解为：

```text
command 的标准输出
        ↓
      file
```

---

## 9. 核心总结

```bash
ps -e -o pid,ppid,state,comm
```

可以理解为：

```text
ps
├── -e
│   └── 查看所有进程
│
└── -o
    └── 自定义字段
        ├── pid   → 进程 ID
        ├── ppid  → 父进程 ID
        ├── state → 进程状态
        └── comm  → 可执行程序名称
```

常见状态：

```text
S → 等待某件事情，正常且非常常见
R → 正在运行或等待 CPU
Z → 已退出，但父进程尚未回收退出状态
```

底层关系：

```text
内核
 ↓
/proc/<pid>/
 ↓
ps 等工具读取并格式化
```

---

# `course-worker` 进程记录任务

## 任务

找到系统中名为：

```text
course-worker
```

的后台进程，并将它在：

```bash
ps -e -o pid,ppid,state,comm
```

输出中的**那一整行原样保存**到：

```text
/home/learner/worker-process
```

要求：

* 文件中只包含 `course-worker` 那一行；
* 不包含表头；
* 不包含其他进程；
* 应直接选择 `ps` 输出的行；
* 不应手动重新输入 PID、PPID 等字段；
* 应保留 `ps` 原本的字段间距。

## 推荐命令

```bash
ps -e -o pid,ppid,state,comm | grep -E '[[:space:]]course-worker$' > /home/learner/worker-process
```

## 命令解释

第一部分：

```bash
ps -e -o pid,ppid,state,comm
```

生成类似：

```text
    PID    PPID S COMMAND
      1       0 S systemd
   2847       1 S course-worker
```

第二部分：

```bash
grep -E '[[:space:]]course-worker$'
```

只选择以 `course-worker` 作为进程名称结束的那一行。

其中：

```text
[[:space:]]
```

表示名称前必须有空白字符。

```text
course-worker
```

表示需要匹配的进程名。

```text
$
```

表示这一名称必须位于行尾。

所以不会把表头或其他无关行保存下来。

最后：

```bash
> /home/learner/worker-process
```

将匹配到的整行直接写入目标文件。

数据流可以表示为：

```text
ps 输出所有进程
        ↓
grep 选择 course-worker 那一整行
        ↓
> 写入 /home/learner/worker-process
```

## 验证结果

执行：

```bash
cat /home/learner/worker-process
```

应该只看到一行，例如：

```text
   2847       1 S course-worker
```

具体 PID、PPID 和状态以机器当前实际运行情况为准，不应该手动假设或重新输入。

## 为什么不建议重新拼接字段

例如使用某些 `awk` 写法：

```bash
awk '{print $1, $2, $3, $4}'
```

虽然字段内容可能正确，但 `awk` 会重新生成字段之间的空格：

```text
2847 1 S course-worker
```

这可能和 `ps` 原始输出：

```text
   2847       1 S course-worker
```

不完全相同。

如果评分要求：

> exactly as `ps` formats it

那么最安全的方法是**直接筛选并保留 `ps` 的原始整行**，而不是提取字段后重新打印。
