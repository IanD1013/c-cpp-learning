# Linux 中的实时监控：`ps`、`top` 与 `watch`

## 1. `ps`：查看某一瞬间的进程状态

`ps`（process status）显示的是命令执行**那一刻**的进程状态，可以把它理解成一张“快照”。

例如：

```bash
ps -e -o pid,ppid,state,comm
```

它只能告诉你：

> “执行 `ps` 的这一瞬间，系统里的进程是什么状态？”

但如果想知道某个任务是否：

* CPU 使用率持续升高；
* 正在逐渐完成；
* 卡住不动；
* 某个值正在不断变化；

只运行一次 `ps` 就不够，因为它不会自动刷新。

---

## 2. `top`：实时观察系统中的进程

`top` 可以理解成会不断刷新的 `ps`。

运行：

```bash
top
```

它会占据整个终端，并且每隔几秒重新显示当前的系统和进程状态。

通常可以看到：

* CPU 使用情况；
* 内存使用情况；
* 系统负载；
* 当前运行的进程；
* 哪些进程正在大量消耗 CPU 或内存。

因此，当一台 Linux 机器突然变慢，而暂时不知道原因时，`top` 往往是最先使用的工具之一。

### 退出 `top`

按：

```text
q
```

即可退出并返回 Shell 提示符。

这里的 `q` 是 `quit` 的意思。

---

## 3. `watch`：重复执行任意命令

`top` 专门用于观察进程，而 `watch` 更通用。

基本语法：

```bash
watch command
```

`watch` 会不断重新执行指定命令，并且只显示最近一次的输出。

例如：

```bash
watch -n 1 cat somefile
```

含义是：

```text
watch
└── 每隔一段时间执行命令

-n 1
└── 间隔 1 秒

cat somefile
└── 每次执行的实际命令
```

因此它实际上在做：

```text
第 1 秒：cat somefile
第 2 秒：cat somefile
第 3 秒：cat somefile
第 4 秒：cat somefile
...
```

只不过不需要自己编写循环。

---

## 4. 为什么 `watch` 很有用

很多 Linux 命令本身只执行一次，例如：

```bash
cat file
```

```bash
ps
```

```bash
ls
```

```bash
df -h
```

通过 `watch`，可以把这些“一次性命令”快速变成实时监控界面。

例如监控磁盘空间：

```bash
watch -n 2 df -h
```

每两秒重新检查一次。

监控目录内容：

```bash
watch -n 1 ls -l /tmp
```

每秒查看一次 `/tmp` 是否发生变化。

监控文件中的进度值：

```bash
watch -n 1 cat progress.txt
```

如果文件内容不断变化，例如：

```text
10
```

然后变成：

```text
35
```

再变成：

```text
80
```

最后：

```text
complete
```

那么 `watch` 就可以非常直观地观察整个过程。

---

## 5. `watch -n 1` 中的 `-n`

`-n` 表示刷新间隔（interval）。

格式：

```bash
watch -n 秒数 command
```

例如：

```bash
watch -n 1 date
```

每 1 秒执行一次：

```bash
date
```

而：

```bash
watch -n 5 date
```

则每 5 秒执行一次。

---

## 6. 如何退出 `watch`

退出 `watch` 使用：

```text
Ctrl-C
```

即按住 `Ctrl`，再按 `C`。

这是因为 `watch` 不像 `top` 那样拥有自己的一套交互式键盘命令。

### `top` 与 `watch` 的退出方式

| 程序      | 退出方式     |
| ------- | -------- |
| `top`   | `q`      |
| `watch` | `Ctrl-C` |

这是一个很容易混淆的地方。

---

## 7. `Ctrl-C` 到底做了什么

`Ctrl-C` 通常会向当前前台进程发送 `SIGINT`（interrupt signal，中断信号）。

因此：

```bash
watch -n 1 cat file
```

运行期间按：

```text
Ctrl-C
```

相当于告诉正在前台运行的 `watch`：

> 停止运行。

随后 Shell 提示符会重新出现。

---

## 8. `ps`、`top`、`watch` 的区别

| 工具      | 作用        | 是否自动刷新 | 典型用途                |
| ------- | --------- | -----: | ------------------- |
| `ps`    | 查看进程快照    |      ❌ | 查看某一瞬间的进程状态         |
| `top`   | 动态查看系统和进程 |      ✅ | 找出 CPU、内存等资源消耗较高的进程 |
| `watch` | 重复运行任意命令  |      ✅ | 观察文件、磁盘、命令输出等变化     |

可以简单记成：

```text
ps
↓
拍一张照片

top
↓
看进程的实时视频

watch
↓
让任意命令变成实时视频
```

---

# 练习解答：监控 `/run/course/counter` 直到完成

## 问题

有一个后台任务正在执行多个步骤，并把当前进度写入：

```text
/run/course/counter
```

任务完成时，该文件的内容会变成：

```text
complete
```

要求：

1. 实时观察该文件；
2. 等待内容变成 `complete`；
3. 将最终值保存到：

```text
/home/learner/monitor-result
```

---

## 第一步：实时观察进度

运行：

```bash
watch -n 1 cat /run/course/counter
```

含义：

```text
watch
└── 重复运行命令

-n 1
└── 每 1 秒执行一次

cat /run/course/counter
└── 显示任务当前的进度
```

屏幕上的值可能会逐渐变化，最终看到：

```text
complete
```

---

## 第二步：看到 `complete` 后退出 `watch`

按：

```text
Ctrl-C
```

返回 Shell。

---

## 第三步：把最终值写入目标文件

运行：

```bash
cat /run/course/counter > /home/learner/monitor-result
```

这里：

```bash
cat /run/course/counter
```

读取：

```text
complete
```

而：

```bash
>
```

表示把标准输出重定向到文件：

```text
/home/learner/monitor-result
```

最终该文件应该包含：

```text
complete
```

---

## 完整操作

```bash
watch -n 1 cat /run/course/counter
```

等待看到：

```text
complete
```

然后按：

```text
Ctrl-C
```

再执行：

```bash
cat /run/course/counter > /home/learner/monitor-result
```

如需验证，可以运行：

```bash
cat /home/learner/monitor-result
```

预期输出：

```text
complete
```

## 核心记忆

```text
ps     = 看一次
top    = 持续看进程
watch  = 持续运行任意命令
```

以及：

```text
top   → q 退出
watch → Ctrl-C 退出
```
