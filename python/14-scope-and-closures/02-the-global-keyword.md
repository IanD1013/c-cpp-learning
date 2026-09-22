### Python 中的 `global` 关键字

当你在函数内部对一个变量进行赋值时，Python 默认会创建一个**局部**变量——即使在模块级别存在同名的变量也是如此。`global` 关键字允许你推翻这种行为，告诉 Python 某个特定的名称指向的是**全局**（模块级别）变量。理解 `global` 很重要，但学会*何时不使用它*更加重要。

### 工作原理

如果不使用 `global`，函数内部的任何赋值操作都会创建一个新的局部变量，并**遮蔽**全局变量。全局变量保持不变：

```python
counter = 0

def increment():
    counter = counter + 1   # UnboundLocalError!
```

Python 看到 `counter = ...` 并将 `counter` 视为局部变量，但接着 `counter + 1` 试图在其被局部赋值之前读取它——从而导致错误。

`global` 关键字通过声明该名称属于全局作用域来解决这个问题：

```python
counter = 0

def increment():
    global counter          # must appear before any use of 'counter'
    counter = counter + 1   # now modifies the global variable
```

### 语法

```python
def some_function():
    global variable_name        # declare before using
    variable_name = new_value   # modifies the global variable
```

关键规则：

* `global variable_name` 必须出现在函数中对该变量进行任何读取或写入操作**之前**。
* 你可以声明多个全局变量：`global x, y, z`。
* 该变量**不必**事先存在——`global` 也可以*创建*一个新的全局变量。

### 示例——为什么 `global` 会带来问题

```python
# Approach 1: Using global (discouraged)
balance = 100

def withdraw(amount):
    global balance
    balance = balance - amount

withdraw(30)
print(balance)  # 70 — but any code anywhere could change 'balance'
```

这使得 `withdraw` 依赖于隐式的外部状态。如果另一个函数也修改了 `balance`，排查 bug 就会变得非常痛苦。测试也变得更加困难——你必须在每次测试之前重置全局变量。

```python
# Approach 2: Pure function (preferred)
def withdraw(amount, current_balance):
    return current_balance - amount

balance = 100
balance = withdraw(30, balance)
print(balance)  # 70 — clear data flow, easy to test
```

第二种版本是**显式**的：数据通过参数传入，并通过返回值输出。没有意外，也没有隐式状态。

### 另一个示例——累加值

```python
# Global approach (fragile)
score = 0

def add_points(pts):
    global score
    score = score + pts

add_points(10)
add_points(5)
print(score)  # 15 — but 'score' is invisible in the function signature
```

```python
# Functional approach (robust)
def add_points(pts, current_score=0):
    return current_score + pts

score = add_points(10)        # 10
score = add_points(5, score)  # 15
```

通过使用**默认参数**，该函数既可以独立工作，*也*可以作为链式调用的一部分——而无需触碰任何全局状态。

### 最佳实践

| **模式**       | **优点**     | **缺点**       |
| ------------ | ---------- | ------------ |
| `global` 关键字 | 快捷、输入较少    | 隐式依赖、难以测试、脆弱 |
| 传递与返回值       | 显式、可测试、可组合 | 略微冗长         |
| 带属性的类        | 封装状态、权责清晰  | 对于简单情况设置较繁琐  |

**经验法则：** 当你想使用 `global` 时，问问自己是否可以改为将值作为参数传递并返回结果。几乎在所有情况下，你都可以做到这一点——并且你的代码会因此变得更好。
