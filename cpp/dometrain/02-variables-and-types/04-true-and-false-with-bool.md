# C++ `bool` 与比较表达式

## 1. 什么是 `bool`

在 C++ 中，`bool` 是专门用来表示“真假”的数据类型。

它只有两个可能的值：

```cpp
true
false
```

例如：

```cpp
bool ready = true;
bool finished = false;
```

可以把 `bool` 理解成：

> 一个只能回答“是”或“否”的变量。

---

# 2. 比较表达式的结果就是 `bool`

C++ 中的比较运算不会返回普通数字，而是直接产生一个 `bool`。

例如：

```cpp
3 > 1
```

这个表达式的结果是：

```cpp
true
```

因此下面的代码：

```cpp
bool ready = 3 > 1;
```

实际上可以理解成：

```cpp
bool ready = true;
```

执行顺序是：

```text
3 > 1
↓
true
↓
存入 ready
```

所以：

```cpp
return ready;
```

返回的就是：

```cpp
true
```

---

# 3. 常见的比较运算符

C++ 常见比较运算符包括：

| 运算符  | 含义   | 示例       | 结果      |
| ---- | ---- | -------- | ------- |
| `>`  | 大于   | `5 > 3`  | `true`  |
| `<`  | 小于   | `5 < 3`  | `false` |
| `>=` | 大于等于 | `5 >= 5` | `true`  |
| `<=` | 小于等于 | `4 <= 5` | `true`  |
| `==` | 等于   | `5 == 5` | `true`  |
| `!=` | 不等于  | `5 != 3` | `true`  |

例如：

```cpp
bool a = 10 > 5;
bool b = 10 < 5;
bool c = 10 == 10;
```

结果分别是：

```cpp
a == true
b == false
c == true
```

---

# 4. 比较结果可以直接 `return`

假设一个函数声明为：

```cpp
static bool isPositive(int value)
```

这意味着：

```text
输入：int
输出：bool
```

如果我们只是想判断：

> `value` 是否大于 `0`

那么：

```cpp
value > 0
```

本身就已经产生了我们需要的答案。

因此可以直接写：

```cpp
return value > 0;
```

不需要先写：

```cpp
bool result = value > 0;
return result;
```

虽然这样写也是正确的：

```cpp
static bool isPositive(int value) {
    bool result = value > 0;
    return result;
}
```

但是如果变量没有其他用途，更简洁的写法通常是：

```cpp
static bool isPositive(int value) {
    return value > 0;
}
```

---

# 5. 为什么不需要 `if`

初学者可能会写成：

```cpp
static bool isPositive(int value) {
    if (value > 0) {
        return true;
    } else {
        return false;
    }
}
```

这段代码逻辑上没有错误。

但是：

```cpp
value > 0
```

本身已经完成了判断，并且结果已经是：

```cpp
true
```

或者：

```cpp
false
```

因此再写：

```cpp
if (...) {
    return true;
} else {
    return false;
}
```

实际上是在重复工作。

可以直接简化为：

```cpp
return value > 0;
```

---

# 6. 一个非常重要的思维模式

遇到下面这种需求：

> 判断某个条件是否成立，并返回 `bool`

优先考虑：

```cpp
return 条件表达式;
```

例如：

```cpp
static bool isAdult(int age) {
    return age >= 18;
}
```

```cpp
static bool isZero(int value) {
    return value == 0;
}
```

```cpp
static bool isNegative(int value) {
    return value < 0;
}
```

```cpp
static bool isEven(int value) {
    return value % 2 == 0;
}
```

这里的：

```cpp
age >= 18
value == 0
value < 0
value % 2 == 0
```

全部都是 `bool` 表达式。

---

# 7. 边界条件非常重要

题目要求：

> 正数返回 `true`

数学上：

```text
正数：value > 0
```

注意：

```text
0 不是正数
```

因此必须使用：

```cpp
value > 0
```

而不能使用：

```cpp
value >= 0
```

两者只有在边界值 `0` 上不同。

例如：

| `value` | `value > 0` | `value >= 0` |
| ------: | ----------- | ------------ |
|     `5` | `true`      | `true`       |
|     `1` | `true`      | `true`       |
|     `0` | `false`     | `true`       |
|    `-1` | `false`     | `false`      |
|    `-7` | `false`     | `false`      |

