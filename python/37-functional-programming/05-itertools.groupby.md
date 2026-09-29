### `itertools.groupby` — 分组连续元素

`itertools.groupby(iterable, key=None)` 是一个强大的工具，用于将序列拆分为具有相同键值的连续元素组。与不考虑顺序对整个数据集进行操作的 SQL `GROUP BY` 不同，Python 的 `groupby` 只对具有相同键的**连续**元素进行分组。这意味着可迭代对象**必须首先按键函数进行排序**，否则你会得到碎片化、重复的分组，而不是合并后的分组。

### 工作原理

`groupby` 逐个遍历可迭代对象中的元素。它生成 `(key_value, group_iterator)` 对。每个 `group_iterator` 产生共享 `key_value` 的所有连续元素。当键发生改变时，当前分组结束并开始一个新的分组。

**关键细节：** `group_iterator` 与 `groupby` 本身共享相同的底层可迭代对象。这意味着你**必须在移动到下一个分组之前消费或转换该分组迭代器（例如使用 `list()`）**。如果不这样做，推进到下一个 `(key_value, group_iterator)` 对将使前一个分组迭代器失效。

### 语法

```python
from itertools import groupby

# General pattern
for key_value, group_iter in groupby(sorted_iterable, key=key_function):
    items = list(group_iter)  # Consume immediately!
    # process items...
```

### 示例

```python
from itertools import groupby

# Example 1: Group numbers by even/odd — sort first!
numbers = [3, 1, 4, 2, 5, 6]
sorted_nums = sorted(numbers, key=lambda x: x % 2)
for k, g in groupby(sorted_nums, key=lambda x: x % 2):
    print(k, list(g))
# 0 [4, 2, 6]
# 1 [3, 1, 5]

# Example 2: Group words by length
words = ["hi", "go", "cat", "dog", "jump", "run"]
sorted_words = sorted(words, key=len)
result = {k: list(g) for k, g in groupby(sorted_words, key=len)}
# {2: ['hi', 'go'], 3: ['cat', 'dog', 'run'], 4: ['jump']}

# Example 3: What happens WITHOUT sorting first?
letters = ['a', 'b', 'a', 'a', 'b']
for k, g in groupby(letters):
    print(k, list(g))
# a ['a']       <-- first 'a'
# b ['b']       <-- first 'b'
# a ['a', 'a']  <-- second run of 'a'
# b ['b']       <-- second run of 'b'
# Notice: 'a' and 'b' each appear TWICE because they aren't consecutive!

# Example 4: Danger — not consuming the group iterator
data = sorted([10, 20, 11, 21], key=lambda x: x // 10)
groups = []
for k, g in groupby(data, key=lambda x: x // 10):
    groups.append((k, g))  # Storing the iterator, NOT consuming it!
# groups[0][1] is now exhausted — list(groups[0][1]) gives []
# Always do list(g) inside the loop!
```

### 常见模式

- **先排序后分组：** 始终将 `sorted(data, key=func)` 与 `groupby(sorted_data, key=func)` 搭配使用，并使用**相同**的键函数。
- **构建字典：** 使用字典推导式或循环将每个键映射到其分组下的成员。
- **按组聚合：** 统计成员数量、查找最大值/最小值、收集特定字段——所有这些都在分组迭代器仍然有效时在循环内部完成。