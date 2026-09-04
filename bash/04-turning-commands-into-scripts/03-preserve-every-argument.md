# Bash 参数传递：正确使用 `"$@"` 保留参数边界

## 核心概念

Bash 脚本接收到的命令行参数会保存为**位置参数（positional parameters）**：

```bash
./list-args.sh "red blue" green
```

脚本实际收到两个参数：

```text
$1 = red blue
$2 = green
```

这里 `"red blue"` 之所以是一个参数，是因为**调用脚本时使用了引号**。

> 参数如何被分组，首先由调用脚本的命令行决定；脚本内部是否还能保持这些参数边界，则取决于参数的展开方式。

---

## `$@`：表示所有位置参数

在 Bash 中：

```bash
$@
```

表示脚本收到的所有位置参数。

例如：

```bash
./list-args.sh "red blue" green
```

脚本收到：

```text
$1 = red blue
$2 = green
```

但是，`$@` 是否能保持这两个参数的边界，关键取决于**有没有双引号**。

---

## `"$@"`：正确保留每个参数

推荐写法：

```bash
"$@"
```

`"$@"` 有一个特殊规则：

> 每一个位置参数都会作为一个独立的 shell word 展开，同时参数内部的空格会被保留。

例如：

```bash
printf '%s\n' "$@"
```

如果调用：

```bash
./list-args.sh "red blue" green
```

效果相当于：

```bash
printf '%s\n' "red blue" "green"
```

输出：

```text
red blue
green
```

因此：

```bash
"$@"
```

是 Bash 中**把收到的所有参数原样继续传递给另一个命令**的标准写法。

---

## 为什么不要写裸的 `$@`

如果写：

```bash
printf '%s\n' $@
```

`$@` 没有被双引号保护。

参数展开之后，Bash 还可能进行 **word splitting（单词分割）**。

例如原本：

```text
$1 = red blue
$2 = green
```

裸 `$@` 可能让：

```text
red blue
```

重新被拆成：

```text
red
blue
```

于是：

```bash
printf '%s\n' $@
```

可能输出：

```text
red
blue
green
```

原本两个参数变成了三个单词，参数边界被破坏。

因此处理参数时应优先使用：

```bash
"$@"
```

而不是：

```bash
$@
```

---

## `"$@"` 与 `"$*"` 的区别

这两个写法非常容易混淆。

假设脚本收到：

```text
$1 = red blue
$2 = green
$3 = yellow orange
```

### `"$@"`

```bash
printf '<%s>\n' "$@"
```

相当于：

```bash
printf '<%s>\n' "red blue" "green" "yellow orange"
```

输出：

```text
<red blue>
<green>
<yellow orange>
```

也就是：

> **一个参数仍然是一个参数。**

---

### `"$*"`

```bash
printf '<%s>\n' "$*"
```

会把所有位置参数组合成**一个字符串参数**。

通常得到：

```text
<red blue green yellow orange>
```

也就是：

> `"$*"` 把所有参数合并成一个参数。

因此可以记成：

| 写法     | 效果              |
| ------ | --------------- |
| `"$@"` | 每个参数保持独立        |
| `"$*"` | 所有参数合并成一个字符串    |
| `$@`   | 参数可能再次被单词分割，不安全 |
| `$*`   | 同样可能发生单词分割，不推荐  |

处理任意命令行参数时，通常应该选择：

```bash
"$@"
```

---

# `printf '%s\n' "$@"` 的工作原理

下面这条命令：

```bash
printf '%s\n' "$@"
```

可以理解为：

```text
对于每一个参数：
    使用 %s 输出参数
    然后使用 \n 换行
```

其中：

```bash
%s
```

表示：

> 把对应值按照字符串输出。

而：

```bash
\n
```

表示换行。

---

## `printf` 会重复使用格式字符串

Bash 的 `printf` 在参数数量多于格式字符串中的占位符时，会重复使用格式字符串。

例如：

```bash
printf '%s\n' apple banana orange
```

只有一个 `%s`，但有三个值。

`printf` 会重复使用：

```text
%s\n
```

所以结果为：

```text
apple
banana
orange
```

因此：

```bash
printf '%s\n' "$@"
```

正好可以实现：

> 每个命令行参数输出一行。

---

# 完整的 `list-args.sh`

脚本内容：

```bash
#!/usr/bin/env bash
printf '%s\n' "$@" > args.txt
```

第一行：

```bash
#!/usr/bin/env bash
```

是 **shebang**，表示直接执行脚本时使用 Bash 解释它。

第二行：

