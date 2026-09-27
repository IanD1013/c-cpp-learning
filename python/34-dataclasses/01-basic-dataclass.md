# `@dataclass` 装饰器

主要用于存储数据的 Python 类通常需要重复编写样板代码：用于赋值属性的 `__init__`、用于生成可读输出的 `__repr__` 以及用于基于值进行比较的 `__eq__`。来自 `dataclasses` 模块的 `@dataclass` 装饰器通过带有类型注解的类字段自动生成这些方法，从而消除了这种冗余。如果你曾经写过 90% 都是样板代码、只有 10% 是实际逻辑的类，那么 dataclass 就是理想的解决方案。

## 工作原理

当你使用 `@dataclass` 装饰一个类时，Python 会检查类体中的带注解字段（带有类型提示的变量）。然后它会生成：

- **`__init__`**：接受每个字段作为参数，并将它们赋值给 `self`
- **`__repr__`**：返回类似于 `ClassName(field1=value1, field2=value2)` 的字符串
- **`__eq__`**：通过将两个实例的所有字段值作为元组进行比较来判断是否相等

如果不使用 `@dataclass`，所有这些都需要你自己编写。想象一个包含 `name`、`price` 和 `category` 的 `Product` 类——这很容易就需要写 15 行以上的双下划线（dunder）方法，而 `@dataclass` 只需一个装饰器和三行带注解的代码即可替代它们。

## 语法

```python
from dataclasses import dataclass, fields

@dataclass
class ClassName:
    field_name: type
    another_field: type
```

`dataclasses` 模块中的 `fields()` 函数会返回 dataclass 的 `Field` 对象元组。每个 `Field` 对象都有一个 `.name` 属性，其中包含以字符串形式表示的字段名称。

## 示例

```python
from dataclasses import dataclass, fields

# 一个简单的 dataclass — 不需要手动编写 __init__、__repr__ 或 __eq__
@dataclass
class Book:
    title: str
    author: str
    pages: int

b1 = Book("Dune", "Frank Herbert", 412)
b2 = Book("Dune", "Frank Herbert", 412)

print(b1)          # Book(title='Dune', author='Frank Herbert', pages=412)
print(b1 == b2)    # True — 基于值的相等性，而非基于同一性（identity）

# 与手动编写进行对比：
class BookManual:
    def __init__(self, title, author, pages):
        self.title = title
        self.author = author
        self.pages = pages

    def __repr__(self):
        return f"BookManual(title={self.title!r}, author={self.author!r}, pages={self.pages!r})"

    def __eq__(self, other):
        if not isinstance(other, BookManual):
            return NotImplemented
        return (self.title, self.author, self.pages) == (other.title, other.author, other.pages)

# 手动编写需要 12 行，而 dataclass 仅需 5 行 — 并且 dataclass 版本更不容易出错。
```

## 检查字段

```python
from dataclasses import dataclass, fields

@dataclass
class Coordinate:
    x: float
    y: float
    z: float

# fields() 返回包含每个字段元数据的 Field 对象
for f in fields(Coordinate):
    print(f.name, f.type)  # x <class 'float'>, y <class 'float'>, z <class 'float'>

# 仅获取名称：
field_names = [f.name for f in fields(Coordinate)]  # ['x', 'y', 'z']
```

## 与常规类的主要区别

| **特性** | **常规类** | **`@dataclass`** |
| --- | --- | --- |
| `__init__` | 手动编写 | 自动生成 |
| `__repr__` | 手动编写 | 自动生成 |
| `__eq__` | 默认基于同一性（`is`） | 默认基于值 |
| 字段自省（Field introspection） | 使用 `__dict__` 或 `vars()` | 使用 `fields()` 获取结构化元数据 |