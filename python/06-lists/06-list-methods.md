### 常用列表方法

Python 列表除了添加和删除元素之外，还提供了几个内置方法。这些方法让你可以对数据进行排序、搜索值、统计出现次数等。掌握它们意味着你可以高效地操作列表，而无需编写复杂的循环。

### 工作原理

某些列表方法会**就地修改列表**（它们会改变原始列表并返回 `None`），而其他方法则会**返回一个新值**而不改变原始列表。理解这种区别至关重要——将就地修改方法的结果赋值给变量是一个非常常见的初学者错误。

### 语法

```python
# Sorting
lst.sort()                # Sorts ascending, modifies in place, returns None
lst.sort(reverse=True)    # Sorts descending, modifies in place, returns None

# Reversing
lst.reverse()             # Reverses in place, returns None

# Searching and counting
lst.index(value)          # Returns the index of the first occurrence of value
lst.count(value)          # Returns how many times value appears in the list

# Copying
lst.copy()                # Returns a shallow copy of the list
```

### 示例

```python
# Sorting a list of temperatures
temps = [72, 65, 80, 65, 90]
temps.sort()
print(temps)        # [65, 65, 72, 80, 90]

# DANGER: sort() returns None!
result = temps.sort()
print(result)       # None  <-- This is a very common mistake!

# Counting occurrences of a grade
grades = ["A", "B", "A", "C", "A", "B"]
print(grades.count("A"))    # 3
print(grades.count("D"))    # 0  (not found, returns 0)

# Finding the position of an element
colors = ["red", "green", "blue", "green"]
print(colors.index("blue"))   # 2
print(colors.index("green"))  # 1  (first occurrence only)

# Reversing a list
nums = [10, 20, 30]
nums.reverse()
print(nums)         # [30, 20, 10]

# Making a copy to avoid modifying the original
original = [1, 2, 3]
backup = original.copy()
backup.sort(reverse=True)
print(original)     # [1, 2, 3]  (unchanged)
print(backup)       # [3, 2, 1]
```

### 常见陷阱

| 错误发生的情况                           | 结果              | 正确做法                                     |
| --------------------------------- | --------------- | ---------------------------------------- |
| `lst = lst.sort()`                | `lst` 变为 `None` | 单独一行调用 `lst.sort()`                      |
| `lst = lst.reverse()`             | `lst` 变为 `None` | 单独一行调用 `lst.reverse()`                   |
| 当 `val` 不在列表中时调用 `lst.index(val)` | 抛出 `ValueError` | 先使用 `val in lst` 检查，或使用 `lst.count(val)` |
