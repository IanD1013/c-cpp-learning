# 本章综合项目：作用域、闭包与工厂函数

这个综合项目汇总了你所学过的所有关于 Python 作用域规则的知识：局部与全局作用域、嵌套函数、`nonlocal` 关键字以及闭包。闭包是 Python 中最强大的模式之一——即使在外部函数执行完毕后，它也能让内部函数“记住”来自其封闭作用域的变量。这种模式是工厂函数、装饰器以及基于回调的设计的基础。

## 闭包的工作原理

当你在一个函数内部定义另一个函数时，内部函数可以访问外部函数的局部变量。通常情况下，局部变量会在函数返回时消失。但如果内部函数*引用*了这些变量，Python 会将它们保存在一个名为**闭包单元**（closure cell）的特殊对象中。内部函数无论传递到哪里，都会随身携带这个单元。

你可以把它想象成打包便当盒：外部函数准备食材（它的局部变量），而内部函数在离开时带走这个便当盒（闭包）。

## 语法

```python
def outer_function(param):
    # param lives in outer_function's local scope

    def inner_function(x):
        # inner_function can access 'param' via closure
        return x + param

    return inner_function  # Return the function itself, not a call
```

## 示例

```python
# Example 1: A greeting factory
def make_greeter(greeting):
    def greet(name):
        return f"{greeting}, {name}!"
    return greet

hello = make_greeter("Hello")
bonjour = make_greeter("Bonjour")

hello("Sam")      # "Hello, Sam!"
bonjour("Sam")    # "Bonjour, Sam!"
# Each returned function remembers its own 'greeting' value

# Example 2: A multiplier factory
def make_multiplier(factor):
    def multiply(n):
        return n * factor
    return multiply

double = make_multiplier(2)
triple = make_multiplier(3)

double(5)   # 10
triple(5)   # 15
# 'factor' is captured from the enclosing scope

# Example 3: A threshold checker
def make_threshold_checker(threshold):
    def exceeds(value):
        return value > threshold
    return exceeds

is_hot = make_threshold_checker(100)
is_hot(95)    # False
is_hot(105)   # True
```

## 涉及的核心概念

| **概念在本练习中的作用** |                                  |
| -------------- | -------------------------------- |
| **局部作用域**      | `min_val` 和 `max_val` 是外部函数的局部变量 |
| **嵌套函数**       | 验证函数定义在外部函数内部                    |
| **闭包**         | 内部函数捕获并记住范围边界                    |
| **工厂模式**       | 外部函数生成定制的内部函数                    |
