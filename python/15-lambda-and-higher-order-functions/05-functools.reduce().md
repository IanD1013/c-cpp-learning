### 导入和使用 `functools.reduce()`

并不是 Python 中的每个函数都是默认可用的。有些函数位于 **模块（modules）** 中 —— 即将相关功能组合在一起的 Python 代码文件。要使用模块中的代码，你需要 **导入（import）** 它。`reduce()` 函数位于 `functools` 模块中，这是了解导入工作原理的绝佳切入点。

### 导入的工作原理

Python 主要有两种导入方式：

```python
import math          # imports the whole module — use as math.sqrt(16)
from math import sqrt  # imports just sqrt — use directly as sqrt(16)
```

当你只需要模块中的一两项内容时，`from module import name` 形式非常方便。对于 `reduce()`，你可以这样写：

```python
from functools import reduce
```

在此行之后，`reduce` 就可以作为普通函数在你的代码中使用了。

### `reduce()` 的工作原理

`reduce()` 接收一个可迭代对象，并通过重复应用一个接受两个参数的函数将其 **折叠（collapses）** 为单个值。它从左到右处理元素，并持续传递累积的结果。

把它想象成一个滚下山坡的雪球 —— 在每一步中，它都会拾取下一个元素并将其与现有的部分结合在一起。

### 语法

```python
from functools import reduce

reduce(function, iterable)
reduce(function, iterable, initial_value)
```

* **function**：一个接收两个参数的函数 —— 累加器（accumulator）和当前元素
* **iterable**：要归约的序列
* **initial_value**（可选）：累加器的起始值。如果提供，它将在处理任何元素之前充当第一个“累加器”

### 示例

```python
from functools import reduce

# Sum all numbers: ((1 + 2) + 3) + 4 = 10
total = reduce(lambda acc, x: acc + x, [1, 2, 3, 4])
# total is 10

# Find the maximum: compares pairs from left to right
largest = reduce(lambda acc, x: acc if acc > x else x, [3, 7, 2, 9, 1])
# largest is 9

# Concatenate strings: (("a" + "b") + "c") + "d" = "abcd"
word = reduce(lambda acc, ch: acc + ch, ["a", "b", "c", "d"])
# word is "abcd"
```

### 初始值 —— 为什么它很重要

如果没有初始值，`reduce()` 会使用可迭代对象的第一个元素作为起始累加器。这意味着在 **空可迭代对象** 上调用 `reduce()` 会引发 `TypeError`。

可选的第三个参数解决了这个问题：

```python
# Without initial value — crashes on empty list!
# reduce(lambda a, b: a + b, [])  # TypeError!

# With initial value — returns 0 safely
result = reduce(lambda a, b: a + b, [], 0)
# result is 0

# With a non-empty list, the initial value is used first:
# reduce(lambda a, b: a + b, [5, 10], 100)
# computes: ((100 + 5) + 10) = 115
```

初始值在处理任何元素之前充当累加器的起点。请选择一个作为运算 **单位元（identity element）** 的初始值 —— 即不会改变结果的值（加法为 0，乘法为 1，字符串拼接为 `""`）。

### 注意事项

`reduce()` 可能会使代码更难阅读。对于求和等简单操作，Python 的内置函数 `sum()` 更加清晰。当你需要没有内置等效项的自定义累积时，才使用 `reduce()`。
