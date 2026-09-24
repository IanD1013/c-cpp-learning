### 使用 `'a'` 模式向文件追加内容

在处理文件时，你通常需要添加新内容而不破坏已有内容。例如日志文件、聊天记录或交易记录，你希望每个新条目都添加到末尾。Python 的 `open()` 函数支持追加模式（`'a'`），正是专为此目的设计的。

### 工作原理

当你使用 `'w'`（写入模式）打开文件时，文件会被**截断**（truncated），因此在写入任何内容之前，所有现有内容都会被清空。使用 `'a'`（追加模式）时，写入光标定位在文件**末尾**，因此新内容会添加到任何现有内容之后。如果文件尚不存在，`'a'` 模式会自动创建它，就像 `'w'` 一样。

关键区别：

* `'w'` 模式：打开文件，**清空所有内容**，从头开始写入
* `'a'` 模式：打开文件，**保留所有内容**，在最后一个字节之后写入

### 语法

```python
# Append mode - adds to end, never truncates
with open(path, 'a') as f:
    f.write(data)

# Read mode - for reading content back
with open(path, 'r') as f:
    content = f.read()        # entire file as a string
    lines = f.readlines()     # list of lines (each includes '\n')
```

### 示例

```python
# Example 1: Building a simple log file
with open('server.log', 'a') as log:
    log.write('Server started at 08:00\n')

# Later...
with open('server.log', 'a') as log:
    log.write('Request received at 08:05\n')

# server.log now contains both lines, not just the second one

# Example 2: Counting lines in a file
with open('server.log', 'r') as f:
    line_count = len(f.readlines())
    # line_count is 2

# Example 3: Appending to a file that doesn't exist yet
# This creates 'new_file.txt' automatically
with open('new_file.txt', 'a') as f:
    f.write('This file was just created!\n')
```

### 常见模式

常见的模式是先**追加**内容，然后**读取**文件以检查或统计其中的内容。由于无法从以 `'a'` 模式打开的文件中读取内容，你需要打开它两次，一次用于追加，一次用于读取：

```python
# Append something
with open('notes.txt', 'a') as f:
    f.write('Remember to buy milk\n')

# Then read to check the state
with open('notes.txt', 'r') as f:
    all_lines = f.readlines()
    print(f'Total notes: {len(all_lines)}')
```

请记住，`f.write()` **不会**自动添加换行符，因此如果你希望每段内容单独成行，必须自己包含 `' '`。
