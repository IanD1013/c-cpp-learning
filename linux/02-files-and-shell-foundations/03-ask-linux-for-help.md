# 学习笔记

## Linux 中如何查命令帮助

在 Linux 中，**不需要记住每个命令的所有参数**。更重要的能力是知道去哪里查。

常用的帮助方式主要有三种：

| 方法                 | 适合场景         | 特点                |
| ------------------ | ------------ | ----------------- |
| `man <command>`    | 查看普通命令的完整文档  | 最详细               |
| `help <builtin>`   | 查看 Bash 内置命令 | 专门针对 Bash builtin |
| `<command> --help` | 快速查看参数和用法    | 简洁、适合快速查询         |

---

## 1. `man`：查看完整手册

例如：

```bash
man ls
```

`man` 是 **manual** 的缩写，会显示 `ls` 的完整参考手册，包括：

* 命令用途
* 参数说明
* 行为细节
* 相关信息

### `man` 中的基本操作

`man` 通常通过一个 **pager（分页阅读器）** 显示长文本。

常用操作：

```text
Space       下一页
↑ / ↓       上下滚动
q           退出
```

因此：

```bash
man ls
```

阅读完成后按：

```text
q
```

即可返回 Shell。

---

## 2. `help`：查看 Bash 内置命令

有些命令并不是独立程序，而是 **Bash 自己实现的内置命令（builtin）**。

例如：

```bash
cd
```

`cd` 必须修改当前 Shell 的工作目录，因此它是 Bash builtin。

查看这类命令的帮助，应使用：

```bash
help cd
```

而不是优先使用：

```bash
man cd
```

### 常见 Bash builtin

例如：

```bash
cd
echo
pwd
export
read
alias
```

可以使用：

```bash
help <command>
```

查看它们的 Bash 文档。

---

## 3. `--help`：快速查询命令参数

很多普通 Linux 命令支持：

```bash
<command> --help
```

例如：

```bash
ls --help
```

它通常会直接在终端输出：

* Usage（基本语法）
* 常见选项
* 简短说明

相比：

```bash
man ls
```

`--help` 更适合快速回答：

> “这个命令的某个 flag 是什么？”

例如，想知道如何让 `ls` 以容易阅读的方式显示文件大小，可以在：

```bash
ls --help
```

中找到：

```text
-h, --human-readable
```

因此常见用法是：

```bash
ls -lh
```

其中：

```text
-l    long listing format，显示详细信息
-h    human-readable，把大小显示成 KB、MB、GB 等
```

例如原本可能显示：

```text
1048576
```

使用 `-h` 后可能显示：

```text
1.0M
```

---

## `man` 和 `--help` 的重要区别

### `man`

```bash
man ls
```

适合：

* 系统性学习一个命令
* 查看完整参数说明
* 阅读详细行为和注意事项

### `--help`

```bash
ls --help
```

适合：

* 快速查 flag
* 查看基本语法
* 将帮助文本保存到文件
* 使用 `grep` 搜索某个关键词

例如：

```bash
ls --help | grep human
```

可以快速寻找与 human-readable 相关的选项。

---

## Pager 与普通输出

直接运行：

```bash
man ls
```

时，`man` 检测到输出目标是终端，因此会使用 pager 方便阅读。

但如果把输出重定向：

```bash
man ls > manual.txt
```

输出目标已经不是终端，因此 `man` 通常不会启动交互式 pager，而是直接把整份格式化手册写入文件。

这意味着：

```bash
man ls > manual.txt
```

可能会保存几百行内容。

如果只是想保存简洁的参数说明，通常：

```bash
ls --help > help.txt
```

更合适。

---

## Shell 重定向：把帮助信息保存到文件

`>` 会把命令的标准输出写入文件：

```bash
command > file
```

例如：

```bash
ls --help > ls-usage.txt
```

含义是：

```text
ls --help
    ↓
产生帮助文本
    ↓
>
    ↓
写入 ls-usage.txt
```

### 注意

`>`：

* 文件不存在 → 创建文件
* 文件已经存在 → **覆盖原内容**

如果想追加内容，则使用：

```bash
>>
```

例如：

```bash
command >> file
```

---

## 用 `cat` 检查保存结果

写入文件后，可以使用：

```bash
cat file
```

查看内容。

例如：

```bash
cat ls-usage.txt
```

这是一个常见的 Shell 工作流程：

```bash
command > file
cat file
```

即：

1. 执行命令
2. 保存输出
3. 检查文件内容

---

## 核心记忆

遇到“不记得某个命令参数”的情况，可以按下面的思路：

```text
普通命令，需要完整文档
        ↓
man command

Bash 内置命令
        ↓
help command

只想快速查参数
        ↓
command --help
```

例如：

```bash
man ls
help cd
ls --help
```

不要试图记住 Linux 的所有 flags。

**真正需要掌握的是如何快速查到它们。**

---

# 问题解答

## 要完成什么？

目标文件：

```text
/home/learner/lab/ls-usage.txt
```

需要保存 `ls --help` 的输出，其中包含以：

```text
Usage: ls
```

开头的 usage 行。

## 执行命令

可以直接运行：

```bash
ls --help > /home/learner/lab/ls-usage.txt
```

或者使用 `~` 表示当前用户的 home directory：

```bash
ls --help > ~/lab/ls-usage.txt
```

然后检查文件：

```bash
cat ~/lab/ls-usage.txt
```

因为：

```text
~ = /home/learner
```

所以：

```text
~/lab/ls-usage.txt
```

等价于：

```text
/home/learner/lab/ls-usage.txt
```

最终关键命令就是：

```bash
ls --help > ~/lab/ls-usage.txt
cat ~/lab/ls-usage.txt
```

这里应该保存**完整的 `ls --help` 输出**，而不是只手动写入 `Usage: ls` 那一行，因为练习明确要求先捕获完整的 `--help` 输出。
