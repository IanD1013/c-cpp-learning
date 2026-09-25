### 装饰器模式综合实践

装饰器是 Python 中最强大的元编程工具之一。在实际的代码库中，你很少会孤立地只使用一种模式 —— 通常是将多个装饰器组合在一起，将校验、转换、日志等功能组合成清晰、可复用的层。本综合实践将把你学到的所有知识融合在一起：头等函数（first-class functions）、闭包、简单装饰器、`@` 语法、`functools.wraps` 以及带参数的装饰器工厂。

### 装饰器堆叠的工作原理

当你堆叠多个装饰器时，它们在定义时是**自底向上**（bottom-up）应用的，但在调用时是**自顶向下**（top-down）执行的。理解这一点至关重要：

```python
@decorator_a
@decorator_b
def my_func(x):
    return x
```

这等价于 `my_func = decorator_a(decorator_b(my_func))`。当你调用 `my_func(5)` 时，`decorator_a` 的 wrapper 首先运行；如果它继续调用下一层，则 `decorator_b` 的 wrapper 接着运行，最后原始的 `my_func` 执行。然后返回值会沿相反方向逐层向上传递回各个 wrapper。

### 装饰器工厂（参数化装饰器）

装饰器工厂是一个**返回**装饰器的函数。这层额外的嵌套允许你传入配置参数：

```python
import functools

def repeat(n):
    def decorator(func):
        @functools.wraps(func)
        def wrapper(*args, **kwargs):
            result = None
            for _ in range(n):
                result = func(*args, **kwargs)
            return result
        return wrapper
    return decorator

@repeat(3)
def greet(name):
    print(f"Hello, {name}!")

greet("Ada")  # 打印三次 "Hello, Ada!"
```

三层嵌套结构 —— factory → decorator → wrapper —— 是标准模式。`repeat(3)` 首先被调用并返回 `decorator`，随后它会包装 `greet`。

### 校验装饰器

一个常见的实际模式是在被包装的函数运行之前校验参数的装饰器：

```python
import functools

def require_non_empty(func):
    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        if not args[0]:
            raise ValueError("First argument must not be empty")
        return func(*args, **kwargs)
    return wrapper

@require_non_empty
def process_name(name):
    return name.upper()

process_name("ada")   # 返回 "ADA"
process_name("")      # 抛出 ValueError
```

请注意，wrapper 在委托调用 `func` 之前检查了 `args[0]`。如果校验失败，原始函数将根本不会执行。

### 为什么 `functools.wraps` 很重要

如果没有 `@functools.wraps(func)`，wrapper 会替换原始函数的 `__name__`、`__doc__` 和其他元数据。使用它可以保留自省能力（introspection）：

```python
import functools

def log_calls(func):
    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        print(f"Calling {func.__name__}")
        return func(*args, **kwargs)
    return wrapper

@log_calls
def add(a, b):
    """Adds two numbers."""
    return a + b

print(add.__name__)  # "add"（而不是 "wrapper"）
print(add.__doc__)   # "Adds two numbers."
```

### 组合装饰器：执行顺序

来看看堆叠一个校验器和一个转换器是如何工作的：

```python
@validate_input
@double_output
def calculate(x):
    return x * 3
```

当你调用 `calculate(4)` 时：

1. `validate_input` 的 wrapper 运行 —— 检查输入
2. 如果有效，它继续调用 `double_output` 的 wrapper
3. `double_output` 的 wrapper 调用原始的 `calculate(4)` → 返回 12
4. `double_output` 的 wrapper 将其翻倍 → 返回 24
5. `validate_input` 的 wrapper 返回 24

**外层**装饰器控制是否继续执行。**内层**装饰器转换结果。顺序非常重要！
