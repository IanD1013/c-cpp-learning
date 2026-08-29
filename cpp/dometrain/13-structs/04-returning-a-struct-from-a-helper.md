# C++：让函数返回 `struct`，一次遍历统计多个结果

## 1. 为什么要让函数返回一个 `struct`

有时一次计算会得到多个彼此相关的结果。

例如遍历一个整数数组时，我们同时想知道：

* 偶数有多少个
* 奇数有多少个

如果函数只能返回一个 `int`，就无法同时返回这两个结果。

这时可以定义一个结构体：

```cpp
struct Split {
    int evens;
    int odds;
};
```

然后让函数直接返回这个结构体：

```cpp
static Split countParities(const std::vector<int> &values) {
    Split split{0, 0};

    for (int value : values) {
        if (value % 2 == 0) {
            split.evens = split.evens + 1;
        } else {
            split.odds = split.odds + 1;
        }
    }

    return split;
}
```

调用：

```cpp
Split split = countParities(numbers);

int evenCount = split.evens;
int oddCount = split.odds;
```

核心思想：

> `struct` 和 `int`、`std::string` 等类型一样，也可以作为函数的返回类型。

---

# 2. 返回的是一个独立的值

例如：

```cpp
Split first = countParities(a);
Split second = countParities(b);
```

`first` 和 `second` 是两个独立的结构体对象。

第二次调用：

```cpp
countParities(b)
```

不会修改第一次调用得到的 `first`。

可以把它理解成：

```cpp
return split;
```

把函数计算得到的结果交给调用者，调用者拥有自己的结果对象。

---

# 3. 为什么辅助函数可以返回 `struct`，评分函数却不能

这道题的运行环境对评分函数的返回类型有限制。

评分器可以比较：

* `int`
* `bool`
* `char`
* `std::string`
* 上述基本类型组成的 `std::vector`

但不能比较自己定义的类型，例如：

```cpp
struct Summary {
    int smallest;
    int largest;
    int count;
};
```

因此下面这种评分入口虽然 C++ 本身允许：

```cpp
static Summary readSummary(...)
```

但评分器无法比较 `Summary`。

正确设计是：

```text
values
  ↓
summarize()
  ↓
Summary
  ↓
readSummaryField()
  ↓
int
```

也就是说：

* `Summary` 只在程序内部使用
* 最终评分函数只返回其中一个 `int`

这是一种很常见的设计方式：

> 内部使用更丰富的数据结构，外部接口只暴露需要的结果。

---

# 4. `Summary` 应该保存什么

本题需要一次遍历同时计算：

```cpp
struct Summary {
    int smallest;
    int largest;
    int count;
};
```

分别代表：

* `smallest`：最小值
* `largest`：最大值
* `count`：元素数量

辅助函数：

```cpp
static Summary summarize(const std::vector<int> &values)
```

负责计算这三个值。

---

# 5. 最小值和最大值为什么不能初始化为 `0`

一个常见错误是：

```cpp
int smallest = 0;
int largest = 0;
```

例如：

```cpp
values = {3, 9, 4, 7};
```

真实最小值是：

```text
3
```

但如果：

```cpp
smallest = 0;
```

随后判断：

```cpp
if (value < smallest)
```

所有元素都大于 0，因此 `smallest` 永远不会改变。

最终错误地得到：

```text
smallest = 0
```

可是 `0` 根本不在原数组中。

---

# 6. 正确方法：用第一个元素初始化

如果数组非空：

```cpp
int smallest = values[0];
int largest = values[0];
```

例如：

```cpp
{3, 9, 4, 7}
```

开始：

```text
smallest = 3
largest = 3
```

随后逐个比较：

```cpp
if (value < smallest) {
    smallest = value;
}

if (value > largest) {
    largest = value;
}
```

这种方法对于以下数据都正确：

```cpp
{3, 9, 4}
{-10, -3, -7}
{0}
```

特别是负数。

例如：

```cpp
{-8, -4, -1}
```

最大值应该是：

```text
-1
```

所以绝不能默认认为：

```text
0 是合理的初始最大值
```

---

# 7. 但读取 `values[0]` 前必须先判断空数组

下面代码有风险：

```cpp
int smallest = values[0];
```

如果：

```cpp
values.empty()
```

那么：

```cpp
values[0]
```

