### 异常层次结构

在构建真实的应用程序或库时，你很少只需要一个自定义异常。你需要一个组织成树状结构的、由相关异常组成的**家族**（family）。基类异常作为根节点，具体异常由此衍生分支。这让调用者可以选择其精度级别：捕获基类以处理所有相关错误，或捕获叶子节点类以处理某个特定情况。

这种模式随处可见——HTTP 库定义了 `HTTPError` 及其子类 `ConnectionError` 和 `TimeoutError`。ORM 定义了 `DatabaseError` 及其子类 `IntegrityError` 和 `OperationalError`。

### 工作原理

Python 的 `except` 子句会捕获一个异常**及其所有子类**。如果 `TimeoutError` 继承自 `NetworkError`，而 `NetworkError` 又继承自 `AppError`，那么 `except AppError` 会同时捕获这三者。这就是异常层次结构的强大之处——一个 `except` 代码块就可以处理一整个子树的错误。

层次结构中的每个类都可以自定义其 `__init__` 方法来接收不同的参数、存储相关属性，并通过 `super().__init__()` 沿着继承链向上传递格式化后的消息。

### 语法

```python
class BaseAppError(Exception):
    def __init__(self, code, message):
        self.code = code
        self.message = message
        super().__init__(f"[{code}] {message}")

class SpecificError(BaseAppError):
    def __init__(self, code):
        super().__init__(code, "something specific went wrong")
```

子类调用的是**父类**的 `__init__`，而不是直接调用 `Exception.__init__`。这保持了整个层次结构中消息格式的一致性。

### 示例

```python
# A payment processing hierarchy
class PaymentError(Exception):
    def __init__(self, tx_id, reason):
        self.tx_id = tx_id
        self.reason = reason
        super().__init__(f"Transaction {tx_id}: {reason}")

class InsufficientFundsError(PaymentError):
    def __init__(self, tx_id, balance, amount):
        self.balance = balance
        self.amount = amount
        super().__init__(tx_id, f"insufficient funds (need {amount}, have {balance})")

class ExpiredCardError(PaymentError):
    def __init__(self, tx_id):
        super().__init__(tx_id, "card is expired")

# Catching at different levels
try:
    process_payment(order)
except InsufficientFundsError as e:
    print(f"Not enough money: need {e.amount}")
except PaymentError as e:
    print(f"Payment failed: {e.reason}")
```

注意 `InsufficientFundsError` 是如何存储额外属性（`balance`、`amount`）但仍将消息处理委托给父类的。而 `ExpiredCardError` 不需要额外数据——它只提供了一个固定的原因字符串。

### 常见模式

* **在调用 super 之前存储属性**：子类通常先保存自己的属性，然后传入组合后的消息调用 `super().__init__()`。
* **捕获并重新抛出**：在函数内部，捕获内置异常（如 `ValueError`）并改为抛出你的自定义异常。这会将低级错误转换为应用程序的错误词汇。
* **叶子类中的固定消息**：最具体的异常通常具有预定义的消息，仅接收标识符或上下文信息。
