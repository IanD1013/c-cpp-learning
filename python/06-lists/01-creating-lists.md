### 列表 —— 有序、可变集合

**列表**（list）是 Python 中最常用的数据结构之一。它存储了一个有序的元素集合，并且在创建后可以修改（可变）。每当你需要将相关联的值组合在一起时 —— 无论是一系列得分、一组用户名，还是一批传感器读数 —— 列表通常都是最合适的工具。

### 工作原理

你可以通过将元素放入**方括号** `[]` 中并用逗号分隔来创建列表。每个元素都保留其位置（顺序很重要），并且你可以在同一个列表中自由混合不同的数据类型。

Python 还为列表提供了内置的 `len()` 函数，其用法与处理字符串时完全相同 —— 它返回列表中元素的数量。而 `type()` 可以确认数据类型：

```python
type([10, 20, 30])   # <class 'list'>
```

### 语法

```python
# Creating lists
empty = []
three_numbers = [10, 20, 30]
mixed = ["text", 3.14, False, None]

# Getting the length
len(three_numbers)   # 3
len(empty)           # 0
```

### 示例

```python
# A list of city names
cities = ["Paris", "Tokyo", "Lima"]
print(cities)        # ['Paris', 'Tokyo', 'Lima']
print(len(cities))   # 3

# A list with mixed types
record = ["Alice", 30, True, 5.7]
print(record)        # ['Alice', 30, True, 5.7]
print(type(record))  # <class 'list'>

# An empty list — useful as a starting point before adding items later
results = []
print(len(results))  # 0

# Building a list from variables
x = 100
y = 200
coordinates = [x, y]
print(coordinates)   # [100, 200]
```

注意在最后一个示例中，将变量放入方括号时，会求出变量所对应的值。你存储的并不是变量名 —— 而是它们在那个时刻所持有的实际值。

### 常见模式

| 模式       | 示例                | 结果         |
| -------- | ----------------- | ---------- |
| 空列表      | `[]`              | `[]`       |
| 同类型列表    | `[1, 2, 3]`       | 全部为相同类型    |
| 混合类型列表   | `["hi", 7, None]` | 不同类型       |
| 由变量构成的列表 | `[x, y, z]`       | x, y, z 的值 |
| 长度检查     | `len([4, 5, 6])`  | `3`        |
