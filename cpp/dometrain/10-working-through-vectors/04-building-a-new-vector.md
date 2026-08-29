# C++ `vector` 过滤：保留满足条件的元素

## 1. 什么是过滤（filtering）

所谓**过滤一个 `vector`**，就是：

> 遍历原来的容器，只把满足某个条件的元素放进一个新的容器中。

例如：

```cpp
std::vector<std::string> input = {"one", "two", "three"};
```

假设我们只想保留长度大于 3 的字符串：

```cpp
std::vector<std::string> kept;

for (const std::string &item : input) {
    if (item.size() > 3) {
        kept.push_back(item);
    }
}
```

最终：

```cpp
kept == {"three"}
```

---

## 2. 过滤的基本模板

最常见的写法是：

```cpp
std::vector<T> result;

for (const T &item : input) {
    if (条件) {
        result.push_back(item);
    }
}

return result;
```

可以把它理解成三个步骤：

```text
创建空结果
    ↓
逐个检查原 vector 中的元素
    ↓
满足条件 → push_back 到结果中
```

例如：

```cpp
std::vector<int> result;

for (int value : numbers) {
    if (value > 0) {
        result.push_back(value);
    }
}
```

这会保留所有正数。

---

## 3. 为什么通常创建一个新的 `vector`

假设函数参数是：

```cpp
const std::vector<std::string> &words
```

这里有两个重点。

### `&`：不复制整个 vector

```cpp
const std::vector<std::string> &words
```

表示 `words` 是原 vector 的引用，因此调用函数时不会复制整个容器。

---

### `const`：函数不能修改原 vector

因为有：

```cpp
const
```

所以不能执行：

```cpp
words.erase(...);      // 不允许
words.push_back(...);  // 不允许
```

这其实非常适合过滤函数：

```text
原始数据 words
      ↓ 只读取
检查每个元素
      ↓
新 vector result
```

这样原始数据完全不会被修改。

---

## 4. 为什么不建议边遍历边 `erase`

即使原 vector 不是 `const`，边遍历边删除元素也容易出错。

例如：

```cpp
{"a", "b", "c", "d"}
```

如果删除 `"b"`：

```cpp
{"a", "c", "d"}
```

原来的 `"c"` 会自动向左移动一个位置。

因此，如果索引继续增加，就可能跳过 `"c"`。

例如：

```text
原来：

index: 0   1   2   3
       a   b   c   d

删除 b 后：

index: 0   1   2
       a   c   d
```

如果循环此时直接进入下一个索引：

```cpp
i = 2
```

那么 `"c"` 就被跳过了。

所以对于“保留满足条件的元素”这种题目，最简单可靠的方式通常是：

```cpp
创建新 vector
+
push_back 满足条件的元素
```

---

# 5. `std::string::find()`：判断字符串是否包含子字符串

这道题真正需要掌握的判断条件是：

```cpp
word.find(text) != std::string::npos
```

它表示：

> `text` 是否出现在 `word` 中。

例如：

```cpp
std::string word = "pineapple";

word.find("apple");
```

可以找到 `"apple"`，因为：

```text
pineapple
    apple
```

---

## 6. `find()` 返回什么

`find()` 并不是直接返回 `true` / `false`。

它返回：

> 子字符串第一次出现的位置。

例如：

```cpp
std::string word = "hello";

word.find("ell");
```

结果是：

```cpp
1
```

因为：

```text
index: 0 1 2 3 4
       h e l l o
         ↑
        ell
```

---

## 7. 什么是 `std::string::npos`

如果没有找到，`find()` 会返回：

```cpp
std::string::npos
```

因此通常写：

```cpp
if (word.find(text) != std::string::npos) {
    // 找到了
}
```

记忆方式：

```text
find(text) != npos
          ↓
       找到了
```

而：

```cpp
word.find(text) == std::string::npos
```

表示：

```text
没有找到
```

---

# 8. `find()` 是“包含”，不是“相等”

例如：

```cpp
std::string word = "pineapple";
```

下面三个搜索都会成功：

```cpp
word.find("pine");
word.find("apple");
word.find("nap");
```

因为搜索文本可以出现在：

* 开头
* 中间
* 结尾

因此：

```cpp
word.find(text) != std::string::npos
```

表示的是：

```text
word contains text
```

而不是：

```text
word == text
```

---

# 9. 保持原有顺序

过滤时使用：

```cpp
for (const std::string &word : words)
```

按照原 vector 的顺序遍历，再执行：

```cpp
result.push_back(word);
```

所以结果会自动保持原顺序。

例如：

```cpp
words = {
    "apple",
    "pineapple",
    "banana",
    "applepie"
};
```

搜索：

```cpp
"apple"
```

结果是：

```cpp
{
    "apple",
    "pineapple",
    "applepie"
}
```

不会自动排序。

---

# 10. 重复元素也应该保留

例如：

```cpp
words = {
    "apple",
    "banana",
    "apple"
};
```

如果搜索：

```cpp
"apple"
```

结果应该是：

```cpp
{
    "apple",
    "apple"
}
```

过滤只是逐个判断元素，并不会自动去重。

---

# 11. 没有匹配元素怎么办

假设：

```cpp
words = {
    "cat",
    "dog"
};
```

搜索：

```cpp
"hello"
```

没有任何元素匹配。

这时结果自然就是：

```cpp
std::vector<std::string> result;
```

也就是空 vector：

```cpp
{}
```

不需要返回特殊错误值。

---

# 12. 输入 vector 为空怎么办

如果：

```cpp
words = {}
```

那么：

```cpp
for (const std::string &word : words)
```

一次都不会执行。

因此：

```cpp
result
```

仍然为空，最后正常返回：

```cpp
{}
```

这也是正确结果。

---

# 13. 为什么循环变量写成 `const std::string &`

推荐：

```cpp
for (const std::string &word : words)
```

而不是：

```cpp
for (std::string word : words)
```

后者每次循环都会复制一个字符串。

前者：

```cpp
const std::string &word
```

只是引用原 vector 中的字符串：

```text
words 中的字符串
        ↑
      word
```

因此：

* 不复制字符串
* 效率更好
* `const` 保证不会意外修改它

对于只读取字符串的范围 `for` 循环，这是很常见的写法。

---

# 14. 过滤题的通用思维模型

以后看到类似题目：

> 返回所有满足某个条件的元素。

可以直接想到：

```cpp
std::vector<T> result;

for (const T &item : input) {
    if (item 满足条件) {
        result.push_back(item);
    }
}

return result;
```

例如：

### 保留正数

```cpp
if (value > 0)
```

### 保留偶数

```cpp
if (value % 2 == 0)
```

### 保留较长字符串

```cpp
if (word.size() > 5)
```

### 保留包含某段文字的字符串

```cpp
if (word.find(text) != std::string::npos)
```

真正变化的通常只有：

```cpp
if (...)
```

里面的过滤条件。

---

# 核心总结

过滤 `vector` 的典型结构：

```cpp
std::vector<T> result;

for (const T &item : input) {
    if (满足条件) {
        result.push_back(item);
    }
}

return result;
```

字符串包含判断：

```cpp
word.find(text) != std::string::npos
```

表示：

```text
text 出现在 word 中
```

过滤时创建新的 vector 有几个优点：

* 不修改原始数据
* 不需要处理 `erase` 导致的元素移动
* 自动保持原有顺序
* 自动保留重复元素
* 没有匹配时自然返回空 vector
* 输入为空时也自然返回空 vector
