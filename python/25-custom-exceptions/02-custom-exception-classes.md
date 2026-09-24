### 自定义异常类

Python 的内置异常（例如 `ValueError` 和 `TypeError`）涵盖了通用的错误类别，但在实际应用中通常需要更具体的错误类型。自定义异常类允许你创建有意义的、特定于业务领域的异常，从而使错误处理更加精确且信息更丰富。与其抛出带有消息字符串的通用 `ValueError`，不如定义一个 `PaymentDeclinedError` 或 `DatabaseConnectionError`，以便调用者可以单独捕获它们。

### 工作原理

自定义异常本质上就是一个继承自 `Exception`（或其任何子类）的类。当你 `raise` 自定义类的实例时，Python 的异常处理机制会像对待其他任何异常一样对待它 —— 可以通过 `try/except` 进行捕获，具有字符串表示形式，并在调用栈中向上传播。其强大之处在于能够添加自定义属性，以携带发生错误时的上下文数据。

### 语法

```python
class MyCustomError(Exception):
    def __init__(self, detail, message="Something went wrong"):
        self.detail = detail
        super().__init__(f"{message}: {detail}")
```

关键点：

* 继承自 `Exception` 或更具体的内置异常
* 重写 `__init__` 以接收自定义参数
* 将有用的数据存储为实例属性
* 使用格式化后的错误消息字符串调用 `super().__init__()`

### 示例

```python
# A custom exception for insufficient funds in a banking app
class InsufficientFundsError(Exception):
    def __init__(self, account_id, amount, balance):
        self.account_id = account_id
        self.amount = amount
        self.balance = balance
        super().__init__(f"Account {account_id}: tried to withdraw {amount}, but balance is {balance}")

# Raising and catching it
def withdraw(account_id, amount, balance):
    if amount > balance:
        raise InsufficientFundsError(account_id, amount, balance)
    return balance - amount

try:
    withdraw("ACC-123", 500, 200)
except InsufficientFundsError as e:
    print(e)              # Account ACC-123: tried to withdraw 500, but balance is 200
    print(e.account_id)   # ACC-123
    print(e.amount)       # 500
```

```python
# A custom exception with a default message
class TemperatureOutOfRangeError(Exception):
    def __init__(self, temp, message="Temperature out of safe range"):
        self.temp = temp
        super().__init__(f"{message}: {temp}°C")

# Using it
raise TemperatureOutOfRangeError(150)
# TemperatureOutOfRangeError: Temperature out of safe range: 150°C

raise TemperatureOutOfRangeError(150, "Critical overheating detected")
# TemperatureOutOfRangeError: Critical overheating detected: 150°C
```

### 常见模式

* **带有可覆盖的默认消息**：接收一个带有合理默认值的 `message` 参数，以便调用者在需要时可以自定义消息
* **存储上下文数据**：将相关值保存为属性（`self.whatever`），以便异常处理程序可以检查出错的原因
* **调用 `super().__init__()`**：始终向父类传递格式化字符串，以便 `str(exception)` 生成可读的消息
* **选择性捕获**：调用者现在可以编写 `except YourSpecificError`，而不是捕获宽泛的异常类型
