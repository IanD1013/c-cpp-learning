### `defaultdict` — 具有自动默认值的字典

Python `collections` 模块中的 `defaultdict` 是 `dict` 的一个子类，它在访问缺失的键时永远不会引发 `KeyError`。相反，当你访问一个不存在的键时，它会调用你在构造时提供的**工厂函数**（factory function），将结果存储在该键下并返回它。

这消除了 Python 中最繁琐的重复模式之一 —— 在修改键的值之前检查该键是否存在。

### 工作原理

普通的 `dict` 在访问缺失的键时会引发 `KeyError`：

```python
d = {}
d["fruit"].append("apple")  # KeyError: 'fruit'
```

使用 `defaultdict` 时，你需要向构造函数传入一个可调用对象（即不带参数的函数）。每当访问缺失的键时，该可调用对象都会被调用以生成默认值：

```python
from collections import defaultdict

d = defaultdict(list)   # factory = list, which returns []
d["fruit"].append("apple")  # No error! 'fruit' is auto-created as []
```

工厂函数存储在 `.default_factory` 属性中，可以是任何零参数的可调用对象：

- `list`
- `int`
- `set`
- `float`
- `lambda: "unknown"`
- 自定义函数

### 语法

```python
from collections import defaultdict

# Create with a factory function
dd = defaultdict(factory_callable)

# Access a missing key — factory is called automatically
dd[missing_key]  # returns factory_callable()

# All normal dict operations work
dd[key] = value

for k, v in dd.items():
    ...

len(dd)

key in dd
```

### 示例

```python
from collections import defaultdict

# Example 1: Counting occurrences (factory = int, which returns 0)
votes = ["yes", "no", "yes", "yes", "no"]

tally = defaultdict(int)

for vote in votes:
    tally[vote] += 1

# tally: {'yes': 3, 'no': 2}


# Example 2: Grouping items into sets (factory = set)
scores = [
    ("Alice", 90),
    ("Bob", 85),
    ("Alice", 78),
    ("Bob", 92)
]

by_student = defaultdict(set)

for name, score in scores:
    by_student[name].add(score)

# by_student: {'Alice': {90, 78}, 'Bob': {85, 92}}


# Example 3: Collecting values into lists (factory = list)
imports = [
    ("math", "sqrt"),
    ("os", "path"),
    ("math", "ceil"),
    ("os", "getcwd")
]

by_module = defaultdict(list)

for module, func in imports:
    by_module[module].append(func)

# by_module: {'math': ['sqrt', 'ceil'], 'os': ['path', 'getcwd']}
```

### `defaultdict` 替代的代码模式

如果没有 `defaultdict`，你每次都需要编写这样冗长的样板代码：

```python
# The tedious way
result = {}

for item in data:
    key = compute_key(item)

    if key not in result:
        result[key] = []

    result[key].append(item)
```

使用 `defaultdict`：

```python
# The defaultdict way — no existence check needed
result = defaultdict(list)

for item in data:
    result[compute_key(item)].append(item)
```

### 转换回普通 `dict`

`defaultdict` 在几乎所有方面的行为都与 `dict` 相同，但如果你需要一个普通的 `dict`（例如用于 JSON 序列化，或者为了防止后续意外自动创建键），可以对其进行转换：

```python
regular = dict(dd)  # shallow copy as plain dict
```

或者：

```python
regular = {
    k: v
    for k, v in dd.items()
}
```