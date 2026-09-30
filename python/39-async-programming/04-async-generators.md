### Python 中的异步生成器

异步生成器（Async generators）结合了生成器（惰性、按需生成值）的强大功能与异步编程。它们允许你生成一个值序列，其中每个值在 `yield` 之前可能需要进行异步操作——例如从数据库获取数据、读取流或等待 I/O。

当你需要遍历异步到达的数据，而不需要一次性将所有内容加载到内存中时，这一点至关重要。

## 工作原理

常规生成器使用 `def` 和 `yield`。

**异步生成器**则同时使用：

- `async def`
- `yield`

关键区别在于：在每次 `yield` 之间，你可以 `await` 异步操作。

消费者使用 `async for`，而不是常规的 `for` 循环来进行迭代。

当你调用一个异步生成器函数时，它返回一个**异步生成器对象**——而不是协程（coroutine）。

该对象实现了：

- `__aiter__`
- `__anext__`

这两个方法构成了异步迭代协议，这也是 Python 知道它可以与 `async for` 配合使用的方式。

## 语法

```python
# Defining an async generator
async def my_async_gen():
    await some_async_operation()
    yield value


# Consuming an async generator
async def consumer():
    async for item in my_async_gen():
        process(item)
```

## 示例

```python
import asyncio


# Async generator that simulates fetching temperatures from sensors
async def read_sensors(sensor_ids):
    for sid in sensor_ids:
        await asyncio.sleep(0.1)  # simulate async I/O
        yield {"sensor": sid, "temp": 20 + sid}


# Consuming the async generator
async def monitor():
    readings = []

    async for reading in read_sensors([1, 2, 3]):
        readings.append(reading)

    return readings


# Result:
# [
#     {"sensor": 1, "temp": 21},
#     {"sensor": 2, "temp": 22},
#     ...
# ]
```

### Async generator with filtering

```python
import asyncio


async def even_numbers(limit):
    for i in range(limit):
        await asyncio.sleep(0)  # yield control to event loop

        if i % 2 == 0:
            yield i


async def get_evens():
    result = []

    async for num in even_numbers(10):
        result.append(num)

    return result


# Result:
# [0, 2, 4, 6, 8]
```

## 识别异步生成器

异步生成器对象**不是**协程。

你可以通过以下方式区分它们：

```python
import asyncio


async def my_coroutine():
    return 42


async def my_async_gen():
    yield 42


coro = my_coroutine()  # This IS a coroutine
gen = my_async_gen()    # This is NOT a coroutine


asyncio.iscoroutine(coro)
# True

asyncio.iscoroutine(gen)
# False

hasattr(gen, "__aiter__")
# True — it's async-iterable

hasattr(gen, "__anext__")
# True — it's an async iterator
```

## 同步运行异步代码

要从同步代码中调用异步函数，你需要一个事件循环（event loop）：

```python
import asyncio


async def fetch_data():
    await asyncio.sleep(0)
    return [1, 2, 3]


# From synchronous code:
loop = asyncio.new_event_loop()

try:
    result = loop.run_until_complete(fetch_data())
finally:
    loop.close()
```

## 进阶：异步生成器方法

与常规生成器类似，异步生成器支持：

- `asend(value)` — 向生成器发送一个值
- `athrow(exc_type)` — 向生成器抛出一个异常
- `aclose()` — 优雅地关闭生成器

这些方法都是可等待的（awaitable），并允许对生成器的执行进行细粒度控制。