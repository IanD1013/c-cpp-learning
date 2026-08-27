# C++ 比较运算符与浮点数比较

## 1. 比较运算的本质：产生一个 `bool`

在 C++ 中，**比较运算符（comparison operators）** 用来比较两个值。

比较表达式最终产生的结果只有两种：

```cpp
true
false
```

也就是说，比较表达式的类型是：

```cpp
bool
```

例如：

```cpp
int count = 7;
int limit = 7;

bool same    = count == limit;  // true
bool differs = count != limit;  // false
bool under   = count < limit;   // false
bool atMost  = count <= limit;  // true
```

可以把比较表达式理解成：

> “问程序一个是/否问题，程序返回 `true` 或 `false`。”

例如：

```cpp
count == limit
```

实际上是在问：

> `count` 和 `limit` 是否相等？

因为：

```cpp
count = 7
limit = 7
```

所以结果为：

```cpp
true
```

---

# 2. C++ 常见的比较运算符

C++ 中最重要的六种比较运算符是：

| 运算符  | 含义    | 示例       |
| ---- | ----- | -------- |
| `==` | 等于    | `a == b` |
| `!=` | 不等于   | `a != b` |
| `<`  | 小于    | `a < b`  |
| `<=` | 小于或等于 | `a <= b` |
| `>`  | 大于    | `a > b`  |
| `>=` | 大于或等于 | `a >= b` |

例如：

```cpp
int a = 5;
int b = 10;
```

那么：

```cpp
a == b   // false
a != b   // true

a < b    // true
a <= b   // true

a > b    // false
a >= b   // false
```

---

# 3. `<=` 和 `<` 的区别

特别需要理解：

```cpp
<
```

和：

```cpp
<=
```

并不一样。

例如：

```cpp
int value = 10;
```

那么：

```cpp
value < 10
```

结果是：

```cpp
false
```

因为 10 并不“小于”10。

但是：

```cpp
value <= 10
```

结果是：

```cpp
true
```

因为：

> 10 小于或等于 10。

其中“等于”这一部分让边界值也被包含进去。

类似地：

```cpp
value > 10
```

不包含 10 本身，而：

```cpp
value >= 10
```

包含 10。

---

# 4. 比较结果可以保存到 `bool` 变量

因为比较表达式本身就是一个 `bool`，所以可以直接保存：

```cpp
int age = 20;

bool adult = age >= 18;
```

执行：

```cpp
age >= 18
```

得到：

```cpp
true
```

于是：

```cpp
adult == true
```

---

# 5. 比较表达式可以直接放进 `if`

通常我们并不需要先保存到变量。

例如：

```cpp
int age = 20;

if (age >= 18) {
    std::cout << "Adult";
}
```

这里：

```cpp
age >= 18
```

本身就会得到：

```cpp
true
```

所以完全没有必要写成：

```cpp
bool adult = age >= 18;

if (adult) {
    std::cout << "Adult";
}
```

两种写法都正确，但第一种通常更简洁。

---

# 6. `==` 和 `=` 完全不是一回事

这是 C++ 初学阶段最重要、也最容易犯的错误之一。

## `==`：比较

```cpp
a == b
```

表示：

> a 是否等于 b？

它不会修改变量。

例如：

```cpp
int a = 5;
int b = 5;

bool result = a == b;
```

结果：

```cpp
result == true
```

---

## `=`：赋值

```cpp
a = b;
```

表示：

> 把 `b` 的值存进 `a`。

例如：

```cpp
int a = 5;
int b = 10;

a = b;
```

执行以后：

```cpp
a == 10
b == 10
```

因此：

```cpp
=
```

和：

```cpp
==
```

完全不同。

---

# 7. 一个危险的错误：在 `if` 中误写 `=`

假设本来想写：

```cpp
if (left == right) {
    ...
}
```

结果不小心写成：

```cpp
if (left = right) {
    ...
}
```

这不是在比较。

它会先执行：

```cpp
left = right;
```

也就是：

> 把 `right` 的值赋给 `left`。

然后整个赋值表达式的结果继续被当成条件判断。

例如：

```cpp
int left = 0;
int right = 5;

if (left = right) {
    std::cout << "yes";
}
```

执行：

```cpp
left = right
```

后：

```cpp
left == 5
```

赋值表达式本身的值也是 `5`。

