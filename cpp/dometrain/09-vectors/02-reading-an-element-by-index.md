# C++ `std::vector`：通过下标访问元素与边界检查

## 1. 使用 `[]` 访问元素

`std::vector` 可以通过方括号 `[]` 按下标访问元素：

```cpp
std::vector<int> v = {10, 20, 30};

int first = v[0];
```

`vector` 的下标从 **0** 开始：

```text
元素:   10   20   30
下标:    0    1    2
```

因此：

```cpp
v[0]  // 10
v[1]  // 20
v[2]  // 30
```

---

## 2. 获取最后一个元素

`v.size()` 返回元素数量。

例如：

```cpp
std::vector<int> v = {10, 20, 30};

v.size();  // 3
```

但最后一个元素的下标是 `2`，因此：

```cpp
最后一个元素的下标 = v.size() - 1
```

所以可以写：

```cpp
int last = v[v.size() - 1];
```

完整示例：

```cpp
std::vector<int> v = {10, 20, 30};

int first = v[0];                 // 10
int last = v[v.size() - 1];       // 30
```

可以记住：

```text
元素数量 = size()
最后下标 = size() - 1
```

---

## 3. `operator[]` 不会检查下标是否合法

这是一个非常重要的区别：

```cpp
v[index]
```

不会自动检查 `index` 是否越界。

例如：

```cpp
std::vector<int> v = {10, 20, 30};

int x = v[100];   // 错误：越界访问
```

这种行为属于 **Undefined Behavior（未定义行为）**。

也就是说，C++ 标准并不保证会发生什么。

可能表现为：

* 得到一个看似正常的数字
* 得到垃圾值
* 程序崩溃
* 某些情况下暂时看起来“没问题”

因此，**不能根据某个平台上“越界后返回 0”来认为这是合法行为**。

正确理解应该是：

> 一旦 `vector` 下标越界，程序行为就是未定义的，结果不可信。

---

## 4. 空 `vector` 特别危险

考虑：

```cpp
std::vector<int> v;
```

此时：

```cpp
v.size() == 0
v.empty() == true
```

下面两个访问都是错误的：

```cpp
v[0];
v[v.size() - 1];
```

尤其第二种还有一个容易忽略的问题。

---

## 5. 为什么空 vector 的 `size() - 1` 不是 `-1`

`size()` 的返回类型不是普通的有符号 `int`，而是类似：

```cpp
std::size_t
```

它是一个**无符号整数类型**。

因此：

```cpp
v.size()
```

如果等于：

```cpp
0
```

那么：

```cpp
v.size() - 1
```

不会得到：

```text
-1
```

而会发生无符号整数下溢，变成一个非常大的正整数。

概念上类似：

```text
0 - 1
→ 一个巨大的 unsigned 数值
```

于是：

```cpp
v[v.size() - 1]
```

实际上是在访问一个远远超出范围的位置。

因此一定要先确认 vector 非空。

---

## 6. 使用 `empty()` 判断是否为空

`empty()` 用于判断 vector 中有没有元素：

```cpp
v.empty()
```

返回：

```cpp
true   // 没有元素
false  // 至少有一个元素
```

例如：

```cpp
if (!v.empty()) {
    int last = v[v.size() - 1];
}
```

这里：

```cpp
!v.empty()
```

表示：

> vector 不是空的。

这时才能安全计算：

```cpp
v.size() - 1
```

---

## 7. 判断必须发生在访问之前

不要先访问再判断。

错误：

```cpp
int first = v[0];

if (!v.empty()) {
    // ...
}
```

因为如果 `v` 是空的，第一行就已经发生越界访问。

正确：

```cpp
if (!v.empty()) {
    int first = v[0];
    int last = v[v.size() - 1];
}
```

核心原则：

> **先证明下标有效，再使用 `[]`。**

---

## 8. 单元素 vector 的情况

例如：

```cpp
std::vector<int> v = {5};
```

此时：

```cpp
v.size() == 1
```

第一个元素：

```cpp
v[0]
```

最后一个元素：

```cpp
v[v.size() - 1]
```

也就是：

```cpp
v[0]
```

所以：

```cpp
first == 5
last == 5
```

这是完全正常的。

---

## 9. `[]` 和 `.at()` 的区别

标准 C++ 中：

```cpp
v[index]
```

不会进行边界检查。

而：

```cpp
v.at(index)
```

会检查下标。

例如：

```cpp
std::vector<int> v = {10, 20, 30};

v.at(1);   // 20
v.at(100); // 抛出 std::out_of_range
```

理论上可以使用：

```cpp
try {
    int x = v.at(100);
} catch (const std::out_of_range& e) {
    // 处理越界
}
```

不过如果题目环境不允许异常处理，或者 grader 会直接终止程序，那么仍然应该主动进行边界检查。

对于当前问题，最简单的方法就是：

```cpp
if (values.empty()) {
    return {};
}
```

然后再访问：

```cpp
values[0]
values[values.size() - 1]
```

---

# 核心总结

访问 `vector` 时：

```cpp
values[index]
```

必须保证：

```cpp
index < values.size()
```

获取首尾元素时：

```cpp
if (!values.empty()) {
    int first = values[0];
    int last = values[values.size() - 1];
}
```

尤其要记住：

```text
vector 下标从 0 开始
最后下标 = size() - 1
[] 不做边界检查
空 vector 不能访问 [0]
size() 是无符号数
空 vector 的 size() - 1 会变成巨大正数
```
