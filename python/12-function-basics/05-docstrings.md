# Python 中的 Docstring

**Docstring**（文档字符串）是作为函数体内部第一条语句出现的字符串字面量。与注释（以 `#` 开头且会被 Python 忽略）不同，docstring 会作为函数对象的一部分存储起来，并可以在运行时通过编程方式访问。它们充当内置文档，供 `help()` 等工具和 IDE 工具提示用来描述代码的功能。

## Docstring 的工作原理

当 Python 在函数中遇到作为第一条语句的字符串字面量时，会将其赋值给函数特殊的 `__doc__` 属性。这意味着你可以通过访问 `function_name.__doc__` 或调用 `help(function_name)` 来检索任何函数的文档。

docstring **不是**注释。注释在编译期间会被剥离，但 docstring 会作为附加在函数对象上的数据持久存在。

## 语法

```python
def some_function(param1, param2):
    """Brief one-line description of what the function does.

    Args:
        param1: Description of the first parameter.
        param2: Description of the second parameter.

    Returns:
        Description of the return value.
    """
    # function body here
```

关键规则：

* docstring 必须是函数体中的**第一条语句**
* 它用**三双引号**括起来：`"""..."""`
* 第一行是对函数用途的简要概述
* 空行之后，`Args:` 列出每个参数及其描述
* `Returns:` 描述函数返回的内容

## 示例

```python
def greet(name):
    """Create a greeting message for the given person.

    Args:
        name: The name of the person to greet.

    Returns:
        A greeting string in the format 'Hello, <name>!'.
    """
    return f"Hello, {name}!"

# Accessing the docstring programmatically:
print(greet.__doc__)
# Output: Create a greeting message for the given person. ...

print(type(greet.__doc__))  # <class 'str'>
```

```python
def find_maximum(values):
    """Find the largest value in a list of numbers.

    Args:
        values: A list of numeric values.

    Returns:
        The maximum value found in the list.
    """
    result = values[0]
    for v in values:
        if v > result:
            result = v
    return result

# The docstring is a real string, not None:
print(find_maximum.__doc__ is not None)  # True
```

## Docstring 与注释的区别

| 特性            | 注释 (`#`)       | Docstring (`"""`) |
| ------------- | -------------- | ----------------- |
| 运行时可访问        | 否              | 是，通过 `__doc__`    |
| 被 `help()` 使用 | 否              | 是                 |
| 用途            | 供阅读代码的开发者参考的说明 | 正式的函数文档           |
| 位置            | 任何位置           | 函数的第一条语句          |

## 没有 Docstring 的情况

如果函数没有 docstring，其 `__doc__` 属性为 `None`：

```python
def mystery(x):
    return x * 2

print(mystery.__doc__)  # None
```
