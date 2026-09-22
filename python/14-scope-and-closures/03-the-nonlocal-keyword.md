### 嵌套函数与 `nonlocal` 关键字

在 Python 中，你可以在其他函数内部定义函数。这些被称为**嵌套函数**（或内部函数）。嵌套函数可以访问其外层函数作用域中的变量——这被称为**闭包**。虽然嵌套函数可以自由地*读取*外层作用域中的变量，但*修改*它们需要使用一个特殊的关键字：`nonlocal`。

这种模式非常强大，可以在不使用类或全局变量的情况下创建带有私有状态的函数——比如计数器、累加器或自定义生成器。

### 工作原理

当 Python 在函数内部遇到变量赋值时，它会假定该变量是该函数的**局部变量**。如果嵌套函数尝试重新为其外层作用域中的变量赋值，Python 会抛出 `UnboundLocalError`，因为它认为你正在创建一个新的局部变量。

`nonlocal` 关键字告诉 Python：“这个变量不是局部的——去最近的外层作用域中查找它。”它在概念上类似于 `global`，但它查找的是**外层函数**的作用域，而不是模块级作用域。

### 语法

```python
def outer_function():
    some_variable = initial_value

    def inner_function():
        nonlocal some_variable
        some_variable = new_value  # modifies the enclosing scope's variable
        return some_variable

    return inner_function
```

### 示例

```python
# Example 1: A greeting function that remembers how many times it's been called
def make_greeter(name):
    times_greeted = 0

    def greet():
        nonlocal times_greeted
        times_greeted += 1
        return f"Hello {name}! (greeted {times_greeted} times)"

    return greet

hello_alice = make_greeter("Alice")
print(hello_alice())  # Hello Alice! (greeted 1 times)
print(hello_alice())  # Hello Alice! (greeted 2 times)


# Example 2: An accumulator that keeps a running total
def make_accumulator(start=0):
    total = start

    def add(value):
        nonlocal total
        total += value
        return total

    return add

acc = make_accumulator()
print(acc(10))   # 10
print(acc(25))   # 35
print(acc(5))    # 40


# Example 3: Without nonlocal — reading works, but modifying fails
def outer():
    message = "original"

    def inner_read():
        return message  # Reading works fine

    def inner_modify():
        # message += "!"  # This would raise UnboundLocalError!
        nonlocal message
        message += "!"
        return message

    print(inner_read())    # "original"
    print(inner_modify())  # "original!"
```

### 关键区别：`global` vs `nonlocal`

| **关键字目标作用域使用场景** |            |            |
| ---------------- | ---------- | ---------- |
| `global`         | 模块级（顶层）作用域 | 在模块级别修改变量  |
| `nonlocal`       | 外层函数的作用域   | 在外层函数中修改变量 |

### 为什么闭包很重要

闭包允许你创建**具有记忆功能的函数**。对外层函数的每次调用都会创建一个全新的、独立的作用域。这意味着你可以创建多个独立的实例：

```python
acc1 = make_accumulator(100)
acc2 = make_accumulator(0)

print(acc1(5))   # 105
print(acc2(5))   # 5  — completely independent!
```
