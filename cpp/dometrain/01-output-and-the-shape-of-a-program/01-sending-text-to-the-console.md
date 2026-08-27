# C++ 控制台输出：`std::cout` 与 `<<`

## 1. 最基本的输出

C++ 中可以使用 `std::cout` 向终端（console）输出文本：

```cpp
#include <iostream>

std::cout << "text";
```

例如：

```cpp
std::cout << "Hello World";
```

运行后会输出：

```text
Hello World
```

这条语句可以拆成几个部分理解：

```cpp
std::cout << "text";
```

| 部分          | 含义                    |
| ----------- | --------------------- |
| `std::cout` | 标准输出流，通常连接到终端         |
| `<<`        | 输出/插入运算符，把右边的数据送入左边的流 |
| `"text"`    | 字符串字面量                |
| `;`         | 结束一条 C++ 语句           |

可以把它形象地理解成：

```text
"text"
   ↓
   <<
   ↓
std::cout
   ↓
终端
```

也就是：

> 把 `"text"` 塞进 `std::cout` 这个输出流。

---

# 2. `std::cout` 不是函数

这是初学者很容易混淆的一点。

正确：

```cpp
std::cout << "Hello";
```

错误：

```cpp
std::cout("Hello");
```

因为：

```cpp
std::cout
```

本身不是一个函数。

所以不能像：

```cpp
printf("Hello");
```

这样调用它。

可以粗略理解：

```cpp
std::cout          // 一个对象
printf(...)        // 一个函数调用
```

`std::cout` 是一个 **stream object（流对象）**。

我们通过 `<<` 操作这个对象：

```cpp
std::cout << something;
```

---

# 3. `<<` 是什么？

这里的：

```cpp
<<
```

叫作 **stream insertion operator（流插入运算符）**。

例如：

```cpp
std::cout << 123;
```

表示把整数：

```text
123
```

送入输出流。

同样：

```cpp
std::cout << "Hello";
```

把字符串送进去。

它还可以连续使用：

```cpp
std::cout << "Age: " << 20;
```

可以理解成：

```text
std::cout
    ← "Age: "
    ← 20
```

最终输出：

```text
Age: 20
```

因此 C++ 中很常见这样的代码：

```cpp
int age = 20;

std::cout << "I am " << age << " years old.";
```

输出：

```text
I am 20 years old.
```

---

# 4. `"text"` 是字符串字面量

例如：

```cpp
"Hello World"
```

叫：

**string literal（字符串字面量）**

双引号中的字符会按照你写的内容输出。

例如：

```cpp
std::cout << "Hello   World";
```

中间有三个空格，那么输出也会有三个空格：

```text
Hello   World
```

所以在自动判题题目中：

```text
Output from a C++ program
```

和：

```text
Output  from a C++ program
```

并不完全相同。

---

# 5. 分号 `;` 表示语句结束

例如：

```cpp
std::cout << "Hello";
```

最后的：

```cpp
;
```

表示这条 C++ statement 结束。

需要注意：

> 分号结束的是“语句”，而不是“屏幕上的一行”。

例如：

```cpp
std::cout << "Hello";
std::cout << "World";
```

虽然代码写成了两行，但输出仍然可能是：

```text
HelloWorld
```

因为你没有输出换行字符。

反过来：

```cpp
std::cout << "Hello\nWorld";
```

虽然只有一条 C++ 语句：

```cpp
std::cout << "Hello\nWorld";
```

输出却有两行：

```text
Hello
World
```

因此要区分：

```text
代码中的换行
≠
终端中的换行
```

---

# 6. `std::` 是什么意思？

完整名字是：

```cpp
std::cout
```

这里：

```cpp
std
```

是 C++ 标准库使用的 **namespace（命名空间）**。

而：

```cpp
::
```

叫：

**scope resolution operator（作用域解析运算符）**

所以：

```cpp
std::cout
```

可以理解成：

> `std` 命名空间里面的 `cout`

类似于：

```text
std
├── cout
├── cin
├── string
├── vector
└── ...
```

所以以后你会经常看到：

```cpp
std::cout
std::cin
std::string
std::vector
```

它们的共同意思都是：

> 使用 C++ 标准库 `std` 命名空间中的某个名字。

---

# 7. 为什么需要 `#include <iostream>`？

要使用：

```cpp
std::cout
```

通常需要：

```cpp
#include <iostream>
```

完整例子：

```cpp
#include <iostream>

int main()
{
    std::cout << "Hello";
}
```

`<iostream>` 提供了标准输入/输出流相关的声明，例如：

```cpp
std::cout
std::cin
std::cerr
```

---

# 8. `#include` 不是普通 C++ 语句

注意：

```cpp
#include <iostream>
```

后面没有：

```cpp
;
```

因为 `#include` 属于 **preprocessor directive（预处理指令）**。

而：

```cpp
std::cout << "Hello";
```

才是普通 C++ 语句。

所以：

```cpp
#include <iostream>   // 不需要 ;
```

而：

```cpp
std::cout << "Hello"; // 需要 ;
```

---

# 9. `#include` 可以怎样理解？

初学阶段可以这样理解：

```cpp
#include <iostream>
```

告诉预处理阶段：

> 我要使用 `<iostream>` 头文件中提供的声明。

传统上经常把它解释成：

> 预处理器把头文件内容“展开/包含”到当前文件中。

这个理解对于学习普通 C++ 项目已经足够。

更准确地说，`#include` 是 C/C++ **预处理阶段**的一部分，并不是运行程序时发生的事情。

