### `@property` 装饰器

在 Python 中，你经常需要公开一个依赖于对象内部属性的计算值。在没有特殊工具的情况下，你会编写一个类似 `get_area()` 的常规方法并带括号调用它。但有时候，一个值*感觉*更应该是一个属性——某个你只需要直接读取，而不是去“调用”的东西。`@property` 装饰器允许你定义一个像属性一样访问的方法，无需括号。

### 工作原理

当你将 `@property` 装饰器放在方法定义的正上方时，Python 会将该方法转换为描述符（descriptor）。从外部看，你的类的使用者可以像访问普通属性一样访问它——无需 `()`。在底层，Python 会调用你的方法并返回结果。这非常适合那些派生自其他属性且应始终保持同步的值。

### 语法

```python
class MyClass:
    def __init__(self, value):
        self.value = value

    @property
    def computed(self):
        # Return something based on self.value
        return self.value * 2
```

关键点：

* `@property` 位于 `def` 行的**正上方**
* 该方法仅接受 `self` 作为参数
* 你访问它时**无需括号**：使用 `obj.computed` 而不是 `obj.computed()`

### 示例

```python
# Example 1: A Circle with a computed diameter
class Circle:
    def __init__(self, radius):
        self.radius = radius

    @property
    def diameter(self):
        return self.radius * 2

c = Circle(5)
print(c.diameter)  # 10  — accessed like an attribute, no parentheses

# Example 2: A Product with a formatted price
class Product:
    def __init__(self, name, price_cents):
        self.name = name
        self.price_cents = price_cents

    @property
    def price_display(self):
        return f"${self.price_cents / 100:.2f}"

p = Product("Widget", 1999)
print(p.price_display)  # $19.99

# Example 3: A Rectangle with computed area
class Rectangle:
    def __init__(self, width, height):
        self.width = width
        self.height = height

    @property
    def area(self):
        return self.width * self.height

r = Rectangle(3, 4)
print(r.area)  # 12
```

注意 `c.diameter`、`p.price_display` 和 `r.area` 看起来都像是在读取一个简单的属性，即使计算是在后台进行的。

### 常见模式

* **组合属性**：将多个存储的值合并为一个可读的结果
* **格式化输出**：从原始数据返回格式美观的字符串
* **派生计算**：从宽/高计算面积、从出生日期计算年龄等
* **保持数据一致**：属性始终反映底层属性的当前状态
