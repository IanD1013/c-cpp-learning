# Python 中的类型注解

Python 是一种动态类型语言——你永远*不必*声明变量或参数的类型。但从 Python 3.5 开始，你可以向代码中添加**类型提示**（type hints，也称为类型注解，type annotations）。这些提示可以作为实时文档，向其他开发者明确表达你的意图，并使强大的静态分析工具（如 **mypy**）能够在代码运行之前捕获错误。

关键理解：**Python 在运行时会完全忽略类型提示。** 它们对代码的执行没有任何影响。你可以将参数注解为 `int`，然后传入一个 `str`——Python 不会报错。这是一种深思熟虑的设计选择，既保持了 Python 的灵活性，又提供了可选的安全保障。

## 工作原理

类型注解使用两处语法：

- 在参数名或变量名后使用**冒号 (`:`)** 来声明其类型
- 在结束 `def` 行的冒号前使用**箭头 (`->`)** 来声明返回类型

当 Python 遇到这些注解时，会将它们存储在函数对象上一个特殊的 `__annotations__` 字典中。它**不会**使用它们来验证参数或返回值。

## 语法

```python
# 带有参数和返回类型注解的函数
def function_name(param1: type1, param2: type2) -> return_type:
    ...

# 变量注解
my_var: int = 42
greeting: str = "hello"
is_active: bool = True

# 无返回值的函数
def log_message(msg: str) -> None:
    print(msg)
```

你可以直接使用的基本内置类型包括：`int`、`str`、`float`、`bool` 和 `None`。

## 示例

```python
# 用于计算面积的带注解函数
def rectangle_area(width: float, height: float) -> float:
    return width * height

# 注解存储在 __annotations__ 中
print(rectangle_area.__annotations__)
# {'width': <class 'float'>, 'height': <class 'float'>, 'return': <class 'float'>}

# 返回格式化字符串的带注解函数
def format_price(item: str, price: float) -> str:
    return f"{item}: ${price:.2f}"

print(format_price("Coffee", 4.5))  # "Coffee: $4.50"

# 演示提示在运行时不会被强制执行
def multiply(a: int, b: int) -> int:
    return a * b

# 尽管类型不匹配，但这依然可以正常运行！
result = multiply("ha", 3)  # 返回 "hahaha" —— 没有错误！
```

## 在运行时检查注解

每个带有注解的函数都有一个 `__annotations__` 属性——一个将参数名（以及表示返回类型的 `"return"`）映射到其注解类型的字典：

```python
def greet(name: str) -> str:
    return f"Hello, {name}"

print(greet.__annotations__)
# {'name': <class 'str'>, 'return': <class 'str'>}

# 其值是实际的类型对象，而不是字符串
print(greet.__annotations__["name"] is str)  # True
```

## 证明类型提示不会被强制执行

因为 Python 在运行时会忽略注解，所以传入“错误”的类型通常仍然有效——只要函数内部的操作支持该类型即可：

```python
def add_numbers(a: int, b: int) -> int:
    return a + b

# 传入字符串可以正常工作，因为 + 会拼接字符串
result = add_numbers("foo", "bar")  # "foobar" —— 没有 TypeError！
```

这有力地证明了为什么注解只是*提示*（hints）而不是*约束*（constraints）。`try/except` 块可以确认调用是成功还是引发错误。