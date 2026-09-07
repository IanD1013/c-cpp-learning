# Bash 算术 `for` 循环：用计数器生成数字序列

## 1. 为什么使用算术 `for` 循环

如果需要生成：

```text
1
2
3
4
5
```

可以直接写死数字列表：

```bash
for i in 1 2 3 4 5; do
  ...
done
```

但这种方式的问题是：最大值改变时，脚本也必须修改。

例如希望同一个脚本既能：

```bash
bash count.sh 5
```

生成 `1~5`，又能：

```bash
bash count.sh 12
```

生成 `1~12`，更适合使用 **算术 `for` 循环（arithmetic for loop）**。

---

## 2. 算术 `for` 循环的基本结构

Bash 的算术 `for` 循环写法类似 C：

```bash
for ((initializer; condition; update)); do
  commands
done
```

其中 `(( ... ))` 内部包含三个部分：

| 部分          | 示例         | 执行时机      | 作用     |
| ----------- | ---------- | --------- | ------ |
| initializer | `i = 1`    | 循环开始前执行一次 | 初始化计数器 |
| condition   | `i <= top` | 每轮循环开始前检查 | 决定是否继续 |
| update      | `i++`      | 每轮循环结束后执行 | 更新计数器  |

例如：

```bash
for ((i = 1; i <= 5; i++)); do
  printf '%d\n' "$i"
done
```

输出：

```text
1
2
3
4
5
```

---

## 3. 循环到底如何执行

考虑：

```bash
for ((i = 1; i <= 5; i++)); do
  printf '%d\n' "$i"
done
```

执行顺序是：

```text
i = 1
  ↓
检查 i <= 5
  ↓ true
执行循环体，输出 1
  ↓
i++
  ↓
检查 i <= 5
  ↓ true
执行循环体，输出 2
  ↓
...
```

直到：

```text
i = 6
```

此时：

```bash
i <= 5
```

为假，循环结束。

### 为什么第一轮看到的是 `1`

因为：

```bash
i++
```

是在**循环体执行完成之后**才执行。

因此流程是：

```text
初始化 i=1
→ 使用 i=1
→ i++
→ 使用 i=2
→ i++
...
```

而不是先增加再使用。

---

## 4. `<` 和 `<=` 决定是否包含最大值

这是计数循环最容易出现的边界错误。

### 使用 `<`

```bash
for ((i = 1; i < 5; i++)); do
  printf '%d\n' "$i"
done
```

输出：

```text
1
2
3
4
```

因为当：

```text
i = 5
```

时：

```bash
i < 5
```

已经为假。

---

### 使用 `<=`

```bash
for ((i = 1; i <= 5; i++)); do
  printf '%d\n' "$i"
done
```

输出：

```text
1
2
3
4
5
```

如果要求“从 1 数到最大值，并且包含最大值”，应使用：

```bash
i <= top
```

---

## 5. 从命令行参数读取最大值

Bash 中：

```bash
$1
```

表示脚本收到的第一个位置参数。

例如：

```bash
bash count.sh 5
```

此时：

```bash
$1
```

就是：

```text
5
```

可以先保存到变量：

```bash
top="$1"
```

然后用于循环：

```bash
for ((i = 1; i <= top; i++)); do
  ...
done
```

完整的计数逻辑：

```bash
top="$1"

for ((i = 1; i <= top; i++)); do
  printf '%d\n' "$i"
done
```

运行：

```bash
bash count.sh 5
```

输出：

```text
1
2
3
4
5
```

运行：

```bash
bash count.sh 12
```

则会输出 `1` 到 `12`。

---

## 6. 为什么 `(( ))` 中变量通常不写 `$`

普通 Bash 变量读取通常写：

```bash
echo "$top"
```

但在算术上下文：

```bash
(( ... ))
```

内部，Bash 会自动把变量名当作数字变量读取。

因此推荐写：

```bash
((i <= top))
```

而不必写：

```bash
((i <= $top))
```

同样：

```bash
i++
```

等价于让 `i` 增加 1。

常见算术写法包括：

```bash
((i++))
((i--))
((i += 2))
((i <= top))
((i == 10))
```

---

## 7. 把每个数字写入文件

要求每个数字单独占一行，可以使用：

```bash
printf '%d\n' "$i"
```

其中：

```text
%d   按整数输出
\n   输出换行符
```

例如：

```bash
printf '%d\n' 3
```

得到：

```text
3
```

并在 `3` 后面带有换行符。

要把内容追加到文件：

```bash
printf '%d\n' "$i" >> count.txt
```

`>>` 表示**追加（append）**。

---

## 8. 为什么循环开始前必须清空旧文件

假设第一次执行：

```bash
bash count.sh 5
```

文件内容为：

```text
1
2
3
4
5
```

如果第二次仍然直接使用：

```bash
>> count.txt
```

