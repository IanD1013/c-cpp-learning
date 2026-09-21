### Python 中的集合（Sets）

**集合**（**set**）是一个由**唯一元素**组成的无序集合。虽然列表和元组可以包含重复项，但集合会自动去除它们。这使得集合非常适合用于跟踪不重复的值、高效检查成员资格或消除数据中的重复项。

### 集合的工作原理

创建集合时，Python 会对每个元素仅存储一次。如果你尝试添加重复项，它会被静默忽略。因为集合是**无序的**，所以它们不支持索引（`my_set[0]` 将无法工作）——但它们在快速查找和保证唯一性方面表现出色。

集合只能包含**不可变（可哈希/hashable）**元素：数字、字符串、布尔值和元组。你不能将列表或字典放入集合中，因为它们是可变的。

### 语法

```python
# Creating a set with curly braces
colors = {"red", "green", "blue"}

# Duplicates are removed automatically
numbers = {4, 7, 4, 9, 7}
# numbers is now {4, 7, 9}

# IMPORTANT: {} creates an empty DICTIONARY, not a set!
empty_dict = {}       # This is a dict
empty_set = set()     # This is a set

# Converting another collection to a set
names_list = ["Alice", "Bob", "Alice", "Charlie"]
names_set = set(names_list)   # {"Alice", "Bob", "Charlie"}
```

### 示例

```python
# Getting the number of unique items using len()
scores = {88, 92, 88, 75, 92, 100}
print(len(scores))   # 4 (duplicates removed: {88, 92, 75, 100})

# Membership testing with 'in'
fruits = {"apple", "banana", "cherry"}
print("banana" in fruits)    # True
print("grape" in fruits)     # False

# Converting a list to a set to find unique elements
letters = ["x", "y", "x", "z", "y", "y"]
unique_letters = set(letters)   # {"x", "y", "z"}
print(len(unique_letters))      # 3

# Iterating over a set
for fruit in fruits:
    print(fruit)
   # Order may vary!

# Sets with tuples (tuples are hashable)
coordinates = {(0, 0), (1, 2), (0, 0)}
print(len(coordinates))   # 2 — duplicate tuple removed
```

### 常见模式

| 操作    | 语法             | 描述                    |
| ----- | -------------- | --------------------- |
| 创建集合  | `{1, 2, 3}`    | 带有元素的花括号              |
| 空集合   | `set()`        | 必须使用 `set()`，而不是 `{}` |
| 从列表转换 | `set(my_list)` | 转换列表，移除重复项            |
| 长度    | `len(my_set)`  | 唯一元素的数量               |
| 成员检查  | `x in my_set`  | 快速检查 `True`/`False`   |
