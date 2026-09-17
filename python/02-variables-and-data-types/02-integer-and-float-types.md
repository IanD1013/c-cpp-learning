# Python 中的整数 `int` 与浮点数 `float`

## 1. Python 的两种常见数字类型

Python 中最常用的数字类型有两种：

* **整数（integer，`int`）**：没有小数部分的整数。
* **浮点数（floating-point number，`float`）**：通常用于表示带小数的数字。

```python
age = 25
temperature = 36.5
```

其中：

```python
25      # int
36.5    # float
```

整数可以是正数、负数或 `0`：

```python
10
-3
0
```

浮点数通常带有小数点：

```python
3.14
-0.5
100.0
```

需要特别注意：

```python
100    # int
100.0  # float
```

虽然它们表示的数学数值相同，但在 Python 中属于不同的类型。

可以使用 `type()` 查看变量的类型：

```python
print(type(100))
# <class 'int'>

print(type(100.0))
# <class 'float'>
```

---

## 2. `int` 和 `float` 的基本区别

| 值      | Python 类型 |
| ------ | --------- |
| `10`   | `int`     |
| `-7`   | `int`     |
| `0`    | `int`     |
| `10.0` | `float`   |
| `3.14` | `float`   |
| `-0.5` | `float`   |

例如：

```python
count = 100
price = 9.99

print(type(count))
# <class 'int'>

print(type(price))
# <class 'float'>
```

Python 会根据赋给变量的值自动决定数字类型，不需要提前声明。

---

# 3. 普通除法 `/`

Python 中 `/` 表示**普通除法（true division）**。

一个非常重要的规则是：

> `/` 的结果总是 `float`。

例如：

```python
print(7 / 2)
# 3.5
```

即使除法结果恰好是整数，也仍然会得到浮点数：

```python
print(20 / 5)
# 4.0
```

而不是：

```text
4
```

例如：

```python
result = 20 / 4

print(result)
# 5.0

print(type(result))
# <class 'float'>
```

因此：

```python
20 / 4
```

得到：

```python
5.0
```

而不是 `5`。

---

# 4. 整除运算 `//`

`//` 称为：

**floor division（向下取整除法）**

例如：

```python
print(7 // 2)
# 3
```

因为：

```text
7 / 2 = 3.5
```

然后向下取整：

```text
3.5 → 3
```

另一个例子：

```python
print(20 // 6)
# 3
```

因为：

```text
20 / 6 ≈ 3.333
```

向下取整后得到：

```text
3
```

---

## 5. `/` 和 `//` 的区别

```python
print(7 / 2)
# 3.5

print(7 // 2)
# 3
```

可以理解为：

```text
/   → 正常除法
//  → 除法之后向下取整
```

对比：

| 表达式       |         结果 |
| --------- | ---------: |
| `20 / 5`  |      `4.0` |
| `20 // 5` |        `4` |
| `7 / 2`   |      `3.5` |
| `7 // 2`  |        `3` |
| `20 / 6`  | `3.333...` |
| `20 // 6` |        `3` |

当 `//` 两边都是整数时，结果通常也是整数：

```python
result = 7 // 2

print(result)
# 3

print(type(result))
# <class 'int'>
```

但是如果其中存在 `float`，结果通常也是 `float`：

```python
print(7.0 // 2)
# 3.0
```

所以不能简单理解为：

> `//` 永远返回整数。

更准确的说法是：

> `//` 执行向下取整除法；如果两个操作数都是 `int`，结果是 `int`。

---

# 6. `//` 是“向下取整”，不是“直接删除小数”

对于正数，两者看起来没有区别：

```python
7 / 2
# 3.5

7 // 2
# 3
```

但是负数可以看出真正的区别：

```python
print(-7 // 2)
# -4
```

因为：

```text
-7 / 2 = -3.5
```

`//` 会向**负无穷方向**取整：

```text
-3.5 → -4
```

而不是简单删除 `.5` 得到 `-3`。

因此：

```python
7 // 2
# 3

-7 // 2
# -4
```

这是理解 `//` 时最容易忽略的一点。

---

# 7. 整数和浮点数可以一起计算

Python 允许 `int` 和 `float` 一起进行数学运算。

例如：

