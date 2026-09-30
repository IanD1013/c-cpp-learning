# 使用 `asyncio.gather` 并发运行协程

当你需要执行多个独立的异步操作时——例如请求多个 API 端点、读取多个文件或处理一批项目——依次运行它们会浪费时间。`asyncio.gather` 是在同一个事件循环内调度多个协程**并发**运行的主要工具。它会等待所有协程完成，并以列表形式返回它们的结果，同时保留原始的输入顺序。

## 工作原理

`asyncio.gather` 接收任意数量的可等待对象（协程、任务、Future），将它们全部调度到事件循环中，并挂起直到每一个都完成。

关键要点在于：这是**并发而非并行**。Python 的 asyncio 是单线程的——协程在 `await` 点轮流执行。

当一个协程遇到：

```python
await asyncio.sleep(...)
```

或：

```python
await some_io()
```

时，事件循环会切换到另一个已就绪的协程。这种交替执行为 I/O 密集型任务提供了无需线程即可达到类似并行的*效果*。

至关重要的是，无论哪个协程先完成，`gather` 都会**按照与输入协程相同的顺序**返回结果。

## 语法

```python
import asyncio

# Basic usage — unpack coroutines with *
results = await asyncio.gather(coro1(), coro2(), coro3())

# results is a list:
# [result_of_coro1, result_of_coro2, result_of_coro3]


# With return_exceptions=True — exceptions become result values
results = await asyncio.gather(
    coro1(),
    failing_coro(),
    return_exceptions=True
)

# results[1] will be the exception object, not raised
```

## 示例

```python
import asyncio


async def fetch_temperature(city):
    await asyncio.sleep(0.1)  # simulate network delay
    temps = {
        "Paris": 22,
        "Tokyo": 28,
        "Lima": 18,
    }
    return temps.get(city, 0)


async def main():
    # All three "requests" run concurrently, not sequentially
    results = await asyncio.gather(
        fetch_temperature("Paris"),
        fetch_temperature("Tokyo"),
        fetch_temperature("Lima"),
    )

    print(results)  # [22, 28, 18] — same order as input


asyncio.run(main())
```

```python
import asyncio

async def safe_divide(a, b):
    await asyncio.sleep(0)
    if b == 0:
        raise ValueError("division by zero")
    return a / b


async def main():
    # Without return_exceptions, the first exception propagates immediately
    # With return_exceptions=True, exceptions are captured as values
    results = await asyncio.gather(
        safe_divide(10, 2),
        safe_divide(9, 0),
        safe_divide(8, 4),
        return_exceptions=True,
    )

    print(results[0])                 # 5.0
    print(type(results[1]))           # <class 'ValueError'>
    print(isinstance(results[1], ValueError))  # True
    print(results[2])                 # 2.0


asyncio.run(main())
```

## 常见模式

### 动态构建协程列表

```python
async def process(x):
    await asyncio.sleep(0)
    return x ** 2


async def main():
    items = [1, 2, 3, 4, 5]

    coroutines = [
        process(item)
        for item in items
    ]

    results = await asyncio.gather(*coroutines)

    print(results)  # [1, 4, 9, 16, 25]
```

这里的 `*` 用于把列表中的协程展开。

例如：

```python
coroutines = [
    process(1),
    process(2),
    process(3),
]
```

那么：

```python
await asyncio.gather(*coroutines)
```

相当于：

```python
await asyncio.gather(
    process(1),
    process(2),
    process(3),
)
```

### 检查结果中的异常对象

```python
for r in results:
    if isinstance(r, Exception):
        print(f"Error occurred: {r}")
    else:
        print(f"Success: {r}")
```

## 实用参考

| Function / Parameter | Description |
| --- | --- |
| `asyncio.gather(*awaitables)` | 并发调度所有可等待对象，按顺序返回结果列表 |
| `return_exceptions=True` | 将异常作为结果值捕获，而不是引发异常 |
| `asyncio.run(coro())` | 从同步代码中运行协程（创建事件循环） |
| `asyncio.sleep(0)` | 短暂让出控制权给事件循环（常用于模拟异步操作） |
| `isinstance(obj, ExceptionType)` | 检查对象是否为特定异常类的实例 |