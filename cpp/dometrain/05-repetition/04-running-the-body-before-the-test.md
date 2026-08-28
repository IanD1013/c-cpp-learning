# C++ `do...while` 循环与整数减半

## 1. `do...while` 的核心特点

C++ 中的 `do...while` 循环结构如下：

```cpp
do {
    // 循环体
} while (condition);
```

它和普通 `while` 循环最大的区别是：

> `do...while` **先执行循环体，再判断条件**。

因此，`do...while` 的循环体**至少会执行一次**。

例如：

```cpp
int n = 0;

do {
    n += 1;
} while (n < 0);
```

执行过程：

```text
初始：
n = 0

第 1 次执行循环体：
n += 1
n = 1

然后检查：
n < 0
1 < 0 → false

循环结束
```

最终：

```cpp
n == 1
```

虽然条件 `n < 0` 从一开始就不成立，但由于 `do...while` 是**先执行、后判断**，循环体仍然执行了一次。

---

# 2. `while` 和 `do...while` 的区别

普通 `while`：

```cpp
while (condition) {
    // body
}
```

执行顺序：

```text
检查 condition
    ↓
true → 执行 body
    ↓
再次检查 condition
```

如果第一次检查条件就是 `false`，循环体一次都不会执行。

例如：

```cpp
int n = 0;

while (n < 0) {
    n += 1;
}
```

因为：

```text
0 < 0 → false
```

所以循环体不会执行：

```cpp
n == 0
```

而对应的：

```cpp
int n = 0;

do {
    n += 1;
} while (n < 0);
```

结果却是：

```cpp
n == 1
```

因此可以总结为：

| 循环           | 条件检查位置 | 最少执行次数 |
| ------------ | ------ | -----: |
| `while`      | 循环体之前  |      0 |
| `do...while` | 循环体之后  |      1 |

---

# 3. `do...while` 的语法细节

标准结构：

```cpp
do {
    // body
} while (condition);
```

有两个容易写错的地方。

## 3.1 条件写在循环体后面

不是：

```cpp
while (condition) {
}
```

而是：

```cpp
do {
}
while (condition);
```

---

## 3.2 最后必须有分号

注意这里：

```cpp
while (condition);
                 ^
```

最后的 `;` 不能省略。

正确：

```cpp
do {
    n++;
} while (n < 10);
```

错误：

```cpp
do {
    n++;
} while (n < 10)
```

后者会导致编译错误。

---

# 4. 整数除法会截断小数部分

在 C++ 中，如果两个操作数都是整数：

```cpp
int x = 5 / 2;
```

结果不是：

```text
2.5
```

而是：

```cpp
2
```

因为整数除法不会保留小数部分。

例如：

```cpp
8 / 2 == 4
4 / 2 == 2
2 / 2 == 1
1 / 2 == 0
```

因此不断把一个正整数除以 2，它最终一定会变成 0。

例如：

```text
8
↓ /2
4
↓ /2
2
↓ /2
1
↓ /2
0
```

一共进行了 4 次除法。

---

# 5. `/=` 复合赋值运算符

下面：

```cpp
value /= 2;
```

等价于：

```cpp
value = value / 2;
```

也就是说，它不只是计算结果，还会把结果重新保存到 `value` 中。

例如：

```cpp
int value = 8;

value /= 2;
```

执行后：

```cpp
value == 4
```

---

# 6. 为什么不能只写 `value / 2`

下面这行：

```cpp
value / 2;
```

虽然会计算：

```cpp
value / 2
```

但是结果没有保存到任何地方，因此会直接被丢弃。

例如：

```cpp
int value = 8;

value / 2;
```

执行以后：

```cpp
value == 8
```

完全没有改变。

如果循环条件依赖 `value`：

```cpp
while (value > 0) {
    value / 2;
}
```

那么：

```text
value = 8
value = 8
value = 8
value = 8
...
```

`value` 永远不发生变化，于是：

```cpp
value > 0
```

永远为 `true`。

最终形成**无限循环**。

正确做法应该是：

```cpp
value /= 2;
```

或者：

```cpp
value = value / 2;
```

---

# 7. 什么叫“halving steps”

题目的任务本质上是：

> 一个整数每执行一次循环，就把它除以 2，并统计一共执行了多少次循环。

例如：

```text
start = 8
```

执行：

```text
第 1 次：8 / 2 = 4
第 2 次：4 / 2 = 2
第 3 次：2 / 2 = 1
第 4 次：1 / 2 = 0
```

所以：

```cpp
halvingSteps(8) == 4
```

---

# 8. 为什么 `halvingSteps(1)` 是 1

对于：

```text
start = 1
```

执行一次：

```text
1 / 2 = 0
```

所以：

```cpp
halvingSteps(1) == 1
```

---

# 9. 最容易出错的情况：`start == 0`

如果这是普通 `while`：

```cpp
while (value > 0) {
    value /= 2;
    count++;
}
```

