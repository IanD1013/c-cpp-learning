### 在 Python 中处理文件内容

当你在 Python 中读取一个文件时，其内容会作为一个单独的字符串返回给你。一个非常常见的实际任务是分析该文本：统计日志文件中的行数、统计文章中的单词数，或者测量用户输入的大小。为了做好这一点，你需要了解如何拆分和度量 Python 字符串。

### 工作原理

读取文件通常如下所示：

```python
with open(path) as file:
    content = file.read()
```

运行之后，`content` 是一个包含文件中所有内容的字符串，包括用于分隔各行的换行符（`\n`）。一旦你获得了该字符串，后续的分析就完全是纯字符串处理了。本练习专注于该处理步骤：直接为你提供内容字符串，以便你可以专注于正确地进行度量统计。

### 实用的字符串工具

| **工具描述示例**       |                            |                                        |
| ---------------- | -------------------------- | -------------------------------------- |
| `len(s)`         | 字符串中的字符数                   | `len("abc")` -> `3`                    |
| `s.split()`      | 按照任意空白字符拆分，忽略开头/结尾/连续的空白间隙 | `"a b c".split()` -> `['a', 'b', 'c']` |
| `s.splitlines()` | 拆分为行列表，去除换行符               | `"a\nb".splitlines()` -> `['a', 'b']`  |

### 示例

```python
# Example 1: counting words in a sentence
text = "the quick brown fox"
print(len(text.split()))       # 4

# Example 2: counting lines
log = "start\nprocessing\ndone"
print(len(log.splitlines()))   # 3

# Example 3: total characters (newlines count too)
snippet = "hi\nyo"
print(len(snippet))            # 5
```

### 重要细节

* `splitlines()` 会妥善处理结尾的换行符：`"a\nb\n".splitlines()` 返回 `['a', 'b']`（2 行，而不是 3 行）。
* 不带参数的 `split()` 会合并连续的空白字符，因此单词之间的多个空格不会产生空条目。
* 在原始内容上调用 `len()` 会统计所有字符，包括任何换行符。
* 空字符串有 `0` 个字符、`0` 个单词，并且应报告为 `0` 行。
