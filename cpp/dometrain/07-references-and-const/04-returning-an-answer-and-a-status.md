# C++：用 `bool + 输出参数` 表示“操作可能失败”

## 1. 为什么不能用某个整数表示“除法失败”？

假设我们要写一个整数除法函数。

```cpp
int divide(int numerator, int denominator);
```

问题在于：

* 除法可能成功
* 也可能失败，例如：

```cpp
denominator == 0
```

因为除以 0 在 C++ 中属于 **Undefined Behavior（未定义行为）**，所以必须在真正执行除法之前检查。

```cpp
if (denominator == 0) {
    // 不能执行 numerator / denominator
}
```

---

## 2. 为什么不能用 `0`、`-1` 等作为失败标记？

一种看似简单的方法是：

```cpp
return 0;
```

表示失败。

但问题是 `0` 本身完全可能是正确答案：

```cpp
0 / 4 == 0
3 / 5 == 0
```

类似地，`-1` 也可能是合法结果：

```cpp
-3 / 2 == -1
```

因此对于整数除法来说：

> **不存在一个特殊的 `int` 值，可以可靠地同时表示“失败”。**

这就是 sentinel value（哨兵值）的局限。

当所有可能的值都可能是合法答案时，应该把：

1. **计算结果**
2. **计算是否成功**

分开表示。

---

# 3. 使用 `bool` 表示状态

一种常见设计是：

```cpp
static bool tryDivide(
    int numerator,
    int denominator,
    int &quotient
)
```

这里有两条输出渠道：

```text
return bool
    ↓
操作是否成功

quotient
    ↓
真正的计算结果
```

也就是说：

```cpp
bool
```

负责告诉调用者：

```text
true  → 成功
false → 失败
```

而：

```cpp
int &quotient
```

负责保存真正的除法结果。

---

# 4. 什么是 out-parameter？

例如：

```cpp
static bool tryHalf(int total, int &result) {
    if (total < 0) {
        return false;
    }

    result = total / 2;
    return true;
}
```

调用：

```cpp
int half = 0;
bool ok = tryHalf(9, half);
```

执行后：

```cpp
ok == true
half == 4
```

这里：

```cpp
int &result
```

是一个 **reference parameter（引用参数）**。

因为函数通过它把结果写回调用者，所以这种参数通常叫：

> **out-parameter（输出参数）**

可以理解为：

```text
普通 return
    → 返回成功 / 失败状态

引用参数
    → 把计算结果写回调用者
```

---

# 5. 引用输出参数实际上修改的是调用者变量

例如：

```cpp
int half = 0;
tryHalf(9, half);
```

调用时：

```cpp
result
```

实际上就是：

```cpp
half
```

的另一个名字。

因此函数内部：

```cpp
result = total / 2;
```

相当于直接修改：

```cpp
half
```

最终：

```cpp
half == 4
```

---

# 6. 一个很重要的函数契约：失败时不要修改输出参数

推荐让函数满足这个规则：

> 如果函数返回 `false`，输出参数保持原值不变。

例如：

```cpp
static bool tryDivide(
    int numerator,
    int denominator,
    int &quotient
) {
    if (denominator == 0) {
        return false;
    }

    quotient = numerator / denominator;
    return true;
}
```

注意失败路径：

```cpp
if (denominator == 0) {
    return false;
}
```

没有写：

```cpp
quotient = ...
```

因此：

```cpp
int answer = 123;

bool ok = tryDivide(10, 0, answer);
```

执行以后：

```cpp
ok == false
answer == 123
```

`answer` 保持原值。

---

# 7. 为什么调用者必须先检查 `bool`？

考虑：

```cpp
int answer = 0;
bool ok = tryDivide(10, 0, answer);
```

此时：

```cpp
ok == false
```

而：

```cpp
answer == 0
```

但是这个 `0` 并不是除法结果。

它只是调用前：

```cpp
int answer = 0;
```

留下来的初始值。

所以不能看到：

```cpp
answer == 0
```

就认为答案是 0。

正确逻辑应该是：

```cpp
if (ok) {
    // 此时才可以把 answer 当作真正结果
}
```

核心原则：

> **输出参数中的值是否有效，由状态值决定。**

---

# 8. `tryDivide` 的正确逻辑

函数：

```cpp
static bool tryDivide(
    int numerator,
    int denominator,
    int &quotient
)
```

应该遵循：

```text
denominator == 0
        ↓
return false
不要修改 quotient

denominator != 0
        ↓
quotient = numerator / denominator
return true
```

代码：

```cpp
static bool tryDivide(
    int numerator,
    int denominator,
    int &quotient
) {
    if (denominator == 0) {
        return false;
    }

    quotient = numerator / denominator;
    return true;
}
```

---

# 9. C++ 的整数除法：向 0 截断

这里的参数都是：

```cpp
int
```

所以：

```cpp
numerator / denominator
```

执行的是整数除法。

例如：

```cpp
7 / 2 == 3
```

而：

```cpp
-7 / 2 == -3
```

注意不是：

```cpp
-4
```

C++ 的整数除法会：

