# C++ 混合类型除法与 `static_cast<double>` 类型转换

## 1. 核心问题：为什么 `7 / 2` 得不到 `3.5`？

在 C++ 中，`/` 的运算结果主要取决于**参与 `/` 运算的两个操作数是什么类型**。

如果两个操作数都是 `int`：

```cpp
7 / 2
```

结果是：

```cpp
3
```

而不是：

```cpp
3.5
```

原因是：

> 两个 `int` 进行除法时，执行的是**整数除法（integer division）**。

整数除法会直接丢弃小数部分，也就是“向 0 截断”。

例如：

```cpp
7 / 2    // 3
5 / 2    // 2
1 / 2    // 0
```

---

# 2. 截断发生在 `/` 运算的时候

这是本节最重要的知识点：

> **小数部分是在执行 `/` 时丢失的，而不是在之后赋值时丢失的。**

例如：

```cpp
double result = 7 / 2;
```

很多初学者可能会以为：

```text
result 是 double
↓
所以 7 / 2 应该得到 3.5
```

实际上不是。

C++ 的处理顺序是：

```cpp
7 / 2
```

首先执行整数除法：

```text
7 / 2
↓
3
```

然后才把 `3` 转换成 `double`：

```text
3
↓
3.0
```

所以最终：

```cpp
double result = 7 / 2;
```

等价于：

```cpp
double result = 3;
```

最终保存的是：

```text
3.0
```

小数部分已经在 `/` 的时候丢掉了，后面无法恢复。

---

# 3. `double` 变量不能“拯救”整数除法

例如：

```cpp
double average = 7 / 2;
```

虽然 `average` 是：

```cpp
double
```

但是 `/` 看到的两个操作数仍然是：

```cpp
int / int
```

因此仍然执行：

```cpp
7 / 2
→ 3
```

然后：

```cpp
3
→ 3.0
```

所以：

```cpp
double average = 7 / 2;
```

得到的是：

```text
3.0
```

而不是：

```text
3.5
```

因此必须记住：

> **结果变量的类型，不会反过来改变前面表达式所采用的运算类型。**

---

# 4. 一个操作数是 `double` 就会变成浮点除法

比较下面两个表达式：

```cpp
7 / 2
```

两个操作数都是 `int`：

```text
int / int
→ int
→ 3
```

但是：

```cpp
7 / 2.0
```

其中：

```cpp
7    // int
2.0  // double
```

当 C++ 遇到：

```cpp
int / double
```

时，会先把 `int` 转换成 `double`：

```text
7
↓
7.0
```

于是实际计算相当于：

```cpp
7.0 / 2.0
```

结果：

```text
3.5
```

所以：

```cpp
7 / 2      // 3
7 / 2.0    // 3.5
```

---

# 5. 混合类型运算

当算术表达式中同时存在：

```cpp
int
```

和：

```cpp
double
```

时，通常会把 `int` 转换成 `double` 后再计算。

例如：

```cpp
int x = 7;
double y = 2.0;

double result = x / y;
```

执行过程可以理解为：

```text
x
= 7
= int

y
= 2.0
= double
```

执行：

```cpp
x / y
```

时：

```text
7 / 2.0
↓
7.0 / 2.0
↓
3.5
```

所以：

```cpp
result == 3.5
```

---

# 6. 类型判断是“每个运算符单独进行”的

这一点非常重要。

C++ 不会观察整个表达式然后统一决定“这个表达式最终要不要保留小数”。

而是：

> 每个运算符根据自己左右两边当时的操作数类型决定如何计算。

例如：

```cpp
double result = 7 / 2;
```

对于 `/` 来说，它只看到：

```text
int / int
```

所以执行整数除法。

它并不会因为外面的变量是：

```cpp
double result
```

就改变自己的行为。

可以把它理解为：

```text
第 1 步：

7 / 2
→ 3

第 2 步：

double result = 3;
→ 3.0
```

---

# 7. 函数返回类型是 `double` 也不能修复整数除法

同样的规则也适用于函数。

例如：

```cpp
static double average(int total, int count) {
    return total / count;
}
```

虽然函数返回类型是：

```cpp
double
```

但是：

```cpp
total
```

和：

```cpp
count
```

都是：

```cpp
int
```

