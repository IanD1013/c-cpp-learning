### 使用 `functools.lru_cache` 实现记忆化（Memoization）

在上一节课程中，你学习了用于预填充函数参数的 `functools.partial`。现在我们来看看 `functools` 中的另一个强大工具：用于**记忆化**（**memoization**）的 `@lru_cache` 装饰器。记忆化会存储开销较大的函数调用结果，这样当再次传入相同的输入时，就会立即返回缓存的结果。在编写需要多次重复访问相同子问题的递归算法时，这项技术至关重要。

### 工作原理

当你使用 `@lru_cache` 装饰一个函数时，Python 会为其包裹一层透明的缓存层。每次调用该函数时，装饰器都会检查之前是否已经处理过这些完全相同的参数。如果是，则直接返回存储的结果（**缓存命中**，**cache hit**）。如果否，则调用原始函数，存储计算结果并返回（**缓存未命中**，**cache miss**）。

**LRU** 代表**最近最少使用**（**Least Recently Used**）。缓存具有最大容量（默认为 128 个条目）。当缓存已满且需要添加新条目时，最长时间未被访问的条目将被逐出。

### 语法

```python
from functools import lru_cache

# 使用默认的 maxsize 128 (Python 3.8+)
@lru_cache
def my_function(x):
    ...

# 显式指定 maxsize
@lru_cache(maxsize=256)
def my_function(x):
    ...

# 无限制缓存（不逐出条目）
@lru_cache(maxsize=None)
def my_function(x):
    ...
```

### 检查缓存

每个被 `@lru_cache` 装饰的函数都会获得两个特殊方法：

| **方法** | **描述** |
| :--- | :--- |
| `func.cache_info()` | 返回一个 named tuple：`CacheInfo(hits, misses, maxsize, currsize)` |
| `func.cache_clear()` | 清空缓存并重置统计信息 |

- **hits** — 缓存结果被复用的次数
- **misses** — 函数必须重新计算新结果的次数
- **maxsize** — 缓存的最大容量
- **currsize** — 当前存储的条目数量

### 示例

以计算阶乘的函数为例：

```python
from functools import lru_cache

@lru_cache
def factorial(n):
    if n <= 1:
        return 1
    return n * factorial(n - 1)

factorial(5)   # 计算 5! = 120（全部未命中：5, 4, 3, 2, 1）
factorial(3)   # 立即返回缓存的 6（命中！）

info = factorial.cache_info()
# CacheInfo(hits=1, misses=5, maxsize=128, currsize=5)
```

注意，在调用 `factorial(5)` 之后调用 `factorial(3)` 是一次缓存命中，因为 `factorial(3)` 已经作为 `factorial(5)` 的子问题被计算过了。

下面是另一个开销较大的字符串处理示例：

```python
@lru_cache(maxsize=64)
def count_vowels(text):
    return sum(1 for ch in text if ch.lower() in 'aeiou')

count_vowels("hello")  # 未命中 → 计算结果为 2
count_vowels("world")  # 未命中 → 计算结果为 1
count_vowels("hello")  # 命中 → 返回缓存的 2

print(count_vowels.cache_info())
# CacheInfo(hits=1, misses=2, maxsize=64, currsize=2)
```

### 经典的斐波那契问题

朴素递归斐波那契算法具有**指数级**时间复杂度，因为它会一遍又一遍地重复计算相同的值。对于 `fib(5)`，值 `fib(2)` 会被单独计算三次。使用 `@lru_cache` 时，每个唯一的输入仅计算一次，从而将复杂度降低到**线性** —— 从 `fib(0)` 到 `fib(n)` 的每个值都恰好计算一次（未命中），随后的每次访问都是命中。

对于 `fib(n)`，缓存将正好包含 `n + 1` 个条目（对应从 0 到 n 的每个值）。未命中的次数等于 `n + 1`，而命中的次数则反映了避免了多少次冗余的子问题计算。

### 内部函数与全新缓存

当你在另一个函数**内部**定义带缓存的函数时，每次调用外部函数都会创建一个全新的缓存。这在你希望每次调用都有独立的缓存统计信息时非常有用：

```python
def analyze_computation(x):
    @lru_cache
    def expensive(n):
        return n * n

    expensive(x)
    expensive(x)  # 这是一次命中
    return expensive.cache_info().hits  # 返回 1
```