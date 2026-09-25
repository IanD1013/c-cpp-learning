### 迭代器协议

每次在 Python 中编写 `for` 循环时，你都在依赖一个特定的协议，该协议控制对象如何一次生成一个值序列。理解这个协议能让你使**任何**对象变得可迭代——不仅是列表和元组，还包括自定义数据结构、惰性序列以及无限流。

迭代器协议由两个双下划线方法（dunder methods）组成：

* **`__iter__()`** — 调用以获取迭代器对象。对于迭代器本身，此方法返回 `self`。
* **`__next__()`** — 调用以检索下一个值。当没有更多值时，它必须抛出 `StopIteration`。

### 工作原理

当 Python 遇到 `for item in obj:` 时，它在后台执行以下操作：

1. 调用 `iter(obj)`，这会触发 `obj.__iter__()` 以获取一个迭代器。
2. 重复调用 `next(iterator)`，这会触发 `iterator.__next__()`，并将每个返回的值赋给 `item`。
3. 当 `__next__()` 抛出 `StopIteration` 时，循环会干净地终止——不会崩溃，也没有回溯信息。

这与列表推导式、`tuple()`、`list()`、使用 `*` 的解包以及用于成员资格测试的 `in` 关键字所依赖的机制**完全相同**。

### 语法

```python
class MyIterator:
    def __init__(self, ...):
        # Initialize any state needed to track iteration
        pass

    def __iter__(self):
        # An iterator returns itself
        return self

    def __next__(self):
        # Return the next value, or raise StopIteration when done
        if some_condition:
            raise StopIteration
        # compute and return the next value
```

### 示例

**示例 1：迭代不超过上限的偶数**

```python
class EvenNumbers:
    def __init__(self, limit):
        self.limit = limit
        self.current = 0

    def __iter__(self):
        return self

    def __next__(self):
        self.current += 2
        if self.current > self.limit:
            raise StopIteration
        return self.current

for num in EvenNumbers(10):
    print(num)  # Prints 2, 4, 6, 8, 10
```

请注意，`__iter__` 返回 `self`，因为该对象**本身就是**迭代器。`current` 属性用于跟踪我们在序列中的位置，而 `__next__` 每次都会推进它。

**示例 2：将一个值重复固定次数**

```python
class Repeat:
    def __init__(self, value, times):
        self.value = value
        self.remaining = times

    def __iter__(self):
        return self

    def __next__(self):
        if self.remaining <= 0:
            raise StopIteration
        self.remaining -= 1
        return self.value

list(Repeat("hello", 3))  # ['hello', 'hello', 'hello']
```

这里的状态只是一个递减的计数器。当它归零时，迭代停止。

**示例 3：将迭代器与 `list()` 结合使用**

```python
# list() calls __iter__() then __next__() repeatedly
result = list(Repeat(42, 4))  # [42, 42, 42, 42]
```

`list()` 构造函数消耗迭代器的方式与 `for` 循环完全相同——底层都是同一个协议。

### 关键要点

* **可迭代对象（iterable）**是任何具有 `__iter__()` 的对象。**迭代器（iterator）**则是同时具有 `__next__()` 的可迭代对象。
* 一旦迭代器抛出 `StopIteration`，它就**耗尽**了。再次迭代将不会产生任何内容。
* 边界条件——即决定**何时**抛出 `StopIteration`——至关重要。这里的差一错误（off-by-one error）意味着你会生成过多或过少的值。
* 状态管理发生在 `__init__` 和 `__next__` 中。可以将 `__init__` 视为设置起始位置，将 `__next__` 视为向前迈出一步。
