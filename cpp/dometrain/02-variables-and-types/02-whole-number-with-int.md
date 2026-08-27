# C++ `int`、整数常量与变量更新

## 1. `int` 用来保存什么？

在 C++ 中：

```cpp
int
```

用于保存**没有小数部分的整数**。

例如：

```cpp
int age = 20;
int score = 100;
int temperature = -5;
```

这些都可以使用 `int`。

但下面这种带小数的值：

```cpp
3.14
2.5
-0.75
```

不属于整数，不应该把它们当作普通 `int` 值来理解。

---

# 2. `int` 的取值范围

在本课程所使用的平台上，`int` 占：

```text
4 bytes
```

也就是：

```text
32 bits
```

因此它可以表示的整数范围是：

```text
-2147483648
到
2147483647
```

也就是大约：

```text
-21 亿 到 +21 亿
```

例如：

```cpp
int x = 100;
int y = -500;
int z = 2000000000;
```

都还在这个范围内。

---

# 3. C++ 中整数类型的大小并不是永远固定的

严格来说，C++ 标准并没有规定：

```cpp
int
```

必须永远是 4 字节。

不同平台上，整数类型的大小可能存在差异。

C++ 还有其他整数类型，例如：

```cpp
short
long
long long
```

以及无符号整数类型：

```cpp
unsigned int
```

不过当前课程主要使用：

```cpp
int
```

并且保证测试数据都不会超出它的安全范围。

因此目前可以重点记住：

> 在当前课程环境中，`int` 是 4 字节的有符号整数。

---

# 4. 什么是整数常量（Integer Literal）？

像下面这些直接写在代码里的整数：

```cpp
0
60
2000
-5
```

其中：

```cpp
0
60
2000
```

这样的数字称为：

> **integer literal（整数常量 / 整数字面量）**

例如：

```cpp
int count = 3;
```

这里：

```cpp
3
```

就是一个整数常量。

---

## 5. Literal 是什么意思？

Literal 可以理解成：

> 直接写在源代码里的固定值。

例如：

```cpp
int age = 20;
```

这里：

```cpp
20
```

就是直接写死在程序中的值。

又例如：

```cpp
int seconds = 60;
```

这里的：

```cpp
60
```

也是整数常量。

它们在编译时就已经确定。

---

# 6. 默认情况下，普通整数常量通常是 `int`

例如：

```cpp
0
60
2000
```

这些数只要能够被 `int` 表示，通常都会被当作：

```cpp
int
```

类型。

因此：

```cpp
int count = 3;
```

左右两边的类型非常自然：

```text
int count = 3;
│           │
│           └─ int 类型的整数常量
└───────────── int 类型变量
```

---

# 7. 变量创建之后，可以修改它

例如：

```cpp
int count = 3;
```

这里首先创建：

```cpp
count
```

并让它保存：

```text
3
```

之后可以写：

```cpp
count = count + 5;
```

这不会创建新变量。

而是：

> 修改原来的 `count`。

---

# 8. 赋值语句的执行顺序

来看：

```cpp
count = count + 5;
```

假设原来：

```text
count = 3
```

执行时，程序先计算右边：

```cpp
count + 5
```

也就是：

```cpp
3 + 5
```

得到：

```text
8
```

然后再把：

```text
8
```

存回左边的：

```cpp
count
```

最终：

```text
count = 8
```

完整过程：

```text
原来：
count = 3

执行：
count = count + 5;
        │
        └─ 先读取旧的 count

3 + 5
  ↓
  8
  ↓
重新存入 count

最终：
count = 8
```

---

# 9. 赋值中的右边使用的是“旧值”

这是理解更新变量的关键。

代码：

```cpp
count = count + 5;
```

并不是逻辑矛盾。

虽然数学中写：

```text
x = x + 5
```

看起来不可能成立，但编程中的：

```cpp
=
```

并不是数学中的“相等”。

它表示：

> 把右边计算出的结果存进左边变量。

所以：

```cpp
count = count + 5;
```

真正表达的是：

```text
读取 count 当前值
        ↓
加 5
        ↓
把结果重新存回 count
```

---

# 10. 初始化与赋值再次区分

下面两句非常像：

```cpp
int count = 3;
count = 8;
```

