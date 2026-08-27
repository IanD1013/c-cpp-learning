# C++ `std::cout`：如何输出“标签 + 数值”

## 1. `<<` 不会自动添加空格

在 C++ 中，`std::cout` 使用 `<<` 把内容依次写入输出流：

```cpp
std::cout << "Total:" << 7;
```

输出：

```text
Total:7
```

注意：

```cpp
<<
```

只是把每一部分按照顺序输出，并不会自动添加：

* 空格
* 逗号
* 冒号
* 换行
* 任何其他分隔符

因此：

```cpp
std::cout << "Hello" << "World";
```

输出的是：

```text
HelloWorld
```

而不是：

```text
Hello World
```

---

## 2. 想要空格，就必须自己写进去

如果希望输出：

```text
Total: 7
```

那么空格通常直接写在字符串中：

```cpp
std::cout << "Total: " << 7;
```

注意这里：

```cpp
"Total: "
```

在冒号 `:` 后面有一个空格。

所以输出过程实际上可以理解为：

```text
"Total: " + 7
```

最终得到：

```text
Total: 7
```

这个空格来自字符串：

```cpp
"Total: "
```

而不是数字 `7`，也不是 `<<` 自动生成的。

---

## 3. 字符串和数字可以直接混合输出

假设有一个变量：

```cpp
int score = 42;
```

可以直接写：

```cpp
std::cout << "Score: " << score;
```

输出：

```text
Score: 42
```

不需要先把 `score` 转换成字符串。

也就是说，不需要：

```cpp
std::to_string(score)
```

因为 `std::cout` 本身就知道如何输出整数。

例如：

```cpp
int age = 20;

std::cout << "Age: " << age;
```

输出：

```text
Age: 20
```

---

## 4. `std::cout` 会自动把整数转换成可显示的数字

对于：

```cpp
int number = -5;

std::cout << number;
```

输出：

```text
-5
```

`std::cout` 会负责把整数值转换成对应的十进制字符。

例如：

```cpp
std::cout << 0;
```

输出：

```text
0
```

```cpp
std::cout << 1000000;
```

输出：

```text
1000000
```

默认情况下，不会自动加千位分隔符，因此不会输出：

```text
1,000,000
```

---

## 5. 负号也是数字输出的一部分

如果：

```cpp
int total = -5;
```

那么：

```cpp
std::cout << "Total: " << total;
```

会输出：

```text
Total: -5
```

不需要自己判断：

```cpp
if (total < 0)
```

也不需要手动输出 `-`。

`std::cout` 会自动处理整数的正负号。

---

## 6. 使用 `\n` 换行

通常一行输出完成后，可以在最后加：

```cpp
"\n"
```

例如：

```cpp
std::cout << "Score: " << score << "\n";
```

如果：

```cpp
score == 42
```

那么输出：

```text
Score: 42
```

然后光标移动到下一行。

可以把整个语句理解为三个部分：

```cpp
std::cout
    << "Score: "
    << score
    << "\n";
```

依次输出：

```text
Score: 
```

然后：

```text
42
```

然后：

```text
换行
```

---

## 7. `<<` 可以连续连接很多内容

下面是一条完整的输出语句：

```cpp
std::cout << "Score: " << score << "\n";
```

虽然里面出现了多个 `<<`：

```cpp
<< "Score: "
<< score
<< "\n"
```

但它们仍然属于同一条 C++ 语句，所以最后只需要一个：

```cpp
;
```

也可以为了可读性写成：

```cpp
std::cout
    << "Score: "
    << score
    << "\n";
```

效果完全相同。

---

## 8. 输出变量时，不要把变量名放进引号

假设：

```cpp
int total = 42;
```

正确：

```cpp
std::cout << "Total: " << total;
```

输出：

```text
Total: 42
```

错误：

```cpp
std::cout << "Total: total";
```

输出：

```text
Total: total
```

因为：

```cpp
"total"
```

是字符串字面量。

只要内容放在：

```cpp
"..."
```

里面，C++ 就会把它当成固定文字，而不是变量。

因此：

```cpp
<< total
```

表示：

> 输出变量 `total` 当前保存的值。

而：

```cpp
<< "total"
```

表示：

> 输出固定文字 `total`。

这是初学 C++ 时非常重要的区别。

---

## 9. `std::cout` 中的 `std::` 是什么？

标准写法是：

```cpp
std::cout
```

其中：

```cpp
std
```

是 C++ 标准库使用的命名空间。

而：

```cpp
cout
```

属于这个命名空间。

所以完整名称是：

```cpp
std::cout
```

某些代码中可能看到：

```cpp
using namespace std;
```

然后写：

```cpp
cout << "Hello";
```

但如果题目明确要求使用：

```cpp
std::cout
```

就应该直接写完整形式：

```cpp
std::cout
```

尤其是在自动评测题中，测试程序有时甚至会直接检查源码里是否出现了某些内容。

---

## 10. `std::cout` 需要 `<iostream>`

要使用：

```cpp
std::cout
```

通常文件中需要：

```cpp
#include <iostream>
```

例如：

```cpp
#include <iostream>

int main()
{
    std::cout << "Hello\n";
}
```

但是如果题目的 starter code 已经给出了：

```cpp
#include <iostream>
```

就不需要重复添加。

---

# 核心规律

看到这种输出要求：

```text
Total: 42
```

可以把它拆成：

```text
固定文字 + 变量值 + 换行
```

对应 C++：

```cpp
std::cout << "Total: " << total << "\n";
```

其中：

```cpp
"Total: "
```

负责固定文字和冒号后的空格；

```cpp
total
```

负责输出变量中的数字；

```cpp
"\n"
```

负责换行。

可以记住这个常见模板：

```cpp
std::cout << "标签: " << 变量 << "\n";
```

例如：

```cpp
std::cout << "Age: " << age << "\n";
std::cout << "Score: " << score << "\n";
std::cout << "Price: " << price << "\n";
std::cout << "Total: " << total << "\n";
```

---

# 最重要的几个易错点

### 错误 1：忘记冒号后的空格

```cpp
std::cout << "Total:" << total;
```

会得到：

```text
Total:42
```

而不是：

```text
Total: 42
```

应该写：

```cpp
std::cout << "Total: " << total;
```

---

### 错误 2：把变量写进字符串里

错误：

```cpp
std::cout << "Total: total";
```

这里输出的是单词：

```text
total
```

而不是变量的值。

正确：

```cpp
std::cout << "Total: " << total;
```

---

### 错误 3：没有必要使用 `to_string`

没必要写：

```cpp
std::cout << "Total: " << std::to_string(total);
```

因为：

```cpp
std::cout << total;
```

本身就可以直接输出整数。

---

### 错误 4：题目只要求写函数，却自己添加 `main`

如果题目已经规定方法签名：

```cpp
static void printTotal(int total)
```

通常只需要完成这个函数。

不要擅自添加：

```cpp
int main()
```

因为测试系统往往已经有自己的 `main()` 来调用你的函数。
