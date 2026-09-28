# Python 中的 Callable 类型与 TypeVar

在 Python 中，函数是一等公民（first-class objects）——你可以将它们作为参数传递、作为返回值返回，以及存储在变量中。但是你该如何为期望接收一个*函数*的参数添加类型注解呢？又该如何表达一个泛型函数的返回类型取决于传入的参数类型？这就是 `Callable` 和 `TypeVar` 大显身手的地方。这些工具让你可以为高阶函数（即接收或返回其他函数的函数）编写精确且具备自文档化特性的类型提示（type hints）。

## Callable 的工作原理

`Callable` 来自 `typing`（或 `collections.abc`），用于描述函数参数的*签名*（signature）：

- `Callable[[ParamType1, ParamType2], ReturnType]` — 接收特定参数类型并返回特定类型的函数。
- `Callable[..., ReturnType]` — 任何返回 `ReturnType` 的可调用对象（callable），不论其参数是什么。

当你将参数注解为 `Callable` 时，Python 不会在运行时强制执行它，但它能向开发者和类型检查器清晰地传达意图。

## TypeVar 的工作原理

`typing` 中的 `TypeVar` 用于创建一个*类型变量*（type variable）——一个将输入和输出类型关联起来的占位符：

```python
from typing import TypeVar

T = TypeVar('T')
```

当你在函数签名中使用 `T` 时，它意味着“无论这里传入什么类型，输出的都是相同的类型”。这就是在 Python 中表达*泛型*（generic）函数的方式。

## 语法

```python
from typing import TypeVar, Callable

# Callable annotation
def run_operation(x: int, op: Callable[[int], str]) -> str:
    return op(x)

# TypeVar for generic functions
T = TypeVar('T')

def identity(value: T) -> T:
    return value
```

## 示例

```python
from typing import Callable, TypeVar

T = TypeVar('T')

# Example 1: A function that takes a formatter callable
def format_items(items: list[str], formatter: Callable[[str], str]) -> list[str]:
    return [formatter(item) for item in items]

# Usage: passing a lambda as the callable
result = format_items(["hello", "world"], lambda s: s.upper())
# result -> ["HELLO", "WORLD"]

# Example 2: A generic function using TypeVar
def last_element(items: list[T]) -> T:
    return items[-1]

# The return type matches the element type of the input
num = last_element([10, 20, 30])    # inferred as int
word = last_element(["a", "b"])     # inferred as str

# Example 3: Checking if something is callable
print(callable(len))          # True
print(callable(42))           # False
print(callable(lambda x: x))  # True
```

## 检查注解

每个带有注解的函数都会将其类型提示存储在 `__annotations__` 属性中：

```python
def greet(name: str) -> str:
    return f"Hi, {name}"

print(greet.__annotations__)
# {'name': <class 'str'>, 'return': <class 'str'>}
```

请注意，`return` 是用于返回类型注解的特殊键。字典的值是实际的类型对象（例如 `str`、`list`、`Callable`）。

## 常见模式

| **模式** | **含义** |
| :--- | :--- |
| `Callable[[int], bool]` | 接收 int 并返回 bool 的函数 |
| `Callable[..., str]` | 返回 str 的任意可调用对象 |
| `callable(obj)` | 检查 `obj` 是否可调用的内置函数 |
| `T = TypeVar('T')` | 声明一个泛型类型变量 |
| `func.__annotations__` | 函数类型注解的字典 |