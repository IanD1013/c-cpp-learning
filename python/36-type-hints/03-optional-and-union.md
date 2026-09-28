# Optional 与 Union 类型提示

Python 的类型系统允许你表达一个变量可能具有多种类型之一，或者可能完全不存在（`None`）。实现这一点的两个关键工具是来自 `typing` 模块的 `Optional` 和 `Union` —— 自 Python 3.10 起，你还可以使用更简洁的 `|` 语法。

理解这些概念有助于你编写具备自解释性的代码，向其他开发者以及像 `mypy` 这样的静态分析工具清晰地传达意图。

## 它们如何工作

**`Optional[X]`** 声明一个值要么是类型 `X`，要么是 `None`。它是 `Union[X, None]` 的简写形式。

当一个函数可能没有有意义的返回值时可以使用它 —— 例如，通过 ID 查找用户时，如果没有找到用户可能会返回 `None`。

**`Union[X, Y, ...]`** 声明一个值可以是几种不同类型中的任意一种。

当一个函数合理地接收或返回不同类型时可以使用它 —— 例如，配置值既可能是字符串标签，也可能是数字代码。

从 Python 3.10 开始，你可以用 `|` 管道语法替换这两者：

- `Optional[int]` → `int | None`
- `Union[int, str]` → `int | str`

## 语法

```python
# Legacy syntax (Python 3.5+)
from typing import Optional, Union

def find_user(user_id: int) -> Optional[dict]:
    ...

def get_config(key: str) -> Union[str, int, float]:
    ...

# Modern syntax (Python 3.10+)
def find_user(user_id: int) -> dict | None:
    ...

def get_config(key: str) -> str | int | float:
    ...
```

## 示例

```python
# Optional: A lookup that might fail
def find_index(items: list[str], target: str) -> int | None:
    try:
        return items.index(target)
    except ValueError:
        return None

find_index(["a", "b", "c"], "b")  # -> 1
find_index(["a", "b", "c"], "z")  # -> None

# Union: A formatter that handles multiple types
def format_value(val: float | bool) -> str:
    if isinstance(val, bool):
        return "yes" if val else "no"
    else:
        return f"{val:.2f}"

format_value(True)    # -> "yes"
format_value(3.14159) # -> "3.14"

# Counting None results from Optional-returning functions
results = [find_index(["x", "y"], s) for s in ["x", "z", "y"]]

# results = [0, None, 1]
none_count = results.count(None)  # -> 1
```

## 常见模式

使用 `try/except` 进行安全转换: 在不同类型之间进行转换时（例如从字符串转换为数字），将转换包装在 `try/except` 中并在失败时返回 `None` 是一个经典的 `Optional` 模式。

使用 `isinstance` 进行基于类型的分支: 当函数接收一个 `Union` 类型时，使用：`isinstance(value, SomeType)` 来确定执行哪个逻辑分支。这比使用：`python type(value) == SomeType`
进行检查更加清晰和可靠。

聚合结果: 在将返回 `Optional` 的函数映射到一个集合后，你通常需要对 `None` 值进行计数或过滤。`list.count(None)` 方法或使用 `is None` / `is not None` 的列表推导式都非常有用。