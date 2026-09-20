### `range()` 函数

Python 的 `range()` 函数可以按需生成一个整数序列。它是 Python 中最常用的工具之一，特别是当你需要重复执行某项操作指定次数，或者遍历一系列可预测的数字时。与列表不同，`range()` 不会一次性将所有数字存储在内存中——而是根据需要逐个生成。

### 工作原理

`range()` 函数有三种形式，取决于你传入的参数数量：

| 形式                         | 生成内容                                  | 关键细节              |
| -------------------------- | ------------------------------------- | ----------------- |
| `range(stop)`              | 从 `0` 到 `stop - 1` 的数字                | 默认从 0 开始          |
| `range(start, stop)`       | 从 `start` 到 `stop - 1` 的数字            | `stop` 值被**排除**在外 |
| `range(start, stop, step)` | 从 `start` 到 `stop - 1`、步长为 `step` 的数字 | 可以使用负步长进行倒序计数     |

最需要记住的一点是：**stop 值永远不会包含**在序列中。如果你想包含它，需要给 stop 加 1。

### 语法

```python
# Basic form: 0 to stop-1
range(stop)

# Two-argument form: start to stop-1
range(start, stop)

# Three-argument form: start to stop-1, stepping by step
range(start, stop, step)
```

### 示例

```python
# range(stop) — generates 0, 1, 2, 3, 4
for i in range(5):
    print(i)  # prints 0 through 4

# range(start, stop) — generates 2, 3, 4, 5
for i in range(2, 6):
    print(i)  # prints 2 through 5 (6 is excluded!)

# range(start, stop, step) — generates 0, 2, 4, 6, 8
for i in range(0, 10, 2):
    print(i)  # prints even numbers from 0 to 8

# Negative step — counts backward: 10, 8, 6, 4, 2
for i in range(10, 0, -2):
    print(i)  # prints 10 down to 2 (0 is excluded)
```

### 将 `range()` 与 For 循环结合使用

将 `for` 循环与 `range()` 结合使用，可以遍历序列中的每个数字。你可以使用循环变量来累加结果、构建字符串或执行计算：

```python
# Counting letters in a word by position
word = "hello"
for i in range(len(word)):
    print(f"Position {i}: {word[i]}")

# Building a product of numbers from 1 to 4 (factorial of 4)
product = 1
for n in range(1, 5):
    product = product * n
# product is now 24 (1 * 2 * 3 * 4)
```

### 常见模式

* **包含终点**：由于 `range(start, stop)` 会排除 `stop`，因此当你想包含它时，请使用 `range(start, stop + 1)`。
* **累加结果**：在循环前初始化一个变量，然后在每次循环迭代中更新它。
* **空范围**：如果 `start >= stop`（步长为正数时），`range` 完全不会生成任何值——循环体永远不会执行。
