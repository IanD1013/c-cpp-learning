# Linux 进程终止：先确认目标，再优雅停止

## 1. 核心原则

终止 Linux 进程时，一个可靠的操作习惯是：

> **先确认你将要操作的进程，再发送终止信号。优先使用 `TERM`，只有在进程拒绝退出时才考虑 `KILL`。**

典型流程：

```bash
pgrep -a <进程名>
pkill <进程名>
```

例如：

```bash
pgrep -a course-spinner
pkill course-spinner
```

第一条命令只负责查看，不会修改任何进程；确认目标正确后，第二条命令才真正发送终止信号。

---

## 2. 为什么不要直接使用 `pkill`

`pkill` 根据**进程名称或模式**寻找进程并发送信号：

```bash
pkill pattern
```

危险之处在于：一个模式可能匹配多个进程。

例如：

```bash
pkill worker
```

可能匹配多个名称中符合 `worker` 模式的进程，而不一定只有你真正想停止的那个。

因此，更安全的习惯是先执行：

```bash
pgrep -a worker
```

确认匹配结果后，再决定是否执行：

```bash
pkill worker
```

可以记成：

```text
pgrep = 先看
pkill = 再杀
```

---

## 3. 使用 `pgrep -a` 检查目标

### 基本形式

```bash
pgrep -a pattern
```

其中：

* `pgrep`：查找符合条件的进程
* `-a`：显示 PID 和完整命令行
* `pattern`：用于匹配进程的模式

例如：

```bash
pgrep -a course-spinner
```

可能得到：

```text
4281 course-spinner
```

其中：

* `4281` 是 PID（Process ID，进程 ID）
* `course-spinner` 是对应的命令

如果看到多个结果：

```text
4281 course-spinner
4310 course-spinner
```

说明当前模式匹配了多个进程。此时应该先判断这些进程是否都应该被终止，而不是直接执行 `pkill`。

### 为什么 `-a` 很有用

只看 PID：

```bash
pgrep course-spinner
```

可能只得到：

```text
4281
```

而：

```bash
pgrep -a course-spinner
```

能同时看到进程的命令行，更容易判断自己是否找对了目标。

---

## 4. `TERM`：优先使用的优雅终止信号

Linux 通过 **signal（信号）**通知进程发生某种事件。

正常停止进程时，首先应该发送：

```text
SIGTERM
```

通常简称：

```text
TERM
```

### 使用 `pkill`

```bash
pkill course-spinner
```

如果没有指定信号，`pkill` 默认发送的就是 `SIGTERM`。

因此它等价于：

```bash
pkill -TERM course-spinner
```

也可以写：

```bash
pkill -15 course-spinner
```

因为：

```text
SIGTERM = signal 15
```

推荐使用名字：

```bash
pkill -TERM course-spinner
```

而不是数字，因为含义更加清楚。

---

## 5. 为什么应该先使用 `TERM`

`TERM` 的含义大致是：

> “请正常关闭自己。”

信号会被交给程序处理，因此程序有机会进行清理，例如：

* 保存尚未写入的数据
* 关闭文件
* 关闭数据库连接
* 删除临时文件
* 释放锁
* 完成日志写入
* 正常执行退出逻辑

因此：

```bash
pkill course-spinner
```

并不是强行立即摧毁程序，而是在**请求程序正常退出**。

这也是停止普通进程时应该优先采用的方法。

---

## 6. `KILL`：最后手段

如果进程收到 `TERM` 后仍然拒绝退出，可以使用：

```text
SIGKILL
```

简称：

```text
KILL
```

例如：

```bash
pkill -KILL course-spinner
```

或者：

```bash
pkill -9 course-spinner
```

因为：

```text
SIGKILL = signal 9
```

### `KILL` 与 `TERM` 的根本区别

| 信号        | 常见写法            | 进程能否处理 | 能否执行清理 | 用途   |
| --------- | --------------- | -----: | -----: | ---- |
| `SIGTERM` | `-TERM` / `-15` |     可以 |     可以 | 正常终止 |
| `SIGKILL` | `-KILL` / `-9`  |    不可以 |    不可以 | 强制终止 |

`SIGKILL` 由内核直接结束进程，程序无法捕获、忽略或处理它。

因此：

```bash
kill -9 ...
```

不应该成为默认操作。

程序可能来不及：

* 刷新文件缓冲区
* 完成正在进行的写操作
* 执行清理函数
* 正常释放资源

正确思路是：

```text
TERM → 检查是否退出 → 必要时 KILL
```

而不是：

```text
一上来就 KILL
```

---

## 7. `kill` 与 `pkill` 的区别

### `kill`：根据 PID 操作

