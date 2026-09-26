# 上下文管理器总结项目：资源生命周期追踪

上下文管理器（Context managers）是 Python 中用于管理资源最优雅的模式之一。无论你是在处理数据库连接、网络套接字、文件句柄还是锁，`with` 语句都能确保正确获取和释放资源——即使发生异常也不例外。本总结项目将你所学到的关于上下文管理器的所有知识整合到一个连贯的练习中。

## 基于类的上下文管理器如何工作

基于类的上下文管理器实现了两个双下划线方法（dunder methods）：

- `__enter__(self)`：在执行进入 `with` 块时调用。它执行设置操作（获取资源）并返回绑定到 `as` 变量的对象。
- `__exit__(self, exc_type, exc_val, exc_tb)`：在执行离开 `with` 块时调用——无论是正常离开还是由于异常离开。它执行拆卸操作（释放资源）。返回 `False`（或 `None`）会让异常继续向外传播；返回 `True` 则会捕获并抑制异常。

该协议的妙处在于 `__exit__` **始终**会被调用，从而保证了清理工作一定能完成。

## 语法

```python
class MyManager:
    def __init__(self, config):
        self.config = config

    def __enter__(self):
        # Setup logic here
        return self  # or return some other resource

    def __exit__(self, exc_type, exc_val, exc_tb):
        # Teardown logic here
        return False  # Don't suppress exceptions

    def do_work(self, item):
        # Operational method available inside the with block
        pass
```

## 示例

```python
# Example 1: A timer context manager that logs duration
class Timer:
    def __init__(self, label):
        self.label = label
        self.events = []

    def __enter__(self):
        self.events.append(f"{self.label}: started")
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.events.append(f"{self.label}: stopped")
        return False

    def checkpoint(self, name):
        self.events.append(f"{self.label}: checkpoint {name}")


events_log = []
t = Timer("download")
t.events = events_log

with t as timer:
    timer.checkpoint("50%")
    timer.checkpoint("100%")

# events_log -> ["download: started", "download: checkpoint 50%",
#                "download: checkpoint 100%", "download: stopped"]


# Example 2: Sharing state through a mutable container
class Auditor:
    def __init__(self, audit_trail):
        self.trail = audit_trail

    def __enter__(self):
        self.trail.append("session_open")
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.trail.append("session_close")
        return False

    def record(self, action):
        self.trail.append(f"action: {action}")


trail = []

with Auditor(trail) as a:
    a.record("login")
    a.record("query")

# trail -> ["session_open", "action: login", "action: query", "session_close"]
```

## 需要记住的关键模式

- `__init__` 方法用于存储任何需要在 `with` 块结束后继续保留的共享状态（例如列表）。
- `__enter__` 返回 `self`，以便 `as` 变量可以访问管理器的方法。
- `__exit__` 接收异常信息，但除非你有意想要抑制错误，否则应该返回 `False`。
- 在 `with` 块内部遍历集合并为每个项目调用方法是一种常见的资源使用模式。