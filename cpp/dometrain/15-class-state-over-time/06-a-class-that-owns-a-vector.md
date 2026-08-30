# C++：类中使用 `std::vector`、封装与去重列表

## 1. `std::vector` 可以作为类的成员

`std::vector<int>` 和 `int`、`std::string` 一样，都可以直接作为类的数据成员。

例如：

```cpp
#include <vector>

class Log {
public:
    void record(int value) {
        entries_.push_back(value);
    }

    const std::vector<int>& entries() const {
        return entries_;
    }

private:
    std::vector<int> entries_;
};
```

这里：

```cpp
std::vector<int> entries_;
```

表示每个 `Log` 对象内部都拥有自己的一个 `vector<int>`。

例如：

```cpp
Log a;
Log b;

a.record(10);
a.record(20);

b.record(30);
```

此时可以理解为：

```text
a.entries_ = [10, 20]
b.entries_ = [30]
```

两个对象的数据互不影响。

---

# 2. `vector` 的生命周期和对象绑定

如果一个 `vector` 是对象的数据成员：

```cpp
class Log {
private:
    std::vector<int> entries_;
};
```

那么它会：

```text
Log 对象创建
    ↓
entries_ 自动创建（默认是空 vector）

对象存活期间
    ↓
entries_ 可以增长、删除、修改

Log 对象销毁
    ↓
entries_ 自动销毁
```

因此通常不需要自己手动管理 `vector` 的内存。

例如：

```cpp
Log log;
```

刚创建时：

```cpp
entries_.size() == 0
```

---

# 3. 为什么 `vector` 应该设为 `private`

假设我们希望这个类维持某种规则：

> vector 中的元素必须满足某些条件。

那么最好让：

```cpp
std::vector<int> items_;
```

保持：

```cpp
private
```

然后所有修改都必须通过类提供的成员函数。

例如：

```cpp
class PositiveNumbers {
public:
    void add(int value) {
        if (value > 0) {
            values_.push_back(value);
        }
    }

private:
    std::vector<int> values_;
};
```

这样类就能保证：

> `values_` 中永远只包含正数。

这就是一种 **class invariant（类不变量）**。

如果把 `values_` 设成 `public`：

```cpp
obj.values_.push_back(-100);
```

外部代码就能绕过 `add()`，直接破坏规则。

因此：

```text
private 数据
+
受控制的 public 操作
=
封装（encapsulation）
```

---

# 4. 返回 `const std::vector<int>&` 的意义

来看这个函数：

```cpp
const std::vector<int>& items() const {
    return items_;
}
```

这里实际上有两个重要的 `const`。

---

## 4.1 返回 `const std::vector<int>&`

```cpp
const std::vector<int>&
```

表示：

> 返回原来那个 vector 的只读引用。

例如：

```cpp
const std::vector<int>& values = obj.items();
```

这里没有复制整个 vector。

可以读取：

```cpp
values.size();
values[0];
```

但不能修改：

```cpp
values.push_back(10);   // 编译错误
values.erase(...);      // 编译错误
values[0] = 100;        // 编译错误
```

因此外部代码只能查看内部数据，不能通过这个引用破坏对象状态。

---

## 4.2 函数末尾的 `const`

```cpp
items() const
```

这个 `const` 表示：

> 调用 `items()` 不会修改当前对象。

也就是说：

```cpp
const DistinctList list;
list.items();
```

仍然是合法的。

---

# 5. 为什么不应该返回普通引用

如果写成：

```cpp
std::vector<int>& items() {
    return items_;
}
```

那么调用者就获得了内部 vector 的修改权限：

```cpp
list.items().push_back(100);
list.items().push_back(100);
```

这样调用者就可以绕过类自己的 `add()`。

如果这个类承诺：

> 每个数字只能出现一次。

那么这种设计就会让这个承诺完全失效。

因此，如果希望：

> 外部可以看，但不能直接修改内部容器

常见写法就是：

```cpp
const std::vector<int>& items() const;
```

---

# 6. `DistinctList` 要维护的不变量

假设我们实现一个：

```cpp
DistinctList
```

它的规则是：

> 每个数字最多出现一次，并保持第一次加入时的顺序。

例如：

```cpp
add(5);
add(2);
add(5);
add(8);
add(2);
```

