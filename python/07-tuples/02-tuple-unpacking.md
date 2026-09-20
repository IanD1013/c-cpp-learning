### 元组解包 (Tuple Unpacking)

元组解包允许你在单条清晰的语句中将元组的每个元素分别赋值给对应的变量。你无需通过索引逐个提取值，而是可以一次性解构整个元组。这是 Python 最优雅的特性之一，并在实际开发代码中频繁出现——从处理数据库行到处理函数返回值。

### 工作原理

当 Python 看到赋值操作左侧有多个变量名，而右侧是一个元组（或任何可迭代对象）时，它会按位置依次进行匹配。第一个变量获取第一个元素，第二个变量获取第二个元素，依此类推。左侧的变量数量**必须**与右侧的元素数量一致，否则 Python 会抛出 `ValueError`。

### 语法

```python
# Basic unpacking
variable1, variable2 = (value1, value2)

# The parentheses are optional on the right side
variable1, variable2 = value1, value2

# Works with any number of elements
a, b, c = (10, 20, 30)
```

### 示例

```python
# Unpacking coordinates
x, y = (5, 12)
print(x)  # 5
print(y)  # 12

# Unpacking in a for loop over a list of tuples
students = [("Alice", 90), ("Bob", 85), ("Carol", 92)]
for name, score in students:
    print(f"{name} scored {score}")
# Alice scored 90
# Bob scored 85
# Carol scored 92

# The classic swap trick — no temporary variable needed!
first = "left"
second = "right"
first, second = second, first
print(first)   # right
print(second)  # left
```

请注意变量交换的示例：`first, second = second, first`。Python 会先计算**整个右侧**，创建一个临时元组 `("right", "left")`，然后将其解包赋值给左侧的变量。这意味着两个值是同时交换的——完全不存在某个值在使用前被覆盖的风险。

### 常见模式

* **交换变量值：** `a, b = b, a` — 解包最经典的用法
* **从函数返回多个值：** `return (x, y)` 随后 `result_x, result_y = my_function()`
* **遍历键值对：** `for key, value in my_dict.items():`
* **使用 `_` 忽略值：** 当只需要姓名时，可以使用 `name, _ = ("Alice", 30)`
