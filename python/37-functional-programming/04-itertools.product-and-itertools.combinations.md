### itertools 中的组合迭代器

除了串联和切片可迭代对象外，`itertools` 还提供了一套功能强大的**组合生成器**（combinatoric generators），它们可以生成数据中所有可能的排列或选择。当需要探索所有可能的配对、排序或子集时，这些生成器至关重要 —— 常见于搜索算法、测试场景、配置生成和数学计算中。

### 它们的工作原理

每个组合函数都接受一个可迭代对象，并生成表示不同选取或排列元素方式的元组：

- **笛卡尔积（Cartesian product）** 解决的问题是：“如果我从每个输入中各取一个元素，所有可能的配对是什么？” 可以将其视为扁平化为单一流的嵌套 `for` 循环。
- **组合（Combinations）** 解决的问题是：“在不考虑顺序的情况下，从集合中选择 r 个元素有多少种方式？” 就像从一组人中选出一个委员会 —— {Alice, Bob} 与 {Bob, Alice} 相同。
- **排列（Permutations）** 解决的问题是：“在考虑顺序的情况下，从集合中排列 r 个元素有多少种方式？” 就像分配名次 —— 第一名 Alice 和第二名 Bob 与反过来的情况不同。
- **含重复元素的组合（Combinations with replacement）** 解决的问题是：“如果允许重复选取同一个元素，选择 r 个元素有多少种方式？”

### 语法

```python
from itertools import product, combinations, combinations_with_replacement, permutations

# 一个或多个可迭代对象的笛卡尔积
product(iterable_A, iterable_B, ..., repeat=1)

# 所有长度为 r 的子序列（忽略顺序，无重复元素）
combinations(iterable, r)

# 所有长度为 r 的子序列（忽略顺序，允许重复元素）
combinations_with_replacement(iterable, r)

# 所有长度为 r 的排列（考虑顺序，无重复元素）
permutations(iterable, r)
```

### 示例

```python
from itertools import product, combinations, combinations_with_replacement, permutations

# 笛卡尔积：颜色与尺寸的每一种配对
colors = ['red', 'blue']
sizes = ['S', 'M']
list(product(colors, sizes))
# [('red', 'S'), ('red', 'M'), ('blue', 'S'), ('blue', 'M')]

# 带 repeat 的 product：所有 2 位二进制字符串
list(product([0, 1], repeat=2))
# [(0, 0), (0, 1), (1, 0), (1, 1)]

# 组合：从 3 种水果中选择 2 种（不考虑顺序）
list(combinations(['apple', 'banana', 'cherry'], 2))
# [('apple', 'banana'), ('apple', 'cherry'), ('banana', 'cherry')]

# 排列：从 3 种水果中排列 2 种（考虑顺序）
list(permutations(['apple', 'banana', 'cherry'], 2))
# [('apple', 'banana'), ('apple', 'cherry'), ('banana', 'banana'), ...]
# 总计：6 种排列

# 含重复元素的组合：从 ['x', 'y'] 中选择 2 个元素（允许重复）
list(combinations_with_replacement(['x', 'y'], 2))
# [('x', 'x'), ('x', 'y'), ('y', 'y')]
```

### 核心区别

| **函数** | **是否区分顺序？** | **是否允许重复？** | **数量计算公式** |
| :--- | :--- | :--- | :--- |
| `combinations(n, r)` | 否 | 否 | n! / (r!(n-r)!) |
| `permutations(n, r)` | 是 | 否 | n! / (n-r)! |
| `combinations_with_replacement(n, r)` | 否 | 是 | (n+r-1)! / (r!(n-1)!) |
| `product(n, repeat=r)` | 是 | 是 | n^r |