最终结果应该是：

```cpp
[5, 2, 8]
```

而不是：

```cpp
[2, 5, 8]
```

因为我们不仅要求：

```text
不重复
```

还要求：

```text
保持加入顺序
```

---

# 7. 为什么这里使用 `vector` 而不是 `set`

`std::set` 天生能够保证元素唯一。

例如：

```cpp
std::set<int> values;
```

但 `set` 会按照值排序。

例如加入：

```cpp
5
2
8
```

得到的通常是：

```cpp
2 5 8
```

而我们需要：

```cpp
5 2 8
```

也就是：

> 保留第一次出现的顺序。

因此这里更适合：

```cpp
std::vector<int>
```

然后自己负责检查重复值。

---

# 8. 使用 `std::find` 查找元素

如果需要知道：

> `value` 是否已经存在于 vector 中

可以使用：

```cpp
#include <algorithm>
```

然后：

```cpp
std::find(items_.begin(), items_.end(), value);
```

完整写法：

```cpp
std::vector<int>::iterator position =
    std::find(items_.begin(), items_.end(), value);
```

---

# 9. `std::find` 返回的不是值，而是位置

这一点非常重要。

```cpp
std::find(...)
```

返回的是一个 **iterator（迭代器）**。

可以暂时把 iterator 理解为：

> 指向容器中某个元素的位置。

例如：

```cpp
std::vector<int> values{10, 20, 30};
```

如果：

```cpp
auto position = std::find(
    values.begin(),
    values.end(),
    20
);
```

那么 `position` 指向：

```text
[10, 20, 30]
      ↑
   position
```

---

# 10. `begin()` 和 `end()`

对于：

```cpp
std::vector<int> values{10, 20, 30};
```

可以粗略理解成：

```text
begin()
  ↓
[10][20][30]
            ↑
           end()
```

注意：

```cpp
end()
```

并不指向最后一个元素。

它指向：

> 最后一个元素之后的位置。

因此：

```cpp
std::find(...)
```

如果找不到目标，就返回：

```cpp
values.end()
```

所以判断“有没有找到”的标准写法是：

```cpp
if (position == items_.end()) {
    // 没找到
}
```

反过来：

```cpp
if (position != items_.end()) {
    // 找到了
}
```

---

# 11. `add()`：只有不存在时才加入

`DistinctList::add()` 的规则是：

> 如果这个值已经存在，就什么也不做；否则加入 vector 尾部。

可以写成：

```cpp
void add(int value) {
    auto position =
        std::find(items_.begin(), items_.end(), value);

    if (position == items_.end()) {
        items_.push_back(value);
    }
}
```

例如：

```cpp
add(4);
add(7);
add(4);
```

过程：

```text
add(4)
[] → [4]

add(7)
[4] → [4, 7]

add(4)
已经存在 → 不添加
```

最终：

```cpp
[4, 7]
```

---

# 12. `remove()`：找到位置，然后删除

删除时也是先查找：

```cpp
auto position =
    std::find(items_.begin(), items_.end(), value);
```

如果找不到：

```cpp
position == items_.end()
```

那么：

```cpp
return false;
```

如果找到：

```cpp
items_.erase(position);
return true;
```

完整实现：

```cpp
bool remove(int value) {
    auto position =
        std::find(items_.begin(), items_.end(), value);

    if (position == items_.end()) {
        return false;
    }

    items_.erase(position);
    return true;
}
```

含义是：

```text
成功找到并删除
→ true

没有找到
→ false
```

---

# 13. `vector::erase()` 接收的是位置，不是值

一个很容易犯的错误是：

```cpp
items_.erase(value);
```

对于 `std::vector` 来说，这通常是不对的。

`vector::erase()` 需要的是：

```cpp
iterator
```

也就是某个元素的位置。

正确：

```cpp
auto position =
    std::find(items_.begin(), items_.end(), value);

if (position != items_.end()) {
    items_.erase(position);
}
```

逻辑是：

```text
value
↓
std::find
↓
iterator
↓
erase(iterator)
```

这和某些其他容器不同。

例如 `std::set` 可以：

```cpp
set.erase(value);
```

但 `vector` 的常见删除方式是：

```cpp
vector.erase(iterator);
```

