# C++：什么时候用 `vector`、`set` 和 `map`

面对一个单词列表时，首先不要急着选择容器，而要先问：

> **我到底想统计什么？**

不同的问题，对应不同的数据结构。

---

## 1. 统计所有单词数量：直接用 `vector::size()`

假设：

```cpp
std::vector<std::string> words{
    "red", "blue", "red", "green", "red"
};
```

如果问题是：

> 一共有多少个单词？

重复单词也要算，所以答案直接是：

```cpp
words.size()
```

结果：

```text
5
```

没有必要再建立 `set` 或 `map`。

### 原因

`vector` 本身已经保存了所有元素：

```text
red
blue
red
green
red
```

元素数量就是总单词数量。

因此：

```cpp
static_cast<int>(words.size())
```

即可得到题目要求的 `int`。

---

# 2. 统计不同单词数量：使用 `std::set`

如果问题变成：

> 一共有多少种不同的单词？

对于：

```text
red
blue
red
green
red
```

不同的单词只有：

```text
red
blue
green
```

所以答案是：

```text
3
```

这类问题非常适合：

```cpp
std::set<std::string>
```

因为 `set` 的重要特点是：

> **同一个值最多只保存一次。**

例如：

```cpp
std::set<std::string> uniqueWords;

uniqueWords.insert("red");
uniqueWords.insert("blue");
uniqueWords.insert("red");
```

最终集合仍然只有：

```text
blue
red
```

第二次插入 `"red"` 不会产生新的元素。

因此可以：

```cpp
std::set<std::string> uniqueWords;

for (const auto &word : words) {
    uniqueWords.insert(word);
}

return static_cast<int>(uniqueWords.size());
```

核心思路：

```text
vector
↓
把每个单词插入 set
↓
重复单词自动消失
↓
set.size()
```

---

# 3. 统计某个单词出现次数：使用 `std::map`

如果问题是：

> `"red"` 出现了多少次？

这时只知道某个单词是否存在已经不够。

我们需要保存：

```text
单词 → 出现次数
```

例如：

```text
red   → 3
blue  → 1
green → 1
```

因此应该使用：

```cpp
std::map<std::string, int>
```

例如：

```cpp
std::map<std::string, int> counts;

for (const auto &word : words) {
    ++counts[word];
}
```

处理完成后：

```cpp
counts["red"]   // 3
counts["blue"]  // 1
counts["green"] // 1
```

因此查询：

```cpp
return counts[probe];
```

即可得到指定单词的出现次数。

---

# 4. 为什么 `set` 和 `map` 不能随便替换？

理解二者最重要的区别：

## `set`

只保存：

```text
value
```

例如：

```text
red
blue
green
```

它只能很好地回答：

```text
"red" 存不存在？
```

结果最多只有：

```text
0 或 1
```

它不能表示：

```text
red 出现了 3 次
```

因为没有地方保存数字 `3`。

---

## `map`

保存：

```text
key → value
```

例如：

```text
red   → 3
blue  → 1
green → 1
```

因此它适合：

```text
某个单词出现多少次？
```

理论上也可以用：

```cpp
std::map<std::string, int>
```

来模拟集合，例如：

```text
red   → 0
blue  → 0
green → 0
```

但这些 `0` 没有任何实际意义。

如果只关心：

```text
某个值是否存在
```

就应该直接使用：

```cpp
std::set
```

让代码本身表达真正的意图。

---

# 5. 三种问题对应三种做法

对于：

```cpp
{"red", "blue", "red", "green", "red"}
```

| 问题             | 合适的方法            | 答案 |
| -------------- | ---------------- | -: |
| 总共有多少个单词？      | `vector::size()` |  5 |
| 有多少个不同单词？      | `std::set`       |  3 |
| `"red"` 出现多少次？ | `std::map`       |  3 |

可以记成：

```text
总数量       → vector.size()
去重         → set
计数 / 频率  → map
```

---

# 6. 空 `vector` 的情况

如果：

```cpp
std::vector<std::string> words;
```

里面没有任何元素，那么三个问题都应该返回：

```text
0
```

也就是：

```cpp
countWords(words)                  // 0
countDistinctWords(words)          // 0
countOccurrencesOf(words, "red")   // 0
```

---

# 7. 为什么使用 `static_cast<int>`？

`size()` 的返回类型不是 `int`，而通常是：

```cpp
std::size_t
```

这是一个不会表示负数的无符号整数类型。

但是题目的函数返回类型规定为：

```cpp
int
```

因此可以显式转换：

```cpp
return static_cast<int>(words.size());
```

以及：

```cpp
return static_cast<int>(uniqueWords.size());
```

这样可以明确表示：

> 我知道这里发生了从 `size_t` 到 `int` 的类型转换。

---

# 8. 最重要的判断原则

选择容器时，不要先想：

```text
哪个最快？
```

而应该先想：

```text
我要表达什么数据关系？
```

在当前问题中：

```text
保存完整序列      → vector
只保存唯一值      → set
保存 key → value → map
```

数据结构首先决定的是：

> **你的程序能够表达什么信息。**

之后才考虑性能问题。
