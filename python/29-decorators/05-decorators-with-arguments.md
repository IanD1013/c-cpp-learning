### 装饰器工厂：接收参数的装饰器

标准的装饰器接收一个函数并返回它的修改版本。但是，如果你希望你的装饰器可配置（例如 `@retry(max_attempts=5)` 或 `@cache(ttl=60)`），该怎么办呢？为了实现这一点，你需要一个**装饰器工厂（decorator factory）**：一个接收配置参数并*返回*装饰器的函数。

这种模式在生产环境的 Python 代码中无处不在——像 Flask（`@app.route("/home")`）、Django 以及各类测试库等框架都依赖于它。

### 工作原理

当 Python 遇到 `@some_decorator(arg)` 时，它会**首先**求值 `some_decorator(arg)`，该调用必须返回真正的装饰器。然后，该装饰器会被应用到其下方的函数上。这意味着你最终会有三层嵌套：

1. **外层函数** —— 接收装饰器的配置参数
2. **中层函数**（装饰器） —— 接收要被装饰的函数
3. **内层函数**（wrapper） —— 接收被装饰函数的参数并实现修改后的行为

关键要点：`@some_decorator(arg)` 等价于 `func = some_decorator(arg)(func)`。

### 语法

```python
import functools

def my_decorator_factory(config_param):
    def decorator(func):
        @functools.wraps(func)
        def wrapper(*args, **kwargs):
            # Use config_param here to control behavior
            result = func(*args, **kwargs)
            return result
        return wrapper
    return decorator

@my_decorator_factory(some_value)
def my_function():
    pass
```

记得在 wrapper 上使用 `functools.wraps(func)`，以便被装饰的函数保留其原始名称、docstring 以及其他元数据。

### 示例

```python
import functools

# Example 1: A decorator factory that logs a custom tag before each call
def tag_logger(tag):
    def decorator(func):
        @functools.wraps(func)
        def wrapper(*args, **kwargs):
            print(f"[{tag}] Calling {func.__name__}")
            return func(*args, **kwargs)
        return wrapper
    return decorator

@tag_logger("DEBUG")
def compute_sum(a, b):
    return a + b

compute_sum(3, 4)  # Prints: [DEBUG] Calling compute_sum, returns 7


# Example 2: A decorator factory that enforces a return type
def enforce_type(expected_type):
    def decorator(func):
        @functools.wraps(func)
        def wrapper(*args, **kwargs):
            result = func(*args, **kwargs)
            if not isinstance(result, expected_type):
                raise TypeError(f"Expected {expected_type}, got {type(result)}")
            return result
        return wrapper
    return decorator

@enforce_type(int)
def get_age():
    return 30  # Works fine

@enforce_type(str)
def get_name():
    return 42  # Raises TypeError


# Example 3: A decorator factory that delays execution
import time

def delay(seconds):
    def decorator(func):
        @functools.wraps(func)
        def wrapper(*args, **kwargs):
            time.sleep(seconds)
            return func(*args, **kwargs)
        return wrapper
    return decorator

@delay(2)
def greet(name):
    return f"Hello, {name}!"
```

### 常见模式

装饰器工厂通常用于收集结果、重试操作、添加带 TTL 的缓存或基于角色控制访问权限。三层嵌套始终遵循相同的结构——只有 wrapper 的逻辑会发生变化。

一个有用的思维模型：外层函数“配置”装饰器，中层函数“接收”目标，内层函数“运行”修改后的行为。
