# Bash：不用编辑器直接创建文本文件

## 核心目标

在 Bash 中，可以不打开任何文本编辑器，直接把指定内容写入文件。

最常见的组合是：

```bash
printf '内容\n' > 文件路径
```

本节需要创建：

```text
/home/learner/lab/owner.txt
```

并让它的内容**精确等于**：

```text
learner
```

同时文件末尾必须包含一个换行符。

---

## 1. `printf`：精确输出文本

`printf` 用来按照指定格式输出字符：

```bash
printf 'learner\n'
```

其中：

* `learner`：实际文本
* `\n`：换行符（newline）

输出效果：

```text
learner
```

与 `echo` 相比，`printf` 的行为更加明确，因此当文件内容需要**精确匹配**时通常更适合使用。

---

## 2. `>`：把输出重定向到文件

默认情况下，命令的输出会显示在终端中。

例如：

```bash
printf 'learner\n'
```

会把内容打印到屏幕。

使用 `>` 后：

```bash
printf 'learner\n' > ~/lab/owner.txt
```

Bash 会把输出写入文件，而不是显示在屏幕上。

这个命令完成两件事：

1. 如果 `owner.txt` 不存在，则创建它。
2. 把 `learner\n` 写入文件。

因此本题的完整命令是：

```bash
printf 'learner\n' > ~/lab/owner.txt
```

由于：

```bash
~ = /home/learner
```

所以它等价于：

```bash
printf 'learner\n' > /home/learner/lab/owner.txt
```

---

## 3. 注意：`>` 会覆盖原文件

如果文件已经存在：

```bash
printf 'learner\n' > ~/lab/owner.txt
```

会**清空原来的内容，然后重新写入**。

例如原文件是：

```text
hello
world
```

执行：

```bash
printf 'learner\n' > owner.txt
```

之后只剩：

```text
learner
```

### `>` 和 `>>` 的区别

| 操作符  | 作用      |
| ---- | ------- |
| `>`  | 覆盖文件    |
| `>>` | 追加到文件末尾 |

例如：

```bash
printf 'hello\n' >> file.txt
```

不会删除已有内容，而是在末尾追加一行。

---

## 4. Linux 文本文件通常以换行符结束

下面两个文件看起来非常接近，但实际上不同。

### 有换行符

```text
learner\n
```

### 没有换行符

```text
learner
```

它们的字节内容并不相同。

因此：

```bash
printf 'learner\n'
```

和：

```bash
printf 'learner'
```

生成的是不同文件。

在 Linux/Unix 中，普通文本文件通常约定以换行符结尾，所以这里必须保留：

```text
\n
```

---

## 5. `cat`：检查文件内容

创建文件之后，可以使用：

```bash
cat ~/lab/owner.txt
```

查看内容。

预期结果：

```text
learner
```

然后下一条 Bash prompt 应该出现在新的一行。

如果看到类似：

```text
learnerlearner@dometrain-linux:~$
```

说明输出和 prompt 粘在一起了，通常意味着文件末尾**缺少换行符**。

正确情况应该类似：

```text
learner
learner@dometrain-linux:~$
```

---

## 6. 完整操作流程

```bash
printf 'learner\n' > ~/lab/owner.txt
cat ~/lab/owner.txt
```

最终要求：

```text
路径：
/home/learner/lab/owner.txt

内容：
learner\n
```

即：

* 文件名必须正确
* 内容必须只有 `learner`
* 后面必须有 **1 个换行符**
* 不能包含额外空格或其他字符

---

## 快速记忆

```bash
printf 'text\n' > file.txt
```

可以理解成：

> **生成精确文本 → 把输出写进文件**

而：

```bash
cat file.txt
```

可以理解成：

> **把文件内容读回终端检查**
