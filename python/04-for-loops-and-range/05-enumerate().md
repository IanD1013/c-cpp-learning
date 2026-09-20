### `enumerate()` 函数

在 Python 中遍历序列时，你经常需要知道自己处于*哪个位置*——而不仅仅是当前项*是什么*。例如，你可能想指出列表中某个值出现的位置，或者构建一个带编号的项目列表显示。Python 内置的 `enumerate()` 函数可以同时为你提供索引和值，而无需繁琐的手动索引。

### 工作原理

如果不使用 `enumerate()`，你可能会编写如下代码来同时获取索引和值：

```python
colors = ["red", "green", "blue"]
for i in range(len(colors)):
    print(i, colors[i])
```

这样可行，但代码冗长且容易出错（例如差一错误、忘记对列表取索引等）。`enumerate()` 可以包装任何可迭代对象，并在每次迭代时生成 `(index, item)` 对。Python 允许你将这两个值直接**解包**到两个循环变量中——这被称为**元组解包**（**tuple unpacking**）。

### 语法

```python
for index, item in enumerate(sequence):
    # index is 0, 1, 2, ...
    # item is the current element from the sequence
```

你还可以更改计数的起始位置：

```python
for index, item in enumerate(sequence, start=1):
    # index is 1, 2, 3, ...
```

### 示例

```python
# Example 1: Numbering items in a grocery list
groceries = ["milk", "eggs", "bread"]
for i, item in enumerate(groceries):
    print(f"Item {i}: {item}")
# Output:
# Item 0: milk
# Item 1: eggs
# Item 2: bread

# Example 2: Finding which day has the highest temperature
temps = [72, 85, 79, 91, 68]
hottest_day = 0
hottest_temp = temps[0]
for i, t in enumerate(temps):
    if t > hottest_temp:
        hottest_temp = t
        hottest_day = i
print(f"Hottest on day {hottest_day}: {hottest_temp}°F")
# Output: Hottest on day 3: 91°F

# Example 3: Using start=1 for human-friendly numbering
winners = ["Alice", "Bob", "Carol"]
for place, name in enumerate(winners, start=1):
    print(f"{place}st/nd/rd: {name}")
# Output:
# 1st/nd/rd: Alice
# 2st/nd/rd: Bob
# 3st/nd/rd: Carol
```

### 元组解包

`for i, char in enumerate("abc")` 之所以有效，是因为 `enumerate()` 会生成形如 `(0, "a")`、`(1, "b")`、`(2, "c")` 的元组。Python 会自动将每个元组拆分到 `i` 和 `char` 这两个变量中。这被称为**元组解包**（**tuple unpacking**），也是你在 Python 中随处可见的核心模式。
