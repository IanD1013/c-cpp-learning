# C++ `std::set`：去重、成员判断与插入

## 1. `std::set` 是什么？

`std::set` 是 C++ 标准库中的一种容器，用来保存一组**不重复的元素**。

使用时需要：

```cpp
#include <set>
```

例如：

```cpp
std::set<int> seen;

seen.insert(4);
seen.insert(9);
seen.insert(4);
```

虽然 `4` 被插入了两次，但最终集合中只有：

```text
4, 9
```

因此：

```cpp
seen.size() == 2
```

---

## 2. `set` 最重要的特点：元素唯一

对于：

```cpp
std::set<int> seen;
```

集合中每个整数最多只能出现一次。

```cpp
seen.insert(4);
seen.insert(4);
seen.insert(4);
```

最终仍然只有一个 `4`。

重复插入：

* 不会报错
* 不需要提前判断
* 不会让集合大小增加

因此，`set` 很适合做：

* 去重
* 判断某个值是否出现过
* 记录已经处理过的元素

---

## 3. `insert()`：向集合中插入元素

基本语法：

```cpp
setName.insert(value);
```

例如：

```cpp
std::set<int> numbers;

numbers.insert(5);
numbers.insert(2);
numbers.insert(5);
```

最终只有：

```text
2, 5
```

注意：

```cpp
numbers.insert(5);
```

即使 `5` 已经存在，也不会产生错误。

---

## 4. 使用 `count()` 判断元素是否存在

对于 `std::set`：

```cpp
seen.count(value)
```

结果只有两种：

```cpp
0   // 不存在
1   // 存在
```

例如：

```cpp
std::set<int> seen;

seen.insert(4);

if (seen.count(4) == 1) {
    // 4 已经存在
}

if (seen.count(8) == 0) {
    // 8 不存在
}
```

因为 `set` 不允许重复元素，所以不可能返回 `2`、`3` 等值。

常见写法：

```cpp
if (seen.count(value) == 0) {
    // value 还没有出现过
}
```

---

## 5. `set` 和 `map` 的区别

`std::set` 只保存一个值：

```cpp
std::set<int>
```

每个元素本身就是需要存储的数据。

而 `std::map` 保存的是：

```text
key -> value
```

例如：

```cpp
std::map<std::string, int> counts;
```

其中：

```text
"apple" -> 3
"banana" -> 5
```

可以简单理解为：

| 容器          | 保存内容               |
| ----------- | ------------------ |
| `set<T>`    | 唯一的 `T`            |
| `map<K, V>` | 唯一的 key + 对应 value |

---

## 6. `set` 没有下标

下面是错误的：

```cpp
std::set<int> seen;

seen[0];   // ❌ 不允许
```

`set` 不像 `vector`：

```cpp
values[0];
values[1];
```

它没有“第 0 个、第 1 个”这种数组式访问接口。

因此：

```cpp
seen[0]
```

不会编译。

---

# 使用 `set` 找出第一次出现的新元素

考虑下面的问题：

```cpp
newValuesInArrivalOrder(already, values)
```

要求返回：

> `values` 中那些之前没有出现过的值，并且每个值只返回一次，同时保持它们在 `values` 中第一次出现的顺序。

例如：

```cpp
already = {}
values = {8, -2, 8, -2, 5}
```

结果应该是：

```cpp
{8, -2, 5}
```

---

## 7. 为什么既需要 `set`，又需要 `vector`？

这里有两个不同的需求。

### `set`：判断是否出现过

使用：

```cpp
std::set<int> seen;
```

记录所有已经出现过的值。

例如处理：

```text
8, -2, 8, -2, 5
```

过程中的 `seen` 会逐渐变成：

```text
{8}
{-2, 8}
{-2, 8}
{-2, 8}
{-2, 5, 8}
```

---

### `vector`：保存到达顺序

`std::set` 本身会按照元素大小排序。

例如：

```cpp
std::set<int> seen = {8, -2, 5};
```

遍历它时顺序是：

```text
-2, 5, 8
```

但题目要求的是：

```text
8, -2, 5
```

也就是第一次出现时的顺序。

因此不能最后再从 `set` 中构造答案。

正确方法是：

```cpp
std::vector<int> answer;
```

第一次看到新值的时候，立即：

```cpp
answer.push_back(value);
```

---

# 8. 核心算法

首先把 `already` 中所有元素放入集合：

```cpp
std::set<int> seen;

for (int value : already) {
    seen.insert(value);
}
```

然后遍历 `values`：

```cpp
for (int value : values) {
    if (seen.count(value) == 0) {
        answer.push_back(value);
        seen.insert(value);
    }
}
```

核心逻辑就是：

```text
如果 value 没见过：
    加入答案
    标记为已经见过
```

---

## 9. 为什么一定要先判断，再插入？

正确：

```cpp
if (seen.count(value) == 0) {
    answer.push_back(value);
    seen.insert(value);
}
```

不要写成：

```cpp
seen.insert(value);

if (seen.count(value) == 0) {
    ...
}
```

因为一旦：

```cpp
seen.insert(value);
```

执行完成后，这个值就已经存在于集合中了。

于是：

```cpp
seen.count(value)
```

一定会得到：

```cpp
1
```

所以判断必须发生在插入之前。

---

## 10. 为什么遍历 `values` 时也要继续插入 `seen`？

假设：

```cpp
already = {}
values = {8, 8, 8}
```

第一次处理 `8`：

```cpp
seen.count(8) == 0
```

所以：

```cpp
answer.push_back(8);
seen.insert(8);
```

第二次处理 `8`：

```cpp
seen.count(8) == 1
```

于是跳过。

最终：

```cpp
answer == {8}
```

如果第一次遇到 `8` 后不把它加入 `seen`，那么后面的 `8` 仍然会被认为是“新元素”。

---

# 11. 示例完整推导

输入：

```cpp
already = {3, 7}
values = {7, 8, 4, 8, 3, 10}
```

初始化：

```text
seen = {3, 7}
answer = {}
```

依次处理：

```text
7
已经存在 → 跳过

8
不存在 → answer = {8}
         seen 加入 8

4
不存在 → answer = {8, 4}
         seen 加入 4

8
已经存在 → 跳过

3
已经存在 → 跳过

10
不存在 → answer = {8, 4, 10}
```

最终：

```cpp
{8, 4, 10}
```

---

# 12. 边界情况

## `already` 为空

```cpp
already = {}
values = {8, -2, 8, 5}
```

结果：

```cpp
{8, -2, 5}
```

---

## `values` 为空

```cpp
already = {1, 2, 3}
values = {}
```

结果：

```cpp
{}
```

这是完全正常的结果，不代表函数失败。

---

## `values` 中没有任何新元素

```cpp
already = {1, 2, 3}
values = {3, 2, 1, 3}
```

结果：

```cpp
{}
```

---

# 13. 需要记住的核心模式

这类题最常见的写法是：

```cpp
std::set<int> seen;
std::vector<int> answer;

for (...) {
    if (seen.count(value) == 0) {
        answer.push_back(value);
        seen.insert(value);
    }
}
```

可以记成：

```text
set   → 判断“以前见过没有”
vector → 保存“第一次出现的顺序”
```

其中最重要的一点是：

> **先检查，后插入。**