访问了不存在的元素。

因此必须先写：

```cpp
if (values.empty()) {
    ...
}
```

再去访问：

```cpp
values[0]
```

这是一个很重要的通用原则：

> 在通过索引读取容器元素之前，要确认该元素确实存在。

---

# 8. `count` 可以直接使用 `size()`

元素数量不需要自己循环统计：

```cpp
values.size()
```

例如：

```cpp
std::vector<int> values{4, 8, 10};

values.size();   // 3
```

空数组：

```cpp
std::vector<int> values;

values.size();   // 0
```

由于 `size()` 返回的是无符号整数类型，而这里结构体成员是：

```cpp
int count;
```

在这类练习中通常可以写：

```cpp
int count = static_cast<int>(values.size());
```

---

# 9. 一次遍历同时求最小值和最大值

本题强调：

```text
all three filled in one pass
```

所以不应该分别遍历：

```cpp
// 第一遍找最小值
for (...) {
}

// 第二遍找最大值
for (...) {
}
```

而应该一次完成：

```cpp
for (int value : values) {
    if (value < smallest) {
        smallest = value;
    }

    if (value > largest) {
        largest = value;
    }
}
```

这样一次循环同时完成两个统计。

时间复杂度：

```text
O(n)
```

额外空间：

```text
O(1)
```

---

# 10. 为什么需要 `absent`

空数组：

```cpp
{}
```

有明确的：

```text
count = 0
```

但是它没有：

```text
smallest
largest
```

不能随便规定：

```text
smallest = 0
largest = 0
```

因为 `0` 可能是真实数据。

也不能使用：

```text
-1
```

因为 `-1` 同样可能是真实数据。

例如：

```cpp
{-5, -3, -1}
```

这里：

```text
largest = -1
```

所以没有任何普通 `int` 可以天然表示：

```text
不存在
```

因此本题由调用者提供：

```cpp
int absent
```

例如调用者可能传：

```cpp
-999
```

当结果不存在时，就返回：

```cpp
absent
```

这种设计叫做：

> sentinel value（哨兵值）

不过这里哨兵值由调用者决定，而不是函数自己硬编码。

---

# 11. `field` 的作用

最终函数签名：

```cpp
static int readSummaryField(
    const std::vector<int> &values,
    const std::string &field,
    int absent
)
```

`field` 决定要读取哪个结果。

只有三个合法名字：

```text
"count"
"smallest"
"largest"
```

注意：

* 大小写敏感
* 必须完全匹配

例如：

```cpp
"count"      // 正确
"Count"      // 无效
"COUNT"      // 无效
```

未知字段返回：

```cpp
absent
```

---

# 12. 推荐的程序结构

整体设计：

```cpp
struct Summary {
    int smallest;
    int largest;
    int count;
};

static Summary summarize(const std::vector<int> &values) {
    ...
}

static int readSummaryField(
    const std::vector<int> &values,
    const std::string &field,
    int absent
) {
    Summary summary = summarize(values);

    ...
}
```

职责分工：

### `summarize`

负责：

```text
数据 → Summary
```

### `readSummaryField`

负责：

```text
Summary + field → 一个 int
```

这样比把所有逻辑塞进一个函数更清晰。

---

# 13. 本题最重要的知识点

### ① `struct` 可以作为函数返回值

```cpp
static Summary summarize(...)
```

---

### ② 多个相关结果可以放进一个结构体

```cpp
struct Summary {
    int smallest;
    int largest;
    int count;
};
```

---

### ③ 最小值和最大值最好从真实数据初始化

```cpp
int smallest = values[0];
int largest = values[0];
```

不要随意写：

```cpp
0
-1
999999
```

---

### ④ 读取 `values[0]` 前先检查空数组

```cpp
if (values.empty()) {
    ...
}
```

---

### ⑤ 空数组有 `count = 0`，但没有最小值和最大值

```text
count → 0
smallest → absent
largest → absent
```

---

### ⑥ 不要把合法整数当成“没有答案”

因为元素可能是任何整数，包括：

```text
0
-1
负数
```

因此题目专门提供了：

```cpp
absent
```

---

### ⑦ 辅助函数可以使用复杂类型，最终接口可以返回简单类型

这是一个非常实用的程序设计思想：

```text
内部数据结构丰富
        ↓
对外接口简单
```
