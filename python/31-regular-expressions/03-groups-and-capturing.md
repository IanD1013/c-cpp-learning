# 正则表达式捕获组 (Regex Capturing Groups)

当你使用 `re.search()` 或 `re.match()` 匹配模式时，通常需要的不仅仅是知道*是否*存在匹配项——你还需要提取匹配文本中的*特定部分*。通过在正则表达式模式中使用括号 `()` 创建的捕获组，可以让你单独隔离并提取匹配项的子部分。这对于解析诸如日志条目、配置值或格式化字符串等结构化文本至关重要。

## 工作原理

当你用括号将正则表达式模式的一部分括起来时，该部分就成为一个**捕获组**。成功匹配后，match 对象允许你通过位置编号访问每个组：

- `match.group(0)` — **整个**匹配到的字符串（等同于 `match.group()`）
- `match.group(1)` — 由**第一对**括号捕获的文本
- `match.group(2)` — 由**第二对**括号捕获的文本
- ...其他组以此类推

`match.groups()` 方法返回一个包含所有捕获组（不包括 group 0）的**元组（tuple）**。

捕获组根据其左括号的位置从左到右依次编号。

## 语法

```python
import re

# Pattern with two capturing groups
pattern = r'(group1_pattern)(group2_pattern)'

match = re.search(pattern, text)
if match:
    full = match.group(0)    # entire match
    first = match.group(1)   # first group
    second = match.group(2)  # second group
    all_groups = match.groups()  # tuple of all groups
```

## 示例

```python
import re

# Example 1: Extract area code and number from a phone pattern
phone_match = re.search(r'\((\d{3})\)-(\d{4})', 'Call (555)-1234 now')
if phone_match:
    print(phone_match.group(0))   # "(555)-1234"
    print(phone_match.group(1))   # "555"
    print(phone_match.group(2))   # "1234"
    print(phone_match.groups())   # ("555", "1234")

# Example 2: Extract first and last name
name_match = re.search(r'([A-Z][a-z]+) ([A-Z][a-z]+)', 'Author: Jane Doe wrote this')
if name_match:
    print(name_match.group(1))    # "Jane"
    print(name_match.group(2))    # "Doe"
    print(name_match.groups())    # ("Jane", "Doe")

# Example 3: Extract key-value pair from config line
config_match = re.search(r'(\w+)=(\S+)', 'timeout=300')
if config_match:
    key = config_match.group(1)   # "timeout"
    value = config_match.group(2) # "300"
```

## 常见模式

| 技巧 | 模式示例 | 说明 |
|---|---|---|
| 多个组 | `(A)(B)(C)` | 每个带括号的部分都是一个独立的组 |
| 字面量 + 捕获组 | `prefix-(\d+)` | 仅捕获数字部分，不捕获前缀 |
| 带量词的组 | `(\w+)+` | 量词作用于整个组 |
| 访问所有组 | `match.groups()` | 返回包含所有捕获组的元组 |