大概流程可以先记成：

```text
源代码
  ↓
预处理
  ↓
编译
  ↓
汇编
  ↓
链接
  ↓
可执行程序
```

其中：

```cpp
#include <iostream>
```

主要发生在前面的预处理阶段。

---

# 10. 一个完整的普通 C++ 程序

如果我们自己写一个完整程序，通常会是：

```cpp
#include <iostream>

int main()
{
    std::cout << "Hello World";
    return 0;
}
```

可以理解成：

```text
#include <iostream>
        ↓
让我可以使用 std::cout

int main()
        ↓
程序入口

std::cout << "Hello World";
        ↓
输出文字
```

---

# 11. 这道题为什么没有让你写 `main()`？

题目已经提供了：

```cpp
static void printMessage()
```

你只需要修改函数内部：

```cpp
static void printMessage()
{
    // 写这里
}
```

例如：

```cpp
static void printMessage()
{
    std::cout << "Hello";
}
```

这里要特别注意：

> “不要写 `main()`”是这个练习平台的要求，不是 C++ 本身的规则。

普通 C++ 程序通常仍然需要：

```cpp
int main()
```

只是这个练习平台已经在后台提供了自己的 `main()`，然后由它去调用：

```cpp
printMessage();
```

可以把它想象成平台背后大概存在：

```cpp
int main()
{
    printMessage();
}
```

因此你不需要再自己定义一个。

---

# 12. `static` 在这里也不要过度理解

题目给出：

```cpp
static void printMessage()
```

其中：

```cpp
void
```

表示函数不返回一个值。

```cpp
printMessage
```

是函数名字。

```cpp
()
```

表示这个函数没有参数。

这里的 `static` 与练习平台如何组织和调用代码有关。

对于这道题：

> 不需要修改函数签名。

直接把输出代码写进函数里面即可。

以后真正学习 C++ 的 `static` 时，需要单独学习，因为：

```cpp
static
```

在不同位置有不同含义。

例如：

```cpp
static int x;
```

```cpp
static void foo();
```

```cpp
class A {
    static int x;
};
```

它们涉及的概念并不完全相同。

所以现在不要简单记成：

> “C++ 函数都应该写 static”

这是错误的。

这里只是题目已经规定好了。

---

# 13. 如何输出换行？

可以使用：

```cpp
\n
```

例如：

```cpp
std::cout << "Hello\n";
```

或者：

```cpp
std::cout << "Hello" << '\n';
```

也可以看到：

```cpp
std::cout << "Hello" << std::endl;
```

对于普通输出，我更推荐先习惯：

```cpp
std::cout << "Hello\n";
```

或：

```cpp
std::cout << "Hello" << '\n';
```

因为：

```cpp
std::endl
```

除了换行之外，还会执行一次 stream flush，所以它并不只是“换行字符”的另一种写法。

---

# 14. 这道题的自动判题在检查什么？

要求输出：

```text
Output from a C++ program
```

并且不能输出其他东西。

题目设计了三个逐渐严格的测试。

可以理解成：

```text
Test 1
↓
单词基本对不对？

Test 2
↓
大小写对不对？

Test 3
↓
每一个字符是否完全一致？
```

因此如果最终测试失败，需要检查：

```text
Output from a C++ program
```

中的：

* `Output` 的 `O` 是否大写
* `C++` 的 `C` 是否大写
* 每两个单词之间是否只有一个空格
* 最后有没有多余空格
* 有没有额外输出其他文本

例如下面是错误的：

```cpp
std::cout << "output from a C++ program";
```

因为：

```text
output
```

应该是：

```text
Output
```

下面也错误：

```cpp
std::cout << "Output  from a C++ program";
```

因为 `Output` 和 `from` 中间有两个空格。

---

# 15. 本题关于换行的特殊规则

题目说比较答案之前，会移除最后的 newline。

因此：

```cpp
std::cout << "Output from a C++ program";
```

和：

```cpp
std::cout << "Output from a C++ program\n";
```

在这道题中都可以通过。

但是在真实程序或其他 Online Judge 中，不一定存在这种特殊处理。

---

# 16. 最终答案

根据题目的函数签名：

```cpp
static void printMessage()
```

你只需要写：

```cpp
static void printMessage()
{
    std::cout << "Output from a C++ program";
}
```

如果题目编辑器已经给你：

```cpp
static void printMessage()
{
    
}
```

那么你真正需要填写的甚至只有这一行：

```cpp
std::cout << "Output from a C++ program";
```

---

# 核心知识总结

这一节最值得记住的是这几个关系：

```cpp
#include <iostream>
```

让我们可以使用标准输入输出设施。

```cpp
std::cout
```

是标准输出流对象。

```cpp
<<
```

把右边的数据送到左边的输出流。

所以：

```cpp
std::cout << "Hello";
```

可以读成：

> 把 `"Hello"` 输出到 `std::cout`。

多个内容可以连续插入：

```cpp
std::cout << "Age: " << 20 << '\n';
```

整体心智模型可以记成：

```text
                    数据
                     │
                     ▼
"Age: " ──┐
          ├── << ──→ std::cout ──→ Terminal
20 ───────┤
          │
'\n' ─────┘
```

以后学习 C++ 输入输出时，会在这个模型上继续扩展到：

```cpp
std::cin   // 标准输入
std::cerr  // 错误输出
std::ofstream // 文件输出
std::ifstream // 文件输入
```

它们背后的核心思想都是：

**stream（流）**。
