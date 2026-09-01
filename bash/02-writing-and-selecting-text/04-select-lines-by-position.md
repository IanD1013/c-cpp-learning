# Bash 使用 `head` 和 `tail` 提取文件开头与结尾

## 核心概念

在 Bash 中，可以使用：

* `head`：读取文件开头的若干行；
* `tail`：读取文件结尾的若干行。

它们都只负责**读取并输出内容**，不会修改原文件。

基本语法：

```bash
head -n 行数 文件名
tail -n 行数 文件名
```

例如：

```bash
head -n 5 access.log
```

表示输出 `access.log` 的前 5 行。

```bash
tail -n 5 access.log
```

表示输出 `access.log` 的最后 5 行。

---

## `-n` 表示“行数”

`-n` 后面的数字表示需要读取多少行：

```bash
head -n 3 app.log
```

读取 `app.log` 的前 3 行。

```bash
tail -n 2 app.log
```

读取 `app.log` 的最后 2 行。

这里的数字是**数量（count）**，不是行号，因此不需要考虑“从 0 还是从 1 开始”。

---

## 把结果保存到文件

`head` 和 `tail` 默认把结果写到标准输出，也就是终端。

如果需要把结果保存到文件，可以使用输出重定向 `>`：

```bash
head -n 3 app.log > first.txt
```

执行过程可以理解为：

```text
app.log
  ↓
head 读取前 3 行
  ↓
标准输出
  ↓ >
first.txt
```

同理：

```bash
tail -n 2 app.log > last.txt
```

会把最后 2 行写入 `last.txt`。

---

## 完整示例

假设当前目录是：

```text
/home/learner/bash-lab
```

其中 `app.log` 有 12 行，需要：

* `first.txt` 保存前 3 行；
* `last.txt` 保存最后 2 行；
* `app.log` 保持原样。

可以执行：

```bash
head -n 3 app.log > first.txt
tail -n 2 app.log > last.txt
```

也可以使用完整路径：

```bash
head -n 3 /home/learner/bash-lab/app.log > /home/learner/bash-lab/first.txt
tail -n 2 /home/learner/bash-lab/app.log > /home/learner/bash-lab/last.txt
```

最终效果：

```text
app.log    → 原来的 12 行仍然全部保留
first.txt  → app.log 的前 3 行
last.txt   → app.log 的最后 2 行
```

---

## 为什么 `app.log` 不会被修改？

下面的命令：

```bash
head -n 3 app.log > first.txt
```

包含两个不同的文件角色：

```text
app.log     输入文件，只读取
first.txt   输出文件，被 > 写入
```

`head` 只是打开并读取 `app.log`，并不会向其中写入数据。

真正被 `>` 操作的是右边的 `first.txt`。

因此：

```bash
head -n 3 app.log > first.txt
```

不会截断或改变 `app.log`。

---

## 注意：`>` 会覆盖目标文件

如果 `first.txt` 已经存在：

```bash
head -n 3 app.log > first.txt
```

Bash 会先清空 `first.txt`，然后再把新的 3 行写进去。

因此，不应该把两个结果都重定向到同一个文件：

```bash
head -n 3 app.log > result.txt
tail -n 2 app.log > result.txt
```

第二条命令会覆盖第一条命令产生的内容，最后 `result.txt` 中只剩最后 2 行。

如果需要两个独立结果，应使用两个不同文件：

```bash
head -n 3 app.log > first.txt
tail -n 2 app.log > last.txt
```

---

## 不需要管道

对于这种任务，不需要使用管道 `|`。

例如没有必要写成某种：

```bash
某命令 | head ...
```

因为 `head` 和 `tail` 本身就可以直接读取文件：

```bash
head -n 3 app.log
tail -n 2 app.log
```

只有当数据来自**另一个命令的输出**时，管道才通常有意义，例如：

```bash
some_command | head -n 10
```

这里的 `head` 读取的是 `some_command` 的标准输出，而不是直接读取文件。

---

## `head`、`tail` 与重定向的职责区别

| 部分          | 作用        |
| ----------- | --------- |
| `head`      | 选择开头若干行   |
| `tail`      | 选择结尾若干行   |
| `-n 3`      | 指定读取 3 行  |
| `app.log`   | 输入文件      |
| `>`         | 把标准输出写入文件 |
| `first.txt` | 保存结果的目标文件 |

例如：

```bash
head -n 3 app.log > first.txt
```

可以拆解为：

```text
head            使用 head 命令
-n 3            读取 3 行
app.log         从这个文件读取
>               重定向标准输出
first.txt       把结果保存到这个文件
```

---

## 常见误区

### 误区 1：`head` 会删除文件后面的内容

不会。

```bash
head -n 3 app.log
```

只是读取前 3 行并输出，原文件不会改变。

---

### 误区 2：`tail -n 2` 表示“第 2 行”

不是。

```bash
tail -n 2 app.log
```

表示：

> 从文件末尾取 2 行。

这里的 `2` 是数量，而不是行号。

---

### 误区 3：必须先把整个文件打印出来

不需要。

```bash
head -n 3 app.log > first.txt
```

可以直接读取文件并把选中的内容保存起来，不需要先执行：

```bash
cat app.log
```

---

### 误区 4：必须使用 `cat`

下面这种写法虽然通常能工作：

```bash
cat app.log | head -n 3
```

但这里的 `cat` 是多余的，因为 `head` 可以自己读取文件：

```bash
head -n 3 app.log
```

更直接，也更清晰。

---

## 最佳实践

当需要从文件两端分别提取内容时，直接使用：

```bash
head -n N 文件 > 输出文件
tail -n N 文件 > 输出文件
```

例如：

```bash
head -n 3 app.log > first.txt
tail -n 2 app.log > last.txt
```

记住三个关键点：

1. `head` 从文件顶部读取；
2. `tail` 从文件底部读取；
3. `>` 保存读取结果，但不会修改作为输入的源文件。