但它们不是同一种操作。

---

## 第一行：初始化

```cpp
int count = 3;
```

意思是：

> 创建一个新的变量 `count`，并让它一开始保存 `3`。

这是：

```text
declaration + initialization
声明 + 初始化
```

---

## 第二行：赋值

```cpp
count = 8;
```

意思是：

> `count` 已经存在，把原来的值替换为 `8`。

这是：

```text
assignment
赋值
```

---

# 11. 更新变量时不要再次写类型

正确：

```cpp
int count = 3;

count = count + 5;
```

错误：

```cpp
int count = 3;

int count = count + 5;
```

第二个：

```cpp
int count
```

并不是“更新 count”。

它是在说：

> 再声明一个叫 `count` 的 `int` 变量。

如果它和第一个变量处于同一个作用域中，编译器会报：

```text
redefinition
重新定义
```

---

# 12. 为什么会发生 redefinition？

考虑：

```cpp
int count = 3;
int count = 8;
```

你实际上是在同一个 block 中说：

```text
创建一个叫 count 的变量
再创建一个也叫 count 的变量
```

而同一个作用域中不能这样重复声明同名局部变量。

因此编译器会报错。

---

# 13. 判断是在“创建变量”还是“修改变量”

可以看有没有类型。

创建变量：

```cpp
int count = 3;
```

有：

```cpp
int
```

说明是在声明一个变量。

修改变量：

```cpp
count = 8;
```

没有重新写：

```cpp
int
```

说明是在更新已经存在的变量。

可以记：

```text
int count = 3;
↑
有类型 → 创建变量

count = 8;
↑
无类型 → 修改已有变量
```

---

# 14. 用旧值更新同一个变量

下面这种模式在编程里非常常见：

```cpp
int counter = 10;

counter = counter + 5;
```

执行后：

```text
counter = 15
```

然后：

```cpp
counter = counter + 3;
```

执行后：

```text
counter = 18
```

也就是说，同一个变量可以不断根据自己的旧值更新。

---

# 15. Running Total：运行中的累计值

这种写法：

```cpp
int counter = start;

counter = counter + firstChange;
counter = counter + secondChange;
```

就是非常典型的：

> running total（运行中的累计值）

例如：

```text
start = 10
firstChange = 5
secondChange = 3
```

开始：

```text
counter = 10
```

第一次更新：

```cpp
counter = counter + firstChange;
```

也就是：

```text
counter = 10 + 5
        = 15
```

第二次更新：

```cpp
counter = counter + secondChange;
```

也就是：

```text
counter = 15 + 3
        = 18
```

最终：

```text
counter = 18
```

---

# 16. 更新顺序非常重要

如果题目说：

> 按顺序进行两个修改

那么代码应该真的表现出这个过程：

```cpp
counter = counter + firstChange;
counter = counter + secondChange;
```

而不是直接把所有东西一次算完：

```cpp
int counter = start + firstChange + secondChange;
```

从最终数学结果来看，两者可能相同。

但它们练习的是不同的知识。

本题重点不是“算出总和”，而是练习：

> **一个变量如何连续被赋值和更新。**

---

# 17. 为什么不能直接把三个值一次加完？

比如：

```cpp
int counter = start + firstChange + secondChange;
```

数学上当然能得到同样结果。

但是这跳过了本题想训练的两个 assignment：

```cpp
counter = counter + firstChange;

counter = counter + secondChange;
```

题目正在练习的是：

```text
创建变量
   ↓
第一次修改
   ↓
第二次修改
   ↓
返回最终值
```

所以需要显式写出变量变化的过程。

---

# 18. `+=` 是什么？

正常 C++ 中，可以把：

```cpp
counter = counter + firstChange;
```

简写成：

```cpp
counter += firstChange;
```

两者的基本意思都是：

```text
把 firstChange 加到 counter 上
```

例如：

```cpp
int x = 10;

x += 5;
```

相当于：

```cpp
x = x + 5;
```

最终：

```text
x = 15
```

---

# 19. 为什么本题不能使用 `+=`？

本题明确要求练习完整的赋值形式：

```cpp
counter = counter + firstChange;
```

因此：

```cpp
+=
```

被禁止使用。

也就是说，本题不是因为 `+=` 不正确，而是因为：

