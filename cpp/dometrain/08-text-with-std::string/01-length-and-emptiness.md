# C++ `std::string`：`size()` 与 `empty()`

## 1. 判断字符串时最常见的两个问题

对于一个 `std::string`，经常需要判断：

1. **字符串是不是空的？**
2. **字符串有多长？**

C++ 分别提供：

```cpp
str.empty()
str.size()
```

例如：

```cpp
std::string label = "Ada";

std::cout << label.size();

if (label.empty()) {
    std::cout << "empty";
}
```

这里：

```cpp
label.size()
```

返回：

```text
3
```

而：

```cpp
label.empty()
```

返回 `false`，所以 `if` 不会执行。

---

## 2. `size()`：获取字符串长度

基本写法：

```cpp
str.size()
```

例如：

```cpp
std::string name = "Ada";

std::cout << name.size();
```

输出：

```text
3
```

因为字符串中保存了 3 个字符：

```text
A d a
```

### `size()` 返回的是“数量”，不是“最后一个索引”

这是很容易混淆的一点。

```cpp
std::string text = "Ada";
```

它的：

```cpp
text.size()
```

是：

```text
3
```

但是合法索引是：

```text
0 1 2
```

对应：

```text
A d a
```

所以：

```cpp
text[2]   // 最后一个字符
```

是合法的。

而：

```cpp
text[3]
```

已经超过字符串中字符的位置。

可以记成：

```text
size = 元素个数

最后一个有效索引 = size - 1
```

---

## 3. `empty()`：判断字符串是否为空

基本写法：

```cpp
str.empty()
```

它返回一个 `bool`：

```cpp
true
false
```

例如：

```cpp
std::string text = "";

if (text.empty()) {
    std::cout << "empty";
}
```

因为 `text` 没有保存任何内容，所以：

```cpp
text.empty()
```

为：

```cpp
true
```

---

## 4. `empty()` 不会清空字符串

注意：

```cpp
text.empty()
```

只是**询问字符串是否为空**。

它不会修改字符串。

例如：

```cpp
std::string text = "hello";

bool result = text.empty();
```

执行后：

```cpp
result == false
```

但：

```cpp
text
```

仍然是：

```text
hello
```

---

## 5. `empty()` 和 `size() == 0`

下面两种写法效果相同：

```cpp
if (text.empty()) {
    ...
}
```

和：

```cpp
if (text.size() == 0) {
    ...
}
```

但是一般更推荐：

```cpp
text.empty()
```

原因是它更直接表达程序的意图：

> “这个字符串是空的吗？”

而不是：

> “这个字符串的长度是不是 0？”

所以：

```cpp
if (text.empty())
```

通常可读性更好。

---

## 6. 空字符串和只有空格的字符串不同

例如：

```cpp
std::string a = "";
std::string b = " ";
```

对于 `a`：

```cpp
a.empty() == true
a.size() == 0
```

但是对于 `b`：

```cpp
b.empty() == false
b.size() == 1
```

因为：

```text
" "
```

虽然人眼看起来像“什么都没有”，但实际上里面有一个**空格字符**。

因此：

```cpp
""
```

和：

```cpp
" "
```

完全不同。

---

# 7. `size()` 的返回类型不是 `int`

`std::string::size()` 返回的通常是：

```cpp
std::string::size_type
```

它本质上是一种用于表示大小的**无符号整数类型**。

因此下面代码：

```cpp
std::string text = "hello";

auto length = text.size();
```

这里的 `length` 并不一定是 `int`。

如果函数明确要求返回 `int`，可以写：

```cpp
return static_cast<int>(text.size());
```

例如：

```cpp
static int lengthOf(const std::string &text) {
    return static_cast<int>(text.size());
}
```

---

## 8. 为什么需要 `static_cast<int>`？

假设函数声明：

```cpp
static int lengthOf(const std::string &text)
```

要求返回：

```cpp
int
```

但是：

```cpp
text.size()
```

返回的是：

```cpp
std::string::size_type
```

所以可以显式转换：

```cpp
static_cast<int>(text.size())
```

含义就是：

> 把 `size()` 得到的值明确转换成 `int`。

---

# 9. `std::string` 的长度与 Unicode

需要特别注意：

```cpp
std::string::size()
```

实际上统计的是 `std::string` 内部保存了多少个 `char`。

在常见的 UTF-8 编码下，一个人眼看到的字符可能需要多个字节。

例如：

```cpp
std::string text = "café";
```

如果字符串采用 UTF-8 编码：

```text
c    1 byte
a    1 byte
f    1 byte
é    2 bytes
```

因此：

```cpp
text.size()
```

可能得到：

```text
5
```

而不是：

```text
4
```

更严谨地说：

> `std::string` 本身并不知道什么是“Unicode 字符”，它只是保存一串 `char`。

因此：

```cpp
size()
```

统计的是存储单元数量，而不一定等于用户看到的文字数量。

对于当前初学阶段，可以先简单记住：

```text
普通 ASCII 字符：

"hello".size() == 5

但遇到中文、emoji、重音字符等 Unicode 内容时，
size() 不一定等于人眼看到的字符数。
```

---

# 10. 与 `const std::string &` 配合

如果函数只需要读取字符串，通常使用：

```cpp
const std::string &text
```

例如：

```cpp
static int lengthOf(const std::string &text) {
    return static_cast<int>(text.size());
}
```

这里：

```cpp
&
```

表示不复制整个字符串。

而：

```cpp
const
```

表示函数不能修改这个字符串。

因此：

```cpp
const std::string &
```

非常适合：

> 函数只读取字符串，但不修改字符串。

---

# 11. 常见模式

## 判断字符串为空

```cpp
if (text.empty()) {
    ...
}
```

## 判断字符串不为空

```cpp
if (!text.empty()) {
    ...
}
```

`!` 表示逻辑取反，因此：

```cpp
!text.empty()
```

可以理解为：

> text 不是空字符串。

---

## 获取长度

```cpp
text.size()
```

---

## 返回 `int` 长度

```cpp
return static_cast<int>(text.size());
```

---

# 12. 核心总结

最重要记住下面几个规则：

```cpp
text.empty()
```

回答：

```text
字符串是否为空？
```

返回：

```cpp
bool
```

---

```cpp
text.size()
```

回答：

```text
字符串内部保存了多少个 char？
```

---

常见判断：

```cpp
if (text.empty()) {
    ...
}
```

```cpp
if (!text.empty()) {
    ...
}
```

如果函数要求返回 `int`：

```cpp
return static_cast<int>(text.size());
```

另外：

```cpp
"".empty()  == true
" ".empty() == false
```

因为空格本身也是一个字符。
