### 章节总结项目：结合多种导入模式

在本章中，你练习了将外部功能引入 Python 代码的三种方式：`import module`、`from module import name` 和 `import module as alias`。这个总结项目将这三种模式整合到一个函数中，对一个数字执行多项数学分析。

### 导入模式如何协同工作

在实际项目中，你经常会在同一个文件中混合使用不同的导入风格。每种风格都有其特定用途：

* **`import module`** 保持命名空间整洁 —— 你通过 `module.function()` 访问所有内容
* **`from module import name`** 允许你直接访问特定项，无需前缀
* **`import module as alias`** 缩短较长的模块名称，同时保持命名空间的清晰性

选择合适的风格取决于你使用每个项的频率以及代码的可读性。

### 语法

```python
# Full module import — access via module prefix
import os
result = os.path.exists("/tmp")

# Selective import — use the name directly
from collections import Counter
c = Counter([1, 2, 2, 3])

# Alias import — shorter prefix
import datetime as dt
today = dt.date.today()
```

### 示例

```python
# Mixing all three patterns in one file
import json
from pathlib import Path
import collections as col

# json requires the prefix
data = json.dumps({"key": "value"})

# Path is used directly
config = Path("/etc/config.txt")

# collections is shortened to col
frequency = col.Counter("mississippi")
```

```python
# Using math in different import styles
import math                    # full module
from math import gcd           # selective
import random as rng           # alias

hyp = math.hypot(3, 4)         # 5.0 — prefix needed
common = gcd(48, 18)            # 6 — direct access
roll = rng.randint(1, 6)       # alias prefix
```

### 实用参考

| **函数模块描述**              |          |                |
| ----------------------- | -------- | -------------- |
| `sqrt(x)`               | math     | x 的平方根         |
| `factorial(n)`          | math     | 非负整数的 n!       |
| `log2(x)`               | math     | x 以 2 为底的对数    |
| `isqrt(n)`              | math     | 整数平方根（向下取整）    |
| `isinstance(obj, type)` | builtins | 检查 obj 是否为给定类型 |
