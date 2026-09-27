### 使用 `json.dumps()` 进行自定义 JSON 序列化

Python 的 `json.dumps()` 可以处理字符串、数字、列表、字典、布尔值和 `None` 等标准类型。但一旦尝试序列化 `datetime.date`、`set` 或自定义对象，它就会抛出 `TypeError`。在实际应用中，数据几乎总是包含超出 JSON 基础类型的对象。`default` 参数提供了一个简洁的钩子（hook），用于优雅地处理这些类型。

### 工作原理

当 `json.dumps()` 遇到无法序列化的对象时，它会调用作为 `default` 传入的函数，并将该对象传递给该函数。你的函数会检查该对象，将其转换为对 JSON 友好的格式（字符串、列表、字典、数字等），并返回该值。如果你的函数同样不知道如何处理该对象，则应抛出 `TypeError` 以提示失败。

至关重要的是，`default` 函数是**针对每个不可序列化的对象单独调用的**，而不是对整个数据结构只调用一次。编码器会递归遍历数据，每次遇到无法处理的内容时，都会仅针对这单个对象调用你的函数。

### 语法

```python
import json

def my_handler(obj):
    # Inspect obj, return a serializable version, or raise TypeError
    if isinstance(obj, SomeType):
        return convert_to_serializable(obj)
    raise TypeError(f"Cannot serialize {type(obj).__name__}")

result = json.dumps(data, default=my_handler)
```

### 示例

```python
import json
from decimal import Decimal
from pathlib import Path

def example_handler(obj):
    # Convert Decimal to float for JSON
    if isinstance(obj, Decimal):
        return float(obj)
    # Convert Path objects to their string representation
    if isinstance(obj, Path):
        return str(obj)
    raise TypeError(f"Not serializable: {type(obj).__name__}")

# Decimal is not natively serializable
print(json.dumps({"price": Decimal("19.99")}, default=example_handler))
# Output: {"price": 19.99}

print(json.dumps({"config": Path("/etc/app.conf")}, default=example_handler))
# Output: {"config": "/etc/app.conf"}

# int and str are natively serializable, so the handler is never called for them
print(json.dumps({"count": 5, "name": "widget"}, default=example_handler))
# Output: {"count": 5, "name": "widget"}
```

### 重要提示：`isinstance` 检查的顺序

在 Python 中，`datetime.datetime` 是 `datetime.date` 的**子类**。这意味着 `isinstance(some_datetime, datetime.date)` 会返回 `True`。当编写需要区分这两种类型的处理函数时，检查的顺序至关重要。如果先检查父类，子类将会匹配该分支，永远无法进入属于自己的分支。

```python
import datetime

dt = datetime.datetime(2026, 3, 15, 10, 30, 0)
print(isinstance(dt, datetime.date))      # True — datetime IS a date!
print(isinstance(dt, datetime.datetime))  # True
```

### 常用参考

| **方法 / 属性** | **描述** | **示例结果** |
| :--- | :--- | :--- |
| `datetime.date.isoformat()` | 以 `YYYY-MM-DD` 字符串形式返回日期 | `"2026-03-15"` |
| `datetime.datetime.isoformat()` | 以 ISO 8601 格式返回日期时间 | `"2026-03-15T10:30:00"` |
| `sorted(iterable)` | 返回一个新的排序后列表 | `sorted({3, 1, 2})` → `[1, 2, 3]` |
| `isinstance(obj, type)` | 检查 obj 是否为 type 的实例 | `isinstance([], list)` → `True` |