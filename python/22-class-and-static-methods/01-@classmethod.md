### `@classmethod` 装饰器与工厂方法

在 Python 中，你在类内部编写的大多数方法都是**实例方法**——它们以 `self` 作为第一个参数，并在特定的实例上操作。但有时你需要一个属于*类本身*而非任何特定实例的方法。这时就可以使用 `@classmethod`。最常见的使用场景是构建**工厂方法**：根据不同类型的输入数据创建实例的备选构造函数。

### 工作原理

当你用 `@classmethod` 装饰一个方法时，Python 会自动将**类**（而非实例）作为第一个参数传入。按照惯例，这个参数被命名为 `cls`。在方法内部，你可以调用 `cls(...)` 来创建并返回一个新实例——就像直接调用类构造函数一样，但其优势在于它在继承体系下也能正确工作。

核心区别：

* **实例方法**：`def greet(self)` —— `self` 是一个已存在的对象
* **类方法**：`@classmethod` + `def create(cls, ...)` —— `cls` 是类本身

### 语法

```python
class MyClass:
    def __init__(self, value):
        self.value = value

    @classmethod
    def from_something(cls, data):
        # Parse or transform data
        # Then create and return an instance
        return cls(transformed_value)
```

### 示例

```python
# A Temperature class with factory methods for different units
class Temperature:
    def __init__(self, celsius):
        self.celsius = celsius

    @classmethod
    def from_fahrenheit(cls, f):
        c = (f - 32) * 5 / 9
        return cls(c)

    @classmethod
    def from_kelvin(cls, k):
        c = k - 273.15
        return cls(c)

    def show(self):
        return f"{self.celsius:.1f}°C"

# Using the standard constructor
t1 = Temperature(100)
print(t1.show())  # 100.0°C

# Using a factory method
t2 = Temperature.from_fahrenheit(212)
print(t2.show())  # 100.0°C

t3 = Temperature.from_kelvin(373.15)
print(t3.show())  # 100.0°C
```

```python
# A Person class with a factory method that parses a full name string
class Person:
    def __init__(self, first_name, last_name):
        self.first_name = first_name
        self.last_name = last_name

    @classmethod
    def from_full_name(cls, name_str):
        first, last = name_str.split(" ", 1)
        return cls(first, last)

    def greeting(self):
        return f"Hello, {self.first_name} {self.last_name}!"

p = Person.from_full_name("Ada Lovelace")
print(p.greeting())  # Hello, Ada Lovelace!
```

请注意其中的模式：`@classmethod` 接收原始数据（字符串、不同单位的数字等），对其进行处理，然后调用 `cls(...)` 来构造实例。

### 为什么使用 `cls` 而不是直接使用类名？

使用 `cls(...)` 而不是硬编码类名（例如 `Temperature(...)`）意味着你的工厂方法在子类中也能正确创建子类的实例。对于可扩展的代码来说，这是一个重要的设计考量。

### 参考信息

| **概念描述示例**          |                   |                                           |
| ------------------- | ----------------- | ----------------------------------------- |
| `@classmethod`      | 使方法接收类作为第一个参数的装饰器 | `def method(cls, ...)` 上方的 `@classmethod` |
| `cls(...)`          | 从类方法内部调用类构造函数     | `return cls(value)`                       |
| `str.split(sep)`    | 按分隔符将字符串拆分为列表     | `"a-b-c".split("-")` → `["a", "b", "c"]`  |
| `int()`             | 将字符串转换为整数         | `int("42")` → `42`                        |
| f-string formatting | 使用 `:02d` 进行前导零补齐 | `f"{5:02d}"` → `"05"`                     |
