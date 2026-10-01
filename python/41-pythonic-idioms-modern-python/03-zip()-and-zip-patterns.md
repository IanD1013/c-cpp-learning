# `zip()` 函数与常见 Zip 模式

`zip(*iterables)` 是 Python 中最通用的内置函数之一。它接收多个可迭代对象并生成一个元组迭代器，其中每个元组包含来自所有输入可迭代对象中相应位置的元素。它会在**最短**的可迭代对象处停止——来自较长可迭代对象的任何多余元素都会被静默丢弃。

## 工作原理

可以将 `zip()` 想象成夹克上的拉链：它将两个独立的边缘逐个元素地咬合在一起。给定长度为 3、5 和 4 的可迭代对象，`zip()` 恰好生成 3 个元组（即最小长度）。每个元组的第一个元素来自第一个可迭代对象，第二个元素来自第二个可迭代对象，依此类推。

## 语法

```python
# 包含两个或更多可迭代对象的基本 zip
zip(iterable1, iterable2, ...)

# 解压（Unzip）模式：将行转置为列
columns = zip(*list_of_tuples)

# 从并行序列创建字典
dict(zip(key_sequence, value_sequence))

# 填充较短的可迭代对象而不是截断
from itertools import zip_longest
zip_longest(iter1, iter2, fillvalue=default)

# 严格模式（Python 3.10+）— 长度不匹配时引发 ValueError
zip(iter1, iter2, strict=True)
```

## 示例

```python
# Pairing names with scores
names = ["Alice", "Bob", "Carol"]
scores = [92, 85, 78]

for name, score in zip(names, scores):
    print(f"{name}: {score}")

# Alice: 92
# Bob: 85
# Carol: 78
```

```python
# Building a dict from two lists
colors = ["red", "green", "blue"]
hex_codes = ["#FF0000", "#00FF00", "#0000FF"]

color_map = dict(zip(colors, hex_codes))

# {'red': '#FF0000', 'green': '#00FF00', 'blue': '#0000FF'}
```

```python
# Truncation with unequal lengths
letters = ["x", "y", "z", "w"]
numbers = [10, 20]

result = list(zip(letters, numbers))

# [('x', 10), ('y', 20)]  — 'z' and 'w' are dropped
```

```python
# The unzip pattern — transposing paired data back into separate sequences
pairs = [("x", 10), ("y", 20), ("z", 30)]

letters_back, numbers_back = zip(*pairs)

# letters_back = ('x', 'y', 'z')
# numbers_back = (10, 20, 30)
# Note: zip(*pairs) unpacks each tuple as a separate argument to zip
```

```python
# Transposing a matrix (list of rows → list of columns)
grid = [
    [1, 2],
    [3, 4],
    [5, 6]
]  # 3 rows, 2 columns

flipped = [list(col) for col in zip(*grid)]

# [[1, 3, 5], [2, 4, 6]]  — now 2 rows, 3 columns
```

```python
# zip_longest to keep all elements
from itertools import zip_longest

team_a = ["Alice", "Bob"]
team_b = ["Carol", "Dave", "Eve"]

matched = list(zip_longest(team_a, team_b, fillvalue="TBD"))

# [('Alice', 'Carol'), ('Bob', 'Dave'), ('TBD', 'Eve')]
```

## 常见模式

| **模式** | **代码** | **目的** |
| :--- | :--- | :--- |
| 配对两个列表 | `list(zip(a, b))` | 从并行列表中创建元组 |
| 构建字典 | `dict(zip(keys, vals))` | 将键映射到值 |
| 解压（Unzip） | `zip(*paired_data)` | 将列重新分离出来 |
| 转置矩阵 | `[list(r) for r in zip(*matrix)]` | 行 ↔ 列 |
| 填充较短项 | `zip_longest(a, b, fillvalue=X)` | 保留所有元素，填充空缺 |
| 统计配对数 | `len(list(zip(a, b)))` | 匹配的配对数量 |