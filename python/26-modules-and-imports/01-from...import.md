### 使用 `from...import` 进行选择性导入

你已经知道如何使用 `import math` 导入整个模块，然后通过点符号（如 `math.sqrt(25)`）访问其内容。但如果你只需要模块中的一两个内容呢？Python 提供了一种更有针对性的方法：`from...import` 语法。这允许你将特定的名称直接引入当前的命名空间中，从而无需模块前缀即可使用它们。

### 工作原理

当你编写 `from module import name` 时，Python 会在后台加载该模块，但仅将指定的名称绑定到本地命名空间中。这意味着你可以像在自己的文件中定义该名称一样使用它——无需前缀。你可以通过用逗号分隔来一次导入多个名称。

与常规 `import` 语句的关键区别：

* `import math` → 名称 `math` 被添加到你的命名空间中，你需要通过 `math.pi`、`math.floor()` 等访问内容。
* `from math import pi, floor` → 名称 `pi` 和 `floor` 被直接添加，因此你只需编写 `pi` 或 `floor(3.7)`。

### 语法

```python
# Import a single name
from module_name import some_function

# Import multiple names
from module_name import name1, name2, name3

# You can also rename on import with 'as'
from module_name import long_function_name as short_name
```

### 示例

```python
# Example 1: Import a constant from the math module
from math import e
print(e)          # 2.718281828459045 — no prefix needed

# Example 2: Import multiple items at once
from math import ceil, log
print(ceil(4.2))  # 5
print(log(100))   # 4.605170185988092 (natural log)

# Example 3: Import and rename for convenience
from math import factorial as fact
print(fact(6))    # 720

# Example 4: Using imported functions in expressions
from math import pow, pi
area = pi * pow(5, 2)   # Area of circle with radius 5
print(area)             # 78.53981633974483
```

### 何时使用 `from...import`

| **方法最适合权衡**             |                   |                      |
| ----------------------- | ----------------- | -------------------- |
| `import math`           | 使用来自 `math` 的许多项目 | 必须为所有内容加上 `math.` 前缀 |
| `from math import sqrt` | 仅使用 `sqrt`        | 代码更清晰，但只有该名称可用       |
| `from math import *`    | 快速编写脚本（不推荐）       | 污染命名空间——在生产代码中应避免    |

当你只需要几个特定的项目并且想要简洁、易读的代码时，`from...import` 风格是理想的选择。
