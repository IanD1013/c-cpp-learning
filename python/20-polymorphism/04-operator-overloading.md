### 运算符重载 —— 隐藏的多态

你已经知道多态意味着“多种形态” —— 相同的接口根据对象的不同而表现出不同的行为。你已经在方法重写（method overriding）和鸭子类型（duck typing）中见识过了。但在 Python 中，多态的体现更加深入：它被直接内置到了**运算符**本身之中。

`+` 运算符就是一个完美的例子。当你编写 `a + b` 时，Python 并没有针对 `+` 的操作预设唯一的硬编码规则。相反，它会询问左侧的对象：*“你打算如何处理 `+`？”* 每种类型都会定义自己的答案。

### 工作原理

在幕后，Python 中的每个运算符都映射到一个**特殊方法**（也称为 "dunder" 方法）。当 Python 遇到 `a + b` 时，实际上会调用 `a.__add__(b)`。每种类型实现 `__add__` 的方式都不同：

* `int.__add__` 执行算术加法
* `str.__add__` 执行字符串拼接
* `list.__add__` 执行列表合并

你不需要直接调用 `__add__` —— Python 会自动将 `+` 转换为正确的方法调用。关键的认知在于：**同一个运算符会根据类型的不同触发不同的行为**，这正是多态的含义。

这种模式被称为**运算符重载**（operator overloading）：单个运算符被“重载”以赋予多重含义，并根据操作数类型选择正确的行为。

### 语法

```python
# The + operator works across many types
result = operand_a + operand_b
```

无需使用 `if/elif` 检查类型。无需使用 `isinstance()`。运算符本身就是多态的 —— 它会根据 `a` 和 `b` 的类型自行确定要做什么。

### 示例

```python
# Arithmetic addition with floats
2.5 + 3.7        # 6.2

# Tuple concatenation
(1, 2) + (3, 4)  # (1, 2, 3, 4)

# String repetition uses * — another overloaded operator
"ha" * 3          # "hahaha"

# The - operator is also polymorphic
{1, 2, 3} - {2}  # {1, 3}  — set difference
10 - 4            # 6       — arithmetic subtraction
```

请注意，这些示例都不需要检查类型。你只需使用运算符，Python 就会分发到正确的实现。这就是运算符重载的力量。

### 为什么这很重要

编写适用于**支持该运算符的任何类型**的代码 —— 且无需检查类型 —— 是 Pythonic 设计的标志。它使你能够编写通用的、可复用的函数。如果有人稍后创建了一个定义了 `__add__` 的自定义类，你的函数也可以直接与它协同工作，无需任何修改。

### 常见模式

| 运算符  | int/float | str  | list  | tuple |
| ---- | --------- | ---- | ----- | ----- |
| `+`  | 加法        | 拼接   | 合并    | 拼接    |
| `*`  | 乘法        | 重复   | 重复    | 重复    |
| `==` | 值相等       | 内容相等 | 逐元素相等 | 逐元素相等 |
