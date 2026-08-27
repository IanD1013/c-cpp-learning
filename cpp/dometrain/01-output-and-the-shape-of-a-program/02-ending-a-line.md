# C++ 中的换行符 `\n` 与 `std::cout <<` 链式输出

## 1. 换行到底是什么？

程序向终端输出文本时，如果希望下一段内容出现在下一行，就需要输出一个特殊字符：

```cpp
\n
```

它叫做 **换行符（newline character）**。

例如：

```cpp
std::cout << "first\nsecond";
```

输出结果：

```text
first
second
```

可以把它理解成：

```text
"first" + 换行 + "second"
```

也就是说，程序先输出：

```text
first
```

遇到 `\n` 后，光标移动到下一行开头，然后继续输出：

```text
second
```

---

# 2. 为什么要写成 `\n`？

在 C++ 字符串字面量中，一些特殊字符不能直接按照普通字符的方式书写，因此 C++ 使用 **转义序列（escape sequence）** 来表示它们。

例如：

```cpp
"\n"
```

这里的反斜杠：

```text
\
```

表示：

> 后面的字符不要按照普通字符解释，而要按照特殊含义解释。

因此：

```cpp
\n
```

并不是两个字符：

```text
\
n
```

而是在字符串中代表 **一个换行字符**。

所以：

```cpp
std::cout << "hello\nworld";
```

不会输出：

```text
hello\nworld
```

而是：

```text
hello
world
```

---

# 3. 什么是 Escape Sequence？

`\n` 属于 C++ 的 **转义序列（escape sequence）**。

常见的转义序列包括：

| 转义序列 | 含义          |
| ---- | ----------- |
| `\n` | 换行          |
| `\t` | Tab         |
| `\\` | 输出一个反斜杠 `\` |
| `\"` | 输出双引号 `"`   |
| `\'` | 输出单引号 `'`   |

例如：

```cpp
std::cout << "Name:\tAlice";
```

其中：

```cpp
\t
```

表示一个 Tab。

再例如：

```cpp
std::cout << "\"Hello\"";
```

输出：

```text
"Hello"
```

因为：

```cpp
\"
```

表示字符串中的普通双引号，而不是字符串结束符。

---

# 4. `\n` 放在哪里非常重要

换行发生的位置，取决于 `\n` 在字符串中的位置。

例如：

```cpp
std::cout << "one\ntwo";
```

输出：

```text
one
two
```

因为换行发生在：

```text
one
```

之后。

---

如果写成：

```cpp
std::cout << "one" << "\ntwo";
```

结果仍然是：

```text
one
two
```

因为第二个字符串：

```cpp
"\ntwo"
```

一开始就是换行符。

执行过程相当于：

```text
输出 one
↓
输出 \n
↓
输出 two
```

---

# 5. `<<` 本身不会换行

这是一个很重要的概念。

很多初学者看到：

```cpp
std::cout << "one" << "two";
```

可能会误以为两个 `<<` 会让内容分开。

实际上不会。

输出结果是：

```text
onetwo
```

因为：

```cpp
<<
```

本身 **没有换行功能**。

它只是：

> 把右边的数据发送给左边的输出流。

例如：

```cpp
std::cout << "Hello";
```

可以理解成：

```text
把 "Hello" 发送给 std::cout
```

而：

```cpp
std::cout << "Hello" << "World";
```

相当于：

```text
先发送 "Hello"
再发送 "World"
```

因此最终结果是：

```text
HelloWorld
```

如果需要换行，必须显式写：

```cpp
\n
```

例如：

```cpp
std::cout << "Hello\nWorld";
```

或者：

```cpp
std::cout << "Hello" << "\n" << "World";
```

两者都输出：

```text
Hello
World
```

---

# 6. 为什么可以连续写多个 `<<`？

C++ 中经常看到这种代码：

```cpp
std::cout << "one" << "\n" << "two\n";
```

这里连续使用了三个：

```cpp
<<
```

之所以可以这样写，是因为：

```cpp
std::cout << something
```

执行完成后，会返回这个输出流本身。

因此：

```cpp
std::cout << "one"
```

执行之后，结果仍然可以继续使用：

```cpp
<< "\n"
```

然后又可以继续：

```cpp
<< "two\n"
```

所以：

```cpp
std::cout << "one" << "\n" << "two\n";
```

可以概念性地理解成：

```text
std::cout
    → 输出 "one"
    → 输出 "\n"
    → 输出 "two\n"
