# `match`/`case` 中的捕获模式（Capture Patterns）

你已经了解了如何使用 Python 的结构化模式匹配来匹配字面量值。捕获模式在此基础上更进一步，它们允许你将匹配的值（或其中的一部分）**绑定**到变量名上，从而使你能够直接访问复杂数据结构的各个组成部分。当你既需要识别结构的形状又需要操作其内容时，这一点至关重要。

## 工作原理

当 Python 在 `case` 模式中遇到裸名称（bare name）时，会将其视为**捕获模式**——它会匹配任何内容并将匹配到的值绑定到该变量名。这与查找现有变量有着本质的不同。如果你编写 `case x:`，Python 不会检查 `x` 是否等于某个先前定义的值；它会将匹配目标的值捕获到 `x` 中。

这带来了一个重要的结论：要与存储在变量中的常量进行匹配，你必须使用**点号分隔的名称**（如 `Color.RED`）或字面量（如 `42`、`"hello"`）。

## 语法

```python
# Capture a single value
match value:
    case name:
        # name is now bound to value

# Sequence patterns with captures
match some_list:
    case []:              # matches empty sequence
    case [x]:             # matches exactly 1 element, captures it as x
    case [x, y]:          # matches exactly 2 elements
    case [x, *rest]:      # matches 1 or more; rest captures the tail
    case [x, *mid, y]:    # captures first, last, and everything in between

# Mapping patterns with captures
match some_dict:
    case {"key": val}:    # matches dict with "key", captures its value as val
```

## 示例

```python
# Example 1: Describing a coordinate
def classify_point(point):
    match point:
        case [x]:
            return f"1D point at {x}"
        case [x, y]:
            return f"2D point at ({x}, {y})"
        case [x, y, z]:
            return f"3D point at ({x}, {y}, {z})"

classify_point([3, 7])       # "2D point at (3, 7)"
classify_point([1, 2, 3])    # "3D point at (1, 2, 3)"
```

```python
# Example 2: Extracting head and tail of a list
def head_tail(items):
    match items:
        case []:
            return "nothing here"
        case [head, *tail]:
            return f"head={head}, tail has {len(tail)} items"

head_tail([10, 20, 30])  # "head=10, tail has 2 items"
head_tail([])             # "nothing here"
```

```python
# Example 3: Star capture in the middle
def bookends(items):
    match items:
        case [first, *middle, last]:
            return f"{first}...({len(middle)} skipped)...{last}"
        case [only]:
            return f"just {only}"

bookends(["A", "B", "C", "D", "E"])  # "A...(3 skipped)...E"
```

## 常见模式

| **模式** | **匹配内容** | **捕获内容** |
| :--- | :--- | :--- |
| `case []` | 空序列 | 无 |
| `case [x]` | 恰好 1 个元素 | `x` = 该元素 |
| `case [x, y]` | 恰好 2 个元素 | `x`, `y` |
| `case [first, *rest]` | 1 个或多个元素 | `first` = 头部，`rest` = 剩余列表 |
| `case [first, *mid, last]` | 2 个或多个元素 | `first`, `last` 以及 `mid` = 中间的所有元素 |
| `case {"k": v}` | 包含键 `"k"` 的字典 | `v` = 对应的值 |

## 重要提示：裸名称始终为捕获模式

一个常见的错误是编写 `case x:` 并期望它与变量 `x` 进行比较。它不会这样做，而是会捕获该值。要匹配存储的常量，请使用点号分隔的名称（例如 `case Constants.X:`）或使用守卫（guard，例如 `case val if val == x:`）。

## 将元组作为序列进行匹配

像 `case [a, b]:` 这样的序列模式既可以匹配列表也可以匹配元组。Python 的结构化模式匹配将 `[...]` 视为通用序列模式，而不是严格的列表模式。如果你需要区分它们，则需要单独检查类型。