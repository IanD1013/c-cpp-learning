# C++ 函数返回值：`return`、`std::string` 与 `void`

这一部分的核心知识是：

> 函数不一定要把结果打印到控制台，也可以把结果“返回”给调用它的代码。

前面的很多练习可能都是：

```cpp
std::cout << "hello";
```

也就是：

> 把结果打印出来。

但现在要学习另一种更重要的方式：

```cpp
return "hello";
```

也就是：

> 把结果交还给调用者。

这两件事情完全不同。

---

# 1. 什么是函数的返回值？

看下面这个函数：

```cpp
static std::string greeting()
{
    return "hello";
}
```

这里：

```cpp
std::string
```

写在函数名 `greeting` 前面，它叫做：

> 返回类型（return type）

它表示：

> 这个函数执行完成后，会返回一个 `std::string` 类型的值。

所以：

```cpp
static std::string greeting()
```

可以理解为：

> 定义一个叫 `greeting` 的函数，它最终会给调用者一个字符串。

---

# 2. `void` 和 `std::string` 返回类型有什么区别？

之前可能见过：

```cpp
static void printMessage()
{
    std::cout << "hello";
}
```

这里的：

```cpp
void
```

表示：

> 这个函数不返回值。

它可以做一些事情，例如：

* 打印内容
* 修改变量
* 调用其他函数

但它不会把一个结果值交还给调用者。

---

而：

```cpp
static std::string greeting()
{
    return "hello";
}
```

这里的：

```cpp
std::string
```

表示：

> 函数必须返回一个字符串。

可以简单对比：

```cpp
void
```

表示：

```text
做事情，但不给我一个结果值
```

而：

```cpp
std::string
```

表示：

```text
执行以后，给我一个字符串结果
```

---

# 3. 打印和返回不是一回事

这是这类题最重要的区别。

## 打印

```cpp
static void printMessage()
{
    std::cout << "hello";
}
```

这里 `"hello"` 被发送到：

```text
控制台
```

调用者并没有拿到 `"hello"` 这个值。

---

## 返回

```cpp
static std::string buildMessage()
{
    return "hello";
}
```

这里 `"hello"` 不会自动显示到控制台。

它被：

```text
返回给调用这个函数的代码
```

例如：

```cpp
std::string result = buildMessage();
```

那么：

```cpp
result
```

里面就会保存：

```text
hello
```

---

# 4. 一个直观的理解

可以把函数想象成一台机器。

例如：

```cpp
static std::string greeting()
{
    return "hello";
}
```

调用：

```cpp
greeting();
```

就像：

```text
调用函数
   ↓
greeting()
   ↓
产生 "hello"
   ↓
return
   ↓
把 "hello" 交还给调用者
```

因此：

```cpp
std::string result = greeting();
```

执行之后：

```cpp
result == "hello"
```

---

# 5. `return` 有两个作用

`return` 不只是“提供结果”，它实际上同时完成两件事。

## 作用一：返回一个值

例如：

```cpp
return "hello";
```

就是：

> 把 `"hello"` 交还给函数调用者。

---

## 作用二：立即结束函数

执行到：

```cpp
return
```

以后，函数会立即结束。

例如：

```cpp
static std::string example()
{
    return "first";

    return "second";
}
```

第二个：

```cpp
return "second";
```

永远不会执行。

因为执行：

```cpp
return "first";
```

以后，函数已经离开了。

可以理解为：

```text
return = 给出结果 + 离开函数
```

---

# 6. 为什么两个 `return` 不能连续返回两个值？

例如：

```cpp
static std::string example()
{
    return "hello";
    return "world";
}
```

有些初学者可能以为会依次返回：

```text
hello
world
```

实际上不会。

执行过程是：

```text
进入函数
   ↓
return "hello";
   ↓
返回 hello
   ↓
函数结束
```

所以：

```cpp
return "world";
```

根本执行不到。

---

# 7. `return` 和 `std::cout` 的本质区别

比较：

```cpp
std::cout << "hello";
```

