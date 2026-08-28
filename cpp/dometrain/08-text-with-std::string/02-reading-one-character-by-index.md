# C++ `std::string`：按下标访问字符与边界检查

## 1. 使用 `s[i]` 访问字符串中的字符

`std::string` 可以通过下标访问某个位置上的字符：

```cpp
std::string s = "hey";

char first = s[0];
char last = s[s.size() - 1];
```

结果：

```cpp
first == 'h'
last == 'y'
```

字符串下标从 **0** 开始：

```text
字符串： h   e   y
下标：   0   1   2
```

如果字符串长度为：

```cpp
s.size() == 3
```

那么：

* 第一个字符下标：`0`
* 最后一个字符下标：`s.size() - 1`
* 合法下标范围：

```cpp
0 <= index < s.size()
```

---

## 2. `s[i]` 返回的是 `char`

通过下标得到的是单个字符：

```cpp
char c = s[0];
```

这里 `c` 的类型是：

```cpp
char
```

因此字符使用单引号：

```cpp
'h'
```

而不是：

```cpp
"h"
```

两者类型不同：

```cpp
'h'   // char
"h"   // 字符串字面量
```

---

## 3. `operator[]` 不会自动进行边界检查

例如：

```cpp
std::string s = "abc";

char c = s[100];
```

`100` 明显已经超出了字符串范围。

但是 `s[index]` 本身不会帮你检查下标是否合法。

越界访问属于未定义行为，可能：

* 得到奇怪的数据
* 看起来暂时正常
* 程序崩溃
* 在不同环境下产生不同结果

因此不能依赖越界访问之后的结果来判断是否失败。

正确做法是：

> **先检查 index，再访问字符串。**

---

## 4. 合法下标的判断条件

如果参数：

```cpp
int index
```

那么合法下标需要同时满足：

```cpp
index >= 0
```

和：

```cpp
index < text.size()
```

也就是：

```cpp
0 <= index < text.size()
```

在 C++ 中通常写成：

```cpp
if (index >= 0 && index < static_cast<int>(text.size())) {
    return text[index];
}
```

不过有时为了让错误情况更明显，也可以先处理非法情况：

```cpp
if (index < 0) {
    return '?';
}

if (index >= static_cast<int>(text.size())) {
    return '?';
}

return text[index];
```

这种写法非常容易阅读：

1. 先排除负数
2. 再排除超过字符串长度的下标
3. 剩下的下标一定合法
4. 最后才访问 `text[index]`

---

## 5. 为什么需要 `static_cast<int>(text.size())`

这是这道题里非常重要的一点。

`text.size()` 返回的不是 `int`，而是一个**无符号整数类型**，通常是：

```cpp
std::string::size_type
```

它通常等价或类似于：

```cpp
std::size_t
```

这是 unsigned 类型。

而题目中的：

```cpp
index
```

是：

```cpp
int
```

属于 signed 类型。

---

## 6. signed 和 unsigned 混合比较的问题

考虑：

```cpp
int index = -1;

if (index < text.size()) {
    ...
}
```

看起来应该是在比较：

```cpp
-1 < 5
```

显然应该是 `true`。

但因为 `text.size()` 是 unsigned，比较时 `index` 可能先被转换成 unsigned。

于是：

```cpp
-1
```

会变成一个非常大的正整数。

概念上类似：

```text
-1
↓ 转换成 unsigned
4294967295
```

于是实际上可能变成类似：

```cpp
4294967295 < 5
```

结果自然是 `false`。

虽然这个结果碰巧阻止了访问，但这是因为隐式类型转换，而不是因为代码清晰地表达了“负数下标非法”。

---

## 7. 更清晰的写法：统一使用 `int`

题目要求：

```cpp
static_cast<int>(text.size())
```

例如：

```cpp
if (index >= static_cast<int>(text.size())) {
    return '?';
}
```

这样比较双方都是：

```cpp
int
```

于是：

```cpp
index = -1
text.size() = 5
```

就是真正按照：

```cpp
-1 >= 5
```

来理解。

代码的逻辑更明确。

---

## 8. 为什么还必须单独检查 `index < 0`

即使写了：

```cpp
if (index >= static_cast<int>(text.size())) {
    return '?';
}
```

仍然不能处理：

```cpp
index = -1;
```

因为：

```cpp
-1 >= 5
```

是：

```cpp
false
```

如果接下来直接：

```cpp
return text[index];
```

还是会发生越界访问。

因此必须单独处理：

```cpp
if (index < 0) {
    return '?';
}
```

完整逻辑：

```cpp
if (index < 0) {
    return '?';
}

if (index >= static_cast<int>(text.size())) {
    return '?';
}

return text[index];
```

---

## 9. 空字符串不需要特殊处理

假设：

```cpp
std::string text = "";
```

那么：

```cpp
text.size() == 0
```

如果：

```cpp
index = 0
```

检查：

```cpp
index >= static_cast<int>(text.size())
```

相当于：

```cpp
0 >= 0
```

结果为：

```cpp
true
```

因此直接返回错误标记。

所以不需要额外写：

```cpp
if (text.empty())
```

已有的边界检查自然就覆盖了空字符串。

---

## 10. 使用 `'?'` 作为 sentinel value

题目要求非法下标返回：

```cpp
'?'
```

例如：

```cpp
characterAt("abc", -1)   // '?'
characterAt("abc", 3)    // '?'
characterAt("abc", 100)  // '?'
```

这里 `'?'` 相当于一个 **sentinel value（哨兵值 / 特殊标记值）**：

```text
'?' = 没有合法字符
```

但这种设计存在一个问题：

```cpp
characterAt("a?b", 1)
```

合法结果本身也是：

```cpp
'?'
```

因此调用方无法只根据返回值判断：

```text
到底是真的读到了 '?'，
还是因为 index 非法返回了 '?'？
```

所以这种 sentinel 方案只适合：

> 调用方能够保证正常数据不会产生该特殊值的情况。

更现代的 C++ 中，如果需要明确区分“有值”和“没有值”，可以考虑：

```cpp
std::optional<char>
```

不过这道题明确要求返回 `char`，因此按要求使用 `'?'`。

---

# 核心总结

访问字符串字符：

```cpp
text[index]
```

合法下标范围：

```cpp
0 <= index < text.size()
```

如果 `index` 是 `int`，推荐显式处理：

```cpp
index < 0
```

以及：

```cpp
index >= static_cast<int>(text.size())
```

最后只有在已经证明下标合法之后，才执行：

```cpp
text[index]
```

典型结构：

```cpp
if (index < 0) {
    // 非法
}

if (index >= static_cast<int>(text.size())) {
    // 非法
}

// 到这里一定合法
return text[index];
```

最重要的原则是：

> **永远先证明下标合法，再访问容器。**
