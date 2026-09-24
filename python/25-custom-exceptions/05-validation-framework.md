### 用于验证的自定义异常层级结构

实际应用程序不仅会捕获错误，还会定义组织成有意义的层级结构的自定义错误类型。例如，支付系统可能以 `PaymentError` 作为基类，并将 `InsufficientFundsError` 和 `ExpiredCardError` 作为专门的子类型。这使得调用者可以根据需求捕获宽泛的类别或具体的问题。

本综合练习结合了你所学到的关于自定义异常的所有知识：定义基类、构建层级结构、存储上下文属性以及在正确的时间抛出正确的异常。

### 异常层级结构的工作原理

当你创建一系列异常类时，捕获父类也会同时捕获所有子类。每个子类都可以携带自己的属性，以提供出错原因的上下文信息。

```python
class AppError(Exception):
    pass

class DatabaseError(AppError):
    def __init__(self, table, operation):
        self.table = table
        self.operation = operation
        super().__init__(f"DB error on {table} during {operation}")

class ConnectionError(AppError):
    def __init__(self, host):
        self.host = host
        super().__init__(f"Cannot connect to {host}")
```

现在调用者可以进行宽泛或具体的处理：

```python
try:
    do_something()
except DatabaseError as e:
    print(f"Table: {e.table}")   # specific handling
except AppError as e:
    print(f"General app error: {e}")  # catches anything else in the hierarchy
```

### 使用 `super()` 定义自定义 `__init__`

当你的异常需要额外的属性时，重写 `__init__`，存储这些属性，并使用描述性消息调用 `super().__init__()`：

```python
class NotFoundError(AppError):
    def __init__(self, resource, resource_id):
        self.resource = resource
        self.resource_id = resource_id
        super().__init__(f"{resource} with id {resource_id} not found")
```

抛出该异常会同时保留消息和属性：

```python
raise NotFoundError("Product", 42)
# str(error) -> "Product with id 42 not found"
# error.resource -> "Product"
# error.resource_id -> 42
```

### 验证模式：检查并抛出

验证函数会检查数据，并针对特定问题抛出特定的异常：

```python
def validate_config(config):
    if "host" not in config:
        raise MissingKeyError("host")
    if not isinstance(config["port"], int) or config["port"] < 1:
        raise InvalidConfigError("port", config["port"], "must be a positive integer")
    return "Config valid"
```

请注意，每次检查都会抛出带有*相关*属性的*不同*异常类型。顺序很重要——在检查有效性之前先检查是否存在。
