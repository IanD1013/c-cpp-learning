### 列表推导式 (List Comprehensions)

**列表推导式**（list comprehension）是一种简洁、富有表现力的方式，只需一行代码即可通过转换可迭代对象中的每个元素来构建一个新列表。通常的模式是先初始化一个空列表，然后在循环中不断向其追加元素；与这种模式不同，列表推导式让你直接描述最终列表应该包含*什么*内容——Python 会为你构建它。列表推导式被认为更加符合 **Python 风格**（Pythonic），而且通常比等效的“循环-追加”（loop-and-append）代码运行得更快，因为其内部迭代是在底层进行的。

### 工作原理

列表推导式会对从**可迭代对象**（iterable）中取出的每个**元素**（item）评估一次**表达式**（expression）。每个结果会按顺序收集到一个新列表中。可以把它理解为在说：*“对于 [那个集合] 中的每个 [元素]，给我一个由 [这个结果] 组成的列表。”*

对应的完整写法形式始终遵循相同的结构：

```python
result = []
for item in iterable:
    result.append(expression)
```

列表推导式将这三行代码浓缩为一行。

### 语法

```python
new_list = [expression for item in iterable]
```

* `expression` — 任何使用了 `item`（或未使用）的有效 Python 表达式。
* `item` — 循环变量，依次获取 `iterable` 中的每个值。
* `iterable` — 任何可以进行循环遍历的对象：`range`、`list`、`str`、`dict` 等。

### 示例

```python
# Convert temperatures from Celsius to Fahrenheit
celsius = [0, 20, 37, 100]
fahrenheit = [c * 9 / 5 + 32 for c in celsius]
# fahrenheit -> [32.0, 68.0, 98.6, 212.0]

# Extract the first character of each word
words = ["apple", "banana", "cherry"]
initials = [w[0] for w in words]
# initials -> ['a', 'b', 'c']

# Build a list of cubes from 0 to 4
cubes = [n ** 3 for n in range(5)]
# cubes -> [0, 1, 8, 27, 64]
```

注意每个列表推导式是如何替代原本需要多行编写的循环的。

### 循环追加 vs. 列表推导式

以下是使用两种方式编写的同一个任务，方便你查看它们之间的等价关系：

```python
# Loop-and-append style
lengths = []
for word in ["cat", "elephant", "ox"]:
    lengths.append(len(word))
# lengths -> [3, 8, 2]

# Comprehension style
lengths = [len(word) for word in ["cat", "elephant", "ox"]]
# lengths -> [3, 8, 2]
```

### 实用参考

| **工具描述示例**           |                                            |                         |
| -------------------- | ------------------------------------------ | ----------------------- |
| `range(start, stop)` | 生成从 `start` 开始到 `stop`（但**不包括** `stop`）的整数 | `range(1, 4)` → 1, 2, 3 |
| `range(stop)`        | 从 0 开始的简写形式                                | `range(3)` → 0, 1, 2    |
| `**` 运算符             | 幂运算                                        | `3 ** 2` → 9            |
