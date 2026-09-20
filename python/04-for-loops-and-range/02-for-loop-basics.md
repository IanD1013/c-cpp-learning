### `for` 循环与 `range()`

`for` 循环会遍历一个序列，并针对序列中的每个元素执行一次其循环体。当将其与 `range()` 搭配使用时，你将得到一个**计数循环** —— 即运行特定次数的循环。当预先知道需要执行多少次迭代时，这是最常用的工具，对于计数重复场景而言，它比 `while` 循环更加简洁且不易出错。

### 工作原理

`for` 循环从序列中依次获取每个元素，将其赋值给一个**循环变量**，并运行缩进的代码块。该代码块执行完毕后，循环会自动移至下一个元素 —— 无需手动递增。当序列中没有更多元素时，循环结束，并继续执行缩进代码块之后的代码。

这是与 `while` 的一个关键区别：你不需要在循环之前初始化计数器，也不需要记得在循环体内对其进行更新。`for` 循环会自动处理所有这些事情。

### 语法

```python
for variable in sequence:
    # This indented block runs once per item
    # 'variable' holds the current item
```

借助 `range()`，你可以控制循环变量取哪些值：

```python
# range(stop) — values from 0 up to (but not including) stop
for i in range(5):
    # i takes values: 0, 1, 2, 3, 4

# range(start, stop) — values from start up to (but not including) stop
for i in range(2, 6):
    # i takes values: 2, 3, 4, 5

# range(start, stop, step) — values from start, incrementing by step
for i in range(1, 10, 2):
    # i takes values: 1, 3, 5, 7, 9
```

### 示例

```python
# Sum numbers from 1 to 100
total = 0
for num in range(1, 101):
    total += num
print(total)  # 5050

# Count down from 5 to 1
for countdown in range(5, 0, -1):
    print(countdown)  # Prints 5, 4, 3, 2, 1

# Build a string by repeating a character
stars = ""
for i in range(4):
    stars += "*"
print(stars)  # "****"

# Accumulate a product
product = 1
for x in range(1, 4):
    product *= x
print(product)  # 6 (which is 1 * 2 * 3)
```

### 常见模式

**累加器模式（Accumulator pattern）：** 在循环之前初始化一个变量，然后在每次迭代中更新它。这就是计算累计总和、乘积或逐字符构建字符串的方法。

```python
# Accumulate a sum
result = 0
for i in range(1, 6):
    result += i
# result is now 15

# Accumulate a product
result = 1
for i in range(1, 6):
    result *= i
# result is now 120
```

请注意初始值的重要性：对于求和，初始值为 `0`（因为加上 0 不会改变结果）；对于求乘积，初始值为 `1`（因为乘以 1 不会改变结果）。
