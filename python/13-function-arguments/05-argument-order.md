### Python 函数中的参数顺序

当你定义的函数混合了常规参数、`*args`、仅限关键字参数（keyword-only parameters）以及 `**kwargs` 时，Python 会强制执行严格的顺序。正确掌握这一点可以带来极大的灵活性 —— 调用者可以毫无歧义地传入位置参数和关键字参数的任意组合。

### 工作原理

Python 从左到右读取函数签名，并对每个参数进行分类：

1. **常规位置参数** —— 可以按位置或按名称传递
2. **`*args`** —— 将任何多余的位置参数捕获为一个元组（tuple）
3. **仅限关键字参数** —— 出现在 `*args` *之后*，并且**必须**按名称传递
4. **`**kwargs`** —— 将任何多余的关键字参数捕获为一个字典（dict）

在签名中位于 `*args`（或单独的 `*`）之后的任何内容都不能通过位置参数填充。Python 强制调用者显式使用参数名，这使得函数调用更具可读性且不易出错。

### 语法

```python
# 完整的参数顺序
def func(pos1, pos2, *args, kw_only1="default", kw_only2=0, **kwargs):
    ...

# 使用单独的 * 创建仅限关键字参数，且不接收多余的位置参数
def func(pos1, pos2, *, kw_only1="default", kw_only2=0):
    ...
```

### 示例

```python
# 示例 1：带有仅限关键字详细程度控制（verbosity）的日志函数
def log(message, *tags, verbose=False):
    tag_str = ", ".join(tags)
    if verbose:
        return f"[{tag_str}] {message} (verbose)"
    return f"[{tag_str}] {message}"

log("Server started", "info", "startup")
# -> '[info, startup] Server started'

log("Error occurred", "error", verbose=True)
# -> '[error] Error occurred (verbose)'


# 示例 2：使用单独的 * 强制使用关键字参数
def create_user(name, *, role="viewer", active=True):
    return {"name": name, "role": role, "active": active}

create_user("Alice", role="admin")
# -> {'name': 'Alice', 'role': 'admin', 'active': True}

# create_user("Alice", "admin")  # TypeError! 'role' is keyword-only


# 示例 3：与 **kwargs 的完整组合
def build_query(table, *columns, distinct=False, **conditions):
    col_str = ", ".join(columns) if columns else "*"
    prefix = "SELECT DISTINCT" if distinct else "SELECT"
    where = " AND ".join(f"{k}='{v}'" for k, v in conditions.items())
    query = f"{prefix} {col_str} FROM {table}"
    if where:
        query += f" WHERE {where}"
    return query

build_query("users", "name", "email", distinct=True, status="active")
# -> "SELECT DISTINCT name, email FROM users WHERE status='active'"
```

### 为什么仅限关键字参数很重要

考虑这样一个函数调用：`process(data, True, False, 10)`。这些 `True`、`False` 和 `10` 分别代表什么？仅限关键字参数强制保证了清晰度：

```python
process(data, verbose=True, dry_run=False, retries=10)
```

通过将配置选项放在 `*args` 或 `*` 之后，你可以确保它们在调用时始终带有参数名。

### 实用参考

| **概念语法行为**   |                        |                           |
| ------------ | ---------------------- | ------------------------- |
| 常规参数         | `def f(a, b):`         | 位置参数或关键字参数                |
| `*args`      | `def f(*args):`        | 将多余的位置参数作为元组捕获            |
| 仅限关键字        | `def f(*args, opt=1):` | `opt` 必须按名称传递             |
| 单独的 `*`      | `def f(a, *, opt=1):`  | 不接收多余的位置参数；`opt` 为仅限关键字参数 |
| `**kwargs`   | `def f(**kwargs):`     | 将多余的关键字参数作为字典捕获           |
| `str.join()` | `sep.join(iterable)`   | 使用分隔符连接字符串                |
