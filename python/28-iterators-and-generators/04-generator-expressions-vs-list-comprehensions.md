# 生成器表达式与列表推导式

你已经知道如何编写列表推导式（list comprehension）和生成器函数（generator function）。生成器表达式（generator expression）结合了推导式的简洁性与生成器的惰性求值（lazy evaluation）特性。它们的外观与列表推导式几乎完全相同，只是使用圆括号替代了方括号 —— 而这一微小的语法差异对内存使用和性能有着深远的影响。

当你构建像 `[x * 2 for x in range(1_000_000)]` 这样的列表推导式时，Python 会一次性为所有 100 万个结果分配内存。而像 `(x * 2 for x in range(1_000_000))` 这样的生成器表达式则会按需生成每个值，在内存中一次只保留一个值。如果你只需要消费该序列一次 —— 例如计算聚合值 —— 就完全没有理由去实例化整个列表。

## 工作原理

生成器表达式会返回一个实现了迭代器协议（`__iter__` 和 `__next__`）的 **generator 对象**。每当消费者请求下一个值时，表达式才会进行必要的求值来产生该值，然后暂停。一旦耗尽，生成器就结束了 —— 在不重新创建新生成器的情况下，你无法重新启动它。

这种惰性是其关键优势：无论底层序列有多大，内存占用都保持恒定。而列表推导式的内存开销会随元素数量线性增长。

## 语法

```python
# 列表推导式 — 立即（eagerly）构建列表
result_list = [expression for item in iterable if condition]

# 生成器表达式 — 惰性（lazily）生成值
result_gen = (expression for item in iterable if condition)
```

当将生成器表达式作为函数的**唯一参数**传递时，可以省略外层的额外圆括号：

```python
# 这两者是等价的：
sum((x for x in range(10)))
sum(x for x in range(10))     # 更简洁 — 无需双层括号
```

## 示例

```python
# 示例 1：使用生成器表达式计算立方和
total = sum(n ** 3 for n in range(1, 6))
# 计算 1 + 8 + 27 + 64 + 125 = 225，无需构建列表

# 示例 2：检查是否存在长度大于 10 个字符的单词
words = ["hello", "encyclopedia", "cat"]
has_long = any(len(w) > 10 for w in words)   # True

# 示例 3：查找最大绝对值
values = [-7, 3, -15, 9]
max_abs = max(abs(v) for v in values)   # 15

# 示例 4：在生成器表达式中进行过滤
even_sum = sum(n for n in range(1, 101) if n % 2 == 0)
# 仅对 1 到 100 之间的偶数求和 → 2550
```

## 接收可迭代对象的内置函数

| **函数描述是否支持生成器？** |                       |   |
| ---------------- | --------------------- | - |
| `sum()`          | 返回所有元素的总和             | 是 |
| `min()`          | 返回最小的元素               | 是 |
| `max()`          | 返回最大的元素               | 是 |
| `any()`          | 如果任一元素为真值，则返回 `True`  | 是 |
| `all()`          | 如果所有元素均为真值，则返回 `True` | 是 |
| `sorted()`       | 从可迭代对象中返回排序后的列表       | 是 |

因为这些函数每次只消费可迭代对象的一个元素，将它们与生成器表达式搭配使用既节省内存又符合 Python 风格（Pythonic）。
