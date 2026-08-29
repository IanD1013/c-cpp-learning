# C++ `vector`：iterator、`erase()` 与 `insert()`

## 1. Index 和 Iterator 是两种不同的东西

访问 `std::vector` 时，我们经常使用 **index（索引）**：

```cpp
std::vector<int> v = {10, 20, 30, 40};

v[0];  // 10
v[1];  // 20
```

这里的 `0`、`1` 是索引。

但 `vector` 的一些成员函数，例如：

```cpp
erase()
insert()
```

通常不接受索引，而是接受一个 **iterator（迭代器）**，用来表示容器中的“位置”。

---

## 2. `begin()` 和 `end()`

假设：

```cpp
std::vector<int> v = {10, 20, 30, 40};
```

可以把它理解成：

```text
index:       0     1     2     3
value:      10    20    30    40
             ↑                 ↑      ↑
          begin()            ...    end()
```

### `v.begin()`

表示第一个元素的位置：

```cpp
v.begin()
```

也就是元素 `10` 所在的位置。

### `v.end()`

表示：

> 最后一个元素之后的位置。

它并不指向任何真正的元素。

因此不能把 `v.end()` 当成最后一个元素来访问。

---

## 3. 从索引得到 iterator

如果我们有索引：

```cpp
std::size_t i = 2;
```

那么：

```cpp
v.begin() + i
```

表示索引 `i` 对应元素的位置。

例如：

```cpp
v.begin() + 0   // 10 的位置
v.begin() + 1   // 20 的位置
v.begin() + 2   // 30 的位置
v.begin() + 3   // 40 的位置
v.begin() + 4   // 等于 v.end()
```

所以可以记住：

```cpp
索引 i
↓
v.begin() + i
↓
对应位置的 iterator
```

---

# 4. `erase()`：删除元素

语法：

```cpp
v.erase(iterator);
```

例如：

```cpp
std::vector<int> v = {10, 20, 30, 40};

std::size_t i = 1;

v.erase(v.begin() + i);
```

`v.begin() + 1` 指向 `20`。

所以删除之后：

```cpp
{10, 30, 40}
```

发生了两件事：

```text
删除 20

10  20  30  40
     ↓ erase

10  30  40
```

后面的元素会整体向左移动。

因此：

```cpp
v.size()
```

也会减少 `1`。

---

## 5. 使用 `erase()` 前必须确保位置有效

如果：

```cpp
std::size_t i = 10;
```

但 vector 只有 4 个元素：

```cpp
v.erase(v.begin() + i);
```

就是非法操作。

因此如果索引来自程序运行过程，通常应保证：

```cpp
i < v.size()
```

例如：

```cpp
if (i < v.size()) {
    v.erase(v.begin() + i);
}
```

---

# 6. `insert()`：插入元素

语法：

```cpp
v.insert(position, value);
```

例如：

```cpp
std::vector<int> v = {10, 30, 40};

v.insert(v.begin(), 99);
```

表示：

> 在第一个元素的位置之前插入 `99`。

结果：

```cpp
{99, 10, 30, 40}
```

原来的元素整体向右移动。

---

## 7. 在不同位置插入

### 插入到最前面

```cpp
v.insert(v.begin(), 99);
```

结果：

```cpp
{99, ...}
```

### 插入到中间

```cpp
v.insert(v.begin() + 2, 99);
```

表示在索引 `2` 原本指向的元素之前插入。

例如：

```cpp
{10, 20, 30, 40}
```

变成：

```cpp
{10, 20, 99, 30, 40}
```

### 插入到最后

```cpp
v.insert(v.end(), 99);
```

结果：

```cpp
{10, 20, 30, 40, 99}
```

效果类似：

```cpp
v.push_back(99);
```

---

# 8. `erase()` 时最容易出现的循环 Bug

假设：

```cpp
std::vector<int> v = {1, 2, 2, 3};
```

我们想删除所有的 `2`。

一个容易写错的版本是：

```cpp
for (std::size_t i = 0; i < v.size(); ++i) {
    if (v[i] == 2) {
        v.erase(v.begin() + i);
    }
}
```

看起来合理，但实际上会漏掉元素。

---

## 9. 为什么会漏掉元素？

开始时：

```text
index:  0  1  2  3
value:  1  2  2  3
```

当：

```cpp
i == 1
```

删除第一个 `2`：

```cpp
v.erase(v.begin() + 1);
```

vector 变成：

```text
index:  0  1  2
value:  1  2  3
```

注意：

原本索引 `2` 的第二个 `2`，已经移动到了索引 `1`。

但是循环结尾执行：

```cpp
++i;
```

于是：

```cpp
i
```

从 `1` 变成 `2`。

程序下一次检查的是：

```cpp
v[2]
```

也就是 `3`。

索引 `1` 的第二个 `2` 被直接跳过去了。

---

# 10. 正确规则：删除后不要增加 index

比较安全的写法是：

```cpp
std::size_t i = 0;

while (i < v.size()) {
    if (v[i] == 2) {
        v.erase(v.begin() + i);
    } else {
        ++i;
    }
}
```

核心规则：

```text
如果删除了当前元素：
    i 不变

如果没有删除：
    ++i
```

原因是删除之后：

> 下一个元素会自动移动到当前索引 `i`。

因此应该继续检查同一个 `i`。

---

# 11. `erase()` / `insert()` 与 iterator 失效

`vector` 的元素删除或插入之后，元素可能会移动。

因此之前保存的 iterator 可能已经不再有效。

例如：

```cpp
auto pos = v.begin() + 2;

v.erase(v.begin());

v.erase(pos);   // 不应该继续使用旧 pos
```

第一次 `erase()` 之后，vector 中的元素已经移动。

此时之前的：

```cpp
pos
```

可能已经失效。

更安全的原则是：

> 修改 vector 后，如果还需要位置，重新从 `begin()` 计算。

例如：

```cpp
v.erase(v.begin() + i);

// 之后如果还需要位置
auto pos = v.begin() + j;
```

而不是长期保存旧 iterator。

---

# 12. 本节最重要的知识点

可以记住下面几个对应关系：

```cpp
v[i]
```

表示：

```text
索引 i 的元素
```

而：

```cpp
v.begin() + i
```

表示：

```text
索引 i 对应的位置（iterator）
```

因此：

```cpp
v.erase(v.begin() + i);
```

就是：

```text
删除索引 i 的元素
```

而：

```cpp
v.insert(v.begin() + i, value);
```

就是：

```text
在索引 i 的位置插入 value
```

最重要的循环规则则是：

```text
erase 当前元素后，不要马上 ++i
```

因为下一个元素已经移动到了当前索引。
