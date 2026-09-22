### 列表推导式中的条件表达式

Python 的列表推导式本身就已经非常强大，但当将其与条件表达式（有时称为三元运算符）结合使用时，表达能力会更上一层楼。这让你可以根据条件**转换集合中的每个元素**——并且只需一行清晰易读的代码即可完成。

### 两种截然不同的模式

在列表推导式中有两种使用 `if` 的方式，它们的作用完全不同：

**模式 1 — 过滤（尾部 `if`）：**

```python
[x for x in items if condition]
```

这会**过滤**可迭代对象。只有当 `condition` 为 `True` 时的元素才会进入结果列表。不匹配的元素会被**完全排除**。

**模式 2 — 转换（条件表达式）：**

```python
[expr_if_true if condition else expr_if_false for x in items]
```

这会**转换每个元素**。没有任何元素会被过滤掉——可迭代对象中的每个元素都会在结果中精确对应生成一个值。条件表达式用于选择使用*哪个*值。

### 条件表达式的工作原理

条件表达式 `a if condition else b` 是 Python 的三元运算符。当 `condition` 为 `True` 时其结果为 `a`，当 `condition` 为 `False` 时其结果为 `b`。

```python
status = "adult" if age >= 18 else "minor"
```

当放在列表推导式内部时，它会对每个元素依次执行：

```python
[expression_using_x if some_check else other_expression for x in items]
```

### 语法

```python
# 过滤：仅保留满足条件的元素
result = [x for x in items if condition]

# 转换：将每个元素映射为两个表达式之一的值
result = [a if condition else b for x in items]

# 结合使用：先过滤，然后转换剩余的元素
result = [a if condition else b for x in items if filter_condition]
```

### 示例

```python
# 将温度转换为标签
temps = [30, 18, 42, 5, 22]
labels = ["hot" if t > 25 else "cool" for t in temps]
# Result: ['hot', 'cool', 'hot', 'cool', 'cool']

# 将负数替换为零，正数保持不变
values = [10, -3, 7, -1, 0]
clamped = [v if v > 0 else 0 for v in values]
# Result: [10, 0, 7, 0, 0]

# 短单词大写，长单词小写
words = ["Hi", "Encyclopedia", "Go", "Refrigerator"]
formatted = [w.upper() if len(w) <= 3 else w.lower() for w in words]
# Result: ['HI', 'encyclopedia', 'GO', 'refrigerator']

# 结合使用：先过滤掉 None，再对剩余元素进行分类
data = [None, 5, None, 12, 3]
result = ["big" if x >= 10 else "small" for x in data if x is not None]
# Result: ['small', 'big', 'small']
```

### 常见模式

| **模式用途示例**                |       |              |
| ------------------------- | ----- | ------------ |
| `"yes" if cond else "no"` | 二元分类  | 将元素标记为两类之一   |
| `x if cond else default`  | 条件默认值 | 替换未通过检查的值    |
| `f(x) if cond else g(x)`  | 条件转换  | 对每个元素应用不同的函数 |
