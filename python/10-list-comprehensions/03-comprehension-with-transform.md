### 带有转换操作的列表推导式

你已经了解了如何使用带有条件的列表推导式来过滤元素。但推导式的真正强大之处在于 **expression**（表达式）部分 —— 即 `for` 关键字前面的部分。该表达式可以是任何你想要的*转换操作*，而不仅仅是原始元素。这使得列表推导式变成了一个紧凑的数据转换管道。

### 工作原理

在 `[expression for item in iterable]` 中，`expression` 会针对每个元素求值一次。它生成的任何结果都会成为新列表中对应的元素。你可以调用该元素的方法、将其传入函数中、将其与其他值组合，或者将多个操作链式连接起来。

### 语法

```python
# General form
new_list = [transform(item) for item in original_list]
```

`transform(item)` 部分可以是任何能够返回值的表达式。

### 示例

```python
# Square each number and add 1
numbers = [2, 5, 8]
result = [n ** 2 + 1 for n in numbers]
# result -> [5, 26, 65]

# Generate formatted labels
labels = [f"row_{i}" for i in range(4)]
# labels -> ['row_0', 'row_1', 'row_2', 'row_3']

# Get the length of each word
fruits = ["apple", "kiwi", "strawberry"]
lengths = [len(fruit) for fruit in fruits]
# lengths -> [5, 4, 10]

# Strip whitespace and lowercase city names
cities = ["  Berlin ", " TOKYO", "Paris  "]
cleaned = [city.strip().lower() for city in cities]
# cleaned -> ['berlin', 'tokyo', 'paris']

# Convert integers to strings
ids = [101, 202, 303]
id_strings = [str(x) for x in ids]
# id_strings -> ['101', '202', '303']
```

### 常见模式

| **模式示例表达式针对元素** **`"hi"`** **的结果** |                                 |         |
| ---------------------------------- | ------------------------------- | ------- |
| 字符串方法                              | `item.title()`                  | `"Hi"`  |
| 内置函数                               | `len(item)`                     | `2`     |
| 算术运算                               | `len(item) * 3`                 | `6`     |
| F-string 格式化                       | `f"{item}!"`                    | `"hi!"` |
| 方法链式调用                             | `item.strip().replace('h','H')` | `"Hi"`  |

请注意，每种模式都将转换操作放在 `for` 关键字**之前**。推导式负责处理迭代，而你只需专注于对每个元素执行什么操作。
