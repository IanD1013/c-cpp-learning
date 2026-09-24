### 异常处理综合练习：结合 try、except、else 和 finally

现实世界中的程序经常需要处理混乱、不可预测的数据。Web 服务器可能会收到格式错误的查询参数，配置文件解析器可能会遇到损坏的条目，或者数据管道可能在期望字符串的地方接收到了 `None`。健壮的异常处理能让你在一个清晰易读的代码块中优雅地处理所有这些情况。

本综合练习汇集了你学到的关于 Python 异常处理的所有知识：`try`、针对特定异常类型的多个 `except` 子句、`else` 以及 `finally`。

### 工作原理

Python 完整的异常处理结构如下所示：

1. **`try`** — 运行可能存在风险的代码
2. **`except ExceptionType`** — 捕获并处理特定的错误（可以有多个）
3. **`else`** — 仅在 `try` 代码块中**没有**发生任何异常时运行
4. **`finally`** — 无论是否发生异常，始终都会运行

顺序非常重要：`try` → `except`（一个或多个）→ `else` → `finally`。Python 会自上而下检查 `except` 子句，并执行第一个匹配的子句。

### 语法

```python
try:
    # Code that might raise exceptions
    risky_operation()
except SpecificError:
    # Handle one type of error
except AnotherError:
    # Handle a different type of error
else:
    # Only runs if try succeeded with no exceptions
finally:
    # Always runs, regardless of success or failure
```

### 示例

```python
# Example 1: Parsing a temperature reading
def parse_temperature(reading):
    try:
        parts = reading.strip().split("=")
        sensor = parts[0]
        temp = float(parts[1])
    except AttributeError:
        return "Error: reading must be a string"
    except (ValueError, IndexError):
        return "Error: malformed reading"
    else:
        return {"sensor": sensor, "temperature": temp}
    finally:
        print("Parse attempt complete")  # Logging, cleanup, etc.

parse_temperature("outdoor=23.5")   # {"sensor": "outdoor", "temperature": 23.5}
parse_temperature("outdoor=hot")    # "Error: malformed reading"
parse_temperature(None)             # "Error: reading must be a string"
```

```python
# Example 2: Safe division with full structure
def safe_divide(a, b):
    try:
        result = a / b
    except ZeroDivisionError:
        return "Error: cannot divide by zero"
    except TypeError:
        return "Error: both arguments must be numbers"
    else:
        return round(result, 2)
    finally:
        pass  # finally can be a no-op if no cleanup is needed

safe_divide(10, 3)      # 3.33
safe_divide(10, 0)      # "Error: cannot divide by zero"
safe_divide("10", 3)   # "Error: both arguments must be numbers"
```

### 常见异常类型

| **异常触发时机**       |                                      |
| ---------------- | ------------------------------------ |
| `ValueError`     | 函数接收到的参数类型正确但值不合适（例如 `int("hello")`） |
| `AttributeError` | 对象不包含正在访问的属性或方法（例如 `None.split()`）   |
| `TypeError`      | 操作应用到了错误类型的对象上                       |
| `KeyError`       | 字典中未找到对应的键                           |
| `IndexError`     | 序列索引超出范围                             |

### 理解带有 None 的 AttributeError

当你对非字符串对象（例如 `None` 或整数）调用字符串方法（如 `.split()`）时，Python 会引发 `AttributeError`，因为 `None` 没有 `split` 方法。这与 `TypeError` 不同 —— 这是属性查找本身失败了。

```python
None.split(",")   # AttributeError: 'NoneType' object has no attribute 'split'
42.split(",")     # AttributeError: 'int' object has no attribute 'split'
```
