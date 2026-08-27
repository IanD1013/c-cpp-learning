# C++ 基础算术运算：`+`、`-`、`*`、`/`

## 1. C++ 中最基本的四种算术运算

C++ 中最常见的四个算术运算符是：

| 运算符 | 含义 | 示例      |
| --- | -- | ------- |
| `+` | 加法 | `a + b` |
| `-` | 减法 | `a - b` |
| `*` | 乘法 | `a * b` |
| `/` | 除法 | `a / b` |

例如：

```cpp
int a = 12;
int b = 4;

int sum = a + b;         // 16
int difference = a - b;  // 8
int product = a * b;     // 48
int share = a / b;       // 3
```

这里：

```cpp
a + b
```

是一个**表达式（expression）**。

它：

1. 读取 `a` 的值；
2. 读取 `b` 的值；
3. 进行计算；
4. 产生一个新的结果。

例如：

```cpp
a + b
```

产生：

```text
16
```

但它不会修改 `a` 或 `b`。

所以计算完成后：

```cpp
a == 12
b == 4
```

仍然成立。

---

# 2. 算术运算符不会自动修改操作数

理解这一点非常重要。

例如：

```cpp
int a = 12;
int b = 4;

a * b;
```

虽然：

```cpp
a * b
```

会计算出：

```text
48
```

但是这个结果没有被保存。

因此这行代码实际上等于：

```text
计算出 48
然后直接丢掉
```

`a` 和 `b` 都不会发生变化。

执行后仍然是：

```cpp
a == 12
b == 4
```

---

## 结果需要有地方接收

通常有三种常见方式。

### 方法一：初始化变量

```cpp
int product = a * b;
```

这里：

```cpp
a * b
```

计算得到 `48`，然后存入：

```cpp
product
```

---

### 方法二：赋值

```cpp
int result = 0;

result = a * b;
```

先计算：

```cpp
a * b
```

再把结果赋值给：

```cpp
result
```

---

### 方法三：直接 `return`

```cpp
static int multiply(int a, int b) {
    return a * b;
}
```

这里不需要额外变量。

程序会：

```text
计算 a * b
↓
得到结果
↓
直接返回给调用者
```

这通常也是简单函数最简洁的写法。

---

# 3. Operand：什么是“操作数”？

一个算术运算符两边参与计算的值叫作：

> **operand（操作数）**

例如：

```cpp
a * b
```

这里：

```cpp
a
```

和：

```cpp
b
```

就是两个 operand。

---

## 操作数可以是什么？

操作数不一定是变量。

它可以是：

### 字面量

```cpp
12 * 4
```

这里 `12` 和 `4` 是整数常量。

---

### 变量

```cpp
a * b
```

---

### 函数参数

```cpp
static int multiply(int x, int y) {
    return x * y;
}
```

这里：

```cpp
x
y
```

是函数参数，但它们同样可以直接参与算术运算。

所以从运算符的角度：

```cpp
12 * 4
```

```cpp
a * b
```

```cpp
groups * perGroup
```

本质上没有区别。

它们都是：

```text
左操作数  运算符  右操作数
```

---

# 4. 两个 `int` 运算，通常仍然得到 `int`

如果算术运算的两个操作数都是：

```cpp
int
```

那么结果通常也是：

```cpp
int
```

例如：

```cpp
int a = 12;
int b = 5;

int result = a + b;
```

结果是：

```text
17
```

类型也是：

```cpp
int
```

乘法和减法同样如此：

```cpp
int x = 3 * 4;   // int
int y = 8 - 10;  // int
```

---

# 5. 特别注意：整数除法

`/` 有一个非常重要的规则：

> 如果 `/` 两边都是 `int`，执行的是整数除法。

例如：

```cpp
int result = 5 / 2;
```

你可能认为结果是：

```text
2.5
```

但实际上：

```text
2
```

因为：

```cpp
5
2
```

都是 `int`。

所以：

```cpp
5 / 2
```

的结果也是 `int`。

小数部分会被丢掉。

---

## 示例

```cpp
int a = 7 / 3;
```

结果：

```text
2
```

而不是：