```

输出顺序按照代码从左到右进行。

---

# 7. 什么叫 Chained Insertion？

这种写法：

```cpp
std::cout << A << B << C;
```

通常称为：

**chained insertion**

即：

**链式插入 / 链式输出**

因为多个输出操作连接在同一个表达式中。

例如：

```cpp
std::cout << "Name: " << "Alice" << "\n";
```

等价于依次输出：

```text
"Name: "
"Alice"
"\n"
```

最终：

```text
Name: Alice
```

然后换行。

---

# 8. 一条 chained statement 只需要一个分号

例如：

```cpp
std::cout << "one"
          << "\n"
          << "two\n";
```

虽然代码视觉上分成了三行，但这仍然是 **一条 C++ 语句**。

真正结束语句的是：

```cpp
;
```

所以只需要最后一个分号。

当然，也可以写成：

```cpp
std::cout << "one" << "\n" << "two\n";
```

两者完全等价。

代码换行和程序输出换行是两回事。

例如：

```cpp
std::cout
    << "hello"
    << "world";
```

源代码写成了三行，但输出仍然是：

```text
helloworld
```

因为字符串中没有：

```cpp
\n
```

---

# 9. 源代码换行 ≠ 输出换行

这是非常容易混淆的一点。

例如：

```cpp
std::cout << "hello"
          << "world";
```

源代码虽然换了一行，但输出：

```text
helloworld
```

不会自动出现换行。

真正控制输出换行的是：

```cpp
\n
```

例如：

```cpp
std::cout << "hello\n"
          << "world";
```

输出：

```text
hello
world
```

因此一定要区分：

```text
代码文件中的换行
```

和：

```text
程序输出中的换行
```

---

# 10. `\n` 可以放在不同的字符串中

例如以下几种写法都可以产生相同结果：

## 写法 1

```cpp
std::cout << "one\ntwo\nthree";
```

## 写法 2

```cpp
std::cout << "one\n"
          << "two\n"
          << "three";
```

## 写法 3

```cpp
std::cout << "one"
          << "\n"
          << "two"
          << "\n"
          << "three";
```

它们都会输出：

```text
one
two
three
```

区别只是：

> `\n` 被放在哪一个字符串字面量里。

---

# 11. `\n` 放在字符串结尾是什么意思？

例如：

```cpp
std::cout << "Line one\n";
```

执行之后：

```text
Line one
```

并且光标已经进入下一行。

所以：

```cpp
std::cout << "Line one\n"
          << "Line two";
```

输出：

```text
Line one
Line two
```

---

# 12. `\n` 放在字符串开头是什么意思？

例如：

```cpp
std::cout << "Line one"
          << "\nLine two";
```

效果同样是：

```text
Line one
Line two
```

因为：

```cpp
"\nLine two"
```

表示：

```text
先换行
然后输出 Line two
```

---

# 13. `std::cout` 是什么？

```cpp
std::cout
```

是 C++ 标准库提供的 **标准输出流**。

通常用于向终端输出内容。

要使用它，一般需要：

```cpp
#include <iostream>
```

例如：

```cpp
#include <iostream>

