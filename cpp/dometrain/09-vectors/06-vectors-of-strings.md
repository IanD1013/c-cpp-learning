# C++：`std::vector<std::string>` 与字符串拼接

## 1. `vector` 也可以存字符串

`std::vector<T>` 中的 `T` 表示元素类型。

如果写：

```cpp
std::vector<int>
```

那么 vector 中保存的是整数。

如果写：

```cpp
std::vector<std::string>
```

那么保存的就是字符串。

例如：

```cpp
#include <string>
#include <vector>

std::vector<std::string> words = {"red", "", "blue"};
```

这个 vector 中有 3 个元素：

```text
words[0] -> "red"
words[1] -> ""
words[2] -> "blue"
```

因此：

```cpp
words.size()
```

返回：

```text
3
```

注意：

```cpp
""
```

虽然是**空字符串**，但它仍然是一个元素，所以仍然会计入 `size()`。

---

# 2. 使用范围 `for` 遍历字符串 vector

可以像遍历数字 vector 一样遍历字符串：

```cpp
std::string joined;

for (const std::string &word : words) {
    joined += word;
}
```

最终：

```text
joined == "redblue"
```

过程大致是：

```text
开始：

joined = ""

第 1 次：
word = "red"
joined = "red"

第 2 次：
word = ""
joined = "red"

第 3 次：
word = "blue"
joined = "redblue"
```

空字符串虽然没有增加字符，但它依然被正常遍历了一次。

---

# 3. 为什么使用 `const std::string &word`

推荐：

```cpp
for (const std::string &word : words)
```

而不是：

```cpp
for (std::string word : words)
```

区别在于后者会复制字符串。

例如：

```cpp
for (std::string word : words)
```

每一次循环都会把 vector 中的字符串复制到 `word` 中。

对于较大的字符串，这种复制没有必要。

而：

```cpp
const std::string &word
```

可以拆成：

```text
const
std::string
&
word
```

含义分别是：

* `std::string`：元素类型是字符串
* `&`：使用引用，不复制原字符串
* `const`：只允许读取，不允许通过 `word` 修改原元素
* `word`：当前元素的变量名

因此，读取容器元素时常见的写法是：

```cpp
for (const T &element : container)
```

例如：

```cpp
for (const std::string &word : words) {
    // 使用 word
}
```

---

# 4. 使用 `+=` 拼接字符串

字符串支持：

```cpp
joined += word;
```

它相当于：

```cpp
joined = joined + word;
```

但通常更适合逐步构造结果字符串。

例如：

```cpp
std::string joined;

joined += "hello";
joined += " ";
joined += "world";
```

最终：

```text
hello world
```

---

# 5. Join 操作：在元素之间添加分隔符

假设：

```cpp
std::vector<std::string> parts = {"red", "green", "blue"};
```

希望使用：

```text
","
```

连接，正确结果应该是：

```text
red,green,blue
```

关键规则是：

> separator 应该放在两个元素之间，而不是简单地放在每个元素之后或之前。

---

## 错误方法 1：每个元素后面都加 separator

```cpp
for (...) {
    joined += part;
    joined += separator;
}
```

结果：

```text
red,green,blue,
```

最后多出了一个分隔符。

---

## 错误方法 2：每个元素前面都加 separator

```cpp
for (...) {
    joined += separator;
    joined += part;
}
```

结果：

```text
,red,green,blue
```

开头多出了一个分隔符。

---

# 6. 正确思路：除了第一个元素，其他元素前都加 separator

可以使用一个 `bool` 记录：

> 当前是不是第一个元素。

例如：

```cpp
bool hasPrevious = false;

for (const std::string &part : parts) {
    if (hasPrevious) {
        joined += separator;
    }

    joined += part;
    hasPrevious = true;
}
```

第一次循环：

```text
hasPrevious == false
```

所以：

```cpp
separator
```

不会添加。

之后：

```text
hasPrevious == true
```

每个新元素之前都会添加 separator。

---

# 7. 空字符串不能被跳过

这是 join 操作中一个非常重要的边界情况。

例如：

```cpp
{"a", "", "b"}
```

使用：

```text
","
```

连接。

正确结果是：

```text
a,,b
```

为什么有两个逗号？

因为实际上存在三个元素：

```text
"a"
""
"b"
```

它们之间有两个位置：

```text
"a" , "" , "b"
```

所以：

```text
a,,b
```

---

再例如：

```cpp
{"", "x", ""}
```

使用：

```text
"-"
```

连接。

结果：

```text
-x-
```

逻辑结构其实是：

```text
"" - "x" - ""
```

第一个和最后一个字符串虽然没有字符，但它们依然是元素，因此中间的 separator 必须保留。

所以不能写类似：

```cpp
if (!part.empty())
```

来决定是否添加 separator。

separator 的位置应该由：

> 元素的位置

决定，而不是由：

> 字符串是否为空

决定。

---

# 8. 重要边界情况

假设函数叫：

```cpp
joinWith(parts, separator)
```

应该满足下面这些情况。

## 空 vector

```cpp
{}
```

结果：

```text
""
```

因为没有任何元素。

---

## 只有一个元素

```cpp
{"hello"}
```

使用任何 separator，例如：

```text
","
```

结果仍然是：

```text
hello
```

因为没有两个元素需要被分隔。

---

## 多个普通元素

```cpp
{"a", "b", "c"}
```

使用：

```text
","
```

得到：

```text
a,b,c
```

---

## 中间存在空字符串

```cpp
{"a", "", "b"}
```

得到：

```text
a,,b
```

---

## 两端存在空字符串

```cpp
{"", "x", ""}
```

使用：

```text
"-"
```

得到：

```text
-x-
```

---

# 9. 方法签名

题目要求：

```cpp
static std::string joinWith(
    const std::vector<std::string> &parts,
    const std::string &separator
)
```

这里两个参数都使用了：

```cpp
const ... &
```

例如：

```cpp
const std::vector<std::string> &parts
```

意味着：

* `&`：不会复制整个 vector
* `const`：函数不能修改这个 vector

而：

```cpp
const std::string &separator
```

同样表示：

* 不复制 separator
* 不修改 separator

对于只需要读取的数据，这是一种非常常见的 C++ 参数写法。

---

# 10. 本节核心知识

记住下面几个模式：

```cpp
std::vector<std::string> words;
```

表示字符串 vector。

读取所有元素时：

```cpp
for (const std::string &word : words) {
    ...
}
```

逐步构造字符串：

```cpp
joined += something;
```

实现 join 时：

```text
separator 应该出现在元素之间，而不是元素两端。
```

一个简单可靠的方法是：

```cpp
bool first = true;
```

然后只在：

```cpp
!first
```

时添加 separator。

尤其要记住：

> 空字符串也是一个正常元素，不能因为它为空就跳过 separator。
