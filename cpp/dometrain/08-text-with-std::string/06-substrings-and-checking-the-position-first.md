# C++ `std::string::substr()`：截取字符串

## 1. `substr()` 是做什么的？

`std::string::substr()` 用来**从一个字符串中截取一段字符，并返回一个新的字符串**。

它不会修改原字符串。

基本形式：

```cpp
string.substr(start);
string.substr(start, length);
```

其中：

* `start`：从哪个位置开始截取
* `length`：最多截取多少个字符
* 返回值：一个新的 `std::string`

例如：

```cpp
std::string text = "abcdefgh";

std::string tail = text.substr(5);
std::string part = text.substr(2, 3);
```

结果：

```cpp
tail == "fgh"
part == "cde"
```

字符串的位置从 `0` 开始：

```text
字符串: a b c d e f g h
位置:   0 1 2 3 4 5 6 7
```

因此：

```cpp
text.substr(5)
```

表示从位置 `5` 开始，一直截取到字符串结尾：

```text
fgh
```

---

## 2. 第二个参数是「长度」，不是结束位置

这是 `substr()` 最容易混淆的地方。

```cpp
text.substr(2, 3)
```

意思是：

> 从位置 `2` 开始，最多取 `3` 个字符。

所以得到：

```text
cde
```

而不是：

```text
cd
```

可以理解为：

```cpp
substr(起始位置, 字符数量)
```

而不是：

```cpp
substr(起始位置, 结束位置)
```

---

## 3. 如果要求的长度超过剩余字符

`substr()` 不会因此报错，而是把剩下的字符全部返回。

例如：

```cpp
std::string text = "abcdefgh";

std::string result = text.substr(7, 4);
```

位置 `7` 只有一个字符：

```text
h
```

虽然要求取 `4` 个字符，但实际上只剩 `1` 个，所以结果为：

```cpp
"h"
```

也就是说：

```cpp
substr(start, length)
```

中的 `length` 是一个**最大长度**。

---

## 4. `substr()` 不会修改原字符串

例如：

```cpp
std::string text = "abcdefgh";

std::string result = text.substr(2, 3);
```

执行后：

```cpp
text   == "abcdefgh"
result == "cde"
```

这是因为 `substr()` 会创建并返回一个新的字符串。

可以简单理解成：

```text
原字符串
abcdefgh
   │
   └──复制 cde──> 新字符串
```

原字符串本身保持不变。

---

# `substr()` 的合法起始位置

假设：

```cpp
std::string text = "abcdefgh";
```

那么：

```cpp
text.size() == 8
```

对于普通字符串索引：

```cpp
text[i]
```

合法索引只有：

```text
0 ~ 7
```

但是 `substr()` 比较特殊。

它允许：

```cpp
start <= text.size()
```

因此：

```cpp
text.substr(8)
```

是合法的。

结果是：

```cpp
""
```

也就是空字符串。

---

## `substr(size)` 为什么合法？

可以把字符串想象成：

```text
a b c d e f g h |
0 1 2 3 4 5 6 7 8
```

位置 `8` 可以理解成：

> 字符串最后一个字符之后的位置。

从这里开始截取，自然没有任何字符，所以得到空字符串。

---

## 但是 `s[size]` 不是合法索引

这是一个非常重要的区别：

```cpp
text.substr(text.size())   // 合法
```

但是：

```cpp
text[text.size()]          // 不能作为正常字符索引使用
```

因此要记住：

```text
字符串索引：
0 <= index < size

substr 起点：
0 <= start <= size
```

也就是：

```cpp
s[i]        → i < size
s.substr(i) → i <= size
```

---

# 超出字符串长度的 `start`

如果：

```cpp
start > text.size()
```

那么：

```cpp
text.substr(start)
```

会抛出 `std::out_of_range` 异常。

例如：

```cpp
std::string text = "abcdefgh";

text.substr(9);
```

这里：

```cpp
text.size() == 8
```

而：

```cpp
9 > 8
```

因此调用非法。

所以如果程序不希望异常中断，应该在调用 `substr()` 之前先检查范围。

---

# 安全使用 `substr()` 的基本规则

假设：

```cpp
int start;
int length;
std::string text;
```

在调用：

```cpp
text.substr(start, length);
```

之前，可以检查：

```cpp
if (start < 0 ||
    length < 0 ||
    start > static_cast<int>(text.size()))
{
    // 非法输入
}
```

这里允许：

```cpp
start == text.size()
```

因为这种情况下：

```cpp
text.substr(start, length)
```

合法，只会返回空字符串。

---

# 为什么使用 `static_cast<int>(text.size())`？

`text.size()` 返回的并不是 `int`，而是：

```cpp
std::string::size_type
```

通常它是一个**无符号整数类型**。

而：

```cpp
start
```

是：

```cpp
int
```

直接比较：

```cpp
start > text.size()
```

可能出现 signed / unsigned 类型比较问题。

因此这里写：

```cpp
start > static_cast<int>(text.size())
```

把：

```cpp
text.size()
```

转换成 `int`，这样双方都是：

```cpp
int
```

比较会更直观。

---

# 本题需要实现的规则

函数：

```cpp
static std::string extractField(
    const std::string &text,
    int start,
    int length
)
```

需要遵守：

```text
start < 0
    → 返回 "?"

length < 0
    → 返回 "?"

start > text.size()
    → 返回 "?"

其他情况
    → 返回 text.substr(start, length)
```

注意：

```cpp
start == text.size()
```

不是错误。

例如：

```cpp
extractField("abcdefgh", 8, 3)
```

应该得到：

```cpp
""
```

而不是：

```cpp
"?"
```

因此这里不能把空字符串当成错误标记，因为：

```cpp
""
```

本身可能是一个完全合法的结果。
