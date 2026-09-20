### F-Strings (Formatted String Literals)

Python f-strings 为你提供了一种简洁、易读的方式，可以直接在字符串中嵌入值和表达式。你无需再别扭地使用 `+` 拼接字符串并用 `str()` 转换类型，只需在以 `f` 为前缀的字符串花括号 `{}` 中放入变量和表达式即可。

### 工作原理

要创建 f-string，只需在开头的引号前加上字母 `f`（或 `F`）。在字符串内部，任何包裹在花括号 `{}` 中的内容都会作为 Python 表达式进行求值，并将其结果插入到字符串中。花括号之外的所有内容都将被视为普通文本字面量。

### 语法

```python
f"Some text {expression} more text"
```

`{}` 内部的表达式可以是变量、计算、函数调用，甚至可以是方法调用——即任何有效的 Python 表达式。

### 示例

```python
# Embedding a variable
city = "Paris"
print(f"Welcome to {city}!")  # Welcome to Paris!

# Embedding an arithmetic expression
width = 5
height = 3
print(f"Area: {width * height}")  # Area: 15

# Calling a method inside the braces
fruit = "banana"
print(f"Fruit: {fruit.upper()}")  # Fruit: BANANA

# Mixing multiple variables
first = "Jane"
last = "Doe"
year = 2024
print(f"{first} {last} joined in {year}.")  # Jane Doe joined in 2024.
```

### 为什么不使用字符串拼接？

对比一下旧方法与 f-string 方法：

```python
# Concatenation — clunky and error-prone
temp = 72
result = "The temperature is " + str(temp) + " degrees."

# F-string — clean and readable
result = f"The temperature is {temp} degrees."
```

使用拼接时，你必须使用 `str()` 手动转换非字符串类型。而 f-strings 会自动处理这一转换。