### 累加器模式

累加器模式是编程中最基础的技术之一。其核心思想很简单：你在循环**之前**创建一个变量来保存不断增长的结果，然后在每次循环迭代中**更新**该变量。当循环结束时，该变量就会包含你的最终结果。你会经常使用这种模式——用于数字求和、构建字符串、收集元素等等。

### 工作原理

该模式包含三个部分：

1. 在循环前**初始化**一个累加器变量（选择一个合理的起始值——求和为 `0`，字符串为 `""`）
2. 在每次迭代中，在循环体内部**更新**累加器
3. 在循环结束后**使用**最终累积的值

可以把它想象成一次一勺地往桶里装水。你从一个空桶开始，每次循环加一勺，最后你会得到一个装满的桶。

### 语法

```python
# Numeric accumulator
accumulator = 0
for item in some_sequence:
    accumulator += some_value

# String accumulator
result = ""
for item in some_sequence:
    result += some_string
```

### 示例

```python
# Example 1: Sum all numbers from 1 to 5
total = 0
for num in range(1, 6):
    total += num
# total is now 15 (1 + 2 + 3 + 4 + 5)

# Example 2: Extract only uppercase letters from a sentence
uppers = ""
for char in "Hello World":
    if char.isupper():
        uppers += char
# uppers is now "HW"

# Example 3: Build a countdown string
countdown = ""
for i in range(3, 0, -1):
    countdown += str(i) + "... "
# countdown is now "3... 2... 1... "
```

请注意每个示例是如何从一个空的累加器开始，在循环中逐渐累积，并最终得到完整结果的。

### 常见模式

| 起始值  | 使用场景  | 更新操作               |
| ---- | ----- | ------------------ |
| `0`  | 数字求和  | `total += value`   |
| `""` | 构建字符串 | `result += text`   |
| `1`  | 计算乘积  | `product *= value` |

关键的理解是 `range(n)` 恰好为你提供 `n` 次迭代——`range(0)` 提供零次迭代，`range(1)` 提供一次，`range(3)` 提供三次，依此类推。这使得它在需要将某项操作重复特定次数时非常合适。