```bash
printf '%s\n' "$@" > args.txt
```

可以拆成三部分理解。

### 1. `"$@"`

表示：

```text
脚本收到的所有参数，并保持每个参数的边界
```

### 2. `printf '%s\n'`

表示：

```text
每个参数输出一次，并在后面换行
```

### 3. `> args.txt`

表示把标准输出重定向到：

```text
args.txt
```

而不是显示在终端中。

---

# 相对路径 `args.txt` 到底写在哪里

这里需要特别注意：

```bash
> args.txt
```

中的 `args.txt` 是**相对路径**。

相对路径是相对于：

> **当前工作目录（current working directory）**

而不是自动相对于脚本所在目录。

例如目录：

```text
bash-lab/
└── list-args.sh
```

如果先进入：

```bash
cd bash-lab
```

然后运行：

```bash
./list-args.sh "red blue" green
```

此时当前目录就是 `bash-lab`，所以会生成：

```text
bash-lab/
├── list-args.sh
└── args.txt
```

但如果在父目录运行：

```bash
bash-lab/list-args.sh "red blue" green
```

那么：

```text
args.txt
```

会出现在**当前所在的父目录**，而不是自动出现在 `bash-lab` 中。

因此，“`args.txt` 会和脚本放在一起”只有在**从脚本所在目录运行脚本**时才成立。

---

# 实际运行

进入目录：

```bash
cd bash-lab
```

确保脚本可执行：

```bash
chmod +x list-args.sh
```

然后运行：

```bash
./list-args.sh "red blue" green
```

查看结果：

```bash
cat args.txt
```

应该得到：

```text
red blue
green
```

注意：

```text
red blue
```

虽然包含空格，但仍然完整地占据一行，因为它始终是一个参数。

---

# 三个参数的例子

例如：

```bash
./list-args.sh "red blue" green "yellow orange"
```

脚本收到：

```text
$1 = red blue
$2 = green
$3 = yellow orange
```

由于使用：

```bash
"$@"
```

三个参数的边界都会被保存。

最终 `args.txt`：

```text
red blue
green
yellow orange
```

其中：

* `red blue` 是一个参数；
* `green` 是一个参数；
* `yellow orange` 是一个参数。

参数内部是否有空格，不影响参数本身的完整性。

---

# 常见错误

## 错误 1：调用时没有给包含空格的值加引号

```bash
./list-args.sh red blue green
```

这里 Bash 在脚本执行之前就已经解析成了三个参数：

```text
$1 = red
$2 = blue
$3 = green
```

脚本内部无法知道：

```text
red blue
```

原本是不是想作为一个整体。

因此应该写：

```bash
./list-args.sh "red blue" green
```

---

## 错误 2：脚本内部使用 `$@`

```bash
printf '%s\n' $@
```

即使调用时正确写了：

```bash
./list-args.sh "red blue" green
```

脚本内部仍可能再次把 `red blue` 拆开。

应该写：

```bash
printf '%s\n' "$@"
```

---

## 错误 3：误用 `"$*"`

```bash
printf '%s\n' "$*"
```

这会把所有参数合成一个参数。

如果收到：

```text
red blue
green
```

最终可能只输出一行：

```text
red blue green
```

所以需要逐个保留参数时，应使用：

```bash
"$@"
```

---

# 参数边界的完整流程

理解 Bash 参数最重要的是区分两个阶段。

假设执行：

```bash
./list-args.sh "red blue" green "yellow orange"
```

## 阶段 1：调用脚本

Shell 根据引号确定参数：

```text
参数 1 → red blue
参数 2 → green
参数 3 → yellow orange
```

因此脚本收到三个位置参数。

## 阶段 2：脚本内部再次展开参数

如果使用：

```bash
"$@"
```

边界保持：

```text
"red blue"
"green"
"yellow orange"
```

如果错误使用裸：

```bash
$@
```

参数中的空格可能再次参与 word splitting，原来的边界就可能丢失。

因此可以记住：

```text
调用时使用引号
        ↓
决定脚本最初收到几个参数

脚本内部使用 "$@"
        ↓
继续保持这些参数的边界
```

---

# 最佳实践

编写需要处理任意数量命令行参数的 Bash 脚本时，优先使用：

```bash
"$@"
```

例如：

```bash
some_command "$@"
```

这意味着：

> 把脚本收到的所有参数，按照原来的参数边界，继续传递给 `some_command`。

需要把每个参数分别写入一行时：

```bash
printf '%s\n' "$@"
```

这是简洁、安全且符合 Bash 惯例的写法。