在条件中，非零整数会被转换为：

```cpp
true
```

所以 `if` 会执行。

---

## 如果赋值的是 0

例如：

```cpp
int left = 10;
int right = 0;

if (left = right) {
    std::cout << "yes";
}
```

执行后：

```cpp
left == 0
```

而 `0` 转换为 `bool` 是：

```cpp
false
```

所以 `if` 不执行。

---

## 记忆方式

```cpp
=    给东西
==   问问题
```

即：

```cpp
x = 5;
```

> 把 5 给 x。

而：

```cpp
x == 5
```

> x 是不是 5？

---

# 8. 为什么 `double` 不适合直接使用 `==`

对于整数：

```cpp
int a = 5;
int b = 5;

a == b
```

通常完全没有问题。

但对于：

```cpp
double
```

情况不同。

例如：

```cpp
double sum = 0.1 + 0.2;
```

数学上：

```text
0.1 + 0.2 = 0.3
```

但是：

```cpp
bool exact = sum == 0.3;
```

很可能得到：

```cpp
false
```

为什么？

---

# 9. 浮点数无法精确表示很多十进制小数

计算机内部通常使用 **二进制浮点数** 保存 `double`。

问题是：

> 很多十进制小数无法用有限长度的二进制精确表示。

这和十进制中：

```text
1 / 3 = 0.333333333...
```

类似。

无论写多少位：

```text
0.333333
```

都只是：

```text
1 / 3
```

的近似值。

在二进制中：

```text
0.1
0.2
0.3
```

也通常不能被精确表示。

因此计算机保存的是非常接近它们的二进制近似值。

于是：

```cpp
0.1 + 0.2
```

计算结果可能和内部表示的：

```cpp
0.3
```

存在极其微小的差别。

因此：

```cpp
0.1 + 0.2 == 0.3
```

可能得到：

```cpp
false
```

---

# 10. 比较 `double`：不要问“完全一样吗”

对于浮点数，更合适的问题通常不是：

> 两个数是否完全相等？

而是：

> 两个数是否足够接近？

例如：

```cpp
double actual = 0.1 + 0.2;
double expected = 0.3;
```

我们可以计算两者之间的距离：

```cpp
actual - expected
```

但是这个差可能是正数，也可能是负数。

例如：

```text
actual = 0.300001
expected = 0.300000

difference = 0.000001
```

或者：

```text
actual = 0.299999
expected = 0.300000

difference = -0.000001
```

我们真正关心的是“距离有多远”，而不是方向。

因此使用绝对值。

---

# 11. 使用 `std::abs`

C++ 中可以通过：

```cpp
#include <cmath>
```

使用：

```cpp
std::abs(...)
```

例如：

```cpp
double difference = -0.000001;

double distance = std::abs(difference);
```

得到：

```cpp
0.000001
```

因此：

```cpp
std::abs(actual - expected)
```

表示：

> `actual` 与 `expected` 之间的绝对距离。

注意：现代 C++ 的 `std::abs` 有针对整数和浮点数的重载。使用 `<cmath>` 时，可以正确处理 `double`：

```cpp
std::abs(0.5)
```

结果仍然是：

```cpp
0.5
```

---

# 12. 使用 tolerance（容差）

我们可以规定：

```cpp
double tolerance = 0.000001;
```

它表示：

> 如果两个数之间的距离不超过 `0.000001`，就认为它们足够接近。

于是：

```cpp
bool within =
    std::abs(actual - expected) <= tolerance;
```

例如：

```cpp
#include <cmath>

double sum = 0.1 + 0.2;

bool exact =
    sum == 0.3;

bool within =
    std::abs(sum - 0.3) <= 0.000001;
```

通常：

```cpp
exact   // false
within  // true
```

---

# 13. 为什么这里必须使用 `<=`

假设：

```cpp
tolerance = 0.01
```

而两个值之间的距离恰好为：

```text
0.01
```

题目的定义是：

> difference equal to tolerance exactly counts as a match.

也就是说：

```text
distance == tolerance
```

也应该算匹配。

因此应该写：

```cpp
distance <= tolerance
```

而不是：

```cpp
distance < tolerance
```

区别：

```text
distance < tolerance
```

表示必须严格小于容差。

而：

```text
distance <= tolerance
```

表示：

```text
距离小于容差
或
距离刚好等于容差
```

