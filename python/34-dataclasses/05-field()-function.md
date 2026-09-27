# Dataclass 中的 `field()` 函数

你已经知道如何使用 `@dataclass`、设置默认值、冻结实例以及使用 `__post_init__`。但 `field()` 函数才是让 dataclass 真正强大的地方 —— 它让你能够细粒度地控制每个字段如何参与初始化、表示、比较和哈希计算。

在实际开发中，你经常需要处理不应出现在日志中的字段（敏感数据）、不应影响相等性检查的字段（内部 ID、时间戳），或者需要每个实例独立的唯独可变默认值（列表、字典）。`field()` 就是处理所有这些情况的利器。

## `field()` 的工作原理

当你使用 `field(...)` 为 dataclass 字段添加注解时，你是在明确告知 Python 在自动生成的方法中如何对待该字段。每个参数都控制着特定的行为：

- **`default` / `default_factory`**：设置默认值。对于不可变值使用 `default`，对于可变值使用 `default_factory`（一个为每个实例生成全新值的可调用对象）。
- **`repr`**：若为 `False`，则该字段会从自动生成的 `__repr__` 字符串中排除。
- **`compare`**：若为 `False`，则该字段会从 `__eq__` 和排序比较中排除。
- **`hash`**：控制是否参与 `__hash__`。通常保留为 `None`（遵循 `compare`）。
- **`init`**：若为 `False`，则该字段不会包含在 `__init__` 参数中（适用于在 `__post_init__` 中设置的计算字段）。
- **`metadata`**：一个只读映射，用于存储任意信息（文档、验证规则、序列化提示）。

## 语法

```python
from dataclasses import dataclass, field

@dataclass
class MyClass:
    visible: str
    hidden: str = field(default="secret", repr=False)
    mutable_default: list = field(default_factory=list)
    not_compared: int = field(default=0, compare=False)
    extra_info: str = field(default="", metadata={"description": "Extra data"})
```

## 示例

### 使用 `default_factory` 的可变默认值

```python
from dataclasses import dataclass, field

@dataclass
class ShoppingCart:
    owner: str
    items: list = field(default_factory=list)

cart1 = ShoppingCart("Alice")
cart2 = ShoppingCart("Bob")
cart1.items.append("Laptop")
print(cart2.items)  # [] — each instance has its own list!
```

如果没有 `default_factory`，写成 `items: list = []` 会导致所有实例共享**同一个**列表对象 —— 这是 Python 中一个众所周知的陷阱。

### 控制 `repr`

```python
@dataclass
class Employee:
    name: str
    salary: float = field(repr=False)  # Don't show salary in repr
    department: str = "Engineering"

e = Employee("Dana", 95000.0)
print(repr(e))  # Employee(name='Dana', department='Engineering')
# salary is stored but hidden from repr output
```

### 控制 `compare`

```python
@dataclass
class Document:
    title: str
    content: str
    revision_id: int = field(default=0, compare=False)

d1 = Document("Report", "Hello", revision_id=1)
d2 = Document("Report", "Hello", revision_id=42)
print(d1 == d2)  # True — revision_id is ignored in comparison
```

### 使用 `metadata`

```python
from dataclasses import dataclass, field, fields

@dataclass
class Sensor:
    temperature: float = field(metadata={"unit": "celsius", "precision": 2})

# Access metadata through fields()
for f in fields(Sensor):
    print(f.name, f.metadata)  # temperature {'unit': 'celsius', 'precision': 2}
```

## 常见模式

| **参数** | **默认值** | **修改后的效果** |
| :--- | :--- | :--- |
| `repr=False` | `True` | 字段从 `repr()` 输出中隐藏 |
| `compare=False` | `True` | 字段在 `==` 和排序中被忽略 |
| `init=False` | `True` | 字段从 `__init__` 签名中排除 |
| `default_factory=list` | — | 每个实例获取一个全新的 `list` |
| `hash=False` | `None` | 字段从 `__hash__` 中排除 |

具有 `init=False` 的字段仍然存在于实例上 —— 你通常会在 `__post_init__` 内部对其进行设置，或在构造之后为其赋值。