# Dataclasses 中的默认值

Dataclass 字段可以拥有默认值，就像函数参数一样。这使您可以在创建实例时无需指定每个字段——只需指定您想要自定义的字段。然而，这里有一条重要的顺序规则，以及一个所有 Python 开发者都需要了解的关于可变默认值的关键陷阱。

## 工作原理

当您定义一个 dataclass 时，没有默认值的字段必须出现在有默认值的字段**之前**——这与函数参数的规则完全相同。`@dataclass` 装饰器会生成一个 `__init__`，其中具有默认值的字段将成为可选的关键字参数。

对于像 `int`、`float`、`bool` 或 `str` 这样的**不可变**默认值，您可以直接赋值：

```python
@dataclass
class Settings:
    name: str = "unnamed"
    retries: int = 5
    enabled: bool = True
```

对于像 `list`、`dict` 或 `set` 这样的**可变**默认值，您**不能**直接赋值。Python 会在所有实例之间共享同一个可变对象——这是经典的可变默认参数 bug。相反，您应该使用 `dataclasses` 模块中的 `field(default_factory=...)`。

## 语法

```python
from dataclasses import dataclass, field

@dataclass
class MyClass:
    # Immutable defaults — assign directly
    count: int = 0
    label: str = "default"

    # Mutable defaults — use field(default_factory=...)
    items: list = field(default_factory=list)
    metadata: dict = field(default_factory=dict)
```

## 示例

```python
from dataclasses import dataclass, field, fields

@dataclass
class Player:
    name: str
    health: int = 100
    level: int = 1
    inventory: list = field(default_factory=list)

# Using all defaults except name (which has no default)
p1 = Player(name="Alice")
print(p1)  # Player(name='Alice', health=100, level=1, inventory=[])

# Overriding some defaults
p2 = Player(name="Bob", health=80, inventory=["sword"])
print(p2)  # Player(name='Bob', health=80, level=1, inventory=['sword'])

# Each instance gets its own list
p1.inventory.append("shield")
print(p1.inventory)  # ['shield']
print(p2.inventory)  # ['sword'] — not affected!

# Equality uses all fields
print(Player(name="Alice") == Player(name="Alice"))  # True
print(p1 == p2)  # False
```

## 动态检查字段

来自 `dataclasses` 的 `fields()` 函数返回 dataclass 实例的所有字段描述符。结合 `getattr`，您可以遍历所有字段而无需硬编码它们的名称：

```python
from dataclasses import fields

for f in fields(p1):
    print(f"{f.name} = {getattr(p1, f.name)}")

# name = Alice
# health = 100
# level = 1
# inventory = ['shield']
```

## 动态修改属性

Python 内置的 `setattr(obj, name, value)` 允许您在运行时按名称设置属性：

```python
setattr(p1, "health", 50)
print(p1.health)  # 50
```