int main()
{
    std::cout << "Hello";
}
```

---

# 14. 为什么这里要求写完整的 `std::cout`？

有些代码会写：

```cpp
using namespace std;
```

然后：

```cpp
cout << "Hello";
```

这在正常 C++ 中是合法的。

但是本题明确要求：

```cpp
std::cout
```

而不是：

```cpp
cout
```

原因并不是 C++ 语法，而是 **grader 会直接检查源代码内容**。

也就是说，它可能检查你的代码里面是否真的出现：

```cpp
std::cout
```

因此即使：

```cpp
cout
```

功能完全正确，也可能无法通过测试。

这是自动评分题中非常常见的一点：

> 不仅要让程序行为正确，还必须满足题目指定的代码形式。

---

# 15. 本题为什么不能使用 `std::endl`？

C++ 中还有一种常见换行方式：

```cpp
std::endl
```

例如：

```cpp
std::cout << "hello" << std::endl;
```

也会换行。

但是本题明确要求：

> 换行必须来自字符串字面量中的 `\n`。

因此不能写：

```cpp
std::cout << "Line one" << std::endl;
```

也不能写：

```cpp
endl
```

或者：

```cpp
std::endl
```

---

# 16. `\n` 和 `std::endl` 的区别

虽然两者通常都会让下一次输出出现在下一行，但并不完全相同。

```cpp
"\n"
```

主要表示：

```text
输出一个换行字符
```

而：

```cpp
std::endl
```

通常表示：

```text
输出换行
+
flush 输出缓冲区
```

也就是：

```cpp
std::endl
```

除了换行，还会强制刷新输出流。

因此普通情况下如果只是需要换行：

```cpp
\n
```

往往更简单，也可能更高效。

不过本题并不是在考性能，而是在明确练习：

```cpp
\n
```

---

# 17. 本题的输出要求

函数：

```cpp
static void printThreeLines()
```

必须按照顺序输出：

```text
Line one
Line two
Line three
```

因此三个短语之间需要两个换行符。

逻辑结构为：

```text
Line one
   ↓ \n
Line two
   ↓ \n
Line three
```

即：

```text
"Line one\nLine two\nLine three"
```

---

# 18. 为什么需要两个 `\n`？

三行文字之间只有两个“边界”：

```text
Line one
---------
Line two
---------
Line three
```

因此：

```text
3 行 → 2 个内部换行符
```

例如：

```cpp
"Line one\nLine two\nLine three"
```

正好有两个：

```cpp
\n
```

---

# 19. 最后一行之后是否需要 `\n`？

以下两个输出：

```cpp
"Line one\nLine two\nLine three"
```

和：

```cpp
"Line one\nLine two\nLine three\n"
```

从严格意义上来说并不完全相同。

第二种结尾还有一个换行符。

但是题目说明 grader 在比较之前会移除末尾换行，因此这两种写法在本题中都会被认为正确。

不过如果没有特殊要求，一般写成：

```cpp
"Line three"
```

即可。

---

# 20. 本题最容易出现的错误

## 错误 1：忘记换行

```cpp
std::cout << "Line one"
          << "Line two"
          << "Line three";
```

输出：

```text
Line oneLine twoLine three
```

---

## 错误 2：少一个 `\n`

```cpp
std::cout << "Line one\n"
          << "Line two"
          << "Line three";
```

输出：

```text
Line one
Line twoLine three
```

---

## 错误 3：使用 `std::endl`

```cpp
std::cout << "Line one" << std::endl;
```

虽然输出效果可能正确，但违反题目要求。

---

## 错误 4：写 `cout` 而不是 `std::cout`

```cpp
cout << "Line one\n";
```

即使程序可以编译，本题 grader 仍可能判错，因为它明确检查：

```cpp
std::cout
```

---

## 错误 5：修改 `main`

题目说明：

```text
main is the runner's to define
```

即测试环境负责提供 `main()`。

因此你只需要实现：

```cpp
static void printThreeLines()
```

不要自己额外定义：

```cpp
int main()
```

否则可能和测试系统提供的 `main()` 冲突。

---

# 21. 核心知识总结

这道题主要在练习四件事：

```text
字符串字面量
        │
        ▼
转义序列 \n
        │
        ▼
std::cout 输出流
        │
        ▼
<< 链式输出
```

最重要的几个规则：

```cpp
"\n"
```

表示一个换行字符。

```cpp
std::cout << "A" << "B";
```

输出：

```text
AB
```

`<<` 本身不会换行。

```cpp
std::cout << "A\n" << "B";
```

输出：

```text
A
B
```

多个 `<<` 可以连续连接：

```cpp
std::cout << A << B << C;
```

并按照从左到右的顺序输出。

---

# 22. 一句话记忆

> `<<` 负责“继续输出”，`\n` 负责“换到下一行”。

因此看到：

```cpp
std::cout << "one\n" << "two\n" << "three";
```

可以直接在脑中翻译成：

```text
输出 one
换行
输出 two
换行
输出 three
```
