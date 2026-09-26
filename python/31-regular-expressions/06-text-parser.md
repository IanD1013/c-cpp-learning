# 正则表达式综合实战：日志解析器

实际应用中经常需要解析结构化文本——例如服务器日志、类似 CSV 的数据、配置文件等。在这些场景中，每行通常遵循某种已知格式，但有些行可能格式错误或无关紧要。正则表达式是匹配结构化模式、提取指定组件以及优雅地跳过无效行的理想工具。

本练习结合了你学过的所有核心正则表达式概念：`re.search()`、字符类、量词、分组、锚点、边界以及对多行文本的迭代。

## 工作原理

当你有一个多行字符串时，可以将其拆分为单独的行，并尝试将每一行与模式进行匹配。匹配成功的行会生成捕获组，你可以将其提取为结构化数据；不匹配的行则会被直接忽略。这种“匹配或跳过”的方法既健壮又优雅。

## 语法

```python
import re

# re.search checks if the pattern exists anywhere in the string
match = re.search(r'pattern_with_(groups)', some_string)

if match:
    captured_value = match.group(1)

# Splitting multiline strings
lines = multiline_string.split('\n')
```

## 示例

### 从传感器读数中解析时间戳

```python
import re

reading = "SENSOR-42 @ 2025-08-10T14:22:07 -> temp=72.3"

match = re.search(
    r'(SENSOR-\d+) @ (\d{4}-\d{2}-\d{2})T(\d{2}:\d{2}:\d{2}) -> temp=([\d.]+)',
    reading
)

if match:
    sensor_id = match.group(1)    # "SENSOR-42"
    date = match.group(2)         # "2025-08-10"
    time = match.group(3)         # "14:22:07"
    temperature = match.group(4)  # "72.3"
```

### 从配置文件行中提取键值对并跳过注释

```python
config = "# This is a comment\nhost=localhost\nport=8080\n\ndb_name=mydb"

pattern = r'^([a-z_]+)=(.+)$'

for line in config.split('\n'):
    m = re.search(pattern, line)

    if m:
        print(f"{m.group(1)} => {m.group(2)}")

# host => localhost
# port => 8080
# db_name => mydb
```

### 使用锚点强制执行严格的行格式

```python
# ^ and $ ensure the ENTIRE line matches, not just a substring
strict = r'^\[\d+\] .+$'

re.search(strict, "[42] Valid entry")       # Match
re.search(strict, "Noise [42] hidden")      # No match (doesn't start with [)
re.search(strict, "[abc] Wrong id format")  # No match (\d+ requires digits)
```

## 需要结合的核心概念

| 概念 | 作用 | 模式片段示例 |
|---|---|---|
| 字面量方括号 | 匹配文本中的 `[` 和 `]` | `\[` 和 `\]` |
| `\d{n}` | 精确匹配 n 个数字 | 年份使用 `\d{4}` |
| `\d{2}` | 精确匹配 2 个数字 | 小时、分钟、秒 |
| `[A-Z]+` | 一个或多个大写字母 | 日志级别关键字 |
| `.+` | 一个或多个任意字符 | 自由格式的消息文本 |
| `\s` | 空白字符 | 组件之间的空格 |
| 分组 `()` | 捕获匹配的部分 | 你需要提取的每个组件 |
| `^` 和 `$` | 锚定到行边界 | 拒绝部分匹配或格式错误的行 |