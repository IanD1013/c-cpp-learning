# Bash 算术展开：在命令中进行整数计算

## 1. 什么是算术展开

Bash 可以直接进行整数运算，不需要额外调用计算器程序。

语法是：

```bash
$(( 算术表达式 ))
```

这叫做**算术展开（Arithmetic Expansion）**。

Bash 会：

1. 计算 `(( ... ))` 中的整数表达式；
2. 把 `$(( ... ))` 整体替换为计算结果；
3. 再执行最终组成的命令。

例如：

```bash
printf '%d\n' "$(( 12 * 34 ))"
```

Bash 会先计算：

```text
12 * 34 = 408
```

因此实际传给 `printf` 的值相当于：

```bash
printf '%d\n' "408"
```

输出：

```text
408
```

---

## 2. 在算术展开中使用变量

假设有两个变量：

```bash
blocks=1493
block_size=4096
```

分别表示：

* `blocks`：块数量
* `block_size`：每个块的字节数

总字节数就是：

```text
块数量 × 每块字节数
```

在 Bash 中可以直接写：

```bash
$(( blocks * block_size ))
```

完整命令：

```bash
printf '%d\n' "$(( blocks * block_size ))"
```

这里有一个重要特点：

> 在 `$(( ... ))` 算术上下文中，变量名通常不需要写 `$`。

推荐：

```bash
$(( blocks * block_size ))
```

而不是：

```bash
$(( $blocks * $block_size ))
```

后者在很多情况下也能工作，但前者更简洁，也是 Bash 算术表达式的常见写法。

---

## 3. 为什么算术表达式中的空格没有问题

普通 Bash 变量赋值不能在 `=` 两边添加空格：

```bash
blocks=1493
```

正确。

下面则是错误的：

```bash
blocks = 1493
```

因为 Bash 会把空格当作命令行参数的分隔符，把 `blocks` 当成一个命令名称。

但是在算术展开中：

```bash
$(( blocks * block_size ))
```

空格完全合法。

以下两种写法计算结果相同：

```bash
$((1493*4096))
```

```bash
$(( 1493 * 4096 ))
```

原因是进入 `$(( ... ))` 后，Bash 不再按照普通命令行的规则解析内容，而是在解析一个**算术表达式**。

为了可读性，通常推荐：

```bash
$(( blocks * block_size ))
```

即在运算符两边留空格。

---

## 4. 将计算结果直接写入文件

如果需要把计算结果保存到文件，可以结合：

* 算术展开 `$(( ... ))`
* `printf`
* 输出重定向 `>`

例如：

```bash
blocks=1493
block_size=4096
printf '%d\n' "$(( blocks * block_size ))" > bytes.txt
```

执行顺序可以理解为：

```text
读取 blocks 的整数值
        ↓
读取 block_size 的整数值
        ↓
计算 blocks * block_size
        ↓
得到十进制整数结果
        ↓
printf 输出结果和换行符
        ↓
> 将输出写入 bytes.txt
```

最终：

```text
bytes.txt
```

中只有一行整数结果，并以换行符结束。

---

## 5. `printf '%d\n'` 的作用

命令：

```bash
printf '%d\n' "$(( blocks * block_size ))"
```

中的格式字符串：

```text
%d\n
```

含义是：

| 内容   | 含义       |
| ---- | -------- |
| `%d` | 按十进制整数输出 |
| `\n` | 输出换行符    |

因此它很适合输出整数计算结果。

例如：

```bash
value=42
printf '%d\n' "$value"
```

输出：

```text
42
```

并且结尾存在换行符。

---

## 6. `>` 会覆盖目标文件

下面的命令：

```bash
printf '%d\n' "$(( blocks * block_size ))" > bytes.txt
```

使用的是：

```bash
>
```

它表示**覆盖重定向**。

如果 `bytes.txt`：

* 不存在：创建文件；
* 已存在：清空旧内容，然后写入新的输出。

这与追加重定向不同：

```bash
>>
```

