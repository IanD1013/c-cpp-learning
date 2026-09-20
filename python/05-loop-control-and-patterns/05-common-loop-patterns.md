### 常见循环模式：寻找最大值

许多编程问题都可以归结为几种基本的循环模式：搜索特定值、计算某项内容出现的次数，或者跟踪集合中最大或最小的元素。这些模式无处不在——面试题、实际数据处理以及你已经使用过的各种内置函数中都能见到它们的身影。

自己实现 `max()` 和 `min()` 是最有价值的练习之一。理解它们的底层工作原理不仅能加深你对迭代的理解，还能为你提供一个可以应用于无数类似问题的模板。

### 工作原理

在序列中查找最大值的模式遵循一个简单的策略：

1. 用序列的第一个元素**初始化一个跟踪变量**。
2. **遍历剩余的元素**，将每个元素与你的跟踪变量进行比较。
3. 每当发现更大的值时，**更新跟踪变量**。

当循环结束时，你的跟踪变量保存的就是最大值。这是因为你已经将每个元素与当前的最佳候选值进行了比较，并在出现更好的值时进行了替换。

同样的模式也适用于查找最小值——只需将比较符号从 `>` 变为 `<`。

### 语法

```python
# General pattern for tracking a "best" value through a loop
best = sequence[0]
for item in sequence[1:]:
    if item > best:    # or < for minimum, or any other comparison
        best = item
```

你也可以使用 `range(1, len(sequence))` 从索引 1 开始：

```python
best = sequence[0]
for i in range(1, len(sequence)):
    if sequence[i] > best:
        best = sequence[i]
```

### 示例

以下是应用于不同问题的一些循环模式（不是你需要解决的任务）：

```python
# Counting how many positive numbers are in a list
scores = [85, -2, 90, 0, 73, -5]
count = 0
for score in scores:
    if score > 0:
        count += 1
# count is 3

# Finding the shortest word in a list
words = ["cat", "elephant", "ox", "giraffe"]
shortest = words[0]
for word in words[1:]:
    if len(word) < len(shortest):
        shortest = word
# shortest is "ox"

# Searching for a value
temps = [68, 72, 75, 80, 65]
found_hot = False
for t in temps:
    if t >= 80:
        found_hot = True
        break
# found_hot is True
```

请注意它们的共同结构：在循环前初始化某些内容，在循环内根据条件进行更新，并在循环后使用该结果。

### 实用参考

| 操作            | 描述                | 示例                                    |
| ------------- | ----------------- | ------------------------------------- |
| `str.split()` | 按空白字符将字符串拆分为字符串列表 | `"4 8 2".split()` → `["4", "8", "2"]` |
| `int(s)`      | 将字符串转换为整数         | `int("42")` → `42`                    |
| `list[0]`     | 访问第一个元素           | `[10, 20, 30][0]` → `10`              |
| `list[1:]`    | 从索引 1 开始切片        | `[10, 20, 30][1:]` → `[20, 30]`       |
| `range(1, n)` | 生成从 1 到 n-1 的整数   | `range(1, 4)` → `1, 2, 3`             |
