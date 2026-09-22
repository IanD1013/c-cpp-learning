### `filter()` 内置函数

Python 的 `filter()` 函数用于根据条件从可迭代对象中筛选元素。虽然列表推导式（list comprehension）可以达到相同的目的，但当你已经在代码的其他地方定义了谓词函数（predicate function）时，`filter()` 会更有优势——直接将其传递给 `filter()` 比将其封装在推导式中更加清晰。

### 工作原理

`filter(function, iterable)` 将函数应用于每个元素。如果函数对某个元素返回 `True`，则保留该元素；如果返回 `False`，则丢弃该元素。其结果是一个 **filter 对象**（一个迭代器），因此通常需要用 `list()` 包装该调用以获取列表。

还有一个特殊行为：如果你将 `None` 作为函数传入，`filter()` 会移除所有**假值（falsy）**（`0`、`""`、`None`、`False`、`[]` 等）。

### 语法

```python
# General form
filter(function, iterable)

# Convert to list
list(filter(function, iterable))

# Using None to remove falsy values
list(filter(None, iterable))
```

### 示例

```python
# Filter even numbers from a list
def is_even(n):
    return n % 2 == 0

numbers = [1, 2, 3, 4, 5, 6]
result = list(filter(is_even, numbers))
# result -> [2, 4, 6]

# Using a lambda to keep strings longer than 3 characters
words = ["hi", "hello", "hey", "greetings", "no"]
long_words = list(filter(lambda w: len(w) > 3, words))
# long_words -> ["hello", "greetings"]

# Passing None removes falsy values
mixed = [0, 1, "", "hello", None, 3, False, [], "world"]
cleaned = list(filter(None, mixed))
# cleaned -> [1, "hello", 3, "world"]
```

### `filter()` 与列表推导式的比较

这两种方法通常会产生相同的结果：

```python
# These are equivalent:
list(filter(is_even, numbers))
[x for x in numbers if is_even(x)]

# With a lambda, the comprehension is often more readable:
list(filter(lambda w: len(w) > 3, words))
[w for w in words if len(w) > 3]
```

当你已经有一个具名的谓词函数时，使用 `filter()` 读起来更自然。当条件以内联方式书写时，通常更倾向于使用推导式。
