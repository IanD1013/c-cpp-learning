# Python 中的 Async 和 Await 基础

Python 的 `async` 和 `await` 关键字支持**协作式多任务处理**（cooperative multitasking）——这是一种并发风格，函数在特定点主动让出控制权，从而允许其他任务继续执行。这与多线程有根本的不同：没有抢占式切换，不需要锁，并且所有代码都在单个线程上运行。当你的代码花费大量时间在*等待*（如等待网络响应、文件 I/O、数据库查询）而不是计算时，异步编程的优势最为明显。

## 工作原理

使用 `async def` 定义的函数称为**协程函数**（coroutine function）。调用协程函数**不会**执行其函数体——相反，它会返回一个**协程对象**（coroutine object）。该对象必须在另一个协程中使用 `await` 进行等待，或者被调度到**事件循环**（event loop）中。

`await` 关键字会暂停当前协程，直到被等待的协程执行完成，然后携带结果恢复执行。至关重要的是，`await` **只能**出现在 `async def` 函数内部——在常规函数中使用它会引发 `SyntaxError`。

要真正从同步上下文中*运行*异步代码，可以使用 `asyncio.run(coro)`。该函数会创建一个事件循环，运行给定的协程直至完成，然后关闭该循环。它充当了同步世界与异步世界之间的桥梁。

## 语法

```python
import asyncio

# Define a coroutine function
async def my_coroutine(x):
    return x * 2

# Await one coroutine from another
async def caller():
    result = await my_coroutine(5)
    return result

# Run from synchronous code
value = asyncio.run(caller())  # returns 10
```

## 示例

```python
import asyncio

# Example 1: Basic coroutine that returns a value
async def square(n):
    return n ** 2

async def compute_squares(numbers):
    results = []
    for n in numbers:
        val = await square(n)  # await pauses here until square completes
        results.append(val)
    return results

print(asyncio.run(compute_squares([2, 3, 4])))  # [4, 9, 16]

# Example 2: Calling a coroutine function returns a coroutine object, not the result
coro = square(7)            # No execution happens yet!
print(type(coro).__name__)  # "coroutine"
coro.close()                # Always close unawaited coroutines to avoid warnings

# Example 3: Inspecting coroutine functions
print(asyncio.iscoroutinefunction(square))  # True
print(asyncio.iscoroutinefunction(print))   # False
```

## 核心概念

| 概念 | 描述 |
| :--- | :--- |
| `async def` | 声明一个协程函数 |
| `await expr` | 暂停协程直到 `expr` 完成，并返回其结果 |
| `asyncio.run(coro)` | 从同步代码运行协程（创建并关闭事件循环） |
| `asyncio.iscoroutinefunction(fn)` | 如果 `fn` 是 `async def` 函数则返回 `True` |
| `type(obj).__name__` | 获取类型名称字符串（用于检查协程对象非常有用） |
| `coro.close()` | 正确关闭未被 await 的协程对象以防止 `RuntimeWarning` |

## 重要注意事项

- 在不使用 `await` 的情况下调用 `async_func(args)` 会得到一个协程**对象**，而不是返回值。你必须 `await` 它或通过事件循环运行它。
- 未被 await 的协程对象应使用 `.close()` 关闭，以消除 `RuntimeWarning: coroutine was never awaited` 警告。
- 当事件循环已经在运行时（例如在另一个异步函数内部），不能调用 `asyncio.run()`。