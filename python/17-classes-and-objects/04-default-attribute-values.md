### `__init__` 中的默认参数值

当你定义一个类时，`__init__` 方法是用于设置每个新对象的构造函数。因为 `__init__` 是附加在类上的常规函数，所以它支持任何其他函数所拥有的所有相同特性——包括**默认参数值**。这意味着调用者可以在不指定每一个属性的情况下创建对象，从而使你的类更加灵活且易于使用。

### 工作原理

你已经了解了默认参数在常规函数中是如何工作的。完全相同的规则也适用于 `__init__` 内部：

* 带有默认值的参数必须放在没有默认值的参数**之后**。
* 如果调用者提供了实参，它将覆盖默认值。
* 如果调用者省略了实参，则使用默认值。
* 默认值在定义函数时仅求值一次（对于列表等可变默认值要小心——但这属于稍后要讨论的主题）。

### 语法

```python
class ClassName:
    def __init__(self, required_param, optional_param="default_value"):
        self.required_param = required_param
        self.optional_param = optional_param
```

### 示例

```python
# 具有默认端口和超时的 Connection 类
class Connection:
    def __init__(self, host, port=8080, timeout=30):
        self.host = host
        self.port = port
        self.timeout = timeout

c1 = Connection("localhost")
print(c1.host)      # "localhost"
print(c1.port)      # 8080 (default)
print(c1.timeout)   # 30 (default)

c2 = Connection("example.com", 443, 60)
print(c2.port)      # 443 (overridden)
print(c2.timeout)   # 60 (overridden)

# 具有默认颜色的 Rectangle 类
class Rectangle:
    def __init__(self, width, height, color="black"):
        self.width = width
        self.height = height
        self.color = color

r1 = Rectangle(10, 5)
print(r1.color)  # "black" (default)

r2 = Rectangle(10, 5, "red")
print(r2.color)  # "red" (overridden)
```

### 常见模式

`__init__` 中的默认参数在实际代码库中极其常见：

* **配置对象** — 合理的默认值加上可选的覆盖项
* **数据模型** — 通常具有标准值的字段（例如 `status="active"`）
* **构建器风格的类** — 在保持简单用法易用的同时逐步增加复杂性

请记住：当你编写一个创建对象的函数时，该函数也可以接受默认参数并将其传递给构造函数。
