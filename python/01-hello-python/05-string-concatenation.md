# Python 字符串拼接：使用 `+` 连接字符串

## 1. 什么是字符串拼接

在 Python 中，`+` 不仅可以用于数字加法，也可以用于连接字符串。

当 `+` 两边都是字符串时，Python 会把它们首尾连接起来，这种操作叫做 **字符串拼接（String Concatenation）**。

```python
print("Hello" + "World")
```

输出：

```text
HelloWorld
```

这里并没有进行数学运算，而是把 `"Hello"` 和 `"World"` 连接成了一个新的字符串：

```text
HelloWorld
```

---

## 2. `+` 的作用取决于数据类型

相同的 `+`，面对不同类型的数据时，含义不同。

### 数字：执行加法

```python
print(3 + 4)
```

输出：

```text
7
```

### 字符串：执行拼接

```python
print("3" + "4")
```

输出：

```text
34
```

因此：

```python
3 + 4
```

和：

```python
"3" + "4"
```

完全不同。

| 表达式         | 数据类型 | 结果     |
| ----------- | ---- | ------ |
| `3 + 4`     | 数字   | `7`    |
| `"3" + "4"` | 字符串  | `"34"` |

---

## 3. 基本语法

两个字符串可以直接使用 `+` 连接：

```python
print("first" + "second")
```

也可以连续连接多个字符串：

```python
print("one" + "two" + "three")
```

Python 会从左到右进行拼接：

```text
"one" + "two" + "three"
↓
"onetwo" + "three"
↓
"onetwothree"
```

最终输出：

```text
onetwothree
```

---

## 4. `+` 不会自动添加空格

字符串拼接最容易出错的地方是：

> `+` 只负责连接字符串，不会自动插入空格。

例如：

```python
print("Good" + "Morning")
```

输出：

```text
GoodMorning
```

如果希望输出：

```text
Good Morning
```

就必须自己把空格写进字符串：

```python
print("Good " + "Morning")
```

这里 `"Good "` 最后包含一个空格：

```text
"Good "
      ↑
    空格
```

因此最终结果是：

```text
Good Morning
```

---

## 5. 多个字符串之间的空格也必须手动处理

例如：

```python
print("one" + "two" + "three")
```

输出：

```text
onetwothree
```

如果希望得到：

```text
one two three
```

可以写：

```python
print("one " + "two " + "three")
```

拼接过程相当于：

```text
"one " + "two " + "three"
   ↓
"one two " + "three"
   ↓
"one two three"
```

---

## 6. `+` 和 `print()` 中的逗号不一样

`print()` 可以同时接收多个值：

```python
print("cat", "dog")
```

默认情况下，`print()` 会在不同参数之间添加一个空格，因此输出：

```text
cat dog
```

但使用 `+`：

```python
print("cat" + "dog")
```

输出：

```text
catdog
```

对比：

| 代码                      | 输出        |
| ----------------------- | --------- |
| `print("cat", "dog")`   | `cat dog` |
| `print("cat" + "dog")`  | `catdog`  |
| `print("cat " + "dog")` | `cat dog` |

核心区别：

* `print("a", "b")`：`print()` 默认在参数之间加入空格
* `"a" + "b"`：直接连接，不加入任何字符

---

## 7. 空格本身也是字符串的一部分

字符串中的空格并不是特殊标记，而是真实的数据。

下面三个字符串是不同的：

```python
"Hello"
"Hello "
" Hello"
```

分别表示：

```text
Hello
Hello␠
␠Hello
```

其中 `␠` 表示空格。

因此下面两种方式都能得到：

```text
Hello World
```

### 把空格放在第一个字符串末尾

```python
print("Hello " + "World")
```

### 把空格单独作为字符串

```python
print("Hello" + " " + "World")
```

通常第一种更简洁。

---

## 8. 常见错误：字符串不能直接和数字用 `+` 拼接

例如：

```python
print("Age: " + 20)
```

会报错，因为：

```python
"Age: "
```

是字符串，而：

```python
20
```

是整数。

Python 不会自动猜测你是想：

* 做数学加法
* 还是把数字变成文本

如果确实需要拼接，可以先使用 `str()` 把数字转换成字符串：

```python
print("Age: " + str(20))
```

输出：

```text
Age: 20
```

不过在实际 Python 程序中，更常见的做法是使用 **f-string**：

```python
age = 20
print(f"Age: {age}")
```

输出同样是：

```text
Age: 20
```

对于初学阶段，先理解 `+` 的字符串拼接规则最重要。

---

## 9. 核心总结

Python 中：

```python
+
```

的含义取决于操作的数据类型。

```python
3 + 4
```

表示数字加法：

```text
7
```

而：

```python
"3" + "4"
```

表示字符串拼接：

```text
34
```

字符串拼接最重要的规则是：

> `+` 只负责把字符串首尾连接起来，不会自动添加空格、逗号或其他分隔符。

因此：

```python
"Hello" + "World"
```

得到：

```text
HelloWorld
```

而：

```python
"Hello " + "World"
```

才会得到：

```text
Hello World
```

---

# Python 字符串拼接练习解答

## 问题

编写一个 `solution()` 函数，使用字符串拼接输出下面两行内容：

```text
Hello, Python!
I am learning to code
```

要求：

1. 第一行必须使用 **两个字符串**通过 `+` 拼接。
2. 第二行必须使用 **三个字符串**通过 `+` 拼接。

---

## 答案

```python
def solution():
    print("Hello, " + "Python!")
    print("I am " + "learning " + "to code")
```

运行：

```python
solution()
```

输出：

```text
Hello, Python!
I am learning to code
```

---

## 为什么这样写

第一行：

```python
"Hello, " + "Python!"
```

包含两个字符串：

```text
"Hello, "
"Python!"
```

第一个字符串末尾包含一个空格，所以拼接结果是：

```text
Hello, Python!
```

第二行：

```python
"I am " + "learning " + "to code"
```

包含三个字符串：

```text
"I am "
"learning "
"to code"
```

前两个字符串末尾分别包含空格，因此最终得到：

```text
I am learning to code
```

---

## 常见错误

下面的代码：

```python
print("Hello," + "Python!")
```

会输出：

```text
Hello,Python!
```

因为 `+` 不会自动添加空格。

同样：

```python
print("I am" + "learning" + "to code")
```

会输出：

```text
I amlearningto code
```

因此使用字符串拼接时，要特别检查每个单词之间需要的空格是否已经包含在字符串中。
