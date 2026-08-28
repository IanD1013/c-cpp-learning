# C++ `<cctype>`：字符分类与大小写转换

`<cctype>` 提供了一组用于**判断单个字符类型**以及**转换字符大小写**的函数。

```cpp
#include <cctype>
```

---

## 1. 常用字符判断函数

常见函数包括：

| 函数                | 判断内容                  |
| ----------------- | --------------------- |
| `std::isdigit(c)` | 是否为数字字符 `'0'` ~ `'9'` |
| `std::isalpha(c)` | 是否为英文字母               |
| `std::isupper(c)` | 是否为大写字母               |

例如：

```cpp
int digits = 0;

if (std::isdigit(static_cast<unsigned char>(c))) {
    digits += 1;
}
```

### 返回值

这些函数返回的是 `int`：

* `0`：条件不成立
* 非 `0`：条件成立

因此直接写：

```cpp
if (std::isdigit(...)) {
}
```

不要写：

```cpp
if (std::isdigit(...) == true) {
}
```

因为函数保证的是“非 0 表示 true”，并不保证返回值一定恰好是 `1`。

---

## 2. 为什么必须转换成 `unsigned char`

推荐始终写成：

```cpp
std::isdigit(static_cast<unsigned char>(c))
```

而不是：

```cpp
std::isdigit(c)
```

原因是 `<cctype>` 中这些函数要求参数满足：

```text
EOF
或
unsigned char 能表示的值（通常是 0~255）
```

但是普通 `char` 在很多系统上实际上表现为有符号类型。

例如某个字节值是：

```text
200
```

如果 `char` 是 signed，它可能被解释成负数。

把负数直接传给：

```cpp
std::isdigit(c)
std::isalpha(c)
std::isupper(c)
std::tolower(c)
```

可能产生 **Undefined Behavior（未定义行为）**。

所以安全写法是：

```cpp
static_cast<unsigned char>(c)
```

即：

```cpp
std::isalpha(static_cast<unsigned char>(c))
```

这是一个值得养成的习惯：

> 调用 `<cctype>` 函数时，如果参数来自 `char`，先转换为 `unsigned char`。

---

## 3. `std::tolower`

`std::tolower` 用来把字符转换成小写：

```cpp
char lower =
    static_cast<char>(
        std::tolower(static_cast<unsigned char>(c))
    );
```

例如：

```cpp
'A' -> 'a'
'D' -> 'd'
'x' -> 'x'
'3' -> '3'
```

注意：

```cpp
std::tolower(c);
```

**不会修改 `c` 本身。**

错误：

```cpp
std::tolower(c);
// c 没变
```

正确：

```cpp
c = static_cast<char>(
    std::tolower(static_cast<unsigned char>(c))
);
```

---

## 4. 为什么 `tolower` 还要转换回 `char`

`std::tolower` 的返回类型是：

```cpp
int
```

所以如果后面需要和字符比较，通常写：

```cpp
char lower =
    static_cast<char>(
        std::tolower(static_cast<unsigned char>(kind))
    );

if (lower == 'd') {
}
```

可以把过程理解为：

```text
char
 ↓
unsigned char
 ↓
传入 std::tolower
 ↓
得到 int
 ↓
转换回 char
```

---

# `countKind` 的需求

函数签名：

```cpp
static int countKind(const std::string &text, char kind)
```

功能：统计字符串中特定类型字符的数量。

`kind` 不区分大小写：

| `kind`        | 含义   |
| ------------- | ---- |
| `'d'` / `'D'` | 数字   |
| `'l'` / `'L'` | 字母   |
| `'u'` / `'U'` | 大写字母 |

例如：

```cpp
countKind("ABC123", 'd')
```

返回：

```text
3
```

```cpp
countKind("ABC123", 'l')
```

返回：

```text
3
```

```cpp
countKind("AbC123", 'u')
```

返回：

```text
2
```

其他 `kind`：

```cpp
countKind("ABC123", 'x')
```

返回：

```text
-1
```

这里 `-1` 表示调用者传入了不支持的类型。

---

## 空字符串和匹配数量为 0

如果字符串中没有匹配字符：

```cpp
countKind("abc", 'd')
```

返回：

```text
0
```

空字符串：

```cpp
countKind("", 'd')
```

也返回：

```text
0
```

因为这是合法的统计结果。

因此：

```text
-1 = kind 非法
0  = kind 合法，但找到了 0 个
```

这是典型的 **sentinel value（哨兵值）**设计。

---

# 核心写法总结

看到：

```cpp
std::isdigit(c)
```

最好形成条件反射，写成：

```cpp
std::isdigit(static_cast<unsigned char>(c))
```

而大小写转换通常写成：

```cpp
char lower =
    static_cast<char>(
        std::tolower(static_cast<unsigned char>(c))
    );
```

本题真正练习的是三个知识点：

1. 用 `<cctype>` 判断字符类型
2. 正确处理 `char` → `unsigned char`
3. 遍历 `std::string` 并进行分类统计
