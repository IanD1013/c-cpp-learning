# C++：使用 `for` 循环遍历 `std::string`

## 1. 为什么遍历字符串要用 `size()`

很多字符串操作都需要逐个处理其中的字符，例如：

* 输出每个字符
* 统计某个字符出现的次数
* 查找某个字符
* 检查字符串内容

但字符串到底有多少个字符，通常只有程序运行时才能知道。

因此，循环的结束条件应该从字符串自身获取：

```cpp
const int length = static_cast<int>(s.size());

for (int i = 0; i < length; ++i) {
    std::cout << s[i];
}
```

假设：

```cpp
std::string s = "hello";
```

那么：

```text
s.size() == 5
```

字符串中合法的下标却是：

```text
0  1  2  3  4
h  e  l  l  o
```

也就是说：

```text
合法下标范围：
0 <= i < s.size()
```

这是遍历字符串时最重要的边界条件。

---

# 2. `for` 循环是怎样遍历字符串的

```cpp
for (int i = 0; i < length; ++i) {
    std::cout << s[i];
}
```

可以把它理解成：

```cpp
int i = 0;

while (i < length) {
    std::cout << s[i];
    ++i;
}
```

执行过程是：

1. `i = 0`
2. 检查 `i < length`
3. 执行循环体
4. 执行 `++i`
5. 再检查 `i < length`
6. 重复以上过程

例如：

```cpp
std::string s = "cat";
```

则：

| `i` | `i < 3` | `s[i]` |
| --: | :-----: | :----: |
|   0 |   true  |  `'c'` |
|   1 |   true  |  `'a'` |
|   2 |   true  |  `'t'` |
|   3 |  false  |   不执行  |

所以字符串中的三个字符刚好都被访问一次。

---

# 3. 为什么必须使用 `<`，不能使用 `<=`

正确：

```cpp
for (int i = 0; i < length; ++i)
```

错误：

```cpp
for (int i = 0; i <= length; ++i)
```

原因是：

```cpp
length == s.size()
```

但是：

```cpp
s[length]
```

并不是字符串中的一个字符。

例如：

```cpp
std::string s = "cat";
```

有：

```text
size() = 3
```

合法字符：

```cpp
s[0]   // 'c'
s[1]   // 'a'
s[2]   // 't'
```

而：

```cpp
s[3]
```

已经超出了字符串字符的合法范围。

因此循环条件应该始终保证：

```cpp
i < length
```

而不是：

```cpp
i <= length
```

可以记成：

> `size()` 是字符数量，不是最后一个字符的下标。

最后一个字符的下标是：

```text
size() - 1
```

前提是字符串非空。

---

# 4. 循环条件实际上在保护字符串访问

考虑：

```cpp
for (int i = 0; i < length; ++i) {
    std::cout << s[i];
}
```

因为循环只有在：

```cpp
i < length
```

时才会执行，所以每次访问：

```cpp
s[i]
```

之前都已经保证：

```text
0 <= i < length
```

而：

```text
length == s.size()
```

因此实际上保证了：

```text
0 <= i < s.size()
```

这正是字符串合法的下标范围。

所以可以把：

```cpp
i < length
```

理解成对循环中每次 `s[i]` 访问的统一边界保护。

---

# 5. 为什么代码中把 `size()` 转成 `int`

`std::string::size()` 返回的并不是 `int`，而是一种无符号整数类型：

```cpp
std::string::size_type
```

通常底层类似：

```cpp
std::size_t
```

因此：

```cpp
int i = 0;

for (int i = 0; i < s.size(); ++i)
```

实际上是在比较：

```text
int
vs
unsigned integer
```

部分编译器会给出 signed/unsigned comparison warning。

一种写法是先转换：

```cpp
const int length = static_cast<int>(s.size());

for (int i = 0; i < length; ++i) {
    std::cout << s[i];
}
```

这样比较的两边都是：

```cpp
int
```

比较简单，也适合目前这种教学代码。

---

# 6. `const int length` 为什么使用 `const`

```cpp
const int length = static_cast<int>(s.size());
```

`const` 表示：

> `length` 初始化以后不会再被修改。

例如：

```cpp
length = 10;
```

会编译失败。

这正符合我们的意图，因为循环过程中：

```cpp
length
```

只是一个固定的上界。

因此：

```cpp
const int length = ...
```

比：

```cpp
int length = ...
```

更清楚地表达：

> 这是一个只读的字符串长度。

---

# 7. 不要用 `size() - 1` 作为这种循环的判断条件

例如不要写：

```cpp
for (int i = 0; i <= s.size() - 1; ++i) {
    std::cout << s[i];
}
```

除了写法更复杂以外，它对空字符串尤其危险。

假设：

```cpp
std::string s = "";
```

那么：

```cpp
s.size() == 0
```

但是 `size()` 是无符号整数，所以：

```cpp
s.size() - 1
```

不是普通数学中的：

```text
-1
```

而可能发生 unsigned underflow，得到一个非常大的整数。

于是循环可能错误地继续执行，并访问不存在的字符。

因此遍历整个字符串时，推荐直接写：

```cpp
for (int i = 0; i < length; ++i)
```

而不要自己计算：

```cpp
size() - 1
```

---

# 8. 经典的字符串遍历模板

在当前学习阶段，可以记住这个模板：

```cpp
const int length = static_cast<int>(text.size());

for (int i = 0; i < length; ++i) {
    char current = text[i];

    // 处理 current
}
```

例如打印：

```cpp
for (int i = 0; i < length; ++i) {
    std::cout << text[i];
}
```

检查字符：

```cpp
for (int i = 0; i < length; ++i) {
    if (text[i] == 'a') {
        // 找到了 'a'
    }
}
```

统计字符：

```cpp
int count = 0;

for (int i = 0; i < length; ++i) {
    if (text[i] == 'a') {
        ++count;
    }
}
```

---

# 9. `==` 和 `=` 一定要区分

比较两个值是否相等：

```cpp
text[i] == target
```

这里必须使用：

```cpp
==
```

因为：

```text
==    比较是否相等
=     赋值
```

例如：

```cpp
if (text[i] == target)
```

意思是：

> 当前字符是否等于 `target`？

而：

```cpp
text[i] = target;
```

表示：

> 把当前位置的字符改成 `target`。

另外，如果函数参数是：

```cpp
const std::string &text
```

那么 `text` 是只读的，因此：

```cpp
text[i] = target;
```

本身也不会被允许。

---

# 10. 核心知识总结

遍历一个字符串时，最重要的规律是：

```text
字符串长度 = size()

合法下标：
0 <= i < size()

第一个字符：
text[0]

最后一个字符：
text[size() - 1]
```

因此最常见的循环模式是：

```cpp
const int length = static_cast<int>(text.size());

for (int i = 0; i < length; ++i) {
    // text[i]
}
```

最重要的是记住：

> **遍历字符串通常使用 `i < size()`，而不是 `i <= size()`。**

因为：

```text
size() 是字符数量
size() - 1 才是最后一个字符的下标
```
