### `any()` 和 `all()` —— 高效的布尔聚合

Python 的内置函数 `any()` 和 `all()` 可以让你解答关于集合的两个基本问题：*“是否存在至少一个真值（truthy）元素？”* 以及 *“是否所有元素都是真值？”* 它们是生产代码中进行验证、搜索和保护子句（guard clauses）的主力工具——当与生成器表达式搭配使用时，它们会变得极其高效。

### 它们的工作原理

`any(iterable)` 在找到**第一个真值**元素时便立即返回 `True`。如果遍历完整个可迭代对象都没有找到，则返回 `False`。

`all(iterable)` 仅在**每个**元素都为真值时才返回 `True`。一旦遇到第一个假值（falsy）元素，它就会立即返回 `False`。

两者都具有**短路求值（short-circuit）**特性：一旦能够确定结果，它们就会立即停止迭代。当可迭代对象非常庞大或计算成本高昂时，这一点尤为重要。

### 语法

```python
any(iterable)   # True if at least one element is truthy
all(iterable)   # True if every element is truthy

# With generator expressions (no intermediate list created!):
any(condition(x) for x in sequence)
all(condition(x) for x in sequence)
```

### 示例

```python
# Basic usage with lists
any([0, 0, 0, 1])   # True — the 1 is truthy
all([1, 2, 3, 4])    # True — all are truthy
all([1, 0, 3, 4])    # False — 0 is falsy

# Edge cases: empty iterables
any([])  # False — no elements, so nothing is truthy
all([])  # True  — "vacuous truth": there's no element to violate the condition

# Generator expressions for efficiency
temperatures = [72, 68, 85, 91, 77]
any(t > 90 for t in temperatures)  # True — 91 > 90, stops immediately
all(t > 60 for t in temperatures)  # True — all exceed 60

# Validation: check all entries are strings
records = ["Alice", "Bob", "Charlie"]
all(isinstance(r, str) for r in records)  # True

# Searching: does any filename end with .py?
files = ["README.md", "setup.py", "main.rs"]
any(f.endswith(".py") for f in files)  # True — stops at "setup.py"

# "None of" pattern — use all() with negated condition
scores = [85, 92, 78, 95]
all(s != 0 for s in scores)  # True — none are zero
# Equivalently: not any(s == 0 for s in scores)
```

### 短路求值行为

因为生成器表达式是**惰性的（lazy）**，如果第一个元素就匹配，带有生成器的 `any()` 不会去计算全部一百万个元素：

```python
# This completes nearly instantly because any() stops at the first True
any(True for _ in range(10_000_000))  # True — evaluated only the first element

# Compare: all() with a generator stops at the first False
all(False for _ in range(10_000_000))  # False — evaluated only the first element
```

这使得配合生成器使用的 `any()`/`all()` 比先构建一个完整的列表要高效得多。

### 组合结果

你可以将 `any()` 和 `all()` 应用于其他 `any()`/`all()` 调用的**结果**上：

```python
checks = [True, True, False, True]
all(checks)              # False — not every check passed
any(not c for c in checks)  # True — at least one check failed
```

### 常用参考表

| **模式**                       | **含义**       | **示例**                         |
| :--------------------------- | :----------- | :----------------------------- |
| `any(cond(x) for x in seq)`  | 至少有一个匹配      | `any(x > 0 for x in nums)`     |
| `all(cond(x) for x in seq)`  | 每个元素都匹配      | `all(x > 0 for x in nums)`     |
| `all(x != val for x in seq)` | 没有元素等于 `val` | `all(x != 0 for x in nums)`    |
| `not any(cond(x) ...)`       | 没有元素满足条件     | `not any(x < 0 for x in nums)` |
| `any([])` / `all([])`        | 空集合边界情况      | `False` / `True`               |