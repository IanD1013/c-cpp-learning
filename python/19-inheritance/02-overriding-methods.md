### Python 中的方法重写（Method Overriding）

当子类继承父类时，它会获得父类的所有方法。但如果父类的方法无法满足子类的需求怎么办？**方法重写**（Method Overriding）允许子类重新定义它所继承的方法，从而在子类实例中完全替换掉父类的版本。这是面向对象编程中最强大的特性之一——它允许你在父类中建立通用接口，同时让每个子类自定义具体的行为。

### 工作原理

当你在对象上调用一个方法时，Python 首先会在该对象自身的类中查找该方法。如果找到了，就会使用该版本。如果没找到，它才会去父类中查找。这意味着如果子类定义了一个与父类中方法**同名**的方法，子类的版本就会胜出——它“重写”了父类的方法。

父类仍然保留其原本的方法，继承自该父类的其他类也仍然使用父类的版本。只有定义了重写的子类才会获得新的行为。

### 语法

```python
class Parent:
    def greet(self):
        return "Hello from Parent"

class Child(Parent):
    def greet(self):
        return "Hello from Child"
```

当你创建一个 `Child` 对象并调用 `greet()` 时，Python 会使用子类的版本。父类的 `greet()` 依然存在——只是在 `Child` 实例上不会被调用。

### 使用 `super()` 调用父类的 `__init__`

在重写 `__init__` 时，子类通常也需要初始化父类的属性。使用 `super()` 来调用父类的构造函数：

```python
class Vehicle:
    def __init__(self, vehicle_type):
        self.vehicle_type = vehicle_type

    def fuel_efficiency(self):
        return 0

class Car(Vehicle):
    def __init__(self, mpg):
        super().__init__("Car")
        self.mpg = mpg

    def fuel_efficiency(self):
        return self.mpg
```

在这里，`Car.__init__` 调用了 `super().__init__("Car")` 以确保父类的 `vehicle_type` 属性被正确设置，然后再添加它自己的 `mpg` 属性。

### 示例

```python
class Animal:
    def __init__(self, species):
        self.species = species

    def sound(self):
        return "..."

class Dog(Animal):
    def __init__(self):
        super().__init__("Dog")

    def sound(self):
        return "Woof!"

class Cat(Animal):
    def __init__(self):
        super().__init__("Cat")

    def sound(self):
        return "Meow!"

d = Dog()
print(d.species)   # "Dog"     — set by parent's __init__
print(d.sound())    # "Woof!"   — child's override

c = Cat()
print(c.sound())    # "Meow!"   — each child has its own version

a = Animal("Unknown")
print(a.sound())    # "..."     — parent's default still works
```

注意观察每个子类是如何提供自己专属的 `sound()` 方法，同时又共享父类的 `__init__` 逻辑来设置 `species` 的。

### 常见模式

* **默认实现**：父类提供一个安全的默认值（例如返回 `0` 或空字符串），子类则用有意义的逻辑进行重写。
* **模板模式**：父类定义一个确立结构的方法，子类重写其中的特定部分。
* **在 `__init__` 中使用 `super()`**：几乎在所有情况下，当子类重写 `__init__` 时，都应该调用 `super().__init__(...)` 以确保父类被正确初始化。
