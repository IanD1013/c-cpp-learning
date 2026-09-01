# Bash 单引号、通配符与 `printf`：安全输出包含空格和 `*` 的文本

## 目标

要让文件 `/home/learner/bash-lab/label.txt` 最终只包含一行：

```text
release * candidate
```

正确命令是：

```bash
printf '%s\n' 'release * candidate' > label.txt
```

这条命令同时解决了三个问题：

* 保留文本中的空格；
* 防止 `*` 被 Bash 展开成文件名；
* 使用 `>` 将结果写入文件。

---

## 1. 为什么不能直接写 `release * candidate`

如果执行：

```bash
printf '%s\n' release * candidate
```

Bash 不会简单地把：

```text
release * candidate
```

原样交给 `printf`。

在命令真正运行之前，Bash 会先处理命令行，其中最重要的两个行为是：

1. **单词分割（word splitting）**
2. **文件名展开（pathname expansion / globbing）**

因此：

```text
release * candidate
```

可能变成多个独立参数。

例如当前目录存在：

```text
release-notes.txt
```

那么 `*` 可能匹配目录中的文件名，最终 `printf` 实际收到的内容可能类似：

```text
release
release-notes.txt
candidate
```

也就是说，`*` 已经不再是普通的星号字符。

> 关键点：这些处理发生在 `printf` 启动之前，是 **Bash** 做的，而不是 `printf` 做的。

---

## 2. 单引号 `'...'` 的作用

Bash 中：

```bash
'release * candidate'
```

表示：

> 把引号内部的内容作为一个整体，并基本按照字面值传递给命令。

因此：

```bash
printf '%s\n' 'release * candidate'
```

传给 `printf` 的第二个参数就是完整的：

```text
release * candidate
```

### 单引号解决了两个问题

#### 保留空格

没有引号时：

```bash
release * candidate
```

会被 Bash 当作多个单词处理。

使用：

```bash
'release * candidate'
```

之后，三个单词和两个空格共同组成**一个参数**。

#### 阻止 `*` 展开

在 Bash 中，未引用的：

```bash
*
```

是通配符，可以匹配当前目录中的文件名。

但：

```bash
'*'
```

或者：

```bash
'release * candidate'
```

中的 `*` 只是普通字符，不会进行文件名匹配。

---

## 3. 引号不会写进文件

命令：

```bash
printf '%s\n' 'release * candidate'
```

中的单引号属于 **Bash 语法**。

Bash 在解析命令时使用这些引号来确定参数边界，然后把引号本身移除。

因此 `printf` 实际收到的是：

```text
release * candidate
```

而不是：

```text
'release * candidate'
```

所以文件中也不会出现单引号。

---

## 4. `printf '%s\n'` 是什么意思

命令：

```bash
printf '%s\n' 'release * candidate'
```

可以拆成两部分：

```text
printf FORMAT ARGUMENT
```

这里：

```bash
FORMAT='%s\n'
ARGUMENT='release * candidate'
```

### `%s`

`%s` 是字符串占位符。

例如：

```bash
printf '%s\n' 'hello'
```

输出：

```text
hello
```

因为 `%s` 被后面的字符串参数替换。

### `\n`

`\n` 表示换行符。

所以：

```bash
printf '%s\n' 'release * candidate'
```

实际上就是：

1. 用 `release * candidate` 替换 `%s`
2. 输出一个换行符

最终得到：

```text
release * candidate
```

并且文件结尾有正常的换行符。

---

## 5. 为什么推荐把文本放进 `%s` 参数，而不是格式字符串

下面两种写法对于简单文本可能都能工作：

```bash
printf 'release * candidate\n'
```

```bash
printf '%s\n' 'release * candidate'
```

但第二种通常更安全、更通用：

```bash
printf '%s\n' "$text"
```

原因是 `printf` 会解释**格式字符串**中的特殊序列，例如：

```text
%s
%d
\n
\t
```

如果待输出的数据本身包含 `%` 或反斜杠，把数据直接作为格式字符串可能产生意外行为。

因此更好的习惯是：

```bash
printf '%s\n' '需要输出的数据'
```

即：

> **格式固定为 `%s`，实际数据作为参数传入。**

例如：

```bash
printf '%s\n' '100% complete'
```

比直接把任意数据当作格式字符串更加稳妥。

---

## 6. 多个参数为什么可能产生多行

例如：

```bash
printf '%s\n' one two three
```

这里：

```text
FORMAT = %s\n
ARGUMENTS = one, two, three
```

Bash 的 `printf` 会重复使用格式字符串，因此输出：

```text
one
two
three
```

这就是为什么：