```text
2.333...
```

如果需要小数除法，则至少让一边是浮点数，例如：

```cpp
double result = 7.0 / 3;
```

这样结果才会接近：

```text
2.333333...
```

---

# 6. 加法和乘法可以交换顺序

对于普通数字：

```cpp
a + b
```

和：

```cpp
b + a
```

结果相同。

例如：

```cpp
12 + 4 == 4 + 12
```

都是：

```text
16
```

因此加法具有交换性。

同样：

```cpp
a * b
```

和：

```cpp
b * a
```

结果也相同。

例如：

```cpp
12 * 4 == 4 * 12
```

都是：

```text
48
```

---

# 7. 减法和除法不能随便交换顺序

减法与除法不同。

它们非常在意：

> 哪个值在左边，哪个值在右边。

例如：

```cpp
int a = 12;
int b = 4;
```

那么：

```cpp
a - b
```

等于：

```text
8
```

但是：

```cpp
b - a
```

等于：

```text
-8
```

因此：

```cpp
a - b != b - a
```

---

## 理解减法方向

可以把：

```cpp
a - b
```

理解为：

> 从 `a` 中减去 `b`

也就是：

```text
left - right
```

左边是“原来的数量”，右边是“要拿走的数量”。

---

# 8. 除法同样具有方向

例如：

```cpp
12 / 4
```

结果：

```text
3
```

但是：

```cpp
4 / 12
```

由于是整数除法，结果是：

```text
0
```

所以：

```cpp
a / b
```

表示：

> 用左边的值除以右边的值。

顺序不能随意交换。

---

# 9. C++ 的运算优先级

当一个表达式中同时出现多个运算符时，C++ 不一定严格从左到右计算。

基本优先级是：

```text
* /
先于
+ -
```

也就是说：

```cpp
a + b * 2
```

不是：

```cpp
(a + b) * 2
```

而是：

```cpp
a + (b * 2)
```

例如：

```cpp
int a = 12;
int b = 4;

int result = a + b * 2;
```

先算：

```cpp
b * 2
```

得到：

```text
8
```

再算：

```cpp
a + 8
```

得到：

```text
20
```

所以：

```cpp
result == 20
```

---

# 10. 使用括号改变计算顺序

如果希望：

```cpp
a + b
```

先算，就必须明确写出括号：

```cpp
(a + b) * 2
```

例如：

```cpp
int result = (12 + 4) * 2;
```

先算：

```text
12 + 4 = 16
```

再算：

```text
16 * 2 = 32
```

因此结果是：

```text
32
```

---

## 推荐习惯

即使你已经知道运算优先级，也可以通过括号提高可读性。

例如：

```cpp
groups * perGroup + extra - missing
```

本身没有问题。

但对于更复杂的表达式，可以写成：

```cpp
(groups * perGroup) + extra - missing
```

一眼就可以看出计算逻辑。

---

# 11. 多个 `+` 和 `-` 时通常从左向右

如果运算符优先级相同，例如：

```cpp
a + b - c
```

通常按从左到右理解：

```cpp
(a + b) - c
```

例如：

```cpp
10 + 5 - 3
```

等于：

```text
15 - 3
= 12
```

而不是：

```cpp
10 + (5 - 3)
```

虽然这个例子碰巧结果相同，但在纯减法中区别非常明显：

```cpp
10 - 5 - 3
```

表示：

```cpp
(10 - 5) - 3
```

结果：

```text
2
```

而不是：

```cpp
10 - (5 - 3)
```

后者结果是：

```text
8
```

所以在复杂表达式中，如果担心歧义，直接加括号是最安全的。

---

# 12. 函数中的算术表达式

假设有这样一个函数：

```cpp
static int totalOf(
    int groups,
    int perGroup,
    int extra,
    int missing
)
```

参数含义可以理解为：

```text
groups     = 有多少组
perGroup   = 每组有多少个
extra      = 额外增加多少
missing    = 缺少多少
```

计算逻辑是：

```text
总数
=
组数 × 每组数量
+ 额外数量
- 缺失数量
```

对应数学表达式：

```text
groups × perGroup + extra - missing
```

