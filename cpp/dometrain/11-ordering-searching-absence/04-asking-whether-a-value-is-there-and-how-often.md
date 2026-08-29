# C++：使用 `std::find` 和 `std::count` 查找与计数

## 1. 两个常用算法

在 `<algorithm>` 中：

* `std::find`：查找**第一个匹配元素的位置**
* `std::count`：统计**匹配元素出现了多少次**

例如：

```cpp
#include <algorithm>
#include <vector>

std::vector<int> v = {3, 8, 3};

auto found = std::find(v.begin(), v.end(), 8);
bool present = found != v.end();

int howMany =
    static_cast<int>(std::count(v.begin(), v.end(), 3));
```

结果：

```cpp
present == true
howMany == 2
```

---

## 2. `std::find` 返回的不是元素，而是迭代器

基本写法：

```cpp
auto found = std::find(v.begin(), v.end(), target);
```

`std::find` 会从 `begin()` 开始寻找第一个等于 `target` 的元素。

如果找到：

```cpp
found
```

指向那个元素。

如果没有找到：

```cpp
found == v.end()
```

因此判断元素是否存在通常写成：

```cpp
bool present =
    std::find(v.begin(), v.end(), target) != v.end();
```

可以把它理解成：

```text
find(...)
    ↓
返回某个位置
    ↓
是不是 end()？
   /      \
不是      是
找到      没找到
```

---

## 3. 为什么用 `end()` 表示“没找到”？

对于：

```cpp
std::vector<int> v = {10, 20, 30};
```

可以把迭代器位置想象成：

```text
begin()
  ↓
[10] [20] [30] [end]
```

`end()` 位于最后一个元素之后。

因此：

```cpp
v.end()
```

**并不指向真正的元素。**

它只是一个特殊位置，可以作为：

> “没有找到元素”

的标志。

所以：

```cpp
found != v.end()
```

表示找到。

而：

```cpp
found == v.end()
```

表示没找到。

---

## 4. 不要解引用 `end()`

下面是错误的：

```cpp
auto found = std::find(v.begin(), v.end(), 100);

std::cout << *found;
```

如果 `100` 不存在：

```cpp
found == v.end()
```

此时：

```cpp
*found
```

实际上是在尝试读取 `end()`，这是未定义行为。

正确方式：

```cpp
auto found = std::find(v.begin(), v.end(), 100);

if (found != v.end()) {
    std::cout << *found;
}
```

因此，一个重要习惯是：

> 先判断迭代器是否等于 `end()`，再解引用。

---

## 5. `std::find` 找的是第一个匹配

例如：

```cpp
std::vector<int> v = {3, 8, 3, 8};

auto found = std::find(v.begin(), v.end(), 8);
```

找到的是：

```text
{3, 8, 3, 8}
    ↑
```

也就是第一个 `8`。

`std::find` 找到后就停止搜索。

因此它特别适合回答：

```text
这个元素存在吗？
```

---

## 6. `std::count` 统计所有匹配

基本写法：

```cpp
std::count(v.begin(), v.end(), target)
```

例如：

```cpp
std::vector<int> v = {3, 8, 3};

auto count = std::count(v.begin(), v.end(), 3);
```

结果是：

```cpp
2
```

因为 `3` 出现了两次。

和 `std::find` 不同：

```text
std::find
```

找到第一个后即可停止。

而：

```text
std::count
```

必须把整个范围检查完，才能知道总共有多少个匹配元素。

---

## 7. `std::find` 和 `std::count` 的区别

| 算法           | 回答的问题       | 返回值 |
| ------------ | ----------- | --- |
| `std::find`  | 第一个匹配元素在哪里？ | 迭代器 |
| `std::count` | 匹配元素有几个？    | 数量  |

例如：

```cpp
std::vector<int> v = {3, 8, 3};
```

执行：

```cpp
std::find(v.begin(), v.end(), 3);
```

得到第一个 `3` 的位置。

执行：

```cpp
std::count(v.begin(), v.end(), 3);
```

得到：

```cpp
2
```

---

## 8. 两者都使用半开区间

这两个算法通常写成：

```cpp
std::find(v.begin(), v.end(), target);

std::count(v.begin(), v.end(), target);
```

范围是：

```text
[begin(), end())
```

即：

```text
包含 begin()
不包含 end()
```

这称为**半开区间**。

因此它实际上遍历的是：

```text
第一个元素
↓
[ element ][ element ][ element ]
                              ↑
                           最后一个元素
```

而 `end()` 只是最后一个元素后面的边界。

---

## 9. 空 vector 不需要特殊处理

例如：

```cpp
std::vector<int> v;
```

此时：

```cpp
v.begin() == v.end()
```

所以：

```cpp
std::find(v.begin(), v.end(), 3)
```

会立即结束，并返回：

```cpp
v.end()
```

而：

```cpp
std::count(v.begin(), v.end(), 3)
```

返回：

```cpp
0
```

因此一般不需要写：

```cpp
if (v.empty()) {
    ...
}
```

可以直接使用算法。

---

## 10. `std::count` 的返回类型

`std::count` 返回的是标准库用于表示距离/数量的整数类型，并不一定是 `int`。

如果题目明确要求：

```cpp
std::vector<int>
```

可以显式转换：

```cpp
int count =
    static_cast<int>(
        std::count(values.begin(), values.end(), target)
    );
```

在普通小型练习中这样写很常见。

不过在真实项目中，如果容器可能非常大，需要注意：

> 容器中的元素数量理论上可能超过 `int` 能表示的范围。

因此不要无意义地把所有计数值都强制转换成 `int`。

---

## 11. 和手写搜索的关系

手写版本可能是：

```cpp
bool found = false;

for (int value : values) {
    if (value == target) {
        found = true;
        break;
    }
}
```

而标准库版本：

```cpp
bool found =
    std::find(values.begin(), values.end(), target)
    != values.end();
```

两者表达的是同一件事。

同样，手写计数：

```cpp
int count = 0;

for (int value : values) {
    if (value == target) {
        ++count;
    }
}
```

可以直接写成：

```cpp
int count =
    static_cast<int>(
        std::count(values.begin(), values.end(), target)
    );
```

学习这些标准算法的重要意义之一就是：

> 当需求已经有标准算法可以表达时，不需要每次重新手写循环。

---

# 总结

记住下面两个经典模式即可：

## 判断元素是否存在

```cpp
bool present =
    std::find(values.begin(), values.end(), target)
    != values.end();
```

## 统计元素出现次数

```cpp
int count =
    static_cast<int>(
        std::count(values.begin(), values.end(), target)
    );
```

核心区别：

```text
std::find  → 找位置
std::count → 算数量
```

其中 `std::find` 没找到时返回：

```cpp
values.end()
```

所以判断是否存在时，本质上是在检查：

```cpp
std::find(...) != values.end()
```
