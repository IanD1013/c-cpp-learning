### 实例方法：让属性协同工作

当类不仅能*存储*数据，还能*使用*数据时，它们才会变得真正强大。通过将相关属性归类在一起并定义对这些属性进行操作的**方法**，你可以创建懂得如何执行自身计算的对象。这就是类存在的基石：将数据与作用于该数据的行为打包在一起。

### 方法是如何工作的

**方法**本质上就是定义在类内部的函数。与普通函数的主要区别在于，方法始终将 `self` 作为其第一个参数。这使得方法能够访问对象上存储的每个属性，从而可以读取、组合或转换它们。

当你在对象上调用方法（如 `my_obj.do_something()`）时，Python 会自动将对象本身作为 `self` 传入——你无需在括号中包含它。

### 语法

```python
class ClassName:
    def __init__(self, attr1, attr2):
        self.attr1 = attr1
        self.attr2 = attr2

    def compute(self):
        # self.attr1 and self.attr2 are available here
        return self.attr1 + self.attr2
```

### 示例

```python
# A Circle class that computes its own circumference
class Circle:
    def __init__(self, radius):
        self.radius = radius

    def circumference(self):
        return 2 * 3.14159 * self.radius

c = Circle(10)
print(c.circumference())  # 62.8318

# A BankAccount that tracks balance and computes interest
class BankAccount:
    def __init__(self, owner, balance, rate):
        self.owner = owner
        self.balance = balance
        self.rate = rate

    def annual_interest(self):
        return self.balance * self.rate

acct = BankAccount("Dana", 1000, 0.05)
print(acct.annual_interest())  # 50.0

# A Trip class that calculates average speed
class Trip:
    def __init__(self, distance_km, hours):
        self.distance_km = distance_km
        self.hours = hours

    def avg_speed(self):
        return self.distance_km / self.hours

t = Trip(240, 3)
print(t.avg_speed())  # 80.0
```

注意这种模式：`__init__` 存储原始数据，而方法将这些属性组合起来以产生有意义的结果。调用者不需要知道或重复编写公式——对象会自行处理。

### 常见模式

* **在 `__init__` 中存储，在方法中计算**：将属性赋值与逻辑分开。
* **方法可以调用其他方法**：`self.area()` 可以在另一个方法（如 `self.describe()`）内部使用。
* **从方法返回值**：方法的行为与函数类似——它们可以返回数字、字符串、列表或任何其他内容。
