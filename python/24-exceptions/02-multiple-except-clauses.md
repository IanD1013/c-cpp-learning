### 多个 Except 子句

当你的代码可能以多种不同方式出错时，单个通用的 `except` 块会迫使你以相同的方式处理所有错误。多个 `except` 子句允许你针对每种失败类型提供定制的消息或恢复策略 —— 就像医生针对具体症状进行诊断，而不是对所有病症都开同一种药一样。

### 工作原理

单个 `try` 块后面可以跟随**多个 `except` 子句**，每个子句针对不同的异常类型。当发生异常时，Python 会**从上到下**检查各个 `except` 子句，并执行**第一个匹配的子句**。一旦找到匹配项，其余子句将被完全跳过 —— 这与 `if / elif` 链非常相似。

因为匹配在命中第一个时就会停止，所以**顺序很重要**。如果你将宽泛的异常（如 `Exception`）放在具体异常（如 `ValueError`）之前，宽泛的子句将捕获所有内容，而具体的子句将永远不会执行。

### 语法

```python
try:
    # code that might raise different exceptions
    risky_operation()
except SpecificError:
    # handle SpecificError
except AnotherError:
    # handle AnotherError
except (ErrorA, ErrorB):
    # handle either ErrorA or ErrorB the same way
```

你也可以在**单个子句**中使用元组来捕获多种异常类型：

```python
except (TypeError, ValueError):
    # runs if either TypeError or ValueError is raised
```

### 示例

```python
# Example 1: Parsing user input
def parse_age(text):
    try:
        return int(text)
    except ValueError:
        return "Not a valid number"
    except TypeError:
        return "Expected a string, got something else"

parse_age("25")    # 25
parse_age("hello") # "Not a valid number"
parse_age(None)    # "Expected a string, got something else"
```

```python
# Example 2: Safe dictionary lookup with conversion
def get_score(scores, name):
    try:
        raw = scores[name]
        return float(raw)
    except KeyError:
        return "Player not found"
    except ValueError:
        return "Score is not a number"

get_score({"Ana": "9.5"}, "Ana")   # 9.5
get_score({"Ana": "9.5"}, "Bob")   # "Player not found"
get_score({"Ana": "N/A"}, "Ana")   # "Score is not a number"
```

```python
# Example 3: Grouping exceptions with a tuple
def divide(a, b):
    try:
        return a / b
    except ZeroDivisionError:
        return "Cannot divide by zero"
    except (TypeError, ValueError):
        return "Invalid operand types"

divide(10, 2)      # 5.0
divide(10, 0)      # "Cannot divide by zero"
divide(10, "two")  # "Invalid operand types"
```

### 常见模式

* **具体先于通用** —— 始终先列出具体的异常类型，以免它们被更宽泛的异常类型所遮蔽。
* **元组分组** —— 当两种异常类型需要相同的处理响应时，使用 `except (ExcA, ExcB):` 将它们组合在一起，而不是编写重复代码。
* **访问异常对象** —— 使用 `except SomeError as e:` 可以在响应中包含原始错误信息。

### 常用参考

| **异常触发时机**          |              |
| ------------------- | ------------ |
| `IndexError`        | 序列索引超出范围     |
| `TypeError`         | 操作应用于不合适的类型  |
| `KeyError`          | 字典键未找到       |
| `ValueError`        | 类型正确但不合适的值   |
| `ZeroDivisionError` | 除法或取模运算的除数为零 |
