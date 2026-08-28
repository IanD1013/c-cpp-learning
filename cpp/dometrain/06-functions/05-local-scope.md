# C++ 局部作用域、变量遮蔽与循环中的变量生命周期

## 1. 花括号 `{}` 会创建一个作用域

在 C++ 中，一对花括号通常会形成一个新的代码块（block）：

```cpp
{
    // 一个新的作用域
}
```

在这个代码块中声明的变量，只能在：

* 从它声明的位置开始
* 到当前代码块对应的 `}` 结束

这一段范围内使用。

例如：

```cpp
int score = 10;

{
    int bonus = 5;
    score += bonus;
}

// bonus 在这里已经不存在
```

`bonus` 的作用域只在内部的 `{ ... }` 中。

这种只能在某个局部代码块中使用的变量称为**局部变量（local variable）**。

---

## 2. 在内部作用域声明同名变量：Shadowing

考虑：

```cpp
int score = 10;

{
    int score = 0;
    score += 5;
}

// 外层 score 仍然是 10
```

这里实际上存在两个完全不同的变量：

```cpp
外层 score = 10
内层 score = 0
```

内部的：

```cpp
int score = 0;
```

并不会“重新使用”外面的 `score`。

它会创建一个**新的变量**。

此时内部变量会暂时隐藏外层的同名变量，这种现象称为：

> **Variable Shadowing（变量遮蔽）**

因此：

```cpp
{
    int score = 0;
    score += 5;
}
```

这里的：

```cpp
score += 5;
```

实际上修改的是内部 `score`：

```text
0 → 5
```

外层的：

```cpp
int score = 10;
```

完全没有被修改。

离开内部代码块之后：

```cpp
}
```

内部 `score` 生命周期结束，外层 `score` 又重新变得可见。

因此最后：

```cpp
score == 10
```

---

## 3. Scope 和 Lifetime 的区别

这两个概念容易混淆。

### Scope：作用域

指：

> 在代码的什么位置可以通过变量名访问这个变量。

例如：

```cpp
{
    int x = 10;
}
```

`x` 的作用域就在这一对 `{}` 中。

### Lifetime：生命周期

指：

> 这个变量实际存在多长时间。

对于普通局部变量：

```cpp
{
    int x = 10;
}
```

通常：

```text
进入代码块
↓
创建 x
↓
执行代码
↓
离开代码块
↓
销毁 x
```

所以局部变量的生命周期通常随着代码块结束而结束。

---

## 4. 循环内部声明的变量，每轮都会重新创建

例如：

```cpp
for (int i = 0; i < 3; ++i) {
    int count = 0;
    count++;
}
```

不要理解成：

```text
count:
0 → 1 → 2 → 3
```

实际上是：

```text
第 1 轮：
创建 count = 0
count = 1
销毁 count

第 2 轮：
创建一个新的 count = 0
count = 1
销毁 count

第 3 轮：
创建一个新的 count = 0
count = 1
销毁 count
```

因此循环结束后，没有任何累计结果。

如果某个变量需要在不同循环迭代之间保存状态，就应该声明在循环外面：

```cpp
int count = 0;

for (int i = 0; i < 3; ++i) {
    count++;
}
```

此时：

```text
count:
0 → 1 → 2 → 3
```

---

# 常见 Bug：在循环内部重新声明计数器

假设我们希望计算：

> 从 `1` 到 `limit` 中，有多少个整数可以被 `factor` 整除。

例如：

```cpp
countMultiples(10, 3)
```

数字为：

```text
1 2 3 4 5 6 7 8 9 10
```

能被 3 整除的是：

```text
3, 6, 9
```

所以结果应该是：

```cpp
3
```

---

## 错误写法

类似下面的代码存在典型的 shadowing bug：

```cpp
int count = 0;

for (int i = 1; i <= limit; ++i) {
    if (i % factor == 0) {
        int count = 0;
        count += 1;
    }
}

return count;
```

关键问题在这里：

```cpp
int count = 0;
```

它创建了一个新的局部变量。

于是结构实际上是：

```cpp
int count = 0;        // 外层 count

for (...) {
    if (...) {
        int count = 0; // 新的内层 count
        count += 1;
    }
}

return count;         // 返回外层 count
```

每次满足条件时：

```text
创建内层 count = 0
↓
count += 1
↓
内层 count = 1
↓
离开 if
↓
销毁
```

而真正最后返回的外层：

```cpp
count
```

从头到尾都是：

```text
0
```

所以程序会错误地一直返回 `0`。

---

# 正确原则：修改已有变量，而不是重新声明

如果已经写了：

```cpp
int count = 0;
```

之后想修改它，直接：

```cpp
count += 1;
```

不要再写：

```cpp
int count = 0;
```

可以记成：

```text
int count = 0;
```

表示：

> 创建一个新的变量。

而：

```text
count = 0;
count += 1;
```

表示：

> 修改已经存在的变量。

其中 `int` 是否出现非常关键。

---

# 参数检查：为什么 factor <= 0 返回 -1

题目要求：

```cpp
if (factor <= 0) {
    return -1;
}
```

这是一个错误值（sentinel value）。

正常情况下，函数返回的是：

```text
0, 1, 2, 3, ...
```

也就是“倍数的数量”，不可能是负数。

因此：

```cpp
-1
```

可以清楚地表示：

> 输入参数 `factor` 无效。

---

## 为什么 factor == 0 必须特别处理

代码需要执行：

```cpp
i % factor
```

如果：

```cpp
factor == 0
```

就会变成：

```cpp
i % 0
```

C++ 中 `% 0` 是未定义行为，因此必须在执行取模之前阻止这种情况：

```cpp
if (factor <= 0) {
    return -1;
}
```

---

# 为什么循环必须从 1 开始

正确：

```cpp
for (int i = 1; i <= limit; ++i)
```

不能写：

```cpp
for (int i = 0; i <= limit; ++i)
```

因为：

```cpp
0 % factor == 0
```

对于任何非零 `factor` 都成立。

例如：

```cpp
countMultiples(10, 1)
```

真正需要统计的是：

```text
1, 2, 3, ..., 10
```

共：

```text
10
```

如果从 `0` 开始：

```text
0, 1, 2, ..., 10
```

就会错误得到：

```text
11
```

---

# 典型测试用例

```cpp
countMultiples(10, 3)
```

结果：

```text
3
```

因为：

```text
3, 6, 9
```

---

```cpp
countMultiples(10, 1)
```

结果：

```text
10
```

---

```cpp
countMultiples(0, 3)
```

结果：

```text
0
```

因为范围 `1 ~ 0` 中没有任何数字。

---

```cpp
countMultiples(3, 10)
```

结果：

```text
0
```

因为：

```text
1, 2, 3
```

都不能被 `10` 整除。

---

```cpp
countMultiples(10, -3)
```

结果：

```text
-1
```

因为负的 `factor` 被视为非法输入。

---

# 核心知识总结

最重要的是区分：

```cpp
int count = 0;
```

和：

```cpp
count += 1;
```

前者：

> 声明并创建一个新变量。

后者：

> 修改已经存在的变量。

如果在更内层的 `{}` 中再次写：

```cpp
int count
```

就会产生一个新的同名变量，并遮蔽外层变量。

因此：

```cpp
int count = 0;

for (...) {
    if (...) {
        count += 1;
    }
}
```

通常才是实现累计计数的正确结构。