第二次执行后就会变成：

```text
1
2
3
4
5
1
2
3
4
5
```

因为 `>>` 不会删除旧内容，只会继续追加。

正确做法是在循环开始前清空文件：

```bash
: > count.txt
```

然后循环内部再追加：

```bash
printf '%d\n' "$i" >> count.txt
```

这样每次执行都会重新从空文件开始。

---

## 9. `: > count.txt` 是什么意思

`:` 是 Bash 的一个内建命令，叫做 **null command（空命令）**。

它：

* 什么实际工作都不做；
* 返回成功状态 `0`。

因此：

```bash
: > count.txt
```

可以理解为：

```text
运行一个什么都不做的命令
并把它的输出重定向到 count.txt
```

虽然 `:` 没有内容可以输出，但：

```bash
>
```

会先把目标文件截断为长度 0。

因此：

```bash
: > count.txt
```

的实际效果就是：

> 创建 `count.txt`（如果不存在），或者清空它（如果已经存在）。

也可以写：

```bash
> count.txt
```

不过：

```bash
: > count.txt
```

意图通常更加明显。

---

## 10. 完整的 `count.sh`

```bash
#!/usr/bin/env bash

top="$1"

: > count.txt

for ((i = 1; i <= top; i++)); do
  printf '%d\n' "$i" >> count.txt
done
```

运行：

```bash
bash count.sh 5
```

得到：

```text
count.txt
```

内容：

```text
1
2
3
4
5
```

再次运行：

```bash
bash count.sh 3
```

文件会被重新生成，而不是追加旧内容：

```text
1
2
3
```

---

## 11. 更高效的文件重定向写法

循环中每次执行：

```bash
>> count.txt
```

都会对文件进行追加操作。

如果希望把整个循环的输出一次性重定向到文件，可以写成：

```bash
#!/usr/bin/env bash

top="$1"

for ((i = 1; i <= top; i++)); do
  printf '%d\n' "$i"
done > count.txt
```

这里：

```bash
done > count.txt
```

表示：

> 把整个循环产生的标准输出写入 `count.txt`。

由于使用的是：

```bash
>
```

旧文件会自动被清空，因此不再需要：

```bash
: > count.txt
```

这种写法通常更加简洁。

---

## 12. 一个重要的安全问题：不要直接信任 `$1`

下面的代码：

```bash
top="$1"

for ((i = 1; i <= top; i++)); do
  ...
done
```

假设 `$1` 是合法整数。

但 Bash 的算术环境并不会简单地把输入当作普通十进制数字。例如：

```bash
bash count.sh '2+3'
```

算术环境可能把：

```text
2+3
```

当作算术表达式计算。

如果脚本面向真实用户输入，应该先验证参数确实是十进制整数。

例如：

```bash
[[ "$1" =~ ^(0|[1-9][0-9]*)$ ]] || exit 2
```

然后再进入算术运算：

```bash
top="$1"
```

完整的稳健版本：

```bash
#!/usr/bin/env bash

[[ "$1" =~ ^(0|[1-9][0-9]*)$ ]] || exit 2

top="$1"

for ((i = 1; i <= top; i++)); do
  printf '%d\n' "$i"
done > count.txt
```

---

## 13. 常见错误

### 错误 1：使用 `<` 导致少一个数字

```bash
for ((i = 1; i < top; i++)); do
```

如果：

```text
top=5
```

只会得到：

```text
1
2
3
4
```

需要包含最大值时使用：

```bash
i <= top
```

---

### 错误 2：忘记更新计数器

```bash
for ((i = 1; i <= top; )); do
  printf '%d\n' "$i"
done
```

`i` 永远保持为 `1`，条件始终成立，因此会形成无限循环。

需要：

```bash
i++
```

---

### 错误 3：直接用 `>>`，却不清理旧文件

```bash
for ((i = 1; i <= top; i++)); do
  printf '%d\n' "$i" >> count.txt
done
```

脚本重复运行时会不断追加。

应该先：

```bash
: > count.txt
```

或者更直接地：

```bash
for ((i = 1; i <= top; i++)); do
  printf '%d\n' "$i"
done > count.txt
```

---

## 14. 核心记忆

算术 `for` 循环的核心结构：

```bash
for ((初始化; 条件; 更新)); do
  ...
done
```

典型的从 `1` 数到 `top`：

```bash
for ((i = 1; i <= top; i++)); do
  ...
done
```

三个阶段分别是：

```text
i = 1      → 从哪里开始
i <= top   → 什么时候继续
i++        → 每轮结束后如何变化
```

如果整个循环的结果要重新写入文件，推荐：

```bash
for ((i = 1; i <= top; i++)); do
  printf '%d\n' "$i"
done > count.txt
```

这样每次运行都会重新构造 `count.txt`，而不会把新结果追加到旧结果之后。
