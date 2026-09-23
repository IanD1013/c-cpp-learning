### 使用 `self` 调用其他方法

在类内部，方法通常需要相互协作。一个方法可以通过使用 `self.method_name()` 来调用同一对象上的另一个方法。这是函数分解（function decomposition）在面向对象中的等价形式——将复杂的行为拆分为更小、更专注的方法，每个方法只做好一件事。这能保持代码条理清晰、避免重复，并使每个部分更容易测试和理解。

### 工作原理

当你在方法内部编写 `self.method_name()` 时，Python 会在当前实例上查找 `method_name` 并调用它。被调用的方法就像任何其他实例方法一样，可以通过 `self` 完全访问该实例的属性。这意味着你可以构建内部计算链，由一个“公开”方法在后台调度组织多个“私有”辅助方法。

按照惯例，以下划线开头的方法（例如 `_helper_method`）表示它们仅供类内部使用，不供外部调用者使用。Python 并不会强制限制这一点——这是一种向其他开发者传达意图的约定。

### 语法

```python
class MyClass:
    def __init__(self, value):
        self.value = value

    def _internal_helper(self):
        # Perform some computation using self.value
        return self.value * 2

    def public_method(self):
        # Call the helper method using self
        result = self._internal_helper()
        return f"Result: {result}"
```

### 示例

```python
class Rectangle:
    def __init__(self, width, height):
        self.width = width
        self.height = height

    def _area(self):
        return self.width * self.height

    def _perimeter(self):
        return 2 * (self.width + self.height)

    def summary(self):
        # Calls two other methods on the same object
        return f"Area: {self._area()}, Perimeter: {self._perimeter()}"

r = Rectangle(5, 3)
print(r.summary())  # "Area: 15, Perimeter: 16"
```

```python
class BankAccount:
    def __init__(self, balance):
        self.balance = balance

    def _apply_fee(self, amount):
        fee = amount * 0.02
        return amount + fee

    def withdraw(self, amount):
        total = self._apply_fee(amount)
        self.balance -= total
        return f"Withdrew {total}, balance: {self.balance}"

acct = BankAccount(1000)
print(acct.withdraw(100))  # "Withdrew 102.0, balance: 898.0"
```

注意每个类是如何使用私有辅助方法（带有 `_` 前缀）来处理内部计算，而公开方法通过 `self` 调用它们以生成最终结果的。

### 常见模式

* **计算辅助方法**：一个方法进行原始计算，另一个方法格式化输出
* **验证辅助方法**：在执行操作前调用 `_validate` 方法
* **分步处理**：一个主方法依次调用多个辅助方法，每个辅助方法处理一个步骤
