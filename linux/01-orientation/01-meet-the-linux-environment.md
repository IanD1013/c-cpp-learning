# 学习笔记

# Linux 入门：操作系统、Debian 与命令行

## 1. Linux 是什么？

**Linux 是一个操作系统（Operating System）**，负责管理计算机的硬件和软件资源，例如：

* CPU
* 内存
* 文件
* 程序
* 网络设备

可以把操作系统理解为：

> **用户 / 程序 与计算机硬件之间的管理层。**

本课程中的浏览器里运行着一台小型虚拟计算机，它使用 Linux 作为操作系统。

---

## 2. Debian 与 Linux 的关系

课程环境具体安装的是 **Debian**。

Debian 是一个 **Linux Distribution（Linux 发行版）**。

Linux 发行版通常包含：

* Linux 内核
* 命令行工具
* 软件包管理系统
* 默认配置
* 一组预装软件

因此：

```text
Linux
└── Debian
```

更准确地说：

> **Linux 是核心基础，而 Debian 是基于 Linux 组织出来的一套完整操作系统环境。**

常见 Linux 发行版还包括：

* Ubuntu
* Fedora
* Debian
* Arch Linux
* Red Hat Enterprise Linux

---

## 3. Linux 与 Unix

Linux 属于 **Unix-like（类 Unix）系统**。

很多 Linux 的概念和命令都受到 Unix 的影响，例如：

* 文件系统结构
* Shell
* 命令行工具
* 用户和权限模型

但刚开始学习 Linux 时，**不需要先学习 Unix 的历史**。

---

## 4. GUI 与 CLI

操作计算机主要有两种方式。

### GUI：图形用户界面

GUI（Graphical User Interface）通过：

* 鼠标点击
* 窗口
* 图标
* 菜单

与计算机交互。

例如文件管理器就是 GUI 工具。

### CLI：命令行界面

CLI（Command-Line Interface）通过输入文本命令控制计算机。

例如：

```bash
echo 'Linux is ready'
```

CLI 是 Linux 中非常重要的操作方式，因为它：

* 操作速度快
* 易于自动化
* 适合远程服务器
* 可以组合大量工具完成复杂任务

---

## 5. Terminal 与 Shell

输入 Linux 命令时，通常会看到一个 **Terminal（终端）** 窗口。

例如：

```text
learner@dometrain-linux:~$
```

这叫做 **Shell Prompt（Shell 提示符）**，表示 Shell 已经准备好接收命令。

需要注意：

> `$` 前面的内容是系统显示的提示符，通常不需要自己输入。

因此如果看到：

```text
learner@dometrain-linux:~$
```

只需要输入：

```bash
echo 'Linux is ready'
```

而不是：

```bash
learner@dometrain-linux:~$ echo 'Linux is ready'
```

---

## 6. 第一条 Linux 命令：`echo`

`echo` 是一个非常简单的命令，用于把指定文本输出到终端。

例如：

```bash
echo 'Linux is ready'
```

输出：

```text
Linux is ready
```

基本结构：

```text
command argument
```

这里：

```text
echo                command
'Linux is ready'    argument
```

可以理解为：

> 执行 `echo` 命令，并把 `'Linux is ready'` 作为参数传给它。

---

## 7. 执行命令的基本流程

在终端中：

1. 等待 Shell Prompt 出现。
2. 输入命令。
3. 按 Enter。
4. Shell 执行命令。
5. 程序输出结果。
6. Shell Prompt 再次出现，等待下一条命令。

例如：

```text
learner@dometrain-linux:~$ echo 'Linux is ready'
Linux is ready
learner@dometrain-linux:~$
```

可以把这个过程理解为：

```text
输入命令
    ↓
Shell 解析命令
    ↓
运行程序
    ↓
显示结果
    ↓
等待下一条命令
```

---

## 8. 当前实验环境是一次性的

课程中的 Linux 机器是一个**可重置的实验环境**。

这意味着：

* 可以放心实验
* 不会修改自己的电脑
* 即使操作错误也没有关系
* Reset 可以恢复到干净状态

Reset 后，本质上相当于：

```text
当前实验机器
      ↓
    删除
      ↓
创建新的干净机器
```

这种 disposable environment（一次性环境）非常适合学习 Linux 命令。

---

## 核心记忆

最重要的几个概念：

```text
Linux
→ 操作系统

Debian
→ Linux 发行版

Terminal
→ 输入和查看命令的界面

Shell
→ 接收并解释命令的程序

Shell Prompt
→ 表示 Shell 正在等待命令

echo
→ 把文本输出到终端
```

第一条可以记住的 Linux 命令：

```bash
echo 'Linux is ready'
```
