# C++：计算 `vector<int>` 平均值时的两个关键问题

计算一个 `std::vector<int>` 的平均值，看起来只是：

```cpp
平均值 = 总和 / 元素个数
```

但在 C++ 中，需要特别处理两个问题：

1. **整数除法与类型转换**
2. **空 vector 无法计算平均值**

---

## 1. 整数除法会丢失小数

例如：

```cpp
int total = 9;
std::vector<int> v = {1, 2, 3, 3};

double result = total / v.size();
```

数学上应该得到：

```text
9 / 4 = 2.25
```

但这里的除法并不是浮点数除法。

`total` 是：

```cpp
int
```

而：

```cpp
v.size()
```

返回的是：

```cpp
std::size_t
```

这是一个**无符号整数类型（unsigned integer type）**。

因此：

```cpp
total / v.size()
```

本质上仍然是**整数之间的除法**，小数部分会直接丢失。

例如：

```cpp
double truncated = total / v.size();
```

结果是：

```cpp
2.0
```

而不是：

```cpp
2.25
```

---

## 2. 正确做法：在除法之前转换成 `double`

应该把其中一个操作数转换成 `double`：

```cpp
double average =
    static_cast<double>(total) / v.size();
```

此时表达式变成：

```text
double / integer
```

C++ 会把整个除法提升为浮点数除法，因此：

```cpp
9 / 4
```

得到：

```cpp
2.25
```

### 对比

错误：

```cpp
double result = total / v.size();
```

正确：

```cpp
double result =
    static_cast<double>(total) / v.size();
```

---

## 3. `static_cast<double>` 必须发生在除法之前

下面这种写法仍然是错误的：

```cpp
double result =
    static_cast<double>(total / v.size());
```

执行顺序是：

```cpp
total / v.size()
```

先执行整数除法：

```text
9 / 4 → 2
```

然后才转换：

```text
2 → 2.0
```

因此不会重新得到已经丢失的小数。

可以把它理解为：

```cpp
static_cast<double>(2)
```

而不是：

```cpp
9.0 / 4
```

所以应该写：

```cpp
static_cast<double>(total) / v.size();
```

而不是：

```cpp
static_cast<double>(total / v.size());
```

---

# 4. `int` 和 `size_t` 混合时还要特别注意负数

这里还有一个比「丢失小数」更危险的问题。

假设：

```cpp
int total = -7;
```

而：

```cpp
v.size()
```

是 `std::size_t`，也就是无符号类型。

当有符号整数 `int` 和无符号整数参与某些运算时，负数可能被转换成一个非常大的无符号整数。

例如概念上：

```text
-7
```

可能变成：

```text
4294967289
```

之类的巨大正数（具体大小取决于平台和类型宽度）。

于是：

```cpp
total / v.size()
```

可能得到一个巨大的正数，而不是期待的：

```text
-3.5
```

因此这里的：

```cpp
static_cast<double>(total)
```

不仅是为了**保留小数**，也避免了 `int` 与 unsigned `size_t` 直接参与整数运算的问题。

---

# 5. 第二个问题：空 vector 没有平均值

假设：

```cpp
std::vector<int> numbers;
```

它的：

```cpp
numbers.size()
```

是：

```cpp
0
```

如果执行：

```cpp
total / numbers.size()
```

就会发生除以 0。

更重要的是，从数学意义上讲：

> 空集合没有平均值。

所以必须在计算之前检查：

```cpp
if (numbers.empty()) {
    ...
}
```

---

# 6. 为什么不能用 `0` 表示「没有平均值」

例如：

```cpp
std::vector<int> numbers = {-2, 2};
```

真正的平均值就是：

```text
0
```

因此如果规定：

```text
0 = 没有平均值
```

就无法区分：

```text
真正平均值是 0
```

和：

```text
vector 是空的
```

所以这里使用一个由调用者提供的备用值：

```cpp
double absent
```

函数签名：

```cpp
static double averageOfOr(
    const std::vector<int> &numbers,
    double absent
)
```

含义是：

> 如果存在平均值，就返回平均值；如果 vector 为空，就返回调用者指定的 `absent`。

例如：

```cpp
averageOfOr({}, 99.0)
```

返回：

```text
99.0
```

而：

```cpp
averageOfOr({-2, 2}, 99.0)
```

返回：

```text
0.0
```

---

# 7. 为什么 `.empty()` 应该最先检查

推荐结构：

```cpp
if (numbers.empty()) {
    return absent;
}
```

然后才：

```cpp
计算 total
计算 average
```

这样可以直接保证：

```cpp
numbers.size() > 0
```

因此后面的除法一定不会除以 0。

---

# 8. 自己使用循环计算总和

题目明确要求不能使用：

```cpp
std::accumulate
```

所以需要自己建立累加器：

```cpp
int total = 0;

for (int number : numbers) {
    total += number;
}
```

这里的 `total` 是一个典型的 **accumulator（累加器）**。

循环不需要索引，因为计算总和只关心：

```text
每个元素的值
```

而不关心：

```text
元素的位置
```

因此 range-based `for` 更合适：

```cpp
for (int number : numbers)
```

---

# 9. 完整思路

函数可以按照三个步骤理解：

```text
1. 如果 vector 为空：
       返回 absent

2. 遍历 vector：
       把所有元素加到 total

3. 返回：
       double(total) / 元素个数
```

核心结构：

```cpp
if (numbers.empty()) {
    return absent;
}

int total = 0;

for (int number : numbers) {
    total += number;
}

return static_cast<double>(total) / numbers.size();
```

---

# 10. 示例

### 普通情况

```cpp
{1, 2, 3, 4}
```

总和：

```text
10
```

元素个数：

```text
4
```

结果：

```text
10.0 / 4 = 2.5
```

---

### 负数

```cpp
{-5, -2}
```

总和：

```text
-7
```

结果：

```text
-7.0 / 2 = -3.5
```

---

### 单个元素

```cpp
{8}
```

结果：

```text
8.0 / 1 = 8.0
```

---

### 平均值真的为 0

```cpp
{-2, 2}
```

即使：

```cpp
absent = 99
```

结果仍然应该是：

```text
0.0
```

因为 vector 并不为空。

---

### 空 vector

```cpp
{}
```

例如：

```cpp
absent = 99.5
```

结果：

```text
99.5
```

并且不会执行除法。

---

# 核心记忆

计算整数 vector 的平均值时：

```cpp
if (numbers.empty()) {
    return absent;
}
```

解决：

```text
空 vector / 除以 0
```

而：

```cpp
static_cast<double>(total) / numbers.size()
```

解决：

```text
整数除法丢失小数
```

以及避免：

```text
负 int 与 unsigned size_t 混合运算
```

最值得记住的是：

```cpp
static_cast<double>(total) / numbers.size()
```

转换必须发生在**除法之前**。
