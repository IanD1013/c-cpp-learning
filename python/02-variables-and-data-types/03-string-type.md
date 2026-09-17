# Python 字符串：字符序列，而不是数字

## 1. 什么是字符串

在 Python 中，**字符串（string，类型为 `str`）** 用来表示文本。

字符串由一系列字符组成，并使用引号包起来：

```python
name = "Alice"
message = "Hello"
number_text = "42"
```

关键点：

> `"42"` 看起来像数字，但它实际上不是整数 `42`，而是由字符 `"4"` 和 `"2"` 组成的字符串。

可以通过 `type()` 查看数据类型：

```python
print(type(42))
# <class 'int'>

print(type("42"))
# <class 'str'>
```

因此：

```python
42
```

表示数字，可以进行数学运算。

而：

```python
"42"
```

表示文本。

---

## 2. 字符串和数字的核心区别

Python 中，数据的**类型决定操作的含义**。

例如，`+` 对数字和字符串有完全不同的作用。

### 数字之间：数学加法

```python
10 + 5
# 15

3.2 + 1.8
# 5.0
```

这里的 `+` 表示数学意义上的加法。

### 字符串之间：字符串拼接

当 `+` 用于两个字符串时，它表示 **concatenation（字符串拼接）**：

```python
"sun" + "flower"
# "sunflower"
```

Python 会把后一个字符串直接接到前一个字符串后面。

例如：

```python
"100" + "200"
# "100200"
```

而不是：

```text
300
```

原因是 `"100"` 和 `"200"` 都是字符串，而不是数字。

可以理解为：

```text
"100" + "200"

相当于：

"100" 接上 "200"

得到：

"100200"
```

---

## 3. 引号决定它是不是字符串

比较下面两组代码。

### 字符串

```python
a = "7"
b = "3"

print(a + b)
```

输出：

```text
73
```

因为：

```python
a  # "7"
b  # "3"
```

都是字符串，因此执行字符串拼接。

### 整数

```python
x = 7
y = 3

print(x + y)
```

输出：

```text
10
```

因为 `x` 和 `y` 都是整数，所以执行数学加法。

### 对比

| 代码              | 类型          | `+` 的含义 | 结果         |
| --------------- | ----------- | ------- | ---------- |
| `7 + 3`         | `int + int` | 数学加法    | `10`       |
| `"7" + "3"`     | `str + str` | 字符串拼接   | `"73"`     |
| `"100" + "200"` | `str + str` | 字符串拼接   | `"100200"` |

所以不要只看内容长得像什么，而要看它的**实际数据类型**。

---

## 4. Python 创建字符串的三种常见方式

### 单引号

```python
name = 'Alice'
```

### 双引号

```python
city = "Berlin"
```

单引号和双引号在 Python 中基本没有功能区别：

```python
"hello"
'hello'
```

它们都是 `str`。

通常选择更方便书写的形式。

例如：

```python
message = "It's a nice day."
```

比下面这样更容易阅读：

```python
message = 'It\'s a nice day.'
```

因为如果字符串本身包含单引号，使用双引号可以避免转义。

同理：

```python
message = 'He said "Hello".'
```

---

## 5. 三引号与多行字符串

Python 还支持三引号：

```python
"""
...
"""
```

或者：

```python
'''
...
'''
```

它们最常用于创建**跨多行的字符串**。

例如：

```python
poem = """Roses are red,
Violets are blue."""

print(poem)
```

输出：

```text
Roses are red,
Violets are blue.
```

普通单引号或双引号通常不能直接跨多行：

```python
message = "hello
world"
```

这会产生语法错误。

如果需要真正的多行文本，三引号会更方便。

---

## 6. 字符串可以存储在变量中

字符串和其他 Python 数据一样，可以赋值给变量：

```python
message = "Good morning!"

print(message)
```

输出：

```text
Good morning!
```

变量本身只是一个名称，它引用了字符串 `"Good morning!"`。

例如：

```python
first = "py"
second = "thon"

language = first + second

print(language)
```

输出：

```text
python
```

执行过程可以理解为：

```text
first  -> "py"
second -> "thon"

"py" + "thon"
      ↓
   "python"
```

---

## 7. Python 不会因为字符串里全是数字就自动把它当数字

这是初学者非常重要的一点。

例如：

```python
a = "100"
b = "200"

print(a + b)
```

输出：

```text
100200
```