和：

```cpp
return "hello";
```

虽然它们都出现了 `"hello"`，但作用完全不同。

| 写法                      | 作用         |
| ----------------------- | ---------- |
| `std::cout << "hello";` | 打印到控制台     |
| `return "hello";`       | 把字符串返回给调用者 |

例如：

```cpp
static std::string greeting()
{
    std::cout << "hello";
    return "world";
}
```

这里：

控制台会显示：

```text
hello
```

但函数实际返回的是：

```text
world
```

所以：

```cpp
std::string x = greeting();
```

最后：

```cpp
x == "world"
```

而不是：

```cpp
x == "hello"
```

这是做自动测试题时非常容易混淆的地方。

---

# 8. `std::string` 是什么？

`std::string` 是 C++ 标准库中用于保存文本的类型。

例如：

```cpp
std::string name = "Alice";
```

可以理解为：

```text
name 是一个字符串变量
它保存了 Alice
```

与数字类似：

```cpp
int age = 20;
```

这里：

```cpp
int
```

用于保存整数。

而：

```cpp
std::string
```

用于保存字符串。

---

# 9. 字符串也可以像数字一样被函数返回

例如数字函数：

```cpp
static int getNumber()
{
    return 42;
}
```

调用：

```cpp
int number = getNumber();
```

得到：

```cpp
number == 42
```

字符串也是一样：

```cpp
static std::string getMessage()
{
    return "hello";
}
```

调用：

```cpp
std::string message = getMessage();
```

得到：

```cpp
message == "hello"
```

所以可以把：

```cpp
std::string
```

理解成：

> 一种可以作为函数输入、变量和返回值使用的文本数据类型。

---

# 10. 返回的值必须与返回类型匹配

如果函数声明为：

```cpp
static std::string greeting()
```

那么它应该返回字符串。

例如：

```cpp
return "hello";
```

是合理的。

因为：

```cpp
"hello"
```

是字符串字面量，可以用于创建一个 `std::string` 返回值。

---

类似地：

```cpp
static int getAge()
{
    return 20;
}
```

因为返回类型是：

```cpp
int
```

所以应该返回整数。

---

可以把函数声明理解成一份“合同”：

```cpp
static std::string greeting()
```

意思是：

> 我承诺：调用这个函数之后，你会得到一个字符串。

因此函数内部必须遵守这个承诺。

---

# 11. 什么是字符串字面量？

例如：

```cpp
"hello"
```

叫：

> 字符串字面量（string literal）

它表示源码中直接写出来的一段文本。

例如：

```cpp
return "hello";
```

这里：

```cpp
"hello"
```

就是字符串字面量。

在：

```cpp
std::string
```

返回函数中，它可以作为返回值使用。

---

# 12. 为什么需要 `#include <string>`？

如果代码中使用：

```cpp
std::string
```

通常需要：

```cpp
#include <string>
```

例如：

```cpp
#include <string>

static std::string greeting()
{
    return "hello";
}
```

原因是：

```cpp
std::string
```

是在 C++ 标准库的：

```cpp
<string>
```

头文件中声明的。

所以：

```cpp
#include <string>
```

相当于告诉编译器：

> 我要使用标准库中的字符串类型。

---

# 13. `<string>` 和 `<iostream>` 是不同的

以前打印内容时可能经常写：

```cpp
#include <iostream>
```

因为：

```cpp
std::cout
```

来自：

```cpp
<iostream>
```

而：

```cpp
std::string
```

对应：

```cpp
<string>
```

可以这样记：

```cpp
#include <iostream>
```

主要用于：

```cpp
std::cout
std::cin
```

而：

```cpp
#include <string>
```

主要用于：

```cpp
std::string
```

---

# 14. 这道题为什么特别强调不要打印？

因为自动测试检查的是：

> 函数返回的值。

而不是：

> 控制台上显示了什么。

例如错误写法：

```cpp
static std::string buildMessage()
{
    std::cout << "Returned instead of printed";
    return "";
}
```

