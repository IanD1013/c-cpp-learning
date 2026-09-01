# Bash 工作目录、相对路径、`cd` 与 `touch` 文件创建

## 核心目标

在 Bash 中创建文件时，必须先理解一个关键概念：

> **如果命令中使用的是相对路径，那么文件最终创建在哪里，取决于当前 Shell 的工作目录（working directory）。**

例如：

```bash
touch notes.txt
```

这条命令只说明“创建或更新一个名为 `notes.txt` 的文件”，并没有明确写出文件应该位于哪个目录。

因此，Bash 会把 `notes.txt` 解释为：

```text
当前工作目录/notes.txt
```

如果目标是创建：

```text
/home/learner/bash-lab/notes.txt
```

一种最自然的做法是：

```bash
cd /home/learner/bash-lab
touch notes.txt
```

---

## 1. 什么是当前工作目录

每个正在运行的 Shell 都有一个**当前工作目录**（Current Working Directory，简称 CWD）。

可以使用：

```bash
pwd
```

查看当前工作目录。

例如：

```bash
pwd
```

输出：

```text
/home/learner
```

说明当前 Shell 位于：

```text
/home/learner
```

此时运行：

```bash
touch notes.txt
```

实际上创建的是：

```text
/home/learner/notes.txt
```

而不是：

```text
/home/learner/bash-lab/notes.txt
```

因此：

> **执行命令前先确认当前工作目录，是避免把文件创建到错误位置的重要习惯。**

---

## 2. 相对路径如何依赖当前工作目录

Linux 路径大致可以分为两种。

### 2.1 绝对路径

绝对路径从根目录 `/` 开始，例如：

```text
/home/learner/bash-lab/notes.txt
```

特点是：

> 无论当前 Shell 位于哪里，它都表示同一个文件位置。

例如：

```bash
touch /home/learner/bash-lab/notes.txt
```

即使当前目录是：

```text
/tmp
```

文件仍然会创建在：

```text
/home/learner/bash-lab/notes.txt
```

---

### 2.2 相对路径

相对路径不是从 `/` 开始的，例如：

```text
notes.txt
```

或者：

```text
bash-lab/notes.txt
```

Bash 会从**当前工作目录**开始解释这些路径。

例如当前目录为：

```text
/home/learner
```

那么：

```bash
touch notes.txt
```

对应：

```text
/home/learner/notes.txt
```

而：

```bash
touch bash-lab/notes.txt
```

对应：

```text
/home/learner/bash-lab/notes.txt
```

因此可以把：

```bash
touch notes.txt
```

理解为近似：

```text
touch <当前工作目录>/notes.txt
```

---

## 3. 只输入目录路径不会进入目录

初学 Bash 时很容易误以为输入：

```bash
/home/learner/bash-lab
```

就会进入这个目录。

实际上不会。

Bash 读取一行命令时，会把第一个单词当作**要执行的命令**。

因此：

```bash
/home/learner/bash-lab
```

会被理解为：

> “请执行 `/home/learner/bash-lab` 这个东西。”

但它是一个目录，不是可执行程序，因此通常会看到类似错误：

```text
bash: /home/learner/bash-lab: Is a directory
```

更重要的是：

> **出现这个错误后，当前工作目录完全没有变化。**

例如原本：

```bash
pwd
```

输出：

```text
/home/learner
```

然后运行：

```bash
/home/learner/bash-lab
```

即使出现：

```text
Is a directory
```

再次运行：

```bash
pwd
```

仍然是：

```text
/home/learner
```

---

## 4. `cd`：改变当前工作目录

进入目录应该使用：

```bash
cd
```

`cd` 是 **change directory** 的缩写，即“改变目录”。

基本格式：

```bash
cd <目录路径>
```

例如：

```bash
cd /tmp
```

之后：

```bash
pwd
```

会输出：

```text
/tmp
```

从这一刻开始，Shell 中使用的相对路径都会以：

```text
/tmp
```

为基准。

例如：

```bash
touch demo.txt
```

创建的是：

```text
/tmp/demo.txt
```

---

## 5. `cd` 的影响会持续多久

执行：

```bash
cd /tmp
```

之后，当前 Shell 的工作目录就变成：

```text
/tmp
```

后续所有相对路径都会从这里解析。

例如：

```bash
cd /tmp

touch a.txt
mkdir demo
touch demo/b.txt
```

分别对应：

```text
/tmp/a.txt
/tmp/demo/
/tmp/demo/b.txt
```

