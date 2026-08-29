# C++：对 `vector<struct>` 进行排序

## 1. `std::sort` 会移动整个结构体元素

假设有一个结构体：

```cpp
struct Item {
    std::string name;
    int quantity;
};
```

并且我们有：

```cpp
std::vector<Item> rows;
```

如果希望按照 `quantity` 从小到大排序：

```cpp
std::sort(rows.begin(), rows.end(),
    [](const Item &a, const Item &b) {
        return a.quantity < b.quantity;
    }
);
```

这里：

```cpp
rows.begin()
rows.end()
```

表示排序范围，也就是整个 `vector`。

第三个参数：

```cpp
[](const Item &a, const Item &b) {
    return a.quantity < b.quantity;
}
```

是一个 **lambda 比较函数**。

它告诉 `std::sort`：

> 两个 `Item` 放在一起比较时，哪个应该排在前面？

---

## 2. Lambda 比较函数

基本形式：

```cpp
[](const Item &a, const Item &b) {
    return 条件;
}
```

其中：

* `[]`：lambda 的捕获列表
* `(const Item &a, const Item &b)`：接受两个待比较元素
* `return true`：表示 `a` 应该排在 `b` 前面
* `return false`：表示不要求 `a` 排在 `b` 前面

例如：

```cpp
return a.quantity < b.quantity;
```

表示：

```text
quantity 小的排前面
```

即升序。

如果写成：

```cpp
return a.quantity > b.quantity;
```

就是：

```text
quantity 大的排前面
```

即降序。

---

## 3. 为什么使用 `const Item &`

推荐写：

```cpp
const Item &a
const Item &b
```

而不是：

```cpp
Item a
Item b
```

因为引用不会复制整个结构体。

例如 `Item` 内部还有：

```cpp
std::string name;
```

如果按值传递，每次比较都可能复制字符串。

使用：

```cpp
const Item &
```

表示：

> 直接读取原来的元素，不复制，也不允许修改它。

这是排序比较函数中非常常见的写法。

---

## 4. 排序的是整个结构体，不只是某个成员

即使比较时只写：

```cpp
a.quantity < b.quantity
```

`std::sort` 移动的仍然是整个 `Item`。

例如原来：

```text
{"apple", 10}
{"banana", 3}
{"orange", 7}
```

按照 `quantity` 升序后：

```text
{"banana", 3}
{"orange", 7}
{"apple", 10}
```

`name` 和 `quantity` 不会被拆开。

这也是使用：

```cpp
std::vector<Item>
```

而不是维护两个平行 vector 的重要优势。

一个结构体代表一整行数据：

```text
name + quantity
```

它们始终一起移动。

---

# 多条件排序与 Tie-break

## 5. 如果主要排序条件相同怎么办？

假设要求：

1. 先按照 `quantity` 升序
2. `quantity` 相同时，再按照 `name` 字母顺序排序

可以写：

```cpp
std::sort(rows.begin(), rows.end(),
    [](const Item &a, const Item &b) {
        if (a.quantity != b.quantity) {
            return a.quantity < b.quantity;
        }

        return a.name < b.name;
    }
);
```

逻辑可以理解成：

```text
quantity 不同
    → quantity 决定顺序

quantity 相同
    → name 决定顺序
```

---

## 6. Tie-break 的通用写法

多条件排序通常写成：

```cpp
if (主要字段不同) {
    return 按主要字段比较;
}

return 按次要字段比较;
```

例如成绩排名：

```cpp
if (a.points != b.points) {
    return a.points > b.points;
}

return a.name < b.name;
```

表示：

```text
points 越高越靠前

points 相同：
name 字母顺序越靠前越靠前
```

例如：

```text
amy   100
zoe   100
bob    80
```

排序结果：

```text
amy   100
zoe   100
bob    80
```

因为 `amy` 和 `zoe` 分数相同，所以进一步比较名字。

---

# 为什么不要连续 sort 两次？

假设目标是：

```text
先按 points
points 相同时再按 name
```

不要简单写成：

```cpp
std::sort(... 按 points ...);
std::sort(... 按 name ...);
```

因为第二次：

```cpp
std::sort
```

会重新按照 `name` 排整个 vector。

第一次建立的 `points` 顺序可能被破坏。

而且普通：

```cpp
std::sort
```

不是稳定排序（stable sort）。

它不保证：

> 两个被认为相等的元素继续保持原来的相对顺序。

因此最直接、可靠的方法是把全部排序规则写进 **同一个 lambda**：

```cpp
std::sort(entries.begin(), entries.end(),
    [](const Entry &a, const Entry &b) {
        if (a.points != b.points) {
            return a.points > b.points;
        }

        return a.name < b.name;
    }
);
```

---

# `std::sort` 比较函数的重要规则

比较函数应该回答：

```text
a 是否应该严格排在 b 前面？
```

因此如果两个元素完全相等：

```cpp
a.name == b.name
a.points == b.points
```

那么：

```cpp
compare(a, b)
```

和：

```cpp
compare(b, a)
```

都应该是：

```cpp
false
```

例如：

```cpp
if (a.points != b.points) {
    return a.points > b.points;
}

return a.name < b.name;
```

对于完全相同的数据：

```cpp
a.name < b.name
```

自然会得到：

```cpp
false
```

所以这个比较规则是一致的。

---

# 排名题的整体思路

如果有两个平行 vector：

```cpp
names
points
```

例如：

```cpp
names  = {"Amy", "Bob", "Zoe"};
points = {90, 70, 90};
```

实际上：

```text
names[0] 与 points[0]
names[1] 与 points[1]
names[2] 与 points[2]
```

共同组成一行数据。

因此最好先组合成结构体：

```cpp
struct Entry {
    std::string name;
    int points;
};
```

得到：

```text
{"Amy", 90}
{"Bob", 70}
{"Zoe", 90}
```

然后排序整个 `Entry`：

```text
{"Amy", 90}
{"Zoe", 90}
{"Bob", 70}
```

最后再只把名字提取出来：

```cpp
{"Amy", "Zoe", "Bob"}
```

---

# 本节核心知识

记住下面四点即可：

```cpp
std::sort(begin, end, comparator);
```

其中 comparator 可以写成：

```cpp
[](const T &a, const T &b) {
    return ...;
}
```

排序结构体时：

1. 比较某个成员，但移动的是整个结构体
2. `const T &` 可以避免无意义复制
3. 多条件排序应该写进同一个比较函数
4. 比较函数返回 `true` 表示 `a` 应排在 `b` 前面

经典多条件模板：

```cpp
if (a.primary != b.primary) {
    return a.primary > b.primary;
}

return a.secondary < b.secondary;
```

即：

```text
主要条件：降序
次要条件：升序
```
