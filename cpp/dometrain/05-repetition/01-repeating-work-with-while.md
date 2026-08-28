# C++ `while` 循环：条件驱动的重复执行

## 1. `while` 是什么

`while` 循环用于：

> **只要某个条件仍然为 `true`，就不断重复执行一段代码。**

基本语法：

```cpp
while (condition) {
    // 重复执行的代码
}
```

其中：

* `condition` 是一个可以得到 `bool` 值的表达式
* 当条件为 `true` 时，执行循环体
* 执行完循环体后，再次检查条件
* 当条件变成 `false` 时，退出循环

例如：

```cpp
int n = 3;

while (n > 0) {
    n -= 1;
}
```

执行过程：

```text
n = 3
3 > 0 → true
执行 n -= 1
n = 2

2 > 0 → true
执行 n -= 1
n = 1

1 > 0 → true
执行 n -= 1
n = 0

0 > 0 → false
退出循环
```

因此循环体总共执行了：

```text
3 次
```

---

# 2. `while` 的核心执行流程

可以把：

```cpp
while (condition) {
    body;
}
```

理解成：

```text
检查 condition
    ↓
true ──→ 执行 body
 ↑           │
 └───────────┘
 
false ──→ 离开循环
```

最重要的一点是：

> `while` 会在**每一次循环开始之前**检查条件。

不仅第二次、第三次循环之前会检查，**第一次循环之前也会检查**。

---

# 3. `while` 可能一次都不执行

例如：

```cpp
int n = 0;

while (n > 0) {
    n -= 1;
}
```

第一次检查：

```cpp
0 > 0
```

结果已经是：

```cpp
false
```

所以循环体：

```cpp
n -= 1;
```

一次都不会执行。

因此：

> `while` 循环可能执行 0 次。

这也是 `while` 的重要特点。

---

# 4. `while` 的两个核心组成部分

一个正常工作的 `while` 循环通常需要两个东西：

1. **退出条件**
2. **朝退出条件前进的变化**

例如：

```cpp
int n = 3;

while (n > 0) {
    n -= 1;
}
```

这里：

```cpp
n > 0
```

是循环条件。

而：

```cpp
n -= 1;
```

负责让程序逐渐接近退出条件。

执行过程中：

```text
3
2
1
0
```

`n` 不断减小，最终使：

```cpp
n > 0
```

变成：

```cpp
false
```

循环因此结束。

---

# 5. 循环变量必须产生“进展”

写 `while` 时，一个非常重要的问题是：

> **循环体有没有改变循环条件所依赖的值？**

例如：

```cpp
int n = 3;

while (n > 0) {
    std::cout << n << '\n';
}
```

这里存在严重问题。

条件一直是：

```cpp
n > 0
```

但是循环体从来没有修改 `n`。

所以：

```text
n = 3
3 > 0 → true

n 仍然是 3
3 > 0 → true

n 仍然是 3
3 > 0 → true

...
```

条件永远不会变成 `false`。

这叫：

# 无限循环（infinite loop）

---

# 6. 无限循环通常不是编译错误

例如：

```cpp
while (n > 0) {
    std::cout << n;
}
```

从 C++ 语法角度来看完全合法。

因此编译器通常不会说：

```text
Error: this loop never ends
```

程序可能正常编译。

问题只会在运行阶段出现。

例如：

* 程序一直运行
* 在线判题系统超时
* 测试框架触发 timeout
* CPU 一直执行循环

因此：

> **能编译，不代表循环逻辑正确。**

---

# 7. 一个好用的检查方法

看到：

```cpp
while (condition) {
    ...
}
```

可以问自己三个问题。

### 问题 1：什么情况下进入循环？

例如：

```cpp
while (start > stopAt)
```

表示：

```text
start > stopAt
```

时继续循环。

---

### 问题 2：循环体里什么东西发生变化？

例如：

```cpp
start -= 1;
```

说明：

