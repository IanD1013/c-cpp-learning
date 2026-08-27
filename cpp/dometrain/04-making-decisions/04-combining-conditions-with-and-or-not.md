# C++ 条件组合：`&&`、`||`、`!`

在 C++ 中，一个比较表达式通常会产生一个 `bool`：

```cpp
int value = 10;

value > 0    // true
value == 10  // true
value < 5    // false
```

但实际程序里的条件往往不只包含一个判断。

例如：

> 一个数字既要大于 0，又要小于 100。

这时就需要使用 **逻辑运算符（logical operators）** 来组合多个条件。

C++ 中最常用的三个逻辑运算符是：

| 运算符  | 含义    | 示例                    |      |                        |
| ---- | ----- | --------------------- | ---- | ---------------------- |
| `&&` | AND，与 | 两边都为 `true` 才是 `true` |      |                        |
| `    |       | `                     | OR，或 | 至少一边为 `true` 就是 `true` |
| `!`  | NOT，非 | 把 `true` 和 `false` 反转 |      |                        |

---

# 1. `&&`：逻辑与 AND

`&&` 表示：

> 左边和右边必须同时成立。

例如：

```cpp
int width = 4;
int height = 3;

bool bothPositive = width > 0 && height > 0;
```

可以把它理解成：

```text
width > 0
AND
height > 0
```

这里：

```cpp
width > 0
```

得到：

```cpp
true
```

而：

```cpp
height > 0
```

也得到：

```cpp
true
```

所以：

```cpp
true && true
```

最终结果是：

```cpp
true
```

---

## `&&` 真值表

```text
true  && true   -> true
true  && false  -> false
false && true   -> false
false && false  -> false
```

也就是说：

> `&&` 只有在两个条件全部成立时才成立。

---

# 2. `||`：逻辑或 OR

`||` 表示：

> 左边或右边至少有一个成立即可。

例如：

```cpp
int width = 4;
int height = 3;

bool eitherZero = width == 0 || height == 0;
```

意思是：

```text
width 是 0
OR
height 是 0
```

这里：

```cpp
width == 0   // false
height == 0  // false
```

因此：

```cpp
false || false
```

结果为：

```cpp
false
```

---

## `||` 真值表

```text
true  || true   -> true
true  || false  -> true
false || true   -> true
false || false  -> false
```

因此：

> 只要有一个条件成立，`||` 整体就成立。

---

# 3. `!`：逻辑非 NOT

`!` 用于反转一个布尔值。

```cpp
!true   // false
!false  // true
```

例如：

```cpp
bool eitherZero = false;

bool usable = !eitherZero;
```

因为：

```cpp
eitherZero == false
```

所以：

```cpp
!eitherZero
```

就是：

```cpp
true
```

完整例子：

```cpp
int width = 4;
int height = 3;

bool bothPositive = width > 0 && height > 0;
bool eitherZero   = width == 0 || height == 0;
bool usable       = !eitherZero;
```

此时：

```cpp
bothPositive == true
eitherZero == false
usable == true
```

---

# 4. 比较运算符与逻辑运算符的优先级

考虑：

```cpp
width > 0 && height > 0
```

C++ 会自动把它理解成：

```cpp
(width > 0) && (height > 0)
```

而不是：

```cpp
width > (0 && height) > 0
```

原因是：

> 比较运算符的优先级高于 `&&`。

因此这种代码通常不需要额外括号：

```cpp
width > 0 && height > 0
```

当然，写成：

```cpp
(width > 0) && (height > 0)
```

也完全正确，而且有时可读性更好。

---

# 5. `&&` 的优先级高于 `||`

如果一个表达式里同时出现：

```cpp
&&
||
```

那么 `&&` 会先组合。

例如：

```cpp
a || b && c
```

等价于：

```cpp
a || (b && c)
```

而不是：

```cpp
(a || b) && c
```

因此：

```cpp
&&
```

比：

```cpp
||
```

优先级更高。

---

## 如果逻辑关系复杂，建议主动加括号

例如：

```cpp
if ((isAdmin || isOwner) && isActive) {
    // ...
}
```

这通常比完全依赖运算符优先级更容易阅读。

写条件表达式时：

> 可读性往往比“少写两个括号”更重要。

---

# 6. `!` 的优先级非常高

`!` 的优先级比比较运算符还高。

这是一个非常容易踩坑的地方。

例如：

```cpp
!width == 9
```

很多初学者可能以为它的意思是：

```cpp
!(width == 9)
```

也就是：

> width 不等于 9

但实际上不是。

C++ 会把：

```cpp
!width == 9
```

理解成：

```cpp
(!width) == 9
```

---

## 为什么？

假设：

```cpp
int width = 4;
```

首先执行：

```cpp
!width
```

在布尔上下文中：

```cpp
4
```

属于非零值，因此相当于：

```cpp
true
```

所以：

```cpp
!4
```

得到：

```cpp
false
```

也就是：

```cpp
0
```

然后再执行：

```cpp
0 == 9
```

得到：

```cpp
false
```

---

