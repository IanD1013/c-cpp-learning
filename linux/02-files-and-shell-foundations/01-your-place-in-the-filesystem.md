# 学习笔记

## Linux 文件系统与工作目录

Linux 中的文件和目录都位于同一棵**目录树（filesystem tree）**中。

整棵树从根目录开始：

```text
/
```

例如：

```text
/
└── home
    └── learner
        └── lab
            └── inbox
```

其中：

* `/`：根目录（root directory），Linux 文件系统的最顶层。
* `/home/learner`：用户 `learner` 的主目录（home directory）。
* `~`：Bash 中当前用户主目录的简写，因此：

```text
~ = /home/learner
```

---

## 工作目录（Working Directory）

Shell 在执行命令时，总是处于某个目录中，这个目录称为**当前工作目录**。

三个最基础的目录操作命令：

| 命令    | 含义       |
| ----- | -------- |
| `pwd` | 显示当前工作目录 |
| `ls`  | 查看目录中的内容 |
| `cd`  | 切换工作目录   |

例如：

```bash
pwd
```

可能输出：

```text
/home/learner
```

说明当前 Shell 位于 `/home/learner`。

---

## 路径：绝对路径 vs 相对路径

Linux 使用**路径（path）**来定位文件或目录。

### 绝对路径

以 `/` 开头，从文件系统根目录开始定位。

例如：

```text
/home/learner/lab/inbox
```

无论当前工作目录在哪里，它始终表示同一个位置。

### 相对路径

不以 `/` 开头，从**当前工作目录**开始解析。

例如当前目录为：

```text
/home/learner
```

那么：

```text
lab/inbox
```

实际上表示：

```text
/home/learner/lab/inbox
```

因此使用相对路径之前，最好先确认当前位置：

```bash
pwd
```

### `~` 路径

`~` 表示当前用户的 home directory：

```bash
~/lab/inbox
```

对于 `learner` 用户等价于：

```bash
/home/learner/lab/inbox
```

---

## 三种路径可能指向同一个目录

假设当前目录为：

```text
/home/learner
```

那么下面三个命令创建的是同一个目录：

```bash
mkdir -p lab/inbox
```

```bash
mkdir -p ~/lab/inbox
```

```bash
mkdir -p /home/learner/lab/inbox
```

区别只在于路径的写法：

```text
lab/inbox
↑
相对路径

~/lab/inbox
↑
以 home directory 为基准

/home/learner/lab/inbox
↑
绝对路径
```

其中，相对路径最依赖当前工作目录；绝对路径最明确。

---

## 使用 `mkdir` 创建目录

`mkdir` 用于创建目录：

```bash
mkdir lab
```

创建一个名为 `lab` 的目录。

如果要一次创建多层目录，可以使用：

```bash
mkdir -p lab/inbox
```

`-p` 表示自动创建路径中缺失的父目录。

例如，即使 `lab` 还不存在：

```bash
mkdir -p lab/inbox
```

也会创建：

```text
lab/
└── inbox/
```

这通常比依次执行下面两个命令更方便：

```bash
mkdir lab
mkdir lab/inbox
```

---

## 示例：创建实验目录

目标结构：

```text
/home/learner/lab/inbox
```

如果当前位于 home directory：

```bash
pwd
```

输出：

```text
/home/learner
```

可以执行：

```bash
mkdir -p lab/inbox
```

然后检查：

```bash
ls ~/lab
```

应该看到：

```text
inbox
```

---

## 核心记忆

```text
/        → 文件系统根目录
~        → 当前用户的 home directory
pwd      → 我现在在哪里？
ls       → 这里有什么？
cd       → 去另一个目录
mkdir    → 创建目录
mkdir -p → 一次创建多层目录
```

判断路径：

```text
以 / 开头       → 绝对路径
不以 / 开头     → 相对路径
以 ~ 开头       → 从当前用户 home directory 开始
```

最重要的一点：

> **相对路径的含义取决于当前工作目录，而绝对路径与当前所在位置无关。**
