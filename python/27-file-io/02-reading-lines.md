### 逐行处理文本

处理文本数据时，你经常需要逐行处理它，而不是将其作为一个整体块来处理。一个常见的任务是计算有多少行实际包含有意义的内容，同时忽略空白行或仅包含空白字符的行。在解析日志、配置文件以及任何基于行的文本格式时，都会用到这种模式。

### 工作原理

多行文本使用换行符 `\n` 来分隔各行。将文本拆分成行时，其中某些行可能是空的，或者仅包含空格、制表符或换行符。为了判断某一行是否包含实际内容，你通常先清理它，然后再检查剩下的内容。

字符串方法 `splitlines()` 可以将一个字符串拆分为行列表，它会自动处理换行分隔符并去除换行符本身：

```python
"a\nb\nc".splitlines()      # ['a', 'b', 'c']
"a\n\nb".splitlines()        # ['a', '', 'b']
"".splitlines()             # []
```

### `strip()` 方法

字符串方法 `strip()` 会移除所有开头和结尾的空白字符，包括空格、制表符和换行符：

```python
"  hello  ".strip()    # Returns "hello"
"".strip()              # Returns "" (empty string)
"   \t   ".strip()      # Returns "" (empty string)
"data".strip()          # Returns "data" (unchanged)
```

在 Python 中，空字符串被视为 falsy（假值），而任何非空字符串都被视为 truthy（真值）。这在检查某一行是否包含有意义的内容时非常有用。

### 语法

```python
for line in text.splitlines():
    # process each line here
    pass
```

### 示例

```python
# Example: count lines that start with a digit
text = "1 apple\nbanana\n2 pears"
count = 0
for line in text.splitlines():
    cleaned = line.strip()
    if cleaned and cleaned[0].isdigit():
        count += 1
# count is 2

# Example: sum numbers where each line holds one number
text = "10\n\n20\n30"
total = 0
for line in text.splitlines():
    cleaned = line.strip()
    if cleaned:
        total += int(cleaned)
# total is 60
```

### 字符串的真假值

请记住，在布尔上下文中，Python 将空字符串视为 `False`，将非空字符串视为 `True`：

```python
bool("")              # False
bool("hello")         # True
bool("   ")           # True, spaces are characters
bool("   ".strip())   # False, after stripping it is empty
```
