### Python 中的综合类型提示

Python 的类型提示系统远不止标注 `int` 和 `str` 这么简单。在实际的代码库中，你将使用**类型别名**来命名复杂类型，使用 **Union 类型**来表示可以是多种类型之一的值，使用 **Optional** 表示可为 null 的值，以及使用 **Callable** 来描述函数签名。将这些特性结合起来可以使代码具备自文档化能力，并帮助工具在运行前捕获错误。

### 类型别名如何工作

类型别名通过为复杂的类型表达式指定一个名称，使类型签名更具可读性：

```python
UserId = int
UserMap = dict[str, list[int]]
Handler = Callable[[str, int], bool]
```

现在可以在任何地方使用 `UserMap`，而无需重复编写 `dict[str, list[int]]`。

### Union 和 Optional 类型

当一个值可以是多种类型之一时，使用 `Union` 或 `|` 语法（Python 3.10+）：

```python
# These are equivalent:
value: Union[str, int] = "hello"
value: str | int = 42

# Optional[X] is shorthand for X | None
result: Optional[float] = None
result: float | None = 3.14
```

### Callable 类型提示

`Callable[[ParamTypes], ReturnType]` 用于描述函数的签名：

```python
from typing import Callable

# A function that takes two ints and returns a bool
Comparer = Callable[[int, int], bool]

def apply_comparison(a: int, b: int, cmp: Comparer) -> bool:
    return cmp(a, b)

# Usage:
is_greater: Comparer = lambda x, y: x > y
apply_comparison(5, 3, is_greater)  # True
```

### 带有类型提示的高阶函数

接收或返回其他函数的函数可以极大地受益于类型注解：

```python
from typing import Callable

Item = dict[str, str | int]
Validator = Callable[[Item], bool]

def select_items(items: list[Item], check: Validator) -> list[Item]:
    return [item for item in items if check(item)]

Transformer = Callable[[Item], Item | None]

def map_items(items: list[Item], fn: Transformer) -> list[Item]:
    output: list[Item] = []
    for item in items:
        result = fn(item)
        if result is not None:
            output.append(result)
    return output

# select_items filters; map_items transforms and drops Nones
products = [{"name": "Pen", "price": 2}, {"name": "Laptop", "price": 999}]
cheap = select_items(products, lambda p: p["price"] < 100)  # [{"name": "Pen", "price": 2}]
```

### 构建摘要字典

将数据聚合为结构化的摘要是一种常见的模式：

```python
from collections import Counter

items = [{"category": "A", "value": 10}, {"category": "B", "value": 20}, {"category": "A", "value": 15}]

count = len(items)
avg_value = round(sum(i["value"] for i in items) / count, 2)  # 15.0
category_counts = dict(Counter(i["category"] for i in items))  # {"A": 2, "B": 1}
```