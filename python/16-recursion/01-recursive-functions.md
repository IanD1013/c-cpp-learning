### 递归函数

**递归函数**（recursive function）是指通过调用自身将问题分解为更小、相同的子问题来求解的函数。递归是一种强大的技术，广泛应用于树遍历、搜索、排序和数学计算等算法中。当问题具有自然的自引用结构时，它提供了一种比循环更优雅的替代方案。

### 工作原理

每个递归函数都包含两个必不可少的部分：

1. **基线条件**（Base case）——函数*不*调用自身而直接返回值的条件。这是终止递归的关键。
2. **递归条件**（Recursive case）——函数使用原始问题的*更小*或*更简单*的版本调用自身，每次调用都更接近基线条件。

如果没有基线条件，函数将无休止地调用自身，直到 Python 抛出 `RecursionError`（Python 的默认递归限制约为 1000 次调用）。

可以把它想象成俄罗斯套娃——你不断打开更小的套娃，直到拿到最小的那一个（基线条件），然后再一层层收回去。

### 语法

```python
def recursive_function(parameter):
    if base_condition:       # Base case
        return base_value
    return some_operation(recursive_function(smaller_parameter))  # Recursive case
```

### 示例

**倒计时：**

```python
def countdown(n):
    if n <= 0:            # Base case: stop at zero
        return "Go!"
    return str(n) + " " + countdown(n - 1)  # Recursive case

# countdown(3) -> "3 2 1 Go!"
# Call chain: countdown(3) -> "3 " + countdown(2)
#                           -> "3 " + "2 " + countdown(1)
#                           -> "3 " + "2 " + "1 " + countdown(0)
#                           -> "3 " + "2 " + "1 " + "Go!"
```

**列表求和：**

```python
def sum_list(items):
    if len(items) == 0:   # Base case: empty list sums to 0
        return 0
    return items[0] + sum_list(items[1:])  # Recursive case

# sum_list([10, 20, 30]) -> 60
# Call chain: 10 + sum_list([20, 30])
#          -> 10 + 20 + sum_list([30])
#          -> 10 + 20 + 30 + sum_list([])
#          -> 10 + 20 + 30 + 0
```

**幂函数：**

```python
def power(base, exp):
    if exp == 0:          # Base case: anything to the power of 0 is 1
        return 1
    return base * power(base, exp - 1)  # Recursive case

# power(2, 4) -> 16
# Call chain: 2 * power(2, 3)
#          -> 2 * 2 * power(2, 2)
#          -> 2 * 2 * 2 * power(2, 1)
#          -> 2 * 2 * 2 * 2 * power(2, 0)
#          -> 2 * 2 * 2 * 2 * 1
```

### 追踪递归调用

理解递归需要追踪调用链。以计算 4 的阶乘（数学中写作 `4!`）为例：

* 一个数的**阶乘**是从 1 到该数字之间所有正整数的乘积
* `4! = 4 × 3 × 2 × 1 = 24`
* 根据定义，`0! = 1`

递归分解过程：

```scss
factorial(4)
  -> 4 * factorial(3)
  -> 4 * 3 * factorial(2)
  -> 4 * 3 * 2 * factorial(1)
  -> 4 * 3 * 2 * 1 * factorial(0)
  -> 4 * 3 * 2 * 1 * 1          # Base case reached!
  -> 24                          # Results bubble back up
```

每次调用都需要等待下一次调用返回后才能计算出自己的结果。一旦基线条件返回，结果就会层层回溯传递给每一个等待的调用。
