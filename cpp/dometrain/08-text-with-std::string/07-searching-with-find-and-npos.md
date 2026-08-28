# C++ `std::string::find`：查找子字符串

## 1. `find` 是做什么的？

`std::string::find()` 用来在一个字符串中查找另一个字符串第一次出现的位置。

```cpp
std::string text = "banana";
auto position = text.find("na");
```

这里：

```text
banana
012345
```

`"na"` 第一次出现在索引 `2`：

```cpp
position == 2
```

虽然 `"na"` 后面还会再次出现：

```text
banana
    ^^
    45
```

但 `find()` 默认只返回**第一次匹配的位置**。

---

# 2. `find` 返回的不是 `bool`

一个容易犯的错误是把 `find()` 当成“找到了 / 没找到”的布尔判断。

实际上：

```cpp
text.find(target)
```

返回的是：

> 子字符串第一次出现的索引位置。

例如：

```cpp
std::string text = "abcdef";

text.find("ab");  // 0
text.find("cd");  // 2
text.find("ef");  // 4
```

因此它不是简单返回：

```cpp
true
false
```

---

# 3. 找不到时返回 `std::string::npos`

如果目标字符串不存在：

```cpp
std::string text = "banana";

auto position = text.find("xyz");
```

不会返回 `-1`。

而是返回：

```cpp
std::string::npos
```

所以标准的判断方式是：

```cpp
if (position == std::string::npos) {
    // 没找到
}
```

找到则可以写：

```cpp
if (position != std::string::npos) {
    // 找到了
}
```

---

# 4. 为什么不是 `-1`？

`find()` 返回的类型通常是：

```cpp
std::string::size_type
```

它本质上是一个**无符号整数类型**，类似于：

```cpp
size_t
```

无符号整数不能正常表示负数，因此字符串库没有使用 `-1` 表示“找不到”。

标准库定义了一个特殊值：

```cpp
std::string::npos
```

它通常就是该无符号整数类型可以表示的最大值。

所以：

```cpp
position < 0
```

这种判断是错误的。

例如：

```cpp
auto position = text.find("abc");

if (position < 0) {  // 错误
}
```

即使没有找到，这个条件通常也不会成立，因为 `position` 是无符号数。

正确写法永远是：

```cpp
if (position == std::string::npos)
```

---

# 5. 为什么推荐使用 `auto`

可以直接写：

```cpp
auto position = text.find(target);
```

这样编译器会自动推断：

```cpp
std::string::size_type
```

比手动写：

```cpp
std::string::size_type position = text.find(target);
```

更加简单。

这里不要一开始就写：

```cpp
int position = text.find(target);
```

因为如果没有找到：

```cpp
std::string::npos
```

会被强行转换成 `int`，可能产生不可靠的结果。

---

# 6. 如果最后必须返回 `int`

假设函数要求：

```cpp
static int firstIndexOr(
    const std::string &text,
    const std::string &target,
    int missing
)
```

要求：

* 找到 `target` → 返回第一次出现的位置
* 没找到 → 返回参数 `missing`

那么应该：

```cpp
auto position = text.find(target);

if (position == std::string::npos) {
    return missing;
}

return static_cast<int>(position);
```

关键顺序是：

```text
find
 ↓
先判断是不是 npos
 ↓
确认找到以后
 ↓
再转换成 int
```

不要这样：

```cpp
int position = static_cast<int>(text.find(target));
```

因为这样会先把 `npos` 转换成 `int`。

---

# 7. `missing` 是调用者指定的

函数参数中有：

```cpp
int missing
```

意味着：

> 找不到时返回什么，由调用者决定。

例如：

```cpp
firstIndexOr("banana", "xyz", -1);
```

返回：

```cpp
-1
```

但：

```cpp
firstIndexOr("banana", "xyz", 100);
```

返回：

```cpp
100
```

所以不能在函数里面固定写：

```cpp
return -1;
```

因为题目明确要求返回：

```cpp
missing
```

---

# 8. `missing` 不一定能区分“找到”和“没找到”

