# C++ `const std::string &`：只读引用参数

## 1. 基本形式

当函数参数写成：

```cpp
const std::string &text
```

它同时表达了两个意思：

1. `&`：使用调用者原本的字符串，不复制
2. `const`：函数只能读取，不能修改这个字符串

例如：

```cpp
static std::string shout(const std::string &text) {
    return text + "!";
}
```

调用：

```cpp
std::string name = "Alice";
std::string result = shout(name);
```

这里 `text` 直接引用 `name`，不会额外复制一份字符串。

---

## 2. 为什么要使用引用 `&`

如果写成：

```cpp
static std::string shout(std::string text)
```

这里是 **值传递（pass by value）**。

调用函数时通常需要复制字符串：

```text
name
 │
 │ copy
 ▼
text
```

如果字符串很大，复制会产生额外开销。

而：

```cpp
const std::string &text
```

属于引用传递：

```text
name
 ▲
 │
text
```

`text` 可以理解为调用者字符串的另一个名字，因此不需要复制。

---

## 3. 为什么还需要 `const`

普通引用：

```cpp
std::string &text
```

允许函数修改调用者的变量：

```cpp
static void change(std::string &text) {
    text = "other";
}
```

例如：

```cpp
std::string name = "Alice";

change(name);

// name 现在变成 "other"
```

而：

```cpp
const std::string &text
```

禁止通过这个参数修改字符串：

```cpp
static void example(const std::string &text) {
    text = "other";   // ❌ 编译错误
}
```

因此：

```cpp
const std::string &
```

表达的是：

> 不复制对象，同时承诺只读取、不修改对象。

---

## 4. 常见参数写法比较

| 写法                        | 是否复制 | 能否修改调用者对象 | 常见用途        |
| ------------------------- | ---: | --------: | ----------- |
| `std::string text`        |    是 |         否 | 函数需要自己的副本   |
| `std::string &text`       |    否 |         是 | 函数需要修改调用者变量 |
| `const std::string &text` |    否 |         否 | 只读取字符串      |

对于函数只需要读取的 `std::string`，非常常见的写法是：

```cpp
const std::string &text
```

---

## 5. 通用形式

这个规则并不只适用于 `std::string`。

一般形式是：

```cpp
const T &name
```

例如：

```cpp
const std::vector<int> &values
const Person &person
const std::string &name
```

适合：

> 对象可能比较大，函数只读取它，不需要复制，也不需要修改。

对于 `int`、`double`、`char` 等很小的基本类型，一般直接值传递即可：

```cpp
void example(int value)
```

通常没必要写：

```cpp
void example(const int &value)
```

---

# `const` 引用为什么可以接收字符串字面量

例如：

```cpp
shout("hello");
```

`"hello"` 会产生一个临时的 `std::string` 对象。

`const` 引用可以绑定到临时对象：

```cpp
const std::string &text
```

因此这种调用是合法的。

但是普通的非 `const` 引用：

```cpp
std::string &text
```

通常不能绑定到这样的临时对象。

例如：

```cpp
void f(std::string &text);

f("hello");   // ❌
```

而：

```cpp
void f(const std::string &text);

f("hello");   // ✅
```

这是 `const T&` 作为只读参数非常重要的另一个原因。

---

# `joinWithFallback` 题目的逻辑

函数签名：

```cpp
static std::string joinWithFallback(
    const std::string &label,
    const std::string &value,
    const std::string &fallback
)
```

三个参数都必须保持：

```cpp
const std::string &
```

不能改成：

```cpp
std::string label
```

也不能改成：

```cpp
std::string &label
```

---

## 函数要求

最终结果格式：

```text
label: value
```

也就是：

```cpp
label + ": " + value
```

例如：

```cpp
joinWithFallback("size", "12", "unknown")
```

返回：

```text
size: 12
```

---

## 当 `value` 为空时

如果：

```cpp
value == ""
```

则使用：

```cpp
fallback
```

例如：

```cpp
joinWithFallback("size", "", "unknown")
```

返回：

```text
size: unknown
```

---

## 什么才算「空字符串」

这里的空字符串严格指：

```cpp
""
```

也就是长度为 0 的字符串。

因此：

```cpp
"0"
```

不是空字符串。

```cpp
" "
```

也不是空字符串，因为里面有一个空格字符。

例如：

```cpp
joinWithFallback("size", "0", "unknown")
```

结果：

```text
size: 0
```

而：

```cpp
joinWithFallback("size", " ", "unknown")
```

中间那个空格会原样保留下来。

---

## `label` 可以为空

例如：

```cpp
joinWithFallback("", "12", "unknown")
```

结果是：

```text
: 12
```

题目没有要求为空的 `label` 使用 fallback。

---

# 本题特别限制

题目禁止：

```cpp
value.empty()
```

也禁止：

```cpp
value.size()
```

因此判断空字符串时直接使用：

```cpp
value == ""
```

即可。

---

# 核心记忆

```cpp
const std::string &text
```

可以理解为：

> 给我这个字符串本身让我看看，但我不会复制它，也不会修改它。

而本题判断 fallback 的核心条件就是：

```cpp
value == ""
```
