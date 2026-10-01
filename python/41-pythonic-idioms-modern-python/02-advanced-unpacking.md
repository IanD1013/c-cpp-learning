### 使用 `*` 进行高级解包与嵌套解包

Python 的解包功能远不止简单的 `a, b = (1, 2)`。使用 `*` 操作符的扩展解包以及嵌套解包模式，让你可以用单行且可读性极强的语句解构复杂的数据结构。在处理矩阵、CSV 数据行、API 响应或任何需要清晰切分元素的结构化数据时，这些技术至关重要。

### 工作原理

**扩展解包**使用带星号的变量（`*name`）将零个或多个剩余元素捕获到一个列表中。每个解包表达式中只允许出现一个带星号的变量，但它可以出现在任何位置——开头、中间或末尾。

**嵌套解包**允许你解构元素内部的元素。如果一个序列包含元组或列表，你可以直接在赋值目标中使用括号对其进行解包。

**元组交换**是解包的一种特殊情况，其中 `a, b = b, a` 会在赋值前对右侧完全求值，使其安全且具备原子性。

### 语法

```python
# Star at the end — captures trailing elements
head, *tail = some_sequence

# Star at the beginning — captures leading elements
*leading, last = some_sequence

# Star in the middle — captures everything between first and last
first, *middle, last = some_sequence

# Nested unpacking
(x, y), (a, b) = (10, 20), (30, 40)

# Nested with star
(name, *scores), total = ("Alice", 90, 85, 92), 267

# Swapping
a, b = b, a

# In for loops
for key, *values in rows:
    process(key, values)
```

### 示例

```python
# Example 1: Extracting head and tail from a log entry list
timestamp, *messages, status = ["2024-01-15", "Started", "Processing", "Done", "OK"]
# timestamp = "2024-01-15", messages = ["Started", "Processing", "Done"], status = "OK"

# Example 2: Nested unpacking from coordinate pairs
(x1, y1), (x2, y2) = (0, 0), (5, 10)
# x1=0, y1=0, x2=5, y2=10

# Example 3: Swap two variables without a temp
width, height = 1920, 1080
width, height = height, width
# width=1080, height=1920

# Example 4: Unpacking in a loop over records
records = [["Alice", 95, 88, 72], ["Bob", 80, 91, 85]]
for name, *grades in records:
    print(f"{name} averaged {sum(grades) / len(grades):.1f}")
# Alice averaged 85.0
# Bob averaged 85.3

# Example 5: Star captures empty list when nothing is left
only_two = ["start", "end"]
first, *middle, last = only_two
# first = "start", middle = [], last = "end"
```

### 常见模式

| **模式** | **作用** | **示例结果** |
| :--- | :--- | :--- |
| `head, *tail = seq` | 第一个元素 + 其余元素 | `head=1, tail=[2,3,4]` |
| `*init, last = seq` | 除最后一个以外的所有元素 + 最后一个元素 | `init=[1,2,3], last=4` |
| `a, *_, b = seq` | 第一个和最后一个，丢弃中间元素 | `a=1, b=5` |
| `for x, *rest in matrix:` | 在迭代中解包每一行 | `x` 是每行的第一列 |
| `a, b = b, a` | 交换值 | 原子交换 |