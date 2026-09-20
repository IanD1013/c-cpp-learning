### 访问列表中的元素

列表按顺序存储多个值，您通常需要从该有序序列中检索特定项。Python 允许您通过**索引（index）**——一个表示元素位置的数字——访问列表并提取其中的任何元素。

### 工作原理

列表中的每个元素都有一个称为**索引（index）**的位置编号。Python 使用**从零开始的索引（zero-based indexing）**，这意味着第一个元素位于索引 `0`，第二个元素位于索引 `1`，依此类推。您可以通过在列表名称后面的方括号内放置索引来访问元素。

Python 还支持**负索引（negative indexing）**。负索引不是从开头计数，而是从列表末尾向前倒数。索引 `-1` 表示最后一个元素，`-2` 表示倒数第二个元素，依此类推。

如果您尝试访问不存在的索引（例如在只有 3 个元素的列表中访问索引 `5`），Python 将引发 `IndexError`。

### 语法

```python
# Positive indexing (from the start)
my_list[0]    # First element
my_list[1]    # Second element
my_list[2]    # Third element

# Negative indexing (from the end)
my_list[-1]   # Last element
my_list[-2]   # Second-to-last element
```

### 示例

```python
colors = ["red", "green", "blue", "yellow"]

colors[0]     # "red" — the first element
colors[2]     # "blue" — the third element
colors[-1]    # "yellow" — the last element
colors[-3]    # "green" — third from the end

# Combining indexing with len()
size = len(colors)   # 4
colors[size - 1]     # "yellow" — equivalent to colors[-1]

# Using indexed values in expressions
prices = [4.99, 12.50, 7.25]
cheapest_and_last = prices[0] + prices[-1]  # 4.99 + 7.25 = 12.24
```

### 常见模式

| **模式描述**            |                           |
| ------------------- | ------------------------- |
| `lst[0]`            | 获取第一个元素                   |
| `lst[-1]`           | 获取最后一个元素                  |
| `lst[len(lst) - 1]` | 同样获取最后一个元素（等同于 `lst[-1]`） |
| `lst[i]`            | 获取位置 `i` 处的元素             |

当您需要最后一个元素但不知道（或不想计算）列表的长度时，负索引尤其有用。
