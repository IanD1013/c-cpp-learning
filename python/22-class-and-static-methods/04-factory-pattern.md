### 使用 `@classmethod` 的工厂模式

在实际应用中，对象通常需要从不同类型的输入中创建。数据库记录、JSON 负载、CSV 行或一组预设默认值都可能生成相同类型的对象。**工厂模式**使用 `@classmethod` 方法作为**替代构造函数**——以特定、清晰易读的方式构建实例的具名方法，同时保持 `__init__` 简单且通用。

### 工作原理

工厂类方法接收 `cls`（类本身）作为其第一个参数，执行任何必要的计算或转换，然后调用 `cls(...)` 来创建并返回一个新实例。由于它调用的是 `cls` 而不是直接调用类名，因此工厂方法也可以在子类中正常工作。

核心要点：`__init__` 存储数据，而工厂类方法在存储数据之前**准备**数据。

### 语法

```python
class MyClass:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    @classmethod
    def from_something(cls, raw_input):
        # Transform raw_input into x and y
        x = compute_x(raw_input)
        y = compute_y(raw_input)
        return cls(x, y)
```

### 示例

考虑一个以摄氏度存储数值的 `Temperature` 类：

```python
class Temperature:
    def __init__(self, celsius):
        self.celsius = celsius

    @classmethod
    def from_fahrenheit(cls, f):
        return cls((f - 32) * 5 / 9)

    @classmethod
    def from_kelvin(cls, k):
        return cls(k - 273.15)

    @classmethod
    def boiling(cls):
        return cls(100.0)

    def display(self):
        return f"{round(self.celsius, 2)}°C"
```

```python
t1 = Temperature.from_fahrenheit(212)   # t1.display() -> "100.0°C"
t2 = Temperature.from_kelvin(373.15)    # t2.display() -> "100.0°C"
t3 = Temperature.boiling()              # t3.display() -> "100.0°C"
```

注意每个工厂方法是如何接受不同格式的，但 `__init__` 始终只处理一种规范单位。调用者可以获得一个能明确表达意图的描述性名称。

另一个示例——带有预设的 `Color` 类：

```python
class Color:
    def __init__(self, r, g, b):
        self.r = r
        self.g = g
        self.b = b

    @classmethod
    def from_hex(cls, hex_str):
        r = int(hex_str[1:3], 16)
        g = int(hex_str[3:5], 16)
        b = int(hex_str[5:7], 16)
        return cls(r, g, b)

    @classmethod
    def red(cls):
        return cls(255, 0, 0)
```

### 常见模式

| **模式目的示例**              |              |                                  |
| ----------------------- | ------------ | -------------------------------- |
| `from_string(cls, s)`   | 将字符串解析为结构化数据 | `Date.from_string("2024-01-15")` |
| `from_dict(cls, d)`     | 从字典构建        | `User.from_dict({"name": "Jo"})` |
| `default(cls)`          | 返回一个合理的默认实例  | `Config.default()`               |
| `from_<unit>(cls, val)` | 从特定单位转换      | `Distance.from_miles(26.2)`      |
