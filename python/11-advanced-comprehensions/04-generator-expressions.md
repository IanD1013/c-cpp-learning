### 生成器表达式 (Generator Expressions)

你已经知道列表推导式如何通过简洁的语法（例如 `[x * 2 for x in items]`）来创建新列表。**生成器表达式（generator expression）在外观上几乎相同，但使用的是圆括号**而不是方括号。关键区别在于它们处理内存的方式：列表推导式会一次性构建整个列表，而生成器表达式则是**按需逐个（one at a time, on demand）**生成值——这被称为*惰性求值（lazy evaluation）*。对于大型数据集，这可以显著节省内存，因为你永远不需要同时将所有值保留在内存中。

### 工作原理

当 Python 遇到生成器表达式时，它不会立即计算任何内容。相反，它会创建一个**生成器对象（generator object）**——这是一种特殊的迭代器（iterator），仅在被请求时才会产出下一个值。每次使用者（例如 `sum()` 或 `for` 循环）请求下一个值时，生成器都会计算该值、将其递交，然后暂停直到下一次请求。一旦所有值都已生成，生成器就会**耗尽（exhausted）**，无法再次重复使用。

可以把它想象成一个出票机：它不会一次性打印出所有票——每当有人按下按钮时，它才会打印一张票，一旦票用光了，就结束了。

### 语法

```python
# General syntax — note the parentheses
gen = (expression for item in iterable)

# With a condition
gen = (expression for item in iterable if condition)
```

### 示例

```python
# Generator expression assigned to a variable
doubles = (n * 2 for n in range(5))
print(type(doubles))  # <class 'generator'>

# Convert to a list to see all values
print(list(doubles))  # [0, 2, 4, 6, 8]

# IMPORTANT: the generator is now exhausted
print(list(doubles))  # [] — empty, can only iterate once!

# Using a generator directly inside max()
prices = [12.50, 8.99, 23.00, 5.75]
highest_discounted = max(p * 0.9 for p in prices)
print(highest_discounted)  # 20.7

# When a generator is the ONLY argument to a function,
# you can omit the extra parentheses:
total_length = sum(len(word) for word in ["hello", "world"])
print(total_length)  # 10

# Compare: with extra parentheses (also valid but redundant)
total_length = sum((len(word) for word in ["hello", "world"]))

# Using any() with a generator to check a condition
temperatures = [18, 22, 35, 27]
is_hot = any(t > 30 for t in temperatures)
print(is_hot)  # True
```

### 接收生成器的内置函数

| **函数描述示例** |                  |                                          |
| ---------- | ---------------- | ---------------------------------------- |
| `sum()`    | 对所有值求和           | `sum(x for x in range(4))` → 6           |
| `min()`    | 找出最小值            | `min(abs(x) for x in [-3, 1, -7])` → 1   |
| `max()`    | 找出最大值            | `max(len(s) for s in ["hi", "hey"])` → 3 |
| `any()`    | 如果有任何值为真则返回 True | `any(x > 0 for x in [-1, 2])` → True     |
| `all()`    | 如果所有值都为真则返回 True | `all(x > 0 for x in [1, 2])` → True      |
| `list()`   | 将所有值具体化为一个列表     | `list(x + 1 for x in [0, 1])` → [1, 2]   |
| `tuple()`  | 将所有值具体化为一个元组     | `tuple(x ** 2 for x in [2, 3])` → (4, 9) |
