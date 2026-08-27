# C++ `double`：小数、浮点运算与整数除法

## 1. `double` 是什么？

在 C++ 中：

* `int` 用来保存整数
* `double` 用来保存带小数部分的数值

例如：

```cpp
int count = 10;

double half = 2.5;
double price = 19.99;
double temperature = -3.75;
```

声明变量时的基本格式仍然是：

```cpp
类型 变量名 = 初始值;
```

例如：

```cpp
double half = 2.5;
```

其中：

* `double`：变量的数据类型
* `half`：变量名
* `2.5`：变量的初始值

---

# 2. 什么是 `double literal`？

直接写在代码里的数值叫做 **literal（字面量）**。

例如：

```cpp
10
25
0
```

这些没有小数点的数字通常属于：

```cpp
int
```

而带有小数点的数字：

```cpp
2.5
0.2
4.0
0.25
```

通常属于：

```cpp
double
```

因此：

```cpp
double half = 2.5;
```

这里的 `2.5` 是一个 **double literal（双精度浮点数字面量）**。

---

# 3. `double` 可以参与小数运算

例如：

```cpp
double half = 2.5;
double tenth = half * 0.2;
```

计算过程是：

```text
half = 2.5

tenth = 2.5 × 0.2
      = 0.5
```

因此：

```cpp
tenth == 0.5
```

因为参与运算的是 `double`，所以小数部分会被保留下来。

---

# 4. 一个非常重要的陷阱：整数除法

学习 C++ 时必须特别注意：

```cpp
1 / 4
```

并不会得到：

```text
0.25
```

而是得到：

```text
0
```

原因是：

```cpp
1
4
```

两边都是 `int`。

所以 C++ 执行的是：

```text
整数 ÷ 整数 → 整数除法
```

也就是：

```cpp
1 / 4
```

实际上相当于：

```text
0.25 → 丢掉小数部分 → 0
```

所以结果是：

```cpp
0
```

---

# 5. 把结果存进 `double` 也救不回来

例如：

```cpp
double result = 1 / 4;
```

很多初学者可能会以为：

```cpp
result == 0.25
```

实际上：

```cpp
result == 0.0
```

这是因为计算顺序是：

```text
1 / 4
↓
两个操作数都是 int
↓
先执行整数除法
↓
得到 0
↓
再把 0 转成 double
↓
0.0
```

关键点是：

> **变量是什么类型，并不会反过来改变右边表达式的计算方式。**

也就是说：

```cpp
double result = 1 / 4;
```

不是：

```text
因为 result 是 double
→ 所以 1 / 4 用小数计算
```

而是：

```text
先计算右边
→ 再把计算结果存进左边
```

---

# 6. 如何正确计算 `1 / 4`？

至少让其中一个操作数成为 `double`：

```cpp
1.0 / 4
```

或者：

```cpp
1 / 4.0
```

或者：

```cpp
1.0 / 4.0
```

这些都会得到：

```text
0.25
```

因为只要运算中出现 `double`，C++ 就会进行浮点数运算。

例如：

```cpp
double quarter = 1.0 / 4;
```

结果：

```text
quarter = 0.25
```

---

# 7. 更简单的方法：直接使用 `0.25`

因为：

```text
1 / 4 = 0.25
```

所以完全可以直接写：

```cpp
double quarter = 0.25;
```

然后：

```cpp
double result = value * 0.25;
```

例如：

```cpp
double value = 6;
double result = value * 0.25;
```

计算：

```text
6 × 0.25 = 1.5
```

因此：

```cpp
result == 1.5
```

---

# 8. 为什么不能把小数暂时存进 `int`？

假设我们写：

```cpp
int quarter = 0.25;
```

这是错误的思路。

因为 `int` 不能保存小数部分。

`0.25` 转成 `int` 后会变成：

```text
0
```

于是：

```cpp
int quarter = 0.25;
```

实际上：

```text
quarter = 0
```

接下来：

```cpp
double result = value * quarter;
```

就变成：

```text
value × 0
```

结果当然始终是：

```text
0
```

所以：

> **只要计算需要保留小数，中间变量也必须使用能够保存小数的类型，例如 `double`。**

---

# 9. 参数也不要随便转成 `int`

例如原本有：

```cpp
double value = 0.5;
```

如果写：

```cpp
int n = value;
```

那么：