```bash
printf '%s\n' release * candidate
```

很危险。

如果这些内容被 Bash 解析成多个参数，`printf` 就可能把它们分别输出成多行。

而：

```bash
printf '%s\n' 'release * candidate'
```

只有一个字符串参数，所以只输出一行。

---

## 7. `>` 将标准输出写入文件

完整命令：

```bash
printf '%s\n' 'release * candidate' > label.txt
```

其中：

```bash
> label.txt
```

是 Bash 的输出重定向。

默认情况下，`printf` 的标准输出会显示在终端：

```bash
printf '%s\n' 'release * candidate'
```

输出：

```text
release * candidate
```

加入：

```bash
> label.txt
```

之后，标准输出改为写入文件：

```text
printf
  │
  │ stdout
  ▼
label.txt
```

因此：

```bash
printf '%s\n' 'release * candidate' > label.txt
```

最终让 `label.txt` 包含：

```text
release * candidate
```

### 注意：`>` 会覆盖原文件

如果 `label.txt` 已经存在：

```bash
> label.txt
```

会先清空原内容，再写入新的结果。

如果目标是保留原内容并追加，应使用：

```bash
>>
```

但本例要求文件最终只包含目标文本，因此应使用：

```bash
>
```

---

## 8. 整条命令的解析过程

命令：

```bash
printf '%s\n' 'release * candidate' > label.txt
```

可以理解为：

### 第一步：Bash 解析单引号

```bash
'%s\n'
```

成为一个参数：

```text
%s\n
```

而：

```bash
'release * candidate'
```

成为另一个完整参数：

```text
release * candidate
```

其中的空格不会产生参数分割，`*` 也不会进行文件名展开。

### 第二步：Bash 处理 `>`

Bash 打开：

```text
label.txt
```

并把 `printf` 的标准输出连接到这个文件。

### 第三步：运行 `printf`

`printf` 实际接收到的逻辑结构是：

```text
格式：%s\n
参数：release * candidate
```

它将参数填入 `%s`，然后输出换行。

最终文件内容为：

```text
release * candidate
```

---

## 9. 常见错误

### 错误一：不引用包含 `*` 的文本

```bash
printf '%s\n' release * candidate > label.txt
```

问题：

* 空格把内容拆成多个参数；
* `*` 可能展开成当前目录中的文件名；
* 最终内容可能完全不同。

正确：

```bash
printf '%s\n' 'release * candidate' > label.txt
```

---

### 错误二：把引号当作文件内容的一部分

```bash
printf '%s\n' "'release * candidate'" > label.txt
```

这样实际上会把单引号作为数据输出，文件可能包含：

```text
'release * candidate'
```

正确做法是让引号只承担 Bash 的语法作用：

```bash
printf '%s\n' 'release * candidate' > label.txt
```

---

### 错误三：误以为 `*` 是 `printf` 展开的

实际上：

```bash
*
```

的文件名匹配是 **Bash 在运行 `printf` 之前完成的**。

理解 Bash 命令时要区分：

```text
你输入命令
    ↓
Bash 解析、引用处理、通配符展开、重定向
    ↓
启动 printf
    ↓
printf 处理最终收到的参数
```

很多 Shell 问题都来自混淆了这两个阶段。

---

## 10. 推荐模式

当需要把一段文本安全地输出为一行时，可以使用：

```bash
printf '%s\n' 'TEXT'
```

写入文件：

```bash
printf '%s\n' 'TEXT' > FILE
```

例如：

```bash
printf '%s\n' 'release * candidate' > label.txt
```

如果数据保存在变量中，更常见的是：

```bash
label='release * candidate'
printf '%s\n' "$label" > label.txt
```

这里变量通常使用双引号：

```bash
"$label"
```

以防变量值中的空格或通配符被 Bash 再次拆分或展开。

---

## 核心总结

```bash
printf '%s\n' 'release * candidate' > label.txt
```

需要理解四个关键点：

| 部分                      | 作用                      |
| ----------------------- | ----------------------- |
| `'release * candidate'` | 将整段文本作为一个字面参数，保护空格和 `*` |
| `'%s\n'`                | `%s` 输出字符串参数，`\n` 添加换行  |
| `>`                     | 将标准输出写入文件，并覆盖原内容        |
| `label.txt`             | 输出目标文件                  |

最重要的 Shell 思维是：

> **命令运行之前，Bash 会先解析命令行。引用（quoting）的主要作用，就是控制 Bash 在这个阶段允许进行哪些解释和展开。**

因此，对于包含空格、`*` 等 Shell 特殊字符的文本，不要直接裸写；应使用合适的引用方式保护它。
