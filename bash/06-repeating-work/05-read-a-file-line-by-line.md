# Bash 安全逐行读取文件：保留空格、反斜杠与末尾无换行的最后一行

## 目标

假设 `input.txt` 包含多行文本，其中可能出现：

* 行首或行尾空格；
* 路径中的反斜杠，例如 `C:\logs`；
* 最后一行没有以换行符 `\n` 结束。

希望逐行读取文件，并生成 `numbered.txt`：

```text
1:first line
2:  indented line
3:C:\logs
```

要求：

* 每个输入记录对应一个输出行；
* 原始文本内容保持不变；
* 行号从 `1` 开始；
* 每次运行都重新生成 `numbered.txt`；
* 即使最后一行没有换行符，也不能丢失。

---

## 不要使用 `for entry in $(cat input.txt)`

错误写法：

```bash
for entry in $(cat input.txt); do
  ...
done
```

这里有两个重要问题。

### 1. 命令替换不是“逐行读取”

```bash
$(cat input.txt)
```

先把整个文件内容变成一段文本，然后 Bash 再对结果进行 **word splitting（单词分割）**。

默认情况下，空格、Tab 和换行都可能成为分隔符。

例如：

```text
hello world
  indented line
```

可能被拆成：

```text
hello
world
indented
line
```

因此：

* 一行可能变成多个循环项；
* 行首空格会丢失；
* 原始记录边界会丢失。

### 2. 命令替换还会删除末尾换行符

`$(...)` 会移除结果末尾的换行符，因此它本来就不适合需要精确保留文件行结构的场景。

---

## `read` 为什么需要 `IFS=` 和 `-r`

推荐形式：

```bash
IFS= read -r line
```

这三个部分分别解决不同问题。

---

## `read line` 的默认行为

直接写：

```bash
read line
```

并不是“原样读取一整行”。

### 默认 `IFS` 会影响空白字符

Bash 默认的 `IFS`（Internal Field Separator，内部字段分隔符）包含：

* 空格；
* Tab；
* 换行。

因此：

```bash
read line
```

读取：

```text
  hello
```

时，开头的空格可能被去掉。

如果这些空格本身属于数据，就会造成内容改变。

---

## `IFS=`：禁止 `read` 修剪空白

写成：

```bash
IFS= read line
```

表示只针对这一次 `read` 命令，把 `IFS` 临时设为空字符串。

这样：

```text
  hello
```

会完整保留为：

```text
  hello
```

所以：

```bash
IFS=
```

的核心作用是：

> 防止 `read` 按默认 `IFS` 处理行首、行尾的空白字符。

这里的赋值只影响紧跟着的命令：

```bash
IFS= read ...
```

不会永久修改整个脚本的 `IFS`。

---

## `read -r`：不要解释反斜杠

默认情况下，`read` 会把反斜杠 `\` 当作转义字符处理。

例如输入：

```text
C:\logs
```

使用：

```bash
read line
```

可能得到：

```text
C:logs
```

因为反斜杠参与了转义处理。

使用：

```bash
read -r line
```

其中 `-r` 表示：

> 不把反斜杠解释成转义字符。

于是：

```text
C:\logs
```

会保持：

```text
C:\logs
```

因此，读取普通文本文件时，最常见的安全模式是：

```bash
IFS= read -r line
```

---

# 标准逐行读取循环

常见写法：

```bash
while IFS= read -r line; do
  ...
done < input.txt
```

工作流程是：

1. `read` 从 `input.txt` 读取一行；
2. 内容保存到变量 `line`；
3. 执行循环体；
4. 再读取下一行；
5. 到达文件末尾后结束。

输入重定向放在：

```bash
done < input.txt
```

表示整个 `while` 循环都从 `input.txt` 获取标准输入。

这种方式比：

```bash
cat input.txt | while ...
```

更直接，也能避免某些 Bash 中管道导致循环运行在子 Shell 里的问题。

---

# 为什么还需要 `|| [[ -n "$line" ]]`

最稳妥的读取形式是：

```bash
while IFS= read -r line || [[ -n "$line" ]]; do
  ...
