# Bash 标准输出、标准错误与重定向

## 1. Bash 命令有两个主要输出通道

Linux/Bash 中，一个命令通常至少有两个独立的输出通道：

| 文件描述符 | 名称   | 英文     | 典型用途       |
| ----- | ---- | ------ | ---------- |
| `1`   | 标准输出 | stdout | 正常执行结果     |
| `2`   | 标准错误 | stderr | 错误、警告、诊断信息 |

例如：

```bash
ls /tmp
```

如果 `/tmp` 存在，目录内容通常写到 **stdout（文件描述符 1）**。

如果路径不存在：

```bash
ls /path/that-does-not-exist
```

则会产生类似：

```text
ls: cannot access '/path/that-does-not-exist': No such file or directory
```

这条错误信息写到的是 **stderr（文件描述符 2）**，而不是 stdout。

---

## 2. 为什么 `>` 不能隐藏错误信息

执行：

```bash
ls /path/that-does-not-exist > out.txt
```

结果通常是：

* `out.txt` 为空；
* 错误信息仍然显示在终端。

原因是：

```bash
> out.txt
```

实际上是：

```bash
1> out.txt
```

也就是：

> 只把文件描述符 `1`（stdout）重定向到文件。

stderr 没有被修改，因此仍然连接到终端。

可以理解为：

```text
                stdout (1) ───────> out.txt
ls
                stderr (2) ───────> terminal
```

所以错误信息仍然能看到。

---

## 3. 单独重定向 stderr：`2>`

如果希望把错误信息保存到文件：

```bash
ls /path/that-does-not-exist 2> errors.txt
```

这里：

```text
2>
```

表示：

> 把文件描述符 2，也就是 stderr，重定向到文件。

此时：

```text
                stdout (1) ───────> terminal
ls
                stderr (2) ───────> errors.txt
```

如果路径不存在，`errors.txt` 中会保存类似：

```text
ls: cannot access '/path/that-does-not-exist': No such file or directory
```

---

## 4. `2>` 中的 `2` 必须紧挨着 `>`

正确：

```bash
ls /bad/path 2> errors.txt
```

错误理解：

```bash
ls /bad/path 2 > errors.txt
```

这两条命令完全不同。

### `2>`

```bash
2>
```

表示：

```text
重定向文件描述符 2
```

### `2 >`

```bash
ls /bad/path 2 > errors.txt
```

Shell 会把 `2` 当成 `ls` 的普通参数。

大致相当于：

```bash
ls /bad/path 2
```

然后把 stdout 重定向到 `errors.txt`。

因此要记住：

```bash
2>errors.txt
```

或：

```bash
2> errors.txt
```

都可以，但 `2` 必须属于重定向操作符，不能写成：

```bash
2 >
```

---

## 5. 同时把 stdout 和 stderr 保存到不同文件

常见形式：

```bash
command > normal.txt 2> errors.txt
```

等价于：

```bash
command 1> normal.txt 2> errors.txt
```

例如：

```bash
ls /tmp > normal.txt 2> errors.txt
```

如果 `/tmp` 存在：

```text
normal.txt  -> 保存目录列表
errors.txt  -> 空文件
```

如果路径不存在：

```bash
ls /path/that-does-not-exist > normal.txt 2> errors.txt
```

则：

```text
normal.txt  -> 空
errors.txt  -> 保存 ls 的错误信息
```

数据流可以表示为：

```text
                  stdout (1) ───────> normal.txt
ls PATH
                  stderr (2) ───────> errors.txt
```

这种方式的优点是：

> 正常结果和错误诊断不会混在一起。

---

# `|| true`：忽略命令失败的退出状态

## 1. Linux 命令不仅产生输出，还会返回退出状态

每个命令执行结束后都会返回一个整数，称为：

```text
exit status
```

通常：

```text
0      = 成功
非 0   = 失败
```

例如：

```bash
ls /tmp
echo $?
```

可能得到：

```text
0
```

而：

```bash
ls /path/that-does-not-exist
echo $?
```

会得到某个非零值，例如：

```text
2
```

这里的 `2` 是 `ls` 的退出状态，与 stderr 的文件描述符 `2` 是两个完全不同的概念。

---

## 2. `||` 的含义

Bash 中：

```bash
command1 || command2
```

意思是：

> 只有 `command1` 失败时，才执行 `command2`。

例如：

```bash
ls /bad/path || echo "ls failed"
```

如果 `ls` 失败，就执行：

```bash
echo "ls failed"
```

