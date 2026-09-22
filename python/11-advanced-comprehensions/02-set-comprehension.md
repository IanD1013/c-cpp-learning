# 集合推导式 (Set Comprehensions)

你已经知道如何使用推导式来构建列表和字典了。Python 还允许你使用非常相似的语法来构建**集合**（**sets**）。当你需要从某种转换中派生出一个由**唯一**值组成的集合时，集合推导式就是首选工具——没有重复项，轻松搞定。

## 工作原理

集合推导式看起来与字典推导式几乎完全相同，但没有键值之间的冒号（`:`）。因为结果是一个 `set`，所以 Python 会自动丢弃表达式生成的任何重复值。与任何其他集合一样，输出也是**无序**的。

核心要点：如果字典推导式是 `{key: value for ...}`，那么集合推导式就只是 `{value for ...}`。冒号的不存在正是告诉 Python 你想要一个 set 而不是 dict 的原因。

## 语法

```python
# Basic set comprehension
{expression for item in iterable}

# With a condition (filtering)
{expression for item in iterable if condition}
```

## 示例

```python
# Get unique first characters from a list of cities
cities = ["Paris", "Prague", "Berlin", "Barcelona", "Perth"]
first_chars = {city[0] for city in cities}
# Result: {'P', 'B'}  — only unique first letters

# Get unique remainders when dividing by 3
numbers = [10, 7, 4, 13, 1, 6, 9]
remainders = {n % 3 for n in numbers}
# Result: {0, 1, 2}  — duplicates removed automatically

# Unique vowels found in a sentence
sentence = "set comprehensions are elegant"
vowels_found = {ch for ch in sentence.lower() if ch in "aeiou"}
# Result: {'a', 'e', 'i', 'o'}  — each vowel appears once
```

## 与列表推导式对比

| 特性   | 列表推导式            | 集合推导式            |
| ---- | ---------------- | ---------------- |
| 语法   | `[expr for ...]` | `{expr for ...}` |
| 重复项  | 保留               | 移除               |
| 有序   | 是                | 否                |
| 结果类型 | `list`           | `set`            |

请记住：如果你写了带有冒号的 `{x: y for ...}`，你会得到一个 **dict**。如果没有冒号，你就会得到一个 **set**。
