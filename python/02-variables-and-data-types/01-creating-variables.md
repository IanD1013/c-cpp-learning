# Python 变量（Variables）

## 1. 什么是变量

**变量（variable）**是一个用来保存值的名字。

例如：

```python
city = "Paris"
year = 2025
```

这里：

* `city` 是变量名，保存字符串 `"Paris"`
* `year` 是变量名，保存整数 `2025`

之后可以直接通过变量名使用这些值：

```python
print(city)
print(year)
```

输出：

```text
Paris
2025
```

变量的价值在于：**给数据一个有意义的名字，让数据可以重复使用，也让代码更容易理解和修改。**

例如：

```python
price = 10

print(price)
print(price * 2)
print(price + 5)
```

不需要每次都重新写 `10`。

---

## 2. 使用 `=` 给变量赋值

Python 使用 `=` 进行**赋值（assignment）**：

```python
变量名 = 值
```

例如：

```python
favorite_color = "blue"
temperature = 72
```

可以理解为：

> 把右边的值保存到左边这个名字所代表的变量中。

因此：

```python
age = 20
```

表示让变量 `age` 保存整数 `20`。

### `=` 不是数学里的“等于”

在 Python 中：

```python
age = 20
```

不是在判断：

> age 是否等于 20？

而是在执行：

> 把 20 赋值给 age。

判断两个值是否相等使用的是：

```python
==
```

例如：

```python
age == 20
```

---

## 3. 变量可以重新赋值

变量保存的值可以发生变化：

```python
score = 10
print(score)

score = 20
print(score)
```

输出：

```text
10
20
```

第二次：

```python
score = 20
```

会让 `score` 开始引用新的值 `20`。

---

## 4. Python 变量名规则

Python 中的变量名属于**标识符（identifier）**，需要遵守以下规则。

### 可以使用

变量名可以包含：

* 字母
* 数字
* 下划线 `_`

例如：

```python
age = 20
player_1 = "Alice"
first_name = "Tom"
_secret = "hello"
```

### 不能以数字开头

错误：

```python
2fast = 100
```

正确：

```python
fast2 = 100
```

---

### 不能包含空格

错误：

```python
first name = "Tom"
```

正确：

```python
first_name = "Tom"
```

Python 通常使用 **snake_case（蛇形命名法）**：

```python
first_name = "Tom"
total_price = 100
user_age = 25
```

---

### 大小写敏感

Python 区分大小写：

```python
score = 10
Score = 20
```

这里是两个不同的变量。

因此通常应保持统一的命名风格，避免：

```python
userName
Username
username
```

混合使用。

---

### 不能使用 Python 关键字

例如下面是错误的：

```python
class = "Math"
for = 10
if = True
```

因为 `class`、`for`、`if` 都是 Python 语言本身使用的关键字。

> `print` 并不是 Python 关键字，而是一个内置函数。但仍然不应该用它作为变量名，否则会覆盖原来的 `print()` 函数。

例如不要这样写：

```python
print = "hello"
```

否则之后：

```python
print("Hi")
```

可能会报错。

---

## 5. 推荐使用有意义的变量名

不推荐：

```python
x = "Paris"
y = 2025
```

更推荐：

```python
city = "Paris"
year = 2025
```

好的变量名应该能够说明这个值代表什么。

例如：

```python
user_name = "Alice"
product_price = 99
temperature = 25
```

---

## 6. 变量可以保存不同类型的数据

Python 变量可以保存很多不同类型的值。

### 字符串 `str`

```python
city = "Paris"
```

### 整数 `int`

```python
year = 2025
```

### 小数 `float`

```python
price = 19.99
```

### 布尔值 `bool`

```python
is_logged_in = True
```

变量本身并不是固定的数据类型容器，Python 会根据当前保存的值确定其类型。

例如：

```python
value = 10
value = "hello"
```

这是合法的，但实际程序中不应随意改变变量所表示的数据含义，否则代码会变得难以理解。

---

## 7. 在 `print()` 中使用变量

变量可以直接传给 `print()`：

```python
fruit = "mango"

print(fruit)
```

输出：

```text
mango
```

也可以参与字符串拼接：

```python
fruit = "mango"

print("I love " + fruit)
```

Python 实际执行的效果类似于：

```python
print("I love " + "mango")
```

输出：

```text
I love mango
```

---

## 8. 字符串与数字不能直接使用 `+` 拼接

假设：

```python
dog_name = "Rex"
dog_age = 5
```

下面可以正常运行：

```python
print("Name: " + dog_name)
```

因为两边都是字符串：

```text
"Name: " + "Rex"
```

但是下面会报错：

```python
print("Age: " + dog_age)
```

原因是：

```text
"Age: "    → str
5          → int
```

Python 不允许直接执行：

```python
str + int
```

