# 上下文管理器协议

你已经使用过 `with open("file.txt") as f:` 来安全地处理文件。但 `with` 并不是什么魔法——它可以与实现了**上下文管理器协议**（context manager protocol）的**任何**对象配合使用。该协议由两个双下划线方法组成：`__enter__` 和 `__exit__`。理解这个协议可以让你为数据库连接、锁、计时器、临时状态更改等构建自己的资源管理器。

## 工作原理

当 Python 遇到 `with` 语句时，它会遵循一个精确的执行顺序：

1. 计算 `with` 后面的表达式以获取一个**上下文管理器**对象。
2. 调用该对象的 `__enter__()` 方法。它的返回值会被绑定到 `as` 后面的变量上。
3. 执行 `with` 代码块的主体。
4. 当代码块退出时——无论是正常退出、通过 `return` 退出，还是由于异常退出——都会调用 `__exit__(exc_type, exc_val, exc_tb)`。

如果发生了异常，`__exit__` 的三个参数会接收异常信息；如果代码块正常完成，则接收 `None, None, None`。如果 `__exit__` 返回真值（truthy value），该异常将被抑制；否则，异常会继续向外传播。

## 语法

```python
class MyContextManager:
    def __init__(self, ...):
        # Store any state needed for setup/teardown
        pass

    def __enter__(self):
        # Acquire the resource or perform setup
        # Return value is bound to the 'as' variable
        return self  # or some other object

    def __exit__(self, exc_type, exc_val, exc_tb):
        # Release the resource or perform teardown
        # Called regardless of how the block exits
        return False  # Don't suppress exceptions


# Usage:
with MyContextManager(...) as obj:
    # obj is whatever __enter__ returned
    # do work here

# __exit__ has been called by this point
```

## 示例

```python
# Example 1: A timer context manager
import time


class Timer:
    def __init__(self):
        self.elapsed = 0

    def __enter__(self):
        self.start = time.time()
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.elapsed = time.time() - self.start
        return False


with Timer() as t:
    sum(range(1000000))

print(f"Took {t.elapsed:.4f} seconds")
```

```python
# Example 2: A temporary directory changer
import os


class ChangeDir:
    def __init__(self, new_path):
        self.new_path = new_path
        self.old_path = None

    def __enter__(self):
        self.old_path = os.getcwd()
        os.chdir(self.new_path)
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        os.chdir(self.old_path)
        return False


# Automatically restores the original directory when done
with ChangeDir("/tmp"):
    print(os.getcwd())  # /tmp

# Back to original directory here
```

```python
# Example 3: __enter__ can return something other than self
class DatabaseConnection:
    def __init__(self, db_name):
        self.db_name = db_name

    def __enter__(self):
        self.conn = connect(self.db_name)  # hypothetical
        return self.conn  # return the connection, not self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.conn.close()
        return False
```

## 常见模式

- **`__enter__` 返回 `self`** —— 最常见的模式，允许 `as` 变量访问上下文管理器自身的方法和属性。
- **`__enter__` 返回不同的对象** —— 当被管理的资源与管理器本身分离时非常有用（例如返回一个连接对象）。
- **`__exit__` 返回 `False`**（或 `None`）—— 允许异常正常向外传播。只有当你特意想要抑制异常时才返回 `True`。
- **`__exit__` 始终会运行** —— 即使代码块内部发生异常也会执行，这使其成为保证清理工作的理想选择。