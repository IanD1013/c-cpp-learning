### 在抽象类中混合抽象方法与具体方法

抽象类不仅仅是抽象方法的容器——它们还可以包含完全实现的**具体方法（concrete methods）**，所有子类都可以直接继承这些方法。这是面向对象设计中最强大的模式之一：抽象类在具体方法中定义共享行为，同时将变化的部分委托给每个子类必须实现的抽象方法。

关键在于抽象类中的具体方法可以**调用**抽象方法。它确信任何子类都会提供相应的实现。这有时被称为**模板方法模式（Template Method Pattern）**——基类定义算法的骨架，而子类负责填充具体细节。

### 工作原理

当定义同时包含这两种方法的抽象类时：

* **抽象方法**（`@abstractmethod`）：没有实现。每个子类*必须*重写它们，否则 Python 会在实例化时引发 `TypeError`。
* **具体方法**：具有完整实现的常规方法。子类会自动继承它们，但如果需要，仍然可以重写它们。

当具体方法调用 `self.some_abstract_method()` 时，Python 的方法解析顺序（MRO）会确保调用子类的实现——即使具体方法是在基类中定义的。

### 语法

```python
from abc import ABC, abstractmethod

class BaseProcessor(ABC):
    @abstractmethod
    def process(self, raw_data):
        """Subclasses must implement this."""
        pass

    def run(self, raw_data):
        """Concrete method that uses the abstract method."""
        result = self.process(raw_data)
        return f"Processed: {result}"
```

### 示例

```python
from abc import ABC, abstractmethod

class Shape(ABC):
    @abstractmethod
    def area(self):
        pass

    def describe(self):
        # Concrete method calling the abstract method
        return f"This shape has an area of {self.area():.2f}"

class Circle(Shape):
    def __init__(self, radius):
        self.radius = radius

    def area(self):
        return 3.14159 * self.radius ** 2

c = Circle(5)
c.describe()  # "This shape has an area of 78.54"
# The base class's describe() called Circle's area() automatically
```

```python
class Notifier(ABC):
    @abstractmethod
    def send(self, message):
        pass

    def send_urgent(self, message):
        # Concrete method that decorates, then delegates to abstract method
        return self.send(f"URGENT: {message}")

class EmailNotifier(Notifier):
    def send(self, message):
        return f"Email -> {message}"

class SmsNotifier(Notifier):
    def send(self, message):
        return f"SMS -> {message}"

EmailNotifier().send_urgent("Server down")  # "Email -> URGENT: Server down"
SmsNotifier().send_urgent("Server down")    # "SMS -> URGENT: Server down"
```

请注意 `send_urgent` 只在基类中编写了一次，但会根据由哪个子类提供 `send` 来产生不同的结果。

### 常见模式

* **截断 / 汇总**：具体方法格式化或截断抽象方法的输出。
* **日志 / 包装**：具体方法在抽象方法调用周围添加日志记录、计时或错误处理。
* **验证**：具体方法在将输入传递给抽象方法之前对其进行验证。

从字典构建字符串也是一种常见的格式化任务：

```python
d = {"x": 10, "y": 20}
", ".join(f"{k}->{v}" for k, v in d.items())  # "x->10, y->20"
"{" + "some content" + "}"  # Wrapping in braces: "{some content}"
```
