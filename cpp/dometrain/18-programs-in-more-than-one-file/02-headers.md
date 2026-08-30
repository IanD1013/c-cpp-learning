# C++ Header（头文件）与多文件声明

## 1. 为什么需要 Header？

当多个 `.cpp` 文件都需要使用同一组函数时，可以在每个文件里手动写函数声明：

```cpp
int lengthOf(const std::string &text);
```

但这样做容易出现问题：某个文件里的声明被修改了，而其他文件没有同步修改，最终不同文件中的函数声明可能互相不一致。

更好的做法是把公共声明统一放进 **header 文件（头文件）**。

例如：

```cpp
// sizes.hpp
#include <string>

int lengthOf(const std::string &text);
```

然后实现放在 `.cpp` 中：

```cpp
// sizes.cpp
#include "sizes.hpp"

int lengthOf(const std::string &text) {
    return static_cast<int>(text.size());
}
```

可以简单理解为：

```text
.hpp：告诉其他文件“有哪些东西可以用”
.cpp：真正实现这些东西
```

---

## 2. 声明与定义

头文件通常放的是 **declaration（声明）**：

```cpp
int lengthOf(const std::string &text);
```

它告诉编译器：

* 函数叫 `lengthOf`
* 参数是 `const std::string &`
* 返回值是 `int`

但没有提供函数具体怎么运行。

函数的 **definition（定义）** 则提供函数体：

```cpp
int lengthOf(const std::string &text) {
    return static_cast<int>(text.size());
}
```

一个典型项目结构：

```text
project/
├── sizes.hpp
├── sizes.cpp
└── main.cpp
```

其中：

```text
sizes.hpp
    ↓ 声明接口

sizes.cpp
    ↓ 实现接口

main.cpp
    ↓ 使用接口
```

---

## 3. 为什么 `.cpp` 自己也应该 include 对应的 header？

推荐：

```cpp
#include "sizes.hpp"

int lengthOf(const std::string &text) {
    ...
}
```

而不是直接在 `sizes.cpp` 里自己重新写函数签名。

这样编译器可以帮助检查：

```cpp
// sizes.hpp
int lengthOf(const std::string &text);
```

和

```cpp
// sizes.cpp
int lengthOf(const std::string &text) {
    ...
}
```

是否一致。

如果头文件声明和实现不匹配，更容易在编译阶段发现问题。

---

## 4. Header 应该自己包含自己依赖的类型

例如：

```cpp
int lengthOf(const std::string &text);
```

这里使用了：

```cpp
std::string
```

因此 `sizes.hpp` 自己应该：

```cpp
#include <string>
```

完整写法：

```cpp
// sizes.hpp
#include <string>

int lengthOf(const std::string &text);
```

而不要依赖别人先写：

```cpp
#include <string>
#include "sizes.hpp"
```

推荐遵循：

> **Header 应该自己包含它声明所需要的依赖。**

也就是说，下面应该能够单独工作：

```cpp
#include "sizes.hpp"
```

调用者不应该需要知道 `sizes.hpp` 内部依赖 `<string>`。

---

# 5. `"..."` 和 `<...>` 的区别

C++ 中常见两种 include：

```cpp
#include "sizes.hpp"
```

和：

```cpp
#include <string>
```

一般来说：

### 项目自己的 Header

使用：

```cpp
#include "sizes.hpp"
```

双引号通常会优先从当前项目的相关目录中查找。

---

### 标准库 / 配置好的 include 路径

使用：

```cpp
#include <string>
#include <vector>
#include <iostream>
```

尖括号表示从编译器配置的 include 路径中查找。

因此：

```cpp
#include <sizes.hpp>
```

不一定能找到项目中的：

```text
sizes.hpp
```

除非编译器显式配置了对应的 include directory。

在本题中要求使用：

```cpp
#include "checks.hpp"
```

而：

```cpp
#include <checks.hpp>
```

被禁止。

---

# 6. 函数签名必须与 Header 完全匹配

假设 `checks.hpp` 中声明：

```cpp
bool allPositive(const std::vector<int> &values);

int firstNegativeIndex(const std::vector<int> &values);
```

实现也必须对应：

```cpp
bool allPositive(const std::vector<int> &values) {
    ...
}
```

不能偷偷改成：

```cpp
bool allPositive(std::vector<int> values) {
    ...
}
```

因为：

```cpp
std::vector<int>
```

和：

```cpp
const std::vector<int> &
```

是不同的参数类型。

从 C++ 的角度看，它们代表两个不同的函数。

于是可能出现：

```text
Header 声明了：
allPositive(const vector<int>&)

你却定义了：
allPositive(vector<int>)

结果：
Header 中声明的那个函数仍然没有 definition。
```

最终通常会导致 linker error。

---

# 7. 为什么使用 `const std::vector<int> &`？

```cpp
const std::vector<int> &values
```

