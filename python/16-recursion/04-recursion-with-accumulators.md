### 递归函数中的累加器

当你编写递归函数时，通常是在每个递归调用**返回的途中**构建结果——最深层的调用最先返回，每个调用方将其部分与返回的结果合并。而**累加器模式**则颠倒了这一过程：你将部分结果作为参数**向前**传递，在深入递归的**过程中**构建结果。当达到基本情况（base case）时，累加器已经保存了最终答案。

这种模式非常类似于支持尾递归优化的语言中的**尾递归**。尽管 Python 不执行尾调用优化，但累加器风格对许多问题来说可能更加直观，是一项值得掌握的重要技巧。

### 工作原理

可以将累加器视为一个实时计数器或一个不断增长的容器，它伴随着不断缩小的输入一起传递。每次递归调用会：

1. 从输入中取出一部分
2. 将该部分合并到累加器中
3. 将变小的输入和更新后的累加器传递给下一次调用

当输入完全被消耗（空列表、空字符串、零等）时，累加器就完成了——只需将其返回即可。

### 语法

```python
# Using a default parameter as the accumulator
def my_function(data, acc=initial_value):
    if base_condition(data):
        return acc
    piece = extract_piece(data)
    remaining = remove_piece(data)
    return my_function(remaining, combine(acc, piece))
```

或者，你也可以使用一个**辅助函数**：

```python
def my_function(data):
    def helper(data, acc):
        if base_condition(data):
            return acc
        piece = extract_piece(data)
        remaining = remove_piece(data)
        return helper(remaining, combine(acc, piece))
    return helper(data, initial_value)
```

### 示例

```python
# Summing a list with an accumulator
def total(numbers, acc=0):
    if numbers == []:
        return acc
    return total(numbers[1:], acc + numbers[0])

total([3, 7, 2])   # -> 12
# Call 1: total([3, 7, 2], 0)  -> acc becomes 0 + 3 = 3
# Call 2: total([7, 2], 3)     -> acc becomes 3 + 7 = 10
# Call 3: total([2], 10)       -> acc becomes 10 + 2 = 12
# Call 4: total([], 12)        -> base case, return 12

# Flattening nested items with an accumulator
def collect_names(people, acc=""):
    if people == []:
        return acc.strip()
    name = people[0]
    return collect_names(people[1:], acc + name + " ")

collect_names(["Ada", "Grace", "Lina"])  # -> "Ada Grace Lina"

# Counting occurrences with a helper function
def count_char(text, target):
    def helper(text, acc):
        if text == "":
            return acc
        increment = 1 if text[0] == target else 0
        return helper(text[1:], acc + increment)
    return helper(text, 0)

count_char("banana", "a")  # -> 3
```

### 对比非累加器风格与累加器风格

| **风格结果构建于...基本情况返回...** |          |                    |
| ----------------------- | -------- | ------------------ |
| 标准递归                    | **回溯**途中 | 一个简单值（例如 `0`、`""`） |
| 累加器模式                   | **深入**途中 | **累加器**（最终完成的结果）   |

### 核心要点

请注意，在累加器风格中，递归调用是发生的**最后一件事**——返回后没有额外的工作需要处理。累加器承载了所有状态，因此基本情况只需直接将其返回即可。
