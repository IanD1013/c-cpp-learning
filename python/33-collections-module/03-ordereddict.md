# OrderedDict 与 LRU Cache

自 Python 3.7 起，常规的 `dict` 已经会保留插入顺序。那么为什么 `collections` 模块中的 `OrderedDict` 依然存在？因为它可以提供普通字典所不具备的能力：用于重新调整键位置的 `move_to_end()` 方法、对顺序敏感的相等性比较，以及带有 `last` 参数的 `popitem()`。这些特性使 `OrderedDict` 成为实现缓存等顺序依赖型数据结构的理想基础组件。

## OrderedDict 与 dict 的区别

两个关键区别让 `OrderedDict` 独具特色：

1. **关注顺序的相等性判断**：包含相同键值对但顺序不同的两个 `OrderedDict` 实例是*不*相等的。而常规字典会认为它们相等。
2. **重新调整键的位置**：`move_to_end(key, last=True)` 可以将一个已有键移动到末尾（如果 `last=False` 则移动到开头），而无需删除并重新插入它。

```python
from collections import OrderedDict

# Order matters for equality
od1 = OrderedDict([('x', 1), ('y', 2)])
od2 = OrderedDict([('y', 2), ('x', 1)])
print(od1 == od2)  # False — order differs

# Regular dicts ignore order
d1 = {'x': 1, 'y': 2}
d2 = {'y': 2, 'x': 1}
print(d1 == d2)  # True
```

## 语法

```python
from collections import OrderedDict

od = OrderedDict()

# Insert items
od['alpha'] = 10
od['beta'] = 20
od['gamma'] = 30

# Move a key to the end (rightmost position)
od.move_to_end('alpha')          # now: beta, gamma, alpha

# Move a key to the beginning (leftmost position)
od.move_to_end('gamma', last=False)  # now: gamma, beta, alpha

# Remove and return the FIRST item (oldest)
od.popitem(last=False)  # returns ('gamma', ...)

# Remove and return the LAST item (newest)
od.popitem(last=True)   # returns ('alpha', ...)
```

## LRU 缓存模式

LRU（Least Recently Used，最近最少使用）缓存会保存固定数量的数据项。当缓存已满且有新数据项进入时，*最近最少使用*的数据项会被淘汰驱逐。核心逻辑如下：

- **访问**一个数据项会使其变为“最近使用”——将其移动到末尾。
- **插入**一个新数据项会将其添加到末尾。如果缓存超出容量，则从前端移除（最旧/最近最少使用的数据项）。

假设有一个容量为 3 的浏览器缓存，保存着页面 `[P1, P2, P3]`（最左侧 = 最旧）。如果你再次访问 `P1`，它会移动到末尾：`[P2, P3, P1]`。如果随后访问一个新页面 `P4`，由于缓存已满，`P2`（最左侧的一项）会被淘汰：`[P3, P1, P4]`。

## 示例

```python
from collections import OrderedDict

# Simple eviction demo with numbers
ocache = OrderedDict()
cache[100] = 'item-a'
cache[200] = 'item-b'
cache[300] = 'item-c'
# Cache: 100 -> 200 -> 300

# Access key 100, moving it to end
cache.move_to_end(100)
# Cache: 200 -> 300 -> 100

# Evict oldest (200)
cache.popitem(last=False)  # removes (200, 'item-b')
# Cache: 300 -> 100
```

## 常见陷阱

| **场景** | **注意事项** |
| :--- | :--- |
| 更新已有键 | 该键的位置也应该被刷新（移动到末尾） |
| 获取不存在的键 | 不应抛出错误——而是返回一个哨兵值 |
| 容量为 1 | 每次新的 `put` 都会淘汰前一个数据项 |
| 连续多次 get | 每次 get 操作都会重新调整所访问键的位置 |