# Bash 前台、后台与作业控制

## 1. 前台进程为什么会“占住终端”

在 Bash 中，普通方式启动的命令默认运行在**前台（foreground）**：

```bash
some-command
```

这时：

* 命令拥有当前终端；
* Bash 会等待该命令结束；
* Shell 提示符暂时不会出现。

因此，“提示符没有回来”并不等于 Shell 卡住了，也不一定意味着程序出问题了。

如果程序仍在正常工作，那么 Bash 只是等待前台程序完成。

---

## 2. Bash 的 Job 是什么

Bash 会管理当前 Shell 启动的一些任务，这些任务称为 **job（作业）**。

可以使用：

```bash
jobs
```

查看当前 Shell 正在管理的 job，例如：

```text
[1]+  Running    some-command &
```

常见信息包括：

| 内容        | 含义        |
| --------- | --------- |
| `[1]`     | Job ID    |
| `Running` | 正在运行      |
| `Stopped` | 已暂停，但没有结束 |
| `Done`    | 已完成       |

Job ID 和进程的 PID 不是同一个东西。

例如：

```text
[1]+  Running    course-report-job &
```

这里的 `1` 是 Bash 内部的 Job ID。

---

## 3. `Ctrl-Z`：暂停前台任务

如果一个程序正在前台运行，可以按：

```text
Ctrl-Z
```

Bash 会暂停这个 job，并返回 Shell 提示符。

典型输出：

```text
^Z
[1]+  Stopped    some-command
```

这里最重要的是：

> `Ctrl-Z` 并没有结束程序，只是暂停了程序。

此时进程仍然存在，只是暂时不执行。

可以通过：

```bash
jobs
```

确认：

```text
[1]+  Stopped    some-command
```

---

## 4. `fg`：恢复到前台运行

如果一个 job 被暂停，可以使用：

```bash
fg
```

将它恢复到前台：

```bash
fg
```

也可以明确指定 Job ID：

```bash
fg %1
```

其中：

```text
%1
```

表示 Job ID 为 `1` 的任务。

执行之后：

* 程序继续运行；
* 再次占用终端；
* Shell 提示符暂时消失。

---

## 5. `bg`：恢复到后台运行

如果希望暂停的 job 继续执行，同时自己继续使用终端，可以执行：

```bash
bg
```

或者：

```bash
bg %1
```

此时程序会从 `Stopped` 变成 `Running`，但运行在后台。

例如：

```text
[1]+ course-report-job &
```

随后：

```bash
jobs
```

可能显示：

```text
[1]+  Running    course-report-job &
```

这意味着：

* 程序仍然在运行；
* Shell 提示符已经可用；
* 可以继续输入其他命令。

---

## 6. 直接使用 `&` 启动后台任务

如果一开始就知道程序应该运行在后台，可以直接在命令末尾加：

```bash
&
```

例如：

```bash
course-report-job &
```

这等价于跳过：

```text
前台运行
→ Ctrl-Z
→ bg
```

直接让程序进入后台。

不过，如果练习目标是理解 Bash job control，那么通常会故意先前台启动，再使用 `Ctrl-Z` 和 `bg`。

---

## 7. `Stopped` 和 `Finished` 完全不同

这是 Bash 作业控制中最容易混淆的地方。

### Stopped

```text
Stopped
```

表示：

* 进程仍存在；
* 程序没有完成；
* 当前只是暂停执行；
* 以后可以通过 `fg` 或 `bg` 继续。

### Finished / Done

```text
Done
```

表示：

* 程序已经执行完毕；
* 不能通过 `fg` 或 `bg` 恢复。

因此：

> `Stopped` ≠ `Finished`

看到：

```text
[1]+  Stopped    some-command
```

时，不应该重新启动一份相同任务，否则可能同时存在两个任务副本。

---

## 8. 为什么退出 Shell 时可能出现 stopped jobs 警告

如果 Shell 中仍有暂停的 job，执行：

```bash
exit
```

时 Bash 可能提示：

```text
There are stopped jobs.
```

