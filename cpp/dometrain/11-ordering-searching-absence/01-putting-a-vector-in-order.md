# C++ `std::sort`：对 `vector` 排序

## 1. `std::sort` 的基本用法

C++ 中可以使用 `std::sort` 对 `vector` 中的元素进行排序：

```cpp
#include <algorithm>
#include <functional>
#include <vector>

std::vector<int> v = {4, 1, 9};

std::sort(v.begin(), v.end(), std::greater<int>());
```

执行后：

```cpp
v == {9, 4, 1}
```

这里使用：

```cpp
std::greater<int>()
```

表示按照 **从大到小** 排序。

需要包含：

```cpp
#include <algorithm>   // std::sort
#include <functional>  // std::less / std::greater
#include <vector>
```

---

# 2. `std::sort` 会直接修改原 vector

`std::sort` 属于 **原地排序（in-place sorting）**：

```cpp
std::vector<int> v = {4, 1, 9};

std::sort(v.begin(), v.end(), std::greater<int>());
```

调用结束后，原来的 `v` 已经被修改：

```cpp
{9, 4, 1}
```

因此不能这样写：

```cpp
std::vector<int> result =
    std::sort(v.begin(), v.end(), std::greater<int>());
```

因为：

```cpp
std::sort(...)
```

**没有返回一个新的 vector。**

它只是重新排列传入范围中的元素。

---

# 3. `begin()` 和 `end()` 表示排序范围

典型写法：

```cpp
std::sort(v.begin(), v.end(), ...);
```

这里：

```cpp
v.begin()
```

表示：

> 第一个元素的位置。

而：

```cpp
v.end()
```

表示：

> 最后一个元素之后的位置。

因此：

```cpp
[v.begin(), v.end())
```

是一个 **左闭右开区间（half-open range）**。

假设：

```cpp
std::vector<int> v = {4, 1, 9};
```

可以理解成：

```text
begin()
  ↓
[4] [1] [9] [end]
             ↑
             end()
```

注意：

```cpp
v.end()
```

**本身并不指向任何元素。**

---

# 4. 为什么必须使用 `end()`，而不是 `end() - 1`

正确：

```cpp
std::sort(v.begin(), v.end(), std::less<int>());
```

如果写：

```cpp
std::sort(v.begin(), v.end() - 1, std::less<int>());
```

那么最后一个元素不会参与排序。

例如：

```cpp
std::vector<int> v = {4, 9, 1};

std::sort(v.begin(), v.end() - 1, std::less<int>());
```

只会排序：

```cpp
{4, 9}
```

最后的：

```cpp
1
```

不会进入排序范围。

因此结果仍可能是：

```cpp
{4, 9, 1}
```

而不是：

```cpp
{1, 4, 9}
```

所以正常排序整个 vector 时始终使用：

```cpp
v.begin(), v.end()
```

---

# 5. 第三个参数：比较器 Comparator

`std::sort` 的第三个参数决定：

> 两个元素中，谁应该排在前面。

例如：

```cpp
std::less<int>()
```

表示：

> 如果第一个值比第二个值小，则第一个应该排前面。

所以：

```cpp
std::sort(
    v.begin(),
    v.end(),
    std::less<int>()
);
```

得到：

```text
小 → 大
```

例如：

```cpp
{4, -2, 9, 1}
```

变成：

```cpp
{-2, 1, 4, 9}
```

---

## `std::greater<int>()`

```cpp
std::sort(
    v.begin(),
    v.end(),
    std::greater<int>()
);
```

表示：

> 如果第一个值比第二个值大，则第一个应该排前面。

因此得到：

```text
大 → 小
```

例如：

```cpp
{4, -2, 9, 1}
```

变成：

```cpp
{9, 4, 1, -2}
```

---

# 6. `std::less<int>()` 和 `std::greater<int>()`

可以简单记忆：

| 比较器                   | 排序结果  |
| --------------------- | ----- |
| `std::less<int>()`    | 小 → 大 |
| `std::greater<int>()` | 大 → 小 |

例如：

```cpp
std::vector<int> v = {4, 1, 9};

std::sort(v.begin(), v.end(), std::less<int>());
```

结果：

```cpp
{1, 4, 9}
```

而：

```cpp
std::sort(v.begin(), v.end(), std::greater<int>());
```

结果：

```cpp
{9, 4, 1}
```

---

# 7. 为什么后面必须有 `()`

正确：

```cpp
std::greater<int>()
```

这里：

```cpp
std::greater<int>
```

是一个 **类型（type）**。

