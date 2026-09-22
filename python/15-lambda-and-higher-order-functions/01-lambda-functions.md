### Lambda 函数

**lambda** 是一种使用 `lambda` 关键字内联定义的小型匿名函数。与使用 `def` 创建的常规函数不同，lambda 是紧凑的、无名称的表达式，专为简短、简单的操作而设计。当需要快速定义一次性函数时，它们格外有用——特别是在直接向另一个函数传递行为，或在数据结构中存储轻量级操作时。

### 工作原理

lambda 可以接受任意数量的参数，计算**单个表达式**，并自动返回结果。它没有 `return` 关键字，没有多行函数体，也没有语句（如 `print` 或 `for`）。可以将其视为精简到极致的函数：输入传入，执行一次计算，输出结果。

### 语法

```python
lambda parameters: expression
```

通用形式是 `lambda` 关键字后跟逗号分隔的参数列表、冒号以及单个表达式。表达式的计算结果即为 lambda 的返回值。

你也可以在 lambda 中使用**条件表达式**（三元运算符）：

```python
lambda x: "positive" if x > 0 else "non-positive"
```

### 示例

```python
# A lambda that adds two numbers
add = lambda a, b: a + b
print(add(3, 4))  # 7

# A lambda with a conditional expression
classify = lambda temp: "hot" if temp > 30 else "cool"
print(classify(35))  # "hot"
print(classify(20))  # "cool"

# Lambdas stored in a collection for dispatch
greetings = {
    "english": lambda name: f"Hello, {name}!",
    "spanish": lambda name: f"¡Hola, {name}!",
    "french": lambda name: f"Bonjour, {name}!"
}

greeter = greetings.get("spanish")
if greeter:
    print(greeter("Carlos"))  # "¡Hola, Carlos!"
```

注意最后一个示例：将 lambda 存储为字典的值允许你通过键**查找**对应的函数并调用它。这种模式——有时被称为**调度表（dispatch table）**——可以用简洁、可扩展的映射来代替冗长的 `if/elif` 链。

### 常见模式

**调度表模式（Dispatch table pattern）：** 在字典中将字符串键映射到 lambda 函数。使用 `.get()` 安全地获取函数（如果键不存在则返回 `None`），如果找到则调用它。

```python
converters = {
    "celsius_to_fahrenheit": lambda c: c * 9 / 5 + 32,
    "km_to_miles": lambda km: km * 0.621371
}

fn = converters.get("km_to_miles")
if fn is not None:
    print(fn(10))  # 6.21371
```

**将 lambda 传递给内置函数：**

```python
names = ["alice", "Bob", "CHARLIE"]
sorted_names = sorted(names, key=lambda s: s.lower())
print(sorted_names)  # ['alice', 'Bob', 'CHARLIE']
```
