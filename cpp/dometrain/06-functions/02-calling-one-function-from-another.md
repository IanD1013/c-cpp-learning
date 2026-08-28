# C++ 函数调用：参数传递、返回值与函数定义顺序

## 1. 什么叫“调用函数”

假设我们定义了一个函数：

```cpp
static int doubled(int value) {
    return value * 2;
}
```

要使用这个函数，需要写：

```cpp
doubled(5)
```

这就叫做 **函数调用（function call）**。

基本形式是：

```cpp
函数名(实参1, 实参2, ...)
```

例如：

```cpp
doubled(5)
```

其中：

* `doubled`：函数名
* `5`：传给函数的参数
* `doubled(5)` 整个表达式的结果是 `10`

---

# 2. parameter 和 argument 的区别

这是学习函数时非常重要的一组术语。

函数定义：

```cpp
static int doubled(int value) {
    return value * 2;
}
```

这里的：

```cpp
value
```

叫做 **parameter（形参）**。

而调用：

```cpp
doubled(5)
```

这里的：

```cpp
5
```

叫做 **argument（实参）**。

可以简单理解为：

```text
parameter = 函数定义中的变量
argument  = 调用函数时真正传进去的值
```

例如：

```cpp
static int subtract(int a, int b) {
    return a - b;
}
```

调用：

```cpp
subtract(10, 3);
```

对应关系是：

```text
a = 10
b = 3
```

---

# 3. 函数调用时发生了什么

考虑下面的程序：

```cpp
static int doubled(int value) {
    return value * 2;
}

static int spread(int low, int high) {
    return doubled(high) - doubled(low);
}
```

如果执行：

```cpp
spread(3, 7)
```

首先：

```text
low = 3
high = 7
```

接下来执行：

```cpp
doubled(high)
```

也就是：

```cpp
doubled(7)
```

进入 `doubled` 后：

```text
value = 7
```

执行：

```cpp
return value * 2;
```

得到：

```text
14
```

因此原来的：

```cpp
doubled(high)
```

可以理解成被结果 `14` 替代。

同理：

```cpp
doubled(low)
```

得到：

```text
6
```

于是：

```cpp
return doubled(high) - doubled(low);
```

相当于：

```cpp
return 14 - 6;
```

最终返回：

```text
8
```

因此可以把函数调用理解为：

> 函数执行完成后，它的返回值会出现在原本函数调用所在的位置。

---

# 4. 返回值可以当普通值使用

假设函数返回 `int`：

```cpp
static int doubled(int value) {
    return value * 2;
}
```

那么：

```cpp
doubled(5)
```

本身就是一个 `int` 表达式。

因此它可以出现在任何需要 `int` 的地方。

例如：

```cpp
int x = doubled(5);
```

也可以：

```cpp
int x = doubled(5) + 3;
```

甚至可以：

```cpp
int x = doubled(5) - doubled(2);
```

因为：

```cpp
doubled(5)
```

得到：

```text
10
```

而：

```cpp
doubled(2)
```

得到：

```text
4
```

所以整个表达式相当于：

```cpp
int x = 10 - 4;
```

最终：

```text
x = 6
```

---

# 5. 函数调用可以嵌套在表达式里

例如：

```cpp
return doubled(high) - doubled(low);
```

这里进行了两次函数调用：

```cpp
doubled(high)
doubled(low)
```

然后使用它们的返回值进行减法。

从概念上理解：

```cpp
return doubled(high) - doubled(low);
```

相当于：

```cpp
int first = doubled(high);
int second = doubled(low);

return first - second;
```

只是前一种写法更加简洁。

---

# 6. 参数是按照位置匹配的

这是函数调用非常重要的一条规则：

> argument 和 parameter 是按照位置匹配，而不是按照变量名匹配。

例如：

```cpp
static int subtract(int first, int second) {
    return first - second;
}
```

调用：

```cpp
subtract(10, 3);
```

对应：

```text
first = 10
second = 3
```

如果写：

```cpp
subtract(3, 10);
```

则对应：

```text
first = 3
second = 10
```

结果就完全不同。

---

## 变量名字不会参与参数匹配

例如：

```cpp
int first = 100;
int second = 20;
```

调用：

```cpp
subtract(second, first);
```

仍然按照位置传递：

```text
函数第一个参数 first  = 调用中的 second = 20
函数第二个参数 second = 调用中的 first  = 100
```

