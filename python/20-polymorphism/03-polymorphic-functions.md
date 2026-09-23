### 多态函数

多态函数（Polymorphic Function）是指只要对象具有共同的方法，就能够对不同类型的对象进行操作的函数。该函数不需要知道每个对象的具体类型——它只需调用共享的方法，并相信每种类型都会正确实现该方法。这是编写灵活、可扩展代码的最强大模式之一，可以在无需修改代码的情况下从容应对新类型。

### 工作原理

假设你有几种不同类型的员工——全职、兼职和外包员工。他们计算工资的规则各不相同，但每个人都实现了一个 `calculate_pay()` 方法。工资核算函数可以遍历由不同员工组成的混合列表，对每个人调用 `calculate_pay()`，并且每次都能获得正确的结果——完全不需要检查每个员工是什么类型。

关键组成要素包括：

1. 一个**基类**（base class），定义了共享方法（通常带有默认或占位实现）。
2. **子类**（subclasses），使用各自特定的逻辑重写（override）该方法。
3. 一个**函数**，接受任何具有该方法的对象并统一调用它。

### 语法

```python
class Animal:
    def speak(self):
        return "..."

class Dog(Animal):
    def speak(self):
        return "Woof!"

class Cat(Animal):
    def speak(self):
        return "Meow!"

# 多态函数 — 适用于任何 Animal 子类
def all_sounds(animals):
    return [a.speak() for a in animals]
```

### 示例

```python
# 示例 1：不同的车辆，相同的接口
class Vehicle:
    def fuel_cost(self, distance):
        return 0

class Car(Vehicle):
    def __init__(self, mpg):
        self.mpg = mpg
    def fuel_cost(self, distance):
        return round(distance / self.mpg * 3.50, 2)

class Bicycle(Vehicle):
    def fuel_cost(self, distance):
        return 0  # 不需要燃料！

# 多态函数 — 不关心车辆类型
def trip_cost(vehicles, distance):
    return sum(v.fuel_cost(distance) for v in vehicles)

fleet = [Car(30), Bicycle(), Car(25)]
print(trip_cost(fleet, 100))  # 累加所有车辆的花费

# 示例 2：从数据构建对象，然后多态地使用它们
def make_vehicle(data):
    kind, value = data
    if kind == "car":
        return Car(value)
    elif kind == "bicycle":
        return Bicycle()
    return Vehicle()

vehicle_data = [("car", 30), ("bicycle", 0), ("car", 25)]
fleet = [make_vehicle(d) for d in vehicle_data]
print(trip_cost(fleet, 100))
```

### 常见模式

* **具有默认行为的基类**：基类提供安全的回退实现（例如返回 `0`），从而即使遇到无法识别的类型也不会导致程序崩溃。
* **基于数据的工厂式构建**：通常你会接收到原始数据（元组、字典、JSON），需要在调用共享方法之前创建正确的对象。
* **多态集合上的聚合**：通过对混合列表中的每个对象调用相同的方法，来进行求和、求平均值或收集结果。

### 实用参考

| **概念描述示例**              |                   |                              |
| ----------------------- | ----------------- | ---------------------------- |
| 方法重写（Method overriding） | 子类重新定义父类方法        | `def area(self): return ...` |
| `round(value, n)`       | 将浮点数四舍五入到 `n` 位小数 | `round(3.14159, 2)` → `3.14` |
| `sum(iterable)`         | 返回所有项的总和          | `sum([1, 2, 3])` → `6`       |
| 元组解包（Tuple unpacking）   | 从元组中提取值           | `kind, val = ("circle", 5)`  |