done < input.txt
```

它解决的是：

> 文件最后一行没有换行符时，不能把这一行丢掉。

---

## `read` 的退出状态

`read` 不仅会把数据保存到变量里，还会返回退出状态。

通常：

```bash
read -r line
```

成功读取到以换行符结束的一行时，返回成功：

```text
status 0
```

但是假设文件最后是：

```text
first
second
third
```

并且 `third` 后面没有 `\n`。

`read` 到达 EOF 时：

1. 已经读取到了 `third`；
2. 已经把 `third` 保存进 `line`；
3. 但是因为没有遇到换行符，`read` 返回失败状态。

如果循环只有：

```bash
while IFS= read -r line; do
  ...
done
```

Shell 会看到 `read` 返回失败，于是直接结束循环。

结果：

```text
third
```

可能不会进入循环体。

---

## `|| [[ -n "$line" ]]` 的作用

完整条件：

```bash
IFS= read -r line || [[ -n "$line" ]]
```

逻辑是：

```text
read 成功
    ↓
执行循环体

read 失败
    ↓
检查 line 是否仍然有内容
    ↓
有内容 → 仍然执行循环体
没内容 → 结束循环
```

例如最后一行是：

```text
C:\logs
```

但没有结尾换行符：

```bash
read
```

虽然返回失败，但：

```bash
[[ -n "$line" ]]
```

仍然为真，因为：

```bash
line='C:\logs'
```

所以循环体会再执行一次。

真正读到文件完全结束时，`line` 为空：

```bash
[[ -n "$line" ]]
```

为假，循环才结束。

因此推荐记住完整模式：

```bash
while IFS= read -r line || [[ -n "$line" ]]; do
  ...
done < file
```

---

# 完整脚本：`number-lines.sh`

```bash
#!/usr/bin/env bash

count=1

: > numbered.txt

while IFS= read -r line || [[ -n "$line" ]]; do
  printf '%d:%s\n' "$count" "$line" >> numbered.txt
  count=$((count + 1))
done < input.txt

exit 0
```

---

## 逐行解释

### 1. 初始化计数器

```bash
count=1
```

第一条记录的编号从 `1` 开始。

---

### 2. 清空输出文件

```bash
: > numbered.txt
```

`:` 是 Bash 的一个什么都不做但成功返回的命令。

这里真正重要的是：

```bash
> numbered.txt
```

重定向会立即把文件截断为长度 `0`。

如果文件不存在，则创建它。

所以：

```bash
: > numbered.txt
```

等价于：

> 在开始处理之前创建或清空 `numbered.txt`。

这样多次运行脚本时，不会留下上一次的输出。

如果遗漏这一步，而循环内部一直使用：

```bash
>>
```

旧内容就会继续保留。

---

### 3. 安全读取一行

```bash
while IFS= read -r line || [[ -n "$line" ]]; do
```

其中：

| 部分                 | 作用                      |   |           |
| ------------------ | ----------------------- | - | --------- |
| `while`            | 条件为真时重复执行               |   |           |
| `IFS=`             | 保留行首、行尾空白               |   |           |
| `read`             | 读取一条记录                  |   |           |
| `-r`               | 保留反斜杠                   |   |           |
| `line`             | 保存当前行                   |   |           |
| `                  |                         | ` | 左边失败时检查右边 |
| `[[ -n "$line" ]]` | 如果 EOF 时仍然读到了内容，则处理最后一行 |   |           |

---

### 4. 写入编号和原始文本

```bash
printf '%d:%s\n' "$count" "$line" >> numbered.txt
```

例如：

```bash
count=2
line='  hello'
```

输出：

```text
2:  hello
```

注意格式字符串：

```bash
'%d:%s\n'
```

表示：

```text
整数:字符串换行
```

其中没有在冒号后额外加入空格，因此输入内容中的空格会保持原样。

如果输入本身是：

```text
  hello
