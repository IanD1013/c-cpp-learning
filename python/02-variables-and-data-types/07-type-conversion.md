# Python 类型转换（Type Conversion）

Python 是动态类型语言，但数据类型依然非常重要。

在实际程序中，从以下来源得到的数据经常是字符串 `str`：

* 用户输入
* 文本文件
* CSV / JSON 数据
* Web API 返回的数据
* 环境变量

即使字符串看起来像数字，例如 `"123"`，它本质上仍然是字符串，不能直接按照数字进行计算。

因此，经常需要进行**类型转换（type conversion）**：把一个类型的值转换成另一个类型的值。

---

## 1. Python 常用的类型转换函数

Python 中最常见的四个转换函数是：

| 函数        | 转换目标        | 示例                       |
| --------- | ----------- | ------------------------ |
| `int()`   | 整数 `int`    | `int("42")` → `42`       |
| `float()` | 浮点数 `float` | `float("3.14")` → `3.14` |
| `str()`   | 字符串 `str`   | `str(25)` → `"25"`       |
| `bool()`  | 布尔值 `bool`  | `bool(1)` → `True`       |

基本语法：

```python
int(value)
float(value)
str(value)
bool(value)
```

这些函数会**返回一个转换后的新值**。

例如：

```python
text = "49"
number = int(text)

print(text)    # "49"
print(number)  # 49
```

这里：

```text
text   → str
number → int
```

`int(text)` 并不会把原来的 `text` 本身修改成整数，而是创建并返回一个新的整数值。

---

# 2. `int()`：转换成整数

`int()` 用来创建整数。

## 字符串转换为整数

```python
price_text = "49"

price = int(price_text)

print(price)        # 49
print(type(price))  # <class 'int'>
```

此时：

```python
"49"      # 字符串
49        # 整数
```

虽然它们看起来很像，但类型完全不同。

---

## 浮点数转换为整数

```python
int(3.9)
# 3
```

需要特别注意：

> `int()` 不会进行四舍五入，而是直接去掉小数部分。

例如：

```python
int(3.9)   # 3
int(7.1)   # 7
int(-2.8)  # -2
```

可以理解为：

```text
3.9   → 3
7.1   → 7
-2.8  → -2
```

这种行为叫做**向零截断（truncate toward zero）**。

因此：

```python
int(3.9)
```

不是：

```python
4
```

如果真正需要四舍五入，应考虑使用：

```python
round(3.9)
# 4
```

---

## 不是所有字符串都能转换成整数

下面可以：

```python
int("123")
# 123
```

但下面不可以：

```python
int("hello")
```

会产生：

```text
ValueError
```

同样：

```python
int("3.14")
```

也会报错，因为 `"3.14"` 不是合法的整数字符串。

如果需要转换小数字符串，可以使用：

```python
float("3.14")
# 3.14
```

---

# 3. `float()`：转换成浮点数

`float()` 用来创建浮点数。

例如：

```python
temperature = float("98.6")

print(temperature)
# 98.6
```

也可以把整数转换成浮点数：

```python
float(5)
# 5.0
```

所以：

```python
5
```

和：

```python
5.0
```

数值上相等，但类型不同：

```python
type(5)
# <class 'int'>

type(5.0)
# <class 'float'>
```

---

# 4. `str()`：转换成字符串

`str()` 可以把其他类型的值转换成字符串。

例如：

```python
age = 25

age_text = str(age)

print(age_text)
# "25"
```

这在拼接字符串时非常常见。

错误示例：

```python
age = 25

message = "I am " + age
```

这里会出现：

```text
TypeError
```

因为 Python 不允许直接使用 `+` 拼接：

```text
str + int
```

应该先转换：

```python
age = 25

message = "I am " + str(age) + " years old"

print(message)
# I am 25 years old
```

实际开发中，如果只是为了把变量放入字符串，更推荐使用 **f-string**：

```python
age = 25

message = f"I am {age} years old"
```

这样通常更清晰，而且 Python 会自动处理字符串表示。

---

# 5. `bool()`：转换成布尔值

`bool()` 会把一个值转换成：

```python
True
```

或者：

```python
False
```

Python 有一套固定的**真值规则（truthiness）**。

---

## 数字

数字 `0` 是 `False`：

```python
bool(0)
# False

bool(0.0)
# False
```

非零数字通常是 `True`：

```python
bool(1)
# True

bool(42)
# True

bool(-5)
# True
```

可以简单记成：

```text
0       → False
非 0    → True
```

---

## 字符串

空字符串是 `False`：

```python
bool("")
# False
```

非空字符串是 `True`：

```python
bool("hello")
# True
```

一个非常容易误解的例子：

```python
bool("False")
# True
```

原因是 `"False"` 本身只是一个**非空字符串**。

Python 并不会因为字符串内容写着 `"False"`，就把它理解成布尔值 `False`。

同样：

```python
bool("0")
# True
```

因为 `"0"` 也是非空字符串。

---

## 常见 False 值

Python 中常见的“假值（falsy values）”包括：

```python
False
None
0
0.0
""
[]
{}
()
```

