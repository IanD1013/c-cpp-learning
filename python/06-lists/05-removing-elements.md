### 从列表中删除元素

列表不仅仅用于添加数据——你还会经常需要删除元素。Python 提供了三种不同的工具来删除元素，每种工具适用于不同的情况。选择哪一种取决于你的目标是**值**、**索引**，还是你需要获取被删除的元素。

### 工作原理

这三种删除方法都会**就地**修改列表，这意味着它们会改变原始列表，而不是创建一个新列表。

**`lst.remove(value)`** — 在列表中搜索与 `value` 匹配的第一个元素并将其删除。如果该值出现多次，则仅删除**第一次出现的项**。如果完全找不到该值，Python 会引发 `ValueError`。

**`lst.pop(index)`** — 删除指定索引处的元素并将其**返回**。如果不带参数调用 `pop()`，它将删除并返回**最后一个**元素。如果索引超出范围，则会引发 `IndexError`。

**`del lst[index]`** — 删除特定索引处的元素。类似于 `pop()`，但**不**返回被删除的元素。你还可以将 `del` 与切片一起使用来删除一段范围内的元素。

### 语法

```python
# Remove by value (first occurrence only)
lst.remove(value)

# Remove by index and get the element back
removed_element = lst.pop(index)
last_element = lst.pop()  # removes last item

# Delete by index (no return value)
del lst[index]
del lst[start:end]  # delete a slice
```

### 示例

```python
# remove() targets a value, not an index
colors = ["red", "blue", "green", "blue"]
colors.remove("blue")
print(colors)  # ["red", "green", "blue"] — only the first "blue" is gone

# pop() removes by index and gives you the element
scores = [95, 87, 72, 91]
dropped = scores.pop(2)
print(dropped)   # 72
print(scores)    # [95, 87, 91]

# pop() with no argument removes the last element
last = scores.pop()
print(last)      # 91
print(scores)    # [95, 87]

# del removes by index without returning anything
fruits = ["apple", "banana", "cherry", "date"]
del fruits[1]
print(fruits)    # ["apple", "cherry", "date"]
```

### 处理不存在的值

对列表中不存在的值调用 `remove()` 会引发 `ValueError`。为了避免程序崩溃，你可以先检查该值是否存在：

```python
names = ["Alice", "Bob", "Charlie"]

# This would crash:
# names.remove("Dave")  # ValueError: list.remove(x): x not in list

# Safe approach: check membership first
if "Dave" in names:
    names.remove("Dave")
print(names)  # ["Alice", "Bob", "Charlie"] — unchanged
```

### 常见模式

| **情况最佳工具原因**     |                  |                       |
| ---------------- | ---------------- | --------------------- |
| 删除已知的值           | `remove()`       | 你知道要删除*什么*，但不知道它在*哪里* |
| 删除指定位置的元素并使用该元素  | `pop(index)`     | 你需要该被删除的值进行进一步处理      |
| 删除指定位置的元素，不需要返回值 | `del lst[index]` | 按位置进行干净的删除            |
| 删除最后一项           | `pop()`          | 类似栈的行为（后进先出）          |
