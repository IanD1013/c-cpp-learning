### 字典：Python 的键值存储

**字典**（dictionary）是**键值对**（key-value pairs）的无序集合。可以把它想象成现实生活中的字典：你查找一个单词（键）来找到它的定义（值）。在编程中，字典是最强大且最常用的数据结构之一——它允许你将有意义的标签与数据关联起来，而不是像列表那样依赖数字索引。

### 工作原理

字典使用花括号 `{}` 编写，每个条目由一个**键**（key）、一个冒号及其**值**（value）组成。键必须是**不可变类型**——字符串、数字或元组——而值可以是任何类型：字符串、数字、列表、其他字典等等。

一个重要规则：**键必须是唯一的**。如果你定义了两次相同的键，最后的值会生效——先前的值会被静默覆盖。

**空字典**可以通过 `{}` 或 `dict()` 创建。

### 语法

```python
# Literal syntax with curly braces
my_dict = {"key1": value1, "key2": value2}

# Empty dictionary
empty = {}
also_empty = dict()

# Using the dict() constructor with keyword arguments
my_dict = dict(key1=value1, key2=value2)

# Number of key-value pairs
count = len(my_dict)
```

### 示例

```python
# Creating a dictionary to store a book's information
book = {"title": "Dune", "author": "Frank Herbert", "year": 1965}
print(book)        # {'title': 'Dune', 'author': 'Frank Herbert', 'year': 1965}
print(len(book))   # 3

# Using the dict() constructor for the same result
car = dict(make="Toyota", model="Corolla", year=2022)
print(car)         # {'make': 'Toyota', 'model': 'Corolla', 'year': 2022}

# Duplicate keys — the last value wins
scores = {"math": 85, "science": 90, "math": 95}
print(scores)      # {'math': 95, 'science': 90}

# Keys can be numbers or tuples (any immutable type)
coordinates = {(0, 0): "origin", (1, 2): "point A"}
print(len(coordinates))  # 2
```

### 常见模式

字典通常用于：

* **建模现实世界的实体**——一个人、一个产品、一项配置设置
* **将相关数据分组**——不再使用独立的变量 `name = "Alice"` 和 `age = 30`，而是将它们打包到一个结构中
* **将标识符映射到值**——用户 ID 映射到用户名，国家代码映射到国家名称

```python
# Modeling a server configuration
config = {"host": "localhost", "port": 8080, "debug": True}

# Mapping country codes
countries = {"US": "United States", "GB": "United Kingdom", "JP": "Japan"}
```
