### 字典推导式

你已经知道如何使用列表推导式（list comprehensions）简洁地构建列表。字典推导式（Dictionary comprehensions）也遵循同样的理念——它们允许你在单个表达式中构建整个字典。当你需要将键映射到计算出的值、反转映射关系或过滤条目时，这非常有用，而且无需编写冗长的循环。

### 工作原理

字典推导式会为可迭代对象中的每个元素求值键表达式和值表达式，并将结果收集到一个新字典中。由于字典的键必须是唯一的，如果两次迭代产生了相同的键，后面的值会覆盖前面的值——就像在普通字典中对同一个键重复赋值一样。

### 语法

```python
# General form
{key_expr: value_expr for item in iterable}

# With a condition (filtering)
{key_expr: value_expr for item in iterable if condition}

# Iterating over key-value pairs of an existing dictionary
{k: v for k, v in some_dict.items() if condition}
```

### 示例

```python
# Squaring numbers: map each number to its square
squares = {n: n ** 2 for n in range(1, 6)}
# Result: {1: 1, 2: 4, 3: 9, 4: 16, 5: 25}

# Inverting a dictionary: swap keys and values
original = {'a': 1, 'b': 2, 'c': 3}
inverted = {v: k for k, v in original.items()}
# Result: {1: 'a', 2: 'b', 3: 'c'}

# Filtering: keep only positive values from a dictionary
scores = {'alice': 85, 'bob': -3, 'carol': 92, 'dave': 0}
passing = {name: score for name, score in scores.items() if score > 0}
# Result: {'alice': 85, 'carol': 92}

# Converting a list of tuples into a dictionary
pairs = [('x', 10), ('y', 20), ('z', 30)]
mapping = {k: v * 2 for k, v in pairs}
# Result: {'x': 20, 'y': 40, 'z': 60}
```

### 常见模式

* **转换值：** `{k: transform(v) for k, v in d.items()}` —— 对每个值应用一个函数。
* **过滤条目：** `{k: v for k, v in d.items() if predicate(k, v)}` —— 仅保留符合条件的键值对。
* **从列表构建：** `{item: some_function(item) for item in my_list}` —— 从每个元素推导出键值关系。
* **键转换为大写：** `{k.upper(): v for k, v in d.items()}` —— 在保留值的同时规范化键。

### 实用参考

| **函数 / 方法描述示例** |              |                                               |
| --------------- | ------------ | --------------------------------------------- |
| `len(s)`        | 返回字符串、列表等的长度 | `len("cat")` → `3`                            |
| `dict.items()`  | 以元组形式返回键值对   | `{'a': 1}.items()` → `dict_items([('a', 1)])` |
| `str.upper()`   | 返回字符串的大写副本   | `"hi".upper()` → `"HI"`                       |
