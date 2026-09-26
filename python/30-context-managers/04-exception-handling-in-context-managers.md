# 上下文管理器中的异常处理

上下文管理器不仅用于管理资源的分配与释放——它们还可以拦截并处理 `with` 块内部发生的异常。这使它们成为构建可复用错误处理模式（如重试逻辑、优雅降级或结构化错误日志记录）的强大工具。

## `__exit__` 如何接收异常信息

当 `with` 块执行完毕时，Python 会调用上下文管理器的 `__exit__` 方法并传入三个参数：

- **`exc_type`**：异常类（例如 `TypeError`、`KeyError`），如果未发生异常则为 `None`。
- **`exc_val`**：异常实例本身，或 `None`。
- **`exc_tb`**：回溯对象（traceback object），或 `None`。

如果未抛出异常，所有这三个参数都为 `None`。如果发生了异常，这些参数能让你完全访问该异常，以便进行检查、记录日志或从中恢复。

## 抑制与传播异常

`__exit__` 的返回值决定了接下来的处理流程：

- **返回 `True`**：异常被**抑制**（suppressed）——Python 会像什么都没发生一样，继续执行 `with` 块后面的代码。
- **返回 `False`（或 `None`）**：异常正常**传播**（propagates），就好像上下文管理器没有参与一样。

这使你可以进行选择性控制：捕获特定的异常类型，同时让其他异常向外抛出。

## 语法

```python
class MyContextManager:
    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        if exc_type is SomeExceptionClass:
            # Handle it — maybe log, store info, etc.
            return True   # Suppress
        return False      # Let everything else propagate
```

## 示例

### 示例 1：抑制特定异常

```python
class IgnoreKeyError:
    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        if exc_type is KeyError:
            return True
        return False


data = {"a": 1}

with IgnoreKeyError():
    print(data["z"])  # KeyError is silently suppressed

print("Continues running!")  # This executes normally
```

### 示例 2：记录错误并抑制该异常

```python
class ErrorLogger:
    def __init__(self):
        self.logged_error = None

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        if exc_type is ZeroDivisionError:
            self.logged_error = str(exc_val)
            return True
        return False


logger = ErrorLogger()

with logger:
    x = 1 / 0

print(logger.logged_error)  # "division by zero"
```

请注意，`logger.logged_error` 在 `with` 块**之后**仍然可以访问，因为 `logger` 是在外部作用域中定义的。

### 示例 3：让未处理的异常继续传播

```python
class OnlyHandleTypeError:
    def __exit__(self, exc_type, exc_val, exc_tb):
        if exc_type is TypeError:
            return True
        return False  # Everything else propagates

    def __enter__(self):
        return self


with OnlyHandleTypeError():
    int("not_a_number")  # Raises ValueError — NOT suppressed!

# ValueError propagates normally and crashes here
```

## 检查异常类型

使用 `is` 运算符将 `exc_type` 与特定的异常类进行比较：

```python
if exc_type is ValueError:    # Exact match
    ...
```

另外，`issubclass` 可以处理继承关系：

```python
if exc_type is not None and issubclass(exc_type, OSError):
    ...
```

使用 `str(exc_val)` 将异常转换为其错误消息字符串。