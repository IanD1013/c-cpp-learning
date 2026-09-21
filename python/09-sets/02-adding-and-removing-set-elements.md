### 修改集合：添加、删除与实际应用

集合不仅用于创建——当你修改它们时，它们会变得非常强大。Python 提供了多种添加和删除元素的方法，每种方法的行为都略有不同。理解这些差异有助于你选择合适的工具并避免运行时错误。

### 集合修改的工作原理

与列表不同，集合会自动强制唯一性。当你添加一个已存在的元素时，集合只会忽略它——既不会报错，也不会产生重复。删除操作则更有趣：Python 提供了多种删除元素的方法，每种方法处理缺失元素的方式各不相同。

### 语法

```python
# Adding elements
my_set.add(element)        # Adds element; no effect if already present

# Removing elements
my_set.remove(element)     # Removes element; raises KeyError if missing
my_set.discard(element)    # Removes element; does nothing if missing (safer)
my_set.pop()               # Removes and returns an arbitrary element
my_set.clear()             # Removes all elements, leaving an empty set
```

### 示例

```python
# add() silently handles duplicates
colors = {"red", "blue"}
colors.add("green")        # {"red", "blue", "green"}
colors.add("red")          # {"red", "blue", "green"} — no change

# remove() vs discard()
fruits = {"apple", "banana", "cherry"}
fruits.remove("banana")    # {"apple", "cherry"}
fruits.discard("mango")    # {"apple", "cherry"} — no error
# fruits.remove("mango")  # This would raise KeyError!

# pop() grabs an arbitrary element
nums = {10, 20, 30}
val = nums.pop()           # val is some element, nums now has 2 elements

# clear() empties everything
nums.clear()               # set()

# Converting between lists and sets for deduplication
scores = [88, 92, 88, 75, 92, 100]
unique_scores = set(scores)          # {88, 92, 75, 100}
sorted_unique = sorted(unique_scores) # [75, 88, 92, 100]
```

### 常见模式

实际应用中常见的模式是清理包含重复项的数据。由于集合会自动消除重复项，因此将集合转换再转换回来是一种简洁的去重方式。内置的 `sorted()` 函数可以接收任何可迭代对象（包括集合）并返回一个新的有序列表。

```python
# Deduplicating user input
raw_tags = ["python", "code", "python", "data", "code"]
clean_tags = sorted(set(raw_tags))  # ["code", "data", "python"]

# Removing duplicates from numeric data
readings = [4.5, 3.2, 4.5, 6.1, 3.2, 6.1]
unique_readings = sorted(set(readings))  # [3.2, 4.5, 6.1]
```
