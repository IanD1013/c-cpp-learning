### 装饰器：包装函数以扩展功能

**装饰器（decorator）**是一个将另一个函数作为参数并返回一个*新*函数的函数，通常用于扩展或修改原函数的行为。因为你已经知道函数在 Python 中是一等公民（first-class objects）——它们可以作为参数传递、从其他函数返回，并赋值给变量——所以装饰器是自然的下一步。它们在真实的 Python 世界中被广泛使用：日志记录、访问控制、缓存、输入验证等。

### 工作原理

核心模式非常直接：

1. 你编写一个接收函数参数的**外部函数**（即装饰器）。
2. 在内部，你定义一个**内部函数**（即包装器 wrapper），在调用原函数之前和/或之后添加行为。
3. 外部函数**返回包装器**，从而实际上用增强版本替换了原函数。

当有人调用被装饰的版本时，他们实际上调用的是包装器，包装器负责编排原函数调用前后的额外行为。

### 语法

```python
# General decorator pattern
def my_decorator(func):
    def wrapper(*args, **kwargs):
        # Do something before
        result = func(*args, **kwargs)
        # Do something after
        return result
    return wrapper

# Manual application
decorated = my_decorator(some_function)
decorated()  # Calls wrapper, which calls some_function internally
```

包装器签名中的 `*args, **kwargs` 确保了它可以接受原函数所需的任何参数。

### 示例

```python
# Example 1: A decorator that prints a border around output
def add_border(func):
    def wrapper(*args, **kwargs):
        print("========")
        func(*args, **kwargs)
        print("========")
    return wrapper

def say_hello():
    print("Hello!")

bordered_hello = add_border(say_hello)
bordered_hello()
# Output:
# ========
# Hello!
# ========
```

```python
# Example 2: A decorator that counts how many times a function is called
def count_calls(func):
    def wrapper(*args, **kwargs):
        wrapper.call_count += 1
        return func(*args, **kwargs)
    wrapper.call_count = 0
    return wrapper

def multiply(a, b):
    return a * b

tracked_multiply = count_calls(multiply)
tracked_multiply(3, 4)  # Returns 12
tracked_multiply(5, 6)  # Returns 30
print(tracked_multiply.call_count)  # 2
```

```python
# Example 3: A decorator that collects return values into a list
def collect_results(func):
    def wrapper(*args, **kwargs):
        collected = []
        value = func(*args, **kwargs)
        collected.append(value)
        return collected
    return wrapper

def get_square(n):
    return n * n

collecting_square = collect_results(get_square)
print(collecting_square(7))  # [49]
```

### 核心概念

* **闭包（Closures）**：包装器函数“闭包引用”来自外部装饰器函数作用域的变量。这就是包装器在装饰器返回之后仍能访问原 `func` 的原因。
* **手动应用**：`decorated = my_decorator(original)` 是应用装饰器的显式方式。Python 的 `@my_decorator` 语法是这种方式的简写。
* **包装器替换原函数**：装饰完成后，调用 `decorated()` 会调用包装器，而不是直接调用原函数。