因此：

```cpp
total / count
```

执行的仍然是：

```text
int / int
```

例如：

```cpp
total = 7
count = 2
```

会先计算：

```cpp
7 / 2
→ 3
```

然后 `return` 再把它转换为：

```text
3.0
```

因此函数返回：

```text
3.0
```

而不是：

```text
3.5
```

---

# 8. 当两个变量都是 `int` 时怎么办？

有时我们的数据本来就是整数：

```cpp
int total = 7;
int count = 2;
```

但我们希望得到：

```text
3.5
```

这时候需要在执行除法**之前**，主动把其中一个操作数转换成 `double`。

C++ 推荐使用：

```cpp
static_cast<double>(...)
```

例如：

```cpp
static_cast<double>(total)
```

如果：

```cpp
total == 7
```

那么：

```cpp
static_cast<double>(total)
```

会产生一个：

```text
7.0
```

类型为：

```cpp
double
```

的值。

---

# 9. `static_cast<double>` 是什么？

基本语法：

```cpp
static_cast<目标类型>(值)
```

例如：

```cpp
int number = 7;

double result = static_cast<double>(number);
```

可以理解为：

```text
number
int 7

↓ 转换

double 7.0
```

因此：

```cpp
static_cast<double>(7)
```

得到：

```text
7.0
```

---

# 10. `static_cast` 不会修改原变量

例如：

```cpp
int wins = 7;

static_cast<double>(wins);
```

这不会把：

```cpp
wins
```

永久改成 `double`。

`wins` 仍然是：

```cpp
int
```

并且值仍然是：

```text
7
```

`static_cast<double>(wins)` 只是产生了一个临时的：

```text
double 7.0
```

可以理解为：

```text
wins
└── int 7

static_cast<double>(wins)
└── double 7.0 的临时副本
```

原来的：

```cpp
wins
```

完全没有改变。

---

# 11. 正确做法：在除法之前转换

假设：

```cpp
int total = 7;
int count = 2;
```

可以写：

```cpp
static_cast<double>(total) / count
```

此时 `/` 左边是：

```cpp
double
```

右边是：

```cpp
int
```

也就是：

```text
double / int
```

C++ 会把右边的 `int` 也转换成 `double`：

```text
7.0 / 2
↓
7.0 / 2.0
↓
3.5
```

因此结果是：

```cpp
3.5
```

---

# 12. 转换任意一边都可以

只需要让 `/` 的一个操作数成为 `double`。

例如：

```cpp
static_cast<double>(total) / count
```

可以。

下面这样也可以：

```cpp
total / static_cast<double>(count)
```

因为两种情况下 `/` 都会变成浮点除法。

### 第一种

```cpp
static_cast<double>(total) / count
```

类型：

```text
double / int
```

最终：

```text
double / double
```

### 第二种

```cpp
total / static_cast<double>(count)
```

类型：

```text
int / double
```

最终同样变成：

```text
double / double
```

---

# 13. 最常见的错误：转换得太晚

下面这种写法是错误的：

```cpp
static_cast<double>(7 / 2)
```

很多人会以为：

```text
转成 double
→ 3.5
```

但实际上 C++ 会先计算括号：

```cpp
7 / 2
```

由于：

```text
int / int
```

所以：

```text
7 / 2
→ 3
```

然后才执行：

```cpp
static_cast<double>(3)
```

得到：

```text
3.0
```

完整过程：

```text
static_cast<double>(7 / 2)

        ↓ 先算括号

static_cast<double>(3)

        ↓ 再转换

3.0
```

因此：

```cpp
static_cast<double>(7 / 2)
```

结果仍然不是 `3.5`。

---

# 14. 正确与错误写法对比

## ❌ 错误

```cpp
double result = 7 / 2;
```

过程：

```text
7 / 2
→ 3

3
→ 3.0
```

---

## ❌ 错误

```cpp
double result = static_cast<double>(7 / 2);
```

过程：

```text
7 / 2
→ 3

static_cast<double>(3)
→ 3.0
```

---

## ✅ 正确

```cpp
double result = static_cast<double>(7) / 2;
```

过程：

```text
7
→ 7.0

7.0 / 2
→ 3.5
```

---

## ✅ 正确

