### 常用字典方法

除了方括号语法和 `.get()` 之外，Python 字典还提供了一组丰富的内置方法，使得处理键值对数据更加富有表现力。这些方法让你可以检查所有键、所有值、遍历键值对、合并字典以及有选择地删除条目 —— 这些任务在处理配置数据、API 响应或任何结构化信息时都会频繁遇到。

### 它们的工作原理

字典方法大致分为两类：用于查看字典内容的**视图（views）**，以及就地修改字典的**修改器（mutators）**。

**视图方法** —— `.keys()`、`.values()` 和 `.items()` —— 返回特殊的视图对象，它们就像是透视字典的窗口。它们不会创建副本；而是始终反映字典的当前状态。

**修改器方法** —— `.update()` 和 `.pop()` —— 直接修改字典。`.update()` 合并来自另一个字典的数据，而 `.pop()` 删除指定的键。

### 语法

```python
# View methods
d.keys()        # returns a view of all keys
d.values()      # returns a view of all values
d.items()       # returns a view of (key, value) tuples

# Mutator methods
d.update(other)  # merges other into d, overwriting shared keys
d.pop(key)       # removes key, returns its value (KeyError if missing)
d.pop(key, default)  # removes key, returns default if missing
```

### 示例

```python
# Working with views
scores = {"math": 90, "science": 85, "english": 92}

print(list(scores.keys()))    # ['math', 'science', 'english']
print(list(scores.values()))  # [90, 85, 92]
print(list(scores.items()))   # [('math', 90), ('science', 85), ('english', 92)]

# Views are dynamic — they reflect changes automatically
k = scores.keys()
print("history" in k)  # False
scores["history"] = 88
print("history" in k)  # True (the view updated!)

# Merging with .update()
defaults = {"color": "blue", "size": "medium", "weight": 10}
overrides = {"size": "large", "material": "cotton"}
defaults.update(overrides)
print(defaults)
# {'color': 'blue', 'size': 'large', 'weight': 10, 'material': 'cotton'}
# Note: 'size' was overwritten by the value from overrides

# Removing with .pop()
inventory = {"apples": 5, "bananas": 3, "oranges": 8}
removed = inventory.pop("bananas")
print(removed)	   # 3
print(inventory)   # {'apples': 5, 'oranges': 8}

# Using a default with .pop() to avoid KeyError
result = inventory.pop("grapes", 0)
print(result)  # 0 (no KeyError raised)
```

### 常见模式

```python
# Iterating over key-value pairs
for key, value in scores.items():
    print(f"{key}: {value}")

# Checking membership efficiently
if "math" in scores:  # uses keys view implicitly
    print("Found math score")

# Creating a copy before mutating
original = {"x": 1, "y": 2}
copy = dict(original)       # shallow copy via dict() constructor
copy["z"] = 3
print(original)  # {'x': 1, 'y': 2} — unchanged
print(copy)      # {'x': 1, 'y': 2, 'z': 3}
```

### 重要提示：`.update()` 会就地修改

请注意，`.update()` 会修改调用它的字典 —— 它**不会**返回一个新字典。如果你需要合并两个字典且不改变任何一个原始字典，你需要考虑先使用副本进行操作。
