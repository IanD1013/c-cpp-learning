### 正则表达式锚点与单词边界

正则表达式不仅可以匹配字符 —— 它们还可以断言字符串中的**位置**。像 `^`、`$` 和 `\b` 这样的锚点不会消耗任何字符；相反，它们断言正则表达式引擎当前处于某个特定位置。当您需要验证**整个字符串**是否符合某种模式，或者需要匹配**完整单词**而不意外匹配嵌入在更长单词中的子字符串时，这种区分至关重要。

### 工作原理

- **`^`** 断言字符串**开头**的位置。在多行模式（`re.MULTILINE`）下，它匹配每行的开头。
- **`$`** 断言字符串**结尾**的位置。在多行模式下，它匹配每行的结尾。
- **`\b`** 断言一个**单词边界** —— 即单词字符（`\w`：字母、数字、下划线）与非单词字符（`\W`：除单词字符外的所有字符）之间的隐形位置，或者当字符串以单词字符开头/结尾时，字符串的开头/结尾位置。

当您编写 `^pattern$` 时，正则表达式引擎要求**整个字符串**从头到尾都匹配 `pattern`。如果没有锚点，`re.search(r'\d+', 'abc123def')` 会顺利在中间找到 `123` —— 但是 `re.search(r'^\d+$', 'abc123def')` 会返回 `None`，因为整个字符串并不全由数字组成。

Python 还提供了 `re.fullmatch(pattern, string)`，其行为类似于在 pattern 两端隐式包裹了 `^` 和 `$`。在验证整个字符串时，这是一个简洁的替代方案。

### 语法

```python
import re

# Using ^ and $ anchors explicitly
re.match(r'^[a-z]+$', text)     # Anchored match from start to end
re.search(r'^[a-z]+$', text)    # Same effect with search

# Using re.fullmatch() — equivalent to ^pattern$
re.fullmatch(r'[a-z]+', text)   # Entire string must be lowercase letters

# Word boundaries
re.findall(r'\bcat\b', text)    # Matches 'cat' as a whole word only
```

### 示例

```python
import re

# Validate that a string is a 5-digit ZIP code
print(bool(re.fullmatch(r'\d{5}', '90210')))    # True
print(bool(re.fullmatch(r'\d{5}', '9021')))     # False (too short)
print(bool(re.fullmatch(r'\d{5}', '90210x')))   # False (extra char)

# Without anchors, partial matches sneak through
print(bool(re.search(r'\d{5}', 'code90210x')))  # True — found inside!
print(bool(re.search(r'^\d{5}$', 'code90210x')))# False — anchored

# Word boundaries to find whole words
text = "The category of a cat is catalogued."
print(re.findall(r'\bcat\b', text))             # ['cat'] — only the whole word
print(re.findall(r'cat', text))                  # ['cat', 'cat', 'cat'] — substrings too

# Validate an email-like pattern (simplified)
pattern = r'[a-zA-Z][a-zA-Z0-9.]*@[a-zA-Z]+\.[a-zA-Z]{2,}'
print(bool(re.fullmatch(pattern, 'dev@mail.com')))  # True
print(bool(re.fullmatch(pattern, '@mail.com')))     # False — must start with letter
```

### 常用模式

| **模式验证内容匹配示例** | | |
| --- | --- | --- |
| `^\d{3}-\d{4}$` | 电话格式 XXX-XXXX | `555-1234` |
| `^[A-Z]{2}\d{4}$` | 类似 AB1234 的代码 | `XY9876` |
| `\b\d+\b` | 文本中的独立数字单词 | `"I have 42 apples"` 中的 `42` |
| `^[a-z_]+$` | 仅限小写字母和下划线 | `my_var` |

### 实用参考

| **函数描述** | |
| --- | --- |
| `re.fullmatch(pattern, string)` | 仅当**整个**字符串都匹配时才成功 |
| `re.match(pattern, string)` | 从字符串的**开头**开始匹配（没有隐式 `$`） |
| `re.search(pattern, string)` | 在字符串的**任意位置**搜索 |
| `bool(match_object)` | 如果找到匹配项则返回 `True`，如果为 `None` 则返回 `False` |

### 字符类复习

| **字符类含义** | |
| --- | --- |
| `[A-Za-z]` | 任意 ASCII 字母 |
| `[A-Za-z0-9_]` | 字母、数字或下划线（在 ASCII 中等同于 `\w`） |
| `{3,16}` | 重复 3 到 16 次（包含两端） |