例如：

```python
bool([])   # False
bool({})   # False
bool(())   # False
```

非空的容器通常是 `True`：

```python
bool([1, 2])       # True
bool({"name": 1})  # True
```

---

# 6. 为什么类型转换很重要

考虑：

```python
"5" + "3"
```

结果是：

```python
"53"
```

而不是：

```python
8
```

原因是：

```python
"5"
```

和：

```python
"3"
```

都是字符串。

对于字符串：

```python
+
```

表示的是**字符串拼接（concatenation）**：

```python
"hello" + "world"
# "helloworld"
```

因此：

```python
"5" + "3"
```

相当于把两个字符连接起来：

```text
"5" + "3"
 ↓
"53"
```

如果想进行数学加法，需要先转换成整数：

```python
int("5") + int("3")
# 8
```

此时过程是：

```text
"5" → 5
"3" → 3

5 + 3 → 8
```

---

# 7. 用户输入是最常见的转换场景

Python 的：

```python
input()
```

**永远返回字符串。**

例如：

```python
age = input("请输入年龄：")
```

假设用户输入：

```text
25
```

变量 `age` 仍然是：

```python
"25"
```

而不是：

```python
25
```

可以检查：

```python
print(type(age))
# <class 'str'>
```

如果想进行数学运算，需要转换：

```python
age = int(input("请输入年龄："))

next_year = age + 1

print(next_year)
```

用户输入：

```text
25
```

结果：

```text
26
```

---

# 8. 一个完整例子：计算两个数字的和

错误写法：

```python
a = input("输入第一个数字：")
b = input("输入第二个数字：")

print(a + b)
```

如果输入：

```text
5
3
```

结果是：

```text
53
```

因为：

```text
a = "5"
b = "3"
```

正确写法：

```python
a = int(input("输入第一个数字："))
b = int(input("输入第二个数字："))

print(a + b)
```

结果：

```text
8
```

这里实际经历了：

```text
用户输入
   ↓
"5"
   ↓ int()
5
   ↓
参与数学运算
```

---

# 9. 类型转换可能失败

类型转换并不是一定成功。

例如：

```python
int("abc")
```

会产生：

```text
ValueError
```

因为 `"abc"` 无法解释成整数。

同样：

```python
float("hello")
```

也会失败。

实际程序中，如果输入可能不合法，通常需要处理异常：

```python
try:
    age = int(input("请输入年龄："))
    print(age)
except ValueError:
    print("请输入有效的整数")
```

这样即使用户输入：

```text
hello
```

程序也不会直接崩溃。

---

# 10. 常见误区

## 误区一：看起来像数字就是数字

```python
"100"
```

看起来是数字，但它实际上是：

```python
str
```

可以检查：

```python
type("100")
# <class 'str'>
```

而：

```python
100
```

才是：

```python
int
```

---

## 误区二：`int()` 会四舍五入

错误理解：

```python
int(3.9) == 4
```

实际：

```python
int(3.9)
# 3
```

`int()` 是截断，不是四舍五入。

---

## 误区三：`bool("False")` 是 `False`

实际：

```python
bool("False")
# True
```

因为布尔转换主要检查字符串是否为空：

```text
""        → False
"False"   → True
"0"       → True
"hello"   → True
```

---

## 误区四：转换会修改原变量

例如：

```python
x = "100"

int(x)

print(type(x))
```

结果仍然是：

```python
<class 'str'>
```

因为：

```python
int(x)
```

只是返回了一个新的整数，没有把结果保存下来。

应该写：

```python
x = int(x)
```

或者：

```python
number = int(x)
```

---

# 11. 类型转换的核心思维模型

可以把类型转换理解为：

```text
原值
 │
 │ 转换函数
 ▼
新类型的值
```

例如：

```text
"42"
 │
 │ int()
 ▼
42
```

或者：

```text
25
 │
 │ str()
 ▼
"25"
```

关键点是：

> Python 的类型决定一个值可以如何被解释，以及可以参与什么操作。

例如：

```python
"5" + "3"
```

是字符串拼接：

```text
"53"
```

而：

```python
5 + 3
```

是数学加法：

```text
8
```

因此，当程序行为和预期不一致时，首先检查：

```python
type(value)
```

通常可以快速发现问题。

---

# 12. 核心总结

Python 最常见的类型转换函数：

```python
int(value)
float(value)
str(value)
bool(value)
```

需要重点记住：

```text
int("42")      → 42
float("3.14")  → 3.14
str(25)        → "25"

int(3.9)       → 3
bool(0)        → False
bool(42)       → True
bool("")       → False
bool("False")  → True
```

尤其注意三个规则：

1. `input()` 返回的始终是字符串。
2. `int()` 对浮点数是截断小数部分，不是四舍五入。
3. `bool()` 判断的是值的“真值”，非空字符串即使内容是 `"False"` 也仍然是 `True`。

当程序出现类似：

```python
"5" + "3" == "53"
```

这样的意外结果时，应首先思考：

> **当前变量到底是什么类型？是否需要先进行类型转换？**