原因是 Bash 在提醒：

> 还有程序没有结束，只是被暂停了。

这也是为什么应该先检查：

```bash
jobs
```

确认任务的实际状态。

---

## 9. 一个典型的 Job Control 工作流程

假设启动一个耗时程序：

```bash
long-running-command
```

发现它需要很长时间，但希望继续使用终端。

### 第一步：暂停

按：

```text
Ctrl-Z
```

得到：

```text
[1]+  Stopped    long-running-command
```

### 第二步：放到后台继续运行

```bash
bg
```

### 第三步：检查状态

```bash
jobs
```

应该看到：

```text
[1]+  Running    long-running-command &
```

现在：

* 工作没有丢失；
* 程序继续执行；
* 终端也可以继续使用。

---

## 10. 常用命令总结

| 操作                | 命令          |
| ----------------- | ----------- |
| 查看 Shell 管理的 jobs | `jobs`      |
| 暂停当前前台 job        | `Ctrl-Z`    |
| 恢复到前台             | `fg`        |
| 恢复到后台             | `bg`        |
| 指定 Job 1 到前台      | `fg %1`     |
| 指定 Job 1 到后台      | `bg %1`     |
| 启动时直接放后台          | `command &` |

核心状态转换可以记成：

```text
foreground Running
        |
      Ctrl-Z
        v
     Stopped
      /    \
    fg      bg
    |        |
    v        v
foreground  background
 Running     Running
```

---

# Bash 作业控制练习：后台运行报告生成器并记录状态

## 问题

报告生成器位于：

```text
/usr/local/bin/course-report-job
```

要求：

1. 在前台启动它；
2. 确认它运行后暂停；
3. 将暂停的 job 恢复到后台继续运行；
4. 将 `jobs` 的输出保存到：

```text
/home/learner/job-state
```

最终文件必须显示该 job 为：

```text
Running
```

而不能是：

```text
Stopped
```

---

## 正确操作

### 1. 前台启动程序

```bash
/usr/local/bin/course-report-job
```

此时程序运行在前台，所以 Shell 提示符不会立即返回。

---

### 2. 暂停程序

按：

```text
Ctrl-Z
```

Bash 通常会显示类似：

```text
[1]+  Stopped    /usr/local/bin/course-report-job
```

这里说明任务只是暂停，并没有结束。

---

### 3. 将它恢复到后台

执行：

```bash
bg
```

或者明确指定 job：

```bash
bg %1
```

这会让程序继续运行，但不再占用前台终端。

---

### 4. 检查状态

```bash
jobs
```

应该看到类似：

```text
[1]+  Running    /usr/local/bin/course-report-job &
```

关键是：

```text
Running
```

---

### 5. 将状态写入指定文件

```bash
jobs > /home/learner/job-state
```

然后可以检查：

```bash
cat /home/learner/job-state
```

预期内容类似：

```text
[1]+  Running    /usr/local/bin/course-report-job &
```

---

## 完整操作顺序

```bash
/usr/local/bin/course-report-job
```

然后按：

```text
Ctrl-Z
```

再执行：

```bash
bg
jobs > /home/learner/job-state
```

---

## 为什么不能在 `Ctrl-Z` 后直接保存

如果执行：

```text
Ctrl-Z
```

之后立刻运行：

```bash
jobs > /home/learner/job-state
```

保存下来的很可能是：

```text
[1]+  Stopped    /usr/local/bin/course-report-job
```

这不满足要求。

必须先执行：

```bash
bg
```

让状态从：

```text
Stopped
```

变成：

```text
Running
```

然后再保存 `jobs` 输出。

---

## 最终关键点

```text
Ctrl-Z  = 暂停，不是结束
bg      = 在后台继续执行
fg      = 在前台继续执行
jobs    = 查看当前 Shell 管理的任务
```

本题最关键的状态变化是：

```text
Running（前台）
→ Ctrl-Z
→ Stopped
→ bg
→ Running（后台）
```

最终再执行：

```bash
jobs > /home/learner/job-state
```

即可把后台运行状态记录下来。
