### 元组的不可变性

Python 中的元组（tuple）看起来与列表类似，但它们有一个本质区别：**元组是不可变的（immutable）**。一旦元组被创建，它的内容就无法更改。这使得元组非常适合用来表示在整个程序中应当保持不变的数据，例如 GPS 坐标、RGB 颜色值或数据库连接设置。

### 工作原理

当你尝试直接修改元组时，Python 会抛出 `TypeError`：

```python
coords = (10, 20, 30)
coords[0] = 99  # TypeError: 'tuple' object does not support item assignment
```

元组也缺少列表拥有的修改方法——元组上没有 `append()`、`remove()`、`insert()` 或 `sort()`。如果你需要一个不同版本的元组，你必须**创建一个新元组**。

### 从现有元组创建新元组

由于无法原地修改元组，Python 提供了基于旧元组构建新元组的工具：

**使用 `+` 进行拼接：**

```python
first_half = (1, 2, 3)
second_half = (4, 5, 6)
combined = first_half + second_half  # (1, 2, 3, 4, 5, 6)
```

请记住，在拼接单元素元组时，需要加上末尾逗号：

```python
original = ("red", "green")
extended = original + ("blue",)  # ("red", "green", "blue")
```

**使用切片提取部分元素：**

对元组进行切片会返回一个包含所选元素的**新元组**：

```python
colors = ("red", "green", "blue", "yellow")
last_two = colors[2:]     # ("blue", "yellow")
middle = colors[1:3]      # ("green", "blue")
all_but_last = colors[:-1] # ("red", "green", "blue")
```

### 组合使用这些技术

你可以结合切片和拼接来模拟修改操作：

```python
scores = (85, 90, 78, 92)
# "Replace" the value at index 2 by building a new tuple
updated = scores[:2] + (100,) + scores[3:]  # (85, 90, 100, 92)
```

### 为什么不可变性很重要

| **优势说明**      |                     |
| ------------- | ------------------- |
| 安全性           | 接收元组的函数可以确信数据不会被篡改  |
| 可哈希（Hashable） | 元组可以用作字典的键或集合的元素    |
| 性能            | Python 可以在内部优化不可变对象 |
| 清晰性           | 向其他开发者表明这些数据应当保持不变  |
