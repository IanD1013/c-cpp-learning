# Bash 使用 `IFS= read -r` 原样读取一整行输入

## 1. 问题：普通 `read` 并不会完全保留原始文本

假设标准输入中有这样一行：

```text
  C:\temp\logs  
```

其中：

* 开头有两个空格；
* 结尾有两个空格；
* 中间包含两个反斜杠 `\`。

如果直接使用：

```bash
read line
```

读取后，变量 `line` 中可能变成：

```text
C:templogs
```

原始内容被改变了：

1. 首尾空格消失；
2. 反斜杠 `\` 消失。

这是因为 `read` 默认同时受到 **IFS** 和 **反斜杠转义规则** 的影响。

---

## 2. `read` 默认如何处理反斜杠

默认情况下：

```bash
read line
```

会把反斜杠 `\` 当作转义字符，而不是普通字符。

例如输入：

```text
C:\temp\logs
```

其中：

```text
\t
\l
```

这里并不是说 Bash 会把 `\t` 变成制表符，而是 `read` 会使用反斜杠去转义后面的字符，因此反斜杠本身不会保留下来。

结果类似：

```text
C:templogs
```

### 使用 `-r` 保留反斜杠

正确写法：

```bash
read -r line
```

`-r` 的含义可以记成：

> raw：不要把反斜杠当作转义字符。

因此：

```text
C:\temp\logs
```

会被保存为：

```text
C:\temp\logs
```

### 实践原则

读取普通文本、文件路径、用户输入时，通常应该优先写：

```bash
read -r variable
```

而不是：

```bash
read variable
```

---

## 3. `IFS` 为什么会导致首尾空格消失

Bash 有一个特殊变量：

```bash
IFS
```

全称是：

> Internal Field Separator，内部字段分隔符

默认情况下，它包含：

* 空格；
* Tab；
* 换行。

`read` 会利用 `IFS` 对输入进行字段处理。

例如：

```text
  hello world  
```

使用：

```bash
read line
```

时，首尾属于 `IFS` 的空白字符会被处理掉，因此变量通常得到：

```text
hello world
```

而不是：

```text
  hello world  
```

---

## 4. `IFS=`：禁止 `read` 按 IFS 处理输入

如果希望完整保留首尾空格，可以让这次 `read` 使用空的 `IFS`：

```bash
IFS= read -r line
```

这里的：

```bash
IFS=
```

表示：

> 执行这条 `read` 命令时，让 `IFS` 为空。

因此这一行同时解决两个问题：

```bash
IFS= read -r line
```

效果如下：

| 设置     | 作用                     |
| ------ | ---------------------- |
| `IFS=` | 保留首尾空格，不进行默认的 IFS 字段处理 |
| `read` | 从标准输入读取一行              |
| `-r`   | 保留反斜杠 `\`              |
| `line` | 把读取结果保存到变量 `line`      |

因此对于：

```text
  C:\temp\logs  
```

变量中保存的仍然是：

```text
  C:\temp\logs  
```

包括首尾空格和反斜杠。

---

## 5. 为什么写成 `IFS= read ...` 而不是永久修改 `IFS`

可以这样写：

```bash
IFS=
read -r line
```

但这会修改当前 Shell 后续使用的 `IFS`，可能影响其他命令。

更好的写法是：

```bash
IFS= read -r line
```

这种“变量赋值 + 命令”的形式让这个 `IFS` 设置只用于这次命令。

因此推荐：

```bash
IFS= read -r line
```

而不是为了读取一行文本永久执行：

```bash
IFS=
```

---

## 6. 完整示例：`capture.sh`

创建脚本：

```bash
#!/usr/bin/env bash

IFS= read -r line
printf '%s\n' "$line" > captured.txt
```

这个脚本完成两件事：

1. 从标准输入读取一整行，尽可能原样保存；
2. 把这一行写入 `captured.txt`。

---

## 7. 通过管道给脚本输入数据

例如：

```bash
printf '  C:\\temp\\logs  \n' | ./capture.sh
```

这里需要区分两个不同的问题。

### `printf` 中为什么写 `\\`

在 `printf` 的格式字符串中：

```bash
\\
```

表示输出一个真正的：

```text
\
```

因此：

```bash
printf '  C:\\temp\\logs  \n'
```

实际产生：

```text
  C:\temp\logs  
```

然后管道：

```bash
|
```

把 `printf` 的标准输出连接到 `capture.sh` 的标准输入：

```text
printf
   │
   │ stdout
   ▼
   │ stdin
