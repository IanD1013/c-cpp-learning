### 使用 `**kwargs` 处理可变关键字参数

在 Python 中，函数有时需要接收一组不可预测的命名选项。以配置数据库连接为例，你可能需要 `host`、`port`、`username`、`password`、`timeout` 或它们的任意组合。硬编码每一个可能的参数是不切实际的。这就是 `**kwargs` 的用武之地：它允许函数接收任意数量的关键字参数，并将它们收集到一个字典中。

### 工作原理

当你在函数定义中的参数名前加上 `**` 时，Python 会将所有未与其他已定义参数匹配的关键字参数收集到一个字典中。字典的键是参数名（作为字符串），值是对应的参数值。名称 `kwargs` 只是一种约定俗成的写法，你也可以写成 `**options`、`**config` 或 `**settings`，它们的效果完全相同。

与 `*args`（将位置参数收集到元组中）的主要区别在于，`**kwargs` 将关键字参数收集到一个字典中。

### 语法

```python
# Basic **kwargs usage
def my_function(**kwargs):
    # kwargs is a regular dictionary inside the function
    for key, value in kwargs.items():
        print(f"{key}: {value}")

# Combining regular params, *args, and **kwargs
def full_function(required, *args, **kwargs):
    # required is a normal parameter
    # args is a tuple of extra positional arguments
    # kwargs is a dict of extra keyword arguments
    pass
```

### 示例

```python
# Example 1: A function that logs events with flexible metadata
def log_event(**details):
    message = f"[{details.get('level', 'INFO')}] {details.get('message', 'No message')}"
    return message

log_event(level="ERROR", message="Disk full")
# Returns: "[ERROR] Disk full"

log_event(message="Started")
# Returns: "[INFO] Started"


# Example 2: Merging configuration with defaults
def configure_server(**overrides):
    defaults = {"host": "localhost", "port": 8080, "debug": False}
    defaults.update(overrides)
    return defaults

configure_server(port=3000, debug=True)
# Returns: {"host": "localhost", "port": 3000, "debug": True}

configure_server()
# Returns: {"host": "localhost", "port": 8080, "debug": False}


# Example 3: Iterating over kwargs
def summarize(**stats):
    parts = []
    for key, value in stats.items():
        parts.append(f"{key}={value}")
    return ", ".join(parts)

summarize(wins=10, losses=3)
# Returns: "wins=10, losses=3"
```

### 常见模式

由于 `kwargs` 是一个标准的字典，所有的字典操作都适用于它：

| **操作描述**                     |                  |
| ---------------------------- | ---------------- |
| `kwargs.items()`             | 遍历键值对            |
| `kwargs.keys()`              | 获取所有参数名称         |
| `kwargs.values()`            | 获取所有参数值          |
| `kwargs.get('key', default)` | 安全地访问值并提供回退默认值   |
| `len(kwargs)`                | 统计传入了多少个关键字参数    |
| `dict(kwargs)`               | 从 kwargs 创建一个新字典 |

当没有传入任何关键字参数时，`kwargs` 只是一个空字典 `{}`。
