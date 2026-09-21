### 集合运算

Python 集合不仅仅是存储唯一值的容器。它们还支持强大的数学运算，能让你高效地比较和合并集合。当你需要查找多组数据之间的共有元素、唯一元素或差异时，集合运算可以用简洁、快速的单行代码替代缓慢的嵌套循环。

### 工作原理

集合运算通过基于哈希的查找（每个元素 O(1)）来比较两个集合的元素，而不是重复扫描每个元素（使用嵌套循环为 O(n²)）。Python 提供了四种核心运算，每种运算都可以作为运算符或方法使用。

### 四种核心运算

| **运算**                      | **运算符**  | **方法**                      | **返回值**                       |
| --------------------------- | -------- | --------------------------- | ----------------------------- |
| 并集 (Union)                  | `a \| b` | `a.union(b)`                | 两个集合中的所有元素                    |
| 交集 (Intersection)           | `a & b`  | `a.intersection(b)`         | 仅在**两个**集合中都存在的元素             |
| 差集 (Difference)             | `a - b`  | `a.difference(b)`           | 在 `a` 中但**不在** `b` 中的元素       |
| 对称差集 (Symmetric Difference) | `a ^ b`  | `a.symmetric_difference(b)` | 在**任意一个**集合中但**不同时存在于两者**中的元素 |

所有四种运算都会返回一个**新集合**。原始集合绝不会被修改。

### 语法

```python
# Using operators
result = set_a | set_b    # union
result = set_a & set_b    # intersection
result = set_a - set_b    # difference
result = set_a ^ set_b    # symmetric difference

# Using methods (identical results)
result = set_a.union(set_b)
result = set_a.intersection(set_b)
result = set_a.difference(set_b)
result = set_a.symmetric_difference(set_b)
```

### 示例

```python
backend_languages = {"Python", "Java", "Go", "Rust"}
frontend_languages = {"JavaScript", "TypeScript", "Python", "Rust"}

# Union: all languages mentioned by either team
all_languages = backend_languages | frontend_languages
# {"Python", "Java", "Go", "Rust", "JavaScript", "TypeScript"}

# Intersection: languages used by both teams
shared = backend_languages & frontend_languages
# {"Python", "Rust"}

# Difference: backend-only languages (order matters)
backend_only = backend_languages - frontend_languages
# {"Java", "Go"}

# Symmetric difference: languages used by exactly one team
exclusive = backend_languages ^ frontend_languages
# {"Java", "Go", "JavaScript", "TypeScript"}
```

```python
# Converting between lists and sets
raw_tags = ["python", "data", "python", "ml", "data"]
unique_tags = set(raw_tags)        # {"python", "data", "ml"}
sorted_tags = sorted(unique_tags)  # ["data", "ml", "python"]
```

### 为什么集合优于循环

使用嵌套循环比较两个列表的时间复杂度为 O(n × m)。转换为集合并使用集合运算的时间复杂度为 O(n + m)。对于两个各包含 10,000 个元素的列表，这是 100,000,000 次比较与约 20,000 次哈希操作的区别，差距极其巨大。

```python
# Slow: nested loop approach for finding overlap
overlap = []
for item in list_a:
    if item in list_b:
        overlap.append(item)

# Fast: set-based approach, same result, much faster
overlap = set(list_a) & set(list_b)
```