所以结果是：

```text
20 - 100 = -80
```

即使函数参数名和调用处变量名相同或不同，都没有关系。

真正决定对应关系的是：

```text
第 1 个 argument → 第 1 个 parameter
第 2 个 argument → 第 2 个 parameter
第 3 个 argument → 第 3 个 parameter
...
```

---

# 7. 同类型参数特别容易传反

例如：

```cpp
static int difference(int low, int high) {
    return high - low;
}
```

下面两个调用都能正常编译：

```cpp
difference(3, 10);
difference(10, 3);
```

因为两个参数都是：

```cpp
int
```

编译器无法判断你是不是把它们写反了。

但结果分别是：

```text
10 - 3 = 7
3 - 10 = -7
```

所以：

> 当一个函数有多个相同类型的参数时，参数顺序尤其重要。

例如：

```cpp
raiseTo(base, exponent)
```

和：

```cpp
raiseTo(exponent, base)
```

虽然都能编译，但数学意义完全不同。

---

# 8. 一个函数调用另一个函数

函数之间可以互相调用，只要被调用函数已经被声明。

例如：

```cpp
static int doubled(int value) {
    return value * 2;
}

static int spread(int low, int high) {
    return doubled(high) - doubled(low);
}
```

这里：

```cpp
spread()
```

调用了：

```cpp
doubled()
```

因此可以理解为：

```text
spread
  ↓
doubled
```

这是一种非常常见的程序设计方式。

复杂程序通常不会把所有逻辑都写进一个大函数，而是拆成很多小函数。

例如：

```text
main
 ↓
powerDifference
 ↓
raiseTo
```

其中：

* `raiseTo` 负责“计算整数幂”
* `powerDifference` 负责“计算两个幂的差”

这种设计叫做 **功能分解（decomposition）**。

---

# 9. 为什么被调用函数通常需要写在前面

考虑：

```cpp
static int spread(int low, int high) {
    return doubled(high) - doubled(low);
}

static int doubled(int value) {
    return value * 2;
}
```

如果前面没有声明 `doubled`，这里通常会出现编译错误：

```cpp
doubled(high)
```

原因是 C++ 编译器在处理 `spread` 时，还不知道：

```cpp
doubled
```

是什么。

所以一种简单写法是：

```cpp
static int doubled(int value) {
    return value * 2;
}

static int spread(int low, int high) {
    return doubled(high) - doubled(low);
}
```

也就是：

> 先定义 helper function，再定义调用它的函数。

---

# 10. 更正式的做法：函数声明

实际上，函数不一定必须完整定义在调用者前面。

只要提前提供 **函数声明（function declaration / prototype）** 即可。

例如：

```cpp
static int doubled(int value);

static int spread(int low, int high) {
    return doubled(high) - doubled(low);
}

static int doubled(int value) {
    return value * 2;
}
```

这一行：

```cpp
static int doubled(int value);
```

告诉编译器：

> 后面会存在一个叫 `doubled` 的函数，它接收一个 `int`，返回一个 `int`。

因此编译器看到：

```cpp
doubled(high)
```

时，就知道这是合法的函数调用。

---

# 11. 函数调用语句也可以丢弃返回值

假设：

```cpp
static int doubled(int value) {
    return value * 2;
}
```

可以写：

```cpp
doubled(5);
```

函数确实会执行，并计算：

```text
10
```

但是这个返回值没有被保存或使用。

也就是说：

```cpp
doubled(5);
```

相当于：

```text
执行 doubled
↓
得到 10
↓
直接丢掉 10
```

因此对于纯计算函数来说，这样写通常没有意义。

例如：

```cpp
raiseTo(2, 3);
```

会计算：

```text
8
```

但如果没有：

```cpp
int result = raiseTo(2, 3);
```

或者：

```cpp
return raiseTo(2, 3);
```

那么 `8` 会被直接丢弃。

---

# 12. 本题中的 helper function：`raiseTo`

题目要求实现：

```cpp
raiseTo(int base, int exponent)
```

含义是：

```text
base^exponent
```

例如：

```cpp
raiseTo(2, 3)
```

应该计算：

```text
2 × 2 × 2 = 8
```

因此一个简单思路是：

```text
product = 1

重复 exponent 次：
    product = product × base

返回 product
```

例如：

```cpp
raiseTo(3, 4)
```

过程：

