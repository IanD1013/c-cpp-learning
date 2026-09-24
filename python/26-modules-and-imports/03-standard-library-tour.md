### Python 标准库模块

Python 自带了**标准库**（standard library）——这是一个庞大的模块集合，无需安装任何额外包即可随时使用。这些模块涵盖了从数学运算到文件路径操作、随机数生成以及文本处理等方方面面。了解标准库中的可用功能可以避免重复造轮子。

### 关键模块概述

**`math`** — 数学函数与常量：

* `math.sqrt(x)` 返回 x 的平方根
* `math.pi` 提供 π 的值
* `math.ceil(x)` 和 `math.floor(x)` 用于向上/向下取整

**`random`** — 伪随机数生成：

* `random.randint(a, b)` 返回满足 a ≤ N ≤ b 的随机整数 N
* `random.choice(seq)` 从非空序列中随机选取一个元素
* `random.shuffle(lst)` 就地打乱列表
* `random.sample(population, k)` 从总体中返回 k 个唯一样本元素

**`string`** — 实用的字符串常量：

* `string.ascii_letters` — 所有 ASCII 字母：`'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ'`
* `string.digits` — 数字 `'0123456789'`
* `string.punctuation` — 常见标点字符，如 `'!"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~'`
* `string.ascii_lowercase` 和 `string.ascii_uppercase` 分别代表纯小写和纯大写字母

**`os.path`** — 跨平台文件路径实用工具：

* `os.path.join('dir', 'file.txt')` 安全构建路径
* `os.path.exists(path)` 检查路径是否存在
* `os.path.splitext('report.pdf')` 返回 `('report', '.pdf')`

### `random.choice()` 的工作原理

`random.choice(sequence)` 接收任意非空序列（如列表、元组或字符串），并返回一个随机选取的元素。由于字符串本身就是字符序列，因此将字符串传递给 `random.choice()` 会从该字符串中返回单个随机字符。

### 语法

```python
import random
import string

# Pick a random element from a sequence
random.choice(['red', 'green', 'blue'])   # e.g., 'green'
random.choice('ABCDEF')                   # e.g., 'D'

# Combine string constants
pool = string.ascii_uppercase + string.digits  # 'ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789'
```

### 示例

```python
import random
import string

# Generate a 6-character hex code
hex_chars = string.digits + 'abcdef'
hex_code = ''.join(random.choice(hex_chars) for _ in range(6))
# hex_code might be 'a3f1b9'

# Generate a 4-digit PIN
pin = ''.join(random.choice(string.digits) for _ in range(4))
# pin might be '0742'

# Build a random color name from a list
colors = ['crimson', 'teal', 'amber', 'slate', 'coral']
random_color = random.choice(colors)
# random_color might be 'teal'
```

### 常见模式

当你需要构建指定长度的随机字符串时，通常的模式为：

1. 通过拼接字符串常量来定义可用字符的**字符池**（pool）
2. 使用**生成器表达式**（generator expression）或列表推导式配合 `random.choice()` 来选取字符
3. 使用 `''.join(...)` 将单个字符拼接为一个完整的字符串

### 实用参考

| **表达式描述结果类型**          |                   |                |
| ---------------------- | ----------------- | -------------- |
| `string.ascii_letters` | 所有大小写 ASCII 字母    | `str` (52 个字符) |
| `string.digits`        | 数字字符 0–9          | `str` (10 个字符) |
| `string.punctuation`   | 常见标点符号            | `str` (32 个字符) |
| `random.choice(seq)`   | seq 中的一个随机元素      | 与元素类型相同        |
| `''.join(iterable)`    | 拼接 iterable 中的字符串 | `str`          |
