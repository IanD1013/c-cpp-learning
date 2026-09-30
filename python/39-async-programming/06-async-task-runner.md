# 在 Python 中结合使用异步特性

Python 的 `asyncio` 模块提供了一套丰富的工具来编写并发代码：

- `async def` 协程
- `await` 表达式
- 用于并发执行的 `asyncio.gather`
- 异步生成器（`async for`）
- 异步上下文管理器（`async with`）

在实际应用中，你可以将这些特性结合起来构建流水线，以非阻塞的方式生成工作项、并发处理它们并管理资源生命周期。

这个综合练习将所有主要的异步特性整合到一个连贯的 task runner 中。

## 异步上下文管理器的工作原理

异步上下文管理器是一个将 `__aenter__` 和 `__aexit__` 实现为协程的类。它们允许你围绕异步操作封装初始化/清理逻辑。

```python
class DatabaseConnection:
    async def __aenter__(self):
        self.conn = await connect_to_db()
        return self.conn

    async def __aexit__(self, exc_type, exc_val, exc_tb):
        await self.conn.close()
        return False  # Don't suppress exceptions


async def fetch_users():
    async with DatabaseConnection() as conn:
        return await conn.query("SELECT * FROM users")
```

`__aenter__` 方法在进入 `async with` 块时运行，而 `__aexit__` 在离开该块时运行（即使发生异常也会执行）。

## 异步生成器的工作原理

异步生成器在 `async def` 中使用 `yield`。你可以使用 `async for` 来消费它：

```python
async def countdown(n):
    while n > 0:
        yield n
        n -= 1
        await asyncio.sleep(0.1)


async def main():
    async for number in countdown(5):
        print(number)  # 5, 4, 3, 2, 1
```

使用 `await asyncio.sleep(0)` 会将控制权交还给事件循环，从而允许其他任务运行。这对于协作式多任务处理至关重要。

## `asyncio.gather` 的工作原理

`asyncio.gather` 并发运行多个协程并按顺序收集它们的结果：

```python
async def fetch_price(item):
    await asyncio.sleep(0)  # Simulate async I/O
    prices = {"apple": 1.50, "bread": 3.00, "milk": 2.25}
    return (item, prices.get(item, 0))


async def get_all_prices():
    results = await asyncio.gather(
        fetch_price("apple"),
        fetch_price("bread"),
        fetch_price("milk")
    )
    return dict(results)  # {"apple": 1.50, "bread": 3.00, "milk": 2.25}
```

## 使用 `asyncio.run` 连接异步与同步

`asyncio.run()` 是从同步代码调用异步函数的标准方式。它会创建一个新的事件循环，运行协程直到完成，然后关闭事件循环：

```python
def synchronous_entry_point():
    result = asyncio.run(some_async_function())
    return result
```