都可以。

---

# 14. 浮点数近似比较的核心公式

以后看到：

> 判断两个浮点数是否足够接近

最基本的模型就是：

```cpp
std::abs(a - b) <= tolerance
```

也就是：

```text
|a - b| <= tolerance
```

数学意义：

> a 和 b 之间的距离不能超过允许误差。

---

# 15. 本题实际上要比较什么？

题目给出：

```cpp
left
right
target
tolerance
```

首先计算：

```cpp
left + right
```

然后判断这个结果和：

```cpp
target
```

是否足够接近。

所以真正比较的是：

```cpp
left + right
```

和：

```cpp
target
```

它们之间的距离：

```cpp
std::abs((left + right) - target)
```

如果：

```cpp
std::abs((left + right) - target) <= tolerance
```

那么返回：

```cpp
true
```

否则返回：

```cpp
false
```

---

# 16. 举例理解

假设：

```cpp
left = 0.1
right = 0.2
target = 0.3
tolerance = 0.000001
```

首先：

```cpp
left + right
```

数学上为：

```text
0.3
```

但计算机内部可能是类似：

```text
0.30000000000000004
```

计算距离：

```cpp
std::abs((left + right) - target)
```

大约得到：

```text
0.00000000000000004
```

而：

```text
0.00000000000000004 <= 0.000001
```

成立。

所以返回：

```cpp
true
```

---

# 17. tolerance 为 0 时意味着什么？

题目允许：

```cpp
tolerance >= 0
```

如果：

```cpp
tolerance = 0
```

判断就变成：

```cpp
std::abs((left + right) - target) <= 0
```

绝对值不可能小于 0，所以只有：

```cpp
std::abs((left + right) - target) == 0
```

才成立。

也就是说：

> 当 tolerance 为 0 时，要求两个结果在实际浮点表示上完全相同。

例如：

```cpp
1.0 + 2.0
```

和：

```cpp
3.0
```

通常能够完全匹配。

但是：

```cpp
0.1 + 0.2
```

和：

```cpp
0.3
```

则可能不能。

---

# 18. `double` 比较中的一个更深入问题

简单教程通常使用：

```cpp
std::abs(a - b) <= tolerance
```

这叫做 **absolute tolerance（绝对容差）**。

对于当前题目，这是正确答案，因为题目明确要求使用给定的 `tolerance`。

但在真实工程中，如果数值范围差异很大，只用固定的绝对容差可能不够。

例如：

```text
a = 1,000,000,000
b = 1,000,000,000.0001
```

和：

```text
a = 0.000001
b = 0.000101
```

虽然绝对差都可以进行比较，但它们相对于数值本身的意义完全不同。

工程代码中经常还会使用：

```text
relative tolerance
```

也就是**相对容差**。

不过这已经超出当前题目的范围。

当前题目只需要记住：

```cpp
std::abs(a - b) <= tolerance
```

---

# 19. 一个非常重要的思维模型

比较整数时通常问：

```text
它们一样吗？
```

对应：

```cpp
a == b
```

比较浮点计算结果时通常问：

```text
它们离得够近吗？
```

对应：

```cpp
std::abs(a - b) <= tolerance
```

可以记成：

```text
整数比较 → equality
浮点近似比较 → distance
```

---

# 20. 本节重点总结

## 比较运算会产生 `bool`

```cpp
a == b
a != b
a < b
a <= b
a > b
a >= b
```

结果都是：

```cpp
true
```

或：

```cpp
false
```

---

## `=` 和 `==` 完全不同

```cpp
a = b;
```

是赋值：

```text
把 b 存进 a
```

而：

```cpp
a == b
```

是比较：

```text
a 是否等于 b？
```

---

## 浮点数不应轻易直接使用 `==`

不要默认：

```cpp
0.1 + 0.2 == 0.3
```

一定成立。

因为 `double` 保存的是二进制浮点近似值。

---

## 浮点近似比较

核心模式：

```cpp
std::abs(a - b) <= tolerance
```

含义：

> 如果两个值之间的距离不超过允许误差，就认为匹配。

---

## 当前题目的核心逻辑

```cpp
std::abs((left + right) - target) <= tolerance
```

也就是：

```text
1. 计算 left + right
2. 计算它与 target 的差
3. 取绝对值
4. 判断是否 <= tolerance
```