# 7. 如何正确表达“不等于”

如果只是判断：

```text
width 不等于 9
```

应该直接写：

```cpp
width != 9
```

这是最清晰的写法。

也可以写：

```cpp
!(width == 9)
```

但通常没有必要。

因此优先推荐：

```cpp
width != 9
```

而不是：

```cpp
!(width == 9)
```

更不要写：

```cpp
!width == 9
```

---

# 8. 否定一个复杂条件时要加括号

例如：

```cpp
!(value > 10)
```

表示：

```text
value > 10
```

这个整个条件的相反结果。

因此：

```cpp
!(value > 10)
```

等价于：

```cpp
value <= 10
```

再例如：

```cpp
!(a == b)
```

等价于：

```cpp
a != b
```

---

# 9. 给复杂条件起 `bool` 名字

复杂条件不一定要全部塞进一个 `if`。

例如：

```cpp
if (
    value >= low &&
    value <= high &&
    value != excludedA &&
    value != excludedB
) {
    // ...
}
```

虽然完全正确，但如果条件继续变复杂，可读性会下降。

可以拆成：

```cpp
bool inRange =
    value >= low &&
    value <= high;

bool excluded =
    value == excludedA ||
    value == excludedB;

bool valid =
    inRange && !excluded;
```

这样代码更接近自然语言：

```text
valid =
    在范围内
    并且
    没有被排除
```

这是一种非常值得养成的编码习惯。

---

# 10. Short-circuit：短路求值

`&&` 和 `||` 都具有一个非常重要的特性：

> short-circuit evaluation
> 短路求值

意思是：

> 当左边已经足以确定整个表达式结果时，右边不会执行。

---

# 11. `&&` 的短路规则

对于：

```cpp
A && B
```

C++ 会先计算：

```cpp
A
```

如果：

```cpp
A == false
```

那么整个：

```cpp
A && B
```

已经必定是：

```cpp
false
```

所以：

```cpp
B
```

根本不会执行。

---

## 示例

```cpp
if (ptr != nullptr && ptr->value > 0) {
    // ...
}
```

执行顺序是：

```text
1. 检查 ptr != nullptr
2. 如果 false，立刻停止
3. 不执行 ptr->value
```

这非常重要。

因为如果：

```cpp
ptr == nullptr
```

那么：

```cpp
ptr->value
```

本来会访问无效地址。

但由于 `&&` 的短路机制：

```cpp
ptr != nullptr && ptr->value > 0
```

是安全的。

---

# 12. `||` 的短路规则

对于：

```cpp
A || B
```

如果：

```cpp
A == true
```

那么整个表达式已经必定是：

```cpp
true
```

所以：

```cpp
B
```

不会执行。

例如：

```cpp
if (cached || loadFromDatabase()) {
    // ...
}
```

如果：

```cpp
cached == true
```

那么：

```cpp
loadFromDatabase()
```

不会被调用。

---

# 13. 短路可以避免昂贵操作

例如：

```cpp
bool valid =
    quickCheck() &&
    expensiveCheck();
```

如果：

```cpp
quickCheck()
```

已经返回：

```cpp
false
```

那么：

```cpp
expensiveCheck()
```

不会运行。

因此可以把：

> 更便宜、更可能提前失败的检查

放在左边。

---

# 14. 已经计算好的 `bool` 不会因为短路“省掉之前的工作”

例如：

```cpp
bool first = expensiveCheckA();
bool second = expensiveCheckB();

bool result = first && second;
```

虽然：

```cpp
first && second
```

本身具有短路规则，但：

```cpp
expensiveCheckA()
expensiveCheckB()
```

早在前两行就都已经执行完成了。

所以这里：

```cpp
first && second
```

并不能避免：

```cpp
expensiveCheckB()
```

的执行。

如果你希望利用短路：

```cpp
bool result =
    expensiveCheckA() &&
    expensiveCheckB();
```

这样只有第一个返回 `true` 时才会执行第二个。

---

# 15. C++ 不能像数学一样写连续比较

数学中可以写：

```text
0 ≤ x ≤ 10
```

但 C++ 不应该写：

```cpp
0 <= value <= 10
```

虽然它可能可以编译，但它的含义完全不是数学里的连续比较。

---

# 16. 为什么 `low <= value <= high` 是错的？

假设：

```cpp
int value = 20;
int low = 0;
int high = 10;
```

写：

```cpp
low <= value <= high
```

C++ 实际会按照：

```cpp
(low <= value) <= high
```

处理。

首先：

```cpp
low <= value
```

就是：

```cpp
0 <= 20
```

结果：

```cpp
true
```

而 `true` 在数字比较中可以转换成：

```cpp
1
```

于是下一步变成：

```cpp
1 <= 10
```

结果：

```cpp
true
```

最终程序居然认为：

```cpp
20
```

在：

```text
0 ~ 10
```

这个范围内。

显然是错误的。

---

# 17. 正确的范围判断

应该把上下界分别比较：

```cpp
low <= value && value <= high
```

或者：

```cpp
value >= low && value <= high
```

