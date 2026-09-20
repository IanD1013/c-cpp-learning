### Python 中的元组 (Tuples)

你已经了解了如何使用列表 (lists) —— 即可以创建、修改和遍历的有序集合。Python 还有另一种内置的序列类型，称为**元组 (tuple)**。元组也是元素的有序集合，但它是使用**圆括号** `()` 而不是方括号 `[]` 创建的。当你想要将相关的值组合在一起时，通常会使用元组，例如坐标、RGB 颜色或数据库记录。

### 元组的工作原理

元组与列表一样，按固定顺序存储元素。你可以通过在圆括号中放置以逗号分隔的值来创建一个元组：

```python
colors = ("red", "green", "blue")
coordinates = (10, 20, 30)
mixed = ("Alice", 30, True)
```

元组可以包含**混合类型** —— 字符串、数字、布尔值，甚至是其他元组或列表。

### 语法

```python
# Creating tuples
my_tuple = (value1, value2, value3)

# Empty tuple
empty = ()
empty_alt = tuple()

# Single-element tuple (trailing comma is REQUIRED!)
single = (42,)

# Without the comma, it's just a number in parentheses
not_a_tuple = (42)   # This is the integer 42, NOT a tuple!
```

⚠️ 尾随逗号规则是一个常见的易错点。`(42,)` 是一个包含单个元素的元组。而 `(42)` 只是包裹在圆括号中的数字 `42`（就像在数学表达式中一样）。

### 示例

```python
# Creating and accessing tuples
rgb = (255, 128, 0)
print(rgb[0])         # 255
print(rgb[-1])        # 0
print(len(rgb))       # 3

# Slicing works just like lists
print(rgb[0:2])       # (255, 128)

# Membership testing
print(128 in rgb)     # True
print(999 in rgb)     # False

# Tuple methods
scores = (85, 92, 85, 78, 92, 85)
print(scores.count(85))   # 3
print(scores.index(92))   # 1 (first occurrence)

# Converting between lists and tuples
fruits_list = ["apple", "banana", "cherry"]
fruits_tuple = tuple(fruits_list)    # ("apple", "banana", "cherry")
back_to_list = list(fruits_tuple)    # ["apple", "banana", "cherry"]
```

### 常见用法

元组经常用于从函数返回多个值：

```python
def get_name_and_age():
    return ("Bob", 25)

result = get_name_and_age()
print(result)        # ("Bob", 25)
print(result[0])     # "Bob"
```

你还可以将元组**解包 (unpack)** 到不同的变量中：

```python
name, age = get_name_and_age()
print(name)   # "Bob"
print(age)    # 25
```

### 常用参考

| **特性** | **语法**           | **描述**    |
| ------ | ---------------- | --------- |
| 创建元组   | `(a, b, c)`      | 将多个值组合在一起 |
| 单元素元组  | `(a,)`           | 必须包含尾随逗号  |
| 空元组    | `()` 或 `tuple()` | 无元素       |
| 索引     | `t[0]`, `t[-1]`  | 按位置访问     |
| 切片     | `t[1:3]`         | 提取子元组     |
| 长度     | `len(t)`         | 元素数量      |
| 成员判断   | `x in t`         | 检查值是否存在   |
| 计数     | `t.count(x)`     | x 出现的次数   |
| 查找索引   | `t.index(x)`     | 第一个 x 的位置 |
| 从列表转换  | `tuple(my_list)` | 将列表转换为元组  |
| 转换为列表  | `list(my_tuple)` | 将元组转换为列表  |
