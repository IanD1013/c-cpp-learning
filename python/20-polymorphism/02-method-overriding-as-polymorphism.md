### 通过方法重写实现多态

多态（Polymorphism）意为“多种形态”。在面向对象编程中，它允许相同的方法名根据调用它的对象不同而表现出不同的行为。当多个子类各自重写相同的父类方法时，调用代码可以统一处理所有这些对象——只需调用该方法，每个对象都会根据自己的实现做出响应。这是面向对象编程中最强大的思想之一，因为它消除了对冗长类型检查条件链的需求。

### 工作原理

当你在父类中定义一个方法并在子类中重新定义它时，对于该子类的实例，子类版本会**替代**父类版本。Python 在运行时根据对象的实际类型（而非变量或参数类型）来解析调用哪个版本。这意味着你可以编写一个接受包含特定方法的*任何*对象的函数，调用该方法，并获得特定于该对象类的行为。

### 语法

```python
class Base:
    def action(self):
        return "base behavior"

class VariantA(Base):
    def action(self):
        return "variant A behavior"

class VariantB(Base):
    def action(self):
        return "variant B behavior"
```

关键在于：`VariantA` 和 `VariantB` 都拥有 `action()`，但各自返回不同的内容。任何在这些对象之一上调用 `.action()` 的代码都会自动获得正确的行为。

### 示例

```python
# 示例：不同形状计算面积
class Shape:
    def __init__(self, label):
        self.label = label

    def area(self): 
        return 0

class Circle(Shape):
    def __init__(self, label, radius):
        super().__init__(label)
        self.radius = radius

    def area(self):
        return 3.14159 * self.radius ** 2

class Square(Shape):
    def __init__(self, label, side):
        super().__init__(label)
        self.side = side

    def area(self):
        return self.side ** 2

# 实践中的多态 —— 相同的方法，不同的结果
shapes = [Circle("Wheel", 5), Square("Tile", 4)]
for s in shapes:
    print(f"{s.label}: {s.area()}")
# 输出：
# Wheel: 78.53975
# Tile: 16
```

```python
# 示例：将字符串键映射到正确的类
class_lookup = {"circle": Circle, "square": Square}
cls = class_lookup["circle"]
obj = cls("MyCircle", 10)  # 创建一个 Circle
print(obj.area())           # 调用 Circle.area()
```

请注意，调用代码从不使用 `isinstance()` 或 `if/elif` 链。它只需调用 `.area()`，并信任每个对象都会做出正确的处理。

### 常见模式

**基于数据的工厂式创建：** 当你有原始数据描述要创建*哪种*类型时，将字符串键映射到类的字典是一种简洁的方法：

```python
registry = {
    "circle": Circle,
    "square": Square
}

def build_shape(label, kind, size):
    cls = registry.get(kind, Shape)
    return cls(label, size)
```

这避免了 `if kind == "circle": ... elif kind == "square": ...`，并在添加新类型时易于扩展。

**统一遍历：** 一旦你拥有一个多态对象列表，就可以遍历并调用每个对象上的共享方法，而无需关心具体类型。