也就是说，`+` 在这里要求两边都是字符串。

---

## 9. 使用 `str()` 把数字转换成字符串

`str()` 可以把一个值转换成字符串：

```python
str(5)
```

结果相当于：

```python
"5"
```

因此可以写：

```python
dog_age = 5

print("Age: " + str(dog_age))
```

执行过程可以理解为：

```python
dog_age
↓
5

str(dog_age)
↓
"5"

"Age: " + "5"
↓
"Age: 5"
```

最终输出：

```text
Age: 5
```

---

## 10. 完整示例

```python
country = "Japan"
population = 125000000

print("Country: " + country)
print("Population: " + str(population))
```

输出：

```text
Country: Japan
Population: 125000000
```

其中：

```python
country
```

本身就是字符串，因此可以直接拼接。

而：

```python
population
```

是整数，所以需要：

```python
str(population)
```

先转换成字符串。

---

## 11. `+` 的含义取决于数据类型

这是理解 Python 时很重要的一点。

对于数字：

```python
print(10 + 20)
```

执行的是**加法**：

```text
30
```

对于字符串：

```python
print("10" + "20")
```

执行的是**字符串拼接**：

```text
1020
```

因此：

```python
10 + 20
```

和：

```python
"10" + "20"
```

完全不同。

---

## 12. 常见错误

### 错误 1：忘记给字符串加引号

错误：

```python
city = Paris
```

Python 会把 `Paris` 当成变量名，而不是文字。

正确：

```python
city = "Paris"
```

---

### 错误 2：字符串和整数直接拼接

错误：

```python
year = 2025

print("Year: " + year)
```

正确：

```python
print("Year: " + str(year))
```

---

### 错误 3：变量名中使用空格

错误：

```python
favorite city = "Paris"
```

正确：

```python
favorite_city = "Paris"
```

---

### 错误 4：混淆赋值和比较

赋值：

```python
year = 2025
```

比较：

```python
year == 2025
```

记住：

```text
=   赋值
==  比较是否相等
```

---

## 13. 更现代的输出方式：f-string

学习字符串拼接时，经常会看到：

```python
city = "Paris"
year = 2025

print("City: " + city)
print("Year: " + str(year))
```

实际 Python 开发中，更常使用 **f-string（格式化字符串）**：

```python
print(f"City: {city}")
print(f"Year: {year}")
```

输出完全相同：

```text
City: Paris
Year: 2025
```

f-string 的优势是数字不需要手动使用 `str()`：

```python
year = 2025

print(f"Year: {year}")
```

不过在练习字符串拼接和 `str()` 时，仍应按照题目要求使用：

```python
"Year: " + str(year)
```

---

## 14. 核心知识总结

```python
city = "Paris"
year = 2025
```

可以拆成三个核心概念：

```text
变量名 = 值
```

变量可以直接使用：

```python
print(city)
```

字符串可以使用 `+` 拼接：

```python
print("City: " + city)
```

数字参与字符串拼接时，需要先转换：

```python
print("Year: " + str(year))
```

最重要的是理解：

```text
city = "Paris"
```

意味着：

> 创建变量 `city`，让它保存字符串 `"Paris"`。

而：

```python
year = 2025
```

意味着：

> 创建变量 `year`，让它保存整数 `2025`。

---

# 练习题解答：使用变量输出城市和年份

## 问题

编写一个 `solution()` 函数：

1. 创建变量 `city`，值为字符串 `"Paris"`
2. 创建变量 `year`，值为整数 `2025`
3. 使用字符串拼接输出：

```text
City: Paris
Year: 2025
```

第二行需要使用 `str()` 将整数 `year` 转换成字符串。

---

## 答案

```python
def solution():
    city = "Paris"
    year = 2025

    print("City: " + city)
    print("Year: " + str(year))
```

如果在普通 Python 文件中希望直接运行这个函数，还需要调用它：

```python
def solution():
    city = "Paris"
    year = 2025

    print("City: " + city)
    print("Year: " + str(year))


solution()
```

输出：

```text
City: Paris
Year: 2025
```

## 代码解析

```python
city = "Paris"
```

创建变量 `city`，保存字符串 `"Paris"`。

```python
year = 2025
```

创建变量 `year`，保存整数 `2025`。

```python
print("City: " + city)
```

因为 `city` 是字符串，所以可以直接拼接：

```text
"City: " + "Paris"
```

得到：

```text
City: Paris
```

第二行：

```python
print("Year: " + str(year))
```

`year` 是整数：

```python
2025
```

先通过：

```python
str(year)
```

转换成：

```text
"2025"
```

然后进行：

```text
"Year: " + "2025"
```

最终输出：

```text
Year: 2025
```

> 函数定义的正确 Python 语法是 `def solution():`，末尾必须有冒号 `:`。
