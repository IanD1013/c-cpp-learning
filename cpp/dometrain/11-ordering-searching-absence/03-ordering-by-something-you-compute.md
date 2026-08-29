# C++：使用 Lambda 按字符串长度排序

## 1. `std::sort` 不一定直接比较元素本身

对于字符串，直接写：

```cpp
a < b
```

比较的是字符串的字典序，例如：

```cpp
{"apple", "zip", "blueberry"}
```

按默认规则排序后：

```cpp
{"apple", "blueberry", "zip"}
```

但 `std::sort` 的比较函数并不要求我们直接比较 `a` 和 `b`。

我们可以先从元素中计算出某个**排序依据（sort key）**，然后比较这个依据。

例如，按字符串长度排序：

```cpp
std::sort(v.begin(), v.end(),
    [](const std::string &a, const std::string &b) {
        return a.size() < b.size();
    }
);
```

如果：

```cpp
std::vector<std::string> v = {
    "apple",
    "zip",
    "blueberry"
};
```

那么排序后：

```cpp
{"zip", "apple", "blueberry"}
```

因为它们的长度分别是：

```text
zip        -> 3
apple      -> 5
blueberry  -> 9
```

---

## 2. 核心思想：比较「元素的某个属性」

Lambda：

```cpp
[](const std::string &a, const std::string &b) {
    return a.size() < b.size();
}
```

可以理解为：

> 如果 `a` 比 `b` 短，那么 `a` 应该排在 `b` 前面。

这里真正被排序的仍然是字符串：

```cpp
"zip"
"apple"
"blueberry"
```

只是排序规则比较的是它们的：

```cpp
size()
```

也就是说：

```text
元素             排序依据
"apple"    ->      5
"zip"      ->      3
"blueberry"->      9
```

这种思路非常常见。

例如：

```cpp
return a.age < b.age;
```

表示按年龄排序。

```cpp
return a.score > b.score;
```

表示按分数从高到低排序。

```cpp
return a.name.size() < b.name.size();
```

表示按名字长度排序。

---

## 3. 为什么不先创建一个长度 vector 再排序？

例如不要这样：

```cpp
std::vector<int> lengths;

for (const std::string &word : words) {
    lengths.push_back(word.size());
}

std::sort(lengths.begin(), lengths.end());
```

这样排序的只是：

```cpp
{5, 3, 9}
```

得到：

```cpp
{3, 5, 9}
```

但原来的字符串：

```cpp
{"apple", "zip", "blueberry"}
```

完全没有被重新排列。

我们真正想做的是：

> 根据字符串的长度，移动字符串本身。

所以排序依据应该放在 `std::sort` 的 comparator 中：

```cpp
return a.size() < b.size();
```

---

## 4. 为什么参数使用 `const std::string &`

推荐写：

```cpp
[](const std::string &a, const std::string &b)
```

而不是：

```cpp
[](std::string a, std::string b)
```

因为如果按值传递：

```cpp
std::string a
```

每次比较都可能复制字符串。

而：

```cpp
const std::string &a
```

表示：

* `&`：引用原来的字符串，不进行不必要的复制
* `const`：比较函数只读取字符串，不修改它

因此：

```cpp
const std::string &
```

是 comparator 接收字符串时非常常见的写法。

---

## 5. `std::sort` 会多次调用比较函数

排序过程中，Lambda 可能被调用很多次：

```cpp
a.size()
b.size()
```

因此这些长度是每次比较时临时计算出来的。

例如排序：

```cpp
{"apple", "zip", "blueberry"}
```

内部可能发生类似：

```text
"apple" 和 "zip" 比较
5 < 3 ? false

"blueberry" 和 "zip" 比较
9 < 3 ? false

"blueberry" 和 "apple" 比较
9 < 5 ? false
```

具体比较顺序由排序算法决定，我们通常不需要关心。

只需要保证 comparator 正确描述：

> 谁应该排在谁前面。

---

## 6. 空字符串不需要特殊处理

空字符串：

```cpp
""
```

它的：

```cpp
size()
```

等于：

```cpp
0
```

所以：

```cpp
"".size() < "no".size()
```

就是：

```cpp
0 < 2
```

自然为 `true`。

因此：

```cpp
{"yes!", "", "three", "no"}
```

按长度排序后：

```cpp
{"", "no", "yes!", "three"}
```

完全不需要额外写：

```cpp
if (a.empty())
```

之类的特殊逻辑。

---

## 7. 不要修改 `const` 输入，排序副本

如果函数参数是：

```cpp
const std::vector<std::string> &words
```

这里的 `const` 表示：

```cpp
words
```

不能被修改。

所以不能直接：

```cpp
std::sort(words.begin(), words.end(), ...);
```

正确做法是先复制：

```cpp
std::vector<std::string> result = words;
```

然后排序：

```cpp
std::sort(result.begin(), result.end(), ...);
```

最后：

```cpp
return result;
```

整体模式：

```cpp
std::vector<std::string> result = words;

std::sort(result.begin(), result.end(), comparator);

return result;
```

这表示：

> 输入保持不变，返回一个排序后的新 vector。

---

## 8. 本题为什么只需要比较长度？

如果两个字符串长度相同：

```cpp
"cat"
"dog"
```

那么：

```cpp
a.size() < b.size()
```

和：

```cpp
b.size() < a.size()
```

都会是：

```cpp
false
```

单纯按长度排序并不能进一步决定它们的先后。

但题目已经说明：

> 所有测试用例中的字符串长度都不同。

因此只需要：

```cpp
return a.size() < b.size();
```

不需要额外处理长度相同的情况。

如果题目要求：

> 长度相同时按字母顺序排列

才需要写成：

```cpp
if (a.size() != b.size()) {
    return a.size() < b.size();
}

return a < b;
```

---

## 9. 通用模板：按计算出的属性排序

以后看到：

> 按某个属性排序

可以直接想到：

```cpp
std::sort(v.begin(), v.end(),
    [](const auto &a, const auto &b) {
        return 属性(a) < 属性(b);
    }
);
```

例如按绝对值：

```cpp
return std::abs(a) < std::abs(b);
```

按字符串长度：

```cpp
return a.size() < b.size();
```

按对象年龄：

```cpp
return a.age < b.age;
```

按分数从高到低：

```cpp
return a.score > b.score;
```

### 记忆重点

```text
std::sort 排的是元素
lambda 决定的是元素之间的先后规则
```

Comparator 不必：

```cpp
return a < b;
```

也可以比较：

```cpp
return property(a) < property(b);
```

这就是自定义排序最重要的思想。
