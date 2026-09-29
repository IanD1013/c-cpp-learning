### Python 中的函数式数据处理管道

Python 的 `functools` 和 `itertools` 模块提供了强大的工具，用于构建可组合且高效的数据处理管道。在实际应用中，你经常需要通过多个阶段来转换数据——应用函数、过滤、组合以及聚合结果——同时保持代码的整洁和高性能。本综合练习将 `functools.partial`、`functools.lru_cache` 以及多个 `itertools` 函数整合到一个连贯的管道中。

### 工作原理

函数式管道通过连续的阶段处理数据，每个阶段的输出作为下一阶段的输入。`partial` 允许你预填函数参数以创建专用版本的函数。`lru_cache` 记忆昂贵的计算结果，以便使用相同参数的重复调用可以立即返回。`itertools` 函数提供了惰性且内存高效的方式来组合、切片、配对和分组序列。

### 关键工具参考

| **工具** | **用途** | **示例** |
| :--- | :--- | :--- |
| `partial(func, arg)` | 固定函数的一个或多个参数 | `partial(multiply, 3)` 创建一个乘以 3 的函数 |
| `lru_cache(maxsize=None)` | 记忆函数结果；通过 `.cache_info()` 跟踪命中/未命中情况 | 修饰任何纯函数 |
| `combinations(iterable, r)` | 所有长度为 r 且不重复的子序列 | `combinations([1,2,3], 2)` → `(1,2), (1,3), (2,3)` |
| `groupby(iterable, key=None)` | 对连续相等的元素进行分组（需先排序！） | 按标识对排序后的值进行分组 |
| `chain(*iterables)` | 惰性连接多个可迭代对象 | `chain([1,2], [3,4])` → `1, 2, 3, 4` |
| `islice(iterable, stop)` | 从任意可迭代对象中提取前 N 个元素 | `islice(range(100), 5)` → `0, 1, 2, 3, 4` |

### 示例

```python
from functools import partial, lru_cache
from itertools import chain, islice, combinations, groupby

# partial: create a specialized function
def multiply(a, b):
    return a * b

double = partial(multiply, 2)
print(double(5))  # 10
print([double(x) for x in [3, 7, 11]])  # [6, 14, 22]

# lru_cache: memoize and inspect cache performance
@lru_cache(maxsize=None)
def fibonacci(n):
    if n < 2:
        return n
    return fibonacci(n - 1) + fibonacci(n - 2)

fibonacci(10)  # 55
info = fibonacci.cache_info()
print(info.hits, info.misses)  # Shows cache performance

# combinations: all 2-element pairs
colors = ['red', 'blue', 'green']
for pair in combinations(colors, 2):
    print(pair)  # ('red','blue'), ('red','green'), ('blue','green')

# groupby: count occurrences in sorted data
scores = [85, 85, 90, 90, 90, 95]
for key, group in groupby(scores):
    print(key, len(list(group)))  # 85 2, 90 3, 95 1

# chain + islice: combine and take first few
first_5 = list(islice(chain([10, 20], [30, 40], [50, 60, 70]), 5))
print(first_5)  # [10, 20, 30, 40, 50]
```

### 关于 `groupby` 的重要注意事项

`groupby` 仅对**连续**相等的元素进行分组。如果你希望将所有相等的元素分组在一起，必须先对可迭代对象进行排序。每个分组都是一个迭代器，当移动到下一个分组时该迭代器就会被消耗——如果你需要其长度或内容，请立即将其转换为列表。

### 关于 `lru_cache` 的重要注意事项

在缓存函数上调用 `.cache_info()` 会返回一个命名元组，包含 `hits`（找到缓存结果的调用次数）和 `misses`（必须计算结果的调用次数）。每次定义函数时缓存都是全新的，因此如果你在另一个函数内部定义它，则每次调用时缓存都会重置。