# Python Dataclasses 综合实践

Python 的 `dataclasses` 模块提供了用于自动生成特殊方法（如 `__init__`、`__repr__` 和 `__eq__`）的装饰器和函数。在实际应用中，dataclass 是对结构化数据建模的首选工具——例如配置对象、API 响应、数据库记录和领域实体。本综合实践将多个 dataclass 特性结合到一个连贯的练习中。

## 工作原理

Dataclass 通过根据类级别的类型注解生成方法来减少样板代码。除了基础功能之外，它们还支持：

- **`frozen=True`**：使实例不可变且可哈希（类似于 namedtuple，但功能更丰富）
- **`field(default_factory=...)`**：为每个实例安全地提供可变默认值（列表、字典等）
- **`field(init=False)`**：将字段排除在 `__init__` 之外——对计算得出的值非常有用
- **`__post_init__`**：在自动生成的 `__init__` 之后运行，非常适合用于验证或计算派生字段

## 语法

```python
from dataclasses import dataclass, field

@dataclass(frozen=True)
class Coordinate:
    x: float
    y: float

@dataclass
class Inventory:
    items: list = field(default_factory=list)
    total: int = field(init=False)

    def __post_init__(self):
        self.total = len(self.items)
```

## 示例

```python
# Frozen dataclass — immutable and hashable
@dataclass(frozen=True)
class Color:
    r: int
    g: int
    b: int

red = Color(255, 0, 0)
{red, red, Color(0, 0, 255)}
# Works in sets!
# -> {Color(r=255, g=0, b=0), Color(r=0, g=0, b=255)}


# Validation in __post_init__
@dataclass
class Temperature:
    celsius: float

    def __post_init__(self):
        if self.celsius < -273.15:
            raise ValueError("Below absolute zero!")

Temperature(-300)  # Raises ValueError


# Computed field with field(init=False)
@dataclass
class ShoppingCart:
    items: list = field(default_factory=list)
    item_count: int = field(init=False)

    def __post_init__(self):
        self.item_count = len(self.items)

cart = ShoppingCart(items=["apple", "bread"])

cart.item_count
# 2

repr(cart)
# "ShoppingCart(items=['apple', 'bread'], item_count=2)"
```

## 需要牢记的核心概念

| **特性** | **作用** | **注意事项** |
| --- | --- | --- |
| `frozen=True` | 不可变性 + 可哈希性 | 创建后无法对字段重新赋值 |
| `field(default_factory=list)` | 安全的可变默认值 | 切勿直接使用 `hobbies: list = []` |
| `field(init=False)` | 从构造函数中排除 | 必须在 `__post_init__` 中设置 |
| `__post_init__` | 验证 / 计算 | 在 `__init__` 执行完毕后运行 |
| `repr()` | 自动生成的字符串表示形式 | 默认包含所有字段 |