```cpp
int total = 7;
int count = 2;

double result = static_cast<double>(total) / count;
```

结果：

```text
3.5
```

---

# 15. 一个非常实用的判断方法

看到除法表达式：

```cpp
a / b
```

不要先看：

```cpp
double result = ...
```

也不要先看函数返回类型。

只问一个问题：

> **在 `/` 真正执行的那一刻，左右两个操作数分别是什么类型？**

如果是：

```text
int / int
```

那么：

```text
整数除法
```

如果至少一个是：

```text
double
```

那么：

```text
浮点除法
```

可以记成：

```text
int    / int
       ↓
      int
```

而：

```text
double / int
int    / double
double / double
       ↓
     double
```

---

# 16. 为什么只转换一个操作数就够了？

例如：

```cpp
static_cast<double>(total) / count
```

假设：

```cpp
total = 7
count = 2
```

那么左边先变成：

```text
double 7.0
```

表达式现在是：

```text
double / int
```

为了进行运算，C++ 会自动把 `count` 转成对应的 `double`：

```text
7.0 / 2
↓
7.0 / 2.0
```

所以没有必要写成：

```cpp
static_cast<double>(total)
    / static_cast<double>(count)
```

虽然这样通常也能工作，但属于多余操作。

通常写：

```cpp
static_cast<double>(total) / count
```

就足够了。

---

# 17. 平均值是这种问题最典型的应用

例如计算：

```text
总分 = 17
人数 = 4
```

平均值应该是：

```text
4.25
```

如果写：

```cpp
int total = 17;
int count = 4;

double average = total / count;
```

实际发生：

```text
17 / 4
→ 4
```

然后：

```text
4
→ 4.0
```

结果错误地变成：

```text
4.0
```

正确做法：

```cpp
double average =
    static_cast<double>(total) / count;
```

此时：

```text
17
↓
17.0

17.0 / 4
↓
4.25
```

---

# 18. 输出 `double` 时不一定看到 `.0`

题目中的测试程序会去掉没有意义的尾随 `0`。

例如函数内部实际上返回：

```text
3.0
```

测试输出可能显示：

```text
3
```

而不是：

```text
3.0
```

这并不意味着返回类型变成了 `int`。

例如：

```cpp
double x = 3.0;
```

它依然是：

```cpp
double
```

只是显示时写成：

```text
3
```

而：

```text
0.125
```

本身有有效的小数部分，因此会显示：

```text
0.125
```

所以：

> **显示格式和变量真实的数据类型不是同一件事。**

---

# 19. 本节最重要的规则

可以把整节内容浓缩成一句话：

> **如果整数除法需要保留小数，必须在 `/` 执行之前把至少一个操作数转换成 `double`。**

推荐写法：

```cpp
static_cast<double>(total) / count
```

而不是：

```cpp
static_cast<double>(total / count)
```

两者最关键的区别是：

```text
先转换，再除法
```

vs.

```text
先除法，再转换
```

### 正确

```text
static_cast<double>(7) / 2

7
↓
7.0

7.0 / 2
↓
3.5
```

### 错误

```text
static_cast<double>(7 / 2)

7 / 2
↓
3

3
↓
3.0
```

---

# 20. 快速复习表

| 表达式                          | `/` 两侧类型        |    结果 |
| ---------------------------- | --------------- | ----: |
| `7 / 2`                      | `int / int`     |   `3` |
| `7.0 / 2`                    | `double / int`  | `3.5` |
| `7 / 2.0`                    | `int / double`  | `3.5` |
| `static_cast<double>(7) / 2` | `double / int`  | `3.5` |
| `7 / static_cast<double>(2)` | `int / double`  | `3.5` |
| `static_cast<double>(7 / 2)` | 先执行 `int / int` | `3.0` |

---

# 21. 最终记忆模型

以后遇到：

```cpp
double result = a / b;
```

不要因为左边写着：

```cpp
double
```

就认为一定存在小数。

应该检查：

```cpp
a / b
```

如果：

```cpp
a: int
b: int
```

那么：

```text
a / b
→ 整数除法
→ 小数被丢掉
```

需要改成：

```cpp
double result =
    static_cast<double>(a) / b;
```

记住：

```text
类型转换必须发生在除法之前。
```
