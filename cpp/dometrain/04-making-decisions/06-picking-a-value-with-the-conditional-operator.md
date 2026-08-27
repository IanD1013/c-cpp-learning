# C++ 条件运算符 `?:`

## 1. 什么是条件运算符

C++ 中除了使用 `if...else` 进行二选一之外，还可以使用 **条件运算符**：

```cpp
condition ? valueWhenTrue : valueWhenFalse
```

它也常被叫做：

* conditional operator
* ternary operator
* 三元运算符

之所以叫“三元运算符”，是因为它一共需要 **3 个操作数**：

1. 条件
2. 条件为 `true` 时产生的值
3. 条件为 `false` 时产生的值

例如：

```cpp
static int fee(int weight) {
    return weight > 20 ? 15 : 5;
}
```

可以读作：

> 如果 `weight > 20`，结果就是 `15`；否则结果就是 `5`。

等价的 `if...else` 写法大致是：

```cpp
static int fee(int weight) {
    if (weight > 20) {
        return 15;
    } else {
        return 5;
    }
}
```

---

# 2. 条件运算符的基本结构

标准形式：

```cpp
condition ? whenTrue : whenFalse
```

例如：

```cpp
int bigger = a > b ? a : b;
```

含义：

```text
a > b ?
    是 → a
    否 → b
```

也就是：

> 如果 `a > b`，`bigger` 得到 `a`，否则得到 `b`。

---

# 3. 条件运算符本身是一个表达式

这是理解 `?:` 最重要的一点。

下面这个：

```cpp
a > b ? a : b
```

不是一条完整的控制语句，而是一个 **expression（表达式）**。

它最终会产生一个值。

因此可以直接把它放在赋值语句右边：

```cpp
int maxValue = a > b ? a : b;
```

也可以直接放在 `return` 后面：

```cpp
return a > b ? a : b;
```

还可以作为其他表达式的一部分：

```cpp
int result = 10 + (a > b ? a : b);
```

所以可以把：

```cpp
condition ? A : B
```

理解成：

```text
“根据 condition，从 A 和 B 中选一个值”
```

---

# 4. `?:` 和 `if...else` 的核心区别

## `if...else`

`if...else` 是一个 **statement（语句）**。

例如：

```cpp
if (score >= 60) {
    result = "pass";
} else {
    result = "fail";
}
```

它的主要作用是：

> 决定执行哪一段代码。

---

## 条件运算符 `?:`

`?:` 是一个 **expression（表达式）**。

例如：

```cpp
std::string result = score >= 60 ? "pass" : "fail";
```

它的主要作用是：

> 决定产生哪个值。

所以一般来说：

```text
需要“选一段代码执行”
→ 使用 if / else

需要“在两个值中选一个”
→ 可以考虑 ?:
```

---

# 5. `?:` 只会计算被选中的一边

例如：

```cpp
int result = condition ? first() : second();
```

如果：

```cpp
condition == true
```

那么只会调用：

```cpp
first()
```

不会调用：

```cpp
second()
```

反过来也一样。

因此条件运算符不是：

```text
先把左右两边都算出来，再选一个
```

而是：

```text
先判断条件，然后只计算需要的那一边
```

这个性质类似 `if...else`。

例如：

```cpp
int result = divisor != 0 ? total / divisor : 0;
```

如果：

```cpp
divisor == 0
```

程序不会去计算：

```cpp
total / divisor
```

因为条件为 `false`，只会计算：

```cpp
0
```

---

# 6. `?` 和 `:` 分别是什么意思

例如：

```cpp
weight > 20 ? 15 : 5
```

可以拆成三部分：

```cpp
weight > 20
```

条件。

```cpp
15
```

条件为 `true` 时产生的值。

```cpp
5
```

条件为 `false` 时产生的值。

其中：

```text
?
```

把“条件”和候选结果分开。

而：

```text
:
```

把两个候选结果分开。

所以可以记成：

```cpp
问题 ? 是的话 : 不是的话
```

例如：

```cpp
age >= 18 ? "adult" : "minor"
```

读作：

```text
age >= 18 吗？
是 → "adult"
否 → "minor"
```

---

# 7. 两个分支必须能够形成一个合理的结果类型

由于：

```cpp
condition ? A : B
```

整个东西最终必须产生 **一个值**，所以 `A` 和 `B` 的类型通常需要能够兼容。

例如：

```cpp
int result = condition ? 10 : 20;
```

没问题，因为两边都是：

```cpp
int
```

---

## 类型不同不一定都报错

需要注意：

> “两边必须完全是同一个类型”是一种为了初学者方便理解的简化说法。

实际上 C++ 会尝试按照语言规则寻找一个共同结果类型。

例如：

```cpp
double result = condition ? 10 : 3.5;
```

这里：

```cpp
10
```

是 `int`，

```cpp
3.5
```

是 `double`。

C++ 可以把 `10` 转换成：

```cpp
10.0
```

因此整个表达式可以得到 `double`。

---

## 但完全不兼容的类型通常无法成立

例如：

```cpp
condition ? 10 : "hello"
```

一个是：

```cpp
int
```

另一个是字符串字面量相关类型。

这种情况下通常没有合适的共同结果类型，因此会产生编译错误。

初学阶段可以采用这个经验：

```text
?: 两边最好返回相同或明显兼容的类型。
```

---

# 8. `?:` 两边必须是表达式

例如：

```cpp
condition ? 10 : 20
```

