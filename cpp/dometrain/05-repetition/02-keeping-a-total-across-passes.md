# C++ `while` 循环中的累加器（Accumulator）

## 1. 为什么累加变量要定义在循环外？

假设我们想计算：

```text
1 + 2 + 3
```

一种常见的错误写法，是把用于保存总和的变量定义在循环内部：

```cpp
while (value <= 3) {
    int total = 0;
    total += value;
    value += 1;
}
```

这样写有两个问题。

### 问题 1：每轮循环都会重新创建 `total`

每次进入循环体时：

```cpp
int total = 0;
```

都会重新执行。

因此过程相当于：

```text
第 1 轮：
total = 0
total += 1   → total = 1

第 2 轮：
total 又重新变成 0
total += 2   → total = 2

第 3 轮：
total 又重新变成 0
total += 3   → total = 3
```

前一轮计算出的结果全部丢失了。

如果想让一个变量保存“之前所有循环累积下来的结果”，它必须在循环开始之前创建。

正确写法：

```cpp
int total = 0;
int value = 1;

while (value <= 3) {
    total += value;
    value += 1;
}
```

执行过程：

```text
开始：
total = 0
value = 1

第 1 轮：
total = 0 + 1 = 1
value = 2

第 2 轮：
total = 1 + 2 = 3
value = 3

第 3 轮：
total = 3 + 3 = 6
value = 4

value <= 3 为 false
循环结束
```

最终：

```cpp
total == 6
```

---

## 2. 变量作用域（Scope）

在 C++ 中，变量只存在于定义它的代码块 `{ ... }` 内。

例如：

```cpp
while (value <= 3) {
    int total = 0;
}

return total;
```

这里会编译失败。

原因是：

```cpp
{
    int total = 0;
}
```

当程序离开右花括号 `}` 时，`total` 的作用域就结束了。

因此：

```cpp
return total;
```

已经访问不到这个变量。

### 正确方式

```cpp
int total = 0;

while (value <= 3) {
    total += value;
}

return total;
```

此时 `total` 定义在循环外，因此循环结束之后依然存在。

可以把作用域理解成：

```text
函数
│
├── total 在这里创建
│
├── while
│   └── 可以访问 total
│
└── return total
    └── 仍然可以访问 total
```

---

# 3. 什么是累加器（Accumulator）？

像下面这样的变量：

```cpp
int total = 0;
```

如果它被不断用于保存之前计算出来的结果：

```cpp
total += value;
```

那么它通常称为：

> **accumulator（累加器）**

例如：

```cpp
int total = 0;

while (...) {
    total += something;
}
```

`total` 会随着循环不断积累结果。

---

## 4. `total += value` 是什么意思？

这句：

```cpp
total += value;
```

等价于：

```cpp
total = total + value;
```

例如：

```cpp
total = 10;
value = 3;

total += value;
```

执行之后：

```cpp
total == 13
```

注意这里非常重要：

```cpp
total += value;
```

是在**之前的 total 基础上继续增加**。

而：

```cpp
total = value;
```

则会直接覆盖之前的结果。

例如：

```cpp
total = 10;
value = 3;

total = value;
```

结果变成：

```cpp
total == 3
```

原来的 `10` 被丢掉了。

---

# 5. 为什么累加器通常初始化为 `0`？

如果要做加法：

```cpp
total += value;
```

最自然的初始值是：

```cpp
int total = 0;
```

因为：

```text
0 + x = x
```

也就是说，`0` 不会改变第一次加入的数字。

例如：

```cpp
total = 0;

total += 5;
```

结果就是：

```cpp
total == 5
```

---

## 6. “零次循环”的结果

初始化值还可以理解成：

> 如果循环一次都不执行，答案应该是多少？

对于求和来说：

```cpp
int total = 0;
```

如果循环完全没有执行，那么：

```cpp
return total;
```

自然就会返回：

```text
0
```

这也是数学上“空集合求和”通常定义为 `0` 的原因。

这个思想非常重要。

例如求区间：

```text
low > high
```

