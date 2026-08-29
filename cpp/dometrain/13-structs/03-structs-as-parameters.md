# C++：用 `const` 引用把结构体传给函数

## 1. 为什么结构体参数通常使用 `const T &`

假设有一个结构体：

```cpp
struct Box {
    std::string tag;
    int side;
};
```

如果一个函数只需要读取 `Box`，推荐这样写：

```cpp
static int areaOf(const Box &box) {
    return box.side * box.side;
}
```

这里：

```cpp
const Box &box
```

可以拆成两部分理解：

* `&`：使用引用，不复制整个 `Box`
* `const`：函数只能读取 `Box`，不能修改它

---

## 2. 按值传递 vs `const` 引用

### 按值传递

```cpp
static int areaOf(Box box) {
    return box.side * box.side;
}
```

调用时会复制整个结构体。

如果结构体里包含：

```cpp
std::string
std::vector
std::map
```

之类的对象，复制可能还要复制它们内部的数据。

例如：

```cpp
Box box{"large-box", 10};

areaOf(box);
```

使用按值传递时，大致相当于：

```cpp
Box copy = box;
```

然后函数操作这个副本。

---

### `const` 引用传递

```cpp
static int areaOf(const Box &box) {
    return box.side * box.side;
}
```

不会创建新的 `Box`。

`box` 只是原对象的另一个名字：

```text
caller 的 Box
      ↑
      │
const Box &box
```

因此这是处理“只读的大对象参数”时非常常见的写法。

---

## 3. `const` 的作用

有：

```cpp
static int areaOf(const Box &box)
```

之后，可以读取：

```cpp
box.side
box.tag
```

但不能修改：

```cpp
box.side = 0;   // 编译错误
```

也不能调用会修改对象的成员函数。

因此：

```cpp
const Box &
```

表达的意思非常清楚：

> 我需要访问这个对象，但不会修改它。

---

## 4. 一个常用经验

对于简单的小类型，例如：

```cpp
int
double
char
bool
```

通常直接按值传递：

```cpp
static int square(int x)
```

对于较大的对象，例如：

```cpp
std::string
std::vector<int>
struct Reading
struct Student
```

如果只读取，通常使用：

```cpp
const T &value
```

例如：

```cpp
static int labelLength(const std::string &label)
```

或者：

```cpp
static int fahrenheitOf(const Reading &reading)
```

---

# 5. `static` 成员函数

如果函数写在：

```cpp
class Solution
```

里面，并且需要从其他 `static` 函数中直接调用，那么辅助函数也应该写成 `static`：

```cpp
class Solution {
public:
    static int helper(...) {
        ...
    }

    static int solve(...) {
        return helper(...);
    }
};
```

如果辅助函数没有 `static`：

```cpp
int helper(...)
```

它属于某个具体的 `Solution` 对象。

通常需要：

```cpp
Solution solution;
solution.helper(...);
```

才能调用。

但是在线编程题的入口函数通常本身就是：

```cpp
static
```

没有 `Solution` 对象，因此辅助函数也应该声明成：

```cpp
static
```

---

# 6. 摄氏度转华氏度

公式是：

```text
F = C × 9 / 5 + 32
```

代码：

```cpp
static int fahrenheitOf(const Reading &reading) {
    return reading.celsius * 9 / 5 + 32;
}
```

注意这里全部使用 `int`。

因此整数除法会直接舍去小数部分。

例如：

```cpp
3 * 9 / 5 + 32
```

计算过程：

```text
3 × 9 = 27
27 / 5 = 5
5 + 32 = 37
```

所以结果是：

```text
37
```

而不是数学上的：

```text
37.4
```

---

## 7. 整数除法需要特别注意

C++ 中：

```cpp
27 / 5
```

如果两边都是 `int`，结果就是：

```cpp
5
```

小数部分被截掉。

再例如：

```cpp
-45 / 5
```

得到：

```cpp
-9
```

所以：

```cpp
-5 * 9 / 5 + 32
```

得到：

```text
23
```

---

# 8. 获取字符串长度

假设：

```cpp
struct Reading {
    std::string label;
    int celsius;
};
```

可以使用：

```cpp
reading.label.size()
```

得到字符数量。

例如：

```cpp
"room".size()
```

结果为：

```text
4
```

空字符串：

```cpp
"".size()
```

结果为：

```text
0
```

---

## 9. 为什么需要 `static_cast<int>`

`std::string::size()` 返回的不是 `int`，而是：

```cpp
std::size_t
```

如果函数要求返回：

```cpp
int
```

可以显式转换：

```cpp
static_cast<int>(reading.label.size())
```

例如：

```cpp
static int labelLengthOf(const Reading &reading) {
    return static_cast<int>(reading.label.size());
}
```

这种写法明确告诉编译器：

> 我知道 `size()` 返回的类型不是 `int`，这里有意把它转换成 `int`。

---

# 10. 本题真正考察的知识点

这道题重点并不是华氏度转换，而是：

```cpp
const Reading &reading
```

也就是：

> 把整个结构体以只读引用的方式传给辅助函数。

正确：

```cpp
static int fahrenheitOf(const Reading &reading)
```

错误：

```cpp
static int fahrenheitOf(Reading reading)
```

因为这是按值复制。

错误：

```cpp
static int fahrenheitOf(Reading &reading)
```

因为这允许函数修改原对象。

最合适的是：

```cpp
static int fahrenheitOf(const Reading &reading)
```

---

# 11. 不要只传结构体中的某个成员

如果题目要求练习“把整个结构体传给函数”，那么应该：

```cpp
fahrenheitOf(reading)
```

而不是：

```cpp
fahrenheitOf(reading.celsius)
```

前者传入的是：

```cpp
Reading
```

整个对象。

辅助函数自己决定需要读取哪个成员：

```cpp
reading.celsius
```

这种设计也更容易扩展。

例如以后计算逻辑同时需要：

```cpp
reading.label
reading.celsius
```

函数签名不需要改变。

---

# 12. 推荐记忆方式

看到这种场景：

```text
函数需要一个 struct / string / vector
+
函数只读取它
+
不希望复制
```

优先想到：

```cpp
const T &
```

例如：

```cpp
const Reading &reading
const Box &box
const std::string &name
const std::vector<int> &numbers
```

可以把它记成：

```text
const = 不改
&     = 不复制
```
