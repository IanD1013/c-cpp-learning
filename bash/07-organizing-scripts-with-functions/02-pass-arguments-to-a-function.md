# Bash 函数参数与位置参数：正确传递脚本参数

## 核心概念：`$1` 的含义取决于当前作用域

Bash 使用**位置参数（positional parameters）**表示传入的参数：

```bash
$1
$2
$3
...
```

在脚本顶层，`$1` 表示脚本收到的第一个参数：

```bash
./format.sh "red blue" green
```

此时脚本中：

```text
$1 = red blue
$2 = green
```

但进入函数后，`$1` 会暂时变成**函数自己的第一个参数**。

例如：

```bash
show() {
  printf '%s\n' "$1"
}

show "hello"
```

函数中的：

```bash
$1
```

表示：

```text
hello
```

而不是脚本启动时的 `$1`。

因此：

> Bash 函数拥有自己的一套位置参数。调用函数时传入什么，函数内部的 `$1`、`$2` 等就是什么。

---

## 函数不会自动继承脚本的位置参数

例如：

```bash
#!/usr/bin/env bash

show() {
  printf '%s\n' "$1"
}

show
```

即使运行脚本时传入：

```bash
./script.sh hello
```

函数调用：

```bash
show
```

没有传递任何参数，所以函数内部：

```bash
$1
```

仍然是空字符串。

如果希望函数处理脚本的第一个参数，必须显式传入：

```bash
show "$1"
```

---

# 完整示例：`format.sh`

目标：

* 定义 `format_item` 函数；
* 每次接收一个值；
* 将值包在方括号中；
* 遍历脚本收到的所有参数；
* 将结果写入 `formatted.txt`；
* 每次运行重新生成文件，而不是追加旧内容。

创建：

```text
bash-lab/format.sh
```

内容：

```bash
#!/usr/bin/env bash

format_item() {
  printf '[%s]\n' "$1"
}

> formatted.txt

for item in "$@"; do
  format_item "$item" >> formatted.txt
done
```

---

## `format_item` 函数

```bash
format_item() {
  printf '[%s]\n' "$1"
}
```

如果调用：

```bash
format_item "red blue"
```

函数内部：

```text
$1 = red blue
```

输出：

```text
[red blue]
```

这里最重要的是：

```bash
"$1"
```

必须加双引号。

---

# 为什么必须写 `"$1"`

正确：

```bash
printf '[%s]\n' "$1"
```

假设：

```text
$1 = red blue
```

双引号会让整个字符串保持为**一个参数**：

```text
red blue
```

所以输出：

```text
[red blue]
```

---

## 不加引号会发生什么

错误写法：

```bash
printf '[%s]\n' $1
```

如果 `$1` 是：

```text
red blue
```

Bash 会进行 **word splitting（单词拆分）**，实际相当于：

```bash
printf '[%s]\n' red blue
```

`printf` 收到了两个值：

```text
red
blue
```

因为格式字符串：

```text
[%s]\n
```

可以重复使用，所以输出会变成：

```text
[red]
[blue]
```

这已经破坏了原始参数。

因此 Bash 中一个非常重要的习惯是：

```bash
"$1"
"$variable"
"$item"
```

只要希望字符串作为完整的一个值传递，通常都应该使用双引号。

---

# `$@`：表示脚本收到的全部参数

脚本中：

```bash
"$@"
```

表示所有位置参数，并且每个参数仍然保持独立。

例如：

```bash
./format.sh "red blue" green
```

脚本收到：

```text
$1 = red blue
$2 = green
```

使用：

```bash
for item in "$@"; do
  ...
done
```

第一次循环：

```text
item = red blue
```

第二次循环：

```text
item = green
```

因此：

```bash
for item in "$@"; do
  format_item "$item"
done
```

等价于依次执行：

```bash
format_item "red blue"
format_item "green"
```

---

## 为什么 `"$@"` 也必须加引号

推荐：

```bash
for item in "$@"; do
```

不要写：

```bash
for item in $@; do
```

对于：

```bash
./format.sh "red blue" green
```

`"$@"` 保留两个参数：

```text
red blue
green
```

而未加引号的 `$@` 可能经过单词拆分，变成三个值：

```text
red
blue
green
```

这会丢失原始参数的边界。

---

# 调用函数时也必须保护参数

即使循环已经正确使用：

```bash
for item in "$@"
```

函数调用仍然应该写：

```bash
format_item "$item"
```

而不是：

```bash
format_item $item
```

假设：

```text
item = red blue
```

正确：

```bash
format_item "$item"
```

函数收到：

```text
$1 = red blue
```

错误：

