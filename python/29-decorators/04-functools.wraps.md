### 使用 `functools.wraps` 保留函数元数据

编写装饰器时，你实际上是用一个包装函数（wrapper function）替换了原始函数。这会带来一个意料之外的副作用：包装函数自身的 `__name__`、`__doc__` 和其他元数据属性会覆盖原始函数的对应属性。这会导致调试、日志记录和内省（introspection）变得不可靠——`help()` 会显示错误的文档字符串，堆栈跟踪会显示错误的函数名称，而依赖 `__name__` 的序列化库可能会完全崩溃。

`functools.wraps` 装饰器正是为了解决这个问题而存在的。它将原始函数的关键元数据复制到包装函数上，从而使被装饰后的函数对代码的其他部分而言仍然“看起来像”原始函数。

### 工作原理

每个 Python 函数对象都带有几个元数据属性：

| **属性用途**       |                               |
| -------------- | ----------------------------- |
| `__name__`     | 在源代码中定义的函数名称                  |
| `__doc__`      | 函数的文档字符串                      |
| `__module__`   | 定义该函数的模块                      |
| `__qualname__` | 限定名称（对于方法包含类名）                |
| `__dict__`     | 在函数上设置的任何自定义属性                |
| `__wrapped__`  | 由 `functools.wraps` 设置，指向原始函数 |

当你在装饰器内部对包装函数应用 `@functools.wraps(original_func)` 时，它会复制 `__name__`、`__doc__`、`__module__`、`__qualname__`、`__dict__`，并设置 `__wrapped__` 以引用原始函数。

### 语法

```python
import functools

def some_decorator(func):
    @functools.wraps(func)    # <-- applied to the wrapper, passing the original func
    def wrapper(*args, **kwargs):
        # ... extra behavior ...
        return func(*args, **kwargs)
    return wrapper
```

关键细节：`@functools.wraps(func)` 应用于 **wrapper** 函数，并将 **原始** 函数（`func`）作为参数传入。

### 示例

```python
import functools

# WITHOUT functools.wraps — metadata is lost
def log_calls_bad(func):
    def wrapper(*args, **kwargs):
        print(f"Calling {func.__name__}")
        return func(*args, **kwargs)
    return wrapper

@log_calls_bad
def greet(name):
    """Return a greeting message."""
    return f"Hello, {name}!"

print(greet.__name__)   # 'wrapper'  — Wrong!
print(greet.__doc__)    # None       — Lost!


# WITH functools.wraps — metadata is preserved
def log_calls_good(func):
    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        print(f"Calling {func.__name__}")
        return func(*args, **kwargs)
    return wrapper

@log_calls_good
def farewell(name):
    """Return a farewell message."""
    return f"Goodbye, {name}!"

print(farewell.__name__)   # 'farewell'                — Correct!
print(farewell.__doc__)    # 'Return a farewell message.' — Preserved!
print(farewell.__wrapped__)  # <function farewell at 0x...> — Original accessible
```

```python
import functools

# Works with decorators that accept arguments too
def repeat(n):
    def decorator(func):
        @functools.wraps(func)
        def wrapper(*args, **kwargs):
            for _ in range(n):
                result = func(*args, **kwargs)
            return result
        return wrapper
    return decorator

@repeat(3)
def say_hi():
    """Print hi."""
    print("Hi!")

print(say_hi.__name__)  # 'say_hi' — preserved even with parameterized decorator
```

### 为什么这在实践中很重要

* **调试**：堆栈跟踪和性能分析工具（profiler）使用 `__name__` 来识别函数。如果不使用 `functools.wraps`，每个被装饰的函数都会显示为 `wrapper`。
* **文档工具**：`help()`、Sphinx 和其他文档生成器会读取 `__doc__`。若不保留元数据，你精心编写的文档字符串就会丢失。
* **序列化**：像 `pickle` 这样的库依赖 `__qualname__` 和 `__module__` 来定位函数。元数据损坏可能会导致序列化失败。
* **测试框架**：某些测试运行器和 mock 库会检查函数元数据来生成报告。

对于你编写的任何装饰器，使用 `@functools.wraps` 都被视为**强制性的最佳实践**。
