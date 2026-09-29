### 使用 `itertools.chain` 和 `itertools.islice` 组合与切片迭代器

当处理多个数据源（日志文件、数据库结果集、API 分页）时，你经常需要将它们视为单个连续流，而无需将所有内容具体化为一个庞大的列表。Python 的 `itertools` 模块为此提供了两个必备工具：用于惰性拼接可迭代对象的 `chain`，以及用于从任意迭代器中切片而无需将所有内容加载到内存中的 `islice`。

既然你已经熟悉了 `functools.partial` 和 `functools.lru_cache`，不妨将 `itertools` 视为惰性求值的对应工具：`functools` 转换的是*函数*，而 `itertools` 转换的是*迭代模式*。

### `chain` 的工作原理

`chain(*iterables)` 会从第一个可迭代对象中产出元素直到其耗尽，然后继续处理下一个，依此类推。它生成一个扁平的单一流，而不会创建中间列表。

此外还有 `chain.from_iterable(iterable_of_iterables)` —— 它不使用 `*` 解包参数，而是接收一个产出其他可迭代对象的单一可迭代对象。当你拥有一个动态生成的可迭代对象集合（例如列表的生成器）且无法使用 `*` 进行解包时，这至关重要。

### `islice` 的工作原理

`islice(iterable, stop)` 返回前 `stop` 个元素的迭代器。扩展形式 `islice(iterable, start, stop, step)` 与 Python 的 `[start:stop:step]` 切片语法类似，但它适用于*任何*迭代器 —— 包括不支持索引的生成器和链式迭代器。

与普通切片的一个关键区别在于：`islice` 在运行过程中会消耗迭代器。元素一旦被消耗就消失了。对同一个迭代器每次调用 `islice` 都会从上一次停止的地方继续。

### 语法

```python
from itertools import chain, islice

# chain: combine multiple iterables
combined = chain(iter1, iter2, iter3)

# chain.from_iterable: combine an iterable of iterables
combined = chain.from_iterable(list_of_lists)

# islice: slice any iterator
first_five = islice(some_iterator, 5)           # first 5 elements
sliced = islice(some_iterator, 2, 10)           # elements at index 2..9
every_third = islice(some_iterator, 0, None, 3) # every 3rd element, all the way through
```

### 示例

```python
from itertools import chain, islice

# Combining three ranges into one stream
stream = chain(range(3), range(10, 13), range(100, 102))
print(list(stream))  # [0, 1, 2, 10, 11, 12, 100, 101]

# Using from_iterable with a list of tuples
some_coords = [(1, 2), (3, 4), (5, 6)]
flat = list(chain.from_iterable(some_coords))
print(flat)  # [1, 2, 3, 4, 5, 6]

# Taking the first 4 elements from an infinite counter
from itertools import count
first_four = list(islice(count(100), 4))
print(first_four)  # [100, 101, 102, 103]

# Every other element from a sequence
letters = "abcdefgh"
result = list(islice(letters, 0, None, 2))
print(result)  # ['a', 'c', 'e', 'g']
```

### 常见模式

| **模式** | **代码** | **描述** |
| :--- | :--- | :--- |
| 扁平化嵌套列表 | `chain.from_iterable(nested)` | 无需列表推导式的一级扁平化 |
| 从大型数据源中获取前 N 个 | `islice(big_iter, n)` | 内存高效的 head 操作 |
| 跳过并获取 | `islice(iter, start, start+n)` | 跳过 `start` 个元素，获取接下来的 `n` 个元素 |
| 每隔 K 个进行采样 | `islice(iter, 0, None, k)` | 等间距采样 |

### 重要提示：迭代器会被消耗

请记住，`chain` 和 `islice` 返回的都是*迭代器*。一旦你遍历它们（例如通过调用 `list()`），它们就会被耗尽。如果你需要多次遍历链式序列，则每次都需要重新创建 chain，或者先将其转换为列表。

```python
# This WON'T work — the iterator is consumed after the first list()
chained = chain([1, 2], [3, 4])
first_pass = list(chained)   # [1, 2, 3, 4]
second_pass = list(chained)  # [] — empty!

# Recreate the chain for each use
list(chain([1, 2], [3, 4]))  # [1, 2, 3, 4] — fresh each time
```