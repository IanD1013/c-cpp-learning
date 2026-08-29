# C++ `std::map` 遍历、结构化绑定与顺序

## 1. 遍历 `std::map` 时拿到的是什么？

`std::map<Key, Value>` 中的每个元素不是一个单独的值，而是一个 **键值对（key-value pair）**。

例如：

```cpp
#include <map>
#include <string>

std::map<std::string, int> totals;

totals["cow"] = 50;
totals["ant"] = 30;
```

这里 `totals` 中有两个 entry：

```text
"ant" -> 30
"cow" -> 50
```

传统写法可以这样遍历：

```cpp
for (const auto &entry : totals) {
    std::string key = entry.first;
    int value = entry.second;
}
```

其中：

```cpp
entry.first
```

表示 key，

```cpp
entry.second
```

表示 value。

也就是说，一个 map entry 可以理解为：

```cpp
std::pair<const Key, Value>
```

因此才有 `.first` 和 `.second`。

---

## 2. 为什么使用 `const auto &`？

推荐：

```cpp
for (const auto &entry : totals) {
    // ...
}
```

而不是：

```cpp
for (auto entry : totals) {
    // ...
}
```

区别在于：

```cpp
auto entry
```

会把 map 中的 entry **复制一份**。

而：

```cpp
const auto &entry
```

表示：

* `&`：直接引用 map 中原来的元素，不复制
* `const`：只读取，不修改

对于：

```cpp
std::map<std::string, int>
```

尤其值得使用引用，因为 key 是 `std::string`，如果每次循环都复制字符串，会产生不必要的开销。

因此，只读遍历通常推荐：

```cpp
for (const auto &entry : totals)
```

---

## 3. C++17 结构化绑定 Structured Binding

相比：

```cpp
for (const auto &entry : totals) {
    std::string key = entry.first;
    int value = entry.second;
}
```

C++17 可以写成：

```cpp
for (const auto &[word, total] : totals) {
    // word  是 key
    // total 是 value
}
```

这叫做：

> **Structured Binding（结构化绑定）**

基本语法：

```cpp
auto [a, b] = pair;
```

对于 `std::map`：

```cpp
for (const auto &[key, value] : map) {
}
```

第一个变量一定绑定 key，第二个变量一定绑定 value。

例如：

```cpp
std::map<std::string, int> totals;
totals["ant"] = 30;

for (const auto &[word, total] : totals) {
    // word  == "ant"
    // total == 30
}
```

变量名完全可以自己决定：

```cpp
for (const auto &[name, score] : scores) {
}
```

或者：

```cpp
for (const auto &[animal, amount] : totals) {
}
```

但注意：

```cpp
for (const auto &[value, key] : totals)
```

并不会交换 key 和 value。

这里只是把：

```text
第一个位置（key）
```

命名成了 `value`，

把：

```text
第二个位置（value）
```

命名成了 `key`。

因此名字最好与真实含义保持一致。

---

# 4. `std::map` 会自动按照 key 排序

`std::map` 一个非常重要的性质是：

> 遍历顺序按照 key 的升序进行。

例如：

```cpp
std::map<std::string, int> totals;

totals["cow"] = 50;
totals["ant"] = 30;
totals["dog"] = 40;
```

虽然插入顺序是：

```text
cow
ant
dog
```

但遍历：

```cpp
for (const auto &[word, total] : totals) {
    std::cout << word << '\n';
}
```

得到：

```text
ant
cow
dog
```

因为字符串按照字典序排序：

```text
ant < cow < dog
```

因此：

```cpp
std::map
```

本身已经维护好顺序，通常不需要再调用：

```cpp
std::sort(...)
```

---

# 5. `std::map` 与 `std::unordered_map` 的区别

## `std::map`

特点：

```text
key 唯一
自动按照 key 排序
遍历顺序确定
```

例如：

```cpp
std::map<std::string, int>
```

遍历时：

```text
ant
cow
dog
```

---

## `std::unordered_map`

特点：

```text
key 唯一
基于哈希表
没有排序保证
遍历顺序不可依赖
```

因此如果题目要求：

> 按照 key 从小到大处理元素

应优先使用：

```cpp
std::map
```

而不是：

```cpp
std::unordered_map
```

---

# 6. 使用 `+=` 对重复 key 累加

假设有：

```cpp
keys   = {"ant", "cow", "ant"};
values = {10,    20,    30};
```

希望相同 key 的值累加。

可以直接：

```cpp
std::map<std::string, int> totals;

for (std::size_t i = 0; i < keys.size(); ++i) {
    totals[keys[i]] += values[i];
}
```

过程：

第一次：

```cpp
totals["ant"] += 10;
```

因为 `"ant"` 不存在：

```cpp
totals["ant"]
```

会自动创建：

```text
"ant" -> 0
```

随后变成：

```text
"ant" -> 10
```

然后：

```cpp
totals["cow"] += 20;
```

得到：

```text
"cow" -> 20
```

最后：

```cpp
totals["ant"] += 30;
```

得到：

```text
"ant" -> 40
```

最终：

```text
ant -> 40
cow -> 20
```

这也是一个非常常见的 C++ 计数/累加模式：

```cpp
map[key] += value;
```

---

# 7. Running Total：运行中的累计和

假设 map 为：

```text
ant -> 30
cow -> 50
dog -> 20
```

因为 `std::map` 按 key 排序，所以遍历顺序：

```text
ant
cow
dog
```

维护：

```cpp
int runningTotal = 0;
```

然后：

```cpp
for (const auto &[key, value] : totals) {
    runningTotal += value;
}
```

变化过程：

```text
开始             0
处理 ant         30
处理 cow         80
处理 dog        100
```

如果 threshold 是：

```text
70
```

那么：

```text
ant 后累计值 = 30
cow 后累计值 = 80
```

第一次达到：

```text
>= 70
```

的位置是：

```text
cow
```

因此返回 `"cow"`。

---

# 8. 为什么判断条件应该是 `>= threshold`？

应该写：

```cpp
if (runningTotal >= threshold)
```

而不是：

```cpp
if (runningTotal > threshold)
```

因为如果：

```text
runningTotal = 50
threshold    = 50
```

此时已经“达到 threshold”。

所以：

```cpp
>=
```

同时覆盖：

```text
刚好达到
超过 threshold
```

---

# 9. 为什么 `absent` 由调用者传入？

函数参数中有：

```cpp
const std::string &absent
```

用于表示：

> 没有找到满足条件的 key 时应该返回什么。

不能简单使用：

```cpp
return "";
```

因为本题允许：

```text
"" 
```

本身就是一个合法 key。

例如：

```cpp
keys = {"", "abc"};
```

那么空字符串可能是正确答案。

因此需要由调用者单独提供一个 sentinel：

```cpp
absent
```

例如：

```cpp
"<not-found>"
```

或者其他约定值。

---

# 10. 本题涉及的核心知识

这道题主要考察：

```cpp
std::map<std::string, int>
```

以及以下几个知识点：

1. `map[key] += value` 可以自动创建并累加 value
2. 重复 key 在 `std::map` 中只保存一份
3. `std::map` 会按照 key 升序遍历
4. range-based `for` 可以遍历整个 map
5. C++17 可以使用 structured binding：

```cpp
for (const auto &[key, value] : map)
```

6. 使用 running total 计算前缀累计值
7. 使用：

```cpp
>= threshold
```

判断第一次达到阈值的位置
8. 输入不合法或找不到答案时返回调用者提供的：

```cpp
absent
```
