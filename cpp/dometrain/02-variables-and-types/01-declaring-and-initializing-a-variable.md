# C++ 变量：声明、初始化与赋值

## 1. 什么是变量？

变量（variable）可以理解为：

> **一块有名字的存储空间，用来保存某种类型的数据。**

例如：

```cpp
int score = 0;
```

这条语句同时完成了两件事：

1. 创建一个名为 `score` 的 `int` 变量
2. 用值 `0` 初始化它

可以从左到右理解：

```text
int     score     =     0;
类型    变量名    初始化  初始值
```

---

## 2. 变量声明的基本结构

最常见的形式是：

```cpp
类型 变量名 = 初始值;
```

例如：

```cpp
int age = 18;
int score = 100;
int total = 5 + 7;
```

这里：

* `int`：变量的类型
* `age` / `score` / `total`：变量名
* `=`：用于初始化变量
* 右边的表达式：计算出变量最初保存的值
* `;`：结束这条语句

例如：

```cpp
int total = 5 + 7;
```

程序会先计算：

```cpp
5 + 7
```

得到：

```cpp
12
```

然后用这个结果初始化：

```cpp
total
```

因此之后：

```cpp
total
```

保存的就是：

```cpp
12
```

---

# 3. 类型决定变量可以保存什么

在：

```cpp
int score = 0;
```

中：

```cpp
int
```

表示这是一个整数变量。

因此它可以保存：

```cpp
0
12
-5
100
```

这样的整数。

例如：

```cpp
int temperature = -4;
int students = 30;
```

类型非常重要，因为 C++ 是一种**静态类型语言（statically typed language）**。

变量一旦被声明为：

```cpp
int score;
```

那么 `score` 的类型就是 `int`。

之后不能突然把它变成另一个完全不同类型的变量。

---

# 4. 声明（Declaration）与初始化（Initialization）

理解 C++ 变量时，一个非常重要的区别是：

* 声明变量
* 初始化变量

## 只有声明

例如：

```cpp
int score;
```

这里创建了一个名为：

```cpp
score
```

的 `int` 变量。

但是对于这种**普通局部变量**来说，并没有给它一个确定的初始值。

因此不要写：

```cpp
int score;

std::cout << score;
```

对于普通未初始化的局部 `int`，读取其不确定值会导致未定义行为（Undefined Behavior）。

也就是说：

> C++ 不保证你会得到什么结果。

它可能看起来像：

```text
0
```

也可能是：

```text
32767
```

或者其他值。

更重要的是：

> 不能依赖这种结果。

---

## 声明并初始化

更好的写法是：

```cpp
int score = 0;
```

这样变量从创建开始就有明确的值。

因此初学阶段最好养成习惯：

> **创建变量时就立即初始化。**

例如：

```cpp
int score = 0;
int count = 0;
int total = first + second;
```

而不是：

```cpp
int score;
```

之后再想办法赋值。

---

# 5. `=` 在这里是什么意思？

例如：

```cpp
int total = first + second;
```

这里的：

```cpp
=
```

不是数学中的“等于”。

这里应该理解为：

> 用右边表达式的结果初始化左边的变量。

执行顺序可以理解为：

```text
first + second
      ↓
先计算结果
      ↓
存进 total
```

假设：

```cpp
first = 7
second = 5
```

那么：

```cpp
int total = first + second;
```

相当于：

```cpp
int total = 12;
```

---

# 6. Copy Initialization（拷贝初始化）

下面这种初始化形式：

```cpp
int score = 0;
```

在 C++ 中称为：

> **copy initialization（拷贝初始化）**

基本形式是：

```cpp
Type variable = expression;
```

例如：

```cpp
int total = first + second;
```

初学阶段可以简单理解成：

1. 计算右边表达式
2. 用计算结果初始化左边变量

例如：

```cpp
int answer = 3 * 4;
```

先计算：

```cpp
3 * 4
```

得到：

```cpp
12
```

然后：

```cpp
answer
```

被初始化为：

```cpp
12
```

---

# 7. 初始化和赋值不是完全相同的概念

虽然都使用：

```cpp
=
```

但下面两段代码在概念上不同。

## 初始化

```cpp
int score = 10;
```

这是：

> 创建变量的同时给它初始值。

---

## 赋值

```cpp
int score = 10;

score = 20;
```

第二句：

```cpp
score = 20;
```

不是初始化。

因为 `score` 已经存在了。

这是：

> assignment（赋值）

也就是把原来的值：

```cpp
10
```

替换成：

```cpp
20
```

因此：

```text
int score = 10;
            ↑
          初始化

score = 20;
        ↑
       赋值
```

这是以后学习 C++ 时非常重要的区别。

---

# 8. 变量的作用域（Scope）

假设：

```cpp
{
    int score = 10;

    std::cout << score;
}
```

`score` 是在这个 `{}` block 中声明的。

因此它的名字主要在这个 block 内有效。

