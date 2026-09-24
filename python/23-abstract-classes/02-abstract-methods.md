### 使用 `@abstractmethod` 的抽象方法

在大型代码库中，你通常会设计一个基类来为其所有子类定义蓝图。但你如何**保证**每个子类都真正实现了你所期望的方法呢？这就是 `@abstractmethod` 的用武之地——它充当了一份**契约**，强制任何具体子类必须提供特定方法的自身实现。如果子类忘记实现某个抽象方法，当你尝试实例化它时，Python 将抛出 `TypeError`。

### 工作原理

当你在一个继承自 `ABC`（Abstract Base Class，抽象基类）的类中使用 `@abstractmethod` 标记某个方法时，该类就变成了**抽象类**——它无法被直接实例化。任何继承自它的子类都**必须**用具体实现来重写每一个抽象方法。如果没有重写，子类本身也会变成抽象类，同样无法被实例化。这种机制在动态语言中为你提供了类似编译时的安全性保障。

有趣的是，抽象方法*可以*包含方法体。该方法体可以充当文档，或者提供子类通过 `super()` 调用的默认逻辑。但无论是否存在方法体，子类**都必须重写**该方法。

### 语法

```python
from abc import ABC, abstractmethod

class MyBaseClass(ABC):
    def __init__(self, value):
        self.value = value

    @abstractmethod
    def do_something(self):
        """Subclasses must implement this."""
        pass
```

### 示例

```python
from abc import ABC, abstractmethod

# 形状的抽象基类
class Shape(ABC):
    def __init__(self, color):
        self.color = color

    @abstractmethod
    def area(self):
        pass

    @abstractmethod
    def describe(self):
        pass

# 具体子类 —— 必须实现所有抽象方法
class Circle(Shape):
    def __init__(self, color, radius):
        super().__init__(color)
        self.radius = radius

    def area(self):
        return 3.14159 * self.radius ** 2

    def describe(self):
        return f"A {self.color} circle with radius {self.radius}"

# 运行正常
c = Circle("red", 5)
print(c.describe())  # "A red circle with radius 5"

# 这会因 TypeError 失败：
# s = Shape("blue")  # 无法实例化抽象类 Shape
```

```python
# 另一个示例：带有方法体的抽象方法
class Animal(ABC):
    def __init__(self, species):
        self.species = species

    @abstractmethod
    def sound(self):
        return "Some generic sound"  # 存在方法体但仍需重写

class Dog(Animal):
    def __init__(self):
        super().__init__("Canine")

    def sound(self):
        # 可以选择调用 super().sound()，但仍必须进行重写
        return "Woof!"
```

### 常见模式

* **工厂风格函数**：函数接收一个类型标识符并返回相应的子类实例。由于所有子类都共享相同的抽象接口，你可以在其中任何一个实例上调用相同的方法——这就是由抽象契约所强化的多态性。
* **在基类中存储共享状态**：抽象类的 `__init__` 可以接收并存储通用属性。子类调用 `super().__init__(...)` 来初始化共享状态，然后添加它们自己的特有属性。
* **具有不同构造函数的子类**：并非每个子类都需要相同的参数。一个子类可能需要额外的数据，而另一个子类在调用 `super().__init__()` 时可能会硬编码某些值。