所以测试：

```cpp
isPositive(0)
```

非常重要。

它能够区分：

```cpp
> 0
```

和：

```cpp
>= 0
```

---

# 8. 为什么测试经常包含边界值

好的测试通常不仅测试普通情况，也会测试“刚好处在分界线上的值”。

对于：

```cpp
value > 0
```

最重要的几个测试就是：

```text
负数
0
正数
```

例如：

```cpp
isPositive(-7)  // false
isPositive(0)   // false
isPositive(5)   // true
```

这里的 `0` 就是 boundary value（边界值）。

编程中常见的边界包括：

```text
0
1
-1

数组第一个元素
数组最后一个元素

最小允许值
最大允许值

空字符串
空数组
```

很多 bug 恰恰发生在这些地方。

---

# 9. `INT_MAX` 仍然可以正常比较

题目还测试了：

```cpp
2147483647
```

在常见的 32 位 `int` 环境中，这是：

```text
int 的最大值
```

也就是：

```cpp
INT_MAX
```

但是表达式：

```cpp
2147483647 > 0
```

只是普通比较，因此结果仍然是：

```cpp
true
```

这里没有发生加法、乘法等运算，所以不会产生整数溢出问题。

---

# 10. 不要直接 `return value`

有时候初学者可能会写：

```cpp
static bool isPositive(int value) {
    return value;
}
```

这虽然有可能通过 C++ 编译，但含义完全不同。

当整数被转换成 `bool` 时：

```text
0      → false
非 0   → true
```

也就是说：

```cpp
1    → true
5    → true
-1   → true
-7   → true
0    → false
```

因此：

```cpp
return value;
```

实际上判断的是：

> `value` 是否不等于 `0`

而不是：

> `value` 是否为正数

例如：

```cpp
value = -7;
```

如果写：

```cpp
return value;
```

会得到：

```cpp
true
```

因为 `-7` 是非零值。

但正确答案应该是：

```cpp
false
```

---

# 11. `bool` 与整数转换

C++ 中整数转换成 `bool` 时：

```cpp
0       -> false
非零值  -> true
```

例如：

```cpp
bool a = 0;
bool b = 1;
bool c = -3;
bool d = 100;
```

等价于：

```cpp
bool a = false;
bool b = true;
bool c = true;
bool d = true;
```

所以：

```cpp
bool b = -100;
```

仍然是：

```cpp
true
```

这也是为什么：

```cpp
return value;
```

不能用来判断正负。

---

# 12. 一个重要区别

下面两个条件不是一回事：

```cpp
value != 0
```

和：

```cpp
value > 0
```

例如：

```text
value = -5
```

那么：

```cpp
value != 0
```

结果：

```cpp
true
```

但是：

```cpp
value > 0
```

结果：

```cpp
false
```

所以应该根据真正的问题选择比较表达式。

---

# 13. 推荐的代码风格

如果函数只是返回一个条件是否成立：

推荐：

```cpp
static bool isPositive(int value) {
    return value > 0;
}
```

通常不推荐写成：

```cpp
static bool isPositive(int value) {
    if (value > 0) {
        return true;
    } else {
        return false;
    }
}
```

也没必要写：

```cpp
static bool isPositive(int value) {
    bool result = value > 0;
    return result;
}
```

最简单的思路就是：

```text
问题：它是否满足某条件？
↓
写出条件
↓
条件本身就是 bool
↓
直接 return
```

即：

```cpp
return condition;
```

---

# 14. 本节核心总结

记住以下几点：

```text
比较表达式的结果是 bool。
```

例如：

```cpp
5 > 0
```

产生：

```cpp
true
```

---

如果函数返回类型是：

```cpp
bool
```

通常可以直接：

```cpp
return 比较表达式;
```

例如：

```cpp
return value > 0;
```

---

不要把：

```cpp
value > 0
```

误写成：

```cpp
value >= 0
```

因为：

```text
0 不是正数
```

---

也不要写：

```cpp
return value;
```

因为整数转 `bool` 的规则是：

```text
0     → false
非 0  → true
```

负数也会变成 `true`。

---

最值得记住的模式是：

```cpp
bool function(...) {
    return condition;
}
```

例如：

```cpp
static bool isPositive(int value) {
    return value > 0;
}
```