> **truncate toward zero（向 0 截断）**

例如：

```text
 7 / 2  =  3.5 →  3
-7 / 2  = -3.5 → -3
```

---

# 10. `divideOrFallback` 的设计

现在再写一个包装函数：

```cpp
static int divideOrFallback(
    int numerator,
    int denominator,
    int fallback
)
```

目的：

```text
除法成功
    → 返回真正的 quotient

除法失败
    → 返回调用者提供的 fallback
```

例如：

```cpp
divideOrFallback(10, 2, -1)
```

返回：

```cpp
5
```

而：

```cpp
divideOrFallback(10, 0, -1)
```

返回：

```cpp
-1
```

---

# 11. `fallback` 是调用者传进来的值

非常重要：

```cpp
fallback
```

不是固定的：

```cpp
-1
```

调用者可以写：

```cpp
divideOrFallback(10, 0, -1);
```

也可以：

```cpp
divideOrFallback(10, 0, 99);
```

甚至：

```cpp
divideOrFallback(10, 0, 12345);
```

因此失败时必须：

```cpp
return fallback;
```

而不是：

```cpp
return -1;
```

---

# 12. `divideOrFallback` 应该真正调用 `tryDivide`

正确结构：

```cpp
static int divideOrFallback(
    int numerator,
    int denominator,
    int fallback
) {
    int answer = 0;

    bool ok = tryDivide(
        numerator,
        denominator,
        answer
    );

    if (!ok) {
        return fallback;
    }

    return answer;
}
```

逻辑流程：

```text
int answer = 0
      ↓
调用 tryDivide(...)
      ↓
   返回 bool
    ↙     ↘
 false    true
   ↓        ↓
fallback   answer
```

---

# 13. 为什么不要自己重新做一次除法？

例如不推荐：

```cpp
static int divideOrFallback(
    int numerator,
    int denominator,
    int fallback
) {
    if (denominator == 0) {
        return fallback;
    }

    return numerator / denominator;
}
```

虽然结果可能一样，但是这样：

```cpp
divideOrFallback
```

完全绕过了：

```cpp
tryDivide
```

失去了这个练习真正想训练的设计：

```text
底层 helper：
tryDivide()

负责：
- 检查失败
- 执行计算
- 写入输出参数
- 返回状态


上层 wrapper：
divideOrFallback()

负责：
- 调用 helper
- 根据状态决定返回结果还是 fallback
```

这种“底层函数报告状态，上层函数决定怎么办”的结构在真实程序中非常常见。

---

# 14. 常见错误

## 错误 1：先除法，再检查 denominator

错误：

```cpp
quotient = numerator / denominator;

if (denominator == 0) {
    return false;
}
```

因为：

```cpp
numerator / 0
```

已经发生了。

必须先检查：

```cpp
if (denominator == 0) {
    return false;
}
```

---

## 错误 2：失败时修改 quotient

例如：

```cpp
if (denominator == 0) {
    quotient = 0;
    return false;
}
```

这样会破坏：

> `false` 时输出参数保持不变

这个函数契约。

更好的写法：

```cpp
if (denominator == 0) {
    return false;
}
```

---

## 错误 3：忽略 `bool`

例如：

```cpp
int answer = 0;
tryDivide(numerator, denominator, answer);

return answer;
```

如果除数是 0：

```cpp
answer
```

仍然只是初始值：

```cpp
0
```

不能把它当成真正的除法结果。

---

## 错误 4：把 fallback 预先放进 answer

例如：

```cpp
int answer = fallback;
tryDivide(numerator, denominator, answer);
return answer;
```

虽然某些输入下最终数字碰巧正确，但它绕过了状态判断。

这个练习希望表达的是：

```cpp
bool ok = tryDivide(...);

if (!ok) {
    return fallback;
}

return answer;
```

即：

> **状态决定结果是否有效。**

---

# 15. 一个更一般的模式

这种设计不仅适用于除法。

通用模式：

```cpp
bool trySomething(Input input, Output &result);
```

例如：

```cpp
bool tryParse(...);
bool tryFind(...);
bool tryConvert(...);
bool tryDivide(...);
```

共同特点：

```text
成功：
    result 被写入
    return true

失败：
    result 不修改
    return false
```

调用方：

```cpp
Output result;

if (trySomething(input, result)) {
    // 使用 result
} else {
    // 处理失败
}
```

---

# 16. 核心总结

遇到一个“可能失败，但所有返回值本身都可能合法”的操作时，不要硬塞一个特殊值表示失败。

可以把：

```text
是否成功
```

和：

```text
真正结果
```

分开表示：

```cpp
bool tryDivide(
    int numerator,
    int denominator,
    int &quotient
);
```

其中：

```cpp
bool
```

表示：

```text
true  = 成功
false = 失败
```

而：

```cpp
int &quotient
```

作为输出参数保存真正结果。

最重要的模式是：

```cpp
if (失败条件) {
    return false;
}

result = ...;
return true;
```

以及调用者：

```cpp
bool ok = trySomething(...);

if (!ok) {
    // 处理失败
}

// 此时结果才有效
```