而：

```cpp
std::greater<int>()
```

表示：

> 创建一个 `std::greater<int>` 对象。

`std::sort` 需要的是一个可以执行比较的对象，所以应该传：

```cpp
std::greater<int>()
```

而不是：

```cpp
std::greater<int>
```

同理：

```cpp
std::less<int>()
```

也是创建比较器对象。

---

# 8. `const vector&` 为什么不能直接排序

假设函数参数：

```cpp
const std::vector<int> &values
```

这里的：

```cpp
const
```

表示：

> 函数不能修改 `values` 中的内容。

但 `std::sort` 会重新排列元素，也就是会修改 vector。

因此不能：

```cpp
std::sort(
    values.begin(),
    values.end(),
    std::less<int>()
);
```

因为 `values` 是 `const`。

---

# 9. 正确方法：先复制，再排序

如果要求：

* 不修改输入 vector
* 返回一个排好序的新 vector

可以先复制：

```cpp
std::vector<int> result = values;
```

这时：

```text
values
  ↓
原始 vector

result
  ↓
独立副本
```

然后：

```cpp
std::sort(
    result.begin(),
    result.end(),
    std::less<int>()
);
```

最后：

```cpp
return result;
```

这样：

```cpp
values
```

保持不变，而：

```cpp
result
```

被排序后返回。

---

# 10. 根据 `bool` 决定排序方向

假设参数：

```cpp
bool smallestFirst
```

约定：

```text
smallestFirst == true
    → 从小到大

smallestFirst == false
    → 从大到小
```

因此可以写：

```cpp
if (smallestFirst) {
    std::sort(
        result.begin(),
        result.end(),
        std::less<int>()
    );
} else {
    std::sort(
        result.begin(),
        result.end(),
        std::greater<int>()
    );
}
```

这里 `bool` 用来选择不同的比较器。

---

# 11. 空 vector 不需要特殊处理

例如：

```cpp
std::vector<int> v;
```

此时：

```cpp
v.begin() == v.end()
```

所以：

```cpp
std::sort(
    v.begin(),
    v.end(),
    std::less<int>()
);
```

排序范围中没有元素。

`std::sort` 什么都不会做，但这是完全合法的。

结果仍然是：

```cpp
{}
```

因此一般不需要额外写：

```cpp
if (v.empty()) {
    ...
}
```

---

# 12. 单元素 vector

例如：

```cpp
{5}
```

只有一个元素，本身已经有序。

无论：

```cpp
std::less<int>()
```

还是：

```cpp
std::greater<int>()
```

结果都是：

```cpp
{5}
```

同样不需要特殊处理。

---

# 13. 重复元素不会消失

例如：

```cpp
{4, 2, 4, 1}
```

从小到大排序：

```cpp
{1, 2, 4, 4}
```

两个 `4` 都会保留。

`std::sort` 只是重新排列元素，不会：

* 删除元素
* 添加元素
* 去重

因此：

```cpp
result.size() == values.size()
```

始终成立。

---

# 14. 整体思路

对于这样的函数：

```cpp
static std::vector<int> sortedValues(
    const std::vector<int> &values,
    bool smallestFirst
)
```

可以把思路记成三步：

```text
1. copy
2. sort
3. return
```

代码结构：

```cpp
std::vector<int> result = values;

if (...) {
    std::sort(...);
} else {
    std::sort(...);
}

return result;
```

---

# 15. 核心总结

### `std::sort`

```cpp
std::sort(begin, end, comparator);
```

表示：

> 对 `[begin, end)` 范围中的元素进行原地排序。

---

### 升序

```cpp
std::less<int>()
```

```text
小 → 大
```

---

### 降序

```cpp
std::greater<int>()
```

```text
大 → 小
```

---

### 完整范围

```cpp
v.begin(), v.end()
```

不要写：

```cpp
v.end() - 1
```

否则最后一个元素不会参与排序。

---

### `const vector&`

```cpp
const std::vector<int> &values
```

不能直接排序，因为排序会修改元素顺序。

如果想保留原 vector：

```cpp
std::vector<int> result = values;
```

然后排序：

```cpp
result
```

---

## 最重要的模板

```cpp
std::vector<int> result = values;

if (smallestFirst) {
    std::sort(
        result.begin(),
        result.end(),
        std::less<int>()
    );
} else {
    std::sort(
        result.begin(),
        result.end(),
        std::greater<int>()
    );
}

return result;
```

可以把它记成：

> **复制输入 → 根据条件选择比较器 → 排序副本 → 返回副本。**
