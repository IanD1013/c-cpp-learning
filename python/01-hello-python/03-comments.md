# Python 注释（Comments）学习笔记

## 1. 什么是注释

**注释（comment）** 是写在代码中、用于帮助人理解程序的说明文字。

Python 在执行程序时不会执行真正的注释，因此注释通常用来：

* 解释代码在做什么；
* 说明为什么采用某种写法；
* 记录注意事项或 TODO；
* 帮助自己或其他开发者阅读代码。

例如：

```python
# 打印一条欢迎信息
print("Hello")
```

运行结果：

```text
Hello
```

`# 打印一条欢迎信息` 不会出现在输出中。

---

## 2. 单行注释：`#`

Python 正式的注释语法是 `#`。

从 `#` 开始，一直到这一行结束，都会被 Python 当作注释。

### 独立一行的注释

```python
# This is a comment
print("Hello")
```

### 写在代码后面的注释

```python
print("Hello")  # Print a greeting
```

两种写法的输出都只有：

```text
Hello
```

### 注意

`#` 只有在字符串外部才表示注释。

例如：

```python
print("# is not a comment here")
```

输出：

```text
# is not a comment here
```

因为这里的 `#` 位于字符串 `"..."` 内部，所以只是普通字符。

---

## 3. 多行说明应该怎么写

严格来说，**Python 没有专门的“多行注释语法”**。

如果需要写多行真正的注释，最标准的方式是连续使用 `#`：

```python
# Calculate the area
# by multiplying the width
# by the height.

width = 10
height = 5
area = width * height
```

这种写法才是真正的 Python 注释。

---

## 4. 三引号不是严格意义上的注释

有时会看到这样的代码：

```python
"""
This text spans
multiple lines.
"""
```

或者：

```python
'''
This text also spans
multiple lines.
'''
```

这种写法看起来很像“多行注释”，但从 Python 语法角度来说，它实际上是一个：

> **多行字符串（multi-line string / triple-quoted string）**

也就是说：

```python
"""
Hello
World
"""
```

本质上和：

```python
"Hello"
```

一样，都是字符串，只不过这个字符串可以跨越多行。

如果这个字符串没有被保存到变量中，也没有被其他代码使用，通常不会产生可见效果：

```python
"""
Some explanation here.
"""

print("Hello")
```

输出：

```text
Hello
```

因此在初学教程或编程练习中，三引号有时会被称为“多行注释”。

但更准确的理解是：

```text
#                    → 真正的 Python 注释
"""...""" / '''...''' → 多行字符串，不是真正的注释
```

---

## 5. 三引号还有一个重要用途：Docstring

三引号最重要的正式用途之一是编写 **docstring（文档字符串）**。

例如：

```python
def add(a, b):
    """
    Return the sum of a and b.
    """
    return a + b
```

这里的三引号字符串不是普通的“无用字符串”，而是函数 `add()` 的文档。

可以通过：

```python
print(add.__doc__)
```

读取它。

因此不要简单地认为：

```text
三引号 = 多行注释
```

更准确的是：

```text
三引号 = 多行字符串
```

它有时可以达到类似注释的视觉效果，也可以正式作为 docstring 使用。

---

## 6. 注释和程序输出没有关系

例如：

```python
# Calculate rectangle area
width = 10
height = 5

area = width * height

print(area)  # Display the result
```

程序只执行：

```python
width = 10
height = 5
area = width * height
print(area)
```

所以输出：

```text
50
```

注释不会被 `print()` 自动打印出来。

---

## 7. 注释应该解释“为什么”，而不是重复代码

不太有价值的注释：

```python
# Set width to 10
width = 10
```

代码本身已经非常清楚。

更有意义的注释：

```python
# Use a fixed width because the API currently accepts only 10-column layouts.
width = 10
```

好的注释通常解释：

* 为什么这样设计；
* 有什么特殊限制；
* 有什么不明显的行为；
* 将来需要注意什么。

代码本身应该尽量清晰，而不是依赖大量注释才能理解。

---

## 8. 核心总结

| 写法                | 实际含义            | 是否是真正的注释 |
| ----------------- | --------------- | -------- |
| `# comment`       | 单行注释            | ✅ 是      |
| 多行连续 `#`          | 多行注释            | ✅ 是      |
| `"""..."""`       | 多行字符串           | ❌ 严格来说不是 |
| `'''...'''`       | 多行字符串           | ❌ 严格来说不是 |
| 函数开头的 `"""..."""` | Docstring 文档字符串 | ❌ 不是普通注释 |

最重要的是记住：

```python
# Python 真正的注释使用 #
```

需要多行注释时，推荐：

```python
# First line
# Second line
# Third line
```

而 `"""..."""` 和 `'''...'''` 本质上是字符串。

---

# Python Comments 编程题解答

## 问题

编写一个 `solution()` 函数，要求：

1. 至少包含一个使用 `#` 的单行注释；
2. 至少包含一个使用 `"""..."""` 或 `'''...'''` 的多行文本块；
3. 控制台必须**只输出**：

```text
Comments are invisible!
```

函数形式：

```python
def solution():
```

---

## 答案

```python
def solution():
    # This is a single-line comment

    """
    This is a multi-line
    block of text.
    """

    print("Comments are invisible!")
```

调用：

```python
solution()
```

输出：

```text
Comments are invisible!
```

---

## 为什么这个答案满足要求

单行注释：

```python
# This is a single-line comment
```

不会被执行。

三引号文本：

```python
"""
This is a multi-line
block of text.
"""
```

按照这道练习题的要求，它被用作所谓的“multi-line comment”。更准确地说，它实际上是一个没有被使用的多行字符串。

真正产生输出的只有：

```python
print("Comments are invisible!")
```

因此最终只有一行输出：

```text
Comments are invisible!
```

注意字符串内容必须完全一致，包括：

* 大小写；
* 空格；
* 最后的 `!`。

例如下面这些都不符合“exactly”的要求：

```python
print("comments are invisible!")
print("Comments are invisible")
print("Comments are invisible! ")
```