capture.sh
```

脚本中的：

```bash
IFS= read -r line
```

于是会从这个管道读取第一行。

---

## 8. `read` 从标准输入读取数据

`read` 并不要求用户必须在键盘上输入。

它读取的是：

```text
标准输入 stdin
```

标准输入可以来自很多地方，例如：

### 键盘

```bash
read -r line
```

Shell 会等待用户输入。

### 管道

```bash
printf 'hello\n' | read ...
```

### 文件重定向

```bash
read -r line < input.txt
```

因此：

```bash
printf 'hello\n' | ./capture.sh
```

中，脚本不需要等待键盘输入，因为数据已经从管道到达标准输入。

---

## 9. 为什么输出变量时仍然必须写 `"$line"`

读取时正确：

```bash
IFS= read -r line
```

还不够。

输出变量时也应该写：

```bash
printf '%s\n' "$line"
```

而不是：

```bash
printf '%s\n' $line
```

原因是未加引号的变量展开可能再次发生：

* word splitting（单词分割）；
* pathname expansion（路径名展开 / glob）。

例如：

```bash
line='  hello world  '
```

使用：

```bash
printf '%s\n' $line
```

Shell 会先处理 `$line`，空格可能再次成为参数分隔符。

因此处理普通字符串时，应遵循：

```bash
"$variable"
```

### 推荐写法

```bash
printf '%s\n' "$line"
```

可以理解成：

> 把 `line` 当作一个完整参数交给 `printf`。

---

## 10. 为什么推荐 `printf` 而不是 `echo`

输出变量时推荐：

```bash
printf '%s\n' "$line"
```

而不是：

```bash
echo "$line"
```

因为不同 Shell 或不同 `echo` 实现对以下内容可能存在特殊处理：

```text
-n
-e
反斜杠转义
```

例如变量恰好是：

```text
-n
```

某些 `echo` 可能把它理解成选项，而不是普通文本。

`printf` 的行为更加明确：

```bash
printf '%s\n' "$line"
```

其中：

* `%s`：输出字符串；
* `\n`：输出换行；
* `"$line"`：作为字符串参数传入。

因此在脚本中输出任意文本时，通常优先使用 `printf`。

---

## 11. `>` 为什么让屏幕上没有输出

脚本中：

```bash
printf '%s\n' "$line" > captured.txt
```

`>` 表示：

> 把命令的标准输出重定向到文件。

正常情况下：

```bash
printf '%s\n' "$line"
```

输出到终端：

```text
终端
```

加入：

```bash
> captured.txt
```

之后变成：

```text
printf
   │
   │ stdout
   ▼
captured.txt
```

所以终端上不会显示这一行。

如果 `captured.txt` 已存在，`>` 会覆盖原来的内容。

---

## 12. 使用 `cat` 查看保存结果

可以执行：

```bash
cat captured.txt
```

`cat` 会读取指定文件，并把内容写到标准输出。

例如文件中保存：

```text
  C:\temp\logs  
```

执行：

```bash
cat captured.txt
```

就会把它显示到终端。

不过普通终端中，首尾空格并不容易肉眼确认。

如果需要检查空格是否真的存在，可以使用：

```bash
cat -A captured.txt
```

或者：

```bash
sed -n 'l' captured.txt
```

---

## 13. `cat` 不指定文件时的行为

`cat` 的基本模式是：

```bash
cat 文件名
```

例如：

```bash
cat captured.txt
```

但如果不指定文件：

```bash
cat
```

它就会从标准输入读取内容。

例如：

```bash
printf 'hello\n' | cat
```

数据流为：

```text
printf → pipe → cat → terminal
```

最终显示：

```text
hello
```

因此可以把 `cat` 理解成：

> 从文件或标准输入读取字节，再原样写到标准输出。

---

## 14. 完整数据流

脚本：

```bash
#!/usr/bin/env bash

IFS= read -r line
printf '%s\n' "$line" > captured.txt
```

运行：

```bash
printf '  C:\\temp\\logs  \n' | ./capture.sh
```

整个过程可以表示为：

```text
printf
  │
  │ 产生：
  │ "  C:\temp\logs  \n"
  ▼
pipe
  │
  ▼
capture.sh 的 stdin
  │
  ▼
IFS= read -r line
  │
  │ line="  C:\temp\logs  "
  ▼
printf '%s\n' "$line"
  │
  ▼
captured.txt
```

最终文件保存：

```text
  C:\temp\logs  
```

并且文件末尾还有一个换行符。

---

## 15. `read` 只读取一行

这条命令：

```bash
IFS= read -r line
```

只读取：

> 标准输入中的第一行。

假设输入是：

```text
first
second
third
```

一次：

```bash
IFS= read -r line
```

只会得到：

```text
first
```

如果需要逐行读取整个输入，通常使用：

```bash
while IFS= read -r line; do
    printf '%s\n' "$line"
done
```

这是 Bash 读取文本文件时最常见的模式之一。

---

## 16. 退出状态

`read` 和其他 Bash 命令一样，也会返回退出状态。

通常：

```bash
0
```

表示成功读取到一行。

可以查看上一条命令的退出状态：

```bash
echo $?
```

例如：

```bash
IFS= read -r line
echo $?
```

如果成功读取到输入，通常得到：

```text
0
```

如果到达 EOF 且没有成功读取到完整的一行，则可能返回非零状态。

---

## 17. 最佳实践

当目标是“读取一整行文本，并尽可能保留原始内容”时，推荐直接记住：

```bash
IFS= read -r line
```

输出变量时则记住：

```bash
printf '%s\n' "$line"
```

组合起来：

```bash
#!/usr/bin/env bash

IFS= read -r line
printf '%s\n' "$line"
```

它解决了三个非常常见的问题：

| 问题                | 解决方法      |
| ----------------- | --------- |
| 反斜杠被 `read` 吃掉    | `read -r` |
| 首尾空格被处理           | `IFS=`    |
| 输出时变量再次被 Shell 拆分 | `"$line"` |

核心原则可以概括为：

```bash
IFS= read -r line
printf '%s\n' "$line"
```

对于需要可靠处理普通文本、路径、带空格字符串和反斜杠内容的 Bash 脚本，这是最值得记住的组合之一。
