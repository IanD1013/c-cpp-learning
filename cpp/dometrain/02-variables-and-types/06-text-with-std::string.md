# C++ `std::string` 字符串拼接：使用 `+` 连接文本

## 1. 什么是字符串拼接

在 C++ 中，把两段文本连接成一段新的文本，叫做 **字符串拼接（concatenation）**。

对于 `std::string`，可以使用 `+` 运算符进行拼接。

```cpp
#include <string>

std::string first = "ada";
std::string last = "lovelace";

std::string full = first + " " + last;
```

此时：

```cpp
full == "ada lovelace"
```

可以把这个表达式理解为：

```text
"ada" + " " + "lovelace"
  ↓
"ada lovelace"
```

---

# 2. `+` 不会修改原字符串

例如：

```cpp
std::string first = "ada";
std::string last = "lovelace";

std::string full = first + " " + last;
```

执行之后：

```cpp
first == "ada"
last == "lovelace"
full == "ada lovelace"
```

`first` 和 `last` 都没有发生变化。

原因是：

> `std::string` 的 `+` 会产生一个新的字符串，而不是修改原来的字符串。

概念上可以理解成：

```cpp
std::string temp = first + " ";
std::string full = temp + last;
```

当然，实际代码没有必要写成这样。

---

# 3. `std::string` 可以和哪些东西使用 `+`

对于这类字符串拼接表达式，最重要的规则是：

> `+` 的两边至少要有一边是 `std::string`。

常见的合法组合包括：

```cpp
std::string + std::string
std::string + "字符串字面量"
"字符串字面量" + std::string
std::string + char
char + std::string
```

例如：

```cpp
std::string name = "Ada";

std::string a = name + " Lovelace";
std::string b = "Hello " + name;
std::string c = name + '!';
```

结果分别是：

```text
Ada Lovelace
Hello Ada
Ada!
```

---

# 4. 为什么 `"ada" + " "` 不可以？

下面这段代码不能按照字符串拼接的意思工作：

```cpp
"ada" + " "
```

原因是它们都只是 **字符串字面量（string literal）**，而不是 `std::string`。

也就是说：

```cpp
"ada"
" "
```

并不是两个 `std::string` 对象。

因此，没有 `std::string` 参与时，不能使用这里所说的 `std::string` 拼接规则。

---

## 正确做法

让其中至少一边成为 `std::string`：

```cpp
std::string("ada") + " "
```

或者：

```cpp
std::string first = "ada";

std::string result = first + " ";
```

---

# 5. 为什么 `first + " " + last` 可以？

观察：

```cpp
std::string full = first + " " + last;
```

`+` 是从左向右结合的，因此可以理解成：

```cpp
(first + " ") + last
```

第一步：

```cpp
first + " "
```

其中：

```cpp
first
```

是一个：

```cpp
std::string
```

所以这一部分合法。

而且它产生一个新的 `std::string`：

```text
"ada "
```

然后再进行：

```cpp
std::string("ada ") + last
```

两边都有 `std::string`，当然也合法。

最终结果：

```text
ada lovelace
```

---

# 6. 字符串字面量和 `std::string` 不完全相同

例如：

```cpp
"hello"
```

这是一个 **字符串字面量**。

而：

```cpp
std::string text = "hello";
```

这里的：

```cpp
text
```

才是一个 `std::string` 对象。

虽然我们平时都把它们理解为“字符串”，但从 C++ 类型系统来看，它们不是同一种类型。

因此下面两个表达式的情况不同：

```cpp
std::string text = "hello";

text + " world";       // 可以
"hello" + " world";    // 不可以这样进行 std::string 拼接
```

第一种表达式有 `std::string` 参与。

第二种没有。

---

# 7. `char` 也可以和 `std::string` 拼接

单个字符使用单引号：

```cpp
'A'
'!'
' '
```

类型是：

```cpp
char
```

例如：

```cpp
std::string name = "Ada";

std::string result = name + '!';
```

结果：

```text
Ada!
```

所以：

```cpp
std::string + char
```

也是合法的。

---

# 8. 注意 `' '` 和 `" "` 的区别

这两个非常容易混淆：

```cpp
' '
```

表示一个字符：

```cpp
char
```

而：

```cpp
" "
```

表示字符串字面量。

因此：

```cpp
std::string name = "Ada";

name + ' ';
```

和：

```cpp
name + " ";
```

都可以工作。

但它们的类型不同。

可以简单记成：

```text
'A'     → char
"A"     → 字符串字面量
```

---

# 9. 为什么要写 `#include <string>`

使用：

```cpp
std::string
```

时，应当显式包含：

```cpp
#include <string>
```