---

# 14. 不要额外维护一个 `count`

假设已经有：

```cpp
std::vector<int> items_;
```

没有必要再写：

```cpp
int count_;
```

因为 vector 自己就知道长度：

```cpp
items_.size()
```

如果同时保存：

```cpp
items_
count_
```

就会出现两个状态必须始终同步的问题。

例如：

```cpp
items_.erase(position);

// 忘记：
count_--;
```

此时：

```text
items_.size() = 3
count_ = 4
```

状态产生矛盾。

因此一个重要设计原则是：

> 如果某个信息可以直接从现有状态计算出来，就尽量不要重复存储。

这里直接使用：

```cpp
items_.size()
```

即可。

---

# 15. 一个完整的 `DistinctList`

```cpp
#include <algorithm>
#include <vector>

class DistinctList {
public:
    void add(int value) {
        auto position =
            std::find(items_.begin(), items_.end(), value);

        if (position == items_.end()) {
            items_.push_back(value);
        }
    }

    bool remove(int value) {
        auto position =
            std::find(items_.begin(), items_.end(), value);

        if (position == items_.end()) {
            return false;
        }

        items_.erase(position);
        return true;
    }

    const std::vector<int>& items() const {
        return items_;
    }

private:
    std::vector<int> items_;
};
```

这个类维护的不变量是：

```text
items_ 中不存在重复元素
```

而且：

```text
元素顺序 = 第一次成功加入的顺序
```

---

# 16. 示例

```cpp
DistinctList list;

list.add(5);
list.add(2);
list.add(5);
list.add(8);
```

此时：

```cpp
list.items()
```

内容为：

```cpp
{5, 2, 8}
```

然后：

```cpp
bool result = list.remove(2);
```

得到：

```cpp
result == true
```

vector 变成：

```cpp
{5, 8}
```

如果：

```cpp
bool result = list.remove(100);
```

由于 `100` 不存在：

```cpp
result == false
```

而 vector 保持不变。

---

# 17. `distinctAfterRemoval` 在测试什么

题目提供：

```cpp
static std::vector<int> distinctAfterRemoval(
    const std::vector<int>& values,
    int toRemove
);
```

它通常做的事情类似：

```cpp
DistinctList list;

for (int value : values) {
    list.add(value);
}

list.remove(toRemove);

return list.items();
```

因为返回类型是：

```cpp
std::vector<int>
```

所以这里：

```cpp
return list.items();
```

虽然 `items()` 返回的是：

```cpp
const std::vector<int>&
```

但返回函数需要的是：

```cpp
std::vector<int>
```

因此会自动复制一份 vector 作为返回结果。

---

# 18. 边界情况

实现时要特别注意这些情况。

## 空输入

```cpp
values = {}
```

最终：

```cpp
{}
```

---

## 所有数字都重复

例如：

```cpp
{4, 4, 4, 4}
```

加入完成后：

```cpp
{4}
```

---

## 删除不存在的数字

例如：

```cpp
{1, 2, 3}
```

删除：

```cpp
10
```

结果：

```cpp
{1, 2, 3}
```

并且：

```cpp
remove(10) == false
```

---

## 删除存在的数字

```cpp
{1, 2, 3}
```

删除：

```cpp
2
```

结果：

```cpp
{1, 3}
```

并且：

```cpp
remove(2) == true
```

---

# 19. 这一节真正需要掌握的知识

这道题表面上是在实现一个“去重列表”，实际上主要训练以下几个 C++ 概念：

```text
std::vector 作为类成员
        ↓
private 数据与封装
        ↓
类不变量
        ↓
const reference 只读访问
        ↓
std::find 查找
        ↓
iterator 表示元素位置
        ↓
end() 表示未找到
        ↓
erase(iterator) 删除元素
```

其中最值得记住的三个模式是：

```cpp
// 查找
auto it = std::find(v.begin(), v.end(), value);
```

```cpp
// 判断是否找到
if (it != v.end()) {
}
```

```cpp
// 删除找到的元素
v.erase(it);
```

以及类的只读 accessor：

```cpp
const std::vector<int>& items() const {
    return items_;
}
```

可以把它理解成：

> **外部可以查看我的数据，但不能绕过我的规则直接修改它。**