```bash
kill 4281
```

默认发送 `SIGTERM`。

等价于：

```bash
kill -TERM 4281
```

适合已经明确知道目标 PID 的情况。

---

### `pkill`：根据名称或模式操作

```bash
pkill course-spinner
```

它会先寻找符合模式的进程，然后发送信号。

优点是方便，不需要手动寻找 PID。

缺点是：

> **模式可能匹配多个进程。**

因此使用 `pkill` 时，尤其值得先运行：

```bash
pgrep -a course-spinner
```

---

## 8. 进程权限与所有者

普通 Linux 用户通常只能向：

> **自己拥有的进程**

发送信号。

如果尝试终止另一个用户拥有的进程，可能得到：

```text
Operation not permitted
```

这通常不是命令语法错误，而是 Linux 内核拒绝了没有权限的操作。

例如：

```bash
kill 1234
```

如果 PID `1234` 属于另一个用户，就可能失败。

---

## 9. 不要因为权限不足就立即使用 `sudo`

下面这种操作需要特别谨慎：

```bash
sudo pkill something
```

普通用户的权限限制原本是一层保护：

```text
普通用户
    ↓
只能影响自己有权限操作的进程
```

而加入 `sudo` 后：

```text
sudo
 ↓
以更高权限执行
 ↓
原来的权限保护可能不再阻止你
```

如果模式本身写错，后果可能比普通 `pkill` 严重得多。

因此在使用高权限终止进程之前，应先确认：

1. 匹配到了哪些进程；
2. 进程属于谁；
3. 是否真的需要管理员权限。

常用检查方式：

```bash
ps -o user,pid,comm -p <PID>
```

例如：

```bash
ps -o user,pid,comm -p 4281
```

可能得到：

```text
USER       PID COMMAND
learner   4281 course-spinner
```

---

## 10. 推荐的安全操作流程

终止一个已知名称的进程时，可以按照下面的流程：

### 第一步：检查匹配对象

```bash
pgrep -a course-spinner
```

确认：

* 是否真的存在目标进程；
* 是否只匹配了预期的进程；
* 命令行是否正确。

### 第二步：发送 `TERM`

```bash
pkill course-spinner
```

或者显式写：

```bash
pkill -TERM course-spinner
```

### 第三步：确认是否已经退出

再次运行：

```bash
pgrep -a course-spinner
```

如果没有输出，通常表示已经没有匹配进程。

### 第四步：只有必要时才使用 `KILL`

如果确认进程长期不响应 `TERM`：

```bash
pkill -KILL course-spinner
```

然后再次确认：

```bash
pgrep -a course-spinner
```

---

## 11. 常见误区

### 误区一：`pkill` 只会杀掉一个进程

不是。

```bash
pkill pattern
```

可能匹配多个进程，并向所有匹配对象发送信号。

因此应该先检查：

```bash
pgrep -a pattern
```

---

### 误区二：停止进程就应该使用 `kill -9`

不是。

通常应该先使用：

```bash
kill PID
```

或者：

```bash
pkill name
```

它们默认发送 `SIGTERM`。

只有进程无法正常退出时，才考虑：

```bash
kill -9 PID
```

---

### 误区三：`kill` 默认就是强制结束

不是。

下面：

```bash
kill 4281
```

默认发送的是：

```text
SIGTERM
```

并不是：

```text
SIGKILL
```

只有显式使用：

```bash
kill -9 4281
```

才是强制终止。

---

### 误区四：`Operation not permitted` 表示命令写错了

不一定。

它通常意味着：

```text
目标存在
+
你的用户没有权限向它发送这个信号
```

应该先检查进程所有者，而不是直接加 `sudo`。

---

# 实操：安全停止 `course-spinner`

## 任务

停止名为：

```text
course-spinner
```

的进程。

要求：

1. 先检查将要收到信号的进程；
2. 然后发送允许进程正常清理并退出的信号。

## 正确操作

首先检查匹配对象：

```bash
pgrep -a course-spinner
```

确认输出确实是目标进程后，发送默认的 `SIGTERM`：

```bash
pkill course-spinner
```

也可以显式写成：

```bash
pkill -TERM course-spinner
```

最后可以再次确认：

```bash
pgrep -a course-spinner
```

如果没有输出，说明已经没有匹配的进程。

完整流程：

```bash
pgrep -a course-spinner
pkill course-spinner
pgrep -a course-spinner
```

这里不应该直接使用：

```bash
pkill -9 course-spinner
```

因为 `SIGKILL` 会强制终止进程，不给它执行正常清理逻辑的机会。对于正常工作的进程，应先使用 `SIGTERM`。
