### 鸭子类型 (Duck Typing)

在许多语言中，两个对象只有在共享一个共同父类或实现相同接口时，才能互换使用。Python 采取了一种截然不同的方法：它不关心对象的类型或继承关系——它只关心**对象能做什么**。如果一个对象拥有你正在调用的方法，Python 就能正常执行。这种哲学被称为**鸭子类型（duck typing）**，源于这句谚语：*“如果它走起路来像鸭子，叫起来也像鸭子，那么它就是鸭子。”*

### 工作原理

当你编写 `obj.some_method()` 时，Python 不会检查 `obj` 是否是某个特定类的实例。它只是在该对象上查找 `some_method`。如果该方法存在，就会被调用；如果不存在，你将得到一个 `AttributeError`。这意味着两个完全无关的类（没有共同的父类）可以被同一个函数使用，只要它们都提供了该函数所期望的方法。

这种机制非常强大，因为它能让你编写灵活、可复用的代码，而无需构建复杂的继承层次结构。

### 语法

```python
# 具有相同方法名的两个不相关的类
class Dog:
    def speak(self):
        return "Woof!"

class Cat:
    def speak(self):
        return "Meow!"

# 可以与任何具有 .speak() 方法的对象一起工作的函数
def make_it_speak(animal):
    return animal.speak()
```

### 示例

```python
# 示例 1：不相关的类，相同的方法
class Circle:
    def __init__(self, radius):
        self.radius = radius

    def area(self):
        return 3.14159 * self.radius ** 2

class Rectangle:
    def __init__(self, width, height):
        self.width = width
        self.height = height

    def area(self):
        return self.width * self.height

# 这个函数不关心类型——只关心是否存在 .area()
def print_area(shape):
    return shape.area()

print_area(Circle(5))       # 78.53975
print_area(Rectangle(3, 4)) # 12

# 示例 2：使用完全不同的类体现鸭子类型
class Warehouse:
    def __init__(self, sq_meters):
        self.sq_meters = sq_meters

    def area(self):
        return self.sq_meters

# Warehouse 并不是一个“形状（shape）”，但 print_area 依然有效！
print_area(Warehouse(500))  # 500

# 示例 3：根据条件创建不同的对象
def get_speaker(kind):
    if kind == "dog":
        return Dog()
    elif kind == "cat":
        return Cat()

animal = get_speaker("dog")
animal.speak()  # "Woof!" — Python 不检查继承关系，只检查行为
```

### 常见模式

鸭子类型经常出现在以下场景中：

* 编写一个可以操作任何具有特定方法的对象的函数
* 根据运行时条件创建不同的对象，然后对创建的对象调用相同的方法
* 在没有形式化接口的情况下构建插件系统或策略模式

关键要点在于**调用方代码不需要进行 `isinstance()` 检查**——它只需直接调用该方法，并相信该对象支持此方法。