那么区间里面没有任何数字。

因此循环可以一次都不执行，而 `total` 保持：

```cpp
0
```

不需要额外写：

```cpp
if (low > high) {
    return 0;
}
```

只要循环条件写正确，就已经自动处理了这种情况。

---

# 7. `while` 循环的三个核心部分

一个典型的计数循环通常有三个部分：

```cpp
int value = 1;          // ① 初始值

while (value <= 3) {    // ② 继续循环的条件
    ...
    value += 1;         // ③ 更新变量
}
```

可以理解为：

```text
初始化
   ↓
检查条件
   ↓
执行循环体
   ↓
更新变量
   ↓
重新检查条件
```

例如：

```cpp
int value = 1;

while (value <= 3) {
    value += 1;
}
```

变化过程：

```text
value = 1 → 条件 true
value = 2 → 条件 true
value = 3 → 条件 true
value = 4 → 条件 false
```

因此循环执行了 3 次。

---

# 8. Inclusive Range：区间两端都包含

题目中强调：

> Both bounds count

意思是：

> `low` 和 `high` 都属于区间。

例如：

```cpp
sumRange(1, 5)
```

应该计算：

```text
1 + 2 + 3 + 4 + 5
```

注意 `5` 也必须加入。

所以循环条件必须写：

```cpp
value <= high
```

而不能写：

```cpp
value < high
```

区别是：

```cpp
value < high
```

只会得到：

```text
1, 2, 3, 4
```

而：

```cpp
value <= high
```

才会得到：

```text
1, 2, 3, 4, 5
```

---

# 9. 负数区间同样适用

循环并不关心数字是不是负数。

例如：

```cpp
sumRange(-3, 2)
```

应该计算：

```text
-3 + -2 + -1 + 0 + 1 + 2
```

结果：

```text
-3
```

因为：

```text
-3 - 2 - 1 + 0 + 1 + 2
= -3
```

循环仍然可以直接写：

```cpp
int value = low;

while (value <= high) {
    total += value;
    value += 1;
}
```

变化过程：

```text
-3
-2
-1
0
1
2
```

---

# 10. `low > high` 时为什么自动返回 0？

假设：

```cpp
sumRange(5, 2)
```

初始化：

```cpp
int total = 0;
int value = 5;
```

循环条件：

```cpp
while (value <= high)
```

实际上是：

```cpp
while (5 <= 2)
```

结果马上就是：

```text
false
```

所以循环一次都不会执行。

因此：

```cpp
total
```

仍然是：

```text
0
```

最后返回：

```cpp
return total;
```

自然得到：

```text
0
```

这是一种很好的程序设计方式：

> 如果循环结构本身已经能够自然处理边界情况，就不必再增加额外的特殊判断。

---

# 11. 一个通用的“区间累加”模板

以后遇到：

> 从 `low` 到 `high`，逐个处理所有数字

可以先想到：

```cpp
int result = 初始值;
int value = low;

while (value <= high) {
    // 使用 value 更新 result

    value += 1;
}

return result;
```

如果是求和：

```cpp
int total = 0;
int value = low;

while (value <= high) {
    total += value;
    value += 1;
}

return total;
```

这里：

```text
total
```

是 **accumulator（累加器）**。

```text
value
```

是 **loop/control variable（循环控制变量）**。

```text
value <= high
```

是 **loop condition（循环条件）**。

```text
value += 1
```

是 **update step（更新步骤）**。

---

# 12. 做这类循环题时的思考框架

看到循环累积类题目，可以依次问自己：

1. **我要最终保存什么结果？**

   * 比如总和 `total`

2. **结果的初始值应该是多少？**

   * 求和通常是 `0`

3. **我要从哪个值开始？**

   * `value = low`

4. **什么时候应该继续循环？**

   * 如果上下界都包含：`value <= high`

5. **每轮循环做什么？**

   * `total += value`

6. **每轮之后循环变量怎么变化？**

   * `value += 1`

7. **循环结束后返回什么？**

   * `return total`

这种思考方式比背代码更加重要。
