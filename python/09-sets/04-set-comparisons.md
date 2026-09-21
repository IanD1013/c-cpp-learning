### 集合比较方法

在处理数据集合时，你经常需要了解它们之间的相互关系。某个组是否包含另一个组的所有元素？两个组之间是否存在任何共同成员？Python 的集合比较方法能让你清晰高效地回答这些问题，而无需编写手动循环。

### 工作原理

Python 在集合上提供了三种核心比较方法：

* **`a.issubset(b)`** — 如果 `a` 中的每个元素也都在 `b` 中，则返回 `True`。可以将其理解为在问：“`a` 是否完全包含在 `b` 中？”
* **`a.issuperset(b)`** — 如果 `a` 包含 `b` 的每个元素，则返回 `True`。这是相反的关系：“`a` 是否完全包含 `b`？”
* **`a.isdisjoint(b)`** — 如果 `a` 和 `b` **没有任何共同元素**，则返回 `True`。

此外还有运算符快捷方式和真子集/真超集变体：

| 方法 / 运算符                     | 含义             | 备注            |
| ---------------------------- | -------------- | ------------- |
| `a.issubset(b)` 或 `a <= b`   | a 是 b 的子集      | a 的每个元素都在 b 中 |
| `a < b`                      | a 是 b 的**真**子集 | 是子集且不相等       |
| `a.issuperset(b)` 或 `a >= b` | a 是 b 的超集      | a 包含 b 的每个元素  |
| `a > b`                      | a 是 b 的**真**超集 | 是超集且不相等       |
| `a.isdisjoint(b)`            | a 和 b 没有共同元素   | 没有运算符快捷方式     |

### 语法

```python
set_a.issubset(set_b)      # True if set_a ⊆ set_b
set_a <= set_b             # Same as issubset
set_a < set_b              # Proper subset (subset but not equal)

set_a.issuperset(set_b)    # True if set_a ⊇ set_b
set_a >= set_b             # Same as issuperset
set_a > set_b              # Proper superset (superset but not equal)

set_a.isdisjoint(set_b)    # True if no shared elements
```

### 示例

```python
# Checking if a team's skills are covered by available training
required = {"Python", "SQL"}
available = {"Python", "SQL", "Docker", "AWS"}
required.issubset(available)   # True — all required skills are available

# Checking if a store carries all items on a shopping list
store_inventory = {"milk", "bread", "eggs", "butter"}
shopping_list = {"milk", "eggs"}
store_inventory.issuperset(shopping_list)  # True — store has everything on the list

# Checking if two class schedules conflict
monday_classes = {"Math", "History"}
tuesday_classes = {"Science", "Art"}
monday_classes.isdisjoint(tuesday_classes)  # True — no overlap

# Proper subset vs subset
small = {1, 2}
equal = {1, 2}
big = {1, 2, 3}
small <= equal   # True  (subset — every element matches)
small < equal    # False (NOT a proper subset — they're equal)
small < big      # True  (proper subset — contained but smaller)
```

### 将其他集合类型转换为 set

请记住，这些方法适用于 **set**，但你可以轻松转换列表或元组：

```python
colors_list = ["red", "blue", "red", "green"]
colors_set = set(colors_list)  # {"red", "blue", "green"} — duplicates removed
```

当你关心的是**元素的唯一性**而不是顺序或出现次数时，这尤其有用。
