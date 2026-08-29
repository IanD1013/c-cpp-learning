# C++：安全读取 `vector` 指定下标的几种返回方式

## 1. 问题本质

假设我们想读取：

```cpp
values[index]
```

那么至少有三种失败情况：

1. `index < 0`
2. `index >= values.size()`
3. `values` 是空的

第三种其实已经被前两种检查覆盖：空 `vector` 的 `size()` 是 0，因此任何合法的非负 `index` 都满足：

```cpp
index >= 0
```

从而被拒绝。

所以核心问题其实是：

> 当指定下标不存在时，函数应该怎样告诉调用者“没有值”？

---

# 2. 四种常见失败处理方式

## 方法一：Sentinel（哨兵值）

例如：

```cpp
int valueAt(...) {
    if (index 无效) {
        return -1;
    }

    return values[index];
}
```

这里用 `-1` 表示“失败”。

问题是：

```cpp
std::vector<int> values = {8, 3, -1};
```

如果读取下标 2：

```cpp
valueAt(values, 2)
```

真实结果本来就是：

```cpp
-1
```

调用者无法区分：

```text
-1 = vector 中真的存了 -1
```

还是：

```text
-1 = 读取失败
```

因此：

> 当所有 `int` 都可能是合法数据时，不能安全地用某个 `int` 作为 sentinel。

---

# 3. `std::optional<int>`

更现代、更清晰的方法是：

```cpp
std::optional<int>
```

它表示：

```text
有一个 int
```

或者：

```text
没有值
```

例如：

```cpp
#include <optional>
#include <vector>

static std::optional<int> valueAt(
    const std::vector<int> &values,
    int index
) {
    const int count = static_cast<int>(values.size());

    if (index < 0 || index >= count) {
        return std::nullopt;
    }

    return values[index];
}
```

成功：

```cpp
auto result = valueAt({4, 9, 25}, 1);
```

相当于：

```text
result = 9
```

失败：

```cpp
auto result = valueAt({4, 9}, 2);
```

相当于：

```text
result = nothing
```

检查：

```cpp
result.has_value()
```

或者：

```cpp
if (result.has_value()) {
    int value = *result;
}
```

---

# 4. 为什么 `optional` 比 sentinel 好

假设：

```cpp
std::vector<int> values = {8, 3, -1};
```

读取下标 2：

```cpp
std::optional<int> result = valueAt(values, 2);
```

结果是：

```text
optional 中装着 -1
```

而不是“失败”。

只有真正不存在时才：

```cpp
std::nullopt
```

因此：

```text
stored -1
```

和：

```text
no value
```

是完全不同的状态。

---

# 5. `bool + 引用 out-parameter`

另一种经典 C++ 写法是：

```cpp
static bool tryValueAt(
    const std::vector<int> &values,
    int index,
    int &found
)
```

其中：

* `bool` 表示成功还是失败
* `found` 用来接收真正的结果

例如：

```cpp
static bool tryValueAt(
    const std::vector<int> &values,
    int index,
    int &found
) {
    const int count = static_cast<int>(values.size());

    if (index < 0 || index >= count) {
        return false;
    }

    found = values[index];
    return true;
}
```

调用：

```cpp
int found = 4242;

bool success = tryValueAt(values, index, found);
```

成功时：

```text
success = true
found = 对应元素
```

失败时：

```text
success = false
found 保持原值不变
```

这里有一个非常重要的设计原则：

> 只有成功时才修改 out-parameter。

因此不要这样写：

```cpp
found = 0;

if (index 无效) {
    return false;
}
```

否则函数失败时也改变了调用者的数据。

---

# 6. 为什么题目强调 `found` 初始化为 4242

例如：

```cpp
int found = 4242;
```

之后调用：

```cpp
tryValueAt(values, index, found);
```

如果读取失败，正确实现必须保证：

```cpp
found == 4242
```

这实际上是在测试：

> `tryValueAt()` 失败时是否真的没有修改 out-parameter。

---

# 7. `askBothAt`

这个函数同时测试：

1. `std::optional<int>` 是否成功
2. `tryValueAt()` 是否成功
3. `tryValueAt()` 失败时有没有修改 `found`

返回：

```cpp
std::vector<int>
```

包含三个数字：

```text
{
    optional 是否有值,
    tryValueAt 是否成功,
    found 最终值
}
```

布尔值需要转成：

```cpp
1
```

或：

```cpp
0
```

例如：

```cpp
values = {4, 9, 25}
index = 1
```

结果：

```cpp
{1, 1, 9}
```

因为：

* optional 找到了值
* `tryValueAt` 成功
* 找到的是 9

而：

```cpp
values = {4, 9}
index = 2
```

结果：

```cpp
{0, 0, 4242}
```

因为：

* optional 没有值
* `tryValueAt` 返回 false
* `found` 没被修改

---

# 8. Fallback（默认值）

有些场景中，调用者不关心“失败”本身，只想：

> 如果没有值，就给我一个默认值。

于是可以写：

```cpp
static int valueAtOrFallback(
    const std::vector<int> &values,
    int index,
    int fallback
)
```

例如：

```cpp
valueAtOrFallback({4, 9}, 5, 100)
```

返回：

```cpp
100
```

但：

```cpp
valueAtOrFallback({8, 3, -1}, 2, 100)
```

必须返回：

```cpp
-1
```

因为下标 2 是合法的，而里面真正存的就是 `-1`。

所以判断依据应该是：

```text
index 是否合法
```

而不是：

```text
读取出来的 value 是否等于某个特殊值
```

---

# 9. 推荐的下标检查方式

题目明确要求两个 helper 都这样做：

```cpp
const int count = static_cast<int>(values.size());

if (index < 0 || index >= count) {
    // invalid
}
```

原因是：

```cpp
values.size()
```

返回的是：

```cpp
std::size_t
```

这是无符号整数。

而：

```cpp
index
```

是：

```cpp
int
```

直接比较：

```cpp
index >= values.size()
```

会发生 signed / unsigned 混合比较。

尤其当：

```cpp
index == -1
```

时可能产生令人困惑的隐式转换。

因此本题统一转换成：

```cpp
int
```

之后再比较：

```cpp
index < 0 || index >= count
```

更容易理解。

---

# 10. 安全原则

一定要先判断：

```cpp
if (index < 0 || index >= count)
```

之后才能：

```cpp
values[index]
```

不能反过来：

```cpp
int value = values[index];

if (index < 0 || index >= count) {
    ...
}
```

因为非法下标已经被访问了。

正确结构永远是：

```cpp
if (invalid) {
    return ...;
}

return values[index];
```

---

# 11. 四种接口的对比

| 方式                   | 返回类型            | 是否占用合法 `int` | 调用者是否能知道失败 |
| -------------------- | --------------- | -----------: | ---------: |
| Sentinel             | `int`           |            是 |   可以，但可能歧义 |
| `bool + int&`        | `bool`          |            否 |         可以 |
| `std::optional<int>` | `optional<int>` |            否 |         可以 |
| Fallback             | `int`           |            否 |    通常不关心失败 |

如果是在现代 C++ 中设计“可能没有结果”的 API，一般优先考虑：

```cpp
std::optional<T>
```

它直接把：

```text
可能不存在
```

写进了返回类型。
