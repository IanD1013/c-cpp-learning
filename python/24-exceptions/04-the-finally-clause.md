### 异常处理中的 `finally` 子句

在处理数据库连接、文件句柄或网络套接字等资源时，无论代码成功还是失败，你都需要确保清理工作能够执行。`finally` 子句提供了这种保证 —— **无论** `try`、`except` 或 `else` 块中发生什么，它都会执行。

### 工作原理

Python 中完整的异常处理结构遵循以下顺序：

1. **`try`** — 可能引发异常的代码
2. **`except`** — 仅在发生匹配的异常时运行
3. **`else`** — 仅在未发生异常时运行
4. **`finally`** — 无论上述发生什么，**总是**运行

即使在以下情况下，`finally` 块也会执行：

- `try` 块成功完成
- 异常被 `except` 捕获
- 未捕获的异常即将向上传播
- 在 `try`、`except` 或 `else` 中执行了 `return` 语句

⚠️ **重要行为**：如果 `finally` 块中包含自己的 `return` 语句，它将**覆盖**来自 `try`、`except` 或 `else` 的任何返回值。这是一个微妙的陷阱，可能会导致难以排查的 bug。

### 语法

```python
try:
    # Attempt risky operation
except SomeException:
    # Handle the error
else:
    # Runs if no exception occurred
finally:
    # Always runs — cleanup goes here

```

### 示例

```python
# Example 1: finally runs after successful execution
def open_vault(code):
    log = []
    try:
        if code != "1234":
            raise PermissionError("Wrong code")
        log.append("vault opened")
    except PermissionError:
        log.append("access denied")
    finally:
        log.append("alarm reset")
    return log

open_vault("1234")   # ["vault opened", "alarm reset"]
open_vault("0000")   # ["access denied", "alarm reset"]
# Notice "alarm reset" appears in BOTH cases


# Example 2: finally runs even with return statements
def check_temperature(temp):
    try:
        if temp > 100:
            return "boiling"
        return "normal"
    finally:
        print("sensor released")  # This STILL prints!

check_temperature(150)  # Prints "sensor released", returns "boiling"


# Example 3: finally return overrides previous returns
def tricky():
    try:
        return "from try"
    finally:
        return "from finally"  # This wins!

tricky()

```

### 常见模式

`finally` 子句最常用于：

- 关闭文件句柄：确保 `file.close()` 始终运行
- 将数据库连接释放回连接池
- 删除临时文件或重置状态
- 记录操作已完成的日志（无论成功与否）

`else` 子句与 `finally` 配合得很好 —— 将 `else` 用于仅限成功的逻辑，将 `finally` 用于始终运行的清理操作：

```python
try:
    connection = db.connect()
except ConnectionError:
    log("connection failed")
else:
    result = connection.query("SELECT * FROM users")
finally:
    log("attempt complete")  # Logged whether connected or not

```

