### 从函数返回多个值

Python 函数不仅限于返回单个值。当你需要从函数返回两个或多个相关的数据时，Python 允许你通过用逗号分隔返回值来优雅地实现这一目的。当你有一组固定且已知的返回值需要返回给调用者时，这样可以避免构建列表或字典的开销。

### 工作原理

当你在函数内部编写 `return a, b` 时，Python 会隐式地将这些值打包成一个 **tuple**（元组）——即你已经熟悉的不可变序列类型。然后调用者可以使用你之前练习过的元组解包语法，将该元组**解包**（unpack）到独立的变量中。这创造了一种自然且可读的流程：函数产生多个结果，调用者为每个结果命名。

你也可以显式添加括号写成 `return (a, b)` —— 其行为完全相同，但有些开发者认为这样更清晰，尤其是在返回表达式比较复杂的时候。

### 语法

```python
def my_function(params):
    # compute values
    return value1, value2        # implicit tuple packing
    # or equivalently:
    # return (value1, value2)    # explicit tuple packing

# Caller unpacks into separate variables
x, y = my_function(args)

# Or keeps it as a tuple
result = my_function(args)
# result is (value1, value2)
print(result[0])                 # access by index
```

### 示例

```python
# Example 1: Return min and max of three numbers
def min_max(a, b, c):
    smallest = min(a, b, c)
    largest = max(a, b, c)
    return smallest, largest

lo, hi = min_max(7, 2, 9)
print(lo)   # 2
print(hi)   # 9

# Example 2: Split a full name into first and last
def split_name(full_name):
    parts = full_name.split()
    return parts[0], parts[-1]

first, last = split_name("Ada Lovelace")
print(first)  # Ada
print(last)   # Lovelace

# Example 3: Return as a tuple and access by index
def rectangle_properties(width, height):
    area = width * height
    perimeter = 2 * (width + height)
    return (area, perimeter)     # explicit parentheses for clarity

props = rectangle_properties(5, 3)
print(props)      # (15, 16)
print(props[0])   # 15 — the area
```

### 常见模式

* **两个相关的计算**：当一个函数自然地从相同的输入中产生两个结果时（例如，同时返回和与积，或者同时返回转换后的值和状态标志）。
* **在赋值中解包**：`x, y = func()` 是接收多个返回值最常见的方式。
* **保留元组**：当你希望传递整个元组或稍后通过索引访问它时，可以使用 `result = func()`。

### 实用参考

| **运算符描述示例** |            |                 |
| ----------- | ---------- | --------------- |
| `//`        | 整数（向下取整）除法 | `17 // 5` → `3` |
| `%`         | 取模（求余）     | `17 % 5` → `2`  |