```bash
format_item $item
```

实际上可能变成：

```bash
format_item red blue
```

函数收到：

```text
$1 = red
$2 = blue
```

而函数只格式化 `$1`，最终只会得到：

```text
[red]
```

所以参数通常需要在**两处都加引号**：

```bash
for item in "$@"; do
  format_item "$item"
done
```

---

# 为什么让函数输出到标准输出

函数写成：

```bash
format_item() {
  printf '[%s]\n' "$1"
}
```

而不是让函数自己负责：

```bash
>> formatted.txt
```

这样函数只负责一件事：

> 把一个值格式化并输出。

至于输出去哪里，由调用者决定。

例如可以输出到终端：

```bash
format_item "hello"
```

也可以写入文件：

```bash
format_item "hello" >> formatted.txt
```

也可以交给其他命令：

```bash
format_item "hello" | another_command
```

这种设计更灵活，也符合 Unix 工具常见的设计思想：

> 函数或程序负责产生标准输出，由调用方决定如何重定向。

---

# `>` 与 `>>`：覆盖和追加

Bash 中：

```bash
>
```

表示**覆盖文件**。

```bash
>>
```

表示**追加到文件末尾**。

因为 `formatted.txt` 每次运行都应该只包含本次脚本参数，所以需要在循环开始前清空：

```bash
> formatted.txt
```

这条命令没有左边的输出内容，它的作用就是：

```text
创建文件（如果不存在）
或
把已有文件清空
```

之后每次循环再追加一行：

```bash
format_item "$item" >> formatted.txt
```

---

# 运行示例

执行：

```bash
cd bash-lab
chmod +x format.sh
./format.sh "red blue" green
```

这里：

```bash
"red blue"
```

必须加引号，否则 shell 会把它当成两个独立参数。

脚本实际收到：

```text
$1 = red blue
$2 = green
```

查看：

```bash
cat formatted.txt
```

结果：

```text
[red blue]
[green]
```

---

# 数据流

整个过程可以理解为：

```text
命令行
    │
    ▼
./format.sh "red blue" green
    │
    ├── $1 = "red blue"
    └── $2 = "green"
    │
    ▼
for item in "$@"
    │
    ├── item = "red blue"
    │      │
    │      ▼
    │   format_item "$item"
    │      │
    │      └── 函数 $1 = "red blue"
    │
    └── item = "green"
           │
           ▼
        format_item "$item"
           │
           └── 函数 $1 = "green"
```

最终得到：

```text
[red blue]
[green]
```

---

# 常见错误

## 错误 1：以为函数中的 `$1` 仍然是脚本的 `$1`

```bash
show() {
  printf '%s\n' "$1"
}

show
```

即使脚本启动时有参数，`show` 没有收到参数，因此函数里的 `$1` 为空。

应该显式传入：

```bash
show "$1"
```

---

## 错误 2：函数内部不引用 `$1`

错误：

```bash
printf '[%s]\n' $1
```

可能把：

```text
red blue
```

拆成：

```text
red
blue
```

正确：

```bash
printf '[%s]\n' "$1"
```

---

## 错误 3：调用函数时不引用变量

错误：

```bash
format_item $item
```

正确：

```bash
format_item "$item"
```

否则包含空格的值可能被拆成多个函数参数。

---

## 错误 4：遍历 `$@` 时不加引号

错误：

```bash
for item in $@; do
```

正确：

```bash
for item in "$@"; do
```

`"$@"` 才能可靠保留原始参数之间的边界。

---

## 错误 5：忘记清空旧文件

如果直接：

```bash
format_item "$item" >> formatted.txt
```

但没有事先：

```bash
> formatted.txt
```

那么每次运行都会把新结果继续追加到旧结果之后。

---

# 推荐实现

```bash
#!/usr/bin/env bash

format_item() {
  printf '[%s]\n' "$1"
}

> formatted.txt

for item in "$@"; do
  format_item "$item" >> formatted.txt
done
```

运行：

```bash
./format.sh "red blue" green
```

结果：

```text
[red blue]
[green]
```

---

# 核心记忆

1. 脚本顶层的 `$1` 是**脚本参数**。
2. 函数内部的 `$1` 是**本次函数调用的第一个参数**。
3. 函数不会自动把脚本参数当成自己的参数，必须显式传入。
4. 使用 `"$@"` 遍历全部脚本参数，并保留每个参数的边界。
5. 使用 `"$item"` 和 `"$1"` 防止包含空格的值被单词拆分。
6. `>` 用于清空或覆盖文件，`>>` 用于追加。
7. 让函数输出到标准输出，把文件重定向放在函数外，通常更灵活。
