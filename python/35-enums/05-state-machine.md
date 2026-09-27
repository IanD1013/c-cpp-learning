### 综合练习：结合 Enum、auto()、IntEnum 与 Enum 方法

Python 的 `enum` 模块提供了多种用于定义符号常量的强大工具。在实际应用中，你经常需要结合使用多个 enum 特性——使用 `auto()` 定义成员、向 enum 添加自定义方法，以及在需要进行数值比较时使用 `IntEnum`。一个常见的应用场景是实现**状态机**（state machine），其中实体根据特定规则在明确定义的状态之间转换。

### Enum 如何协同工作

`Enum` 用于创建一组命名常量。`auto()` 会自动分配值，因此你无需手动管理它们。你可以像在任何其他 Python 类中一样向 enum 类添加**方法**——这些方法可以访问 `self`（即当前成员）。`IntEnum` 是一个特殊变体，其成员同时也是整数，这意味着你可以使用 `<`、`>`、`max()` 等对它们进行比较。

### 语法

```python
from enum import Enum, IntEnum, auto

class Color(Enum):
    RED = auto()
    GREEN = auto()
    BLUE = auto()

    def is_primary(self):
        return self in (Color.RED, Color.GREEN, Color.BLUE)

class Severity(IntEnum):
    LOW = 1
    MEDIUM = 2
    HIGH = 3
```

### 示例

```python
# Accessing members by name from a string
color = Color["RED"]       # Color.RED
color.name                  # "RED"
color.value                 # 1 (assigned by auto())
color.is_primary()          # True

# IntEnum supports integer operations and comparisons
sev = Severity.HIGH
int(sev)                    # 3
Severity.HIGH > Severity.LOW  # True
max(Severity.LOW, Severity.HIGH)  # Severity.HIGH

# Enum methods can return other enum members
class TrafficLight(Enum):
    RED = auto()
    YELLOW = auto()
    GREEN = auto()

    def next_light(self):
        sequence = {TrafficLight.RED: TrafficLight.GREEN,
                    TrafficLight.GREEN: TrafficLight.YELLOW,
                    TrafficLight.YELLOW: TrafficLight.RED}
        return sequence[self]

TrafficLight.RED.next_light()  # TrafficLight.GREEN

# Looking up enum members by name string
TrafficLight["YELLOW"]  # TrafficLight.YELLOW
```

### 常见模式

- **基于名称的查找**：`MyEnum["MEMBER_NAME"]` 从字符串检索成员——在处理字符串输入时非常有用。
- **转换映射**：将允许的状态变更存储为字典，将每个成员映射到有效后继成员的列表。
- **用于排名的 IntEnum**：当 enum 成员表示级别或优先级时，`IntEnum` 允许你直接使用 `max()`、`min()` 和比较运算符。

### 常用参考

| **特性** | **描述** | **示例** |
| :--- | :--- | :--- |
| `auto()` | 自动分配递增的值 | `DRAFT = auto()` |
| `Enum[name]` | 通过字符串名称查找成员 | `State["REVIEW"]` |
| `member.name` | 获取成员的字符串名称 | `State.DRAFT.name` → `"DRAFT"` |
| `int(ie)` | 获取 IntEnum 成员的整数值 | `int(Priority.HIGH)` → `3` |
| 自定义方法 | 在 enum 类上定义方法 | `def allowed(self): ...` |