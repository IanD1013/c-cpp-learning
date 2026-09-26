# 正则表达式字符类与量词

当你需要在文本中查找遵循结构化规则的模式时（如电话号码、电子邮件地址或数值），字符类和量词能让你精确控制模式中每个位置所匹配的内容以及它可以重复的次数。这两个特性结合在一起，将正则表达式从简单的字面量匹配转变成一种强大的模式描述语言。

## 字符类的工作原理

**字符类**定义了一组可以在单个位置匹配的字符集合。你可以使用方括号自定义字符类，或者使用内置的简写形式：

| **类匹配内容示例** |  |  |
| --- | --- | --- |
| `[a-z]` | 任意小写字母 | `[a-z]` 匹配 `"q3"` 中的 `'q'` |
| `[A-Za-z]` | 任意字母 | `[A-Za-z]` 匹配 `"H2O"` 中的 `'H'` |
| `[0-9]` | 任意数字 | `[0-9]` 匹配 `"x7y"` 中的 `'7'` |
| `\d` | 任意数字（`[0-9]` 的简写形式） | `\d` 匹配 `"room4"` 中的 `'4'` |
| `\w` | 单词字符：`[a-zA-Z0-9_]` | `\w` 匹配 `"a_b"` 中的 `'_'` |
| `\s` | 任意空白字符（空格、制表符、换行符） | `\s` 匹配 `"a b"` 中的 `' '` |

大写版本表示对该类的取反：`\D` 匹配任意非数字字符，`\W` 匹配任意非单词字符，`\S` 匹配任意非空白字符。

## 量词的工作原理

**量词**跟在字符类（或字面量）之后，用于指定连续匹配的出现次数：

| **量词含义示例模式在 `"aabbb"` 中的匹配项** |  |  |  |
| --- | --- | --- | --- |
| `+` | 一次或多次 | `b+` | `"bbb"` |
| `*` | 零次或多次 | `b*` | `""`、`"bbb"`（取决于上下文） |
| `?` | 零次或一次 | `b?` | `"b"` 或 `""` |
| `{n}` | 恰好 n 次 | `b{2}` | `"bb"` |
| `{n,m}` | n 到 m 次之间 | `b{1,3}` | `"bbb"` |
| `{n,}` | n 次或更多 | `b{2,}` | `"bbb"` |

## 语法

```python
import re

# re.findall returns ALL non-overlapping matches as a list of strings
results = re.findall(r'pattern', text)
```

请记得使用**原始字符串**（`r'...'`），这样反斜杠就不会在传给正则表达式引擎之前被 Python 解释。

## 示例

```python
import re

# Find all words that start with an uppercase letter
text = "Alice met Bob at Central Park"
re.findall(r'[A-Z]\w*', text)
# Returns: ['Alice', 'Bob', 'Central', 'Park']

# Find all sequences of digits
log = "Error 404 at line 23, code 500"
re.findall(r'\d+', log)
# Returns: ['404', '23', '500']

# Find hexadecimal color codes like #1a2B3c
css = "color: #ff0099; background: #1a2b3c;"
re.findall(r'#[0-9a-fA-F]{6}', css)
# Returns: ['#ff0099', '#1a2b3c']

# Find words of exactly 3 letters
sentence = "The cat sat on a mat"
re.findall(r'\b\w{3}\b', sentence)
# Returns: ['The', 'cat', 'sat', 'mat']
```

## 结合字符类与量词

真正的威力来自于将它们结合使用。考虑匹配模式中的可选部分：`?` 量词使前面的元素成为可选的（出现零次或一次）。你可以将量词应用于字面量字符或分组。

```python
# Match "colour" or "color"
re.findall(r'colou?r', "I like colour and color")
# Returns: ['colour', 'color']

# Match a digit sequence optionally followed by a dot and more digits
# \d+ matches one or more digits
# \.\d+ matches a dot followed by one or more digits
# Wrapping with ? makes the dot-digits part optional
```

仔细思考“可选”意味着什么：`X?` 表示 X 可以出现零次或一次。当将其与分组或精心设计的顺序结合时，你就可以匹配包含可选部分的结构。