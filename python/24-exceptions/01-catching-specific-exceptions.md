### 捕获特定异常类型

当你的代码在运行时遇到错误时，Python 会引发一个**异常（exception）**——一个描述发生了什么问题的对象。你已经知道如何使用 `try-except` 捕获 `ZeroDivisionError`。但 Python 还有几十种内置的异常类型，每种都代表着不同类型的问题。捕获*正确*的类型至关重要：它既能让你优雅地处理预期的失败，又能让未预期的 bug 立即显现出来。

### 为什么明确异常类型很重要

考虑一个裸的 `except:` 子句——它会捕获**所有内容**，包括 `KeyboardInterrupt`、`SystemExit` 以及诸如 `NameError` 这样的编程错误。这会悄悄掩盖你本该看到并修复的 bug。通过指明具体的异常类型，你为*某一种特定的失败模式*创建了安全网，并让其他所有错误正常向外传播。

```python
# DANGEROUS — hides all errors, even bugs
try:
    result = do_something()
except:
    result = None

# BETTER — only handles the failure you expect
try:
    result = do_something()
except FileNotFoundError:
    result = None
```

### 语法

```python
try:
    # code that might raise an exception
except SomeExceptionType:
    # runs only if SomeExceptionType (or a subclass) is raised

# To access the exception object itself:
try:
    # risky code
except SomeExceptionType as e:
    print(str(e))  # the human-readable error message
```

你也可以捕获多种特定类型：

```python
try:
    value = my_dict[key]
except KeyError:
    print("Key not found")
except TypeError:
    print("Invalid key type")
```

### 常用内置异常类型

| **异常触发时机**          |                    |
| ------------------- | ------------------ |
| `ValueError`        | 函数接收到的参数类型正确，但值不合适 |
| `TypeError`         | 对错误类型的对象应用了某种操作    |
| `KeyError`          | 字典中未找到指定的键         |
| `IndexError`        | 序列索引超出范围           |
| `FileNotFoundError` | 请求的文件或目录不存在        |
| `AttributeError`    | 对象不包含所请求的属性        |

### 示例

```python
# Example 1: Handling KeyError when accessing a dictionary
def get_grade(grades, student):
    try:
        return grades[student]
    except KeyError:
        return "Student not found"

scores = {"Alice": 92, "Bob": 85}
get_grade(scores, "Alice")   # 92
get_grade(scores, "Charlie") # "Student not found"
```

```python
# Example 2: Capturing the exception message with 'as e'
def parse_float(text):
    try:
        return float(text)
    except ValueError as e:
        return f"Could not parse: {str(e)}"

parse_float("3.14")    # 3.14
parse_float("abc")     # "Could not parse: could not convert string to float: 'abc'"
```

```python
# Example 3: IndexError for safe list access
def safe_get(lst, index):
    try:
        return lst[index]
    except IndexError:
        return None

safe_get([10, 20, 30], 1)   # 20
safe_get([10, 20, 30], 99)  # None
```
