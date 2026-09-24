### `try-except` 块中的 `else` 子句

你已经了解如何使用 `try` 和 `except` 来优雅地处理异常。Python 提供了另一个子句 —— `else` —— 它附加在 `except` 块的末尾，并且**仅在 `try` 块完成且未引发任何异常时**运行。这个看似不起眼的特性在编写清晰、可预测的错误处理代码中起着重要作用。

### 为什么不把所有代码都放进 `try`？

设想你有一段有风险的代码（例如打开文件）和一段成功后执行的代码（例如处理文件内容）。如果将两者都放在 `try` 块内，你的 `except` 子句可能会意外捕获成功代码所引发的异常 —— 而这原本是你并不打算处理的异常。`else` 子句通过为你提供一个专门存放仅在成功时运行的代码的位置来解决这个问题，该位置位于 `except` 处理程序的作用域**之外**。

### 工作原理

1. Python 执行 `try` 块。
2. 如果发生异常，Python 将跳转到匹配的 `except` 子句，并完全**跳过** `else` 块。
3. 如果**未**发生异常，Python 会跳过所有 `except` 子句并**运行** `else` 块。

可以这样理解：`try` = “尝试这件有风险的事”，`except` = “处理失败”，`else` = “庆祝成功”。

### 语法

```python
try:
    # Risky operation that might raise an exception
    result = risky_operation()
except SomeException:
    # Handle the error
    handle_error()
else:
    # Runs ONLY if try succeeded (no exception)
    use_result(result)
```

`else` 子句必须位于所有 `except` 子句**之后**。如果需要，你还可以将其与 `finally` 结合使用（`else` 会在 `finally` 之前运行）。

### 示例

```python
# Example 1: Safe dictionary key lookup
def get_user_age(data, username):
    try:
        age = data[username]
    except KeyError:
        return f"User '{username}' not found"
    else:
        return f"{username} is {age} years old"

users = {"alice": 30, "bob": 25}
get_user_age(users, "alice")   # "alice is 30 years old"
get_user_age(users, "eve")     # "User 'eve' not found"
```

```python
# Example 2: Safe division with post-processing in else
def safe_divide_and_format(a, b):
    try:
        result = a / b
    except ZeroDivisionError:
        return "Cannot divide by zero"
    else:
        # Formatting only happens on success — if round() or f-string
        # raised an error, it would NOT be caught by except above
        return f"Result: {round(result, 2)}"

safe_divide_and_format(10, 3)   # "Result: 3.33"
safe_divide_and_format(10, 0)   # "Cannot divide by zero"
```

```python
# Example 3: Why else matters — accidental catch prevention
def process_config(raw_json):
    import json
    try:
        config = json.loads(raw_json)
    except json.JSONDecodeError:
        return "Invalid JSON"
    else:
        # If config["timeout"] raises KeyError here, it will NOT
        # be silently caught — it will propagate as intended
        return config["timeout"] * 1000
```

请注意示例 3 中：如果我们将 `config["timeout"] * 1000` 放在 `try` 块内，缺少键引发的 `KeyError` 可能会干扰调试。通过将其放入 `else` 中，只有 JSON 解析错误会被捕获。

### 常见模式

| 模式     | `try` 包含            | `else` 包含        |
| ------ | ------------------- | ---------------- |
| 类型转换   | `int()`、`float()`   | 对转换后的值进行数学计算或格式化 |
| 文件 I/O | `open()` 和 `read()` | 处理文件内容           |
| 网络调用   | HTTP 请求             | 解析并使用响应          |
| 字典访问   | `data[key]`         | 转换或返回值           |
