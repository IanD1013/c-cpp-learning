# C++ 函数声明、函数定义与链接

## 1. 函数声明是什么？

在 C++ 中，如果你想告诉编译器：

> “有这么一个函数，它的实现可能写在别的地方。”

就可以写一个**函数声明（function declaration）**：

```cpp
int helper(int n);
```

它包含三部分信息：

```text
返回类型    函数名      参数类型
int        helper      int
```

最后用分号 `;` 结束。

函数声明**没有函数体**，因此它并没有真正实现这个函数。

---

## 2. 函数声明和函数定义的区别

### 函数声明

```cpp
int helper(int n);
```

只告诉编译器函数的接口。

### 函数定义

```cpp
int helper(int n) {
    return n * 2;
}
```

除了接口之外，还真正提供了函数的实现。

可以简单理解为：

```text
声明 = 告诉编译器“这个函数存在”

定义 = 真正写出这个函数做什么
```

最明显的语法区别是：

```cpp
int helper(int n);       // 声明

int helper(int n) {      // 定义
    return n * 2;
}
```

声明的位置用 `;` 代替了函数体 `{ ... }`。

---

## 3. 为什么必须先声明？

C++ 编译器基本按照**从上到下**的顺序读取代码。

例如：

```cpp
int main() {
    int x = helper(3);
}
```

如果编译器读到：

```cpp
helper(3)
```

时还不知道 `helper` 是什么，就会报错。

因此可以提前声明：

```cpp
int helper(int n);

int main() {
    int x = helper(3);
}
```

之后真正的函数定义可以放在下面：

```cpp
int helper(int n) {
    return n * 2;
}
```

完整结构：

```cpp
int helper(int n);   // declaration

int main() {
    return helper(3);
}

int helper(int n) {  // definition
    return n * 2;
}
```

这就是所谓的：

> **先声明，后定义。**

---

## 4. 参数名字可以省略

函数声明主要告诉编译器的是**参数类型**。

因此：

```cpp
int helper(int n);
```

也可以写成：

```cpp
int helper(int);
```

两者对编译器来说基本等价。

参数名主要是为了让代码更容易阅读。

例如：

```cpp
int calculateDistance(int speed, int time);
```

比：

```cpp
int calculateDistance(int, int);
```

更容易理解。

---

## 5. 声明必须和定义匹配

假设声明是：

```cpp
int helper(int value);
```

那么定义必须具有相同的函数签名：

```cpp
int helper(int x) {
    return x * 2;
}
```

这里：

```text
value
x
```

名字不同没有关系。

重要的是：

```text
返回类型
函数名
参数类型
```

必须匹配。

因此：

```cpp
int helper(int value);
```

可以对应：

```cpp
int helper(int n) {
    return n * 2;
}
```

但不能对应：

```cpp
double helper(int n) {
    return n * 2;
}
```

因为返回类型不同。

也不能对应：

```cpp
int helper(double n) {
    return n * 2;
}
```

因为参数类型不同。

---

# 6. 一个函数可以声明多次，但只能定义一次

同一个函数可以出现多个相同声明：

```cpp
int helper(int);

int helper(int);

int helper(int);
```

这是允许的。

但通常整个程序中只能存在一个对应的函数定义：

```cpp
int helper(int n) {
    return n * 2;
}
```

如果出现两个相同定义：

```cpp
int helper(int n) {
    return n * 2;
}

int helper(int n) {
    return n * 3;
}
```

就会产生类似：

```text
redefinition of 'helper'
```

这样的错误。

可以记成：

```text
声明：可以有很多份

定义：只能有一份
```

---

# 7. 为什么不同 `.cpp` 文件之间需要声明？

正常 C++ 项目通常会分别编译每个 `.cpp` 文件。

例如：

```text
main.cpp
helpers.cpp
```

假设：

```cpp
// helpers.cpp

int tripleOf(int value) {
    return value * 3;
}
```

`main.cpp` 在编译时并不会自动看到 `helpers.cpp` 的代码。

因此如果 `main.cpp` 想调用：

```cpp
tripleOf(5)
```

必须先知道函数的声明：

```cpp
int tripleOf(int value);
```

例如：

```cpp
// main.cpp

int tripleOf(int value);

int main() {
    return tripleOf(5);
}
```

之后：

```text
编译器
```

分别编译：

```text
main.cpp
helpers.cpp
```

最后：

```text
linker（链接器）
```

负责把：

```cpp
tripleOf(5)
```

这个函数调用和：

```cpp
int tripleOf(int value) {
    return value * 3;
}
```

这个函数定义连接起来。

---

# 8. 编译和链接的区别

可以把正常 C++ 构建过程简单理解成：

```text
.cpp 文件
   ↓
Compiler
   ↓
目标文件
   ↓
Linker
   ↓
最终程序
```

例如：

```text
main.cpp
   ↓ compile
main.o

helpers.cpp
   ↓ compile
helpers.o
```

然后：

```text
main.o
helpers.o
   ↓ linker
program
```

编译 `main.cpp` 时：

```cpp
int tripleOf(int value);
```

已经足够让编译器确认调用是否合法。

真正的函数实现：

```cpp
int tripleOf(int value) {
    return value * 3;
}
```

