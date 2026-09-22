### 使用 `def` 定义函数

在本课程中，你一直在函数体内部编写代码——现在是时候理解它们背后的正式结构了。**函数（function）**是一个具名的、可重复使用的代码块，它接收输入，对其进行处理，并产生输出。函数是构建结构良好、易于维护的 Python 程序的基本构件。

### 工作原理

Python 中的每个函数都以 `def` 关键字开头，后跟一个**函数名**、一对包含零个或多个**参数（parameters）**的圆括号，以及一个冒号。函数体在下方缩进，并在函数被**调用（called）**时执行。`return` 语句将一个值返回给调用方。如果没有执行到 `return`，函数默认返回 `None`。

一个重要的区别：**形参（parameters）**是函数定义中列出的变量名，而**实参（arguments）**是你在调用函数时传入的实际值。

### 语法

```python
def function_name(param1, param2):
    # indented body
    result = param1 + param2
    return result
```

函数名遵循 **snake_case** 命名规范：小写单词之间用下划线分隔。

### 示例

```python
# A function that multiplies two numbers
def multiply(x, y):
    return x * y

# Calling the function with different arguments
print(multiply(4, 5))    # 20
print(multiply(10, -3))  # -30

# A function with no return statement returns None
def greet(name):
    message = f"Hello, {name}!"
    # no return — this function returns None

result = greet("Alice")
print(result)  # None

# A function that checks if a number is even
def is_even(n):
    return n % 2 == 0

print(is_even(7))   # False
print(is_even(12))  # True
```

请注意，`multiply` 只定义了一次，但可以使用不同的参数多次调用。这就是函数的力量——编写一次逻辑，即可根据需要重复使用任意多次。

### 形参与实参（Parameters vs Arguments）

| **术语出现位置含义**      |             |        |
| ----------------- | ----------- | ------ |
| **Parameter（形参）** | 在 `def` 所在行 | 占位符变量名 |
| **Argument（实参）**  | 调用函数时       | 传入的实际值 |

在 `def multiply(x, y)` 中，`x` 和 `y` 是**形参（parameters）**。在 `multiply(4, 5)` 中，`4` 和 `5` 是**实参（arguments）**。

### `return` 语句

`return` 关键字具有两个作用：它指定函数产生的返回值，并立即退出函数。在同一代码块中，`return` 之后的任何代码都不会执行。

```python
def absolute_value(n):
    if n < 0:
        return -n
    return n

print(absolute_value(-8))  # 8
print(absolute_value(3))   # 3
```
