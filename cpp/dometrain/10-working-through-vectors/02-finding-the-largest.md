# C++：在 `vector` 中手动寻找最大值

## 1. 为什么最大值不能默认从 `0` 开始？

一种看起来很自然的写法是：

```cpp
int best = 0;
```

然后遍历所有元素：

```cpp
for (int value : numbers) {
    if (value > best) {
        best = value;
    }
}
```

但这种方法有一个严重问题：**如果所有元素都是负数，就会得到错误答案。**

例如：

```cpp
std::vector<int> numbers = {-9, -4, -7};
```

如果：

```cpp
int best = 0;
```

那么：

* `-9 > 0` → false
* `-4 > 0` → false
* `-7 > 0` → false

最终：

```cpp
best == 0
```

但 `0` 根本不在 `vector` 中。

正确最大值应该是：

```text
-4
```

---

## 2. 正确思路：从第一个真实元素开始

寻找最大值时，最安全的初始值通常是：

```cpp
int best = numbers[0];
```

例如：

```cpp
std::vector<int> numbers = {6, 2, 11, 2};

int best = numbers[0];

for (int value : numbers) {
    if (value > best) {
        best = value;
    }
}
```

执行过程：

| 当前 `value` | `best` 之前 | 是否更新 | `best` 之后 |
| ---------: | --------: | ---- | --------: |
|          6 |         6 | 否    |         6 |
|          2 |         6 | 否    |         6 |
|         11 |         6 | 是    |        11 |
|          2 |        11 | 否    |        11 |

最终：

```cpp
best == 11
```

这里可以把 `best` 理解为：

> **到目前为止已经看到的最大值。**

这是一个非常常见的循环模式：

```cpp
初始化 best

for (每个元素) {
    if (当前元素比 best 更好) {
        更新 best
    }
}
```

---

## 3. 第一个元素为什么可以再次参与循环？

上面的代码中：

```cpp
int best = numbers[0];

for (int value : numbers) {
    ...
}
```

第一个元素实际上会被处理两次：

1. 用来初始化 `best`
2. 又作为循环中的第一个 `value`

这是完全没问题的。

例如：

```cpp
best = 6;
value = 6;
```

判断：

```cpp
if (6 > 6)
```

结果为 `false`。

所以不会产生任何错误。

与“求和”不同，寻找最大值不是累加操作。同一个值比较两次，并不会让答案变大。

---

## 4. 最大问题：空 `vector`

下面这句代码：

```cpp
int best = numbers[0];
```

有一个前提：

> `numbers` 至少有一个元素。

如果：

```cpp
std::vector<int> numbers;
```

那么：

```cpp
numbers[0]
```

访问的是一个不存在的元素，属于非法访问。

因此必须在读取：

```cpp
numbers[0]
```

**之前**检查：

```cpp
numbers.empty()
```

正确结构应该是：

```cpp
if (numbers.empty()) {
    // 处理没有元素的情况
}

int best = numbers[0];
```

而不是：

```cpp
int best = numbers[0];

if (numbers.empty()) {
    ...
}
```

因为此时已经太晚了。

---

## 5. 空 `vector` 没有最大值

数学上：

```text
{}
```

没有所谓“最大的整数”。

因此不能随便规定：

```cpp
return 0;
```

因为 `0` 完全可能是真实数据中的最大值。

同样：

```cpp
return -1;
return -1000;
```

也存在相同问题。

例如：

```cpp
{-5, -1, -8}
```

它真正的最大值就是 `-1`。

所以不存在一个特殊的 `int`，能够永远代表：

```text
没有最大值
```

---

## 6. Sentinel / fallback value：让调用者决定空值

这道题采用的方法是：

```cpp
largestValueOr(numbers, absent)
```

其中：

```cpp
absent
```

是调用者提供的备用值。

函数签名：

```cpp
static int largestValueOr(
    const std::vector<int> &numbers,
    int absent
)
```

如果 `vector` 为空：

```cpp
return absent;
```

如果不为空：

```cpp
return 真正的最大值;
```

例如：

```cpp
largestValueOr({}, -1)
```

返回：

```text
-1
```

而：

```cpp
largestValueOr({}, 999)
```

返回：

```text
999
```

关键是：

> 函数不能自己决定空数组返回什么，而是必须原样返回调用者传入的 `absent`。

---

## 7. `absent` 可能和真实数据冲突

例如：

```cpp
largestValueOr({-5, 0, -2}, 0)
```

真正的最大值就是：

```text
0
```

函数仍然必须返回：

```text
0
```

这时调用者无法仅凭返回值判断：

```text
vector 为空
```

还是：

```text
最大值真的等于 0
```

因此这种 `absent` 方案只适合调用者已经知道数据范围的情况。

例如调用者知道：

```text
所有合法数据 >= 0
```

那么就可以用：

```cpp
-1
```

作为不存在的标记。

---

## 8. 更通用的设计

如果所有 `int` 都可能是合法数据，那么单独返回 `int` 无法同时表达：

```text
有最大值
```

和：

```text
没有最大值
```

更合理的方式包括：

```cpp
bool + 输出参数
```

例如：

```cpp
bool largestValue(
    const std::vector<int> &numbers,
    int &result
);
```

或者现代 C++ 中使用：

```cpp
std::optional<int>
```

例如：

```cpp
std::optional<int> largestValue(
    const std::vector<int> &numbers
);
```

这样：

```cpp
std::nullopt
```

可以明确表示：

```text
不存在最大值
```

而不会与任何合法的 `int` 冲突。

不过本题明确要求返回：

```cpp
int
```

因此应按照 `absent` 参数的设计实现。

---

# 本题核心模式

寻找最大值时：

```cpp
if (numbers.empty()) {
    return absent;
}

int best = numbers[0];

for (int value : numbers) {
    if (value > best) {
        best = value;
    }
}

return best;
```

记住两个关键点：

1. **最大值的初始值应该来自真实数据，而不是随便写 `0`。**
2. **读取第一个元素之前，一定要先处理空容器。**