例如：

```cpp
firstIndexOr("banana", "xyz", 3);
```

因为 `"xyz"` 不存在，所以返回：

```cpp
3
```

但是 `3` 本身也是一个合法的字符串索引。

因此单独看到返回值：

```cpp
3
```

并不能判断：

* 是真的匹配在索引 `3`
* 还是没找到，于是返回了 `missing == 3`

这是这个函数接口本身的设计决定。

---

# 9. 不要直接把 `find()` 放进 `if`

下面这种代码非常危险：

```cpp
if (text.find(target)) {
    ...
}
```

因为 C++ 会把返回的位置转换为 `bool`。

### 情况一：匹配发生在位置 0

```cpp
std::string text = "banana";

text.find("ba");   // 0
```

而：

```cpp
static_cast<bool>(0)
```

是：

```cpp
false
```

于是：

```cpp
if (text.find("ba"))
```

竟然会认为没有匹配。

---

### 情况二：完全没有匹配

```cpp
text.find("xyz")
```

返回：

```cpp
std::string::npos
```

这是一个很大的非零整数。

而非零整数转换成 `bool` 是：

```cpp
true
```

于是：

```cpp
if (text.find("xyz"))
```

反而可能进入 `if`。

所以这种代码几乎把逻辑完全颠倒了。

正确写法：

```cpp
if (text.find(target) != std::string::npos) {
    // 找到了
}
```

---

# 10. `find()` 区分大小写

例如：

```cpp
std::string text = "banana";
```

下面能找到：

```cpp
text.find("na");
```

但：

```cpp
text.find("NA");
```

返回：

```cpp
std::string::npos
```

因为：

```text
n != N
a != A
```

`std::string::find()` 默认进行大小写敏感匹配。

---

# 11. 空字符串的特殊情况

这是 `find()` 很值得记住的边界情况。

## 空 `target`

```cpp
std::string text = "banana";

text.find("");
```

结果：

```cpp
0
```

因为空字符串被认为可以从位置 `0` 开始匹配。

甚至：

```cpp
std::string text = "";

text.find("");
```

同样返回：

```cpp
0
```

所以：

```text
空 target → 0
```

---

## 空 `text` + 非空 `target`

例如：

```cpp
std::string text = "";

text.find("abc");
```

找不到，因此返回：

```cpp
std::string::npos
```

在 `firstIndexOr()` 中就应该返回：

```cpp
missing
```

---

# 12. `target` 比 `text` 长

例如：

```cpp
std::string text = "abc";

text.find("abcdef");
```

显然不可能完整匹配，因此：

```cpp
std::string::npos
```

所以 `firstIndexOr()` 返回：

```cpp
missing
```

---

# 13. 本题需要记住的核心模板

查找字符串时最常见的代码模板：

```cpp
auto position = text.find(target);

if (position == std::string::npos) {
    // 没找到
} else {
    // position 就是第一次匹配的位置
}
```

如果需要返回 `int`：

```cpp
auto position = text.find(target);

if (position == std::string::npos) {
    return missing;
}

return static_cast<int>(position);
```

---

# 14. 常见错误总结

错误：

```cpp
if (text.find(target))
```

原因：索引 `0` 会被当成 `false`，`npos` 会被当成 `true`。

---

错误：

```cpp
if (position < 0)
```

原因：`find()` 返回无符号整数，不应该使用负数判断。

---

错误：

```cpp
return -1;
```

原因：本题要求使用调用者传进来的：

```cpp
missing
```

---

错误：

```cpp
int position = static_cast<int>(text.find(target));
```

原因：可能在判断之前把 `npos` 转换成 `int`。

---

正确：

```cpp
auto position = text.find(target);

if (position == std::string::npos) {
    return missing;
}

return static_cast<int>(position);
```

---

# 15. 一句话记忆

> `find()` 返回第一次匹配的索引；找不到返回 `std::string::npos`。一定先和 `npos` 比较，确认找到后，再把位置转换成需要的类型。
