# C++ `std::string` 追加内容：`+=`、`+` 与 `std::to_string`

## 1. 用 `+=` 在字符串末尾追加内容

`std::string` 可以使用 `+=` 把新的内容直接追加到末尾。

```cpp
std::string result;

result += "ab";
result += '!';
result += std::to_string(7);
```

执行后：

```cpp
result == "ab!7"
```

`+=` 会直接修改左边的字符串。

常见可追加类型包括：

```cpp
std::string s;

s += "hello";              // 字符串字面量
s += '!';                  // char
s += std::string("abc");   // std::string
```

---

## 2. 默认构造的 `std::string` 是空字符串

```cpp
std::string result;
```

虽然没有显式初始化，但 `result` 并不是随机内容，而是：

```cpp
""
```

也就是空字符串。

因此可以直接：

```cpp
result += "hello";
```

不需要先写：

```cpp
std::string result = "";
```

两者效果相同。

---

## 3. 数字不能直接当作文本追加

这是很容易出错的一点：

```cpp
result += 7;
```

这里的 `7` 是整数，而 `std::string::operator+=` 可以接受 `char`。

因此它可能把 `7` 当成：

```cpp
char(7)
```

也就是字符编码为 7 的字符，而不是字符：

```cpp
'7'
```

所以如果希望追加数字的十进制文本，需要先转换：

```cpp
result += std::to_string(7);
```

得到：

```text
"7"
```

例如：

```cpp
int n = -25;

std::string result;
result += std::to_string(n);
```

最终：

```cpp
result == "-25"
```

`std::to_string()` 也会自动处理负号。

---

# 4. `+=` 和 `+` 的区别

## `+=`：修改原字符串

```cpp
std::string result = "ab";

result += "cd";
```

结果：

```cpp
result == "abcd"
```

---

## `+`：创建新的字符串

例如：

```cpp
std::string result = "ab";

result + "cd";
```

这一行虽然计算出了：

```cpp
"abcd"
```

但结果没有保存，所以立刻被丢弃。

`result` 仍然是：

```cpp
"ab"
```

如果允许赋值，可以写：

```cpp
result = result + "cd";
```

但如果题目要求必须使用 `+=`，就应该直接写：

```cpp
result += "cd";
```

通常也更直接。

---

# 5. 为什么两个 `char` 不能直接用 `+` 拼字符串

假设：

```cpp
char a = 'A';
char b = 'B';
```

下面并不会得到 `"AB"`：

```cpp
a + b
```

因为两个 `char` 在算术运算时会被提升成整数。

例如 ASCII 中：

```text
'A' = 65
'B' = 66
```

因此：

```cpp
a + b
```

实际类似于：

```cpp
65 + 66
```

结果是整数 `131`。

如果想构造字符串，可以逐个追加：

```cpp
std::string result;

result += a;
result += b;
```

得到：

```cpp
"AB"
```

---

# 6. 在字符之间插入分隔符

假设：

```cpp
text = "abc"
separator = "-"
```

目标是：

```text
a-b-c
```

注意分隔符只存在于两个字符之间：

```text
a - b - c
```

不能得到：

```text
-a-b-c
```

也不能得到：

```text
a-b-c-
```

---

## 推荐思路：不是第一个字符时先加 separator

```cpp
for (int i = 0; i < text.size(); ++i) {
    if (i > 0) {
        result += separator;
    }

    result += text[i];
}
```

执行过程：

### `i == 0`

```text
result = "a"
```

不追加 separator。

### `i == 1`

先：

```text
"a-"
```

再：

```text
"a-b"
```

### `i == 2`

得到：

```text
"a-b-c"
```

这种写法的优点是不会产生末尾多余的 separator。

---

# 7. 为什么“先加字符，再加 separator”更容易出错

例如：

```cpp
for (...) {
    result += text[i];
    result += separator;
}
```

对于：

```text
abc
```

会得到：

```text
a-b-c-
```

最后多了一个 separator。

之后再删除末尾字符虽然可以实现，但会产生额外的边界处理。

更好的方式是：

```cpp
if (i > 0) {
    result += separator;
}

result += text[i];
```

也就是：

> 在元素之前决定是否加 separator，而不是每次无条件在元素之后加。

这个模式非常常见，例如构造：

```text
a,b,c
```

```text
Alice | Bob | Charlie
```

```text
1 -> 2 -> 3
```

---

# 8. 空字符串时循环会自动执行 0 次

假设：

```cpp
text == ""
```

那么：

```cpp
text.size() == 0
```

因此：

```cpp
for (int i = 0; i < text.size(); ++i)
```

第一次条件判断就是 false。

循环不会执行。

如果：

```cpp
std::string result;
```

那么最终自然返回：

```text
""
```

所以这种实现通常不需要额外写：

```cpp
if (text.empty()) {
    ...
}
```

---

# 9. separator 为空时也无需特殊处理

假设：

```cpp
text = "abc";
separator = "";
```

循环仍然会执行：

```cpp
result += "";
```

追加空字符串不会改变任何内容。

最终：

```text
abc
```

因此空 separator 也是自然支持的。

---

# 10. 一个字符的字符串

例如：

```cpp
text = "a";
separator = "-";
```

循环只有一次：

```cpp
i == 0
```

所以：

```cpp
i > 0
```

为 false。

不会添加 separator。

最终：

```text
a
```

这正是需要的结果。

---

# 11. 可选追加数字

如果函数还有：

```cpp
bool appendNumber
int trailingNumber
```

那么是否追加数字，只应该由：

```cpp
appendNumber
```

决定。

例如：

```cpp
if (appendNumber) {
    ...
}
```

不要根据：

```cpp
trailingNumber > 0
```

判断。

因为负数也是合法数字：

```cpp
-5
```

甚至 `0` 也可能需要追加。

例如：

```cpp
appendNumber == true
trailingNumber == 0
```

应该追加：

```text
0
```

---

# 12. 数字前是否需要 separator

如果原来的 `text` 非空：

```text
a-b
```

追加数字时应该得到：

```text
a-b-65
```

所以数字前需要 separator。

但是如果：

```cpp
text == ""
```

那么结果应该是：

```text
65
```

而不是：

```text
-65
```

因此逻辑是：

```cpp
if (appendNumber) {
    if (!text.empty()) {
        result += separator;
    }

    result += std::to_string(trailingNumber);
}
```

这里判断的是：

```cpp
!text.empty()
```

因为 separator 是否需要出现，取决于前面是否已经有字符内容。

---

# 13. 本题的核心模式

整个问题可以抽象成：

```text
element separator element separator element
```

而不是：

```text
separator element separator element separator
```

最稳定的实现习惯是：

```cpp
for (...) {
    if (不是第一个元素) {
        result += separator;
    }

    result += 当前元素;
}
```

然后如果还有额外元素：

```cpp
if (需要追加额外元素) {
    if (前面已有内容) {
        result += separator;
    }

    result += 额外元素;
}
```

---

# 14. 需要记住的几个重点

```cpp
std::string result;
```

创建空字符串。

```cpp
result += text;
```

在字符串末尾追加内容。

```cpp
result += c;
```

追加一个 `char`。

```cpp
result += std::to_string(number);
```

追加整数的文本表示。

构造带 separator 的序列时，优先采用：

```cpp
if (i > 0) {
    result += separator;
}
```

这样可以从根本上避免开头和结尾出现多余 separator。
