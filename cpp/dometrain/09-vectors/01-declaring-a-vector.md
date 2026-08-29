# C++ `std::vector` 基础

## 1. `std::vector` 是什么

`std::vector` 可以理解为一种**长度可以动态变化的数组**。

它用来连续保存一组**相同类型**的数据，例如：

```cpp
#include <vector>

std::vector<int> scores = {1, 2, 3};
```

这里：

```cpp
std::vector<int>
```

表示变量的类型：

* `std::vector`：使用 C++ 标准库中的 vector
* `<int>`：vector 中每个元素都是 `int`
* `scores`：变量名

因此：

```cpp
std::vector<int> scores;
```

可以理解为：

> 声明一个名叫 `scores` 的 vector，它只能保存 `int`。

例如这些是合法的：

```cpp
std::vector<int> numbers = {10, 20, 30};

std::vector<double> prices = {1.5, 2.8, 3.2};

std::vector<std::string> names = {"Alice", "Bob"};
```

一个 vector 中的元素类型必须统一。

---

## 2. 使用 `vector` 必须包含 `<vector>`

`std::vector` 定义在标准库头文件：

```cpp
<vector>
```

因此需要：

```cpp
#include <vector>
```

例如：

```cpp
#include <vector>

int main()
{
    std::vector<int> scores = {1, 2, 3};
}
```

如果没有：

```cpp
#include <vector>
```

编译器可能无法识别 `std::vector`。

这和使用：

```cpp
std::string
```

通常需要：

```cpp
#include <string>
```

是同样的道理。

---

## 3. 使用 `{}` 初始化 vector

下面的代码：

```cpp
std::vector<int> scores = {1, 2, 3};
```

会创建：

```text
scores
┌───┬───┬───┐
│ 1 │ 2 │ 3 │
└───┴───┴───┘
```

元素按照 `{}` 中给出的顺序保存。

也可以创建空 vector：

```cpp
std::vector<int> scores = {};
```

或者更常见地写：

```cpp
std::vector<int> scores;
```

此时：

```text
scores
[]
```

这是一个完全正常的 vector，只是当前没有任何元素。

---

## 4. `.size()`：获取元素数量

vector 自己会记录当前保存了多少个元素。

可以使用：

```cpp
vectorName.size()
```

获取数量。

例如：

```cpp
std::vector<int> scores = {1, 2, 3};

scores.size();
```

结果是：

```cpp
3
```

再例如：

```cpp
std::vector<int> empty;

empty.size();
```

结果是：

```cpp
0
```

因此：

```cpp
std::vector<int> values = {5, 10, 15, 20};

values.size();
```

返回：

```text
4
```

注意：

> `.size()` 返回的是“元素数量”，不是某个元素的值。

---

## 5. `.size()` 返回的并不是 `int`

这是一个很容易忽略的细节。

例如：

```cpp
values.size()
```

返回的类型通常是类似：

```cpp
std::size_t
```

而不是：

```cpp
int
```

这是因为容器大小理论上可能非常大，因此标准库使用专门的无符号整数类型表示大小。

很多时候下面这样可以工作：

```cpp
int count = values.size();
```

但它实际上发生了整数类型转换。

如果代码需要明确把结果转换成 `int`，推荐写：

```cpp
static_cast<int>(values.size())
```

例如：

```cpp
int count = static_cast<int>(values.size());
```

---

## 6. 为什么 `{}` 中可能需要 `static_cast<int>`

假设函数需要返回：

```cpp
std::vector<int>
```

那么可以写：

```cpp
return {
    static_cast<int>(values.size()),
    static_cast<int>(baseline.size())
};
```

而不推荐直接写：

```cpp
return {
    values.size(),
    baseline.size()
};
```

原因是：

```cpp
values.size()
```

返回的是 `std::size_t`，而目标 vector 要保存的是 `int`。

C++ 的 braced initialization：

```cpp
{ ... }
```

对可能发生信息丢失的 **narrowing conversion（窄化转换）** 检查比较严格。

因此显式转换：

```cpp
static_cast<int>(...)
```

可以明确告诉编译器：

> 我知道这里正在把这个大小转换成 `int`。

---

# `const std::vector<int>&` 是什么意思

题目的参数是：

```cpp
const std::vector<int> &values
```

可以拆成三部分理解。

## `std::vector<int>`

表示：

> 一个保存 `int` 的 vector。

---

## `&`

表示**引用**。

如果写：

```cpp
std::vector<int> values
```

通常意味着函数会得到一份 vector 的副本。

而：

```cpp
std::vector<int> &values
```

表示函数直接引用原来的 vector，而不是复制整个 vector。

对于可能比较大的 vector，这样效率更高。

---

## `const`

`const` 表示：

> 函数只能读取这个 vector，不能修改它。

因此：

```cpp
const std::vector<int> &values
```

整体可以理解为：

> 以只读引用的方式接收一个 `std::vector<int>`。

这是 C++ 中非常常见的函数参数写法：

```cpp
const T &value
```

适合：

* 数据可能比较大
* 不希望复制
* 函数只需要读取，不需要修改

---

# 本题需要掌握的核心

假设：

```cpp
values = {10, 20, 30, 40};
```

那么：

```cpp
values.size()
```

是：

```text
4
```

如果函数内部创建：

```cpp
std::vector<int> baseline = {1, 2, 3};
```

那么：

```cpp
baseline.size()
```

是：

```text
3
```

最终需要返回：

```cpp
{4, 3}
```

注意第二个数字虽然永远是 `3`，但题目要求不要直接写：

```cpp
return {4, 3};
```

而是让两个 vector 分别计算自己的大小：

```cpp
values.size()
baseline.size()
```

这样代码表达的是：

> 返回这两个 vector 当前各自拥有多少个元素。

而不是：

> 我碰巧知道第二个答案是 3。
