# C++ `std::vector`：使用索引遍历元素

## 1. 用下标遍历 `vector`

`std::vector` 中的每个元素都有一个下标（index）。

下标从 **0** 开始：

```cpp
std::vector<int> v = {3, 1, 4};
```

对应关系是：

```text
下标:   0  1  2
元素:   3  1  4
```

因此可以使用普通的 `for` 循环遍历：

```cpp
#include <cstddef>
#include <iostream>
#include <vector>

std::vector<int> v = {3, 1, 4};

for (std::size_t i = 0; i < v.size(); ++i) {
    std::cout << v[i] << " ";
}
```

输出：

```text
3 1 4
```

---

## 2. 为什么循环条件必须是 `i < v.size()`

这是索引遍历中最重要的一点。

假设：

```cpp
v.size() == 3
```

合法下标只有：

```text
0
1
2
```

因此循环应该执行：

```text
i = 0
i = 1
i = 2
```

当：

```cpp
i == 3
```

时就必须停止。

所以正确条件是：

```cpp
i < v.size()
```

完整结构：

```cpp
for (std::size_t i = 0; i < v.size(); ++i) {
    // 使用 v[i]
}
```

可以记成：

```text
合法下标范围：

0 <= i < v.size()
```

注意最后一个合法下标永远是：

```cpp
v.size() - 1
```

而不是：

```cpp
v.size()
```

---

## 3. 为什么不能写 `<=`

错误写法：

```cpp
for (std::size_t i = 0; i <= v.size(); ++i) {
    std::cout << v[i];
}
```

假设：

```cpp
v = {3, 1, 4}
```

循环会尝试访问：

```text
v[0]
v[1]
v[2]
v[3]   // 错误
```

但是 `v[3]` 根本不存在。

这叫做：

```text
out-of-bounds access
越界访问
```

`operator[]`：

```cpp
v[i]
```

不会自动检查下标是否合法。

越界访问属于 **Undefined Behavior（未定义行为）**。

程序可能：

* 崩溃；
* 输出奇怪的数据；
* 看起来暂时正常；
* 在不同机器上表现不同。

因此：

```cpp
i < v.size()
```

不仅是一种习惯，它实际上保证了：

```text
循环体中的每一个 v[i] 都是合法元素。
```

---

## 4. 空 `vector` 也能正常工作

例如：

```cpp
std::vector<int> v;
```

此时：

```cpp
v.size() == 0
```

循环：

```cpp
for (std::size_t i = 0; i < v.size(); ++i) {
    // ...
}
```

第一次判断就是：

```cpp
0 < 0
```

结果为：

```cpp
false
```

所以循环一次都不会执行。

这很好，因为空 `vector` 本来就没有任何元素可以访问。

因此通常不需要额外写：

```cpp
if (v.empty()) {
    ...
}
```

普通的 `< size()` 循环天然就能正确处理空 `vector`。

---

## 5. 为什么索引变量通常写成 `std::size_t`

`vector::size()` 返回的类型是：

```cpp
std::size_t
```

所以通常写：

```cpp
for (std::size_t i = 0; i < v.size(); ++i)
```

而不是：

```cpp
for (int i = 0; i < v.size(); ++i)
```

`std::size_t` 是一种用于表示：

```text
大小
数量
数组/容器索引
```

的无符号整数类型。

它不会表示负数。

它定义在：

```cpp
#include <cstddef>
```

中。

---

## 6. `int` 能不能作为索引？

很多情况下下面的代码确实可以运行：

```cpp
for (int i = 0; i < v.size(); ++i) {
    std::cout << v[i];
}
```

但是：

```cpp
i
```

是：

```text
signed integer
```

而：

```cpp
v.size()
```

返回：

```text
unsigned integer
```

因此编译器可能给出：

```text
signed/unsigned comparison warning
```

所以更合适的写法是：

```cpp
std::size_t i
```

---

# 使用循环计算 `vector` 元素总和

假设我们要计算：

```cpp
{3, 1, 4}
```

所有元素的和。

正确思路是先创建一个累加器：

```cpp
int total = 0;
```

然后遍历元素：

```cpp
total += values[i];
```

例如：

```cpp
int total = 0;

for (std::size_t i = 0; i < values.size(); ++i) {
    total += values[i];
}
```

这里：

```cpp
total += values[i];
```

等价于：

```cpp
total = total + values[i];
```

执行过程：

```text
开始：

total = 0

读取 3：
total = 0 + 3 = 3

读取 1：
total = 3 + 1 = 4

读取 4：
total = 4 + 4 = 8
```

最终：

```text
total == 8
```

---

## 7. 累加变量必须声明在循环外面

正确：

```cpp
int total = 0;

for (...) {
    total += values[i];
}

return total;
```

错误：

```cpp
for (...) {
    int total = 0;
    total += values[i];
}

return total;
```

这里存在两个问题。

### 问题 1：作用域

在：

```cpp
{
    int total = 0;
}
```

中声明的 `total` 只存在于这对 `{}` 内。

循环结束之后：

```cpp
return total;
```

已经找不到这个变量，所以代码无法编译。

---

### 问题 2：每次循环都会重新变成 0

即使不考虑作用域：

```cpp
for (...) {
    int total = 0;
}
```

每执行一次循环都会重新创建：

```cpp
total = 0
```

因此无法保存之前累加的结果。

累加器必须在循环开始前创建一次：

```cpp
int total = 0;
```

---

## 8. `=` 和 `+=` 的区别

错误：

```cpp
total = values[i];
```

这会不断覆盖之前的值。

例如：

```cpp
values = {3, 1, 4}
```

过程：

```text
total = 3
total = 1
total = 4
```

最终：

```text
total == 4
```

只剩最后一个元素。

而正确的是：

```cpp
total += values[i];
```

即：

```cpp
total = total + values[i];
```

最终：

```text
3 + 1 + 4 = 8
```

---

# `const std::vector<int>&` 是什么意思？

方法参数：

```cpp
const std::vector<int> &values
```

可以拆开理解。

### `std::vector<int>`

表示：

```text
一个保存 int 的 vector
```

### `&`

表示通过引用传递：

```cpp
std::vector<int> &values
```

这样不会为了调用函数而复制整个 `vector`。

如果 `vector` 很大，这通常更加高效。

### `const`

表示函数承诺不会修改这个 `vector`：

```cpp
const std::vector<int> &values
```

所以函数可以：

```cpp
values[i]
values.size()
```

但不能：

```cpp
values.push_back(10);
values[0] = 100;
```

因此对于“只读取、不修改”的函数，常见写法是：

```cpp
const std::vector<int>&
```

---

# 核心模式

使用索引遍历 `vector` 时，可以记住这个模板：

```cpp
for (std::size_t i = 0; i < values.size(); ++i) {
    // values[i]
}
```

其中最重要的是：

```text
起点：i = 0

条件：i < values.size()

当前元素：values[i]

更新：++i
```

合法索引始终满足：

```text
0 <= i < values.size()
```

这是避免 `vector` 越界访问的核心规则。
