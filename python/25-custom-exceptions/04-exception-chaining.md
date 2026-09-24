### 使用 `raise ... from ...` 进行异常链（Exception Chaining）

在构建分层应用程序时，像 `KeyError` 或 `FileNotFoundError` 这样的低级别异常经常会冒泡经过多层代码。对于不了解底层内部实现的调用者来说，原始的底层错误可能会让人困惑。异常链允许你引发一个有意义的高级别异常，同时保留原始的起因，从而两全其美：为调用者提供清晰的错误消息，**并且**保留完整的调试上下文。

### 工作原理

当你编写 `raise NewException("message") from original_exception` 时，Python 会做两件事：

1. 它将 `NewException` 作为活动异常引发。
2. 它将 `original_exception` 存储在新异常的 `__cause__` 属性中。

Python 的 traceback 将显示由短语 **"The above exception was the direct cause of the following exception"** 连接的两个异常。这种显式链是故意的且清晰的。

如果不使用 `from`，当你在 `except` 块中引发异常时，Python 仍会隐式记录上下文（在 `__context__` 中），但 traceback 会显示 **"During handling of the above exception, another exception occurred"** —— 这样不够精确。使用 `from` 可以让因果关系变得明确。

### 语法

```python
try:
    risky_operation()
except SomeLowLevelError as e:
    raise HighLevelError("descriptive message") from e
```

你还可以使用 `raise SomeError("msg") from None` 完全抑制链接，但那是另一种模式。

### 示例

```python
# Example 1: Wrapping a KeyError in a domain-specific exception
class ConfigError(Exception):
    def __init__(self, message, cause=None):
        super().__init__(message)
        self.cause = cause

def get_setting(config, key):
    try:
        return config[key]
    except KeyError as e:
        raise ConfigError(f"Missing setting: {key}", e) from e

# When called with a missing key, the traceback shows:
# KeyError: 'database_url'
# The above exception was the direct cause of the following exception:
# ConfigError: Missing setting: database_url
```

```python
# Example 2: Accessing __cause__ programmatically
try:
    get_setting({}, "database_url")
except ConfigError as e:
    print(e)             # Missing setting: database_url
    print(e.__cause__)   # 'database_url'
    print(type(e.cause)) # <class 'KeyError'>
```

```python
# Example 3: A custom exception storing the original for later inspection
class ParseError(Exception):
    def __init__(self, msg, original=None):
        super().__init__(msg)
        self.original = original

def parse_age(text):
    try:
        age = int(text)
        if age < 0:
            raise ValueError("Age cannot be negative")
        return age
    except ValueError as e:
        raise ParseError(f"Invalid age input: '{text}'", e) from e
```

### 常见模式

* **带有 `original` 属性的自定义异常**：将捕获的异常存储在实例属性中，以便调用者无需解析 traceback 即可检查根本原因。
* **分层 try/except**：外层函数捕获领域异常并格式化出用户友好的响应，而内层函数处理低级别的转换并链接错误。
* **`str(exception)`**：在异常上调用 `str()` 会返回传递给其构造函数的错误消息，这对于构建错误字符串非常有用。
