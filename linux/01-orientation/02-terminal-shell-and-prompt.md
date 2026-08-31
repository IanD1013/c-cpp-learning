# 学习笔记

## Terminal、Shell、Bash 和 Linux 的关系

在 Linux 中输入命令时，可以把整个过程理解为：

```text
你
↓
Terminal（终端）
↓
Shell（例如 Bash）
↓
Linux
```

它们的职责不同：

* **Terminal（终端）**：负责显示文字、接收键盘输入，是你和命令行环境交互的界面。
* **Shell（Shell 程序）**：读取你输入的命令，并负责解释、执行它们。
* **Bash**：一种具体的 Shell。
* **Linux**：底层操作系统，最终负责运行程序、管理文件、进程、硬件等资源。

例如你输入：

```bash
date
```

大致过程是：

```text
Terminal 接收到 "date"
        ↓
Bash 读取并解析命令
        ↓
请求 Linux 运行 date 程序
        ↓
程序输出当前日期和时间
        ↓
Terminal 显示结果
```

---

## Shell Prompt：命令提示符

终端中可能看到：

```text
learner@dometrain-linux:~$
```

这叫做 **Shell Prompt（命令提示符）**。

它的含义通常可以拆成：

```text
learner @ dometrain-linux : ~ $
│          │              │ │
用户名     主机名         目录 提示符
```

### `learner`

当前登录的**用户名**。

### `dometrain-linux`

当前机器的 **hostname（主机名）**。

它相当于这台 Linux 机器的名字。

### `~`

表示当前位于用户的 **Home Directory（主目录）**。

例如用户 `learner` 的主目录通常类似：

```text
/home/learner
```

因此：

```text
~
```

可以理解为当前用户主目录的简写。

### `$`

表示 Shell 已经准备好接收命令。

`$` **不是命令的一部分**。

---

## 一次完整的命令执行过程

假设屏幕上显示：

```text
learner@dometrain-linux:~$ date
Tue Sep 1 10:30:00 NZST 2026
learner@dometrain-linux:~$
```

可以拆成四部分：

### 1. 第一个 Prompt

```text
learner@dometrain-linux:~$
```

表示：

> Bash 已经准备好接收命令。

### 2. 输入的命令

```text
date
```

这是用户真正输入的内容。

### 3. 命令输出

```text
Tue Sep 1 10:30:00 NZST 2026
```

这是 `date` 命令运行产生的输出。

### 4. 新的 Prompt

```text
learner@dometrain-linux:~$
```

表示上一条命令已经结束，Bash 再次等待输入下一条命令。

因此，一个典型的命令行交互循环就是：

```text
Prompt
↓
输入命令
↓
执行命令
↓
显示输出
↓
再次出现 Prompt
```

---

## 文档中的 `$` 和 `#` 不要复制

Linux 教程经常把完整 Prompt 简写成：

```bash
$ date
```

这里：

```text
$    → Prompt
date → 真正要输入的命令
```

所以实际只输入：

```bash
date
```

不要输入：

```bash
$ date
```

同理，有些文档会写：

```bash
# apt update
```

这里的 `#` 通常表示这是一个 **root 用户的 Shell Prompt**，也不是命令本身。

实际命令仍然只是：

```bash
apt update
```

---

## `$` 与 `#` 的常见含义

Linux 文档常用：

| Prompt | 通常表示              |
| ------ | ----------------- |
| `$`    | 普通用户 Shell        |
| `#`    | root / 超级用户 Shell |

例如：

```bash
$ whoami
learner
```

表示普通用户执行命令。

而：

```bash
# whoami
root
```

表示 root 用户执行命令。

> **重要：** 教程中的 `$` 和 `#` 通常只是为了说明当前用户身份，不需要输入。

---

## 核心记忆

```text
Terminal = 输入和显示命令的界面

Shell = 读取并解释命令的程序

Bash = 一种 Shell

Linux = 底层操作系统
```

命令行中的：

```text
learner@dometrain-linux:~$
```

可以理解为：

> 用户 `learner` 正在机器 `dometrain-linux` 上，当前位于自己的主目录，Bash 正等待下一条命令。

看到教程中的：

```bash
$ date
```

真正需要输入的只有：

```bash
date
```
