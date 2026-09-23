### 类继承

继承是面向对象编程的核心支柱之一。它允许你基于现有类（**父类**或**超类**）创建一个新类（称为**子类**）。子类会自动获得父类的所有属性和方法——无需你重写任何一行代码。这促进了代码复用：通用行为存在于父类中，而特定行为在子类中被添加或重写。

### 工作原理

定义子类时，将父类名称放在子类名称后面的括号中。子类随后会**继承**父类的一切内容。如果子类定义了一个与父类同名的方法，它就会**重写**该方法——使用子类的版本作为替代。子类还可以定义父类中不存在的全新方法和属性。

重要的是，如果子类没有定义自己的 `__init__`，它会自动使用父类的 `__init__`。如果子类需要扩展初始化逻辑，可以调用 `super().__init__(...)` 来调用父类的构造函数。

### 语法

```python
class Parent:
    def __init__(self, value):
        self.value = value

    def describe(self):
        return f"Value is {self.value}"


class Child(Parent):
    # Inherits __init__ and describe from Parent
    # Can override methods or add new ones
    def describe(self):
        return f"Child value is {self.value}"
```

### 示例

```python
# Example 1: Basic inheritance with no overrides
class Vehicle:
    def __init__(self, make):
        self.make = make

    def info(self):
        return f"{self.make} vehicle"

class Truck(Vehicle):
    pass  # Inherits everything from Vehicle

t = Truck("Ford")
print(t.info())  # "Ford vehicle" — inherited from Vehicle


# Example 2: Overriding a method in the child class
class Shape:
    def __init__(self, color):
        self.color = color

    def description(self):
        return f"A {self.color} shape"

class Circle(Shape):
    def description(self):
        return f"A {self.color} circle"

s = Shape("red")
print(s.description())   # "A red shape"

c = Circle("blue")
print(c.description())   # "A blue circle" — overridden method


# Example 3: Deciding which class to instantiate at runtime
class Appliance:
    def __init__(self, brand):
        self.brand = brand

    def operate(self):
        return f"{self.brand} is running"

class Blender(Appliance):
    def operate(self):
        return f"{self.brand} is blending"

def get_appliance(brand, kind):
    if kind == "blender":
        return Blender(brand)
    return Appliance(brand)

print(get_appliance("Ninja", "blender").operate())  # "Ninja is blending"
print(get_appliance("Generic", "toaster").operate()) # "Generic is running"
```

### 核心概念

* **`class Child(Parent):`** —— 声明继承的方式
* 子类**自动**获得父类的 `__init__` 和所有方法
* **重写（Overriding）**：在子类中定义同名方法以替换父类的版本
* 子类可以访问 `self.name`（或父类 `__init__` 设置的任何属性），因为它继承了该构造函数
* 你可以**在运行时**根据条件决定创建哪个类，并在任一类上调用相同的方法——这就是多态的实际应用
