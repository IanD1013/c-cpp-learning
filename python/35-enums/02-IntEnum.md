# IntEnum — 兼具整数特性的枚举成员

你已经知道 `Enum` 可以创建带有名称和值的符号常量。但常规的 `Enum` 成员**不是**整数 —— 比较 `Color.RED == 1` 会返回 `False`，而尝试 `Color.RED > Color.BLUE` 会引发 `TypeError`。不过，有时你确实需要行为类似于整数的枚举成员：状态码、严重程度级别、协议标志等。这就是 `IntEnum` 的用武之地。

`IntEnum` 是 `int` 和 `Enum` **两者**的子类。它的成员是碰巧也拥有名称的真正整数。这意味着它们支持算术运算、与普通 `int` 值的比较，并且可以传递给任何期望 `int` 参数的函数。

## 工作原理

当你定义一个继承自 `IntEnum` 的类时，每个成员同时既是 `int` 实例也是枚举成员。这种双重身份意味着：

- **直接整数比较**可行：`member > 5`、`member == 3`
- **算术运算**可行：`member + 2`、`member * 3`（结果是普通 `int`）
- **排序**天然按数值大小进行
- **从 int 转换**可行：`MyEnum(3)` 返回值为 3 的成员

然而，这种强大功能的背后也有所取舍。常规 `Enum` 提供了**类型隔离** —— 即使它们具有相同的底层值，你也不会意外混淆 `Color` 和 `Direction`。`IntEnum` 打破了这种隔离，因为如果两者的值都等于 `1`，那么 `Color.RED == Direction.NORTH` 将为 `True`。仅在确实需要整数互操作性时才使用 `IntEnum`。

## 语法

```python
from enum import IntEnum

class Severity(IntEnum):
    DEBUG = 0
    INFO = 1
    WARNING = 2
    ERROR = 3
    FATAL = 4
```

## 示例

```python
from enum import IntEnum

class HttpStatus(IntEnum):
    OK = 200
    NOT_FOUND = 404
    SERVER_ERROR = 500

# Members are actual integers
print(HttpStatus.OK == 200)          # True (would be False with regular Enum!)
print(HttpStatus.NOT_FOUND > 400)    # True — direct comparison with int

# Arithmetic produces plain ints
print(HttpStatus.OK + 4)             # 204

# Convert from integer
status = HttpStatus(404)
print(status.name)                   # "NOT_FOUND"
print(status.value)                  # 404

# Sorting works by value
for s in sorted(HttpStatus):
    print(s.name, s.value)
# OK 200
# NOT_FOUND 404
# SERVER_ERROR 500

# Iterating gives all members
names = [s.name for s in HttpStatus]
print(names)  # ['OK', 'NOT_FOUND', 'SERVER_ERROR']
```

## 常见模式

| **操作** | **常规 Enum** | **IntEnum** |
| :--- | :--- | :--- |
| `member == 3` | `False` | `True`（如果值为 3） |
| `member > other_member` | `TypeError` | 可行（数值比较） |
| `member + 1` | `TypeError` | 返回一个 `int` |
| `SomeEnum(3)` | 返回成员 | 返回成员 |
| `member.name` | 可行 | 可行 |
| `sorted(SomeEnum)` | 按定义顺序 | 按数值大小 |

## 实用参考

| **特性** | **描述** | **示例** |
| :--- | :--- | :--- |
| `IntEnum(value)` | 将 int 转换为成员 | `Priority(2)` → `Priority.MEDIUM` |
| `member.name` | 获取字符串名称 | `Priority.HIGH.name` → `"HIGH"` |
| `member.value` | 获取整数值 | `Priority.HIGH.value` → `3` |
| `member > int` | 与整数比较 | `Priority.HIGH > 2` → `True` |
| `member + member` | 将两个成员相加 | 返回一个普通 `int` |
| `sorted(EnumClass)` | 按值对所有成员进行排序 | 返回成员列表 |