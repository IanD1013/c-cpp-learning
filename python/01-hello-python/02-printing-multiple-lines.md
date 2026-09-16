# Python `print()` 多次调用与多行输出

## 1. 多次调用 `print()` 的作用

在 Python 中，每调用一次 `print()`，默认都会：

1. 输出指定的内容；
2. 在输出末尾添加一个**换行符**；
3. 让下一次输出从新的一行开始。

例如：

```python
print("Hello")
print("World")
```

输出：

```text
Hello
World
```

不需要手动告诉 Python 换行，因为 `print()` 默认已经会这样做。

---

## 2. `print()` 为什么会自动换行

实际上：

```python
print("Hello")
```

可以简单理解为输出：

```text
Hello\n
```

其中：

```text
\n
```

表示**换行符（newline character）**。

因此：

```python
print("Hello")
print("World")
```

执行过程可以理解为：

```text
输出 Hello
换行
输出 World
换行
```

最终得到：

```text
Hello
World
```

---

## 3. Python 按顺序执行多个 `print()`

Python 通常按照代码**从上到下**执行。

例如：

```python
print("Apples")
print("Bananas")
print("Cherries")
```

执行顺序是：

```text
print("Apples")
        ↓
print("Bananas")
        ↓
print("Cherries")
```

所以输出也是：

```text
Apples
Bananas
Cherries
```

代码中 `print()` 的顺序决定了最终输出内容的顺序。

---

## 4. 一个 `print()` 通常输出一行

例如：

```python
print("One")
print("Two")
print("Three")
```

输出：

```text
One
Two
Three
```

这里有三个 `print()` 调用，因此产生三行输出。

需要注意的是，这并不是说 `print()` **永远只能输出一行**。例如字符串内部也可以包含换行符：

```python
print("Hello\nWorld")
```

输出：

```text
Hello
World
```

不过在初学阶段，需要输出多行内容时，使用多个 `print()` 通常更加清晰。

---

## 5. 在函数中使用多个 `print()`

多个 `print()` 也可以放在函数中：

```python
def greet():
    print("Hello")
    print("World")
```

这里只是**定义函数**，此时函数内部的代码还不会执行。

需要调用函数：

```python
greet()
```

才会得到：

```text
Hello
World
```

完整示例：

```python
def greet():
    print("Hello")
    print("World")


greet()
```

---

## 6. 函数定义的基本语法

Python 函数的基本结构是：

```python
def function_name():
    statement
    statement
```

例如：

```python
def show_numbers():
    print("One")
    print("Two")
```

其中：

* `def`：表示定义函数；
* `show_numbers`：函数名称；
* `()`：函数参数列表，这里表示没有参数；
* `:`：函数定义末尾必须有冒号；
* 缩进的代码：属于函数内部。

因此：

```python
def print_lines()
```

不是完整的 Python 函数定义，因为缺少 `:`。

正确写法是：

```python
def print_lines():
```

---

## 7. 常见错误

### 错误一：忘记函数定义后的冒号

错误：

```python
def print_lines()
    print("Line 1")
```

正确：

```python
def print_lines():
    print("Line 1")
```

---

### 错误二：函数内部没有缩进

错误：

```python
def print_lines():
print("Line 1")
```

正确：

```python
def print_lines():
    print("Line 1")
```

Python 使用缩进判断哪些代码属于函数。

---

### 错误三：只定义函数但没有调用

```python
def print_lines():
    print("Line 1")
    print("Line 2")
```

运行这段代码通常不会打印任何东西，因为这里只定义了函数。

需要调用：

```python
print_lines()
```

函数内部的代码才会真正执行。

---

## 8. 核心知识点

* `print()` 用于向控制台输出内容。
* `print()` 默认会在输出结尾添加换行。
* 多个 `print()` 可以产生多行输出。
* Python 通常按照代码从上到下执行。
* 多个 `print()` 可以写在函数内部。
* 定义函数时需要 `def`、函数名、`()` 和 `:`。
* 函数内部代码需要缩进。
* **定义函数**和**调用函数**是两个不同的操作。

---

# `print_lines()` 编程题解答

## 问题

编写一个名为 `print_lines()` 的 Python 函数，使它依次输出三行：

```text
Line 1
Line 2
Line 3
```

函数不需要接收任何参数。

---

## 答案

```python
def print_lines():
    print("Line 1")
    print("Line 2")
    print("Line 3")
```

如果需要实际运行它：

```python
def print_lines():
    print("Line 1")
    print("Line 2")
    print("Line 3")


print_lines()
```

输出：

```text
Line 1
Line 2
Line 3
```

## 为什么这样写

函数内部有三个 `print()`：

```python
print("Line 1")
print("Line 2")
print("Line 3")
```

每次 `print()` 执行后都会自动换行，所以三个调用依次产生三行输出。

`print_lines()` 没有参数，因此函数定义使用：

```python
def print_lines():
```

调用时同样不需要传入任何内容：

```python
print_lines()
```
