# C++ `std::optional`：表示“可能没有结果”的返回值

## 1. 为什么需要 `std::optional`

有些函数**不一定能找到一个有效结果**。

例如：寻找 vector 中第一个负数。

```cpp
#include <optional>
#include <vector>

std::optional<int> firstNegative(const std::vector<int> &values) {
    for (const int &value : values) {
        if (value < 0) {
            return value;
        }
    }

    return std::nullopt;
}
```

这里函数的返回类型是：

```cpp
std::optional<int>
```

它表示函数有两种可能结果：

* 有结果：里面存着一个 `int`
* 没有结果：为空

例如：

```cpp
return -5;
```

表示 optional 中存储 `-5`。

而：

```cpp
return std::nullopt;
```

表示：

> 没有找到任何结果。

---

## 2. 为什么不用特殊数字表示“没找到”

以前可能会这样写：

```cpp
int findSomething(...) {
    ...
    return -1;
}
```

问题在于：

```cpp
-1
```

本身可能就是合法结果。

`std::optional` 可以明确区分：

```text
optional<int>{0}   → 找到了，结果是 0
optional<int>{-1}  → 找到了，结果是 -1
nullopt            → 没找到
```

因此它不会占用某个特殊整数作为“失败标记”。

---

## 3. 使用 `std::optional` 需要头文件

```cpp
#include <optional>
```

否则无法使用：

```cpp
std::optional
std::nullopt
```

---

# 4. 调用 optional 函数

例如：

```cpp
std::optional<int> result = firstNegative(values);
```

此时 `result` 可能：

```text
有一个 int
```

也可能：

```text
为空
```

所以通常分成两个步骤。

### 第一步：检查有没有值

```cpp
if (result.has_value()) {
```

`has_value()` 返回 `bool`：

```cpp
true   // 有值
false  // 没有值
```

---

### 第二步：取出值

只有确认存在之后才：

```cpp
result.value()
```

完整写法：

```cpp
std::optional<int> result = firstNegative(values);

if (result.has_value()) {
    int answer = result.value();
}
```

核心模式：

```cpp
if (result.has_value()) {
    // 此时才能安全使用 result.value()
}
```

---

# 5. `0` 也是一个真正的值

非常容易犯的错误是把 `0` 当成“没有结果”。

实际上：

```cpp
std::optional<int> result = 0;
```

此时：

```cpp
result.has_value()
```

仍然是：

```cpp
true
```

因为：

```text
值为 0
```

和：

```text
没有值
```

完全是两个不同的状态。

例如：

```text
optional<int>{0}
```

表示：

> 找到了，答案正好是 0。

而：

```text
std::nullopt
```

才表示：

> 没找到。

---

# 6. 不要在 optional 为空时调用 `.value()`

错误：

```cpp
std::optional<int> result = firstNegative(values);

int answer = result.value();
```

如果 `result` 是空的，这样访问会失败。

正确：

```cpp
std::optional<int> result = firstNegative(values);

if (result.has_value()) {
    int answer = result.value();
}
```

记住：

```text
先 has_value()
再 value()
```

---

# 7. “找到第一个”意味着立即返回

例如要求：

> 找到第一个严格大于 threshold 的元素。

可以写：

```cpp
std::optional<int> firstAbove(
    const std::vector<int> &values,
    int threshold
) {
    for (const int &value : values) {
        if (value > threshold) {
            return value;
        }
    }

    return std::nullopt;
}
```

例如：

```cpp
values = {3, 9, 12, 7}
threshold = 5
```

遍历过程：

```text
3 > 5   → false
9 > 5   → true → 立即 return 9
```

所以答案是：

```text
9
```

而不是最大值：

```text
12
```

因为题目要求的是：

```text
first
```

而不是：

```text
maximum
```

---

# 8. “严格大于”要使用 `>`

如果要求：

> strictly greater than threshold

必须写：

```cpp
value > threshold
```

不能写：

```cpp
value >= threshold
```

例如：

```cpp
values = {6, 10}
threshold = 6
```

因为：

```text
6 > 6  → false
10 > 6 → true
```

所以答案是：

```text
10
```

---

# 9. optional + fallback 的常见模式

有时候内部函数使用：

```cpp
std::optional<int>
```

但最终函数必须返回普通：

```cpp
int
```

这时可以提供一个 fallback：

```cpp
std::optional<int> result = firstAbove(values, threshold);

if (result.has_value()) {
    return result.value();
}

return fallback;
```

逻辑就是：

```text
找到答案
    ↓
返回答案

没找到
    ↓
返回 fallback
```

这样 `optional` 只负责函数内部的“有没有答案”，最终接口仍然可以返回普通 `int`。

---

# 10. 核心记忆模板

### 返回 optional

```cpp
std::optional<int> findSomething(...) {
    for (...) {
        if (...) {
            return value;
        }
    }

    return std::nullopt;
}
```

### 使用 optional

```cpp
std::optional<int> result = findSomething(...);

if (result.has_value()) {
    return result.value();
}

return fallback;
```

最重要的三个知识点：

```text
std::optional<T>  → T 或者没有值
std::nullopt      → 没有值
has_value()       → 检查是否存在值
value()           → 取出里面的值
```
