### 字符串方法：搜索、拆分、连接和替换

Python 字符串自带了一组丰富的内置方法，用于处理和分析文本。除了你已经知道的基础方法（`upper()`、`lower()`、`strip()`、`len()`）之外，还有五个能够开启强大文本处理能力的方法：`split()`、`join()`、`replace()`、`find()` 和 `count()`。

### 它们如何工作

**`split(delimiter)`** 将字符串拆分为子字符串列表。当不带参数调用时，它会按任意空白字符（空格、制表符、换行符）进行拆分，并自动忽略前导、尾随以及重复的空白字符。当你传入特定的分隔符时，它会精确地按该字符进行拆分。

**`join(iterable)`** 是 `split()` 的逆操作——它在分隔符字符串上调用，并将字符串列表重新拼接在一起，在每个元素之间插入该分隔符。

**`replace(old, new)`** 查找字符串中所有不重叠出现的 `old` 并将其替换为 `new`，返回一个新字符串。

**`find(substring)`** 搜索子字符串首次出现的位置并返回其起始索引。如果未找到子字符串，它会返回 `-1` 而不是引发错误。

**`count(substring)`** 统计子字符串在字符串中不重叠出现的次数。

### 语法

```python
# Splitting
result_list = some_string.split()          # split on whitespace
result_list = some_string.split(",")       # split on comma

# Joining
combined = separator.join(list_of_strings)

# Replacing
new_string = some_string.replace(old_sub, new_sub)

# Finding
index = some_string.find(substring)        # returns -1 if not found

# Counting
num = some_string.count(substring)
```

### 示例

```python
# split() with default whitespace splitting
colors = "red  green   blue".split()
# colors is ["red", "green", "blue"] — extra spaces are handled

# split() with a specific delimiter
csv_row = "Alice,30,Engineer".split(",")
# csv_row is ["Alice", "30", "Engineer"]

# join() to reassemble strings
fruits = ["apple", "banana", "cherry"]
menu = " | ".join(fruits)
# menu is "apple | banana | cherry"

# replace() swaps all occurrences
message = "I like cats and cats like me"
new_msg = message.replace("cats", "dogs")
# new_msg is "I like dogs and dogs like me"

# find() locates the first match
sentence = "the quick brown fox"
pos = sentence.find("quick")
# pos is 4 (the index where "quick" starts)

missing = sentence.find("slow")
# missing is -1 (not found)

# count() tallies occurrences
text = "banana"
num_a = text.count("a")
# num_a is 3
```

### 常见用法

A very common pattern is combining `split()` with `len()` to analyze text structure. Another frequent use is chaining `split()` and `join()` together to reformat text — for example, normalizing irregular spacing.

```python
# Normalize messy whitespace
messy = "  too   many    spaces  "
clean = " ".join(messy.split())
# clean is "too many spaces"
```

### 实用参考

| **方法返回值关键行为**       |       |                      |
| ------------------- | ----- | -------------------- |
| `split()`           | 字符串列表 | 无参数 = 按任意空白字符拆分，移除空项 |
| `split(delim)`      | 字符串列表 | 按精确分隔符拆分             |
| `join(list)`        | 字符串   | 在分隔符上调用，而不是在列表上      |
| `replace(old, new)` | 字符串   | 替换所有出现的位置            |
| `find(sub)`         | int   | 如果未找到则返回 `-1`        |
| `count(sub)`        | int   | 不重叠计数                |