Python 不会看到 `"100"` 后想：

> 这里都是数字字符，我自动把它转换成整数吧。

它只关心当前的数据类型：

```python
type(a)
# str

type(b)
# str
```

所以执行的是：

```text
字符串 + 字符串
```

也就是拼接。

---

## 8. 字符串数字如何转换成真正的数字

实际程序中，经常会得到这样的数据：

```python
age = "18"
```

虽然内容看起来像数字，但它仍然是字符串。

如果想做数学运算，需要进行类型转换。

### 转成整数：`int()`

```python
age = "18"

age_number = int(age)

print(age_number + 1)
```

输出：

```text
19
```

此时：

```python
type(age)
# str

type(age_number)
# int
```

### 转成浮点数：`float()`

```python
price = "19.99"

price_number = float(price)

print(price_number + 5)
```

输出：

```text
24.99
```

---

## 9. 数字也可以转换成字符串

反过来，可以使用 `str()` 把数字转换成字符串：

```python
age = 18

text = str(age)

print(type(text))
# <class 'str'>
```

转换之后：

```python
"Age: " + str(age)
```

可以进行字符串拼接。

例如：

```python
age = 18

print("Age: " + str(age))
```

输出：

```text
Age: 18
```

如果直接写：

```python
"Age: " + 18
```

会报错，因为 Python 不能直接把字符串和整数使用 `+` 拼接。

---

## 10. 常见错误：期待 `"5" + "5"` 等于 `10`

错误理解：

```python
"5" + "5"
```

很多初学者会认为结果应该是：

```text
10
```

实际上结果是：

```text
55
```

原因：

```python
"5"   # str
"5"   # str
```

所以执行的是字符串拼接：

```text
"5" + "5"
     ↓
   "55"
```

如果想计算：

```text
5 + 5 = 10
```

应该使用真正的数字：

```python
5 + 5
# 10
```

或者先转换：

```python
int("5") + int("5")
# 10
```

---

## 11. 字符串不能随意和数字使用 `+`

例如：

```python
age = 18

print("Age: " + age)
```

会出现类似错误：

```text
TypeError
```

因为 Python 不知道你想执行：

* 数学加法；
* 还是字符串拼接。

必须明确转换：

```python
print("Age: " + str(age))
```

输出：

```text
Age: 18
```

实际 Python 代码中，也经常使用 f-string：

```python
age = 18

print(f"Age: {age}")
```

输出同样是：

```text
Age: 18
```

对于把变量插入文本，f-string 通常比手动使用 `+` 和 `str()` 更清晰。

---

## 12. 字符串本质上是字符序列

字符串不仅仅是“一整块文本”，它本质上是按照顺序排列的一系列字符。

例如：

```python
word = "Python"
```

可以理解为：

```text
P y t h o n
```

每个字符都有自己的位置：

```text
位置：  0 1 2 3 4 5
字符：  P y t h o n
```

因此可以访问单个字符：

```python
word = "Python"

print(word[0])
# P

print(word[1])
# y
```

同样：

```python
number_text = "42"
```

内部可以理解为：

```text
位置：  0 1
字符：  4 2
```

所以 `"42"` 本质上是两个字符组成的字符串，而不是数学意义上的四十二。

---

## 13. 一个判断字符串与数字的简单方法

看到 Python 值时，可以先问：

> 它有没有被引号包起来？

例如：

```python
100
```

没有引号：

```python
type(100)
# int
```

而：

```python
"100"
```

有引号：

```python
type("100")
# str
```

再例如：

```python
3.14
```

是：

```python
float
```

而：

```python
"3.14"
```

是：

```python
str
```

引号里的内容会被作为文本处理，即使它看起来像数字。

---

## 14. 核心总结

Python 中：

```python
"42"
```

和：

```python
42
```

是两种完全不同的数据。

```text
"42" → str → 文本
42   → int → 整数
```

`+` 的行为取决于操作对象的数据类型：

```python
10 + 5
# 15
```

表示数学加法。

```python
"10" + "5"
# "105"
```

表示字符串拼接。

最重要的规则是：

> **在 Python 中，内容看起来像什么并不重要，真正决定操作行为的是数据类型。**

需要数学计算时使用数字类型：

```python
int
float
```

需要处理文本时使用：

```python
str
```

必要时可以使用：

```python
int()
float()
str()
```

在不同数据类型之间进行明确转换。