这种状态会持续到：

1. 再次执行 `cd`
2. 或当前 Shell 会话结束

例如：

```bash
cd /tmp
touch a.txt

cd /home/learner
touch b.txt
```

最终：

```text
/tmp/a.txt
/home/learner/b.txt
```

---

# `touch` 命令

## 6. `touch` 的主要作用

`touch` 通常有两个作用：

1. 文件不存在时，创建一个空文件
2. 文件已经存在时，更新时间戳

例如：

```bash
touch notes.txt
```

如果 `notes.txt` 不存在，就会创建它。

---

## 7. `touch` 创建的文件默认是空的

假设运行：

```bash
touch notes.txt
```

并且文件之前不存在。

创建后的文件：

```text
notes.txt
```

通常大小为：

```text
0 bytes
```

因为：

> `touch` 不会向文件中写入任何文本内容。

可以使用：

```bash
ls -l notes.txt
```

检查文件。

可能看到：

```text
-rw-r--r-- 1 learner learner 0 Sep 1 17:00 notes.txt
```

其中：

```text
0
```

表示文件大小为 0 字节。

也就是说这是一个空文件。

---

## 8. `touch` 和“写入文件”不是一回事

下面两个操作有本质区别。

### 创建空文件

```bash
touch notes.txt
```

结果：

```text
notes.txt
```

存在，但内容为空。

---

### 创建并写入内容

例如：

```bash
echo "hello" > notes.txt
```

结果文件内容变成：

```text
hello
```

因此，如果任务明确要求：

> 文件必须为空

就不要使用：

```bash
echo ...
```

或者：

```bash
printf ...
```

向文件中写入内容。

---

# 文件所有者

## 9. 新文件通常属于创建它的用户

Linux 文件有所有者（owner）和所属组（group）。

如果当前命令是由用户：

```text
learner
```

执行的：

```bash
touch notes.txt
```

那么新文件通常属于：

```text
learner
```

可以检查：

```bash
ls -l notes.txt
```

例如：

```text
-rw-r--r-- 1 learner learner 0 Sep 1 17:00 notes.txt
```

其中：

```text
learner learner
```

分别表示：

```text
所有者    所属组
learner   learner
```

因此：

> 普通用户自己创建的文件，默认通常由这个用户拥有。

---

# 实际操作：在指定目录创建空文件

## 10. 推荐做法

目标是创建：

```text
/home/learner/bash-lab/notes.txt
```

并满足：

* 是普通文件
* 文件所有者为 `learner`
* 文件内容为空

可以执行：

```bash
cd /home/learner/bash-lab
touch notes.txt
```

第一条命令：

```bash
cd /home/learner/bash-lab
```

把当前工作目录切换到：

```text
/home/learner/bash-lab
```

第二条命令：

```bash
touch notes.txt
```

因为 `notes.txt` 是相对路径，所以 Bash 将它解析为：

```text
/home/learner/bash-lab/notes.txt
```

---

## 11. 另一种等价方法：直接使用绝对路径

实际上也可以直接执行：

```bash
touch /home/learner/bash-lab/notes.txt
```

这样就不需要先使用：

```bash
cd
```

因为路径已经明确指定了完整位置。

两种方式的结果相同：

```bash
cd /home/learner/bash-lab
touch notes.txt
```

和：

```bash
touch /home/learner/bash-lab/notes.txt
```

都会创建：

```text
/home/learner/bash-lab/notes.txt
```

不过，如果后面还要在该目录执行很多操作，先：

```bash
cd /home/learner/bash-lab
```

通常更方便。

---

# 如何验证结果

创建文件后，可以检查是否满足要求。

## 12. 检查当前位置

```bash
pwd
```

应看到：

```text
/home/learner/bash-lab
```

---

## 13. 检查文件是否存在

```bash
ls -l notes.txt
```

应该看到类似：

```text
-rw-r--r-- 1 learner learner 0 Sep 1 17:00 notes.txt
```

---

## 14. 判断是否为普通文件

`ls -l` 输出的第一个字符表示文件类型。

例如：

```text
-rw-r--r--
```

最前面的：

```text
-
```

表示：

> 普通文件（regular file）

常见类型包括：

| 首字符 | 类型   |
| --- | ---- |
| `-` | 普通文件 |
| `d` | 目录   |
| `l` | 符号链接 |

因此：

```text
-rw-r--r--
```

说明 `notes.txt` 是普通文件。

---

## 15. 检查所有者

例如：

