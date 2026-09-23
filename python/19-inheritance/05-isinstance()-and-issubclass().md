### 使用 `isinstance()` 和 `issubclass()` 进行类型检查

在处理继承层次结构时，你经常需要在运行时确定对象的类型。Python 为此提供了两个内置函数：`isinstance()` 和 `issubclass()`。这是编写根据所接收对象类型而表现出不同行为的代码的基本工具——这也是多态设计中的常见模式。

### 工作原理

你可能会倾向于使用 `type(obj) == SomeClass` 来检查对象的类型，但这有一个主要缺陷：它不考虑继承关系。如果 `Dog` 继承自 `Animal`，那么即使 Dog *是* 一种 Animal，`type(my_dog) == Animal` 也会返回 `False`。`isinstance()` 函数通过沿继承链向上查找解决了这个问题。

* **`isinstance(obj, ClassName)`** — 如果 `obj` 是 `ClassName` *或其任何继承子类* 的实例，则返回 `True`。
* **`issubclass(ChildClass, ParentClass)`** — 如果 `ChildClass`（直接或间接）继承自 `ParentClass`，则返回 `True`。这适用于类本身，而不是实例。

### 语法

```python
# Check if an object is an instance of a class (or its subclasses)
isinstance(object, classinfo)

# Check if a class is a subclass of another
issubclass(class, classinfo)

# classinfo can also be a tuple of classes
isinstance(obj, (ClassA, ClassB))  # True if obj is instance of either
```

### 示例

```python
class Vehicle:
    def __init__(self, make):
        self.make = make

class Car(Vehicle):
    def __init__(self, make):
        super().__init__(make)

class Truck(Vehicle):
    def __init__(self, make):
        super().__init__(make)

my_car = Car("Toyota")
my_truck = Truck("Ford")
generic = Vehicle("Unknown")

# isinstance respects inheritance
isinstance(my_car, Vehicle)   # True — a Car IS a Vehicle
isinstance(my_car, Car)       # True — a Car IS a Car
isinstance(my_car, Truck)     # False — a Car is NOT a Truck

# Contrast with type() which does NOT respect inheritance
type(my_car) == Vehicle       # False — type() gives the exact class only

# issubclass works on classes, not instances
issubclass(Car, Vehicle)      # True — Car inherits from Vehicle
issubclass(Vehicle, Car)      # False — Vehicle does NOT inherit from Car
issubclass(Car, Car)          # True — a class is considered a subclass of itself

# A base class instance is NOT an instance of child classes
isinstance(generic, Car)      # False
isinstance(generic, Truck)    # False
isinstance(generic, Vehicle)  # True
```

### 为什么不用 `type()`？

| **方法** | `type(my_car) == Vehicle` | `isinstance(my_car, Vehicle)` |
| ------ | ------------------------- | ----------------------------- |
| 结果     | `False`                   | `True`                        |
| 原因     | `type()` 仅返回精确的 `Car`     | `isinstance()` 检查完整的继承层次结构    |
| 适用场景   | 仅精确类型匹配                   | 考虑继承关系的检查                     |
