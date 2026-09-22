### 列表推导式中的嵌套迭代

列表推导式可以包含多个 `for` 子句，使你能够在一行可读的代码中表达嵌套循环。这是处理多维数据结构的强大技术——凡是通常需要编写两个或更多嵌套 `for` 循环的地方，你通常都可以将它们折叠成一个推导式。

### 工作原理

当你编写带有多个 `for` 子句的列表推导式时，Python 会**从左到右**处理它们，就像你自上而下阅读嵌套循环一样。最左边的 `for` 是外层循环，随后的每个 `for` 都是更深层次的内层循环。对于每个值组合，前面的表达式都会被计算并添加到结果列表中。

可以把它想象成看书：你浏览每个章节（外层循环），在每个章节内，你浏览每一页（内层循环）。

### 语法

```python
# 单层嵌套迭代
[expression for x in outer_iterable for y in inner_iterable]

# 等价的嵌套 for 循环
result = []
for x in outer_iterable:
    for y in inner_iterable:
        result.append(expression)
```

你还可以在任意 `for` 子句之后添加条件：

```python
[expression for x in outer for y in inner if condition]
```

### 示例

```python
# 在 3x3 网格上生成所有坐标对
coords = [(r, c) for r in range(3) for c in range(3)]
# [(0,0), (0,1), (0,2), (1,0), (1,1), (1,2), (2,0), (2,1), (2,2)]

# 将每种颜色与每个尺码组合
colors = ["red", "blue"]
sizes = ["S", "M", "L"]
combinations = [f"{c}-{s}" for c in colors for s in sizes]
# ['red-S', 'red-M', 'red-L', 'blue-S', 'blue-M', 'blue-L']

# 展平结构：从句子列表中提取所有单词
sentences = [["hello", "world"], ["foo", "bar", "baz"]]
all_words = [word for sentence in sentences for word in sentence]
# ['hello', 'world', 'foo', 'bar', 'baz']
```

请注意在展平示例中，**外层** `for` 遍历每个句子，而**内层** `for` 遍历该句子中的每个单词。推导式中从左到右的顺序与等价嵌套循环中自上而下的顺序完全一致。

### 常见模式

| **模式推导式使用场景** |                                                |                         |
| ------------- | ---------------------------------------------- | ----------------------- |
| 所有元素对         | `[(a, b) for a in X for b in Y]`               | 笛卡尔积（Cartesian product） |
| 展平            | `[item for group in groups for item in group]` | 2D → 1D                 |
| 带过滤的元素对       | `[(a, b) for a in X for b in Y if a != b]`     | 带条件的组合                  |