```text
start 每次减 1
```

---

### 问题 3：这个变化最终能不能让条件变成 false？

例如：

```cpp
while (start > stopAt) {
    start -= 1;
}
```

因为：

```text
start 每次减小
stopAt 不变
```

所以只要整数范围允许，最终：

```cpp
start == stopAt
```

此时：

```cpp
start > stopAt
```

变成：

```cpp
false
```

循环结束。

---

# 8. `while` 循环中的“状态”

循环通常会维护某些不断变化的变量。

例如：

```cpp
int n = 5;

while (n > 0) {
    n -= 1;
}
```

这里 `n` 同时承担两个作用：

```text
当前状态
+
循环控制变量
```

它既告诉程序“现在在哪里”，也决定循环是否继续。

另一个常见情况是额外维护一个计数器：

```cpp
int count = 0;

while (condition) {
    // 做一次事情

    count += 1;
}
```

这里：

```cpp
count
```

表示：

> 循环体已经执行了多少次。

---

# 9. 使用计数器统计循环次数

例如我们想知道从 `5` 减到 `0` 需要多少次：

```cpp
int n = 5;
int count = 0;

while (n > 0) {
    n -= 1;
    count += 1;
}
```

执行过程：

| 循环次数 | `n` | `count` |
| ---: | --: | ------: |
|   开始 |   5 |       0 |
|    1 |   4 |       1 |
|    2 |   3 |       2 |
|    3 |   2 |       3 |
|    4 |   1 |       4 |
|    5 |   0 |       5 |

最后：

```cpp
count == 5
```

---

# 10. 一次循环代表一次“step”

很多题目要求：

> 每移动一步，就统计一次循环。

例如：

```cpp
while (start > stopAt) {
    start -= 1;
    passes += 1;
}
```

这里每执行一次：

```cpp
start -= 1;
```

就代表：

```text
start 向 stopAt 靠近一步
```

所以同时：

```cpp
passes += 1;
```

这两件事情应该一起发生。

可以把它理解为：

```text
走一步
↓
记一次
```

---

# 11. `while` 的边界条件

假设：

```cpp
int start = 4;
int stopAt = 4;
```

循环：

```cpp
while (start > stopAt) {
    start -= 1;
}
```

第一次检查：

```cpp
4 > 4
```

结果：

```cpp
false
```

因此循环执行：

```text
0 次
```

这正是 `while` “先判断、后执行”的结果。

---

# 12. 起点已经低于终点时

例如：

```cpp
int start = 2;
int stopAt = 5;
```

循环：

```cpp
while (start > stopAt) {
    start -= 1;
}
```

第一次条件：

```cpp
2 > 5
```

结果：

```cpp
false
```

所以：

```text
0 次循环
```

注意这里通常完全不需要额外写：

```cpp
if (start < stopAt) {
    return 0;
}
```

因为：

```cpp
while (start > stopAt)
```

本身就已经处理了这种情况。

这是一种很重要的编程思想：

> 如果循环条件本身已经覆盖某个边界情况，就不要重复写额外判断。

---

# 13. 为什么这里应该使用 `>` 而不是 `>=`

假设：

```cpp
start = 4
stopAt = 4
```

题目要求：

```text
应该执行 0 次
```

因此条件应该是：

```cpp
while (start > stopAt)
```

因为：

```cpp
4 > 4
```

是：

```cpp
false
```

但如果错误地写：

```cpp
while (start >= stopAt)
```

那么：

```cpp
4 >= 4
```

仍然是：

```cpp
true
```

程序还会继续执行一次：

```cpp
start -= 1;
```

把 `start` 从 4 减成 3。

这已经超过了目标。

因此要仔细区分：

```cpp
>
```

和：

```cpp
>=
```

---

# 14. 负数不会改变这个循环逻辑

例如：

```cpp
start = 3
stopAt = -3
```

只要：

```cpp
start > stopAt
```

就继续：

