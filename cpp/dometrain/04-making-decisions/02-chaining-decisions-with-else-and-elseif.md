# C++ `if / else if / else` 条件分支教程

## 1. `else` 是什么？

在 C++ 中，`else` 表示：

> 如果前面的 `if` 条件不成立，就执行这里。

例如：

```cpp
bool stored = true;
std::string state;

if (stored) {
    state = "has contents";
} else {
    state = "nothing stored";
}
```

执行过程：

```text
检查 stored
   │
   ├── true  → state = "has contents"
   │
   └── false → state = "nothing stored"
```

因为：

```cpp
stored == true
```

所以最终：

```cpp
state == "has contents"
```

这里最重要的一点是：

> `if` 和 `else` 是互斥的。

也就是说，一次判断中只会执行其中一个分支。

不会出现：

```text
if 执行
然后 else 也执行
```

这种情况。

---

# 2. 基本结构：`if ... else`

最基本的二选一结构是：

```cpp
if (condition) {
    // condition 为 true 时执行
} else {
    // condition 为 false 时执行
}
```

例如：

```cpp
int age = 20;
std::string result;

if (age >= 18) {
    result = "adult";
} else {
    result = "minor";
}
```

因为：

```cpp
age >= 18
```

为 `true`，所以：

```cpp
result == "adult"
```

---

# 3. `else if`：处理多个情况

如果不只是两个情况，而是多个情况，可以使用：

```cpp
if (...) {
    ...
} else if (...) {
    ...
} else if (...) {
    ...
} else {
    ...
}
```

例如：

```cpp
int count = 3;
std::string size;

if (count < 1) {
    size = "none";
} else if (count < 4) {
    size = "a few";
} else {
    size = "many";
}
```

这里相当于把所有 `count` 分成三个范围：

```text
count < 1          → "none"

1 <= count < 4     → "a few"

count >= 4         → "many"
```

---

# 4. `if / else if / else` 是从上往下检查的

一个条件链：

```cpp
if (condition1) {
    ...
} else if (condition2) {
    ...
} else if (condition3) {
    ...
} else {
    ...
}
```

会按照下面的顺序执行：

```text
检查 condition1
    │
    ├── true → 执行第一个分支 → 整个条件链结束
    │
    └── false
         ↓
检查 condition2
    │
    ├── true → 执行第二个分支 → 整个条件链结束
    │
    └── false
         ↓
检查 condition3
    │
    ├── true → 执行第三个分支 → 整个条件链结束
    │
    └── false
         ↓
执行 else
```

因此核心规则是：

> **从上往下判断，第一个匹配的分支获胜。**

英文里经常叫：

> **first match wins**

一旦某个条件为 `true`，下面所有 `else if` 都不会继续检查。

---

# 5. 为什么条件顺序非常重要？

来看：

```cpp
int count = 0;

if (count < 1) {
    size = "none";
} else if (count < 4) {
    size = "a few";
} else {
    size = "many";
}
```

`count == 0` 时：

```cpp
count < 1
```

为 `true`。

因此直接：

```cpp
size = "none";
```

然后整个条件链结束。

后面的：

```cpp
count < 4
```

虽然其实也为 `true`，但根本不会再执行。

---

## 如果顺序反过来呢？

```cpp
if (count < 4) {
    size = "a few";
} else if (count < 1) {
    size = "none";
}
```

如果：

```cpp
count == 0
```

第一个条件：

```cpp
count < 4
```

已经为 `true`。

于是：

```cpp
size = "a few";
```

第二个：

```cpp
count < 1
```

永远没有机会执行。

因此结果就错了。

---

# 6. 一个重要技巧：从小范围到大范围判断

很多数值范围问题都可以写成：

```cpp
if (value < boundary1) {
    ...
} else if (value < boundary2) {
    ...
} else if (value < boundary3) {
    ...
} else {
    ...
}
```

例如：

