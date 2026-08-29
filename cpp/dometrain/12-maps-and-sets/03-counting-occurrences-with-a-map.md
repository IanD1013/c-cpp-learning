# C++ `std::map` 计数技巧：`++counts[word]`

在 C++ 中，`std::map<std::string, int>` 很适合用来统计每个字符串出现了多少次。

典型写法：

```cpp
#include <map>
#include <string>

std::map<std::string, int> counts;

++counts["ant"];
++counts["ant"];
```

执行后：

```cpp
counts["ant"] == 2
```

---

## 1. 为什么 `++counts[word]` 可以直接计数？

关键在于 `std::map` 的 `operator[]`。

假设：

```cpp
std::map<std::string, int> counts;
```

此时 `"ant"` 还不存在。

执行：

```cpp
counts["ant"];
```

如果 key `"ant"` 不存在，`map` 会自动创建：

```cpp
"ant" -> 0
```

因为 `int` 的默认值是 `0`。

所以：

```cpp
++counts["ant"];
```

实际上可以理解为：

```cpp
// 如果 ant 不存在
counts["ant"] = 0;

// 然后 +1
counts["ant"] += 1;
```

因此第一次出现：

```text
0 -> 1
```

第二次出现：

```text
1 -> 2
```

第三次出现：

```text
2 -> 3
```

这就是经典的计数写法：

```cpp
for (const auto &word : words) {
    ++counts[word];
}
```

---

# 2. 前置 `++` 和后置 `++` 的区别

下面两个表达式最终都会把数值增加 1：

```cpp
++counts[word];
counts[word]++;
```

但是**整个表达式的返回值不同**。

## 前置 `++`

```cpp
++counts[word]
```

含义：

> 先加 1，再返回增加后的值。

例如：

```cpp
int x = 1;
int y = ++x;
```

结果：

```cpp
x == 2
y == 2
```

因此：

```cpp
if (++counts[word] == 2)
```

表示：

> 如果这个单词刚刚变成“出现第 2 次”，执行逻辑。

---

## 后置 `++`

```cpp
counts[word]++
```

含义：

> 先返回旧值，再增加 1。

例如：

```cpp
int x = 1;
int y = x++;
```

结果：

```cpp
x == 2
y == 1
```

因此下面的代码：

```cpp
if (counts[word]++ == 2)
```

实际上是在检查：

> 增加之前是不是已经出现了 2 次。

所以它会在**第三次出现**时才成立。

---

# 3. 为什么判断 `== 2`？

假设题目要求：

> 统计有多少个不同的单词至少出现两次。

例如：

```cpp
{"ant", "bee", "ant", "cow"}
```

出现次数：

```text
ant -> 2
bee -> 1
cow -> 1
```

因此答案：

```text
1
```

可以这样写：

```cpp
int repeated = 0;

for (const auto &word : words) {
    if (++counts[word] == 2) {
        ++repeated;
    }
}
```

为什么是：

```cpp
== 2
```

而不是：

```cpp
>= 2
```

因为一个单词无论出现多少次，都只能算 **1 个重复单词**。

假设：

```text
ant 出现 5 次
```

计数变化：

```text
第一次：1
第二次：2  -> repeated +1
第三次：3
第四次：4
第五次：5
```

只有从：

```text
1 -> 2
```

这一刻需要增加答案。

所以：

```cpp
if (++counts[word] == 2)
```

非常漂亮地表达了：

> “这个单词是否刚刚第二次出现？”

如果写：

```cpp
if (++counts[word] >= 2)
```

那么同一个单词出现第 3、4、5 次时都会继续增加答案，导致重复统计。

---

# 4. `map` 必须声明在循环外

正确：

```cpp
std::map<std::string, int> counts;

for (const auto &word : words) {
    ++counts[word];
}
```

因为整个循环必须共享同一个计数表。

错误：

```cpp
for (const auto &word : words) {
    std::map<std::string, int> counts;
    ++counts[word];
}
```

这样每次循环都会创建一个全新的空 `map`。

效果相当于：

```text
ant -> 第一次出现
bee -> 第一次出现
ant -> 又被当成第一次出现
cow -> 第一次出现
```

因此永远无法统计重复。

---

# 5. 不需要排序

例如：

```cpp
{"ant", "bee", "cow", "ant"}
```

两个 `"ant"` 并不相邻，但没有关系。

因为 `map` 会根据 key 找到对应的计数：

```cpp
++counts["ant"];
```

无论 `"ant"` 出现在数组什么位置，访问的都是同一个：

```text
"ant" -> count
```

因此不需要：

```cpp
std::sort(...)
```

也不需要先把相同单词放到一起。

---

# 6. 时间复杂度

对于：

```cpp
std::map<std::string, int>
```

一次查找或插入通常是：

```text
O(log k)
```

其中 `k` 是不同单词的数量。

如果一共有 `n` 个单词，总体大约：

```text
O(n log k)
```

额外空间：

```text
O(k)
```

因为每个不同单词在 `map` 中存一个 entry。

如果题目没有要求必须使用 `std::map`，实际工程中做频率统计时也经常使用：

```cpp
std::unordered_map<std::string, int>
```

平均查找复杂度通常为：

```text
O(1)
```

因此平均整体复杂度可以接近：

```text
O(n)
```

---

# 7. 核心模板

统计元素出现次数：

```cpp
std::map<std::string, int> counts;

for (const auto &word : words) {
    ++counts[word];
}
```

统计“至少出现两次的不同元素数量”：

```cpp
std::map<std::string, int> counts;
int repeated = 0;

for (const auto &word : words) {
    if (++counts[word] == 2) {
        ++repeated;
    }
}
```

---

# 8. 最值得记住的思想

```cpp
++counts[word]
```

可以理解为：

```text
取得 word 的计数
↓
不存在则自动创建为 0
↓
计数 +1
↓
返回增加后的计数
```

因此：

```cpp
if (++counts[word] == 2)
```

准确表达：

> **如果这个单词刚刚第二次出现。**

这是 C++ 中非常常见、非常实用的频率统计 idiom。
