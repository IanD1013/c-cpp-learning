### 使用 `as` 设置导入别名

Python 允许你使用 `as` 关键字重命名任何导入项。当模块或函数名称过长、两个模块重名，或者简短的名称能让代码更具可读性时，这非常有用。别名只是一种书写上的便利 —— 底层模块或函数的工作方式完全相同。

### 工作原理

导入别名有两种形式：

1. **模块别名：** `import module as alias` —— 整个模块在别名下可用。
2. **名称别名：** `from module import name as alias` —— 特定的函数、类或变量以新名称导入。

一旦设置了别名，你就在代码中通篇使用该别名。原始名称**不可用** —— 只有别名绑定到了你的命名空间中。

### 语法

```python
# Alias an entire module
import some_long_module_name as short

# Now use short.something instead of some_long_module_name.something
result = short.some_function()

# Alias a specific import
from some_module import some_verbose_function as svf

# Now call svf() instead of some_verbose_function()
result = svf(42)
```

### 示例

```python
# Example 1: Aliasing the collections module
import collections as col

counter = col.Counter(["a", "b", "a", "c", "a"])
print(counter)  # Counter({'a': 3, 'b': 1, 'c': 1})

# Example 2: Aliasing a specific function from os.path
from os.path import join as path_join

full_path = path_join("/home", "user", "documents")
print(full_path)  # /home/user/documents

# Example 3: Avoiding a name conflict
from json import dumps as json_dumps
from pickle import dumps as pickle_dumps

# Both are called 'dumps' in their modules, but aliases keep them distinct
print(json_dumps({"key": "value"}))  # '{"key": "value"}'
```

### 常见模式

有些别名使用非常广泛，已经成为了约定俗成的惯例：

| **导入常用别名用法**                 |        |                       |
| ---------------------------- | ------ | --------------------- |
| `import datetime`            | `dt`   | `dt.date.today()`     |
| `import numpy`               | `np`   | `np.array([1, 2, 3])` |
| `import pandas`              | `pd`   | `pd.DataFrame(data)`  |
| `from math import factorial` | `fact` | `fact(10)`            |

使用众所周知的别名可以让你的代码更容易被其他 Python 开发者理解。

### 实用参考

| **函数 / 属性说明示例**         |                            |                                        |
| ----------------------- | -------------------------- | -------------------------------------- |
| `datetime.date.today()` | 以 `date` 对象形式返回今天的日期       | `datetime.date.today()` → `2026-02-17` |
| `date.isoformat()`      | 将日期作为 `YYYY-MM-DD` 字符串返回   | `d.isoformat()` → `"2026-02-17"`       |
| `math.factorial(n)`     | 返回 `n!`（n 的阶乘）             | `math.factorial(5)` → `120`            |
| `str(date_object)`      | 同样会为日期对象生成 `YYYY-MM-DD` 格式 | `str(d)` → `"2026-02-17"`              |
