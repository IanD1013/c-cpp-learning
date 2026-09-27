### Dataclass 中的 `__post_init__` 方法

当你使用 `@dataclass` 时，Python 会自动生成一个 `__init__` 方法，该方法会从构造函数参数中为每个声明的字段赋值。但是，如果你需要验证这些值、对其进行转换或计算依赖于输入的新字段，该怎么办呢？这正是 `__post_init__` 的用武之地 —— 它是一个在 `__init__` 执行完毕后立即自动运行的钩子（hook）。

### 工作原理

执行顺序非常直观：

1. **首先运行 `__init__`** —— 它会根据传入构造函数的参数为所有声明的字段赋值。
2. **接着运行 `__post_init__`** —— 它可以访问 `__init__` 刚刚设置的所有字段，因此你可以对其进行验证、转换，或者用它们来计算新值。

这种两阶段初始化既保留了自动生成 `__init__` 的便利性，又提供了自定义逻辑的灵活性。

### 使用 `field(init=False)` 的计算字段

有时你希望某个字段出现在 dataclass 中（以及 `repr` 中），但它**不是**构造函数参数。你可以使用 `field(init=False)` 来声明它，这会将其从生成的 `__init__` 中排除。然后你在 `__post_init__` 内部为其赋值。

### 语法

```python
from dataclasses import dataclass, field

@dataclass
class MyClass:
    input_field: type
    computed_field: type = field(init=False)

    def __post_init__(self):
        # Validate input_field
        if some_condition:
            raise ValueError("Descriptive message")

        # Compute derived field
        self.computed_field = some_expression
```

### 示例

```python
from dataclasses import dataclass, field

@dataclass
class Rectangle:
    width: float
    height: float
    area: float = field(init=False)

    def __post_init__(self):
        if self.width <= 0 or self.height <= 0:
            raise ValueError("Dimensions must be positive")

        self.area = round(self.width * self.height, 2)


r = Rectangle(4.5, 3.0)

print(r.area)    # 13.5
print(repr(r))   # Rectangle(width=4.5, height=3.0, area=13.5)


# Validation in action:
try:
    Rectangle(-1, 5)
except ValueError as e:
    print(e)     # Dimensions must be positive
```

```python
@dataclass
class FullName:
    first: str
    last: str
    display: str = field(init=False)

    def __post_init__(self):
        if not self.first.strip() or not self.last.strip():
            raise ValueError("Names cannot be blank")

        self.display = f"{self.first} {self.last}"


print(FullName("Ada", "Lovelace").display)  # Ada Lovelace
```

请注意，`area` 和 `display` 是真正的字段 —— 它们会显示在 `repr`、相等性检查以及其他所有地方 —— 但调用者从不需要将它们传递给构造函数。

### 验证模式

一种常见的模式是捕获已知不良输入的验证错误，以确认你的保护子句（guard clauses）是否正常工作：

```python
try:
    Rectangle(-3, 10)
    result = "missed"    # __post_init__ didn't raise
except ValueError:
    result = "caught"    # validation worked correctly
```