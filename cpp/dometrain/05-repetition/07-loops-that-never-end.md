# C++ `while` 循环：如何保证循环一定能结束

## 1. 循环终止的核心条件

一个 `while` 循环想要最终结束，最重要的不是“循环里有更新语句”，而是：

> **循环体必须让 `while` 条件所依赖的值逐渐发生变化，并最终让条件变成 `false`。**

例如：

```cpp
int value = 3;
int passes = 0;

while (value < 20) {
    value *= 2;
    ++passes;
}
```

执行过程：

```text
初始：value = 3

第 1 次：
value = 6
passes = 1

第 2 次：
value = 12
passes = 2

第 3 次：
value = 24
passes = 3

再次检查：
24 < 20  → false
```

因此循环结束：

```cpp
passes == 3
```

这里循环之所以能结束，是因为：

```cpp
value *= 2;
```

会不断增大 `value`。

---

# 2. 什么叫“循环没有进展”

考虑：

```cpp
int value = 0;

while (value < 20) {
    value *= 2;
}
```

数学上：

```text
0 × 2 = 0
0 × 2 = 0
0 × 2 = 0
...
```

所以 `value` 永远都是：

```cpp
0
```

条件：

```cpp
value < 20
```

永远是：

```cpp
0 < 20
```

即：

```text
true
```

于是形成：

```text
无限循环
```

也叫：

```text
non-terminating loop
```

或者：

```text
infinite loop
```

---

# 3. “修改变量”不代表一定有进展

这是一个非常重要的概念。

下面代码确实执行了：

```cpp
value *= 2;
```

但是如果：

```cpp
value == 0
```

那么执行完：

```cpp
value *= 2;
```

之后仍然是：

```cpp
value == 0
```

因此真正应该问的不是：

> 循环有没有修改变量？

而应该问：

> 这个修改是否会让循环条件逐渐接近 `false`？

---

# 4. 判断循环是否能终止的方法

分析一个循环时，可以按照下面三个问题检查。

## 第一步：条件依赖哪些变量？

例如：

```cpp
while (value < target)
```

条件依赖：

```text
value
target
```

如果 `target` 不变，那么循环主要依靠 `value` 改变。

---

## 第二步：循环中这些变量如何变化？

例如：

```cpp
value *= 2;
```

意味着：

```text
value → 2 × value
```

---

## 第三步：这种变化一定会让条件最终失败吗？

如果：

```cpp
value > 0
```

并且：

```cpp
value < target
```

那么不断翻倍：

```text
value
2 × value
4 × value
8 × value
...
```

最终通常会达到或超过：

```cpp
target
```

于是：

```cpp
value < target
```

变成 `false`。

但是如果：

```cpp
value == 0
```

则：

```text
0 → 0 → 0 → 0
```

永远无法结束。

---

# 5. 为什么负数也是问题

考虑：

```cpp
int value = -3;

while (value < 20) {
    value *= 2;
}
```

变化：

```text
-3
-6
-12
-24
-48
...
```

注意：

```text
value 不但没有接近 20，
反而越来越小。
```

因此：

```cpp
value < 20
```

始终成立。

而且 C++ 的 `int` 有有限范围。

例如典型的 32 位 `int`：

```text
-2147483648 ～ 2147483647
```

不断执行：

```cpp
value *= 2;
```

最终可能超过 `int` 可以表示的范围。

---

# 6. 有符号整数溢出与 Undefined Behavior

在 C++ 中，如果有符号整数发生溢出，例如：

```cpp
int x = 2000000000;
x *= 2;
```

结果超出了 `int` 范围。

这属于：

```text
Undefined Behavior
```

即：

```text
未定义行为
```

这意味着：

> C++ 标准不保证程序会得到什么结果。

所以不要简单认为：

```text
超过最大值以后会自动绕回负数
```

这种行为不能作为正确程序逻辑的一部分。

---

