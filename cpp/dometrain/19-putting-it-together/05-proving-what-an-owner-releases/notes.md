# C++ RAII、`unique_ptr` 与析构顺序

## 1. 这道题真正考什么

这道题主要考察四个知识点：

1. **RAII：资源跟随对象生命周期自动释放**
2. **`std::unique_ptr` 的独占所有权**
3. **`std::vector::erase()` 删除元素时会立即触发析构**
4. **需要特定析构顺序时，不能依赖 `vector` 自己的析构顺序**

题目中没有额外的“泄漏检测器”。

判断某个 `Resource` 是否真的被释放，唯一证据就是：

```cpp
~Resource()
```

执行时把自己的 `id` 写入一个 `record` 数组。

因此：

> `record` 中 id 出现的顺序，就是各个 `Resource` 实际析构的顺序。

---

# 2. `Resource` 不拥有 `record`

`Resource` 需要在析构时向外部的 `std::vector<int>` 写入自己的 id。

因此它保存：

```cpp
std::vector<int>* record_;
```

这里使用**普通指针**是合理的，因为 `Resource` 并不拥有这个 vector。

生命周期关系是：

```text
record
│
├── ResourceStore
│    ├── Resource
│    ├── Resource
│    └── Resource
│
└── record 最后才销毁
```

也就是说：

> `record` 的生命周期比所有 `Resource` 都长。

所以 `Resource` 只需要借用它即可，而不应该使用：

```cpp
std::unique_ptr<std::vector<int>>
```

或其他所有权指针。

---

# 3. `Resource` 的析构函数

要求是：

* 如果 `record_ != nullptr`
* 把自己的 `id_` 加入 record
* 否则什么都不做

逻辑：

```cpp
~Resource() {
    if (record_ != nullptr) {
        record_->push_back(id_);
    }
}
```

例如：

```cpp
looseRelease(5)
```

内部只有：

```cpp
{
    Resource resource(5, &record);
}
```

离开 `{}` 时，`resource` 被析构，于是：

```cpp
record == {5}
```

这就是最基本的 RAII：

```text
进入作用域
↓
构造对象
↓
使用对象
↓
离开作用域
↓
自动调用析构函数
```

---

# 4. `ResourceStore` 才是 Resource 的所有者

容器内部保存：

```cpp
std::vector<std::unique_ptr<Resource>> items_;
```

因此：

```text
ResourceStore
    │
    └── unique_ptr
            │
            └── Resource
```

每个 `Resource` 从创建出来开始就只有一个 owner。

不需要手动：

```cpp
delete
```

因为：

```cpp
std::unique_ptr<Resource>
```

销毁时会自动销毁它所拥有的 `Resource`。

这就是 RAII 最重要的思想之一：

> 不要手动管理资源释放，让拥有资源的对象负责生命周期。

---

# 5. `add()`：使用 `std::make_unique`

加入资源时：

```cpp
items_.push_back(
    std::make_unique<Resource>(id, record_)
);
```

其中：

```cpp
std::make_unique<Resource>(id, record_)
```

负责创建：

```cpp
Resource(id, record_)
```

并立即把它放进一个：

```cpp
std::unique_ptr<Resource>
```

中。

可以理解为：

```text
make_unique
    ↓
创建 Resource
    ↓
unique_ptr 获得独占所有权
    ↓
unique_ptr 放入 vector
```

优点是完全不需要手动写：

```cpp
new
delete
```

---

# 6. 通过 `unique_ptr` 访问 Resource

假设：

```cpp
items_[i]
```

类型是：

```cpp
std::unique_ptr<Resource>
```

那么：

```cpp
items_[i]->id()
```

等价于：

```cpp
items_[i].get()->id()
```

通常直接使用：

```cpp
items_[i]->id()
```

即可。

这是 `unique_ptr` 重载 `operator->` 后提供的便利语法。

---

# 7. `remove()` 为什么会立即触发析构

删除逻辑：

```cpp
if (items_[i]->id() == id) {
    items_.erase(items_.begin() + i);
    return;
}
```

这里有两个重点。

## 7.1 `erase()` 接收 iterator，不是 index

下面错误：

```cpp
items_.erase(i);
```

因为 `erase()` 需要的是一个迭代器。

因此要：

```cpp
items_.begin() + i
```

例如：

```text
items_

index:     0    1    2
           A    B    C

begin()
  ↑

begin() + 1
       ↑
       B
```

所以：

```cpp
items_.erase(items_.begin() + i);
```

删除第 `i` 个元素。

---

## 7.2 erase unique_ptr 会销毁 Resource

假设：

```text
items_

[ unique_ptr(A), unique_ptr(B), unique_ptr(C) ]
```

执行：

```cpp
items_.erase(items_.begin() + 1);
```

被删除的是：

```cpp
unique_ptr(B)
```

这个 `unique_ptr` 被销毁后：

```text
unique_ptr(B) 销毁
      ↓
Resource B 销毁
      ↓
~Resource()
      ↓
record.push_back(B.id)
```