例如：

```cpp
#include <string>

std::string name = "Ada";
```

即使某些情况下：

```cpp
#include <iostream>
```

可能间接让程序碰巧能够使用 `std::string`，也不应该依赖这种行为。

正确习惯是：

> 使用哪个标准库组件，就包含声明该组件的对应头文件。

因此：

```cpp
std::string
```

对应：

```cpp
#include <string>
```

而：

```cpp
std::cout
```

通常对应：

```cpp
#include <iostream>
```

例如：

```cpp
#include <iostream>
#include <string>
```

---

# 10. 引用参数：`const std::string &`

题目给出的函数签名是：

```cpp
static std::string joinWithSpace(
    const std::string &first,
    const std::string &second
)
```

这里：

```cpp
const std::string &first
```

可以拆成三个部分理解。

---

## `std::string`

参数是一个字符串。

---

## `&`

`&` 表示引用。

也就是说，函数可以直接引用调用者传入的字符串，而不是先完整复制一份。

对于较大的对象，例如：

```cpp
std::string
```

这样可以避免不必要的复制。

---

## `const`

`const` 表示函数不允许通过这个参数修改原字符串。

例如：

```cpp
const std::string &first
```

意味着：

```text
使用原来的字符串
但不能修改它
```

因此：

```cpp
const std::string &
```

是 C++ 中非常常见的“只读传参”方式。

可以暂时把它记成：

> **不复制，并且不允许修改原对象。**

---

# 11. `joinWithSpace` 应该做什么

函数：

```cpp
joinWithSpace(first, second)
```

需要返回：

```text
first + 一个空格 + second
```

例如：

```cpp
joinWithSpace("hello", "world")
```

应该得到：

```text
hello world
```

代码的核心表达式就是：

```cpp
first + " " + second
```

---

# 12. 空字符串也不会取消中间的空格

这一题有一个很重要的边界条件：

> 无论两边字符串是否为空，都必须插入 **恰好一个空格**。

例如：

```cpp
joinWithSpace("alpha", "beta")
```

得到：

```text
"alpha beta"
```

如果：

```cpp
joinWithSpace("", "beta")
```

那么按照：

```cpp
first + " " + second
```

计算：

```text
"" + " " + "beta"
```

结果实际上是：

```text
" beta"
```

注意 `beta` 前面存在一个空格。

类似地：

```cpp
joinWithSpace("alpha", "")
```

结果是：

```text
"alpha "
```

末尾有一个空格。

如果：

```cpp
joinWithSpace("", "")
```

那么：

```text
"" + " " + ""
```

结果是：

```text
" "
```

也就是一个长度为 `1` 的字符串，其中只有一个空格。

---

# 13. 不要自动删除空格

题目会对返回结果进行精确比较。

因此：

```text
"hello world"
```

和：

```text
"hello world "
```

是不同的。

下面这些都可能导致测试失败：

```text
少一个空格
多一个空格
删除开头空格
删除结尾空格
自动 trim
```

这道题要求的规则非常简单：

```cpp
first + " " + second
```

不要再做额外处理。

---

# 14. 非 ASCII 字符也可以正常参与拼接

例如：

```cpp
std::string first = "café";
std::string second = "noir";

std::string result = first + " " + second;
```

结果：

```text
café noir
```

这里 `std::string` 本质上保存的是字节序列。

字符串拼接只是把：

```text
左边的字节
+
中间空格
+
右边的字节
```

连接起来。

它不会主动修改这些字符。

---

# 15. 这道题真正想考什么

这道题表面上只是让你连接两个字符串，但实际上同时考察几个重要的 C++ 基础知识：

```text
1. std::string 的基本使用
2. 使用 + 进行字符串拼接
3. 字符串字面量和 std::string 的区别
4. #include <string>
5. const std::string & 参数
6. return 返回字符串
7. 精确处理空格
```

其中最核心的代码其实只有：

```cpp
return first + " " + second;
```

---

# 16. 核心记忆

可以把这一节压缩成下面几句话：

```text
std::string 可以使用 + 拼接文本。

std::string a = "hello";
std::string b = "world";

a + " " + b
→ "hello world"
```

`+` 不会修改原来的字符串，而是产生一个新的字符串。

进行这类拼接时，每个 `+` 至少要有一边能够参与 `std::string` 的拼接运算。

使用：

```cpp
std::string
```

应显式包含：

```cpp
#include <string>
```

常见的字符串只读参数写法是：

```cpp
const std::string &value
```

表示：

```text
不复制原字符串
+
不允许修改原字符串
```

最典型的字符串拼接：

```cpp
return first + " " + second;
```