当：

```cpp
value = 0
```

第一次检查：

```text
0 > 0 → false
```

循环一次都不会执行，所以：

```text
count = 0
```

但是本题明确要求使用：

```cpp
do...while
```

例如：

```cpp
do {
    value /= 2;
    count++;
} while (value > 0);
```

当：

```cpp
value = 0
```

执行过程是：

```text
先执行循环体：

0 / 2 = 0
count = 1

然后检查：

0 > 0 → false
```

因此：

```cpp
halvingSteps(0) == 1
```

这正体现了 `do...while`：

> 循环体至少执行一次。

---

# 10. Guard Clause：提前处理非法输入

题目规定：

```text
start < 0
```

不属于合法输入。

遇到负数应该直接返回：

```cpp
-1
```

因此可以在循环之前写：

```cpp
if (start < 0) {
    return -1;
}
```

这种写法通常称为：

> **Guard Clause（守卫语句 / 提前返回）**

它的作用是：

```text
先处理特殊情况
↓
如果输入非法，直接结束函数
↓
剩下的代码只考虑正常情况
```

例如：

```cpp
static int halvingSteps(int start) {
    if (start < 0) {
        return -1;
    }

    // 从这里开始，可以确定 start >= 0
}
```

这样比把所有逻辑塞进一个很大的 `if...else` 更清晰。

---

# 11. 使用计数器统计循环次数

我们需要记录：

```text
一共执行了多少次减半
```

所以定义：

```cpp
int count = 0;
```

每执行一次循环：

```cpp
count++;
```

例如：

```cpp
int count = 0;

do {
    value /= 2;
    count++;
} while (value > 0);
```

如果输入为 8：

```text
value    count

8        0
4        1
2        2
1        3
0        4
```

最终：

```cpp
return count;
```

得到：

```cpp
4
```

---

# 12. 完整的执行顺序

假设：

```cpp
start = 8;
```

初始化：

```cpp
int value = start;
int count = 0;
```

此时：

```text
value = 8
count = 0
```

### 第 1 次循环

```cpp
value /= 2;
```

得到：

```text
value = 4
```

然后：

```cpp
count++;
```

得到：

```text
count = 1
```

检查：

```cpp
value > 0
```

即：

```text
4 > 0 → true
```

继续。

### 第 2 次循环

```text
value = 2
count = 2
```

继续。

### 第 3 次循环

```text
value = 1
count = 3
```

继续。

### 第 4 次循环

```text
value = 0
count = 4
```

检查：

```text
0 > 0 → false
```

结束。

返回：

```cpp
4
```

---

# 13. 几个重要测试案例

应该得到：

```cpp
halvingSteps(-1) == -1
halvingSteps(0) == 1
halvingSteps(1) == 1
halvingSteps(2) == 2
halvingSteps(3) == 2
halvingSteps(4) == 3
halvingSteps(8) == 4
```

为什么 `3` 只需要 2 次？

因为整数除法：

```text
3 / 2 = 1
1 / 2 = 0
```

所以：

```cpp
halvingSteps(3) == 2
```

---

# 14. 最大输入 `1000000`

题目说明最大合法输入是：

```cpp
1000000
```

不断进行整数除以 2：

```text
1000000
500000
250000
125000
62500
31250
15625
7812
3906
1953
976
488
244
122
61
30
15
7
3
1
0
```

因此一共执行：

```text
20 次
```

所以：

```cpp
halvingSteps(1000000) == 20
```

---

# 15. 本题真正想训练的知识

这道题主要考察四个知识点。

### ① `do...while` 至少执行一次

```cpp
do {
    ...
} while (...);
```

和：

```cpp
while (...) {
    ...
}
```

最大的区别就是第一次条件检查发生的位置不同。

---

### ② 整数除法

```cpp
1 / 2 == 0
```

而不是：

```text
0.5
```

所以不断整数除以 2，正整数最终会降到 0。

---

### ③ 循环变量必须发生变化

如果条件依赖：

```cpp
value
```

那么循环体中通常必须让：

```cpp
value
```

逐渐接近循环终止条件。

正确：

```cpp
value /= 2;
```

错误：

```cpp
value / 2;
```

后者不会修改变量，容易造成无限循环。

---

### ④ 特殊输入优先处理

对于：

```cpp
start < 0
```

直接：

```cpp
return -1;
```

这种 guard clause 可以让主体逻辑更加简单。

---

# 16. 总结

`do...while` 的核心执行流程可以记成：

```text
执行 body
    ↓
检查 condition
    ↓
true → 再执行 body
false → 结束
```

所以：

```cpp
do {
    ...
} while (...);
```

一定至少执行一次。

本题中的核心循环模式是：

```cpp
do {
    value /= 2;
    count++;
} while (value > 0);
```

其中：

```cpp
value /= 2;
```

负责让数值不断靠近 0，

而：

```cpp
count++;
```

负责统计循环实际执行了多少次。