例如：

```cpp
bool inRange =
    value >= low &&
    value <= high;
```

这表示：

```text
value >= low
AND
value <= high
```

也就是：

```text
low <= value <= high
```

对应的正确 C++ 写法。

---

# 18. Inclusive Range：包含边界

如果题目说：

> value lies between low and high, both bounds counting as inside

意思是：

> `low` 和 `high` 本身也算在范围内。

因此需要使用：

```cpp
>=
<=
```

而不是：

```cpp
>
<
```

正确：

```cpp
value >= low && value <= high
```

例如：

```cpp
low = 5;
high = 10;
```

那么：

```text
value = 5   -> true
value = 6   -> true
value = 10  -> true
```

但：

```text
value = 4   -> false
value = 11  -> false
```

---

# 19. 排除特定值

假设：

```cpp
excludedA
excludedB
```

表示两个不能接受的值。

那么判断：

> value 是否等于其中任意一个

可以写：

```cpp
bool excluded =
    value == excludedA ||
    value == excludedB;
```

因为：

```cpp
||
```

表示：

> 至少满足一个条件。

---

# 20. 判断“没有被排除”

如果：

```cpp
excluded
```

表示：

```text
value 被排除
```

那么：

```cpp
!excluded
```

自然就是：

```text
value 没有被排除
```

因此最终条件可以写：

```cpp
inRange && !excluded
```

意思是：

```text
在范围内
AND
没有被排除
```

---

# 21. 空范围不需要特殊处理

假设：

```cpp
low > high
```

例如：

```cpp
low = 10;
high = 5;
```

这种范围叫：

```text
empty range
```

因为不存在一个数字能同时满足：

```cpp
value >= 10
```

和：

```cpp
value <= 5
```

我们的正常条件：

```cpp
value >= low && value <= high
```

会自动得到：

```cpp
false
```

因此通常不需要额外写：

```cpp
if (low > high) {
    return false;
}
```

因为逻辑表达式本身已经正确处理了这种情况。

---

# 22. 一个完整的思考模板

如果需求是：

> value 必须位于 `[low, high]` 范围内，并且不能等于 excludedA 或 excludedB。

可以把问题分成两个独立问题。

第一步：

```text
value 是否在范围内？
```

```cpp
bool inRange =
    value >= low &&
    value <= high;
```

第二步：

```text
value 是否属于被排除的值？
```

```cpp
bool excluded =
    value == excludedA ||
    value == excludedB;
```

最终：

```text
在范围内
AND
没有被排除
```

```cpp
return inRange && !excluded;
```

这种：

```text
拆分条件
→ 给条件命名
→ 最后组合
```

的写法在真实项目中非常常见。

---

# 23. 运算符优先级总结

在本节涉及的运算符中，可以大致记住：

```text
!
↓
< <= > >= == !=
↓
&&
↓
||
```

也就是：

```text
!            优先级最高
comparison
&&
||           优先级最低
```

例如：

```cpp
value >= low && value <= high || admin
```

会被理解成：

```cpp
(value >= low && value <= high) || admin
```

不过当逻辑越来越复杂时，不建议完全依赖记忆优先级。

更推荐明确写：

```cpp
bool inRange =
    value >= low &&
    value <= high;

if (inRange || admin) {
    // ...
}
```

---

# 24. 常见错误总结

## 错误 1：把数学连续比较搬到 C++

错误：

```cpp
low <= value <= high
```

正确：

```cpp
low <= value && value <= high
```

---

## 错误 2：错误使用 `!`

错误：

```cpp
!value == 10
```

这实际上接近：

```cpp
(!value) == 10
```

如果你想写“不等于”：

```cpp
value != 10
```

---

## 错误 3：把 AND 和 OR 写反

如果要求：

> 两个条件都满足

应该使用：

```cpp
&&
```

如果要求：

> 任意一个条件满足

应该使用：

```cpp
||
```

---

## 错误 4：忘记短路会影响函数是否执行

```cpp
condition && function();
```

当：

```cpp
condition == false
```

时：

```cpp
function()
```

不会执行。

同样：

```cpp
condition || function();
```

当：

```cpp
condition == true
```

时：

```cpp
function()
```

不会执行。

如果函数具有副作用，这一点尤其重要。

---

# 25. 本节核心知识

最重要的是记住下面几条：

```cpp
A && B
```

表示：

```text
A 和 B 都必须成立
```

```cpp
A || B
```

表示：

```text
A 和 B 至少一个成立
```

```cpp
!A
```

表示：

```text
A 的相反结果
```

范围判断不能写：

```cpp
low <= value <= high
```

而应该写：

```cpp
value >= low && value <= high
```

复杂条件可以拆成有意义的 `bool`：

```cpp
bool inRange = ...;
bool excluded = ...;

return inRange && !excluded;
```

而 `&&` 和 `||` 最值得额外记住的特性是：

```text
short-circuit evaluation
短路求值
```

即：

```cpp
false && anything
```

右边不会执行；

```cpp
true || anything
```

右边也不会执行。
