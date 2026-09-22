### 使用 Key 函数进行排序

Python 内置的 `sorted()` 函数本身就非常强大——它可以按数值大小对数字进行排序，按字母顺序对字符串进行排序。但如果你需要按*不同的标准*进行排序呢？这就是 `key` 参数发挥作用的地方。它允许你告诉 Python 应该使用每个元素的*哪个方面*来进行比较，而不是直接比较元素本身。

这种模式在实际代码中经常出现：按日期对数据库记录进行排序、按价格对商品进行排序、按分数对玩家进行排名——只要“自然”顺序不符合你的需求，就可以使用它。

### 工作原理

当你向 `sorted()` 传递一个 `key` 函数时，Python 会在开始排序前对**每个元素调用一次**该函数。它使用 key 函数的返回值来决定顺序，但最终放入结果列表中的仍然是原始元素。

可以把它想象成按考试成绩对学生进行排序。你并不是在重新排列成绩——而是在根据成绩重新排列*学生*。key 函数从每个元素中提取出“成绩”。

重要的是，Python 的排序是**稳定**的：如果两个元素具有相同的 key 值，它们会保持原本的相对顺序。这是你可以依赖的一项保证。

### 语法

```python
sorted(iterable, key=function, reverse=False)
```

* `iterable` — 任何可迭代对象（list、tuple、set、generator 等）
* `key` — 一个接收单个元素并返回用于排序的值的函数
* `reverse` — 如果为 `True`，则按降序排序（默认为 `False`）
* **返回一个新列表** — 原始可迭代对象保持不变

### 示例

```python
# Sort numbers by their absolute value
nums = [-8, 3, -1, 5, -2]
result = sorted(nums, key=abs)
# result: [-1, -2, 3, 5, -8]

# Sort tuples by their second element using a lambda
coordinates = [(3, 7), (1, 2), (5, 4)]
result = sorted(coordinates, key=lambda point: point[1])
# result: [(1, 2), (5, 4), (3, 7)]

# Sort dictionaries by a specific field
employees = [
    {"name": "Zara", "salary": 55000},
    {"name": "Alex", "salary": 72000},
    {"name": "Mina", "salary": 48000}
]
result = sorted(employees, key=lambda e: e["salary"])
# result: [{"name": "Mina", ...}, {"name": "Zara", ...}, {"name": "Alex", ...}]

# Descending order with reverse=True
scores = [82, 95, 71, 88]
result = sorted(scores, reverse=True)
# result: [95, 88, 82, 71]
```

### sorted() 与 list.sort()

| 特性     | `sorted(iterable)` | `list.sort()` |
| ------ | ------------------ | ------------- |
| 返回值    | 一个**新列表**          | `None`（就地修改）  |
| 适用对象   | 任何可迭代对象            | 仅限列表          |
| 原始数据   | 保持不变               | 被修改           |
| Key 参数 | ✅ `key=func`       | ✅ `key=func`  |

两者都支持 `key` 和 `reverse`，但当你希望保留原始数据时，`sorted()` 是更安全的选择。

### 常见的 Key 函数

| Key 函数               | 排序依据          | 示例用途         |
| -------------------- | ------------- | ------------ |
| `len`                | 字符串/序列的长度     | 从最短到最长       |
| `abs`                | 数字的绝对值        | 最接近零的排在前面    |
| `str.lower`          | 小写形式          | 忽略大小写的字母顺序   |
| `lambda x: x[n]`     | 元组/列表的第 n 个元素 | 按某个字段对记录进行排序 |
| `lambda d: d["key"]` | 字典的值          | 按属性对字典进行排序   |