```python
result = 10 + 2.5

print(result)
# 12.5
```

结果会变成 `float`：

```python
print(type(result))
# <class 'float'>
```

类似：

```python
10 * 2.5
# 25.0

10 - 0.5
# 9.5
```

可以简单理解为：

> 当普通数学运算同时涉及 `int` 和 `float` 时，Python 通常会把结果表示为 `float`。

---

# 8. 数字不能直接使用 `+` 与字符串拼接

假设：

```python
count = 100
```

下面的代码是错误的：

```python
print("Count: " + count)
```

因为：

```python
"Count: "   # str
count       # int
```

`+` 在这里表示**字符串拼接**，因此两边都必须是字符串。

Python 会报类似错误：

```text
TypeError: can only concatenate str (not "int") to str
```

需要先使用 `str()` 将数字转换成字符串：

```python
print("Count: " + str(count))
# Count: 100
```

对于浮点数也是一样：

```python
price = 9.99

print("Price: " + str(price))
# Price: 9.99
```

---

# 9. `str()` 不会修改原来的数字类型

例如：

```python
speed = 88

print("Speed: " + str(speed))
# Speed: 88
```

这里：

```python
str(speed)
```

只是临时产生字符串：

```python
"88"
```

变量 `speed` 本身仍然是整数：

```python
print(type(speed))
# <class 'int'>
```

同理：

```python
weight = 72.5

print("Weight: " + str(weight))
# Weight: 72.5

print(type(weight))
# <class 'float'>
```

---

# 10. 更常见的输出方式：逗号和 f-string

虽然可以使用：

```python
print("Price: " + str(price))
```

但 Python 中通常还有更方便的写法。

## 使用逗号

```python
price = 9.99

print("Price:", price)
# Price: 9.99
```

`print()` 会自动处理数字转换，因此不需要手动调用 `str()`。

---

## 使用 f-string

现代 Python 中非常常见的是 **f-string（格式化字符串）**：

```python
price = 9.99

print(f"Price: {price}")
# Price: 9.99
```

整数也一样：

```python
count = 100

print(f"Count: {count}")
# Count: 100
```

相比：

```python
print("Count: " + str(count))
```

f-string 通常更加清晰，尤其是在一个字符串中需要插入多个变量时：

```python
name = "Apple"
count = 5
price = 2.5

print(f"{name}: {count} items, ${price}")
# Apple: 5 items, $2.5
```

---

# 11. 浮点数并不是数学意义上的完全精确小数

`float` 使用计算机的二进制浮点格式存储数字，因此某些十进制小数无法被完全精确地表示。

例如：

```python
print(0.1 + 0.2)
```

可能得到：

```text
0.30000000000000004
```

这不是 Python 算错了，而是浮点数表示方式导致的精度问题。

因此：

```python
0.1 + 0.2 == 0.3
```

可能得到：

```python
False
```

对于刚开始学习 Python，只需要记住：

> `float` 适合绝大多数普通小数计算，但它不一定能精确表示所有十进制小数。

涉及货币或高精度计算时，通常需要使用 `Decimal` 等更适合的工具，而不是直接依赖 `float`。

---

# 12. 完整示例

```python
count = 100
price = 9.99

print(type(count))
# <class 'int'>

print(type(price))
# <class 'float'>

print(20 / 5)
# 4.0

print(7 / 2)
# 3.5

print(20 // 6)
# 3

print(7 // 2)
# 3

print(-7 // 2)
# -4

print("Count: " + str(count))
# Count: 100

print(f"Price: {price}")
# Price: 9.99
```

---

# 核心总结

```text
int
→ 整数
→ 例如 10、-3、0

float
→ 浮点数
→ 例如 3.14、-0.5、100.0

/
→ 普通除法
→ 总是返回 float
→ 20 / 5 == 4.0

//
→ 向下取整除法
→ 7 // 2 == 3
→ -7 // 2 == -4

str(number)
→ 把数字转换成字符串
→ 用于字符串拼接
```

最需要记住的几个例子：

```python
100      # int
100.0    # float

20 / 5   # 4.0
7 / 2    # 3.5

20 // 6  # 3
-7 // 2  # -4

str(100)   # "100"
str(9.99)  # "9.99"
```
