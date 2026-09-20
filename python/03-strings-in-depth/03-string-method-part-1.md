### 字符串转换方法

Python 中的字符串附带了一系列内置方法，可让你转换文本——更改大小写、删除不需要的空白字符以及格式化以供显示。这些对于清理用户输入、一致地比较文本以及准备用于存储或输出的数据至关重要。

### 工作原理

Python 字符串是**对象**，对象可以拥有**方法**——即属于它们自身的函数。你可以使用**点表示法**调用方法：写出字符串（或保存字符串的变量），然后加一个点，接着是带括号的方法名。

一个关键的细节：Python 中的字符串是**不可变的**（immutable）。这意味着没有任何方法会更改原始字符串。相反，每个方法都会返回一个应用了转换的**新**字符串。原始字符串保持完全不变。

### 语法

```python
my_string.method_name()
```

你还可以将方法**链式**调用，一个接一个地调用：

```python
my_string.method_a().method_b()
```

这之所以可行，是因为每个方法都会返回一个新字符串，而你可以立即对该新字符串调用下一个方法。

### 常用方法

| **方法**         | **功能说明**              | **示例**                       | **结果**           |
| -------------- | --------------------- | ---------------------------- | ---------------- |
| `upper()`      | 将所有字符转换为大写            | `"café".upper()`             | `"CAFÉ"`         |
| `lower()`      | 将所有字符转换为小写            | `"LOUD".lower()`             | `"loud"`         |
| `strip()`      | 去除首尾的空白字符（空格、制表符、换行符） | `" hi ".strip()`             | `"hi"`           |
| `title()`      | 将每个单词的首字母大写           | `"good morning".title()`     | `"Good Morning"` |
| `capitalize()` | 仅将第一个字母大写；其余字母小写      | `"hELLO wORLD".capitalize()` | `"Hello world"`  |

### 示例

```python
# upper() and lower() for case conversion
greeting = "Good Evening"
print(greeting.upper())    # "GOOD EVENING"
print(greeting.lower())    # "good evening"

# strip() removes whitespace from both ends, not the middle
raw_input = "   New York   "
print(raw_input.strip())   # "New York"

# title() capitalizes each word
book = "the great gatsby"
print(book.title())        # "The Great Gatsby"

# capitalize() only affects the very first character
message = "pYTHON IS FUN"
print(message.capitalize())  # "Python is fun"

# Chaining methods together
data = "   MESSY DATA   "
print(data.strip().lower())  # "messy data"
```

### 不可变性的实际表现

```python
original = "Hello"
result = original.upper()
print(original)  # "Hello"  — unchanged!
print(result)    # "HELLO"  — the new string
```

原始变量仍然保存着 `"Hello"`，因为 `upper()` 创建并返回了一个全新的字符串。