```text
开始：
product = 1

第 1 次：
product = 1 × 3 = 3

第 2 次：
product = 3 × 3 = 9

第 3 次：
product = 9 × 3 = 27

第 4 次：
product = 27 × 3 = 81
```

最后：

```text
3^4 = 81
```

---

# 13. 为什么乘积必须从 1 开始

计算乘法累积时：

```cpp
int product = 1;
```

而不是：

```cpp
int product = 0;
```

因为如果从 0 开始：

```text
0 × base = 0
```

之后永远都会是：

```text
0
```

例如：

```text
0 × 2 = 0
0 × 2 = 0
0 × 2 = 0
```

所以乘法累积的初始值应该使用：

```text
1
```

这个 `1` 可以理解为乘法中的“空操作值”。

类似地：

```text
加法累计 → 从 0 开始
乘法累计 → 从 1 开始
```

例如：

```cpp
int sum = 0;
int product = 1;
```

---

# 14. 为什么 `exponent == 0` 时结果是 1

数学规定：

```text
x^0 = 1
```

因此：

```cpp
raiseTo(4, 0)
```

应该返回：

```text
1
```

我们的循环算法刚好自然满足这个规则。

假设：

```cpp
int product = 1;

for (int i = 0; i < exponent; ++i) {
    product *= base;
}
```

如果：

```text
exponent = 0
```

条件一开始就是：

```cpp
i < 0
```

也就是：

```text
0 < 0
```

为 false。

所以循环一次也不会执行。

于是：

```text
product
```

仍然是：

```text
1
```

最终返回：

```text
1
```

这是一种非常漂亮的算法设计，因为不需要专门写：

```cpp
if (exponent == 0)
```

---

# 15. 本题真正考察的参数顺序

题目要求：

```cpp
powerDifference(int base, int exponent)
```

计算：

```text
base^exponent - exponent^base
```

也就是：

```cpp
raiseTo(base, exponent)
-
raiseTo(exponent, base)
```

注意两个调用：

```cpp
raiseTo(base, exponent)
```

和：

```cpp
raiseTo(exponent, base)
```

参数顺序正好相反。

例如：

```cpp
powerDifference(2, 3)
```

第一个调用：

```cpp
raiseTo(2, 3)
```

得到：

```text
2^3 = 8
```

第二个调用：

```cpp
raiseTo(3, 2)
```

得到：

```text
3^2 = 9
```

因此：

```text
8 - 9 = -1
```

---

# 16. 测试几个例子

## `powerDifference(2, 3)`

```text
2^3 - 3^2
= 8 - 9
= -1
```

---

## `powerDifference(3, 2)`

```text
3^2 - 2^3
= 9 - 8
= 1
```

---

## `powerDifference(5, 5)`

```text
5^5 - 5^5
= 0
```

---

## `powerDifference(4, 0)`

```text
4^0 - 0^4
```

因为：

```text
4^0 = 1
```

而：

```text
0^4 = 0
```

所以：

```text
1 - 0 = 1
```

---

## `powerDifference(0, 3)`

```text
0^3 - 3^0
= 0 - 1
= -1
```

---

# 17. 本节核心知识总结

### 函数调用

```cpp
function(argument1, argument2)
```

---

### 参数按位置对应

```text
第 1 个 argument → 第 1 个 parameter
第 2 个 argument → 第 2 个 parameter
```

变量名字不会参与匹配。

---

### 返回值会替代函数调用表达式

```cpp
int x = doubled(5);
```

可以理解成：

```cpp
int x = 10;
```

---

### 返回 `int` 的函数可以出现在任何需要 `int` 的表达式中

```cpp
return doubled(high) - doubled(low);
```

---

### helper function 通常应该先定义

```cpp
helper()

caller() {
    helper();
}
```

或者提前声明：

```cpp
int helper(...);
```

---

### 乘法累计

通常：

```cpp
int product = 1;
```

然后不断：

```cpp
product *= value;
```

---

### `base^exponent`

可以通过重复乘法实现：

```text
product = 1
重复 exponent 次：
    product *= base
```

---

### 本题最容易出错的地方

不是循环，而是：

```cpp
raiseTo(base, exponent)
```

和：

```cpp
raiseTo(exponent, base)
```

参数顺序不同。

本题本质上同时考察：

```text
函数调用
参数按位置传递
返回值
helper function
for 循环
累积变量
函数定义顺序
```
