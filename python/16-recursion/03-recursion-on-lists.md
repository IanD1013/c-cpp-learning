### 列表上的递归

递归与列表是天然的组合。许多列表操作可以通过将列表拆分为两部分来优雅地表达：**第一个元素**和**其余所有元素**。这种“分治”（divide and conquer）方法让你可以一次处理一个数据片段，并将剩余部分委托给递归调用。

### 工作原理

核心思想很简单：每个非空列表都可以拆分为它的 **head**（头部，第一个元素，`lst[0]`）和 **tail**（尾部，剩余元素，`lst[1:]`）。你处理头部，然后让递归处理尾部。当列表最终为空时，你就到达了**基准情况**（base case），并返回某个单位元/初始值（identity value）。

可以把它想象成一条传送带——你拿起第一件物品，对其进行处理，然后将传送带的其余部分传递下去。

### 语法

```python
def recursive_function(lst):
    if not lst:          # Base case: empty list
        return identity  # Return the appropriate base value
    # Recursive case: process lst[0], recurse on lst[1:]
    return combine(lst[0], recursive_function(lst[1:]))
```

* `lst[0]` 获取第一个元素
* `lst[1:]` 创建一个包含除第一个元素之外的所有元素的新列表
* 当列表为空（`[]`）时，`not lst` 为 `True`

### 示例

以下是一些用递归表达的列表操作——注意其一致的模式：

```python
# Count the number of elements in a list
def recursive_length(lst):
    if not lst:
        return 0
    return 1 + recursive_length(lst[1:])

recursive_length(["a", "b", "c"])  # -> 3
recursive_length([])                # -> 0

# Find the maximum value in a non-empty list
def recursive_max(lst):
    if len(lst) == 1:
        return lst[0]
    rest_max = recursive_max(lst[1:])
    return lst[0] if lst[0] > rest_max else rest_max

recursive_max([3, 7, 2, 9, 1])  # -> 9

# Multiply all elements in a list
def recursive_product(lst):
    if not lst:
        return 1  # Identity value for multiplication
    return lst[0] * recursive_product(lst[1:])

recursive_product([2, 3, 4])  # -> 24
recursive_product([])          # -> 1
```

请注意每个函数如何遵循相同的骨架：检查基准情况，然后将 `lst[0]` 与 `lst[1:]` 上的递归结果进行合并。

### 常见模式

| **模式基准情况返回合并步骤** |                    |                   |
| ---------------- | ------------------ | ----------------- |
| Accumulate（累加）   | 单位元/初始值 (0, 1, "") | 将头部与递归结果合并        |
| Search（搜索）       | `False` 或 `None`   | 检查头部，如果未找到则进行递归   |
| Transform（转换）    | `[]`               | 将转换后的头部添加到递归结果的前面 |

### 性能说明

`lst[1:]` 每次被调用时都会创建一个**新列表**，因此该方法在每次调用时会使用 O(n) 的额外内存。对于生产环境代码，出于性能考虑通常首选迭代。但递归模式在概念上非常清晰，也是推导列表处理的一种强大方式。
