# C++：把 `std::map` 转换成 Grader 可比较的结果

## 1. 为什么不能直接返回 `std::map`

这类自动评分器只能比较一些简单类型，例如：

* `int`
* `bool`
* `char`
* `std::string`
* `std::vector<int>`
* `std::vector<char>`
* `std::vector<std::string>`

但通常不能直接比较：

* `std::map`
* `std::set`
* `std::vector<std::vector<...>>`
* `std::vector<bool>`

特别注意：

```cpp
bool
```

本身可以返回，但：

```cpp
std::vector<bool>
```

可能不被 grader 支持。

所以如果我们使用 `std::map` 来处理中间数据，最后需要把它转换成 grader 能识别的类型。

---

# 2. `map` 适合统计单词出现次数

例如：

```cpp
std::map<std::string, int> counts;

++counts["fig"];
++counts["pear"];
++counts["pear"];
++counts["pear"];
```

结果相当于：

```text
fig  -> 1
pear -> 3
```

常见统计模式：

```cpp
for (const std::string &word : words) {
    ++counts[word];
}
```

这里：

```cpp
counts[word]
```

如果 `word` 不存在，会自动创建：

```cpp
word -> 0
```

然后：

```cpp
++
```

把它增加到 1。

---

# 3. 把 `map` 转换成 `vector<string>`

假设：

```cpp
std::map<std::string, int> counts;
counts["fig"] = 1;
counts["pear"] = 3;
```

可以转换成：

```cpp
std::vector<std::string> report;

for (const auto &[word, count] : counts) {
    report.push_back(
        word + "=" + std::to_string(count)
    );
}
```

最终：

```text
["fig=1", "pear=3"]
```

这里有两个重要操作。

---

## 4. `std::to_string`

`count` 是：

```cpp
int
```

而我们要构造的是：

```cpp
std::string
```

因此不能直接写：

```cpp
word + "=" + count
```

因为字符串不能直接和整数拼接。

需要：

```cpp
std::to_string(count)
```

例如：

```cpp
std::to_string(3)
```

得到：

```cpp
"3"
```

所以正确写法是：

```cpp
word + "=" + std::to_string(count)
```

例如：

```cpp
"pear" + "=" + "3"
```

得到：

```text
pear=3
```

---

# 5. `push_back`

`push_back` 用来向 `vector` 尾部添加一个元素：

```cpp
report.push_back("apple=2");
```

如果：

```cpp
std::vector<std::string> report;
```

连续执行：

```cpp
report.push_back("apple=2");
report.push_back("pear=3");
```

结果就是：

```text
["apple=2", "pear=3"]
```

---

# 6. 为什么结果自动按单词排序

`std::map` 会按照 **key 的升序** 保存和遍历元素。

例如插入顺序：

```cpp
counts["pear"] = 3;
counts["apple"] = 2;
counts["fig"] = 1;
```

遍历：

```cpp
for (const auto &[word, count] : counts)
```

得到的顺序仍然是：

```text
apple
fig
pear
```

因此不需要再：

```cpp
std::sort(...)
```

也就是说：

> `map` 本身已经帮助我们按照单词字典序产生结果。

---

# 7. 根据 `minCount` 过滤

题目要求：

> 只保留出现次数 `>= minCount` 的单词。

因此：

```cpp
if (count >= minCount) {
    ...
}
```

例如：

```text
apple -> 2
fig   -> 1
pear  -> 3
```

如果：

```cpp
minCount = 2
```

结果为：

```text
apple=2
pear=3
```

`fig` 被跳过。

推荐直接：

```cpp
if (count >= minCount) {
    report.push_back(...);
}
```

而不是先全部加入，再删除不符合条件的元素。

---

# 8. 边界情况

## `words` 为空

```cpp
words = {}
```

那么：

```cpp
counts
```

也是空的，所以返回：

```cpp
{}
```

---

## `minCount` 大于所有出现次数

例如：

```text
apple -> 2
pear  -> 3
```

但：

```cpp
minCount = 10
```

没有任何元素满足条件，因此返回：

```cpp
{}
```

---

## `minCount <= 0`

只要一个单词存在于 `map` 中，它至少出现了 1 次。

因此：

```cpp
minCount <= 0
```

时，所有出现过的单词都会被保留。

---

# 9. 这道题的核心流程

整个问题可以拆成三步：

```text
vector<string> words
        ↓
用 map<string, int> 统计次数
        ↓
根据 minCount 过滤
        ↓
把每个结果转换为 "word=count"
        ↓
vector<string>
```

核心代码模式：

```cpp
std::map<std::string, int> counts;

for (const std::string &word : words) {
    ++counts[word];
}

std::vector<std::string> result;

for (const auto &[word, count] : counts) {
    if (count >= minCount) {
        result.push_back(
            word + "=" + std::to_string(count)
        );
    }
}

return result;
```

---

# 10. 最值得记住的几个写法

单词计数：

```cpp
++counts[word];
```

遍历 `map`：

```cpp
for (const auto &[word, count] : counts)
```

整数转字符串：

```cpp
std::to_string(count)
```

添加到 `vector`：

```cpp
result.push_back(...)
```

过滤条件：

```cpp
count >= minCount
```

最终组合：

```cpp
result.push_back(
    word + "=" + std::to_string(count)
);
```
