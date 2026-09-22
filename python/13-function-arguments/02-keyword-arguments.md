### 关键字参数 (Keyword Arguments)

在 Python 中调用函数时，通常会按照形参定义的精确顺序传递参数。但 Python 提供了一个更灵活的选择：**关键字参数**。通过在调用处指定形参名称，不仅能提高代码的可读性，还可以让你摆脱记忆参数顺序的烦恼。

### 工作原理

每个函数形参都有一个名称。调用函数时，你可以使用该名称配合 `=` 显式地为其赋值。这被称为**关键字参数**（与仅依赖位置的**位置参数**相对）。使用关键字参数时，Python 会按名称将每个值匹配到对应的形参，而与顺序无关。

有一条重要规则：在函数调用中，**位置参数必须始终在关键字参数之前**。你可以混合使用这两种风格，但一旦使用了关键字参数，其后的所有参数也必须是关键字参数。

### 语法

```python
# Defining a function — nothing changes here
def connect(host, port, timeout=30):
    ...

# Calling with positional arguments (order matters)
connect("localhost", 5432, 10)

# Calling with keyword arguments (order doesn't matter)
connect(timeout=10, port=5432, host="localhost")

# Mixing positional and keyword (positional first!)
connect("localhost", timeout=10, port=5432)
```

### 示例

```python
def create_tag(element, content, bold=False):
    if bold:
        return f"<{element}><b>{content}</b></{element}>"
    return f"<{element}>{content}</{element}>"

# All positional — you need to remember the order
create_tag("p", "Hello", True)
# Returns: '<p><b>Hello</b></p>'

# Using keyword arguments — much clearer what True means
create_tag("p", "Hello", bold=True)
# Returns: '<p><b>Hello</b></p>'

# Reordering with keywords — works perfectly
create_tag(content="Hello", element="p")
# Returns: '<p>Hello</p>'

# Mixing: first arg positional, rest as keywords
create_tag("div", bold=True, content="World")
# Returns: '<div><b>World</b></div>'
```

注意，`bold=True` 比单纯的 `True` 可读性要高得多——读者能立即明白该布尔值控制着什么。这是关键字参数最大的优势之一：**自文档化代码**。

### 常见模式

* **布尔标志**作为关键字参数几乎总是更清晰：`process(data, verbose=True)` 优于 `process(data, True)`
* **带有默认值的可选参数**非常适合使用关键字方式调用——你只需指定想要覆盖的参数
* **拥有多个参数的函数**在调用者使用关键字时会更易于管理：`send_email(to="a@b.com", subject="Hi", body="Hello")` 读起来就像一张表单
