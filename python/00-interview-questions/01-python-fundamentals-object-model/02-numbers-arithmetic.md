# Python 数值类型

Python 的数值类型看起来很简单，直到面试官让你预测代码的输出结果。真正容易踩坑的地方包括：**浮点数精度、两种除法运算符的区别、负数取模的行为，以及 `bool` 实际上是 `int` 的子类**。

本节将介绍：

- `int`
- `float`
- `complex`
- 除法和幂运算符
- 数值字面量
- 类型转换函数
- 舍入（rounding）

---

### `int` 没有固定大小

Python 的整数是**任意精度（arbitrary precision）**的。

整数会根据你存储的数值自动扩展，因此不会像 C 或 Java 中的固定宽度整数那样发生溢出，也不存在类似 `long` 和 `int` 的区分。

```python
print(2 ** 100)   # 1267650600228229401496703205376
print(10 ** 50)   # 一个 51 位整数，不会溢出
```

这就是为什么在 Python 中计算阶乘（factorial）和非常大的幂时可以直接正常工作。

唯一真正的限制是**可用内存**。

---

### `float` 使用 IEEE-754，因此并不精确

Python 的 `float` 是 **64 位双精度浮点数（double）**。

很多十进制小数无法用二进制精确表示，因此会出现经典的浮点数精度问题：

```python
print(0.1 + 0.2)          # 0.30000000000000004
print(0.1 + 0.2 == 0.3)   # False
```

不要直接使用 `==` 比较浮点数。

应该使用：

```python
math.isclose(a, b)
```

另外需要注意：

```python
0.1 + 0.2
```

实际上比：

```python
0.3
```

稍微大一点，而不是稍微小一点，因此一些简单粗暴的舍入方式仍然可能产生意外结果。

---

### `bool` 是 `int` 的子类

在 Python 中：

```python
True
```

相当于：

```python
1
```

而：

```python
False
```

相当于：

```python
0
```

并且 `bool` 继承自 `int`。

因此，布尔值可以参与算术运算，也可以作为索引使用。

```python
print(True + True)            # 2
print(True == 1)              # True
print(isinstance(True, int))  # True
print(sum([True, False, True]))  # 2
```

对布尔值使用 `sum()` 是一种常见的统计 `True` 数量的方法。

不过需要记住：

```python
1 == True
0 == False
```

这在使用字典键时可能会产生意外结果。

例如：

```python
{1: "a", True: "b"}
```

最终会合并成一个键，因为：

```python
1 == True
```

---

### 除法：`/` 总是返回 `float`，`//` 向下取整

普通除法 `/` 总是返回 `float`，即使计算结果是整数。

而整除运算符 `//` 会向**负无穷方向取整（floor）**，而不是简单地向 `0` 截断。

```python
print(7 / 2)     # 3.5
print(6 / 2)     # 3.0  （是 float，不是 int）
print(7 // 2)    # 3
print(-7 // 2)   # -4   （向下取整，不是截断成 -3）
```

例如：

```python
-7 / 2
```

结果是：

```text
-3.5
```

因为 `//` 是向负无穷方向取整：

```text
-4 < -3.5 < -3
```

所以：

```python
-7 // 2 == -4
```

这个结果经常会让习惯 C 风格“向零截断”的人感到意外。

---

### 负数取模 `%`：结果的符号跟随除数

Python 中 `%` 的结果符号跟随**除数（divisor）**。

Python 始终满足下面这个关系：

```python
(a // b) * b + (a % b) == a
```

例如：

```python
print(7 % 3)     # 1
print(-7 % 3)    # 2   （符号跟随除数 3）
print(7 % -3)    # -2  （符号跟随除数 -3）
```

这与 C 和 Java 不同，在 C 和 Java 中，`%` 的结果符号通常跟随被除数。

`divmod(a, b)` 可以一次性返回：

```python
(a // b, a % b)
```

例如：

```python
print(divmod(-7, 3))   # (-3, 2)
```

---

### 幂运算符 `**` 与运算优先级

`**` 的优先级高于一元负号（unary minus），并且 `**` 是**右结合（right-associative）**的。

```python
print(2 ** 3 ** 2)   # 512  （2 ** (3 ** 2)），不是 64
print(-2 ** 2)       # -4   （-(2 ** 2)），不是 4
print((-2) ** 2)     # 4
```

例如：

```python
2 ** 3 ** 2
```

会被解释成：

```python
2 ** (3 ** 2)
```

也就是：

```python
2 ** 9
```

结果：

```text
512
```

而不是：

```python
(2 ** 3) ** 2
```

另外，**负数底数 + 小数指数**可能会得到 `complex`（复数）结果。

例如：

```python
(-8) ** (1/3)
```

---

### 数值字面量

Python 允许使用下划线 `_` 作为数字分隔符，也支持使用不同的前缀表示不同进制的整数。

这些数值在解析之后，本质上都只是普通的 `int`。

```python
print(1_000_000)   # 1000000
print(0xFF)        # 255  （十六进制）
print(0o17)        # 15   （八进制）
print(0b1010)      # 10   （二进制）
print(1e3)         # 1000.0  （float，注意 .0）
```

其中：

- `0x` → 十六进制（hexadecimal）
- `0o` → 八进制（octal）
- `0b` → 二进制（binary）

需要特别注意：

```python
1e3
```

虽然数学结果是整数 `1000`，但因为使用了**科学计数法（exponent notation）**，所以它是一个 `float`：

```python
print(type(1e3))
# <class 'float'>
```

---

### 类型转换与舍入

`int()` 是**向零截断（truncate toward zero）**，它并不是四舍五入。

```python
print(int(3.9))    # 3   （截断，不是舍入）
print(int(-3.9))   # -3  （向 0 截断）
```

而 `round()` 使用的是所谓的 **banker's rounding（银行家舍入）**。

当数字正好位于两个整数中间时，会舍入到**最近的偶数**。

```python
print(round(2.5))  # 2   （舍入到偶数）
print(round(3.5))  # 4   （舍入到偶数）
print(round(0.5))  # 0
```

所以：

```text
2.5 → 2
3.5 → 4
4.5 → 4
5.5 → 6
```

`round(x)` 没有第二个参数时返回 `int`：

```python
round(3.5)
# 4
```

而：

```python
round(x, n)
```

指定小数位数时返回 `float`：

```python
round(3.14159, 2)
# 3.14
```

银行家舍入的目的是在大量数值进行舍入时减少累计偏差（cumulative bias），但对于习惯了传统“四舍五入”的人来说：

```python
round(2.5) == 2
```

往往会比较令人意外。