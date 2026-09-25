### 使用 `__iter__` 使自定义集合可迭代

你已经了解了迭代器协议（iterator protocol）—— `__iter__`、`__next__` 和 `StopIteration`。现在是时候应用这些知识，让你自己的集合类与 `for` 循环、`list()`、解包（unpacking）以及其他期望可迭代对象的 Python 结构无缝协作了。

关键的见解是**关注点分离**（separation of concerns）：*可迭代对象*（iterable，即集合）和*迭代器*（iterator，即跟踪位置的对象）是不同的角色。你的集合类只需要实现 `__iter__()`，它会返回一个负责处理实际迭代的对象。

### 工作原理

当 Python 遇到 `for item in obj:` 时，它会调用 `obj.__iter__()` 来获取一个迭代器。然后反复调用该迭代器的 `__next__()` 方法，直到引发 `StopIteration`。

你的集合类根本不需要 `__next__()` —— 它只需要通过 `__iter__()` 返回一个有效的迭代器。这可以是：

1. **一个单独的迭代器类**，拥有自己的 `__iter__` 和 `__next__` 方法
2. **一个生成器**（使用 `yield`），它会自动创建一个迭代器对象

这种分离非常强大，因为每次对 `__iter__()` 的调用都会创建一个拥有自身状态的**全新**迭代器。这意味着你可以独立地多次迭代同一个集合。

### 语法

```python
class MyCollection:
    def __init__(self, data):
        self.data = data

    def __iter__(self):
        # Option A: return a separate iterator object
        return MyCollectionIterator(self.data)

        # Option B: use a generator (yield)
        # for item in self.data:
        #     yield item
```

### 示例

```python
# Example 1: A collection of even numbers using a generator in __iter__
class EvenNumbers:
    def __init__(self, limit):
        self.limit = limit

    def __iter__(self):
        n = 0
        while n < self.limit:
            yield n
            n += 2

evens = EvenNumbers(10)
print(list(evens))  # [0, 2, 4, 6, 8]
print(list(evens))  # [0, 2, 4, 6, 8]  — works again because __iter__ creates a new generator


# Example 2: A Countdown collection using a separate iterator class
class CountdownIterator:
    def __init__(self, start):
        self.current = start

    def __iter__(self):
        return self

    def __next__(self):
        if self.current <= 0:
            raise StopIteration
        val = self.current
        self.current -= 1
        return val

class Countdown:
    def __init__(self, start):
        self.start = start

    def __iter__(self):
        return CountdownIterator(self.start)

for num in Countdown(3):
    print(num)  # 3, 2, 1


# Example 3: Multiple independent iterations
letters = EvenNumbers(6)
for a in letters:
    for b in letters:  # Fresh iterator each time!
        print(a, b)
# Prints all combinations: (0,0), (0,2), (0,4), (2,0), (2,2), (2,4), (4,0), (4,2), (4,4)
```

### 常见模式

* **基于生成器的 `__iter__`**：最简单的方法。在 `__iter__()` 内部使用 `yield` 逐个生成值。Python 会为你处理所有的迭代器底层机制。
* **单独的迭代器类**：当迭代逻辑较复杂或需要单独公开迭代器时非常有用。
* **委托给内部数据**：如果你的集合包装了一个 list 或 dict，`__iter__` 可以直接 `yield from self._internal_list` 或返回 `iter(self._internal_list)`。
