### 带条件的列表推导式

你已经知道如何使用基础列表推导式（如 `[expr for item in iterable]`）来转换可迭代对象中的每个元素。但如果你只需要其中的*部分*元素呢？在实际编码中，你经常需要过滤数据——提取有效条目、筛选符合条件的记录或剔除异常值。在列表推导式中添加 `if` 子句，可以让你通过单个且可读性强的表达式实现这一目标。

### 工作原理

带条件的列表推导式会对每个元素计算 `if` 子句。只有当条件为 `True` 时，该元素才会进入结果列表中。不符合条件的元素会被静默跳过。可以把它想象成门口的保镖——每个元素都在排队，但只有通过检查的元素才能进入。

### 语法

```python
[expression for item in iterable if condition]
```

这等价于更冗长的循环模式：

```python
result = []
for item in iterable:
    if condition:
        result.append(item)
```

推导式版本将这四行代码缩减为了一行。

你还可以串联多个条件：

```python
[expression for item in iterable if condition1 if condition2]
```

这与 `if condition1 and condition2` 的效果相同。

### 示例

```python
# Keep only positive numbers from a mixed list
numbers = [-5, 3, -1, 7, 0, -2, 9]
positives = [n for n in numbers if n > 0]
# Result: [3, 7, 9]

# Extract words longer than 3 characters
words = ["hi", "hello", "go", "world", "ok", "python"]
long_words = [w for w in words if len(w) > 3]
# Result: ["hello", "world", "python"]

# Filter strings that start with a vowel
names = ["Alice", "Bob", "Eve", "Oscar", "Tina"]
vowel_names = [name for name in names if name[0].lower() in "aeiou"]
# Result: ["Alice", "Eve", "Oscar"]

# Chain two conditions: numbers between 10 and 50 (exclusive)
data = [5, 12, 48, 55, 30, 3, 99, 25]
mid_range = [x for x in data if x > 10 if x < 50]
# Result: [12, 48, 30, 25]
```

### 常见模式

| **模式示例结果** |                                            |         |
| ---------- | ------------------------------------------ | ------- |
| 按值过滤       | `[x for x in nums if x > 0]`               | 保留正数    |
| 按类型检查过滤    | `[x for x in items if isinstance(x, str)]` | 保留字符串   |
| 按成员资格过滤    | `[x for x in words if x in allowed]`       | 保留允许的单词 |
| 使用方法过滤     | `[s for s in lines if s.strip()]`          | 移除空行    |
