# Python `print()` 函数

## 1. `print()` 是什么？

`print()` 是 Python 的**内置函数（built-in function）**，用于把内容输出到控制台。

最简单的例子：

```python
print("Hello, World!")
```

运行后：

```text
Hello, World!
```

可以把 `print()` 理解为：

> 让 Python 程序把信息显示给人看。

它不仅适合初学时观察程序结果，在实际开发中也经常用于临时调试。

---

## 2. 打印字符串

Python 中的文本称为**字符串（string）**。

字符串需要使用引号包起来：

```python
print("Welcome to Python")
```

或者：

```python
print('Welcome to Python')
```

输出都是：

```text
Welcome to Python
```

Python 中单引号 `'...'` 和双引号 `"..."` 在表示普通字符串时作用基本相同。

例如：

```python
print("Good morning!")
print('Good morning!')
```

---

## 3. 为什么需要引号？

引号告诉 Python：

> 这里是一段文本，而不是变量名或其他 Python 代码。

例如：

```python
print("Hi")
```

这里 `"Hi"` 是字符串，因此输出：

```text
Hi
```

但是：

```python
print(Hi)
```

Python 会把 `Hi` 当成一个变量名。

如果之前没有定义这个变量，就会出现：

```text
NameError
```

因此：

```python
print("Hello")
```

和：

```python
print(Hello)
```

含义完全不同。

---

## 4. 引号不会被输出

代码：

```python
print("Hello, World!")
```

输出：

```text
Hello, World!
```

而不是：

```text
"Hello, World!"
```

引号属于 Python 语法，只用于标记字符串的开始和结束。

字符串内部的内容，例如：

* 大小写
* 空格
* 逗号
* 感叹号

都会按照实际字符串内容输出。

---

## 5. `print()` 中的括号是什么意思？

基本语法：

```python
print(...)
```

其中：

* `print`：函数名称
* `()`：表示调用函数
* 括号内部：传递给函数的数据

例如：

```python
print("Python")
```

可以理解为：

```text
调用 print 函数
        ↓
把字符串 "Python" 交给它
        ↓
print 将内容输出到控制台
```

在 Python 3 中，调用 `print` 必须使用括号。

正确：

```python
print("Hi")
```

错误：

```python
print "Hi"
```

后者是旧版 Python 2 的写法，在 Python 3 中会产生 `SyntaxError`。

---

## 6. 单引号和双引号

下面两个字符串等价：

```python
"Hello"
```

```python
'Hello'
```

因此：

```python
print("Hello")
print('Hello')
```

都会输出：

```text
Hello
```

但开始和结束的引号必须匹配。

正确：

```python
print("Hello")
print('Hello')
```

错误：

```python
print("Hello')
```

Python 无法正确判断字符串在哪里结束，因此会产生语法错误。

---

## 7. `print()` 不只能打印字符串

虽然最开始通常使用字符串学习 `print()`，但 `print()` 实际上可以输出很多类型的数据。

### 打印字符串

```python
print("42")
```

输出：

```text
42
```

这里的 `"42"` 是**字符串**。

### 打印整数

```python
print(42)
```

输出：

```text
42
```

这里的 `42` 是**整数（int）**。

虽然显示结果一样，但数据类型不同：

```python
"42"   # 字符串
42     # 整数
```

这一区别在后续进行数学运算时非常重要。

---

## 8. `print()` 默认会换行

例如：

```python
print("Hello")
print("Python")
```

输出：

```text
Hello
Python
```

这是因为 `print()` 默认会在输出结束后添加一个换行符。

因此，每调用一次普通的 `print()`，下一次输出通常会从新的一行开始。

---

## 9. 注释不是程序输出

代码中可以使用 `#` 编写注释：

```python
# 打印欢迎信息
print("Welcome to Python")
```

程序真正输出的只有：

```text
Welcome to Python
```

`#` 后面的内容用于给人阅读，Python 不会执行它。

例如：

```python
print("Hello")  # 输出 Hello
```

其中：

```python
# 输出 Hello
```

只是注释。

---

## 10. 常见错误

| 错误代码          | 问题                | 常见结果               |
| ------------- | ----------------- | ------------------ |
| `print "Hi"`  | Python 3 中没有使用括号  | `SyntaxError`      |
| `print(Hi)`   | `Hi` 被当成变量，而不是字符串 | 未定义时产生 `NameError` |
| `print("Hi')` | 开始和结束引号不匹配        | `SyntaxError`      |

### 错误 1：忘记括号

错误：

```python
print "Hello"
```

正确：

```python
print("Hello")
```

### 错误 2：打印文字时忘记引号

错误：

```python
print(Hello)
```

正确：

```python
print("Hello")
```

### 错误 3：引号不匹配

错误：

```python
print("Hello')
```

正确：

```python
print("Hello")
```

或者：

```python
print('Hello')
```

---

## 11. 第一个完整 Python 程序

最经典的 Python 入门程序是：

```python
print("Hello, World!")
```

输出：

```text
Hello, World!
```

需要特别注意：

* `Hello` 的 `H` 是大写
* `Hello` 后面有逗号 `,`
* 逗号后面有一个空格
* `World` 的 `W` 是大写
* 最后有感叹号 `!`

在自动测试或编程练习中：

```text
Hello, World!
```

和：

```text
hello world
```

通常会被认为是不同的输出。

---

## 核心总结

`print()` 的基本形式：

```python
print("要输出的内容")
```

最重要的几个知识点：

1. `print()` 用于输出内容。
2. 文本在 Python 中称为字符串。
3. 字符串需要使用 `'...'` 或 `"..."` 包起来。
4. 字符串的开始和结束引号必须匹配。
5. 引号本身不会被打印。
6. Python 3 调用 `print()` 时需要括号。
7. 输出要求“完全一致”时，要注意大小写、空格和标点符号。
