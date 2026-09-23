### `super()` 函数

当子类继承自父类时，通常需要**扩展**（extend）父类的行为，而不是完全替换它。`super()` 函数会返回一个代理对象，将方法调用委托给父类。这在 `__init__` 内部尤其关键——如果不调用父类的初始化方法，子类对象将缺少父类本应设置的属性。

### 工作原理

当你在子类方法中调用 `super()` 时，Python 会查找方法解析顺序（Method Resolution Order, MRO），并返回对链中下一个类（通常是父类）的引用。然后你可以在该代理上调用任何方法，它将执行父类的版本。可以把它理解为：“在执行我自己的设置之前，先让父类完成它的设置。”

如果不使用 `super().__init__(...)`，父类的 `__init__` 就永远不会运行。这意味着父类初始化的任何属性（例如 `self.name`）根本不会存在于子类实例上，从而在你尝试访问它们时导致 `AttributeError`。

### 语法

```python
class Child(Parent):
    def __init__(self, parent_arg, child_arg):
        super().__init__(parent_arg)   # Call parent's __init__
        self.child_attr = child_arg    # Add child-specific attributes

    def some_method(self):
        result = super().some_method()  # Call parent's version
        return result + " extended"     # Extend with child behavior
```

### 示例

```python
# Example 1: Basic super() in __init__
class Vehicle:
    def __init__(self, make, year):
        self.make = make
        self.year = year

    def info(self):
        return f"{self.year} {self.make}"

class Truck(Vehicle):
    def __init__(self, make, year, payload):
        super().__init__(make, year)   # Sets self.make and self.year
        self.payload = payload         # Adds truck-specific attribute

    def info(self):
        return f"{self.year} {self.make}, payload: {self.payload}t"

t = Truck("Ford", 2023, 2)
print(t.info())  # "2023 Ford, payload: 2t"
```

```python
# Example 2: Extending a parent method with super()
class Animal:
    def __init__(self, species):
        self.species = species

    def sound(self):
        return f"The {self.species} makes a sound"

class Dog(Animal):
    def __init__(self, species, name):
        super().__init__(species)
        self.name = name

    def sound(self):
        base = super().sound()         # Get parent's version
        return f"{base}: Woof! ({self.name})"

d = Dog("Canine", "Rex")
print(d.sound())  # "The Canine makes a sound: Woof! (Rex)"
```

```python
# Example 3: What happens WITHOUT super()
class Base:
    def __init__(self, x):
        self.x = x

class Broken(Base):
    def __init__(self, x, y):
        # Forgot to call super().__init__(x)!
        self.y = y

b = Broken(10, 20)
print(b.y)   # 20
print(b.x)   # AttributeError! Parent never set self.x
```

### 常见模式

| **模式描述**                 |                                |
| ------------------------ | ------------------------------ |
| `super().__init__(args)` | 在添加子类特有属性之前初始化父类属性             |
| `super().method()`       | 调用被重写方法的父类版本                   |
| 扩展 vs 替换                 | 使用 `super()` 进行扩展；省略它则完全替换原有行为 |
