### 实例方法 (Instance Methods)

实例方法是定义在类内部、用于操作特定对象数据的函数。每个实例方法都接收 `self` 作为其第一个参数，这使得它能够访问存储在该特定对象上的所有属性。这就是对象将数据和行为捆绑在一起的方式 —— 数据保存在属性中，而方法则使用这些数据来执行有意义的操作。

### 工作原理

当你在对象上调用诸如 `my_dog.bark()` 之类的方法时，Python 会自动将该对象本身作为 `self` 参数传入。在方法内部，`self.attribute_name` 用于读取或修改该对象的数据。方法除了 `self` 之外还可以接收额外的参数，并且它们可以像独立函数一样返回值。

一个类通常包含多个方法，每个方法负责不同的操作。这使得相关功能与它所需的数据组织在一起。

### 语法

```python
class ClassName:
    def __init__(self, param1, param2):
        self.param1 = param1
        self.param2 = param2

    def method_one(self):
        # Access attributes via self
        return self.param1 + self.param2

    def method_two(self, extra):
        # Methods can take additional parameters
        return self.param1 * extra
```

### 示例

```python
# A BankAccount class with multiple instance methods
class BankAccount:
    def __init__(self, owner, balance):
        self.owner = owner
        self.balance = balance

    def deposit(self, amount):
        self.balance += amount
        return self.balance

    def get_summary(self):
        return f"{self.owner}: ${self.balance:.2f}"

account = BankAccount("Alice", 1000)
account.deposit(250)          # Returns 1250
account.get_summary()         # Returns "Alice: $1250.00"


# A Rectangle class where methods compute derived values
class Rectangle:
    def __init__(self, width, height):
        self.width = width
        self.height = height

    def perimeter(self):
        return 2 * (self.width + self.height)

    def is_square(self):
        return self.width == self.height

r = Rectangle(4, 4)
r.perimeter()    # Returns 16
r.is_square()    # Returns True
```

### 常见模式

* **Getter 风格方法**：从属性中返回计算后的值，而不修改它们。
* **动作方法 (Action methods)**：修改对象的内部状态（如上面的 `deposit`）。
* **返回衍生数据的方法**：在计算中结合多个属性并返回结果。
* **在外部使用方法的结果**：调用方法并使用 `round()`、格式化字符串或将结果存储在变量中。
