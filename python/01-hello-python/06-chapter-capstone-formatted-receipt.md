# Python 基础综合：`print()`、算术、字符串拼接、`str()` 与注释

## 1. 核心目标

实际程序经常需要把**文字说明**和**计算结果**一起输出，例如：

```text
Total: 6
Distance: 180 km
Area: 20
```

这会同时涉及：

* `print()`：输出内容
* 算术运算：计算结果
* 字符串拼接 `+`：连接文本
* `str()`：把数字转换成字符串
* `#`：添加代码注释

---

## 2. `print()` 可以直接输出计算结果

`print()` 中可以放数学表达式：

```python
print(3 * 2)
```

Python 会先计算：

```text
3 * 2 → 6
```

然后输出：

```text
6
```

常见算术运算符：

| 运算符 | 含义 | 示例      | 结果    |
| --- | -- | ------- | ----- |
| `+` | 加法 | `3 + 2` | `5`   |
| `-` | 减法 | `3 - 2` | `1`   |
| `*` | 乘法 | `3 * 2` | `6`   |
| `/` | 除法 | `6 / 2` | `3.0` |

---

## 3. 字符串拼接要求两边都是字符串

两个字符串可以使用 `+` 连接：

```python
print("Hello " + "Python")
```

输出：

```text
Hello Python
```

但下面的代码会报错：

```python
print("Result: " + 42)
```

原因是：

```python
"Result: "   # str
42           # int
```

Python 不会自动把整数 `42` 转换成字符串。

因此会出现 `TypeError`。

---

## 4. 使用 `str()` 把数字转换成字符串

`str()` 可以把其他值转换成字符串形式：

```python
str(42)
```

得到：

```python
"42"
```

因此：

```python
print("Result: " + str(42))
```

实际上相当于：

```python
print("Result: " + "42")
```

输出：

```text
Result: 42
```

### 基本模式

```python
"text" + str(value)
```

例如：

```python
print("Age: " + str(35))
```

输出：

```text
Age: 35
```

---

## 5. `str()` 可以直接包住算术表达式

例如：

```python
print("Total: " + str(3 * 2))
```

理解这行代码时，可以从内到外看。

第一步，计算：

```python
3 * 2
```

得到：

```python
6
```

第二步：

```python
str(6)
```

得到字符串：

```python
"6"
```

第三步：

```python
"Total: " + "6"
```

得到：

```python
"Total: 6"
```

最后：

```python
print("Total: 6")
```

所以输出：

```text
Total: 6
```

可以把整个过程理解为：

```text
3 * 2
  ↓
  6
  ↓
str(6)
  ↓
"6"
  ↓
"Total: " + "6"
  ↓
"Total: 6"
```

---

## 6. 使用变量进行计算

实际程序通常不会把所有数字直接写死，而是使用变量：

```python
speed = 60
hours = 3

print("Distance: " + str(speed * hours) + " km")
```

计算过程：

```text
speed * hours
→ 60 * 3
→ 180
```

然后：

```python
str(180)
```

变成：

```python
"180"
```

最终输出：

```text
Distance: 180 km
```

---

## 7. 注释的正确使用方式

Python 的普通注释使用 `#`：

```python
# Calculate the total price
print("Total: " + str(3 * 2))
```

也可以写在代码后：

```python
print("Total: " + str(3 * 2))  # 3 apples × 2 dollars
```

`#` 后面的内容不会被 Python 当作代码执行。

### 注意：三引号不是普通的多行注释语法

下面的内容：

```python
"""
Some text
"""
```

本质上是一个**多行字符串**，不是 Python 专门的注释语法。

三引号常用于编写文档字符串（docstring）：

```python
def calculate_total():
    """Calculate and display the total price."""
```

如果只是普通代码注释，更推荐：

```python
# Calculate the total price
# and display it to the user.
```

---

## 8. 综合示例：计算矩形面积

```python
# Calculate the area of a rectangle
width = 5
height = 4

print("Area: " + str(width * height))
```

输出：

```text
Area: 20
```

这里同时使用了：

```text
#                  → 注释
width * height     → 算术运算
str(...)           → 数字转字符串
+                  → 字符串拼接
print(...)         → 输出结果
```

---

## 9. 常见错误

### 错误 1：字符串直接加整数

错误：

```python
print("Total: " + 6)
```

因为：

```text
str + int
```

不能直接拼接。

正确：

```python
print("Total: " + str(6))
```

---

### 错误 2：把计算表达式放进引号

```python
print("3 * 2")
```

输出：

```text
3 * 2
```

因为引号中的内容只是普通文字。

如果希望 Python 真正计算：

```python
print(3 * 2)
```

输出：

```text
6
```

---

### 错误 3：把计算结果直接写死

例如：

```python
print("Total: " + str(6))
```

虽然输出正确，但如果要求程序真正计算：

```text
3 × 2
```

就应该写：

```python
print("Total: " + str(3 * 2))
```

这样数字 `6` 是程序计算出来的，而不是手动写进去的。

---

## 10. 更现代的字符串格式化方式

在实际 Python 项目中，更常使用 **f-string（格式化字符串）**：

```python
price = 2
quantity = 3

print(f"Total: {price * quantity}")
```

输出：

```text
Total: 6
```

f-string 会自动把表达式结果转换成适合显示的文本，因此不需要手动调用 `str()`。

不过在学习字符串类型和字符串拼接时：

```python
"Total: " + str(3 * 2)
```

仍然非常重要，因为它清楚展示了：

> 数字和字符串是不同的数据类型，使用 `+` 拼接之前必须进行类型转换。

---

# 练习解答：打印商店收据

## 问题

编写函数：

```python
def print_receipt():
```

调用后必须准确输出：

```text
--- Receipt ---
Apples: 3
Price per apple: 2
Total: 6
--- Thank You ---
```

要求：

1. `Total` 必须通过 `3 * 2` 计算，不能直接写死 `6`
2. `Total` 行必须使用字符串拼接 `+`
3. 必须使用 `str()` 转换计算结果
4. 至少包含一个 `#` 注释

## 正确实现

```python
def print_receipt():
    # Print a simple apple receipt
    print("--- Receipt ---")
    print("Apples: 3")
    print("Price per apple: 2")
    print("Total: " + str(3 * 2))
    print("--- Thank You ---")
```

调用函数：

```python
print_receipt()
```

输出：

```text
--- Receipt ---
Apples: 3
Price per apple: 2
Total: 6
--- Thank You ---
```

## 关键代码解释

最重要的一行是：

```python
print("Total: " + str(3 * 2))
```

执行顺序：

```text
3 * 2
→ 6

str(6)
→ "6"

"Total: " + "6"
→ "Total: 6"

print(...)
→ 输出 Total: 6
```

因此，这一行同时练习了：

```text
3 * 2       → 算术运算
str(...)    → 类型转换
+           → 字符串拼接
print(...)  → 输出
```

这也是把 Python 基础知识组合起来使用的一个典型例子。
