### `@staticmethod` 装饰器

在 Python 中，类不仅可以包含实例方法和类方法——它们还可以包含**静态方法**。静态方法本质上是一个普通的函数，纯粹出于组织代码的目的而放置在类内部。它**不会**接收 `self`（实例）或 `cls`（类）作为其第一个参数。当你的工具函数在逻辑上与类相关，但不需要读取或修改任何实例或类数据时，就可以使用 `@staticmethod`。

### 工作原理

当你使用 `@staticmethod` 装饰一个方法时，Python 会跳过自动传递实例或类引用的过程。该方法的行为与普通函数完全一致——它只能访问自己的参数。这使得静态方法非常适合将辅助函数归类到某个类的命名空间下，在保持代码整洁的同时避免污染模块级别的作用域。

静态方法可以通过两种方式调用：

* 直接通过类调用：`ClassName.method()`
* 通过实例调用：`instance.method()`（不过通过类调用更为常见）

### 语法

```python
class SomeClass:
    @staticmethod
    def some_utility(arg1, arg2):
        # 没有 self，没有 cls — 只有普通参数
        return arg1 + arg2
```

### 示例

```python
class TemperatureConverter:
    @staticmethod
    def celsius_to_fahrenheit(c):
        return c * 9 / 5 + 32

    @staticmethod
    def fahrenheit_to_celsius(f):
        return (f - 32) * 5 / 9

# 通过类调用 — 无需实例化
print(TemperatureConverter.celsius_to_fahrenheit(100))  # 212.0
print(TemperatureConverter.fahrenheit_to_celsius(32))   # 0.0
```

```python
class StringUtils:
    @staticmethod
    def is_palindrome(text):
        cleaned = text.lower().replace(" ", "")
        return cleaned == cleaned[::-1]

    @staticmethod
    def truncate(text, max_length):
        if len(text) <= max_length:
            return text
        return text[:max_length] + "..."

print(StringUtils.is_palindrome("racecar"))    # True
print(StringUtils.truncate("Hello World", 5))  # Hello...
```

请注意，`TemperatureConverter` 和 `StringUtils` 都没有存储任何数据——它们只是将相关的工具函数归类在一个有意义的类名之下。

### 比较 `@staticmethod` 与其他方法类型

| **装饰器**         | **第一个参数** | **访问权限** | **最适用于**     |
| --------------- | --------- | -------- | ------------ |
| *（无）*           | `self`    | 实例数据     | 读取/修改实例状态的方法 |
| `@classmethod`  | `cls`     | 类数据      | 工厂方法、备用构造函数  |
| `@staticmethod` | *（无）*     | 无自动传递内容  | 归类在类下的工具函数   |
