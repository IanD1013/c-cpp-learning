### `map()` 内置函数

Python 的 `map()` 函数可以在不编写显式循环的情况下，将某种转换操作应用到可迭代对象中的每一个元素上。它是一种函数式编程工具，可以很自然地与你已经了解的 lambda 函数和具名函数配合使用。虽然在现代 Python 中为了提高可读性通常更推荐使用列表推导式（list comprehension），但当你已经有一个现成的具名函数需要应用时，`map()` 会格外好用——它能让代码保持简洁和声明式风格。

### 工作原理

`map(function, iterable)` 接收两个参数：一个函数和一个可迭代对象（如 list、tuple 或 set）。它会逐个对每个元素调用该函数，并生成一个 **map 对象**，这是一个惰性迭代器（lazy iterator）。这意味着只有在你消费（读取）结果时（通常是使用 `list()` 将其转换为列表），转换操作才会被计算。

你可以把 `map()` 想象成一条传送带：每个元素穿过函数，完成转换，然后从另一端输出。

### 语法

```python
# 通用形式
result = map(function, iterable)

# 由于 map() 返回的是迭代器，因此用 list() 包裹以获取列表
result_list = list(map(function, iterable))

# 配合具名函数使用
list(map(some_function, some_list))

# 配合 lambda 使用
list(map(lambda x: expression, some_list))
```

### 示例

```python
# 使用具名函数：将字符串转换为大写
names = ["alice", "bob", "charlie"]
uppercase_names = list(map(str.upper, names))
# 结果: ['ALICE', 'BOB', 'CHARLIE']

# 使用 lambda：将每个数字翻倍
numbers = [10, 20, 30, 40]
doubled = list(map(lambda x: x * 2, numbers))
# 结果: [20, 40, 60, 80]

# 使用你自定义的具名函数
def add_tax(price):
    return round(price * 1.08, 2)

prices = [9.99, 24.50, 3.75]
with_tax = list(map(add_tax, prices))
# 结果: [10.79, 26.46, 4.05]

# 类型转换：将字符串转换为整数
digits = ["1", "42", "7"]
integers = list(map(int, digits))
# 结果: [1, 42, 7]
```

### `map()` 与列表推导式对比

两者可以达到相同的效果，但各有优势：

```python
# 这两行代码等价：
list(map(lambda x: x * 2, numbers))   # 带有 lambda 的 map
[x * 2 for x in numbers]              # 列表推导式

# 当你已经有具名函数时，map() 更简洁：
list(map(str.strip, lines))            # map —— 简洁明了
[s.strip() for s in lines]            # 推导式 —— 也很不错

# 当转换逻辑较复杂时，列表推导式的可读性更好：
[x * 2 + 1 if x > 0 else 0 for x in numbers]  # 带有条件的推导式
```

通用原则：当应用已存在的函数时使用 `map()`；当转换包含更复杂的表达式或需要过滤条件时，优先使用列表推导式。

### 实用参考

| **模式描述示例**                     |                  |                                            |
| ------------------------------ | ---------------- | ------------------------------------------ |
| `map(func, iterable)`          | 将 `func` 应用到每个元素 | `map(abs, [-1, -2, 3])`                    |
| `list(map(...))`               | 将 map 对象转换为列表    | `list(map(len, ["hi", "bye"]))` → `[2, 3]` |
| `map(lambda x: ..., iterable)` | 内联转换             | `map(lambda x: x + 1, [1, 2])`             |