# 7. 无限循环通常是运行时逻辑错误

考虑：

```cpp
while (value < 20) {
    value *= 2;
}
```

如果：

```cpp
value == 0
```

这段代码：

* 语法正确
* 编译器可以成功编译
* 但程序运行时永远结束不了

因此它通常不是：

```text
编译错误
```

而是：

```text
逻辑错误
```

也可以称为：

```text
runtime logic bug
```

---

# 8. 编译成功 ≠ 程序正确

这是编程中非常重要的区别。

## 编译错误

例如：

```cpp
int x =
```

语法不完整。

编译器会直接报错。

---

## 逻辑错误

例如：

```cpp
int value = 0;

while (value < 20) {
    value *= 2;
}
```

编译器认为完全合法。

但是程序逻辑有问题。

所以：

> **能够编译只说明代码符合语言规则，不代表算法正确。**

---

# 9. 为什么在线判题可能“没有结果”

很多在线编程环境都会限制程序最大运行时间。

例如：

```text
Time Limit = 20 seconds
```

如果程序进入无限循环：

```cpp
while (true) {
    ...
}
```

运行环境可能在 20 秒后强制结束程序。

这通常叫：

```text
Time Limit Exceeded
```

简称：

```text
TLE
```

有时界面可能只表现为：

```text
没有返回结果
```

这不代表程序“算出了空结果”。

真正发生的是：

```text
程序根本没有正常运行结束。
```

---

# 10. 为什么重新提交相同代码没有意义

如果代码中存在确定性的无限循环：

```cpp
int value = 0;

while (value < 20) {
    value *= 2;
}
```

那么重新运行：

```text
不会神奇地成功。
```

每次都会：

```text
0
0
0
0
...
```

因此修复问题必须修改：

* 循环条件
* 输入检查
* 或循环内部的状态变化

而不是反复运行相同代码。

---

# 11. Guard Clause：循环前先处理非法输入

假设我们要写：

```cpp
countDoublings(start, target)
```

含义是：

> `start` 需要翻倍多少次，才能达到或超过 `target`。

例如：

```cpp
countDoublings(3, 20)
```

过程：

```text
3 → 6 → 12 → 24
```

所以答案：

```text
3
```

但是如果：

```cpp
start == 0
```

那么：

```text
0 → 0 → 0 → ...
```

永远到不了正数 `target`。

因此应该在进入循环之前处理：

```cpp
if (start <= 0) {
    return -1;
}
```

这种写法叫：

```text
Guard Clause
```

中文可以理解为：

```text
提前返回 / 防御性检查
```

---

# 12. 为什么 Guard Clause 比复杂 `while` 条件更清晰

一种写法是：

```cpp
while (value > 0 && value < target) {
    ...
}
```

这个条件确实可以避免：

```cpp
value <= 0
```

时进入无限循环。

但是有一个语义问题。

假设：

```cpp
start == 0
```

那么：

```cpp
value > 0
```

一开始就是 `false`。

循环直接不执行。

如果函数最后：

```cpp
return count;
```

而：

```cpp
count == 0
```

那么函数会错误地表示：

```text
0 次翻倍就可以达到 target
```

但实际上：

```text
start = 0 根本永远无法通过翻倍达到正数 target。
```

所以：

```cpp
0
```

和：

```cpp
-1
```

代表的是两个完全不同的情况。

---

# 13. 不同返回值应该有明确语义

这个函数中可以设计：

```text
返回 0
```

表示：

> 不需要进行任何翻倍。

例如：

```cpp
countDoublings(8, 8)
```

因为一开始：

```text
8 >= 8
```

所以答案：

```cpp
0
```

同理：

```cpp
countDoublings(50, 10)
```

因为：

```text
50 >= 10
```

答案也是：

```cpp
0
```

---

而：

```text
返回 -1
```

表示：

> 输入无效，或者无法通过该操作完成目标。

例如：

```cpp
countDoublings(0, 20)
```