```cpp
if (score < 50) {
    grade = "fail";
} else if (score < 70) {
    grade = "pass";
} else if (score < 85) {
    grade = "good";
} else {
    grade = "excellent";
}
```

你不需要写：

```cpp
score >= 50 && score < 70
```

因为能够走到第二个分支，本身就意味着：

```cpp
score < 50
```

已经失败了。

所以自然已经知道：

```cpp
score >= 50
```

因此：

```cpp
else if (score < 70)
```

实际表达的是：

```text
50 <= score < 70
```

---

# 7. 为什么后面的分支只需要写“上界”？

来看：

```cpp
if (count < 1) {
    size = "none";
} else if (count < 4) {
    size = "a few";
} else {
    size = "many";
}
```

第二个分支看起来只写了：

```cpp
count < 4
```

但它实际上处理的是：

```text
1 <= count < 4
```

为什么？

因为只有：

```cpp
count < 1
```

为 `false`，程序才可能进入第二个判断。

也就是说，到达这里时，我们已经知道：

```cpp
count >= 1
```

然后再检查：

```cpp
count < 4
```

因此两个信息组合起来：

```text
count >= 1
并且
count < 4
```

也就是：

```text
1 <= count < 4
```

这就是 `else if` 链非常方便的地方。

---

# 8. `else` 不需要条件

最后通常写：

```cpp
else {
    ...
}
```

而不是：

```cpp
else (...)   // 错误
```

因为 `else` 的意思就是：

> 前面所有条件都不成立时执行。

例如：

```cpp
if (value < 0) {
    ...
} else if (value < 10) {
    ...
} else if (value < 100) {
    ...
} else {
    ...
}
```

最后的 `else` 自动表示：

```text
value >= 100
```

根本不需要再次写条件。

---

# 9. `else if` 链和多个独立 `if` 完全不同

这是非常重要的区别。

## 写法 A：一个条件链

```cpp
if (value < 0) {
    ...
} else if (value < 10) {
    ...
} else if (value < 100) {
    ...
} else {
    ...
}
```

特点：

> 最多只有一个分支执行。

---

## 写法 B：多个独立 `if`

```cpp
if (value < 0) {
    ...
}

if (value < 10) {
    ...
}

if (value < 100) {
    ...
}
```

特点：

> 每一个 `if` 都会独立检查。

因此可能有多个 `if` 同时执行。

---

# 10. 一个例子理解两者区别

假设：

```cpp
int value = 5;
std::string label;
```

如果写：

```cpp
if (value < 10) {
    label = "one digit";
}

if (value < 100) {
    label = "two digits";
}
```

因为：

```cpp
5 < 10    // true
5 < 100   // true
```

两个 `if` 都执行。

首先：

```cpp
label = "one digit";
```

然后又执行：

```cpp
label = "two digits";
```

最终：

```cpp
label == "two digits"
```

前一次赋值被覆盖了。

这通常不是我们想要的。

---

# 11. 使用 `else if` 就不会覆盖

改成：

```cpp
if (value < 10) {
    label = "one digit";
} else if (value < 100) {
    label = "two digits";
}
```

如果：

```cpp
value == 5
```

第一个条件：

```cpp
value < 10
```

为 `true`。

执行：

```cpp
label = "one digit";
```

然后整个条件链结束。

第二个条件：

```cpp
value < 100
```

根本不会检查。

所以结果正确。

---

# 12. `else` 属于哪个 `if`？

`else` 会和它前面对应的、尚未匹配 `else` 的 `if` 配对。

例如：

```cpp
if (condition) {
    ...
} else {
    ...
}
```

非常清楚。

但是如果省略 `{}`，可能出现容易误解的代码：

```cpp
if (a)
    if (b)
        doSomething();
    else
        doSomethingElse();
```

这里的：

```cpp
else
```

属于：

```cpp
if (b)
```

而不是：

```cpp
if (a)
```

这种问题叫做：

> dangling `else`

因此学习阶段以及实际工程代码中，都建议始终使用 `{}`：

