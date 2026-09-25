### 函数作为一等对象（First-Class Objects）

在 Python 中，函数是**一等对象**（first-class objects）。这意味着它们与其他任何值（如 `int`、`str`、`list`）受到的待遇完全相同。你可以将它们赋值给变量、存储在字典中、作为参数传递给其他函数，以及从函数中返回它们。正是这一特性使得 Python 如此灵活且富有表现力，它也是装饰器（decorators）、回调模式（callback patterns）以及函数式编程惯用法（functional programming idioms）的概念基石。

### 工作原理

当你使用 `def` 定义一个函数时，Python 会创建一个函数对象并将其绑定到你选择的名称上。该名称只是一个变量，它指向内存中的函数对象。如果你引用该名称时**不带括号**，你得到的就是函数对象本身。如果加上**括号**，你就会*调用*它。

```python
def shout(text):
    return text.upper()

# 'shout' is a variable pointing to a function object
print(type(shout))  # <class 'function'>

# Assign the function to another variable, no parentheses!
yell = shout
print(yell("hello"))  # HELLO
```

请注意，`yell` 和 `shout` 现在都指向**同一个**函数对象。这里没有进行任何复制，它们只是同一个对象的两个不同名称。

### 将函数存储在数据结构中

因为函数是普通对象，所以你可以将它们放在可以放置其他任何值的地方：

```python
import math

operations = {
    "negate": lambda x: -x,
    "double": lambda x: x * 2,
    "sqrt": math.sqrt,
}

for name, op in operations.items():
    print(f"{name}(16) = {op(16)}")
# negate(16) = -16
# double(16) = 32
# sqrt(16) = 4.0
```

### 将函数作为参数传递

接受另一个函数作为参数的函数称为**高阶函数**（higher-order function）。你之前已经使用过以这种方式工作的内置函数，比如 `map`、`filter` 和 `sorted`。你自己编写一个也同样简单：

```python
def transform(operation, items):
    return [operation(item) for item in items]

prices = [19.99, 5.50, 42.00]
discounted = transform(lambda p: round(p * 0.9, 2), prices)
print(discounted)  # [17.99, 4.95, 37.8]
```

在这里，`transform` 并不知道也不关心 `operation` *具体做什么*，它只是对每个元素调用该操作。调用者通过传入一个特定的函数来决定其行为。

### 从函数中返回函数

函数还可以**创建并返回**新的函数：

```python
def make_multiplier(factor):
    def multiplier(x):
        return x * factor
    return multiplier

triple = make_multiplier(3)
print(triple(10))  # 30
print(triple(7))   # 21
```

内部函数 `multiplier` 会“记住”来自其封闭作用域的 `factor`，这就是**闭包**（closure）。返回的函数是一个完全独立的可调用对象。

### 组合函数调用

因为函数调用会返回一个值，而函数又可以接收任何值，所以你可以自然地进行**链式**或**嵌套**调用：

```python
def add_one(n):
    return n + 1

# Nested calls: inner call runs first, its result feeds the outer call
result = add_one(add_one(5))  # add_one(6) -> 7
```

这种将函数应用于前一次应用结果的模式，就是函数组合（function composition）背后的核心思想。
