### 遍历字典

字典是 Python 中最通用且实用的数据结构之一，掌握如何高效地遍历字典是日常编程的基本功。无论你是要转换数据、筛选条目，还是基于现有结构构建新结构，字典遍历都是你会经常用到的工具。

### 工作原理

Python 提供了三种不同的方式来循环遍历字典，每种方式都可以让你访问键值对的不同部分：

1. **遍历键（keys）** — 当你直接在字典上进行循环时，Python 会依次返回键。这是默认行为，也是最常见的模式。
2. **遍历值（values）** — 当你只关心值而不关心它们属于哪些键时，可以只遍历值。
3. **遍历键值对（key-value pairs）** — 当你在每次迭代中同时需要键和其对应的值时，可以将它们解包为元组。

### 语法

```python
# Pattern 1: Iterate over keys (default)
for key in my_dict:
    # key is available here

# Pattern 1 (explicit, identical behavior):
for key in my_dict.keys():
    # key is available here

# Pattern 2: Iterate over values only
for value in my_dict.values():
    # value is available here

# Pattern 3: Iterate over key-value pairs (tuple unpacking)
for key, value in my_dict.items():
    # both key and value are available here
```

### 示例

```python
# Example 1: Iterating over keys (default behavior)
scores = {"math": 92, "english": 85, "science": 78}
subjects = []
for subject in scores:
    subjects.append(subject)
# subjects is now ["math", "english", "science"]

# Example 2: Iterating over values to compute a total
prices = {"apple": 1.20, "banana": 0.50, "cherry": 3.00}
total = 0
for price in prices.values():
    total += price
# total is now 4.70

# Example 3: Using items() to build a new dictionary
# Suppose we want to create a dictionary with only high-scoring subjects
scores = {"math": 92, "english": 85, "science": 78}
high_scores = {}
for subject, score in scores.items():
    if score >= 90:
        high_scores[subject] = score
# high_scores is now {"math": 92}

# Example 4: Building a transformed dictionary
temps_celsius = {"London": 15, "Tokyo": 28, "Sydney": 22}
temps_fahrenheit = {}
for city, celsius in temps_celsius.items():
    temps_fahrenheit[city] = celsius * 9 / 5 + 32
# temps_fahrenheit is now {"London": 59.0, "Tokyo": 82.4, "Sydney": 71.6}
```

### 常见模式

基于现有字典构建新字典是非常常见的操作。通常的模式如下：

```python
new_dict = {}
for key, value in original_dict.items():
    # Transform or rearrange as needed
    new_dict[some_new_key] = some_new_value
```

这种模式是许多数据转换的基础——例如重命名键、重构数据，甚至是交换键和值的角色。