对应 C++：

```cpp
groups * perGroup + extra - missing
```

---

# 13. 为什么 `missing` 必须写在减号右边？

正确逻辑是：

```cpp
total - missing
```

也就是：

> 从已有总数中扣掉缺失数量。

因此：

```cpp
groups * perGroup + extra - missing
```

是正确的。

不能写成：

```cpp
missing - (groups * perGroup + extra)
```

因为这变成了：

> 用 `missing` 去减总数量。

符号会反过来。

---

## 示例

假设：

```cpp
groups = 3
perGroup = 4
extra = 2
missing = 5
```

那么：

```text
3 × 4 = 12
12 + 2 = 14
14 - 5 = 9
```

所以结果：

```text
9
```

如果顺序反过来：

```text
5 - 14 = -9
```

结果完全不同。

这也是为什么减法的左右顺序非常重要。

---

# 14. `extra = 0` 不需要特殊处理

例如：

```cpp
extra = 0
```

表达式：

```cpp
groups * perGroup + extra
```

自然就变成：

```cpp
groups * perGroup + 0
```

而：

```text
x + 0 = x
```

因此不需要写：

```cpp
if (extra == 0)
```

这种额外判断。

算术表达式本身已经能够正确处理。

---

# 15. 最终结果可以是负数

不要默认“总数一定是正数”。

例如：

```cpp
groups = 2;
perGroup = 3;
extra = 1;
missing = 10;
```

先计算：

```text
2 × 3 = 6
```

再：

```text
6 + 1 = 7
```

再：

```text
7 - 10 = -3
```

所以正确结果就是：

```text
-3
```

这不是错误。

`int` 本身可以保存负数。

因此不要为了“避免负数”额外写：

```cpp
if (result < 0) {
    return 0;
}
```

除非题目明确要求这么做。

---

# 16. `return` 和 `std::cout` 是完全不同的事情

如果题目要求：

```cpp
static int totalOf(...)
```

那么测试代码通常会类似：

```cpp
int answer = totalOf(...);
```

然后检查：

```cpp
answer
```

是不是正确。

因此：

```cpp
std::cout << result;
```

只是把结果打印到屏幕。

它不会把这个数字作为函数结果返回。

---

## 错误示例

```cpp
static int totalOf(
    int groups,
    int perGroup,
    int extra,
    int missing
) {
    int result = groups * perGroup + extra - missing;

    std::cout << result;
}
```

这里虽然屏幕上可能打印出了正确数字，但函数并没有：

```cpp
return result;
```

所以并没有正确完成“返回一个 `int`”这个任务。

---

## 正确形式

```cpp
return result;
```

或者更简单：

```cpp
return groups * perGroup + extra - missing;
```

---

# 17. 一个表达式本身并不等于“保存结果”

要区分下面三件事情。

### 只计算

```cpp
a + b;
```

结果计算出来以后直接丢掉。

---

### 计算并保存

```cpp
int result = a + b;
```

---

### 计算并返回

```cpp
return a + b;
```

可以记成：

```text
expression
    ↓
产生一个值
```

接下来你必须决定：

```text
保存？
赋值？
返回？
还是直接丢弃？
```

---

# 18. 本题涉及的核心知识

这一部分最值得以后复习：

```text
+   加法
-   减法
*   乘法
/   除法
```

其中：

```text
* 和 / 的优先级
高于
+ 和 -
```

因此：

```cpp
a * b + c - d
```

可以理解为：

```cpp
(a * b) + c - d
```

另外：

```text
+ 和 * 通常可以交换左右顺序
- 和 / 不可以随意交换左右顺序
```

函数题中还需要记住：

```text
打印值 != 返回值
```

如果题目要求函数返回：

```cpp
int
```

通常最终应该看到：

```cpp
return ...;
```

---

# 19. 一句话总结

看到：

```cpp
groups * perGroup + extra - missing
```

可以从左到右理解成：

```text
先计算每组数量的总和
↓
加入额外数量
↓
扣掉缺失数量
↓
把最终结果 return
```

也就是：

```text
总数 = groups × perGroup + extra - missing
```
