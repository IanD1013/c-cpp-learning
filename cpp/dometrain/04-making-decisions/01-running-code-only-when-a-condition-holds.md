# C++ `if` 条件语句：根据条件决定是否执行代码

## 1. `if` 是做什么的？

`if` 语句用于：

> **只有当某个条件成立时，才执行一段代码。**

基本语法：

```cpp
if (condition) {
    // condition 为 true 时执行
}
```

例如：

```cpp
bool verbose = true;
std::string line = "done";

if (verbose) {
    line = line + " in 12 steps";
}
```

因为：

```cpp
verbose == true
```

所以 `if` 中的代码会执行，最终：

```cpp
line == "done in 12 steps"
```

如果：

```cpp
bool verbose = false;
```

那么：

```cpp
if (verbose) {
    line = line + " in 12 steps";
}
```

这一部分会被跳过，`line` 仍然是：

```cpp
"done"
```

---

# 2. `if` 的核心：条件必须能表示 true / false

`if` 括号中的表达式最终需要表示一个布尔结果：

```cpp
true
```

或者：

```cpp
false
```

例如：

```cpp
if (true) {
    // 一定执行
}
```

```cpp
if (false) {
    // 永远不执行
}
```

更常见的是使用一个 `bool` 变量：

```cpp
bool ready = true;

if (ready) {
    // ready 为 true 时执行
}
```

这里不需要写成：

```cpp
if (ready == true)
```

虽然这样通常也可以工作，但属于多余写法。

更自然的是：

```cpp
if (ready)
```

因为 `ready` 本身已经是一个 `bool`。

---

# 3. 比较表达式可以直接放进 `if`

比较运算符会直接产生一个 `bool`。

例如：

```cpp
value < 0
```

它的结果只有两种：

```cpp
true
false
```

所以可以直接写：

```cpp
if (value < 0) {
    // value 是负数时执行
}
```

例如：

```cpp
int value = -5;

if (value < 0) {
    value = 0;
}
```

由于：

```cpp
-5 < 0
```

为：

```cpp
true
```

所以：

```cpp
value = 0;
```

会执行。

---

# 4. 边界值非常重要

看这个条件：

```cpp
value < 0
```

它只表示：

> `value` 严格小于 0。

所以：

```cpp
-2 < 0   // true
-1 < 0   // true
0 < 0    // false
1 < 0    // false
```

特别要注意：

```cpp
0 < 0
```

是 `false`。

因此：

```cpp
if (value < 0)
```

只处理负数，不处理 0。

---

# 5. `{}` 表示 `if` 控制的代码块

推荐始终使用大括号：

```cpp
if (condition) {
    statement1;
    statement2;
}
```

如果条件为 `true`，整个 `{}` 中的语句都会执行。

例如：

```cpp
if (value < 0) {
    value = 0;
    std::cout << "negative";
}
```

两个语句都属于这个 `if`。

---

# 6. 不写 `{}` 时，`if` 只控制下一条语句

C++ 允许：

```cpp
if (condition)
    statement;
```

例如：

```cpp
if (value < 0)
    value = 0;
```

这在语法上是正确的。

但是危险之处在于：

```cpp
if (value < 0)
    value = 0;
    count++;
```

看缩进可能会误以为：

```cpp
count++;
```

也属于 `if`。

实际上 C++ 会理解成：

```cpp
if (value < 0) {
    value = 0;
}

count++;
```

也就是说：

```cpp
count++;
```

**无论条件是真是假都会执行。**

---

# 7. C++ 不根据缩进判断代码结构

这是非常重要的一点：

> **C++ 看的是 `{}`，不是缩进。**

例如：

```cpp
if (value < 0)
    value = 0;
    result++;
```

虽然两行看起来缩进相同，但是只有：

```cpp
value = 0;
```

属于 `if`。

因此更安全的习惯是：

```cpp
if (value < 0) {
    value = 0;
    result++;
}
```

即使只有一行，也建议使用 `{}`：

```cpp
if (value < 0) {
    value = 0;
}
```

这样以后添加代码时更不容易产生 bug。

---

# 8. 一个非常危险的错误：`if (...) ;`

错误示例：

```cpp
if (value < 0);
{
    value = 0;
}
```

关键问题是这里：

```cpp
if (value < 0);
```

分号 `;` 本身就是一条完整的空语句。

因此实际上相当于：

```cpp
if (value < 0) {
    // 什么都不做
}

{
    value = 0;
}
```

结果就是：

```cpp
value = 0;
```

无论条件是真是假都会执行。

---

## 更隐蔽的例子

```cpp
if (value < 0);
    result = 0;
```

看起来像：

```cpp
if (value < 0) {
    result = 0;
}
```

但实际上不是。

真正含义是：

```cpp
if (value < 0) {
    // 空语句
}

result = 0;
```

所以 `result = 0` 永远执行。

---

# 9. 正确位置：`)` 后面直接接 `{`

正确：

```cpp
if (value < 0) {
    result = 0;
}
```

错误：

```cpp
if (value < 0); {
    result = 0;
}
```

