### 使用 `auto()` 自动分配值

在定义枚举时，手动为每个成员分配连续的整数值会变得繁琐且容易出错——尤其是当枚举规模变大时。Python `enum` 模块中的 `auto()` 函数通过自动为你生成值来解决这一问题。默认情况下，`auto()` 会生成从 1 开始递增的整数，但你也可以完全自定义此行为。

### 工作原理

当 Python 遇到 `auto()` 作为成员的值时，它会调用一个名为 `_generate_next_value_` 的内部方法来决定要分配什么值。默认实现返回递增的整数：第一次调用 `auto()` 返回 `1`，第二次返回 `2`，依此类推。每次调用都会感知先前已分配的值，因此序列保持一致。

真正的强大之处在于能够在你的枚举类中**重写** `_generate_next_value_`。这个静态方法接收四个参数：

- `name` — 正在定义的成员名称
- `start` — 起始值（对于 Enum 默认为 1）
- `count` — 目前已创建的成员数量
- `last_values` — 之前分配的值的列表

通过从此方法返回不同的内容，你可以控制 `auto()` 生成的值。

### 语法

```python
from enum import Enum, auto

# Basic auto() usage — values are 1, 2, 3
class Priority(Enum):
    LOW = auto()
    MEDIUM = auto()
    HIGH = auto()

# Customizing auto() by overriding _generate_next_value_
class Color(Enum):
    @staticmethod
    def _generate_next_value_(name, start, count, last_values):
        return name.upper()  # or any transformation

    RED = auto()
    GREEN = auto()
    BLUE = auto()
```

### 示例

```python
from enum import Enum, auto

# Example 1: Default auto() behavior
class Direction(Enum):
    NORTH = auto()
    SOUTH = auto()
    EAST = auto()
    WEST = auto()

print(Direction.NORTH.value)  # 1
print(Direction.WEST.value)  # 4

# Example 2: Iterating over auto-assigned members
for d in Direction:
    print(f"{d.name} -> {d.value}")
# NORTH -> 1, SOUTH -> 2, EAST -> 3, WEST -> 4

# Example 3: Custom _generate_next_value_ returning lowercase names
class LogLevel(Enum):
    @staticmethod
    def _generate_next_value_(name, start, count, last_values):
        return name.lower()

    DEBUG = auto()
    INFO = auto()
    WARNING = auto()
    ERROR = auto()

print(LogLevel.DEBUG.value)  # "debug"
print(LogLevel.WARNING.value)  # "warning"

# Example 4: Accessing a member by name using bracket notation
member = LogLevel["ERROR"]
print(member.value)  # "error"

# Example 5: Building a dict from enum members
all_levels = {m.name: m.value for m in LogLevel}
# {"DEBUG": "debug", "INFO": "info", "WARNING": "warning", "ERROR": "error"}
```

### 常见模式

| **模式** | **描述** |
| :--- | :--- |
| `member.value` | 访问自动生成的值 |
| `member.name` | 以字符串形式访问成员的名称 |
| `EnumClass["NAME"]` | 通过名称（字符串）查找成员 |
| `for m in EnumClass` | 按定义顺序遍历所有成员 |
| `{m.name: m.value for m in EnumClass}` | 创建包含所有名称-值对的字典 |

### 关于 `_generate_next_value_` 的重要提示

`_generate_next_value_` 的重写**必须定义在使用 `auto()` 的任何成员之前**。Python 从上到下处理类主体，因此如果你先定义成员，它们将使用默认的生成器。请将该静态方法放置在枚举类的顶部。