出了这个 block：

```cpp
{
    int score = 10;
}

std::cout << score; // 错误
```

这里已经不能再使用：

```cpp
score
```

了。

因此可以简单理解：

> 局部变量通常从声明的位置开始，一直到所在 `{}` block 结束都可以使用。

这种规则叫：

> **scope（作用域）**

---

# 9. 函数参数本身也是变量

例如：

```cpp
static int sumOfTwo(int first, int second)
```

这里：

```cpp
first
```

和：

```cpp
second
```

都是函数参数（parameters）。

它们在函数内部可以像普通变量一样使用：

```cpp
first + second
```

例如调用：

```cpp
sumOfTwo(7, 5);
```

进入函数后，可以理解成：

```text
first  → 7
second → 5
```

于是：

```cpp
first + second
```

就是：

```cpp
7 + 5
```

结果为：

```cpp
12
```

---

# 10. 把计算结果保存到变量

如果要计算两个参数之和，可以写：

```cpp
int total = first + second;
```

这里：

```cpp
first + second
```

先进行计算。

结果被保存到：

```cpp
total
```

中。

例如：

```text
first = 7
second = 5

first + second
      ↓
     12
      ↓
total = 12
```

之后就可以：

```cpp
return total;
```

---

# 11. `return` 返回变量中的值

假设函数声明为：

```cpp
static int sumOfTwo(int first, int second)
```

函数名前面的：

```cpp
int
```

表示：

> 这个函数最终需要返回一个整数。

因此可以写：

```cpp
int total = first + second;

return total;
```

执行流程：

```text
first + second
      ↓
   得到结果
      ↓
存入 total
      ↓
return total
      ↓
返回给调用者
```

例如：

```cpp
sumOfTwo(7, 5)
```

内部：

```cpp
int total = 7 + 5;
```

得到：

```cpp
total = 12;
```

然后：

```cpp
return total;
```

最终函数调用：

```cpp
sumOfTwo(7, 5)
```

产生值：

```cpp
12
```

---

# 12. 返回值和输出不是一回事

这是初学者特别容易混淆的地方。

## 返回

```cpp
return total;
```

意思是：

> 把值交给调用这个函数的代码。

它本身**不会打印任何东西**。

---

## 输出

```cpp
std::cout << total;
```

意思是：

> 把内容显示在控制台。

它并不等于：

```cpp
return total;
```

因此：

```cpp
return 12;
```

和：

```cpp
std::cout << 12;
```

是两件完全不同的事情。

可以记成：

```text
return
  ↓
把值交给调用者

std::cout
  ↓
把内容显示给人看
```

如果题目测试的是函数的返回值，那么需要：

```cpp
return
```

而不是：

```cpp
std::cout
```

---

# 13. 为什么最好创建变量时立即初始化？

不推荐：

```cpp
int total;

total = first + second;
```

虽然如果在读取 `total` 之前肯定完成了赋值，这段代码本身可以正常工作，但中间存在一个阶段：

```cpp
int total;
```

此时普通局部 `total` 没有一个可靠的初始值。

更直接、安全的方式是：

```cpp
int total = first + second;
```

它同时完成：

```text
创建变量
   +
计算初始值
   +
初始化变量
```

代码也更加清楚。

---

# 14. 关于“未初始化变量”的一个重要例外

需要稍微精确一点：

```cpp
int score;
```

是否会自动变成 `0`，与变量的存储方式有关。

对于这里讨论的普通局部变量：

```cpp
void example() {
    int score;
}
```

`score` 不会自动初始化为 `0`。

但某些具有静态存储期的变量，例如：

```cpp
static int score;
```

会进行零初始化。

因此不要死记成：

> 所有 `int score;` 都一定是垃圾值。

更准确的初学者规则是：

> **普通局部内置类型变量如果没有显式初始化，不要读取它。最好创建时就初始化。**

---

# 15. 本节最重要的代码模板

如果一个函数：

1. 接收两个整数
2. 做计算
3. 把结果存进变量
4. 返回结果

可以写成：

```cpp
static int functionName(int first, int second)
{
    int result = /* expression */;

    return result;
}
```

例如：

```cpp
static int sumOfTwo(int first, int second)
{
    int total = first + second;

    return total;
}
```

---

# 16. 核心知识总结

需要记住下面这几个关系：

```text
int score = 0;
│   │       │
│   │       └─ 初始值
│   └───────── 变量名
└───────────── 类型
```

变量创建：

```cpp
int score = 0;
```

计算并保存：

```cpp
int total = first + second;
```

重新赋值：

```cpp
total = 100;
```

返回变量：

```cpp
return total;
```

以及最重要的区别：

```text
声明：
int score;

初始化：
int score = 0;

赋值：
score = 10;

返回：
return score;

输出：
std::cout << score;
```

可以把完整思维过程记成：

```text
输入参数
   ↓
计算表达式
   ↓
保存到变量
   ↓
return 返回
```
