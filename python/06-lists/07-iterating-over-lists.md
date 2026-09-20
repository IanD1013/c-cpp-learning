### 遍历列表

一旦你掌握了创建、访问和修改列表，下一项必不可少的技能就是**遍历（iterating）**——逐一检查每个元素以进行查看、转换或组合。遍历几乎是所有数据处理任务的核心基础：统计得分、过滤结果、格式化输出等等。

### 工作原理

Python 为遍历列表提供了三种主要模式：

| **模式你获得的内容最适用于**                |                 |           |
| ------------------------------- | --------------- | --------- |
| `for item in lst`               | 直接获得每个值         | 当你只需要值时   |
| `for i in range(len(lst))`      | 每个索引 (0, 1, 2…) | 当你需要位置时   |
| `for i, item in enumerate(lst)` | 索引和值兼具          | 当你同时需要两者时 |

选择能够提供所需信息的最简单模式。在大多数情况下，`for item in lst` 就足够了。

### 语法

```python
# Pattern 1 — iterate over values
for item in lst:
    # do something with item

# Pattern 2 — iterate over indices
for i in range(len(lst)):
    # access lst[i]

# Pattern 3 — iterate with both index and value
for i, item in enumerate(lst):
    # use i and item together
```

### 累加器模式

一种非常常见的技巧是**累加器模式（accumulator pattern）**：在循环*之前*创建一个变量（累加器），在循环*内部*的每次迭代中更新它，并在循环*之后*使用或返回它。

```python
# Example: Count how many words are longer than 4 characters
words = ["apple", "fig", "banana", "kiwi", "pear"]
count = 0                    # accumulator starts at 0
for word in words:
    if len(word) > 4:
        count += 1           # update the accumulator
print(count)                 # 2  ("apple" and "banana")
```

```python
# Example: Build a new string by joining initials
names = ["Alice", "Bob", "Charlie"]
initials = ""                # accumulator starts as empty string
for name in names:
    initials += name[0]      # grab first character
print(initials)              # "ABC"
```

```python
# Example: Find the longest word in a list
fruits = ["mango", "fig", "dragonfruit", "plum"]
longest = ""
for fruit in fruits:
    if len(fruit) > len(longest):
        longest = fruit
print(longest)               # "dragonfruit"
```

注意这个三步节奏：**初始化 → 循环并更新 → 使用结果**。
