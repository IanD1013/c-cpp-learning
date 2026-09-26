# 自定义上下文管理器类

上下文管理器（Context Manager）是 Python 中用于管理资源和状态最优雅的模式之一。虽然你可能已经对文件操作使用过 `with open(...)`，但相同的协议允许你封装**任何**设置/清理（setup/teardown）逻辑——数据库连接、锁、临时配置更改或性能测量。编写自己的上下文管理器类让你可以完全控制在代码块执行前后发生的事情，包括在发生异常时进行妥善的清理。

## 工作原理

上下文管理器类实现了两个特殊方法：

- **`__enter__(self)`**：在执行进入 `with` 代码块时被调用。它执行所有准备工作（获取资源、记录状态等）并**返回**绑定到 `as` 变量的对象。通常会返回 `self`，但也可以返回任何内容。
- **`__exit__(self, exc_type, exc_val, exc_tb)`**：在执行离开 `with` 代码块时被调用——无论是正常离开还是由于异常离开。这三个参数用于描述发生的任何异常（如果没有异常则为 `None`）。此方法负责执行清理工作。返回 `False`（或 `None`）会正常传播异常；返回 `True` 则会抑制异常。

关键保证：即使在 `with` 块内部引发异常，`__exit__` 也**始终**会运行。这使得上下文管理器在资源管理方面比手动的 try/finally 更可靠。

## 语法

```python
class MyContextManager:
    def __enter__(self):
        # Setup: acquire resource, record state, etc.
        return self  # or return some other managed object

    def __exit__(self, exc_type, exc_val, exc_tb):
        # Teardown: release resource, restore state, etc.
        return False  # Don't suppress exceptions


# Usage:
with MyContextManager() as manager:
    # Code that uses the managed resource
    pass

# __exit__ has been called by this point
```

## 示例

### 示例 1：跟踪缩进级别

```python
class IndentTracker:
    def __init__(self):
        self.level = 0

    def __enter__(self):
        self.level += 1
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.level -= 1
        return False


tracker = IndentTracker()

with tracker:
    print(tracker.level)  # 1

    with tracker:
        print(tracker.level)  # 2

print(tracker.level)  # 0
```

### 示例 2：临时更改目录

```python
import os


class ChangeDir:
    def __init__(self, new_path):
        self.new_path = new_path
        self.saved_path = None

    def __enter__(self):
        self.saved_path = os.getcwd()
        os.chdir(self.new_path)
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        os.chdir(self.saved_path)
        return False
```

### 示例 3：在管理器上存储计算结果

```python
import time

class Benchmark:
    def __init__(self):
        self.duration = None

    def __enter__(self):
        self._begin = time.monotonic()
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.duration = time.monotonic() - self._begin
        return False


with Benchmark() as b:
    sorted(range(5000, 0, -1))

print(f"Took {b.duration:.4f} seconds")
```

请注意在示例 3 中，`duration` 属性是在 `__exit__` 内部设置的，但可以在 `with` 代码块结束**之后**访问，因为变量 `b` 仍然引用同一个对象。

## 常见模式

| 模式 | `__enter__` 执行的操作 | `__exit__` 执行的操作 |
| --- | --- | --- |
| 资源管理 | 获取（打开、连接、加锁） | 释放（关闭、断开连接、解锁） |
| 状态保留 | 保存当前状态 | 恢复原始状态 |
| 测量 | 记录起始指标 | 计算并存储最终指标 |
| 日志/跟踪 | 记录“进入”日志 | 记录带有结果的“退出”日志 |

## 实用参考

| 函数 | 描述 |
| --- | --- |
| `time.time()` | 以浮点数形式返回当前时间（自纪元以来的秒数） |
| `time.monotonic()` | 返回单调时钟值（更适合测量时间间隔） |
| `time.perf_counter()` | 高分辨率性能计数器 |