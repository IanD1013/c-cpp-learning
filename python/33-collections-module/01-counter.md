### Counter 类

`collections` 模块中的 `Counter` 是一个专门用于对可哈希（hashable）对象进行计数的 `dict` 子类。与手动编写循环来统计出现次数不同，`Counter` 可以自动为你处理计数——你只需传入一个可迭代对象，它就会返回一个将每个元素映射到其频次的字典。这是一个一旦掌握就会在数据处理、文本分析和算法问题中频繁使用的强大工具。

### 工作原理

当你向 `Counter()` 传入一个可迭代对象时，它会遍历每个元素，将每个元素作为字典的键，并对其关联的值进行递增。其结果是一个类字典对象，其中的键是唯一的元素，值是它们出现的次数。由于 `Counter` 是 `dict` 的子类，你可以使用所有常见的字典方法——但它也自带了许多强大的特性。

### 语法

```python
from collections import Counter

# Create from any iterable
counter = Counter(iterable)

# Access a count (missing keys return 0, never KeyError)
count = counter[element]

# Get the n most common elements as [(element, count), ...]
top_n = counter.most_common(n)

# Arithmetic between Counters
combined = counter_a + counter_b    # adds counts
difference = counter_a - counter_b  # subtracts (drops zero/negative)
```

### 示例

```python
from collections import Counter

# Counting characters in a string
letter_counts = Counter("mississippi")
print(letter_counts)
# Counter({'s': 4, 'i': 4, 'p': 2, 'm': 1})

# Counting items in a list
fruits = ["apple", "banana", "apple", "cherry", "banana", "apple"]
fruit_counts = Counter(fruits)
print(fruit_counts["apple"])   # 3
print(fruit_counts["mango"])   # 0  (no KeyError!)

# Getting the 2 most common fruits
print(fruit_counts.most_common(2))
# [('apple', 3), ('banana', 2)]

# Arithmetic: combining two inventories
warehouse_a = Counter({"bolts": 40, "nuts": 30})
warehouse_b = Counter({"bolts": 20, "washers": 50})
total = warehouse_a + warehouse_b
print(total)
# Counter({'washers': 50, 'bolts': 60, 'nuts': 30})

# Subtraction drops keys that reach zero or below
sold = Counter({"bolts": 45, "nuts": 30})
remaining = total - sold
print(remaining)
# Counter({'washers': 50, 'bolts': 15})
```

### 常见用法

- **频次分析**：将任意序列传递给 `Counter()` 即可立即获取频次表。
- **安全访问**：与普通字典不同，访问不存在的键会返回 `0`——无需使用 `.get()` 或 `defaultdict`。
- **Top-N 查询**：`most_common(n)` 返回一个按频次降序排列的 `(element, count)` 元组列表。不带参数调用 `most_common()` 会返回按计数排序的所有元素。
- **规范化输入**：在统计单词时，转换为统一的大小写（例如小写）可确保 `"Hello"` 和 `"hello"` 被一同统计。

### 常用参考

| **方法 / 操作** | **描述** | **示例结果** |
| :--- | :--- | :--- |
| `Counter(iterable)` | 从可迭代对象创建计数器 | `Counter(['a','b','a'])` → `{'a': 2, 'b': 1}` |
| `counter[key]` | 返回计数（若不存在则返回 0） | `counter['z']` → `0` |
| `most_common(n)` | 按计数获取前 n 个元素 | `[('a', 5), ('b', 3)]` |
| `counter_a + counter_b` | 逐元素相加计数 | 合并两个计数器 |
| `counter_a - counter_b` | 逐元素相减（忽略 ≤ 0 的项） | 扣除售出项目 |
| `str.lower()` | 将字符串转换为小写 | `"Hello"` → `"hello"` |
| `str.split()` | 按空白字符拆分为列表 | `"a b c"` → `['a', 'b', 'c']` |