---

## 3. `true` 是什么

`true` 是一个非常简单的命令：

```bash
true
```

它：

* 什么都不输出；
* 永远返回成功状态 `0`。

例如：

```bash
true
echo $?
```

得到：

```text
0
```

---

## 4. `|| true` 的作用

因此：

```bash
ls /bad/path || true
```

表示：

1. 执行 `ls`；
2. 如果 `ls` 失败；
3. 执行 `true`；
4. 最终整个命令链返回成功状态 `0`。

例如：

```bash
ls /path/that-does-not-exist || true
echo $?
```

最终：

```text
0
```

需要注意：

> `|| true` 并不会让 `ls` 本身成功，也不会隐藏它的错误信息。

它只是把最终返回状态变成成功。

如果还希望隐藏或保存错误信息，必须使用 stderr 重定向。

---

# 实际脚本：分别保存正常输出和错误输出

创建：

```text
/home/learner/bash-lab/run-check.sh
```

内容：

```bash
#!/usr/bin/env bash

ls "$1" > normal.txt 2> errors.txt || true
```

如果希望无论从什么目录运行脚本，都把输出固定写到 `/home/learner/bash-lab/`，更稳妥的写法是：

```bash
#!/usr/bin/env bash

ls "$1" > /home/learner/bash-lab/normal.txt 2> /home/learner/bash-lab/errors.txt || true
```

---

## `#!/usr/bin/env bash`

第一行：

```bash
#!/usr/bin/env bash
```

称为 **shebang**。

它告诉系统：

> 使用 `bash` 来解释和运行这个脚本。

---

## `$1`：脚本的第一个参数

脚本运行：

```bash
bash run-check.sh /tmp
```

那么：

```bash
$1
```

就是：

```text
/tmp
```

如果运行：

```bash
bash run-check.sh /path/that-does-not-exist
```

那么 `$1` 就是：

```text
/path/that-does-not-exist
```

因此：

```bash
ls "$1"
```

实际上是在执行用户传入的路径。

---

## 为什么应该写 `"$1"` 而不是 `$1`

推荐：

```bash
ls "$1"
```

而不是：

```bash
ls $1
```

因为路径中可能包含空格。

例如：

```text
/home/learner/My Files
```

使用：

```bash
ls "$1"
```

整个字符串会被当成一个参数。

如果不加引号，Shell 可能把它拆成：

```text
/home/learner/My
Files
```

两个参数。

所以处理路径时通常应该：

```bash
"$1"
```

---

# 脚本执行过程

核心命令：

```bash
ls "$1" > normal.txt 2> errors.txt || true
```

可以拆成三个部分理解。

## 第一部分：执行 `ls`

```bash
ls "$1"
```

列出调用者传入的路径。

---

## 第二部分：分别重定向两个输出通道

```bash
> normal.txt
```

等价于：

```bash
1> normal.txt
```

stdout 保存到：

```text
normal.txt
```

然后：

```bash
2> errors.txt
```

把 stderr 保存到：

```text
errors.txt
```

---

## 第三部分：忽略 `ls` 的失败状态

```bash
|| true
```

如果 `ls` 失败，则执行 `true`。

于是脚本最终仍返回：

```text
0
```

---

# 测试一：传入不存在的路径

运行：

```bash
cd /home/learner/bash-lab

bash run-check.sh /path/that-does-not-exist
```

此时 `ls`：

* 没有正常结果；
* 产生错误诊断；
* 返回非零状态。

所以结果应该是：

### `normal.txt`

为空：

```text
```

### `errors.txt`

包含 `ls` 实际产生的错误，例如：

```text
ls: cannot access '/path/that-does-not-exist': No such file or directory
```

### 脚本退出状态

检查：

```bash
echo $?
```

应该得到：

```text
0
```

这是因为：

```bash
|| true
```

最终提供了成功状态。

---

# 测试二：传入存在的路径

例如：

```bash
bash run-check.sh /tmp
```

如果 `/tmp` 可以正常列出：

### `normal.txt`

包含 `/tmp` 的实际目录列表，例如：

```text
some-file
some-directory
```

具体内容取决于系统。

### `errors.txt`

为空：

```text
```

因为 `ls` 没有产生错误。

---

# 为什么不能预先向文件写固定内容

错误做法：

```bash
echo "No such file or directory" > errors.txt
ls "$1"
```

虽然文件里可能出现错误文字，但这不是 `ls` 真正产生的 stderr。

正确做法是：