控制台可能看起来完全正确：

```text
Returned instead of printed
```

但 grader 检查：

```cpp
buildMessage()
```

返回了什么。

这里返回的是：

```text
空字符串
```

因此仍然失败。

---

正确方式是：

```cpp
static std::string buildMessage()
{
    return "Returned instead of printed";
}
```

---

# 15. 返回值中的空格和换行也属于数据

假设标准答案是：

```text
Returned instead of printed
```

那么：

```cpp
return "Returned instead of printed";
```

正确。

但下面这些都可能错误：

```cpp
return "Returned instead of printed ";
```

最后多一个空格。

---

```cpp
return " Returned instead of printed";
```

开头多一个空格。

---

```cpp
return "Returned instead of printed\n";
```

最后多一个换行符。

它们肉眼看起来可能差不多，但字符串实际内容不同。

---

# 16. 自动测试通常是逐字符比较

假设期望值是：

```cpp
"Returned instead of printed"
```

grader 可能进行类似：

```cpp
actual == expected
```

的比较。

这意味着：

* 大写必须一样
* 小写必须一样
* 空格数量必须一样
* 标点必须一样
* 不能多换行
* 不能多 Tab
* 不能多空格

所以：

```text
Returned instead of printed
```

和：

```text
returned instead of printed
```

不同。

因为：

```text
R ≠ r
```

同样：

```text
Returned  instead of printed
```

中间两个空格，也不同于：

```text
Returned instead of printed
```

---

# 17. 为什么几个测试可能一起通过或一起失败？

如果多个测试都只是检查同一个返回值：

```cpp
buildMessage()
```

是否等于：

```text
Returned instead of printed
```

那么测试可能都会检查同一件事情。

因此可能出现：

```text
3 个全绿
```

或者：

```text
3 个全红
```

而不像之前某些输出题一样：

```text
一个测试检测大小写
一个测试检测空格
一个测试检测换行
```

所以这里不要试图根据“哪个测试红了”猜字符问题。

最可靠的方法就是：

> 精确返回题目指定的字符串。

---

# 18. `return "";` 是什么意思？

starter code 可能是：

```cpp
static std::string buildMessage()
{
    return "";
}
```

其中：

```cpp
""
```

叫：

> 空字符串（empty string）

它长度为：

```text
0
```

也就是里面没有任何字符。

这个通常只是练习模板中的占位返回值。

你的任务就是把：

```cpp
return "";
```

替换成真正要求返回的内容。

---

# 19. 不要自行添加 `main()`

如果题目说明：

```text
main belongs to the runner
```

意思是：

> 测试系统已经提供了 `main()`。

所以你只需要实现指定函数：

```cpp
static std::string buildMessage()
```

不要再写：

```cpp
int main()
```

否则可能与测试系统自己的 `main()` 冲突。

---

# 20. 这部分最重要的思维模型

遇到函数题时，先看返回类型。

如果是：

```cpp
void
```

通常意味着：

```text
这个函数不返回一个值
```

如果是：

```cpp
int
```

意味着：

```text
最终要返回整数
```

如果是：

```cpp
std::string
```

意味着：

```text
最终要返回字符串
```

然后再看题目到底要求：

```text
print
```

还是：

```text
return
```

这两个词非常重要。

---

# 核心总结

## 返回类型

```cpp
static std::string buildMessage()
```

其中：

```cpp
std::string
```

表示函数返回字符串。

---

## 返回字符串

```cpp
return "hello";
```

表示：

> 把 `"hello"` 返回给调用者，然后立即结束函数。

---

## 打印字符串

```cpp
std::cout << "hello";
```

表示：

> 把 `"hello"` 显示在控制台。

这不是返回值。

---

## 最重要的区别

```cpp
std::cout << "hello";
```

是：

```text
打印
```

而：

```cpp
return "hello";
```

是：

```text
返回
```

在自动测试中，如果题目检查返回值：

> 控制台看起来正确并不代表答案正确。

必须让函数真正 `return` 正确的值。