可以由链接器之后找到。

---

# 9. 为什么头文件 `.h` 很重要？

现实项目一般不会在每个 `.cpp` 文件里手动重复声明。

通常会把声明放进头文件：

```cpp
// helpers.h

int tripleOf(int value);
bool isNegative(int value);
```

然后：

```cpp
// helpers.cpp

#include "helpers.h"

int tripleOf(int value) {
    return value * 3;
}

bool isNegative(int value) {
    return value < 0;
}
```

使用这些函数的地方：

```cpp
#include "helpers.h"

int main() {
    return tripleOf(5);
}
```

因此常见习惯是：

```text
.h / .hpp
    ↓
放函数声明

.cpp
    ↓
放函数定义
```

---

# 10. 本题的特殊运行环境

本题的运行器并不是完全按照普通 C++ 项目的方式分别编译文件。

它会把多个 `.cpp` 文件按照顺序拼接起来。

大致类似：

```text
你的代码
+
helpers.cpp
```

最终变成一个大的 `.cpp` 文件。

因此**代码顺序会影响结果**。

例如 `helpers.cpp` 在你的代码后面：

```cpp
// 你的代码

struct Solution {
    ...
};

// 后面才出现 helpers.cpp

int tripleOf(int value) {
    return value * 3;
}
```

那么当编译器读取 `Solution` 时，它还没有看到：

```cpp
tripleOf
```

因此需要提前声明：

```cpp
int tripleOf(int value);
```

---

# 11. 为什么不能自己再实现一次？

题目已经在 `helpers.cpp` 中提供：

```cpp
int tripleOf(int value) {
    return value * 3;
}
```

如果你自己再写：

```cpp
int tripleOf(int value) {
    return value * 3;
}
```

那么文件拼接以后会出现两个定义：

```cpp
int tripleOf(int value) {
    return value * 3;
}

...

int tripleOf(int value) {
    return value * 3;
}
```

编译器就会报：

```text
redefinition
```

所以正确做法不是重新实现，而只是提前**声明**：

```cpp
int tripleOf(int value);
```

同理：

```cpp
bool isNegative(int value);
```

---

# 12. 本题涉及的两个辅助函数

题目已经提供两个函数。

### `tripleOf`

```cpp
int tripleOf(int value);
```

功能：

```text
返回 value 的 3 倍
```

例如：

```cpp
tripleOf(2)  // 6
tripleOf(5)  // 15
tripleOf(0)  // 0
```

---

### `isNegative`

```cpp
bool isNegative(int value);
```

功能：

```text
value < 0  → true
否则       → false
```

例如：

```cpp
isNegative(-3)  // true
isNegative(0)   // false
isNegative(5)   // false
```

特别注意：

```cpp
0
```

并不是负数。

因此：

```cpp
isNegative(0)
```

返回：

```cpp
false
```

---

# 13. 本题算法：只计算非负数的三倍之和

方法签名：

```cpp
static int sumOfTriples(const std::vector<int> &values)
```

要求：

1. 遍历 `values`
2. 如果 `value` 是负数，跳过
3. 否则计算：

```cpp
tripleOf(value)
```

4. 加到总和中
5. 返回结果

伪代码：

```text
total = 0

for 每个 value:
    如果 value 是负数:
        跳过

    total += value 的三倍

return total
```

---

## 示例

输入：

```cpp
{2, 5, 1}
```

计算：

```text
tripleOf(2) = 6
tripleOf(5) = 15
tripleOf(1) = 3
```

因此：

```text
6 + 15 + 3 = 24
```

返回：

```cpp
24
```

---

## 包含负数

例如：

```cpp
{2, -4, 5}
```

过程：

```text
2  → 非负 → 6
-4 → 负数 → 跳过
5  → 非负 → 15
```

结果：

```text
6 + 15 = 21
```

---

## 空 vector

```cpp
{}
```

循环一次都不会执行，因此：

```cpp
total = 0
```

返回：

```cpp
0
```

---

## 全是负数

```cpp
{-1, -2, -3}
```

全部被跳过：

```text
total = 0
```

因此返回：

```cpp
0
```

---

## 包含 0

```cpp
{0}
```

因为：

```cpp
isNegative(0)
```

为 `false`，所以不会跳过。

会执行：

```cpp
tripleOf(0)
```

结果：

```cpp
0
```

所以最终仍然返回：

```cpp
0
```

---

# 核心知识总结

本题真正要掌握的不是 `for` 循环，而是 C++ 的**声明与定义**。

记住：

```text
函数声明
    ↓
告诉编译器函数长什么样

函数定义
    ↓
真正实现函数
```

例如：

```cpp
int tripleOf(int value);      // declaration
```

和：

```cpp
int tripleOf(int value) {     // definition
    return value * 3;
}
```

在多文件项目中：

```text
编译器只需要声明
↓
就可以检查函数调用

链接器之后找到定义
↓
把函数调用和实现连接起来
```

最常见的工程结构是：

```text
header (.h/.hpp)
    → declarations

source (.cpp)
    → definitions
```

以及最重要的一条规则：

```text
同一个函数可以声明多次，
但整个程序通常只能有一个定义。
```
