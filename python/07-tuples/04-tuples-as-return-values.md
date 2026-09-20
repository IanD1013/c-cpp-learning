### 元组作为返回值

Python 函数只能返回一个单独的对象，但如果你需要返回两个或更多相关的具体信息该怎么办？惯用的解决方案是返回一个**元组（tuple）**。这种模式在 Python 中随处可见——从内置函数如 `divmod()` 到诸如 `os.path.splitext()` 之类的库。每当一个函数计算出多个相关的结果时，将它们打包成一个元组可以保持代码的整洁与富有表现力。

### 工作原理

当你在函数内部编写 `return a, b` 时，Python 会自动将 `a` 和 `b` 打包为一个元组 `(a, b)` 并返回该单独的元组对象。调用方随后可以在一行代码中将结果**解包（unpack）**到不同的变量中。这与你已经了解的元组打包/解包完全相同——只是它发生在函数调用的边界上。

编写 `return a, b` 与 `return (a, b)` 是完全等价的。圆括号是可选的，为了提高可读性通常会省略。

### 语法

```python
def compute_something(data):
    # ... calculate first_result and second_result ...
    return first_result, second_result   # returns a tuple

# Caller unpacks into two variables:
x, y = compute_something(my_data)

# Or keeps it as a tuple:
result = compute_something(my_data)   # result is (first_result, second_result)
```

### 示例

```python
# Example 1: Return the sum and product of two numbers
def sum_and_product(a, b):
    return a + b, a * b

s, p = sum_and_product(3, 4)
# s = 7, p = 12

# Example 2: Split a full name into first and last
def split_name(full_name):
    parts = full_name.split()
    return parts[0], parts[-1]

first, last = split_name("Ada Lovelace")
# first = "Ada", last = "Lovelace"

# Example 3: Count positives and negatives in a list
def count_signs(numbers):
    pos = 0
    neg = 0
    for n in numbers:
        if n > 0:
            pos += 1
        elif n < 0:
            neg += 1
    return pos, neg

p, n = count_signs([5, -2, 0, 8, -1])
# p = 2, n = 2
```

### 常见模式

在遍历集合查找特定值时，典型的做法是：

1. 使用集合的第一个元素来初始化跟踪变量。
2. 遍历剩余的元素，当找到更符合条件的候选值时更新变量。
3. 将结果作为元组返回。

这种“先初始化后扫描”的模式避免了选择任意初始值带来的问题，并且适用于任何非空集合。
