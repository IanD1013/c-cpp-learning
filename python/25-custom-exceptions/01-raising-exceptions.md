### 使用 `raise` 抛出异常

在 Python 中，你已经知道如何使用 `try`/`except` 来*捕获*（catch）异常。但是另一方面——自己*抛出*（raise）异常又是怎样的呢？当你的函数接收到违背其约定（contract）的输入时，你应该立即发出问题信号，而不是返回一个无意义的垃圾结果或像 `-1`、`None` 这样的特殊哨兵值（sentinel value）。`raise` 关键字允许你主动触发异常，迫使调用者来处理该问题。

### 工作原理

当 Python 遇到 `raise` 语句时，它会立即停止正常执行并开始展开调用栈（unwinding the call stack），寻找匹配的 `except` 块。如果没有找到匹配项，程序将终止并输出 traceback（回溯信息）。这与 Python 自身遇到 `ZeroDivisionError` 或 `KeyError` 时触发的机制完全相同——你只是根据自己的规则和条件来主动触发它。

核心要点：**抛出异常是函数表达“此输入根本上是错误的”的方式**，而不是“这里有一个略显奇怪的结果”。它将错误处理的责任从函数的提供者转移到了消费者。

### 语法

```python
# Raise an exception with a descriptive message
raise ExceptionType("Descriptive error message")

# Common built-in exception types to raise:
# ValueError  - wrong value (right type, wrong content)
# TypeError   - wrong type entirely
# RuntimeError - general operational error
```

### 示例

```python
# Example 1: Validating a temperature reading
def set_temperature(celsius):
    if not isinstance(celsius, (int, float)):
        raise TypeError("Temperature must be a number")
    if celsius < -273.15:
        raise ValueError("Temperature cannot be below absolute zero")
    return f"Temperature set to {celsius}°C"

set_temperature(22)        # Returns: "Temperature set to 22°C"
set_temperature(-300)      # Raises ValueError: Temperature cannot be below absolute zero
set_temperature("warm")   # Raises TypeError: Temperature must be a number

# Example 2: Validating a username
def create_username(name):
    if not isinstance(name, str):
        raise TypeError("Username must be a string")
    if len(name) < 3:
        raise ValueError("Username must be at least 3 characters")
    if len(name) > 20:
        raise ValueError("Username must be 20 characters or fewer")
    return f"User '{name}' created"

create_username("alice")  # Returns: "User 'alice' created"
create_username("ab")     # Raises ValueError: Username must be at least 3 characters
create_username(42)        # Raises TypeError: Username must be a string
```

### 使用 `isinstance` 进行类型检查

在验证输入类型时，`isinstance()` 是标准方法：

```python
isinstance(42, int)       # True
isinstance(3.14, int)     # False
isinstance("hello", str)  # True
isinstance(True, int)     # True! — booleans are a subclass of int in Python
```

最后一行是一个容易踩的坑（gotcha）。在 Python 中，`True` 和 `False` 在技术上是整数（`True == 1`，`False == 0`）。如果你的验证逻辑需要拒绝布尔值，你需要显式检查 `bool`，因为 `isinstance(True, int)` 会返回 `True`。
