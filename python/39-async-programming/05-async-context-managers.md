# 异步上下文管理器（Async Context Managers）

在同步 Python 中，上下文管理器（`with` 语句）负责处理资源的生命周期 —— 在进入时获取资源，并在退出时释放资源。

但是，如果获取或释放资源本身就是异步的呢？

例如：

- 打开数据库连接池
- 建立 WebSocket 连接
- 获取分布式锁

这些操作都可能涉及可 `await`（await-able）的操作。

这就是**异步上下文管理器**（配合 `async with` 使用）的用武之地。

## 工作原理

异步上下文管理器是实现了两个特殊异步方法的对象：

- `__aenter__(self)` — 在进入 `async with` 代码块时被调用。它执行异步初始化设置并返回资源（通常为 `self`）。
- `__aexit__(self, exc_type, exc_val, exc_tb)` — 在离开 `async with` 代码块时被调用，即使发生异常也会执行。它执行异步清理工作。

这两个方法**必须**使用 `async def` 定义，因为运行时环境会 `await` 它们。

`__aexit__` 方法接收异常信息（如果没有异常，则全为 `None`）。

从 `__aexit__` 返回 `False`（或无返回值）会让异常继续向外传播；返回 `True` 则会捕获并抑制异常。

## 语法

```python
class MyAsyncManager:
    async def __aenter__(self):
        # Perform async setup
        await some_async_operation()
        return self  # or return a resource

    async def __aexit__(self, exc_type, exc_val, exc_tb):
        # Perform async cleanup
        await some_async_cleanup()
        return False  # don't suppress exceptions

# Usage inside an async function:
async def do_work():
    async with MyAsyncManager() as mgr:
        # mgr is whatever __aenter__ returned
        await mgr.do_something()

    # __aexit__ is guaranteed to run here
```

## 示例

### 示例 1：模拟异步文件处理器

```python
import asyncio

class AsyncFileHandler:
    def __init__(self, filename):
        self.filename = filename
        self.data = None

    async def __aenter__(self):
        await asyncio.sleep(0)  # simulate async open
        self.data = f"Contents of {self.filename}"
        return self

    async def __aexit__(self, exc_type, exc_val, exc_tb):
        await asyncio.sleep(0)  # simulate async close
        self.data = None
        return False

async def read_file():
    async with AsyncFileHandler("config.yaml") as fh:
        print(fh.data)  # "Contents of config.yaml"

    # fh.data is now None — cleanup happened
```

### 示例 2：跟踪状态转换

```python
import asyncio

events = []

class AsyncLock:
    def __init__(self, lock_name):
        self.lock_name = lock_name

    async def __aenter__(self):
        await asyncio.sleep(0)
        events.append(f"acquired: {self.lock_name}")
        return self

    async def __aexit__(self, exc_type, exc_val, exc_tb):
        await asyncio.sleep(0)
        events.append(f"released: {self.lock_name}")
        return False

async def critical_section():
    async with AsyncLock("db_write") as lock:
        events.append(f"working: {lock.lock_name}")

asyncio.run(critical_section())

# events: ["acquired: db_write", "working: db_write", "released: db_write"]
```

## 替代方案：`contextlib.asynccontextmanager`

正如用于同步代码的 `@contextlib.contextmanager` 一样，Python 提供了 `@contextlib.asynccontextmanager`，用于使用基于生成器的方法来编写异步上下文管理器：

```python
from contextlib import asynccontextmanager
import asyncio

@asynccontextmanager
async def managed_connection(host):
    await asyncio.sleep(0)  # async setup
    conn = {"host": host, "status": "open"}

    try:
        yield conn  # this is what "as" binds to
    finally:
        await asyncio.sleep(0)  # async cleanup
        conn["status"] = "closed"
```

本次练习主要关注**基于类的方法**，它能让你对生命周期拥有完全的控制权。

## 实用参考

| 概念 | 描述 |
| :--- | :--- |
| `async def __aenter__(self)` | 异步设置；返回值绑定到 `as` 后面的变量 |
| `async def __aexit__(self, ...)` | 异步清理拆卸；即使出现异常也会运行 |
| `async with X() as y:` | 进入和退出异步上下文管理器 |
| `asyncio.run(coro)` | 从同步代码中运行异步协程 |
| `hasattr(cls, attr)` | 检查类是否具有给定的属性/方法 |