```text
-rw-r--r-- 1 learner learner 0 Sep 1 17:00 notes.txt
```

其中第一个：

```text
learner
```

是文件所有者。

因此满足：

```text
owner = learner
```

---

## 16. 检查文件是否为空

最简单的方法之一：

```bash
ls -l notes.txt
```

如果文件大小是：

```text
0
```

就表示文件为空。

也可以使用：

```bash
wc -c notes.txt
```

输出：

```text
0 notes.txt
```

其中：

```text
0
```

表示文件包含 0 个字节。

---

# 常见错误

## 错误 1：没有先进入目标目录

假设当前目录是：

```text
/home/learner
```

直接运行：

```bash
touch notes.txt
```

文件会被创建为：

```text
/home/learner/notes.txt
```

而不是目标：

```text
/home/learner/bash-lab/notes.txt
```

### 正确做法

```bash
cd /home/learner/bash-lab
touch notes.txt
```

或者：

```bash
touch /home/learner/bash-lab/notes.txt
```

---

## 错误 2：直接输入目录路径

错误：

```bash
/home/learner/bash-lab
```

这不是“进入目录”，Bash 会尝试执行这个目录。

可能得到：

```text
Is a directory
```

正确：

```bash
cd /home/learner/bash-lab
```

---

## 错误 3：误以为 `touch` 会写入文本

```bash
touch notes.txt
```

不会在里面写：

```text
notes
```

也不会写任何其他内容。

它创建的是：

```text
大小为 0 字节的空文件
```

---

## 错误 4：为了“创建文件”使用 `echo`

例如：

```bash
echo "notes" > notes.txt
```

虽然也会创建文件，但文件已经不为空。

如果要求：

```text
notes.txt 必须为空
```

这种做法就不符合要求。

---

# 路径解析的核心思维模型

可以把 Bash 的路径理解成下面的规则。

## 绝对路径

```bash
touch /home/learner/bash-lab/notes.txt
```

因为以 `/` 开头：

```text
从文件系统根目录开始寻找
```

不依赖当前工作目录。

---

## 相对路径

```bash
touch notes.txt
```

因为没有以 `/` 开头：

```text
从当前工作目录开始寻找
```

如果：

```bash
pwd
```

得到：

```text
/home/learner/bash-lab
```

那么：

```text
notes.txt
```

实际上就是：

```text
/home/learner/bash-lab/notes.txt
```

可以概括为：

```text
相对路径
   ↓
当前工作目录 + 相对路径
   ↓
实际文件路径
```

例如：

```text
当前工作目录：
/home/learner/bash-lab

相对路径：
notes.txt

实际路径：
/home/learner/bash-lab/notes.txt
```

---

# 推荐的 Bash 操作习惯

在文件系统中工作时，可以形成下面的习惯：

```bash
pwd
ls
cd <目标目录>
pwd
ls
执行文件操作
```

例如：

```bash
pwd
cd /home/learner/bash-lab
pwd
touch notes.txt
ls -l notes.txt
```

这样可以显著减少：

* 文件创建在错误位置
* 删除了错误目录中的文件
* 操作了错误文件
* 对相对路径理解错误

等问题。

特别是在执行：

```bash
rm
mv
cp
```

等会修改文件系统的命令之前，确认：

```bash
pwd
```

通常是非常好的习惯。

---

# 最终命令

要在 `/home/learner/bash-lab` 中创建一个属于 `learner` 的空普通文件 `notes.txt`：

```bash
cd /home/learner/bash-lab
touch notes.txt
```

可以进一步验证：

```bash
pwd
ls -l notes.txt
wc -c notes.txt
```

预期满足：

```text
路径：
/home/learner/bash-lab/notes.txt

类型：
普通文件

所有者：
learner

文件大小：
0 字节

文件内容：
空
```

# 知识点总结

* Shell 始终有一个**当前工作目录**。
* `pwd` 用于查看当前工作目录。
* `cd` 用于改变当前工作目录。
* 直接输入一个目录路径不会进入目录。
* 以 `/` 开头的是**绝对路径**。
* 不以 `/` 开头的是**相对路径**。
* 相对路径会基于当前工作目录解析。
* `touch 文件名` 在文件不存在时会创建一个空文件。
* `touch` 不会向文件中写入文本。
* 普通用户创建的新文件通常由该用户拥有。
* `ls -l` 可以检查文件类型、所有者和大小。
* 如果要求文件保持为空，应避免使用 `echo > 文件` 等写入操作。
