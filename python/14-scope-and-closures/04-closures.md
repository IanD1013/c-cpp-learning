### Python 中的闭包

**闭包**（closure）是指即使外部函数已经执行完毕并返回，该函数仍然能够“记住”其外部作用域中变量的函数。每当嵌套函数引用了外层函数中定义的变量时，就会发生这种情况 —— Python 会保持该变量处于活动状态，并将其与内部函数打包在一起作为一个闭包。闭包是 Python 中最强大的工具之一，用于从通用模板创建专门的、可重用的函数。

### 工作原理

当你在一个函数内部定义另一个函数，且内部函数使用了外部函数的变量时，外部函数返回后 Python 并不会丢弃该变量。相反，它会将该变量附加到内部函数对象的一个名为 `__closure__` 的特殊属性中。每次调用外部函数时，你都会得到一个**新的**内部函数，它拥有自己**独立**的外层变量快照。这意味着每个闭包都是独立的 —— 除非你刻意为之，否则它们不会共享状态。

你可以把它想象成一个工厂：外部函数是一个生产专门工人（内部函数）的工厂。每个工人都记得在创建时收到的指令（外层变量）。

### 语法

```python
def outer_function(parameter):
    # parameter is "enclosed" — it will be remembered
    def inner_function(arg):
        # inner_function can use parameter freely
        return some_operation(parameter, arg)
    return inner_function  # return the function itself, not a call to it
````

你也可以使用 `lambda` 来编写简洁的闭包：

```python
def outer_function(parameter):
    return lambda arg: some_operation(parameter, arg)
```

### 示例

```python
# Example 1: A greeting factory
def make_greeter(greeting):
    def greet(name):
        return f"{greeting}, {name}!"
    return greet

hello = make_greeter("Hello")
bonjour = make_greeter("Bonjour")

hello("Alice")     # "Hello, Alice!"
bonjour("Alice")   # "Bonjour, Alice!"
# Each closure remembers its own greeting

# Example 2: A power function factory
def make_power(exponent):
    return lambda base: base ** exponent

square = make_power(2)
cube = make_power(3)

square(5)   # 25
cube(5)     # 125
# square and cube are independent closures with different exponents

# Example 3: An adder factory
def make_adder(offset):
    def add(value):
        return value + offset
    return add

add_ten = make_adder(10)
add_hundred = make_adder(100)

add_ten(5)       # 15
add_hundred(5)   # 105
```

### 为什么闭包很重要

闭包允许你**参数化行为**。你无需在每次调用函数时都传递配置参数，而是一次性将其融入其中，并获取一个专门的函数。这种模式在 Python 中随处可见：

* **Decorators** 是包装其他函数的闭包
* **Callback factories** 用于创建具有预配置上下文的事件处理程序
* **Partial application** 可针对特定用例将通用函数特化
