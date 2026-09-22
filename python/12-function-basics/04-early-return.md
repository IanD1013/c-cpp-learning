### 函数中的提前返回 (Early Return)

一个 Python 函数可以包含多个 `return` 语句，但每次函数调用只会执行其中一个。一旦 Python 执行到任何 `return`，函数就会立即退出并返回该值。这种行为引出了一种强大的模式，称为**提前返回**（**early return**）——在函数的最顶部处理特殊情况或边界情况，这样其余代码就不需要包裹在深层嵌套的 `if`/`else` 代码块中。

### 工作原理

如果不使用提前返回，你可能最终会写出类似这样的代码：

```python
def process(data):
    if data is not None:
        if len(data) > 0:
            # ... actual logic deeply nested ...
            result = do_something(data)
            return result
        else:
            return "empty"
    else:
        return "no data"
```

使用提前返回时，你会先处理不寻常的情况并立即退出，从而将主要逻辑保留在最顶层的缩进中：

```python
def process(data):
    if data is None:
        return "no data"
    if len(data) == 0:
        return "empty"
    result = do_something(data)
    return result
```

两个版本产生的结果相同，但提前返回版本结构更扁平，更易于阅读和维护。

### 语法

```python
def some_function(params):
    if edge_case_condition:
        return edge_case_value      # exits immediately

    if another_edge_case:
        return another_value         # exits immediately

    # Main logic runs only if no edge case was triggered
    return main_result
```

你也可以在循环内部使用 `return`。当在 `for` 或 `while` 循环内部遇到 `return` 时，循环会终止，整个函数也会立即退出。

```python
def some_function(items):
    for item in items:
        if some_condition(item):
            return item              # exits the loop AND the function
    return default_value             # only reached if no item matched
```

### 示例

```python
# Example 1: Early return for a guard clause
def get_discount(price, member_level):
    if price <= 0:
        return 0.0                   # invalid price, return immediately
    if member_level == "gold":
        return price * 0.20
    if member_level == "silver":
        return price * 0.10
    return 0.0                       # non-members get no discount

get_discount(-5, "gold")    # -> 0.0  (early return triggered)
get_discount(100, "silver") # -> 10.0
get_discount(100, "bronze") # -> 0.0  (falls through to the end)
```

```python
# Example 2: Early return inside a loop to find the first match
def find_first_even(numbers):
    for n in numbers:
        if n % 2 == 0:
            return n                 # found it — exit immediately
    return None                      # no even number found

find_first_even([1, 3, 4, 6])  # -> 4  (stops at the first even)
find_first_even([1, 3, 5])     # -> None
find_first_even([])            # -> None
```

```python
# Example 3: Multiple guard clauses cleaning up nested logic
def safe_divide(a, b):
    if b == 0:
        return None
    if not isinstance(a, (int, float)):
        return None
    return a / b

safe_divide(10, 0)       # -> None (early return)
safe_divide(10, 3)       # -> 3.333...
```

### 常见模式

* **卫语句（Guard clauses）**：在顶部检查无效输入（空集合、`None`、零值）并提前返回。
* **搜索循环**：遍历集合并返回第一个符合条件的项。如果没有匹配项，则在循环后返回默认值。
* **验证链**：逐个检查多个条件，对每个不满足的条件返回错误或默认值。