应该返回：

```cpp
-1
```

因为：

```text
0 → 0 → 0
```

永远不能达到 20。

---

# 14. 推荐的函数逻辑结构

这种问题通常可以写成：

```cpp
if (非法输入) {
    return 特殊错误值;
}

初始化状态;

while (还没有达到目标) {
    执行操作;
    更新计数;
}

return 结果;
```

对应本题：

```cpp
if (start <= 0) {
    return -1;
}

int value = start;
int count = 0;

while (value < target) {
    value *= 2;
    ++count;
}

return count;
```

---

# 15. 为什么计数应该放在翻倍之后

这里：

```cpp
value *= 2;
++count;
```

含义非常直接：

```text
完成一次翻倍
→ 记录一次操作
```

例如：

```text
value = 3
target = 20
```

执行：

```text
3 → 6      count = 1
6 → 12     count = 2
12 → 24    count = 3
```

所以：

```cpp
return 3;
```

---

# 16. 边界情况：一开始已经达到目标

例如：

```cpp
countDoublings(8, 8)
```

初始：

```cpp
value = 8
target = 8
```

循环条件：

```cpp
value < target
```

即：

```cpp
8 < 8
```

结果：

```text
false
```

因此循环执行：

```text
0 次
```

返回：

```cpp
0
```

这是正确的。

---

再例如：

```cpp
countDoublings(50, 10)
```

条件：

```cpp
50 < 10
```

也是 `false`。

所以：

```cpp
return 0;
```

这正好体现了：

> `while` 循环可能一次都不执行。

---

# 17. 一个重要设计原则：让 `while` 只负责主要循环条件

如果已经使用 Guard Clause：

```cpp
if (start <= 0) {
    return -1;
}
```

那么循环就可以保持简单：

```cpp
while (value < target)
```

而不需要：

```cpp
while (value > 0 && value < target)
```

这种结构通常更容易理解：

```text
输入是否合法？
    ↓
不合法 → 直接 return
    ↓
合法
    ↓
进入核心算法
```

这叫做：

```text
separation of concerns
```

即：

> 不同代码分别负责不同的问题。

---

# 18. 如何系统分析循环题

以后遇到类似问题，可以用下面这套思路。

## ① 循环条件是什么？

例如：

```cpp
value < target
```

---

## ② 哪个变量必须发生变化？

这里是：

```cpp
value
```

---

## ③ 它每次怎么变化？

```cpp
value *= 2;
```

---

## ④ 是否存在“不变化”的输入？

有：

```cpp
value == 0
```

因为：

```text
0 × 2 = 0
```

---

## ⑤ 是否存在“向错误方向变化”的输入？

有：

```cpp
value < 0
```

因为：

```text
-1 → -2 → -4 → -8
```

离正数目标越来越远。

---

## ⑥ 是否需要 Guard Clause？

因此需要：

```cpp
if (start <= 0) {
    return -1;
}
```

---

## ⑦ 已经满足目标怎么办？

由：

```cpp
while (value < target)
```

自然处理。

如果：

```cpp
value >= target
```

循环执行 0 次。

---

# 19. 总结

判断一个循环是否正确，核心不是看它“有没有更新变量”，而是看：

> **更新是否保证循环条件最终会变成 `false`。**

本题中的关键情况：

```text
start > 0
```

翻倍可以逐渐达到目标：

```text
3 → 6 → 12 → 24
```

而：

```text
start == 0
```

没有任何进展：

```text
0 → 0 → 0 → ...
```

负数则朝错误方向移动：

```text
-1 → -2 → -4 → ...
```

因此最合理的程序结构是：

```cpp
if (start <= 0) {
    return -1;
}

while (value < target) {
    value *= 2;
    ++count;
}
```

可以把这个思想记成一句话：

> **先用 Guard Clause 排除不可能正常终止的输入，再让循环条件只描述真正的算法目标。**
