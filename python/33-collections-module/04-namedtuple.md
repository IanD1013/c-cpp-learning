# 具名元组（Named Tuples）—— 具有含义的元组

普通元组非常适合存储轻量级数据，但它们存在可读性差的问题：`employee[0]` 根本无法直观体现该值所代表的含义。`collections` 模块中的 `namedtuple` 解决了这个问题，它创建了元组子类，使每个位置都拥有可读的字段名称。它们与普通元组一样是不可变的（immutable），但你可以通过字段名称而不是晦涩的索引来访问字段。

当你需要一个简单、不可变的数据容器，又不想编写完整类（class）的额外开销时，`namedtuple` 是理想之选。

## 工作原理

你可以通过调用 `namedtuple()` 并传入类型名称及一组字段名称序列来定义 `namedtuple`。这将返回一个新的类，用于创建实例。每个实例的行为与普通元组完全相同（支持索引、解包、迭代），同时也支持通过字段名进行属性风格的访问。

由于它们是不可变的，你无法直接就地修改字段。相反，`namedtuple` 提供了特殊方法（带有 `_` 前缀，以避免与你的字段名发生冲突）用于常见的转换操作。

## 语法

```python
from collections import namedtuple

# Define a namedtuple type
Color = namedtuple('Color', ['red', 'green', 'blue'])

# Create instances
c = Color(255, 128, 0)
c = Color(red=255, green=128, blue=0)  # keyword arguments work too
```

## 示例

```python
from collections import namedtuple

# Define and create
Employee = namedtuple('Employee', ['name', 'department', 'salary'])
e = Employee('Dana', 'Engineering', 95000)

# Access by name or index — both work
print(e.name)        # 'Dana'
print(e[2])          # 95000

# _fields gives you the field names as a tuple
print(e._fields)     # ('name', 'department', 'salary')

# _asdict() returns an OrderedDict (dict-like) representation
print(e._asdict())   # {'name': 'Dana', 'department': 'Engineering', 'salary': 95000}

# _replace() returns a NEW instance with modified fields (immutability preserved)
updated = e._replace(salary=105000)
print(updated)       # Employee(name='Dana', department='Engineering', salary=105000)
print(e.salary)      # 95000 — original is unchanged

# Convert from regular tuples
raw_data = [('Alice', 'Sales', 70000), ('Bob', 'Marketing', 65000)]
employees = [Employee(*row) for row in raw_data]
```

`Employee(*row)` 中的 `*` 会将元组解包为单独的位置参数，因此：

```python
Employee(*('Alice', 'Sales', 70000))
```

与调用：

```python
Employee('Alice', 'Sales', 70000)
```

完全等价。

这与你在 `*args` 中看到的星号相同，只是用在调用方。

## 常见用法模式

```python
# Namedtuples work seamlessly with math operations via field access
Vector = namedtuple('Vector', ['dx', 'dy'])
v = Vector(3, 4)
magnitude = (v.dx ** 2 + v.dy ** 2) ** 0.5  # 5.0

# Aggregating over collections of namedtuples
temps = [Weather('Mon', 72), Weather('Tue', 68), Weather('Wed', 75)]
avg_temp = sum(w.temp for w in temps) / len(temps)
hottest = max(temps, key=lambda w: w.temp)

# Converting to plain dicts for serialization
dict_list = [dict(w._asdict()) for w in temps]
```