```cpp
start -= 1;
```

变化过程：

```text
3
2
1
0
-1
-2
-3
```

需要的步骤：

```text
3 → 2     第 1 步
2 → 1     第 2 步
1 → 0     第 3 步
0 → -1    第 4 步
-1 → -2   第 5 步
-2 → -3   第 6 步
```

所以：

```text
6 次
```

负数本身并不特殊。

关键仍然只是：

```text
当前值是否仍然 > 目标值
```

---

# 15. `while` 中常见的代码模式

## 模式一：倒计时

```cpp
int n = 5;

while (n > 0) {
    n -= 1;
}
```

---

## 模式二：统计循环次数

```cpp
int count = 0;

while (condition) {
    // 做事情
    count += 1;
}
```

---

## 模式三：一边移动，一边计数

```cpp
int count = 0;

while (current > target) {
    current -= 1;
    count += 1;
}
```

这里：

```cpp
current -= 1;
```

负责产生进展。

而：

```cpp
count += 1;
```

负责统计完成了多少次进展。

---

# 16. `while` 的典型思考模板

以后看到需要用 `while` 的题目，可以按照下面的方式思考：

```text
1. 当前状态是什么？
2. 什么时候还需要继续？
3. 每次循环要做什么？
4. 哪个变量发生变化？
5. 这个变化最终会不会让条件变成 false？
6. 需要记录循环执行了多少次吗？
```

例如：

```text
当前状态：
start

目标：
stopAt

继续条件：
start > stopAt

每次变化：
start -= 1

统计：
passes += 1
```

于是自然得到：

```cpp
int passes = 0;

while (start > stopAt) {
    start -= 1;
    passes += 1;
}
```

---

# 17. 常见错误

## 错误一：忘记修改循环变量

错误：

```cpp
while (n > 0) {
    count += 1;
}
```

`n` 永远不变，因此可能无限循环。

正确：

```cpp
while (n > 0) {
    n -= 1;
    count += 1;
}
```

---

## 错误二：变量变化方向错了

错误：

```cpp
while (n > 0) {
    n += 1;
}
```

如果 `n` 最开始是：

```cpp
3
```

则变化：

```text
3
4
5
6
7
...
```

距离退出条件越来越远。

正确：

```cpp
while (n > 0) {
    n -= 1;
}
```

---

## 错误三：边界条件写成 `>=`

如果目标是：

```text
到达 stopAt 时立即结束
```

通常应该：

```cpp
while (start > stopAt)
```

而不是：

```cpp
while (start >= stopAt)
```

---

## 错误四：忘记初始化计数器

错误：

```cpp
int passes;

while (start > stopAt) {
    start -= 1;
    passes += 1;
}
```

`passes` 没有初始化，其初始值不确定。

正确：

```cpp
int passes = 0;
```

---

# 18. `while` 的核心心智模型

可以把：

```cpp
while (condition) {
    body;
}
```

记成一句话：

> **先问“还需要继续吗？”，如果需要就做一次，然后回来再问。**

也就是：

```text
检查
↓
执行一次
↓
检查
↓
执行一次
↓
检查
↓
...
↓
false
↓
结束
```

---

# 19. 本节最重要的知识点

记住下面几点即可：

```text
while = 条件为 true 时重复执行
```

并且：

1. `while` 在每次循环之前检查条件
2. 第一次检查失败时，循环执行 0 次
3. 循环条件必须最终能够变成 `false`
4. 循环体通常需要修改条件依赖的变量
5. 修改方向必须朝退出条件前进
6. 需要统计循环次数时，可以使用计数器
7. 边界情况往往可以由 `while` 条件自然处理
8. 无限循环通常是逻辑错误，而不是编译错误

最典型的模式：

```cpp
int count = 0;

while (current > target) {
    current -= 1;
    count += 1;
}
```

可以理解为：

```text
还没到目标？
    ↓ 是
走一步
记一次
再检查
```
