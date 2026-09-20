### 列表切片

你已经知道如何使用索引来访问列表中的单个元素。但如果你需要列表的*一部分*——比如前三个元素或每隔一个元素呢？这就是**列表切片**（list slicing）的用武之地。它能让你高效且富有表现力地提取子列表，就像你在字符串切片中学到的一样。

### 工作原理

列表切片使用与字符串相同的 `[start:end:step]` 语法。它会创建一个包含所选元素的**新列表**。原始列表绝不会被修改。

* **`start`** — 切片开始的索引（包含）
* **`end`** — 切片停止的索引（不包含 — 该元素*不*包含在内）
* **`step`** — 每次选择元素之间移动的位置步长

这三个部分都是可选的。省略 `start` 默认从列表开头开始。省略 `end` 默认到列表末尾结束。省略 `step` 默认为 `1`。

一个重要的特性：切片**绝不会引发** **`IndexError`**，即使索引超出了列表的长度也是如此。Python 只会返回落在有效范围内的所有元素。

### 语法

```python
# 通用切片语法
new_list = some_list[start:end]
new_list = some_list[start:end:step]

# 省略部分参数
new_list = some_list[:end]      # 从开头到 end（不包括 end）
new_list = some_list[start:]    # 从 start 到末尾
new_list = some_list[::step]    # 带有步长的整个列表
```

### 示例

```python
colors = ["red", "orange", "yellow", "green", "blue", "purple"]

# 前三个元素
colors[:3]          # ["red", "orange", "yellow"]

# 从索引 2 开始及之后
colors[2:]          # ["yellow", "green", "blue", "purple"]

# 中间部分
colors[1:4]         # ["orange", "yellow", "green"]

# 每隔一个元素
colors[::2]         # ["red", "yellow", "blue"]

# 使用负步长会反转遍历方向
colors[4:1:-1]      # ["blue", "green", "yellow"]

# 超出范围的索引不会导致错误
colors[2:100]       # ["yellow", "green", "blue", "purple"]
colors[-100:2]      # ["red", "orange"]
```

### 负步长

当步长为负数时，切片会逆向遍历列表。这意味着 `start` 的索引应该大于 `end` 的索引。负步长最常见的用法是 `[::-1]`，它会从末尾到开头遍历*整个*列表：

```python
numbers = [10, 20, 30, 40, 50]
numbers[3:0:-1]     # [40, 30, 20]  — 从索引 3 开始，在索引 0 之前停止
numbers[::-1]       # [50, 40, 30, 20, 10]  — 整个列表被反转
```

请记住，切片总是会生成一个**新列表**。原始列表保持不变。
