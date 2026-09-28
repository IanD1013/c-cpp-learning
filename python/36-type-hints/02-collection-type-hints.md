### 集合的类型提示

基础类型注解允许你使用诸如 `int`、`str` 和 `bool` 等类型来为变量和参数添加注解。但对于列表（list）、字典（dict）、元组（tuple）和集合（set）呢？Python 3.9+ 允许你**直接使用内置集合类型**作为泛型类型提示，指定它们包含的元素类型。这使你的代码具有自解释性，并有助于工具捕获意外混淆元素类型的错误。

### 工作原理

在 Python 3.9 之前，你需要从 `typing` 模块进行特殊导入：

```python
from typing import List, Dict, Tuple, Set

def old_style(items: List[int]) -> Dict[str, int]: ...
```

从 Python 3.9 开始，**小写的内置类型**可以直接作为泛型使用：

```python
# No imports needed!
def new_style(items: list[int]) -> dict[str, int]: ...
```

现在相较于从 `typing` 导入，**更推荐**使用小写形式（`list`、`dict`、`tuple`、`set`）。

### 语法

```python
# List of a single type
def process(values: list[int]) -> list[str]: ...

# Dictionary with key and value types
def lookup(registry: dict[str, float]) -> dict[str, bool]: ...

# Fixed-length tuple with specific types per position
def get_record() -> tuple[int, str, float]: ...

# Variable-length tuple (all elements same type)
def get_ids() -> tuple[int, ...]: ...

# Set of a single type
def unique_words(text: str) -> set[str]: ...

# Nested collections
def grouped(data: dict[str, list[float]]) -> list[tuple[str, float]]: ...
```

### 示例

```python
# A function that merges two lists of strings
def merge_names(first: list[str], second: list[str]) -> list[str]:
    return first + second

# A function returning a dict mapping names to scores
def build_scoreboard(names: list[str], scores: list[int]) -> dict[str, int]:
    return dict(zip(names, scores))

# A function returning a fixed tuple: (min, max, mean)
def summarize(values: list[float]) -> tuple[float, float, float]:
    return (min(values), max(values), sum(values) / len(values))

# Nested types: dict of string keys to lists of floats
def scale_all(groups: dict[str, list[float]], factor: float) -> dict[str, list[float]]:
    return {k: [v * factor for v in vals] for k, vals in groups.items()}
```

### 在运行时检查类型注解

Python 将类型提示存储在函数的 `__annotations__` 字典中。这对于内省（introspection）、文档生成器以及运行时验证框架非常有用。

```python
def greet(name: str, times: int) -> str:
    return (name + "! ") * times

print(greet.__annotations__)
# {'name': <class 'str'>, 'times': <class 'int'>, 'return': <class 'str'>}
```

当你使用像 `list[int]` 这样的泛型类型添加注解时，注解存储的是**参数化类型对象（parameterized type object）**，而不仅仅是 `list`：

```python
def double_all(nums: list[int]) -> list[int]:
    return [n * 2 for n in nums]

print(double_all.__annotations__)
# {'nums': list[int], 'return': list[int]}
```