`>>` 会把新内容追加到文件末尾。

如果要求文件中只能有一次计算结果，应使用：

```bash
>
```

而不是：

```bash
>>
```

---

## 7. 完整示例

在目录：

```text
/home/learner/bash-lab
```

中执行：

```bash
blocks=1493
block_size=4096
printf '%d\n' "$(( blocks * block_size ))" > bytes.txt
```

然后可以查看文件：

```bash
cat bytes.txt
```

文件应满足：

* 只有一行；
* 内容是 `1493 × 4096` 的计算结果；
* 行末包含换行符；
* 没有其他文字。

---

## 8. 为什么不应该手动计算结果

不推荐写成：

```bash
printf '%d\n' "某个手动算出来的数字" > bytes.txt
```

因为这样把输入值和结果分离了。

如果以后：

```bash
blocks
```

或：

```bash
block_size
```

发生变化，手动写死的结果不会自动更新。

而：

```bash
printf '%d\n' "$(( blocks * block_size ))"
```

表达的是计算关系：

```text
结果 = blocks × block_size
```

只要变量变化，结果就会重新计算。

这也是在 Shell 脚本中使用变量和算术展开的主要价值之一。

---

## 9. 常见错误

### 错误 1：变量赋值时在 `=` 两边加空格

错误：

```bash
blocks = 1493
```

正确：

```bash
blocks=1493
```

变量赋值必须写成一个连续的 shell 单词。

---

### 错误 2：忘记 `$(( ... ))`

下面只是普通文本：

```bash
printf '%s\n' "blocks * block_size"
```

输出会是：

```text
blocks * block_size
```

要让 Bash 真正计算，必须进入算术上下文：

```bash
printf '%d\n' "$(( blocks * block_size ))"
```

---

### 错误 3：使用了命令替换而不是算术展开

命令替换是：

```bash
$(command)
```

它表示：

> 执行一个命令，并捕获命令输出。

算术展开是：

```bash
$(( expression ))
```

它表示：

> 计算一个整数算术表达式。

两者不要混淆。

例如：

```bash
name=$(basename /srv/releases/api)
```

属于**命令替换**。

而：

```bash
bytes=$(( blocks * block_size ))
```

属于**算术展开**。

---

### 错误 4：使用 `>>` 导致重复结果

如果反复执行：

```bash
printf '%d\n' "$(( blocks * block_size ))" >> bytes.txt
```

文件中会不断增加新行。

如果要求文件始终只保存最新结果，应使用：

```bash
>
```

---

## 10. 算术展开还可以做哪些运算

Bash 的 `$(( ... ))` 支持常见整数运算，例如：

```bash
$(( a + b ))
$(( a - b ))
$(( a * b ))
$(( a / b ))
$(( a % b ))
```

分别表示：

| 运算符 | 含义   |
| --- | ---- |
| `+` | 加法   |
| `-` | 减法   |
| `*` | 乘法   |
| `/` | 整数除法 |
| `%` | 取余   |

例如：

```bash
a=10
b=3

printf '%d\n' "$(( a + b ))"
printf '%d\n' "$(( a * b ))"
printf '%d\n' "$(( a / b ))"
printf '%d\n' "$(( a % b ))"
```

其中：

```bash
$(( 10 / 3 ))
```

得到的是整数：

```text
3
```

而不是：

```text
3.333...
```

因为 Bash 算术展开主要进行**整数运算**。

---

## 11. 核心记忆

```bash
name=value
```

用于变量赋值，`=` 两边不能有空格。

```bash
$(( expression ))
```

用于整数算术计算。

在算术展开中可以直接写变量名：

```bash
$(( blocks * block_size ))
```

可以结合 `printf` 和重定向，把结果直接写入文件：

```bash
blocks=1493
block_size=4096
printf '%d\n' "$(( blocks * block_size ))" > bytes.txt
```

最重要的思想是：

> 不要手动计算并写死结果，而是让 Bash 保存输入值，并在需要结果的地方动态完成计算。
