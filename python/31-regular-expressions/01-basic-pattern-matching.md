### 使用 `re` 进行正则表达式模式匹配

正则表达式（regex）是用于在文本中搜索、匹配和提取模式的强大工具。Python 内置的 `re` 模块为您提供了完整的正则表达式支持。在实际开发中，正则表达式在验证用户输入、解析日志文件、从非结构化文本中抓取结构化数据以及执行复杂的查找和替换操作等任务中不可或缺。

### 工作原理

`re` 模块提供了两个用于查找模式的主要函数：

- **`re.search(pattern, string)`** — 扫描*整个*字符串以查找模式产生匹配的第一个位置。成功时返回一个 **match object**（匹配对象），如果未找到匹配项则返回 `None`。
- **`re.match(pattern, string)`** — 仅检查字符串*开头*是否存在匹配项。如果从索引 0 开始模式不匹配，即使该模式存在于字符串的后半部分，它也会返回 `None`。

当任一函数成功时，返回的 **match object** 具有 `.group()` 方法，可返回实际匹配到的文本。

### 语法

```python
import re

# Search anywhere in the string
result = re.search(r"pattern", some_string)

# Match only at the beginning
result = re.match(r"pattern", some_string)

# Extract the matched text (if result is not None)
if result:
    matched_text = result.group()
```

注意 `r"..."` 前缀 — 这会创建一个 **raw string**（原始字符串），防止 Python 将反斜杠解释为转义序列。对于正则表达式模式，请始终使用原始字符串。

### 关键模式元素

| Pattern | Meaning | Example |
| --- | --- | --- |
| `abc` | 字面字符 | `r"hello"` 匹配 `"hello"` |
| `.` | 除换行符外的任意字符 | `r"h.t"` 匹配 `"hat"`、`"hit"`、`"h3t"` |
| `[abc]` | 集合中的任意一个字符 | `r"[aeiou]"` 匹配任意元音字母 |
| `[a-z]` | 字符范围 | `r"[0-9]"` 匹配任意数字 |
| `+` | 前一个元素的一个或多个 | `r"[0-9]+"` 匹配 `"42"`、`"7"`、`"100"` |
| `{n,}` | 前一个元素的 n 个或更多 | `r"[a-z]{3,}"` 匹配 3 个及以上小写字母 |
| `\.` | 转义的字面点 | `r"3\.14"` 精确匹配 `"3.14"` |

### 示例

```python
import re

# Example 1: Find a phone number pattern in text
text = "Call me at 555-1234 or visit our office."
match = re.search(r"[0-9]{3}-[0-9]{4}", text)
if match:
    print(match.group())  # Output: 555-1234

# Example 2: re.match() vs re.search()
log_line = "ERROR: disk full at /dev/sda1"

# re.match checks only the start of the string
result = re.match(r"ERROR", log_line)
print(result.group())  # Output: ERROR

# re.search finds a match anywhere
result = re.search(r"/dev/[a-z0-9]+", log_line)
print(result.group())  # Output: /dev/sda1

# Example 3: Handling no match gracefully
result = re.search(r"[0-9]+", "no digits here")
if result is None:
    print("No match found")  # Output: No match found

# Example 4: Using character classes for structured patterns
date_text = "The deadline is 2025-07-15, don't forget!"
match = re.search(r"[0-9]{4}-[0-9]{2}-[0-9]{2}", date_text)
if match:
    print(match.group())  # Output: 2025-07-15
```

### 常见模式

`[...]` 内的正则表达式字符类可以组合范围和特定字符：

- `[a-zA-Z]` — 任意字母（大写或小写）
- `[a-zA-Z0-9]` — 任意字母数字字符
- `[a-zA-Z0-9._%+-]` — 字母数字加上若干特殊字符

字符类后面的 `+` 量词表示“这些字符中的一个或多个”，而 `{2,}` 表示“两个或更多”。

### `re.search()` 与 `re.match()` — 何时使用哪一个

当您需要在字符串中*任意位置*查找可能出现的模式时，请使用 `re.search()`。

当您明确需要模式从字符串开头开始匹配时，请使用 `re.match()`。

对于大多数模式嵌入在周围文本中的提取任务，`re.search()` 是正确的选择。