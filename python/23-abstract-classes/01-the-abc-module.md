### 抽象基类（Abstract Base Classes，ABCs）

在实际软件开发中，你经常需要保证一组相关的类都共享一个通用接口——即它们**必须**实现的一组方法。例如，每个支付处理器都必须有一个 `charge()` 方法，或者每个数据库驱动程序都必须有一个 `connect()` 方法。抽象基类在类级别强制执行这一点：如果子类忘记实现必需的方法，Python 会在创建任何实例之前引发 `TypeError`。

### 工作原理

Python 的 `abc` 模块提供了两个关键工具：

* **`ABC`** —— 一个辅助类，通过继承它来使你的类成为抽象类。
* **`@abstractmethod`** —— 一个装饰器，用于将方法标记为必需方法。任何具体（非抽象）子类**必须**重写每个 `@abstractmethod`，否则 Python 将拒绝实例化它。

可以将抽象类视为一份契约：“任何自称为 `Vehicle` 的类都必须具有 `fuel_type()` 方法。”抽象类本身不能被实例化——它的存在仅仅是为了定义规则。

### 语法

```python
from abc import ABC, abstractmethod

class MyAbstractClass(ABC):
    @abstractmethod
    def required_method(self):
        pass

    def concrete_method(self):
        # This method has an implementation and is NOT abstract
        return "I work as-is"
```

子类必须重写 `required_method`，但可以直接免费继承 `concrete_method`。

### 示例

```python
from abc import ABC, abstractmethod

class Animal(ABC):
    @abstractmethod
    def speak(self):
        pass

    def breathe(self):
        return "inhale... exhale..."

class Dog(Animal):
    def speak(self):
        return "Woof!"

class Cat(Animal):
    def speak(self):
        return "Meow!"

# This works:
dog = Dog()
print(dog.speak())     # "Woof!"
print(dog.breathe())   # "inhale... exhale..."

# This raises TypeError — Animal is abstract:
# animal = Animal()  # TypeError: Can't instantiate abstract class
```

请注意，`Dog` 和 `Cat` 都重写了 `speak()`，因此它们可以被实例化。如果 `Dog` 忘记定义 `speak()`，创建 `Dog()` 将引发 `TypeError`。

```python
# Abstract classes can also have __init__ and @abstractmethod with parameters:
class PaymentProcessor(ABC):
    def __init__(self, merchant_id):
        self.merchant_id = merchant_id

    @abstractmethod
    def charge(self, amount):
        pass

class StripeProcessor(PaymentProcessor):
    def charge(self, amount):
        return f"Charged ${amount:.2f} via Stripe for merchant {self.merchant_id}"

processor = StripeProcessor("M-1234")
print(processor.charge(49.99))  # "Charged $49.99 via Stripe for merchant M-1234"
```

### 常见模式

* **工厂风格的函数**通常接收数据，并根据某种标识符（字符串、枚举等）创建适当的具体子类，然后多态地调用所创建对象上的方法。
* 使用 `round(value, 2)` 进行**四舍五入**可以保留两位小数——这在面积计算中很有用。
* **元组解包**允许你提取值：如果 `data = ("dog", "Rex")`，那么 `data[0]` 为 `"dog"`，`data[1]` 为 `"Rex"`。