同时包含两个重要思想。

## `&`：引用传递

如果写：

```cpp
std::vector<int> values
```

函数会得到 vector 的一个副本。

对于大型 vector，这可能产生不必要的复制。

而：

```cpp
const std::vector<int> &values
```

直接引用原来的 vector，不需要复制整个容器。

---

## `const`：不允许修改

```cpp
const std::vector<int> &values
```

表示：

> 函数只读取这个 vector，不修改它。

因此对于“检查数据”的函数，这是很常见的签名：

```cpp
bool checkSomething(const std::vector<int> &values);
```

---

# 8. `allPositive`

函数：

```cpp
bool allPositive(const std::vector<int> &values);
```

要求：

> 所有元素都必须严格大于 `0`。

例如：

```cpp
{1, 2, 3}
```

返回：

```cpp
true
```

但是：

```cpp
{4, 0, 9}
```

返回：

```cpp
false
```

因为：

```cpp
0 > 0
```

不成立。

同样：

```cpp
{4, -1, 9}
```

返回：

```cpp
false
```

---

## 空 vector 为什么返回 true？

例如：

```cpp
{}
```

没有任何元素违反：

```cpp
value > 0
```

因此按照“所有元素都满足条件”的逻辑：

```cpp
allPositive({})
```

返回：

```cpp
true
```

这种现象称为 **vacuous truth（空真）**。

可以把算法理解成：

```text
默认认为答案是 true

检查每一个元素：

    如果找到一个 <= 0
        → false

如果全部检查完都没找到反例
        → true
```

所以空 vector 根本找不到反例，自然返回 `true`。

---

# 9. `firstNegativeIndex`

函数：

```cpp
int firstNegativeIndex(const std::vector<int> &values);
```

需要寻找：

> 第一个 `< 0` 的元素的位置。

注意 index 从 `0` 开始。

例如：

```cpp
{4, -1, 9, -3}
```

index 为：

```text
value:  4  -1   9  -3
index:  0   1   2   3
```

第一个负数是：

```cpp
-1
```

所以返回：

```cpp
1
```

再例如：

```cpp
{-2, 5}
```

第一个元素已经是负数，因此：

```cpp
return 0;
```

如果完全没有负数：

```cpp
{1, 2, 3}
```

则返回特殊值：

```cpp
-1
```

因为合法 vector index 不会是负数，所以 `-1` 很适合表示：

```text
not found
```

---

# 10. 不要给这些函数加 `static`

本题要求 grader 能够通过函数名称找到：

```cpp
allPositive
firstNegativeIndex
```

因此不要写：

```cpp
static bool allPositive(...);
```

这里的文件级 `static` 会让函数具有 internal linkage，使函数只在当前 translation unit 内部可见。

应该保持：

```cpp
bool allPositive(...);
```

---

# 11. 本题的 Header

题目已经提供：

```cpp
checks.hpp
```

并且它已经：

```cpp
#include <vector>
```

所以 `main.cpp` 中只需要保留：

```cpp
#include "checks.hpp"
```

然后在下面提供两个函数 definition。

不需要再次：

```cpp
#include <vector>
```

虽然重复 include 标准库通常不会出问题，但这里没有必要，因为 `checks.hpp` 已经声明自己的依赖。

---

# 12. Header 还缺少什么？

当前示例：

```cpp
// sizes.hpp
#include <string>

int lengthOf(const std::string &text);
```

还有一个问题：

> 如果同一个 header 在同一个 translation unit 中被 include 多次怎么办？

例如：

```text
main.cpp
 ├── A.hpp
 │    └── checks.hpp
 │
 └── B.hpp
      └── checks.hpp
```

这样 `checks.hpp` 可能间接进入 `main.cpp` 两次。

因此 header 通常需要防止重复包含。

常见方式有：

```cpp
#pragma once
```

或者传统的 include guard：

```cpp
#ifndef CHECKS_HPP
#define CHECKS_HPP

// declarations

#endif
```

这部分通常叫：

> **Header Guards / Include Guards**

核心作用是：

```text
同一个 header 即使被 include 多次，
它的内容也只展开一次。
```

---

# 总结

多文件 C++ 项目可以记成：

```text
Header (.hpp)
    ↓
声明接口

Source (.cpp)
    ↓
实现接口

其他 .cpp
    ↓
#include Header
    ↓
知道如何调用接口
```

几个重要原则：

```cpp
// 1. 项目自己的 header
#include "checks.hpp"

// 2. 标准库
#include <vector>

// 3. Header 自己声明自己的依赖

// 4. declaration 和 definition 的函数签名必须一致

// 5. 大型只读对象常使用 const T&

// 6. Header 通常需要 include guard / #pragma once
```

可以把 Header 看成一个 **接口契约（interface contract）**：

> `.hpp` 规定函数“长什么样”，`.cpp` 负责真正实现它。
