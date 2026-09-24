# 从文本文件中处理结构化数据

许多实际任务都涉及读取包含结构化内容的文件，例如每行一个值、逗号分隔的字段或键值对。比如日志文件、配置文件、导出的电子表格或保存到磁盘的传感器读数。读取这些文件、解析其内容并计算结果的能力是数据处理中的一项基本技能。

## 工作原理

处理结构化文本文件的常见模式遵循以下步骤：

1. **打开文件**：使用上下文管理器（`with open(...)`）以确保文件被正确关闭
2. **逐行读取**：避免一次性将整个文件加载到内存中
3. **去除空白字符**：每行通常包含末尾的换行符（`\n`）或空格
4. **跳过空行**：文件中的数据条目之间经常包含空白行
5. **解析每一行**：将其转换为合适的数据类型（string、int、float 等）
6. **累加结果**：根据任务需求累加求和、构建列表、字典等

## 语法

```python
# Reading a file line by line
with open(path, 'r') as f:
    for line in f:
        cleaned = line.strip()   # Remove leading/trailing whitespace and newlines
        if cleaned:              # Skip blank lines
            value = float(cleaned)  # Parse to the needed type
```

## 示例

假设你有一个包含每日温度读数的文件 `temperatures.txt`：

```undefined
72.3
68.1

75.0
69.8
```

你可以像这样计算平均温度：

```python
def average_temperature(path):
    readings = []
    with open(path, 'r') as f:
        for line in f:
            stripped = line.strip()
            if stripped:
                readings.append(float(stripped))
    return sum(readings) / len(readings) if readings else 0.0

# average_temperature('temperatures.txt') -> 71.3
```

或者设想一个每行包含一个单词且部分行为空的文件 `words.txt`：

```python
def count_words(path):
    count = 0
    with open(path, 'r') as f:
        for line in f:
            if line.strip():
                count += 1
    return count
```

## 常见模式

| **模式作用核心方法**     |                   |                    |
| ---------------- | ----------------- | ------------------ |
| `line.strip()`   | 去除两端的 `\n`、空格、制表符 | `str.strip()`      |
| `if stripped:`   | 跳过空字符串（空白行）       | 真值检查（Truthy check） |
| `float(value)`   | 将字符串转换为浮点数        | `float()`          |
| `int(value)`     | 将字符串转换为整数         | `int()`            |
| `with open(...)` | 安全的文件处理（支持自动关闭）   | 上下文管理器             |

请注意，`float()` 既可以处理整数字符串（`"10"` 会变为 `10.0`），也可以处理小数字符串（`"20.5"` 会变为 `20.5`）。