```bash
ls "$1" > normal.txt 2> errors.txt || true
```

这样 `errors.txt` 保存的才是：

> `ls` 针对实际传入路径产生的真实诊断信息。

同样，成功情况下也不能自己写一个假目录列表：

```bash
echo "file1" > normal.txt
```

因为要求保存的是：

```text
ls 实际产生的 stdout
```

而不是提前准备好的文本。

---

# `>` 会覆盖旧文件

每次运行：

```bash
ls "$1" > normal.txt 2> errors.txt
```

Shell 都会重新打开两个文件并清空原来的内容。

因此：

第一次失败：

```text
normal.txt = 空
errors.txt = 错误信息
```

随后第二次成功：

```text
normal.txt = 新的目录列表
errors.txt = 空
```

旧的错误不会残留。

这正适合这种“每次文件只保存当前运行结果”的需求。

---

# `>` 与 `>>` 的区别

## `>`：覆盖

```bash
command > output.txt
```

每次先清空文件，再写入新内容。

---

## `>>`：追加

```bash
command >> output.txt
```

不会删除旧内容，而是在文件末尾继续添加。

例如：

```bash
echo one > file.txt
echo two >> file.txt
```

最后：

```text
one
two
```

如果题目要求：

> 文件只保存本次运行的结果

应该使用：

```bash
>
```

而不是：

```bash
>>
```

---

# stdout 和 stderr 为什么要设计成两个通道

分离 stdout 和 stderr 的重要意义是：

> 程序的“结果数据”和“诊断信息”可以被独立处理。

例如：

```bash
some-command > data.txt
```

程序的正常数据进入 `data.txt`，但是错误仍显示在终端。

这样即使正在采集大量结果，也不会错过错误信息。

也可以分别保存：

```bash
some-command > data.txt 2> errors.txt
```

甚至选择丢弃错误：

```bash
some-command 2> /dev/null
```

这种设计让 Shell 管道和自动化脚本更加灵活。

---

# 常用重定向速查表

| 写法                 | 含义                 |       |                         |
| ------------------ | ------------------ | ----- | ----------------------- |
| `> file`           | stdout 覆盖写入文件      |       |                         |
| `1> file`          | stdout 覆盖写入文件      |       |                         |
| `2> file`          | stderr 覆盖写入文件      |       |                         |
| `>> file`          | stdout 追加到文件       |       |                         |
| `2>> file`         | stderr 追加到文件       |       |                         |
| `> out 2> err`     | stdout、stderr 分别保存 |       |                         |
| `2> /dev/null`     | 丢弃 stderr          |       |                         |
| `> /dev/null`      | 丢弃 stdout          |       |                         |
| `> /dev/null 2>&1` | 丢弃 stdout 和 stderr |       |                         |
| `command           |                    | true` | 命令失败后执行 `true`，使最终状态为成功 |

---

# 最终脚本

```bash
#!/usr/bin/env bash

ls "$1" > /home/learner/bash-lab/normal.txt 2> /home/learner/bash-lab/errors.txt || true
```

它实现的逻辑是：

```text
                    ┌── stdout (1) ──> normal.txt
用户输入路径 ──> ls ┤
                    └── stderr (2) ──> errors.txt

ls 成功
    └── 脚本返回 0

ls 失败
    └── true 执行
          └── 脚本最终返回 0
```

## 验证命令

失败场景：

```bash
bash /home/learner/bash-lab/run-check.sh /path/that-does-not-exist

cat /home/learner/bash-lab/normal.txt
cat /home/learner/bash-lab/errors.txt
echo $?
```

预期：

* `normal.txt` 为空；
* `errors.txt` 包含 `ls` 的错误；
* 退出状态为 `0`。

成功场景：

```bash
bash /home/learner/bash-lab/run-check.sh /tmp

cat /home/learner/bash-lab/normal.txt
cat /home/learner/bash-lab/errors.txt
echo $?
```

预期：

* `normal.txt` 包含 `/tmp` 的目录列表；
* `errors.txt` 为空；
* 退出状态为 `0`。

## 核心记忆

```bash
> file
```

只处理：

```text
stdout（文件描述符 1）
```

而：

```bash
2> file
```

才处理：

```text
stderr（文件描述符 2）
```

因此，要把正常结果和错误信息完全分开：

```bash
command > normal.txt 2> errors.txt
```

如果还希望即使 `command` 失败，整个命令链仍然返回成功：

```bash
command > normal.txt 2> errors.txt || true
```