```cpp
if (a) {
    if (b) {
        doSomething();
    } else {
        doSomethingElse();
    }
}
```

可读性会高很多。

---

# 13. 条件链可能没有任何分支执行

如果写：

```cpp
std::string result = "unknown";

if (value < 0) {
    result = "negative";
} else if (value < 10) {
    result = "small";
}
```

假设：

```cpp
value == 100
```

那么：

```cpp
value < 0    // false
value < 10   // false
```

两个分支都不执行。

因此：

```cpp
result
```

继续保留原来的：

```cpp
"unknown"
```

如果希望所有可能情况都有结果，通常应该加上：

```cpp
else {
    ...
}
```

---

# 14. 如何分析一个条件链？

遇到这种代码：

```cpp
if (value < 0) {
    ...
} else if (value < 10) {
    ...
} else if (value < 100) {
    ...
} else {
    ...
}
```

最好不要只盯着每一个条件单独看，而是把它转换成范围：

| 分支            | 实际范围                |
| ------------- | ------------------- |
| `value < 0`   | `value < 0`         |
| `value < 10`  | `0 <= value < 10`   |
| `value < 100` | `10 <= value < 100` |
| `else`        | `value >= 100`      |

这是理解 `else if` 最有效的方法之一。

---

# 15. 一个非常常见的范围分类模式

例如我们想把数字分类：

```text
负数
0~9
10~99
100+
```

推荐写：

```cpp
if (value < 0) {
    ...
} else if (value < 10) {
    ...
} else if (value < 100) {
    ...
} else {
    ...
}
```

而不是写得非常冗长：

```cpp
if (value < 0) {
    ...
} else if (value >= 0 && value <= 9) {
    ...
} else if (value >= 10 && value <= 99) {
    ...
} else if (value >= 100) {
    ...
}
```

后者虽然也能工作，但重复了很多已经由前面分支保证的信息。

通常第一种更简洁、更容易维护。

---

# 16. 记忆模型

可以把：

```cpp
if
else if
else if
else
```

想象成一道从上往下的关卡：

```text
          value
            │
            ▼
      第一个条件？
       /        \
    true        false
     │             │
   执行           ▼
   结束       第二个条件？
              /        \
           true        false
            │             │
          执行            ▼
          结束        第三个条件？
                       ...
                        │
                        ▼
                       else
```

关键点只有一句话：

> **从上往下，第一个满足条件的分支执行，其他分支全部跳过。**

---

# 17. `if`、`else if`、`else` 的职责

可以这样理解：

```cpp
if (...)
```

表示：

> 首先检查这个情况。

```cpp
else if (...)
```

表示：

> 如果前面的情况不成立，再检查这个情况。

```cpp
else
```

表示：

> 如果前面的情况全部不成立，那就一定属于这里。

---

# 18. 最重要的知识点总结

## `if`

```cpp
if (condition) {
    ...
}
```

条件为 `true` 才执行。

---

## `if / else`

```cpp
if (condition) {
    ...
} else {
    ...
}
```

两个分支必定执行其中一个。

---

## `if / else if / else`

```cpp
if (...) {
    ...
} else if (...) {
    ...
} else {
    ...
}
```

表示多个互斥情况。

执行原则：

```text
从上往下
↓
找到第一个 true
↓
执行该分支
↓
停止整个链
```

---

## 多个独立 `if`

```cpp
if (...) {
    ...
}

if (...) {
    ...
}
```

每一个条件都会检查，因此可能执行多个分支。

---

## 范围问题的常见写法

```cpp
if (value < 0) {
    ...
} else if (value < 10) {
    ...
} else if (value < 100) {
    ...
} else {
    ...
}
```

实际表示：

```text
value < 0

0 <= value < 10

10 <= value < 100

value >= 100
```

---

# 一句话记忆

> `else if` 链是“从上往下，第一个匹配就停止”；多个独立 `if` 则是“每一个条件都检查”。
