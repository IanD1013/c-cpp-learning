### `__str__` 与 `__repr__`：控制字符串表示形式

每个 Python 对象都可以转换为字符串，但实际上一个对象可以拥有**两种不同**的字符串表示形式。`__str__` 方法定义了**用户友好**的版本——即最终用户看到的内容。`__repr__` 方法定义了**开发者友好**的版本——即开发者在调试时看到的内容。理解何时调用它们以及它们如何相互配合，对于编写清晰、易于调试的 Python 类至关重要。

### 工作原理

当你调用 `str(obj)` 或 `print(obj)` 时，Python 会调用 `obj.__str__()`。当你调用 `repr(obj)` 或在交互式控制台中检查对象时，Python 会调用 `obj.__repr__()`。

这里有一个重要的**回退规则**：如果一个类定义了 `__repr__` 但没有定义 `__str__`，那么 `str(obj)` 会回退使用 `__repr__`。然而，反过来则**不成立**——仅定义 `__str__` 不会影响 `repr()`。

另一个微妙但关键的行为：当对象被放入列表、字典或集合等**容器**中时，Python 会使用 `__repr__`（而不是 `__str__`）来显示它们。因此，`print([my_obj])` 会对 `my_obj` 调用 `__repr__`，即使 `print(my_obj)` 会调用 `__str__`。

### 约定规范

按照惯例，`__repr__` 理想情况下应该返回一个如果传递给 `eval()` 能够**重新创建**该对象的字符串。这让调试变得更加容易，因为你可以清楚地看到如何重构任何对象。

### 语法

```python
class MyClass:
    def __init__(self, value):
        self.value = value

    def __str__(self):
        return "a human-readable string"

    def __repr__(self):
        return "MyClass(value_that_recreates_it)"
```

### 示例

```python
class Temperature:
    def __init__(self, celsius):
        self.celsius = celsius

    def __str__(self):
        return f"{self.celsius}°C"

    def __repr__(self):
        return f"Temperature({self.celsius})"

t = Temperature(100)
print(str(t))        # 100°C        (user-friendly)
print(repr(t))       # Temperature(100)  (developer-friendly, recreatable)
print(t)             # 100°C        (print uses __str__)
print([t])           # [Temperature(100)] (containers use __repr__!)
```

```python
class Color:
    def __init__(self, r, g, b):
        self.r = r
        self.g = g
        self.b = b

    def __str__(self):
        return f"rgb({self.r}, {self.g}, {self.b})"

    def __repr__(self):
        return f"Color({self.r}, {self.g}, {self.b})"

c = Color(255, 128, 0)
str(c)    # "rgb(255, 128, 0)"
repr(c)   # "Color(255, 128, 0)"
```

### 回退行为

```python
class OnlyRepr:
    def __repr__(self):
        return "OnlyRepr()"

obj = OnlyRepr()
print(repr(obj))  # OnlyRepr()
print(str(obj))   # OnlyRepr()  <-- falls back to __repr__!
```
