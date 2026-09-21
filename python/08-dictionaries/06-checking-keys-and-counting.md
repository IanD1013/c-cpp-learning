### 检查键是否存在并使用字典进行计数

在使用字典时，在尝试更新某个键的值之前，你通常需要知道该键是否已经存在。当你进行计数统计时尤为常见——例如统计文档中的单词数、跟踪库存或汇总问卷调查结果。Python 为此提供了简洁、易读的工具。

### 工作原理

`in` 运算符用于检查键是否存在于字典中。如果键存在，它将返回 `True`，否则返回 `False`。这只会检查**键**，而不会检查值。

```python
scores = {"alice": 95, "bob": 82}

"alice" in scores   # True
"charlie" in scores # False
95 in scores        # False — checks keys, not values
```

当将其与条件逻辑结合起来逐步构建字典时，它会变得非常强大。

### 计数模式

Python 中一个非常常见的模式是遍历一个序列并统计每个元素出现的次数。传统方法是在更新前使用 `in` 进行检查：

```python
fruits = ["apple", "banana", "apple", "cherry", "banana", "apple"]
fruit_counts = {}

for fruit in fruits:
    if fruit in fruit_counts:
        fruit_counts[fruit] += 1
    else:
        fruit_counts[fruit] = 1

# fruit_counts is now {'apple': 3, 'banana': 2, 'cherry': 1}
```

请注意这里的逻辑：如果键已经存在，我们就将其递增。如果不存在，我们就将其初始化为 `1`。

### 使用 `setdefault` 进行简化

Python 字典有一个名为 `setdefault(key, default)` 的方法，可以简化这个过程。它的工作方式如下：

* 如果 `key` **存在**于字典中，它会返回当前值（不做其他操作）。
* 如果 `key` **不存在**，它会插入带有值 `default` 的 `key` 并返回 `default`。

```python
warehouse = {"screws": 200}

warehouse.setdefault("screws", 0)   # Returns 200 — key exists, no change
warehouse.setdefault("bolts", 0)    # Returns 0 — inserts {"bolts": 0}

# warehouse is now {'screws': 200, 'bolts': 0}
```

在计数时，`setdefault` 允许你在递增之前确保键具有初始值：

```python
colors = ["red", "blue", "red", "green", "blue", "red"]
color_counts = {}

for color in colors:
    color_counts.setdefault(color, 0)
    color_counts[color] += 1

# color_counts is now {'red': 3, 'blue': 2, 'green': 1}
```

这完全消除了 `if/else` 分支——调用 `setdefault` 确保了在你给键加 `1` 之前，该键已经存在且值为 `0`。

### 两种方法的对比

| 方法                   | 优点          | 缺点         |
| -------------------- | ----------- | ---------- |
| `if key in dict`     | 非常明确且易读     | 稍微繁琐一些     |
| `setdefault(key, 0)` | 简洁，一行代码即可设置 | 对初学者来说不太直观 |

两种方法都完全可行。根据你的使用场景选择更清晰易读的方式即可。