因此删除某个资源时，它的 id 会**立即**写入 `record`。

例如加入：

```text
1, 2, 3
```

然后：

```cpp
remove(2);
```

此时立刻：

```cpp
record == {2}
```

之后 `1` 和 `3` 才会在 store 析构时释放。

---

# 8. 为什么 `remove()` 找到后必须 `return`

要求：

> 只删除第一个 id 相同的 Resource。

例如：

```text
items = [1, 5, 5, 8]
```

执行：

```cpp
remove(5);
```

正确结果：

```text
[1, 5, 8]
```

只删除第一个 `5`。

因此：

```cpp
if (items_[i]->id() == id) {
    items_.erase(items_.begin() + i);
    return;
}
```

这个：

```cpp
return;
```

非常重要。

如果 id 不存在，例如：

```cpp
remove(-1);
```

循环结束即可：

```text
什么都不删除
什么都不析构
```

---

# 9. 为什么 `ResourceStore` 要自己控制析构顺序

题目要求最终：

```text
先记录被 remove 的资源
然后按照添加顺序记录剩余资源
```

例如：

```cpp
ids = {10, 20, 30, 40}
removeId = 20
```

要求：

```cpp
record == {20, 10, 30, 40}
```

其中：

```text
20
```

在 `remove()` 时析构。

然后 store 结束时需要：

```text
10 → 30 → 40
```

按添加顺序释放。

问题是：

> 不能依赖 `std::vector` 自身析构时元素的销毁顺序来实现这个业务要求。

当题目要求明确的释放顺序时，应该自己显式控制。

---

# 10. 使用 `unique_ptr::reset()` 显式释放

`unique_ptr` 可以调用：

```cpp
ptr.reset();
```

如果它当前拥有对象：

```text
unique_ptr
   │
   └── Resource
```

调用：

```cpp
ptr.reset();
```

之后：

```text
Resource 被销毁
unique_ptr 变为空
```

也就是：

```cpp
ptr == nullptr
```

因此 Store 的析构函数可以：

```cpp
for (std::size_t i = 0; i < items_.size(); i += 1) {
    items_[i].reset();
}
```

这样析构顺序就是：

```text
items_[0]
↓
items_[1]
↓
items_[2]
↓
...
```

也就是资源原来的添加顺序。

---

# 11. 为什么 reset 后 vector 再析构不会重复释放

调用：

```cpp
items_[i].reset();
```

后，那个 `unique_ptr` 已经不再拥有 Resource：

```cpp
nullptr
```

因此循环结束后：

```text
items_

[nullptr, nullptr, nullptr]
```

之后 `items_` 自己被销毁时，只是在销毁这些空的 `unique_ptr`。

不会再次调用 Resource 的析构函数。

所以不会出现：

```text
重复释放
```

的问题。

---

# 12. 完整生命周期示例

假设：

```cpp
releaseOrder({1, 2, 3}, 2)
```

执行过程：

```text
record = {}

创建 ResourceStore

add(1)
items = [1]

add(2)
items = [1, 2]

add(3)
items = [1, 2, 3]

remove(2)
↓
erase unique_ptr<Resource(2)>
↓
Resource(2) 析构
↓
record = {2}

items = [1, 3]

离开 ResourceStore 作用域
↓
~ResourceStore()

reset items_[0]
↓
Resource(1) 析构
↓
record = {2, 1}

reset items_[1]
↓
Resource(3) 析构
↓
record = {2, 1, 3}
```

最终：

```cpp
{2, 1, 3}
```

---

# 13. 几个典型边界情况

## 删除不存在的 id

```cpp
releaseOrder({1, 2, 3}, -1)
```

没有任何资源在中途释放。

最终：

```cpp
{1, 2, 3}
```

---

## 有重复 id

```cpp
releaseOrder({5, 5, 8}, 5)
```

只删除第一个 `5`：

```text
remove:
5
```

剩余：

```text
5, 8
```

最终：

```cpp
{5, 5, 8}
```

虽然结果看起来 id 重复，但第一个 `5` 是被 `remove()` 删除的对象，第二个 `5` 是之后正常析构的对象。

---

## 空输入

```cpp
releaseOrder({}, 10)
```

没有 Resource 被创建。

最终：

```cpp
{}
```

---

# 14. 核心知识总结

这题可以浓缩成四条：

```cpp
std::make_unique<T>(...)
```

负责**创建并获得资源所有权**。

```cpp
vector.erase(iterator)
```

负责**删除一个 owner，因此立即销毁对应对象**。

```cpp
unique_ptr.reset()
```

负责**立即释放当前拥有的对象**。

```cpp
~Resource()
```

负责**记录真正的释放发生了**。

整体生命周期：

```text
make_unique
    ↓
unique_ptr owning Resource
    ↓
erase / reset / unique_ptr destruction
    ↓
Resource destructor
    ↓
record.push_back(id)
```

这就是典型的：

> **RAII + 独占所有权 + 确定性资源释放。**