> 教学目标就是让你练习 `=` assignment。

所以应该写：

```cpp
counter = counter + firstChange;
counter = counter + secondChange;
```

而不是：

```cpp
counter += firstChange;
counter += secondChange;
```

---

# 20. 正数、负数和 0 都可以直接相加

假设：

```text
start = 9
firstChange = -4
secondChange = -5
```

开始：

```text
counter = 9
```

第一次：

```text
counter = 9 + (-4)
        = 5
```

第二次：

```text
counter = 5 + (-5)
        = 0
```

所以最终：

```text
0
```

完全不需要：

```cpp
if
```

---

# 21. Counter 可以变成负数

例如：

```text
start = 3
firstChange = -10
secondChange = 2
```

第一次：

```text
3 + (-10) = -7
```

第二次：

```text
-7 + 2 = -5
```

最终：

```text
-5
```

变量并不会因为名字叫：

```cpp
counter
```

就必须保持非负。

它只是：

```cpp
int
```

所以可以保存：

```text
正数
0
负数
```

---

# 22. 加 0 不需要特殊处理

例如：

```text
start = 8
firstChange = 0
secondChange = -3
```

第一次：

```text
8 + 0 = 8
```

第二次：

```text
8 + (-3) = 5
```

所以：

```text
5
```

不需要写：

```cpp
if (firstChange == 0)
```

因为普通的加法已经自动正确处理 `0`。

---

# 23. 每一步计算都要注意 `int` 范围

例如：

```text
start = 900000000
firstChange = 100000000
secondChange = -500000000
```

第一步：

```text
900000000 + 100000000
= 1000000000
```

第二步：

```text
1000000000 - 500000000
= 500000000
```

这些值都处于：

```text
-2147483648
到
2147483647
```

范围内。

所以在本课程环境下使用：

```cpp
int
```

没有问题。

---

# 24. 中间结果也很重要

不要只看最终结果是否在范围内。

如果某个计算的**中间结果**超出 `int` 范围，也可能发生整数溢出。

例如假设：

```text
start = 2100000000
firstChange = 100000000
secondChange = -100000000
```

最终数学结果可能看起来还是：

```text
2100000000
```

但第一步：

```text
2100000000 + 100000000
= 2200000000
```

已经超过：

```text
2147483647
```

所以这就有问题。

本题特别说明：

> 所有测试过程中每一步都保持在 `int` 的安全范围内。

因此不需要额外处理溢出。

---

# 25. 本节代码模式

这一节最值得记住的是：

```cpp
int value = initialValue;

value = value + firstChange;
value = value + secondChange;

return value;
```

执行过程：

```text
initialValue
     ↓
   value
     ↓
+ firstChange
     ↓
更新 value
     ↓
+ secondChange
     ↓
再次更新 value
     ↓
return
```

---

# 26. 初始化、赋值和重新声明对比

## 初始化

```cpp
int count = 3;
```

含义：

> 创建 `count`，并给它第一个值。

---

## 赋值

```cpp
count = count + 5;
```

含义：

> 计算新值并覆盖旧值。

---

## 错误的重新声明

```cpp
int count = 3;
int count = 8;
```

含义：

> 尝试在同一作用域再次创建同名变量。

结果：

```text
编译错误：redefinition
```

---

# 27. 核心总结

### `int`

保存整数：

```cpp
int x = 10;
```

当前课程环境范围：

```text
-2147483648 ~ 2147483647
```

---

### Integer literal

直接写在代码中的整数：

```cpp
0
60
2000
```

---

### 初始化

```cpp
int count = 3;
```

---

### 赋值

```cpp
count = 8;
```

---

### 根据旧值更新变量

```cpp
count = count + 5;
```

---

### 连续更新

```cpp
int counter = start;

counter = counter + firstChange;
counter = counter + secondChange;
```

---

### 不要重复类型

错误：

```cpp
int count = 3;
int count = count + 5;
```

正确：

```cpp
int count = 3;
count = count + 5;
```

---

### 本节最重要的思维模型

```text
创建变量
   ↓
读取旧值
   ↓
计算新值
   ↓
用 = 存回同一个变量
   ↓
继续下一次更新
   ↓
返回最终值
```