可以记：

```text
if (条件) {
    ...
}
```

而不是：

```text
if (条件); {
    ...
}
```

---

# 10. `if` 不一定需要 `else`

很多初学者容易写成：

```cpp
if (condition) {
    ...
} else {
    ...
}
```

但实际上：

> 如果“不满足条件时什么都不需要做”，就不需要 `else`。

例如：

```cpp
int result = value;

if (value < 0) {
    result = 0;
}

return result;
```

这里最开始：

```cpp
int result = value;
```

已经完成了默认情况。

假设：

```cpp
value = 5;
```

那么：

```cpp
result = 5;
```

而：

```cpp
value < 0
```

是 `false`，所以 `if` 被跳过。

最后：

```cpp
return result;
```

自然返回：

```cpp
5
```

完全不需要：

```cpp
else {
    result = value;
}
```

因为 `result` 本来就已经等于 `value`。

---

# 11. 常见编程思路：先设置默认值，再处理特殊情况

这是非常实用的模式：

```cpp
ResultType result = defaultValue;

if (specialCondition) {
    result = specialValue;
}

return result;
```

例如：

```cpp
int result = value;

if (value < 0) {
    result = 0;
}

return result;
```

逻辑是：

```text
默认：
    result = value

特殊情况：
    如果 value 是负数
    result = 0

最后：
    返回 result
```

这种写法通常比：

```cpp
if (...) {
    ...
} else {
    ...
}
```

更简洁。

---

# 12. 用执行流程理解 `if`

假设：

```cpp
int value = -3;
int result = value;

if (value < 0) {
    result = 0;
}

return result;
```

程序执行：

```text
value = -3
↓
result = -3
↓
检查 value < 0
↓
-3 < 0 → true
↓
result = 0
↓
return 0
```

---

如果：

```cpp
value = 5;
```

执行流程：

```text
value = 5
↓
result = 5
↓
检查 value < 0
↓
5 < 0 → false
↓
跳过 if
↓
return 5
```

---

# 13. `if` 条件只检查一次

执行：

```cpp
if (value < 0) {
    ...
}
```

时，程序会先计算：

```cpp
value < 0
```

得到：

```cpp
true
```

或者：

```cpp
false
```

然后决定：

```text
true
→ 执行 if body

false
→ 跳过 if body
```

可以把它理解成：

```text
计算条件
    ↓
得到 bool
    ↓
true? ── 是 ──→ 执行 {}
    │
    否
    ↓
跳过 {}
```

---

# 14. `if` 与之前学习的 `bool` 是直接连接的

例如：

```cpp
bool isNegative = value < 0;

if (isNegative) {
    result = 0;
}
```

也可以直接简化成：

```cpp
if (value < 0) {
    result = 0;
}
```

因为：

```cpp
value < 0
```

本身已经产生 `bool`。

也就是说：

```cpp
if (comparison)
```

是非常自然的写法。

---

# 15. 常见错误总结

### 错误 1：把 `if` 写成 `if (...);`

错误：

```cpp
if (value < 0);
{
    result = 0;
}
```

结果：

```cpp
result = 0;
```

总是执行。

正确：

```cpp
if (value < 0) {
    result = 0;
}
```

---

### 错误 2：以为缩进决定代码归属

错误理解：

```cpp
if (value < 0)
    result = 0;
    count++;
```

实际上：

```cpp
if (value < 0) {
    result = 0;
}

count++;
```

---

### 错误 3：不必要地写 `== true`

可以写：

```cpp
if (ready == true) {
}
```

但更自然的是：

```cpp
if (ready) {
}
```

---

### 错误 4：把 `< 0` 写成 `<= 0`

如果题目要求：

> 只有负数变成 0。

那么应该写：

```cpp
value < 0
```

不是：

```cpp
value <= 0
```

虽然在这个具体题目中 `0` 最终仍然是 `0`，但条件语义已经不一样了。

---

# 16. 推荐编码风格

推荐：

```cpp
if (condition) {
    statement;
}
```

即使 body 只有一行：

```cpp
if (value < 0) {
    result = 0;
}
```

而不是：

```cpp
if (value < 0)
    result = 0;
```

原因不是后者错误，而是 `{}` 更：

* 清晰
* 安全
* 容易扩展
* 不容易因为增加第二行而制造 bug

---

# 17. 核心知识速记

```cpp
if (condition) {
    // condition 为 true 时执行
}
```

核心规则：

```text
if 条件最终产生 bool

true
→ 执行 {}

false
→ 跳过 {}
```

比较表达式可以直接作为条件：

```cpp
if (value < 0) {
}
```

`bool` 变量也可以直接作为条件：

```cpp
if (ready) {
}
```

不要误写：

```cpp
if (condition);
```

因为这里的：

```cpp
;
```

会变成 `if` 的空语句。

推荐始终写：

```cpp
if (condition) {
    ...
}
```

---

# 18. 一句话理解 `if`

> `if` 的意思就是：**如果这个条件是 `true`，执行这一块代码；否则直接跳过去。**