```

输出就是：

```text
2:  hello
```

两个空格全部来自原始数据。

---

## 为什么推荐 `printf` 而不是 `echo`

写文件时更推荐：

```bash
printf '%d:%s\n' "$count" "$line"
```

而不是：

```bash
echo "$count:$line"
```

原因是不同 Shell 的 `echo` 对某些特殊内容可能有不同解释，例如：

```text
-n
\e
```

`printf` 的行为更明确、更可控。

同时应该把变量写成：

```bash
"$line"
```

而不是：

```bash
$line
```

这样可以避免变量内容再次发生单词分割或文件名展开。

---

### 5. 增加计数器

```bash
count=$((count + 1))
```

每处理完一条记录，计数器加 `1`：

```text
1 → 2 → 3 → 4 → ...
```

---

### 6. 把文件连接到循环输入

```bash
done < input.txt
```

这意味着：

```text
input.txt
    ↓
while / read
```

不需要：

```bash
cat input.txt
```

也不需要先把整个文件存进变量。

文件始终是一行一行被读取。

---

### 7. 成功结束

```bash
exit 0
```

退出状态 `0` 表示脚本成功完成。

---

# 相对路径与绝对路径

脚本中使用：

```bash
input.txt
numbered.txt
```

都是 **相对路径**。

它们相对于：

> 当前工作目录（current working directory）

而不是自动相对于脚本自身所在的目录。

例如目录：

```text
/home/learner/bash-lab/
├── input.txt
└── number-lines.sh
```

执行：

```bash
cd /home/learner/bash-lab
bash number-lines.sh
```

那么：

```bash
input.txt
```

实际指：

```text
/home/learner/bash-lab/input.txt
```

而：

```bash
numbered.txt
```

会生成：

```text
/home/learner/bash-lab/numbered.txt
```

---

## 为什么有时使用相对路径

如果脚本写死：

```bash
/home/learner/bash-lab/input.txt
```

那么脚本只适用于这一个固定位置。

使用：

```bash
input.txt
```

则可以把：

```text
input.txt
number-lines.sh
```

一起复制到另一个目录，然后在那个目录运行：

```bash
bash number-lines.sh
```

脚本仍然可以处理当地的 `input.txt`。

但需要特别记住：

> 裸文件名是相对于“运行脚本时所在的目录”，不是脚本文件所在的目录。

---

# 示例

假设 `input.txt` 的内容是：

```text
alpha
  beta
C:\logs
```

其中第二行开头有两个空格。

执行：

```bash
bash number-lines.sh
```

得到：

```text
1:alpha
2:  beta
3:C:\logs
```

第二行的两个空格和第三行的反斜杠都会被保留。

即使最后的：

```text
C:\logs
```

之后没有换行符，第三行也不会丢失。

---

# 常见错误

## 错误 1：使用命令替换遍历文件

```bash
for line in $(cat input.txt); do
  ...
done
```

问题：

* 按空白字符拆分；
* 一行可能变成多个元素；
* 行首、行尾空格丢失；
* 无法可靠保留记录结构。

应该使用：

```bash
while IFS= read -r line || [[ -n "$line" ]]; do
  ...
done < input.txt
```

---

## 错误 2：只写 `read line`

```bash
while read line; do
  ...
done
```

可能改变：

* 行首/行尾空白；
* 反斜杠。

更安全：

```bash
while IFS= read -r line; do
```

---

## 错误 3：忽略最后一行没有换行符

```bash
while IFS= read -r line; do
  ...
done < input.txt
```

对于普通文本通常可以工作，但严格处理任意文件时，最好写成：

```bash
while IFS= read -r line || [[ -n "$line" ]]; do
  ...
done < input.txt
```

---

## 错误 4：忘记清空输出文件

如果只写：

```bash
printf ... >> numbered.txt
```

多次运行脚本后，新内容会追加在旧内容后面。

应该先执行：

```bash
: > numbered.txt
```

然后在循环中继续使用：

```bash
>>
```

---

# 最佳实践

处理普通文本文件并希望尽量原样保留每一行时，可以把下面的模式作为 Bash 中的标准写法：

```bash
while IFS= read -r line || [[ -n "$line" ]]; do
  # 使用 "$line"
done < input.txt
```

记忆三个关键点：

```text
IFS=   → 不让默认字段分隔规则吃掉空白
-r     → 不解释反斜杠
|| ... → 不丢失没有结尾换行符的最后一行
```

再配合：

```bash
printf '%s\n' "$line"
```

就能可靠地处理大多数需要“逐行、原样读取”的 Bash 文本文件场景。
