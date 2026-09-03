# Bash 使用 `grep` 和正则表达式筛选日志中的 5xx 错误

## 1. `grep` 的作用

`grep` 用于从文本中筛选**符合某个模式（pattern）的整行内容**。

基本形式：

```bash
grep '模式' 文件
```

例如：

```bash
grep 'error' app.log
```

会输出 `app.log` 中所有包含 `error` 的行。

处理日志时，`grep` 比人工逐行查找更可靠，也能直接处理成千上万行数据。

---

## 2. `grep` 的模式不是 Shell glob

需要区分两种常见的模式语法：

### Shell glob

主要用于匹配**文件名**：

```bash
*.log
```

这里的 `*` 表示任意长度的字符序列。

例如：

```bash
ls *.log
```

可能匹配：

```text
access.log
error.log
server.log
```

### `grep` 正则表达式

`grep` 的第一个参数默认被解释为 **Basic Regular Expression，基本正则表达式**。

例如：

```bash
grep '[0-9]' file.txt
```

表示查找包含数字的行。

因此：

> Shell glob 和正则表达式是两套不同的语法，不要把 `*.log` 中 `*` 的含义直接套到 `grep` 正则表达式中。

---

## 3. 本例需要掌握的正则表达式

假设访问日志每一行最后都是 HTTP 状态码：

```text
GET /index.html 200
POST /login 500
GET /api/users 503
GET /health 200
```

目标是找到所有 **5xx Server Error**。

HTTP 5xx 状态码范围是：

```text
500 ~ 599
```

因此可以描述为：

1. 一个空格
2. 字符 `5`
3. 两个数字
4. 紧接着行尾

对应正则表达式：

```regex
 5[0-9][0-9]$
```

---

## 4. 正则各部分的含义

### `[0-9]`：匹配一个数字

```regex
[0-9]
```

表示：

> 匹配从 `0` 到 `9` 中的任意一个字符。

所以：

```regex
5[0-9][0-9]
```

能够匹配：

```text
500
501
503
520
599
```

但不会匹配：

```text
404
600
50
5000
```

---

### `$`：匹配行尾

```regex
$
```

不是一个真正存在的字符，而是一个**位置锚点（anchor）**，表示：

> 前面的内容必须出现在这一行的结尾。

因此：

```regex
 5[0-9][0-9]$
```

要求状态码必须位于行尾。

例如会匹配：

```text
GET /api 503
```

但不会因为路径中的数字而误匹配：

```text
GET /files/503/report 200
```

这里虽然出现了 `503`，但它不在行尾，所以不会被选中。

---

### `.`：匹配任意单个字符

正则表达式中的：

```regex
.
```

表示：

> 任意一个字符。

例如：

```regex
5..
```

可能匹配：

```text
500
5ab
5--
```

因此寻找 HTTP 5xx 状态码时，使用：

```regex
5[0-9][0-9]
```

通常比：

```regex
5..
```

更准确，因为后者并不保证后两个字符是数字。

---

## 5. 为什么应该给正则表达式加引号

推荐：

```bash
grep ' 5[0-9][0-9]$' /var/log/course/access.log
```

而不是依赖未加引号的模式。

单引号的作用是让 Bash 将：

```text
 5[0-9][0-9]$
```

完整地作为一个参数交给 `grep`，避免 Shell 对其中的特殊字符进行额外解释。

这是使用 `grep` 时非常重要的习惯：

```bash
grep '正则表达式' 文件
```

---

## 6. 将所有 5xx 请求写入文件

要求：

* 输入日志：

```text
/var/log/course/access.log
```

* 输出文件：

```text
/home/learner/errors.log
```

可以执行：

```bash
grep ' 5[0-9][0-9]$' /var/log/course/access.log > /home/learner/errors.log
```

### 执行过程

```text
/var/log/course/access.log
        │
        ▼
grep ' 5[0-9][0-9]$'
        │
        ├── 非 5xx 行 → 丢弃
        │
        └── 5xx 行 → 输出
                     │
                     ▼
          /home/learner/errors.log
```

`grep` 会：

1. 从上到下读取日志；
2. 检查每一行；
3. 只输出以 `5xx` 状态码结尾的行；
4. 保持原来的行顺序；
5. 保留正常的换行结构。

---

## 7. `>` 重定向的作用

命令：

```bash
grep ' 5[0-9][0-9]$' /var/log/course/access.log > /home/learner/errors.log
```

中的：

```bash
>
```

表示把标准输出写入文件，而不是显示在终端。

如果目标文件已经存在：

```text
/home/learner/errors.log
```

原内容会被覆盖。

因此最终文件只包含本次 `grep` 筛选出的结果。

如果有两条 5xx 日志：

```text
POST /login 500
GET /api/users 503
```

最终：

```text
/home/learner/errors.log
```

内容就是：

```text
POST /login 500
GET /api/users 503
```

---

## 8. 为什么行尾锚点 `$` 很重要

一个容易写出的不够严格的模式是：

```bash
grep '5[0-9][0-9]' /var/log/course/access.log
```

它只要求一行中**任何位置**出现三个以 `5` 开头的数字。

例如：

```text
GET /orders/512/details 200
```

也可能被匹配。

但真正需要检查的是 HTTP 状态码，而状态码位于行尾。

因此应该利用日志格式本身的结构：

```regex
 5[0-9][0-9]$
```

这种写法不仅判断“有没有 5xx 数字”，还判断“它是不是位于状态码应该出现的位置”。

> 处理结构化文本时，应尽可能利用字段位置和边界条件缩小匹配范围，减少误匹配。

---

## 9. 常见误区

### 误区一：把 glob 和正则表达式混为一谈

例如：

```bash
grep '5*' access.log
```

这里的 `*` 并不表示“任意字符”。

在基本正则表达式中，`*` 表示：

> 前面的表达式重复零次或多次。

因此：

```regex
5*
```

表示零个或多个 `5`，与 Shell 中：

```text
5*
```

完全不是同一个意思。

---

### 误区二：忽略行尾位置

```bash
grep '5[0-9][0-9]' access.log
```

可能匹配 URL、请求参数或其他字段中的数字。

更准确：

```bash
grep ' 5[0-9][0-9]$' access.log
```

---

### 误区三：使用 `.` 代替数字范围

```bash
grep ' 5..$' access.log
```

虽然很多日志中可能碰巧有效，但 `.` 可以匹配字母、符号等任意字符。

更严格：

```bash
grep ' 5[0-9][0-9]$' access.log
```

---

### 误区四：忘记输出重定向

```bash
grep ' 5[0-9][0-9]$' /var/log/course/access.log
```

只会把结果打印到终端，不会创建要求的结果文件。

需要：

```bash
grep ' 5[0-9][0-9]$' /var/log/course/access.log > /home/learner/errors.log
```

---

## 10. 最终命令

```bash
grep ' 5[0-9][0-9]$' /var/log/course/access.log > /home/learner/errors.log
```

核心思路：

```text
grep           → 筛选匹配的行
[0-9]          → 一个数字
5[0-9][0-9]   → 任意 5xx 状态码
$              → 必须位于行尾
'...'          → 防止 Bash 干扰正则表达式
>              → 将筛选结果写入文件
```

最值得记住的原则是：

> **不要只匹配“看起来像目标的数据”，还要利用数据所在的位置和边界进行约束。**
> 对 HTTP 5xx 日志而言，`$` 确保匹配的是行尾的状态码，而不是请求路径中偶然出现的三位数字。
