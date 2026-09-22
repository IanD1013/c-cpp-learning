### 隐式返回 `None` 的函数

在 Python 中，每个函数都会返回一个值——即使你没有写 `return` 语句。如果一个函数缺少显式的 `return`，或者使用了后面不带任何值的 `return`，Python 会静默地返回 `None`。理解这种行为非常重要，因为它是常见的 bug 来源：你调用一个函数期望得到结果，但实际得到的却是 `None`。

返回 `None` 的函数通常用于**副作用**（side effects）——例如打印到屏幕、写入文件或原地修改可变对象等操作。而计算并返回结果的函数则使用带有值的 `return`。

### 工作原理

当 Python 到达函数体末尾而未遇到 `return` 语句时，其行为就如同最后一行是 `return None`。如果你单独写一个 `return`，效果也是一样的。

```python
def say_hi():
    print("Hi!")

result = say_hi()   # Prints "Hi!" to the console
print(result)        # Prints: None
print(type(result))  # Prints: <class 'NoneType'>
```

请注意，`print()` 本身就是一个返回 `None` 的函数。它将文本输出到控制台作为副作用，但不会生成可以存储或传递的值。

### 返回值与副作用

对比以下两种计算平方的方法：

```python
# Side-effect only — prints the result, returns None
def show_square(n):
    print(n * n)

# Returns the result — caller can use it however they want
def compute_square(n):
    return n * n
```

```python
a = show_square(5)     # Prints 25 to console, a is None
b = compute_square(5)  # Nothing printed, b is 25

# With a returned value, you can chain operations:
total = compute_square(3) + compute_square(4)  # total is 25
```

带有**返回值**的函数更具灵活性——调用者可以决定如何处理该结果：存储它、传递给另一个函数、包含在表达式中，甚至打印它。

### 使用 f-string 构建字符串

当函数需要生成字符串时，使用带有 `return` 的 f-string：

```python
def describe_temp(city, degrees):
    return f"It is {degrees}°C in {city}."

weather = describe_temp("Oslo", -3)
print(weather)  # It is -3°C in Oslo.
```

关键区别在于：`describe_temp` **返回**了字符串。随后调用者选择将其打印出来。如果我们是在函数内部使用 `print()` 而不是 `return`，该函数虽然会输出文本，但返回的却是 `None`。

### 常见错误

```python
def make_label(item, qty):
    print(f"{qty}x {item}")  # Side effect only!

label = make_label("Bolt", 50)  # Prints "50x Bolt"
print(label)                     # Prints: None  <-- oops!
```

如果你希望 `make_label` 生成一个可用的字符串，你需要使用 `return` 而不是 `print`。
