### 异步休眠（Async Sleep）：让出控制权而不发生阻塞

`asyncio.sleep(seconds)` 是 `time.sleep()` 的异步对应版本。`time.sleep()` 会阻塞**整个线程**，冻结所有代码的执行，而 `asyncio.sleep()` 仅挂起**当前协程（coroutine）**，允许事件循环（event loop）在暂停期间运行其他协程。这一区别是 Python 异步并发的基石。

## 工作原理

当协程遇到 `await asyncio.sleep(delay)` 时，它会告知事件循环：

> “我现在先停一下，`delay` 秒后再回来找我。”

随后事件循环会切换到另一个已准备就绪的协程。即使是 `asyncio.sleep(0)` 也具有实际意义：它会瞬间让出控制权，在恢复执行前让其他协程有机会运行。

可以把它想象成在熟食店柜台前取号排队。

- `time.sleep()` 就像站在柜台前赖着不走。
- `asyncio.sleep()` 则是坐下来等待叫号，在此期间让其他人先得到服务。

## 语法

```python
import asyncio

async def my_coroutine():
    await asyncio.sleep(2.5)    # Pause 2.5 seconds, yield to event loop
    await asyncio.sleep(0)      # Yield immediately, no actual delay
```

## 示例

```python
import asyncio
import time

# BLOCKING: time.sleep freezes everything
def blocking_example():
    print("Start")
    time.sleep(3)       # Nothing else can run for 3 seconds
    print("End")

# NON-BLOCKING: asyncio.sleep yields to event loop
async def non_blocking_example():
    print("Start")
    await asyncio.sleep(3)  # Other coroutines can run during this wait
    print("End")

# Collecting results with asyncio.sleep(0) to yield between iterations
async def generate_squares(n):
    squares = []
    for i in range(1, n + 1):
        squares.append(i * i)
        await asyncio.sleep(0)  # Yield control each iteration
    return squares

# Running from synchronous code
result = asyncio.run(generate_squares(4))
print(result)  # [1, 4, 9, 16]
```

## 常见模式

- **`asyncio.sleep(0)`** 可以在没有延迟的情况下让出控制权。在循环中非常有用，可防止单个协程独占事件循环。
- **`asyncio.run(coroutine)`** 连接同步代码与异步代码。它会创建一个事件循环，运行协程并返回结果。
- **在异步循环中构建列表**：在循环内部追加元素，并在每次迭代之间使用 `asyncio.sleep(0)` 让出控制权。

## 实用参考

| 函数 | 描述 | 返回值 |
|---|---|---|
| `asyncio.sleep(seconds)` | 将协程挂起 `seconds` 秒 | `None` |
| `asyncio.sleep(0)` | 立即让出控制权 | `None` |
| `asyncio.run(coro)` | 从同步代码中运行协程 | 协程的返回值 |