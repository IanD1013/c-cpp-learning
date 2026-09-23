### 在子类中添加新方法

当你通过继承创建子类时，你不仅限于重写父类的方法。子类还可以引入父类中根本不存在的**全新方法**。这就是你如何为专门的类型赋予其独特的专属功能，同时仍然继承父类的共享行为。

可以把它想象成智能手机继承了基础“手机”的功能（通话、短信），但添加了像 `take_photo()` 或 `install_app()` 这样的新功能——这些是基础手机从未有过的功能。

### 工作原理

当 Python 在对象上查找方法时，它首先检查对象本身的类，然后沿着继承链向上查找。**仅**在子类上定义的方法只能被该子类的实例（或其自身的子类）访问。父类对此一无所知，父类实例无法调用它。

这意味着：

* 子类实例可以调用父类方法和它们自己的新方法**两者**
* 父类实例只能调用父类方法
* 新的子类方法可以自由访问 `self` 以及由父类 `__init__` 设置的任何属性

### 语法

```python
class Parent:
    def __init__(self, name):
        self.name = name

    def greet(self):
        return f"Hello, {self.name}"

class Child(Parent):
    def __init__(self, name, skill):
        super().__init__(name)
        self.skill = skill

    # 只有 Child 拥有的全新方法
    def show_skill(self):
        return f"{self.name} can {self.skill}"
```

### 示例

```python
# 具有共享行为的基础 Animal
class Animal:
    def __init__(self, name, energy=50):
        self.name = name
        self.energy = energy

    def eat(self, amount):
        self.energy = min(100, self.energy + amount)

# Dog 添加了 fetch() —— 这是只有狗才会做的事情
class Dog(Animal):
    def __init__(self, name, energy=50):
        super().__init__(name, energy)

    def fetch(self, times):
        self.energy = max(0, self.energy - times * 5)
        return f"{self.name} fetched {times} times!"

rex = Dog("Rex", 80)
rex.eat(10)          # 继承自 Animal -> energy = 90
rex.fetch(4)         # Dog 的新方法 -> energy = 90 - 20 = 70

cat = Animal("Whiskers", 60)
cat.eat(15)          # 正常工作 -> energy = 75
# cat.fetch(3)       # AttributeError! Animal 没有 fetch 方法
```

```python
# 另一个示例：Shape 和 Circle
class Shape:
    def __init__(self, color):
        self.color = color

    def describe(self):
        return f"A {self.color} shape"

class Circle(Shape):
    def __init__(self, color, radius):
        super().__init__(color)
        self.radius = radius

    # Circle 独有的新方法
    def area(self):
        return 3.14159 * self.radius ** 2

c = Circle("red", 5)
c.describe()   # 继承自父类："A red shape"
c.area()       # 新方法：78.53975
```

### 核心原则

* **新方法可以修改继承的属性**：子类方法可以读取和写入 `self.fuel`、`self.energy` 等属性，即使这些属性是在父类的 `__init__` 中设置的。
* **限制数值范围（Clamping values）**：使用 `max(lower_bound, value)` 来强制执行最小值，使用 `min(upper_bound, value)` 来强制执行最大值。
* **`super().__init__(...)`** 确保在子类添加任何新内容之前，父类先设置好其属性。