```text
0.5 → 0
```

小数部分立即丢失。

之后再进行计算：

```cpp
double result = n * 0.25;
```

实际上已经变成：

```text
0 × 0.25 = 0
```

而正确答案应该是：

```text
0.5 × 0.25 = 0.125
```

所以像：

```cpp
0.5
-3.5
1.25
```

这样的数值，在整个计算过程中都应该保持为 `double`。

---

# 10. 一个重要原则：不要过早丢失精度

假设原始数据是：

```cpp
double value = 0.5;
```

正确：

```cpp
double result = value * 0.25;
```

数据流：

```text
0.5
 ↓
double
 ↓
× 0.25
 ↓
0.125
```

错误：

```cpp
int temp = value;
double result = temp * 0.25;
```

数据流：

```text
0.5
 ↓
转成 int
 ↓
0
 ↓
× 0.25
 ↓
0
```

一旦小数部分已经因为转换成 `int` 而消失，之后再转换回 `double` 也无法恢复。

例如：

```cpp
double value = 0.5;

int temp = value;        // 0
double result = temp;    // 0.0
```

`0.0` 并不会重新变成 `0.5`。

---

# 11. 返回 `double` 的函数

函数也可以返回 `double`。

例如：

```cpp
static double halfOf(double value) {
    return value * 0.5;
}
```

这里：

```cpp
static double halfOf(double value)
```

可以拆成：

```text
static
double
halfOf
(double value)
```

其中：

* `double`：返回值类型
* `halfOf`：函数名
* `double value`：函数接收一个 `double` 参数

所以：

```cpp
halfOf(10)
```

返回：

```text
5.0
```

而：

```cpp
halfOf(3)
```

返回：

```text
1.5
```

---

# 12. `return` 中可以直接进行计算

不一定非要创建中间变量。

例如：

```cpp
static double halfOf(double value) {
    double result = value * 0.5;
    return result;
}
```

可以直接简化成：

```cpp
static double halfOf(double value) {
    return value * 0.5;
}
```

两者效果相同。

如果中间变量没有让代码更容易理解，通常直接返回表达式更加简洁。

---

# 13. 为什么 `4.0` 最后可能显示成 `4`？

一个 `double` 的值即使数学意义上是：

```text
4.0
```

输出程序也不一定显示：

```text
4.0
```

它可能显示：

```text
4
```

例如题目的 runner 会：

* 最多打印 15 位有效数字
* 删除没有必要的末尾 `0`

所以：

```text
4.0  → 4
1.50 → 1.5
0.125 → 0.125
```

因此：

> **输出显示为 `4` 并不代表结果的数据类型是 `int`。**

例如：

```cpp
double result = 4.0;
```

`result` 仍然是：

```cpp
double
```

只是打印时显示为了：

```text
4
```

---

# 14. 类型和“显示形式”是两回事

一定要区分：

```text
变量的数据类型
```

和：

```text
控制台如何显示这个变量
```

例如：

```cpp
double value = 4.0;
```

变量类型仍然是：

```text
double
```

即使输出为：

```text
4
```

所以不能仅仅根据输出有没有：

```text
.0
```

来判断变量是不是 `double`。

---

# 15. 本节核心总结

## `int`

保存整数，例如：

```cpp
int x = 4;
```

---

## `double`

保存可以带小数的数值，例如：

```cpp
double x = 4.5;
```

---

## 整数字面量

```cpp
1
4
16
```

通常是：

```cpp
int
```

---

## 小数字面量

```cpp
0.25
2.5
4.0
```

通常是：

```cpp
double
```

---

## 整数除法

```cpp
1 / 4
```

结果：

```text
0
```

因为：

```text
int / int → int
```

---

## 浮点除法

```cpp
1.0 / 4
```

结果：

```text
0.25
```

因为运算中存在 `double`。

---

## 不要把需要小数精度的数据存进 `int`

错误：

```cpp
int quarter = 0.25;
```

因为最终：

```text
quarter == 0
```

正确：

```cpp
double quarter = 0.25;
```

---

## 最值得记住的一句话

> **C++ 会先根据表达式中操作数的类型进行计算，再把最终结果存入变量。**

所以：

```cpp
double x = 1 / 4;
```

仍然得到：

```text
0.0
```

而：

```cpp
double x = 1.0 / 4;
```

才得到：

```text
0.25
```
