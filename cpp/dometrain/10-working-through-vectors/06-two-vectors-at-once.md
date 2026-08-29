# C++：同时遍历两个 `vector`

## 1. 为什么需要“同一个索引”遍历两个 vector？

当两个 `vector` 中相同位置的元素彼此对应时，可以使用同一个索引 `i` 同时访问它们。

例如：

```cpp
std::vector<int> a = {1, 2, 3};
std::vector<int> b = {10, 20, 30};

std::vector<int> result;

for (std::size_t i = 0; i < a.size(); ++i) {
    result.push_back(a[i] + b[i]);
}
```

最终：

```cpp
result == {11, 22, 33}
```

这里的关键关系是：

```text
a[0] <-> b[0]
a[1] <-> b[1]
a[2] <-> b[2]
```

也就是说，**位置 `i` 在两个 vector 中具有相同含义**。

---

## 2. 为什么只需要一个索引？

不要分别维护两个计数器：

```cpp
std::size_t i = 0;
std::size_t j = 0;
```

因为只要其中一个计数器没有同步变化，两个 vector 的对应关系就会错位。

如果两个容器本来就是“按位置对应”，最自然的方式就是：

```cpp
a[i]
b[i]
```

共用同一个 `i`。

---

## 3. 为什么这里通常不用 range-based for？

普通范围 `for`：

```cpp
for (int value : a) {
    // ...
}
```

只直接提供元素 `value`，没有索引。

但这里需要通过同一个位置访问：

```cpp
a[i]
b[i]
```

所以索引循环更加直接：

```cpp
for (std::size_t i = 0; i < a.size(); ++i)
```

---

## 4. 最重要的问题：两个 vector 的长度必须安全

下面的循环：

```cpp
for (std::size_t i = 0; i < a.size(); ++i) {
    result.push_back(a[i] + b[i]);
}
```

只证明了：

```cpp
i < a.size()
```

它并没有证明：

```cpp
i < b.size()
```

例如：

```cpp
a = {1, 2, 3};
b = {10};
```

当 `i == 1` 时：

```cpp
a[1]   // 合法
b[1]   // 越界
```

因此，如果算法要求两个 vector 一一对应，应该**在循环开始之前检查长度**：

```cpp
if (a.size() != b.size()) {
    return {};
}
```

然后再遍历：

```cpp
for (std::size_t i = 0; i < a.size(); ++i) {
    result.push_back(a[i] + b[i]);
}
```

---

## 5. 为什么必须在循环之前检查？

错误做法：

```cpp
for (...) {
    // 已经访问两个 vector
}

if (a.size() != b.size()) {
    return {};
}
```

此时已经太晚了。

如果长度不一致，循环过程中可能早就发生了越界访问。

正确顺序应该是：

```cpp
检查前置条件
↓
确认安全
↓
执行循环
```

即：

```cpp
if (a.size() != b.size()) {
    return {};
}

for (...) {
    ...
}
```

---

## 6. 不要依赖 vector 越界时“报错”

使用：

```cpp
v[i]
```

进行越界访问属于未定义行为。

程序可能：

* 崩溃；
* 返回奇怪的数据；
* 看起来正常运行；
* 在另一台机器上表现不同。

因此不能依赖：

> “如果越界，程序自然会告诉我。”

正确做法是提前保证索引合法。

---

# 把“长度是否相等”拆成独立函数

如果题目要求实现：

```cpp
static bool sizesMatch(
    const std::vector<int> &left,
    const std::vector<int> &right
);
```

那么逻辑非常简单：

```cpp
return left.size() == right.size();
```

例如：

```cpp
sizesMatch({1, 2}, {3, 4})      // true
sizesMatch({1}, {2, 3})         // false
sizesMatch({}, {})              // true
```

注意：

```cpp
{} 和 {}
```

长度都是 `0`，所以它们的长度是匹配的。

---

# “结果为空”不等于“输入长度不匹配”

这是这道题比较重要的设计点。

考虑：

```cpp
left  = {}
right = {}
```

两个 vector 长度相同，所以操作是合法的。

结果自然也是：

```cpp
{}
```

再考虑：

```cpp
left  = {1}
right = {2, 3}
```

长度不同，函数也要求返回：

```cpp
{}
```

因此单独看返回结果：

```cpp
{}
```

无法判断究竟是：

1. 两个输入本来都是空的；
2. 还是长度不匹配。

所以题目另外提供：

```cpp
sizesMatch(...)
```

让调用者能够明确判断输入是否匹配。

---

# 推荐实现结构

```cpp
static bool sizesMatch(
    const std::vector<int> &left,
    const std::vector<int> &right
) {
    return left.size() == right.size();
}

static std::vector<int> sumAtEachPosition(
    const std::vector<int> &left,
    const std::vector<int> &right
) {
    std::vector<int> result;

    if (!sizesMatch(left, right)) {
        return result;
    }

    for (std::size_t i = 0; i < left.size(); ++i) {
        result.push_back(left[i] + right[i]);
    }

    return result;
}
```

核心思路：

```text
先检查长度
    ↓
长度不同 → 返回空 vector
    ↓
长度相同 → 使用同一个索引遍历
    ↓
result[i] = left[i] + right[i]
```

---

# 复习重点

同时遍历两个按位置对应的 `vector` 时：

```cpp
for (std::size_t i = 0; i < left.size(); ++i) {
    // left[i]
    // right[i]
}
```

但必须先保证：

```cpp
left.size() == right.size()
```

否则：

```cpp
i < left.size()
```

并不能保证：

```cpp
i < right.size()
```

这类题的核心不是“怎么相加”，而是：

> **在使用同一个索引访问多个容器之前，必须先证明这个索引对所有容器都合法。**