其中：

```cpp
10
```

和：

```cpp
20
```

都是表达式。

但不能把普通语句直接塞进去，例如：

```cpp
condition ? return 10 : return 20;
```

这是不正确的写法。

因为：

```cpp
return 10;
```

属于语句，不是普通的值表达式。

正确写法是：

```cpp
return condition ? 10 : 20;
```

先让条件运算符产生：

```cpp
10
```

或：

```cpp
20
```

然后再由：

```cpp
return
```

把这个结果返回。

---

# 9. 分支内部不应该写分号

正确：

```cpp
return x > 0 ? x : 0;
```

不要写成：

```cpp
return x > 0 ? x; : 0;
```

因为：

```cpp
x
```

和：

```cpp
0
```

是条件运算符的两个操作数，它们是表达式的一部分，不是单独的语句。

整个表达式完成后，最后才写：

```cpp
;
```

即：

```cpp
return condition ? A : B;
```

---

# 10. 很适合 `?:` 的场景

条件运算符最适合：

> 根据一个简单条件，在两个简单值中选一个。

例如：

```cpp
int maxValue = a > b ? a : b;
```

```cpp
int sign = value >= 0 ? 1 : -1;
```

```cpp
std::string status = online ? "online" : "offline";
```

```cpp
return score >= 60 ? 1 : 0;
```

这些代码都比较简短，而且意思清楚。

---

# 11. 不适合过度嵌套

C++ 允许嵌套条件运算符。

例如：

```cpp
int result =
    score >= 90 ? 1 :
    score >= 80 ? 2 :
    score >= 70 ? 3 :
                  4;
```

虽然合法，但随着嵌套增加，会越来越难读。

例如：

```cpp
a ? b ? x : y : c ? z : w
```

很容易让人看不清：

* 哪个 `?`
* 对应哪个 `:`
* 条件的层级是什么

因此初学阶段最好遵循：

```text
简单二选一 → ?:

复杂分支逻辑 → if / else if / else
```

---

# 12. 用条件运算符求两个数之间的距离

假设有：

```cpp
int left;
int right;
```

我们想计算两个整数之间的距离。

距离永远不能是负数。

例如：

```text
left = 8
right = 3
```

距离：

```text
8 - 3 = 5
```

但是：

```text
left = 3
right = 8
```

如果直接计算：

```cpp
left - right
```

得到：

```text
-5
```

而距离应该是：

```text
5
```

因此需要根据谁比较大来决定减法顺序：

```text
如果 left > right
    left - right
否则
    right - left
```

这正好是一个简单的二选一问题。

所以非常适合使用：

```cpp
left > right ? left - right : right - left
```

---

# 13. 为什么相等时也没问题

如果：

```cpp
left == right
```

那么条件：

```cpp
left > right
```

为：

```cpp
false
```

所以走：

```cpp
right - left
```

例如：

```cpp
5 - 5
```

结果：

```text
0
```

这正好就是两个相同数字之间的距离。

因此不需要单独写：

```cpp
left == right
```

这个情况。

---

# 14. 跨过 0 的距离

例如：

```text
left = -3
right = 5
```

因为：

```cpp
left > right
```

即：

```cpp
-3 > 5
```

为 `false`，

所以计算：

```cpp
right - left
```

即：

```cpp
5 - (-3)
```

注意减去负数相当于加：

```text
5 + 3 = 8
```

所以：

```text
-3 到 5 的距离 = 8
```

可以从数轴理解：

```text
-3 → -2 → -1 → 0 → 1 → 2 → 3 → 4 → 5
```

总距离为：

```text
8
```

---

# 15. 为什么两个参数都必须使用

题目特别说明：

> 某些测试输入可能拥有相同的 `left`，但不同的 `right`，而且答案不同。

这其实是在防止一种错误写法，比如：

```cpp
return left > 0 ? left : -left;
```

这种代码实际上只是在算：

```text
|left|
```

完全忽略了：

```cpp
right
```

但题目要求的是：

```text
left 和 right 之间的距离
```

因此必须同时使用：

```cpp
left
```

和：

```cpp
right
```

---

# 16. 本题为什么不需要 `std::abs`

数学上，两个数之间的距离也可以表示成：

```cpp
abs(left - right)
```

例如：

```cpp
std::abs(left - right)
```

但是这道题的学习目标明显是：

> 练习条件运算符 `?:`

而且题目已经给出：

```cpp
left > right
```

要求你根据条件选择正确的减法顺序。

因此这里应该写：

```cpp
left > right ? left - right : right - left
```

而不是绕过练习目标使用：

```cpp
std::abs(...)
```

---

# 17. 核心模板

最值得记住的是：

```cpp
condition ? valueIfTrue : valueIfFalse
```

例如：

```cpp
int bigger = a > b ? a : b;
```

```cpp
int smaller = a < b ? a : b;
```

```cpp
int difference = a > b ? a - b : b - a;
```

---

# 18. 一句话总结

条件运算符：

```cpp
condition ? A : B
```

本质上是在说：

> 根据一个条件，在 `A` 和 `B` 两个值中选择一个，并把被选中的值作为整个表达式的结果。

它特别适合简单的“二选一”场景：

```cpp
return left > right ? left - right : right - left;
```

而当逻辑开始涉及：

* 多个步骤
* 多个条件
* 副作用
* 多层嵌套

通常应该改用：

```cpp
if
else if
else
```

以保证代码可读性。
