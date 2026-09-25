### `@decorator` 语法

你已经知道如何通过重新赋值函数来手动应用装饰器：

```python
def my_func():
    return "data"

my_func = my_decorator(my_func)
```

Python 提供了 `@` 语法作为**语法糖**——这是一种更简洁、更具可读性的方式来完成完全相同的事情。在函数定义正上方的一行放置 `@decorator_name` 与手动重新赋值是**完全等价的**。它使得装饰在函数定义的地方一目了然，而不是埋藏在下方的某处。

### 工作原理

当 Python 在 `def` 语句上方遇到 `@` 符号及随后的名称时，它会：

1. 像往常一样定义该函数
2. 将该函数对象传递给装饰器
3. 将函数名重新绑定到装饰器返回的内容

因此：

```python
@my_decorator
def say_hi():
    return "hi"
```

会被 Python 转换为：

```python
def say_hi():
    return "hi"

say_hi = my_decorator(say_hi)
```

`@` 语法只是将装饰移动到了函数的**上方**，从而让人立即清楚该函数正在被修改。

### 语法

```python
def my_decorator(func):
    def wrapper(*args, **kwargs):
        # Do something before/after calling the original
        result = func(*args, **kwargs)
        return modified_result
    return wrapper

@my_decorator
def target_function(params):
    return something
```

### 示例

**示例 1 — 添加日志记录：**

```python
def log_call(func):
    def wrapper(*args, **kwargs):
        print(f"Calling {func.__name__}")
        return func(*args, **kwargs)
    return wrapper

@log_call
def compute_area(radius):
    return 3.14159 * radius * radius

compute_area(5)  # Prints "Calling compute_area", returns 78.53975
```

**示例 2 — 转换返回值：**

```python
def make_uppercase(func):
    def wrapper(*args, **kwargs):
        result = func(*args, **kwargs)
        return result.upper()
    return wrapper

@make_uppercase
def get_city(name):
    return name

get_city("paris")  # Returns "PARIS"
```

**示例 3 — 堆叠多个装饰器（自底向上应用）：**

```python
def bold(func):
    def wrapper(*args, **kwargs):
        return "<b>" + func(*args, **kwargs) + "</b>"
    return wrapper

def italic(func):
    def wrapper(*args, **kwargs):
        return "<i>" + func(*args, **kwargs) + "</i>"
    return wrapper

@bold
@italic
def greet(name):
    return name

greet("Eve")  # Returns "<b><i>Eve</i></b>"
# italic is applied first (closest to the function), then bold wraps the result
```

### 堆叠顺序

当堆叠多个 `@` 装饰器时，它们是**自底向上**应用的——最靠近 `def` 的装饰器最先应用。然后将其结果传递给上方的下一个装饰器。可以将其理解为嵌套函数调用：

```python
@A
@B
@C
def f(): ...

# Equivalent to: f = A(B(C(f)))
```
