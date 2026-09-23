# 顺序魔术方法（Ordering Dunder Methods）：`__lt__`、`__le__`、`__gt__`、`__ge__`

你已经了解了如何通过 `__eq__` 来为你的对象定义 `==` 的含义。但是 `<`、`<=`、`>` 和 `>=` 该如何处理呢？Python 提供了一组**顺序魔术方法（ordering dunder methods）**，让你可以为类的实例定义自然排序规则。每当你想要比较对象时——例如按绩效对员工进行排名、按价格对产品进行排序、或确定哪个任务具有更高优先级时，这都是至关重要的。

## 工作原理

Python 将每个比较运算符映射到一个特定的魔术方法：

| Operator | Method                | Description |
| -------- | --------------------- | ----------- |
| `<`      | `__lt__(self, other)` | 小于          |
| `<=`     | `__le__(self, other)` | 小于或等于       |
| `>`      | `__gt__(self, other)` | 大于          |
| `>=`     | `__ge__(self, other)` | 大于或等于       |
| `==`     | `__eq__(self, other)` | 等于          |

当你编写 `a > b` 时，Python 首先尝试调用 `a.__gt__(b)`。如果未定义该方法，Python 会回退并尝试 `b.__lt__(a)` —— 这被称为**反射操作（reflected operation）**。由于这种推导机制，**通常只需定义 `__lt__` 和 `__eq__`**，就足以让 Python 处理全部六个比较运算符。

这些方法也为排序提供了支持。当你调用 `sorted()` 或 `list.sort()` 时，Python 默认使用 `<`（即 `__lt__`）来确定元素的顺序。

## 语法

```python
class MyClass:
    def __init__(self, value):
        self.value = value

    def __eq__(self, other):
        return self.value == other.value

    def __lt__(self, other):
        return self.value < other.value
```

仅定义这两个方法后，以下所有操作都可以正常工作：

```python
a < b    # calls a.__lt__(b)
a > b    # Python infers via b.__lt__(a)
a <= b   # Python infers from __lt__ and __eq__
a >= b   # Python infers from __lt__ and __eq__
sorted([a, b, c])  # uses __lt__ to order
```

## 示例

```python
# A Temperature class with natural ordering
class Temperature:
    def __init__(self, city, degrees):
        self.city = city
        self.degrees = degrees

    def __eq__(self, other):
        return self.degrees == other.degrees

    def __lt__(self, other):
        return self.degrees < other.degrees

nyc = Temperature("New York", 72)
la = Temperature("Los Angeles", 85)
chi = Temperature("Chicago", 72)

print(nyc < la)    # True  (72 < 85)
print(la > nyc)    # True  (Python infers from nyc.__lt__(la))
print(nyc == chi)  # True  (72 == 72)
print(nyc >= chi)  # True  (equal, so >= is True)

# Sorting works automatically!
cities = [la, nyc, chi]
ranked = sorted(cities)  # Orders by degrees: [nyc(72), chi(72), la(85)]
```

```python
# A Task class ordered by priority (lower number = higher priority)
class Task:
    def __init__(self, description, priority):
        self.description = description
        self.priority = priority

    def __eq__(self, other):
        return self.priority == other.priority

    def __lt__(self, other):
        return self.priority < other.priority

urgent = Task("Fix bug", 1)
routine = Task("Write docs", 5)

print(urgent < routine)  # True (1 < 5)
print(routine > urgent)  # True (inferred)
```

## 反射操作机制

当 Python 计算 `a > b` 时，它首先尝试调用 `a.__gt__(b)`。如果 `__gt__` 未定义，Python 会尝试其**反射（reflected）**版本：`b.__lt__(a)`。这意味着如果你定义了 `__lt__`，Python 就可以自动处理相反方向的 `>` 比较。类似地，`a >= b` 可以被推导为 `not a.__lt__(b)`，而 `a <= b` 可以被推导为 `a.__lt__(b) or a.__eq__(b)`。

这就是为什么仅定义 `__lt__` 和 `__eq__` 就能为你提供完整的比较功能，而无需编写额外的四个方法。
