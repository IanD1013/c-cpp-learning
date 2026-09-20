### 转义字符与原始字符串

Python 中的字符串可以包含直接输入时不可见的特殊字符。这些**转义字符**让你可以将换行符、制表符和其他格式直接嵌入到字符串中。理解它们的工作原理——以及如何使用原始字符串禁用它们——对于处理文件路径、多行输出和格式化文本至关重要。

### 转义字符的工作原理

转义字符以反斜杠（`\`）开头，后跟一个特定的字母。Python 识别到这种组合后会将其替换为特殊字符：

| **转义序列结果描述** |     |             |
| ------------ | --- | ----------- |
| `\n`         | 换行  | 移动到下一行      |
| `\t`         | 制表符 | 插入水平制表符     |
| `\\`         | `\` | 字面反斜杠       |
| `\"`         | `"` | 双引号字符串内的双引号 |
| `\'`         | `'` | 单引号字符串内的单引号 |

核心要点：`\n` 在字符串内部是**一个字符**，而不是两个。反斜杠告诉 Python "下一个字符是特殊的"。

### 语法

```python
# Escape characters in regular strings
greeting = "Hello\nWorld"    # Contains a newline between Hello and World
indented = "Name:\tAlice"    # Contains a tab between Name: and Alice
path = "C:\\Users\\docs"    # Contains literal backslashes

# Raw strings — prefix with r to treat backslashes as literal
raw_path = r"C:\Users\docs"  # Backslashes are kept as-is
raw_text = r"no\nnewline"    # \n stays as backslash + n (two characters)
```

### 示例

```python
# Escape characters produce special formatting
header = "Name\tAge\tCity"
print(header)
# Output:
# Name    Age    City

poem = "Roses are red\nViolets are blue"
print(poem)
# Output:
# Roses are red
# Violets are blue

# len() reveals the true character count
print(len("AB\nCD"))   # 5 — A, B, newline, C, D
print(len(r"AB\nCD"))  # 6 — A, B, \, n, C, D (raw string, no escape)

# Embedding quotes
dialogue = "She said \"hello\" to me"
print(dialogue)  # She said "hello" to me

# Using \n as a join separator
fruits = ["apple", "banana", "cherry"]
result = "\n".join(fruits)
print(result)
# Output:
# apple
# banana
# cherry
```

### 常见模式

当你需要根据数据构建多行字符串时，经常会用到 `\n`：

```python
# Joining items with a separator
colors = ["red", "green", "blue"]
tab_separated = "\t".join(colors)   # "red\tgreen\tblue"
line_separated = "\n".join(colors)  # "red\ngreen\nblue"

# Splitting and rejoining with a different separator
csv_line = "Alice,Bob,Charlie"
names = csv_line.split(",")          # ["Alice", "Bob", "Charlie"]
formatted = " | ".join(names)        # "Alice | Bob | Charlie"
```

原始字符串在处理反斜杠应保持字面含义的 **Windows 文件路径**和**正则表达式**时最为有用。
