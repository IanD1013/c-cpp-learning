### `functools.partial` — 创建专用函数

`functools.partial` 允许你“冻结”可调用对象的部分参数，从而生成一个具有更简单签名的新可调用对象。这是 *partial application*（偏函数应用）的一种形式 —— 来自函数式编程的一个概念，即把一个接受多个参数的函数转换为一个接受较少参数的函数。当你需要传递一个期望特定签名的回调函数、根据通用函数配置可重用的行为，或者减少重复代码时，它非常有用。

### 工作原理

当你调用 `partial(func, *args, **kwargs)` 时，Python 会返回一个 `partial` 对象。该对象是可调用的。当你传入额外参数调用它时，它会*首先*使用最初冻结的参数调用 `func`，然后再传入你提供的任何新参数。关键字参数会被合并，如果发生冲突，新的关键字参数会覆盖已冻结的参数。

关键在于，`partial` **不会**立即调用该函数 —— 它会生成一个新的可调用对象，该对象会记住冻结的参数以备后续使用。

### 语法

```python
from functools import partial

new_callable = partial(original_function, frozen_arg1, frozen_arg2, key=frozen_kwarg)
result = new_callable(remaining_arg1, remaining_arg2)
```

### 示例

```python
from functools import partial

# 示例 1：特化 int() 以解析二进制字符串
parse_binary = partial(int, base=2)
parse_binary("1010")   # 10
parse_binary("1111")   # 15

# 示例 2：创建日志前缀
def log(level, message):
    return f"[{level}] {message}"

warn = partial(log, "WARN")
warn("Disk space low")  # "[WARN] Disk space low"

# 示例 3：配置折扣计算器
def apply_discount(rate, price):
    return round(price * (1 - rate), 2)

half_off = partial(apply_discount, 0.5)
half_off(80.0)  # 40.0
half_off(25.0)  # 12.5
```

### 内省 Partial 对象

每个 `partial` 对象都公开了三个属性，用于检查被冻结的内容：

| **属性** | **说明** | **示例** |
| :--- | :--- | :--- |
| `.func` | 包装的原始函数 | `half_off.func` → `apply_discount` |
| `.args` | 冻结的位置参数元组 | `half_off.args` → `(0.5,)` |
| `.keywords` | 冻结的关键字参数字典 | `parse_binary.keywords` → `{'base': 2}` |

你还可以通过 `.func` 访问原始函数的元数据，例如通过 `.func.__name__` 获取字符串形式的函数名称。

```python
warn.func.__name__   # "log"
warn.args            # ("WARN",)
warn.keywords        # {}
```

### 位置参数顺序很重要

当你冻结位置参数时，它们会占据*最左侧*的参数槽位。调用偏函数时传入的任何参数都会从左到右填入剩余的槽位。

```python
from operator import sub

subtract_from_100 = partial(sub, 100)
subtract_from_100(30)  # sub(100, 30) → 70
subtract_from_100(99)  # sub(100, 99) → 1
```

这意味着 `partial(pow, 3)` 会创建一个可调用对象，其中 `3` 始终作为 `pow` 的**第一个**参数 —— 因此使用 `4` 调用它时计算的是 `pow(3, 4)` = 81，而不是 `pow(4, 3)`。