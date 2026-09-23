### 使用 `__add__` 和 `__sub__` 进行算术运算符重载

Python 允许你通过特殊方法（dunder methods）`__add__` 和 `__sub__` 为自己的类定义 `+` 和 `-` 的含义。这就是像 `int`、`float` 甚至 `str` 等内置类型支持算术运算的方式——你也可以为自定义类型提供同样自然的语法。

### 工作原理

当 Python 遇到类似 `a + b` 的表达式时，它会在后台调用 `a.__add__(b)`。类似地，`a - b` 会调用 `a.__sub__(b)`。这些方法接收 `self`（左操作数）和 `other`（右操作数），并且它们应该**返回一个表示结果的新对象**。这一点非常重要：不要修改任何一个操作数。你应该创建并返回一个新实例，就像 `3 + 5` 得到 `8` 而不会改变 `3` 或 `5` 一样。

### 语法

```python
class MyType:
    def __init__(self, value):
        self.value = value

    def __add__(self, other):
        # Return a NEW instance with combined values
        return MyType(self.value + other.value)

    def __sub__(self, other):
        # Return a NEW instance with the difference
        return MyType(self.value - other.value)
```

### 示例

```python
# A Vector2D class that supports addition and subtraction
class Vector2D:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __add__(self, other):
        return Vector2D(self.x + other.x, self.y + other.y)

    def __sub__(self, other):
        return Vector2D(self.x - other.x, self.y - other.y)

    def __repr__(self):
        return f"Vector2D({self.x}, {self.y})"

v1 = Vector2D(3, 4)
v2 = Vector2D(1, 2)
v3 = v1 + v2          # Vector2D(4, 6) — a new object!
v4 = v1 - v2          # Vector2D(2, 2) — another new object
print(v1)             # Vector2D(3, 4) — unchanged!
```

```python
# A Temperature class — subtraction gives a difference
class Temperature:
    def __init__(self, degrees):
        self.degrees = degrees

    def __add__(self, other):
        return Temperature(self.degrees + other.degrees)

    def __sub__(self, other):
        return Temperature(self.degrees - other.degrees)

t1 = Temperature(100.0)
t2 = Temperature(37.5)
diff = t1 - t2        # Temperature with degrees = 62.5
combined = t1 + t2    # Temperature with degrees = 137.5
```

### 核心原则：返回新对象

请注意，在每个示例中，`__add__` 和 `__sub__` 都会创建并返回一个**新**实例。它们绝不会执行 `self.value += other.value`。这保持了操作数的不可变性，而这正是算术运算的预期行为——就像 `5 + 3` 不会改变数字 `5` 一样。

### 常见模式

| 模式                      | 描述                              |
| ----------------------- | ------------------------------- |
| `return ClassName(...)` | 始终从 `__add__`/`__sub__` 返回一个新实例 |
| `other.attribute`       | 访问右操作数上的相同属性                    |
| `round(value, n)`       | 将 float 四舍五入到 `n` 位小数           |
