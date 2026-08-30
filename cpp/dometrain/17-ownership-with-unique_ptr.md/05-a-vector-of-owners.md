# C++：`vector<unique_ptr<T>>`——在容器中管理多个独占对象

## 1. 核心思想

`std::unique_ptr<T>` 表示：

> **某个对象只有一个 owner（所有者）。**

因此，如果我们需要同时管理很多堆对象，可以把多个 `unique_ptr` 放进 `vector`：

```cpp
std::vector<std::unique_ptr<Line>> owners;
```

这里的含义是：

```text
owners
  |
  +--> unique_ptr --> Line
  |
  +--> unique_ptr --> Line
  |
  +--> unique_ptr --> Line
```

每个 vector 元素都独占一个 `Line` 对象。

当 `owners` 被销毁时：

```text
vector 析构
   ↓
每个 unique_ptr 析构
   ↓
对应 Line 自动释放
```

所以完全不需要手动写：

```cpp
delete ...
```

这正是 RAII 的典型用法。

---

## 2. 创建对象并直接放入 vector

推荐：

```cpp
owners.push_back(std::make_unique<Line>("first"));
owners.push_back(std::make_unique<Line>("second"));
```

可以理解成：

```cpp
std::make_unique<Line>("first")
```

先创建：

```text
unique_ptr
    |
    v
 Line("first")
```

然后这个 owner 被移动到 `vector` 中。

最终变成：

```text
owners[0]
   |
   v
Line("first")
```

整个过程中始终只有一个 owner。

因此，这是非常自然的写法：

```cpp
std::vector<std::unique_ptr<Line>> owners;

for (const std::string &word : words) {
    owners.push_back(std::make_unique<Line>(word));
}
```

---

## 3. 为什么不能这样遍历？

错误：

```cpp
for (auto owner : owners) {
    owner->length();
}
```

原因在于：

```cpp
auto owner
```

意味着：

> 把 vector 中的元素复制到局部变量 `owner`。

如果元素类型是：

```cpp
std::unique_ptr<Line>
```

就相当于尝试：

```cpp
std::unique_ptr<Line> owner = owners[i];
```

但是 `unique_ptr` **不能复制**。

否则就会出现两个 owner：

```text
owners[i] ──┐
            ├──> 同一个 Line
owner ──────┘
```

这样两个 `unique_ptr` 最后都会认为自己应该释放同一个对象，因此 C++ 直接禁止这种复制。

---

## 4. 正确方法：使用引用

应该写：

```cpp
for (const std::unique_ptr<Line> &owner : owners) {
    int measured = owner->length();
}
```

这里：

```cpp
const std::unique_ptr<Line> &owner
```

不是创建新的 `unique_ptr`。

它只是给 vector 中现有的 `unique_ptr` 起一个临时别名：

```text
owner
  |
  | reference
  v
owners[i]
  |
  v
Line
```

所以并没有产生第二个 owner。

---

## 5. 更推荐的简写

实际 C++ 代码通常会写：

```cpp
for (const auto &owner : owners) {
    int measured = owner->length();
}
```

其中：

```cpp
auto
```

推导为：

```cpp
std::unique_ptr<Line>
```

所以：

```cpp
const auto &owner
```

等价于：

```cpp
const std::unique_ptr<Line> &owner
```

这是遍历 `vector<unique_ptr<T>>` 时非常常见的写法。

---

## 6. 单独访问元素时也一样

错误：

```cpp
auto current = owners[0];
```

因为这仍然是在复制：

```cpp
std::unique_ptr<Line> current = owners[0];
```

而 `unique_ptr` 不允许复制。

正确：

```cpp
const auto &current = owners[0];
```

或者明确写：

```cpp
const std::unique_ptr<Line> &current = owners[0];
```

然后：

```cpp
current->length();
```

---

## 7. `->` 访问被管理对象

假设：

```cpp
std::unique_ptr<Line> owner;
```

那么：

```cpp
owner->length();
```

表示调用：

```cpp
Line::length()
```

可以把：

```cpp
owner->length()
```

理解成：

```cpp
(*owner).length()
```

也就是说：

```text
owner
 ↓
Line object
 ↓
length()
```

---

## 8. `const unique_ptr<T>&` 的含义

这里：

```cpp
const std::unique_ptr<Line> &owner
```

`const` 主要限制的是：

> 不能通过这个引用修改 `unique_ptr` 自身的 ownership。

例如不能：

```cpp
owner.reset();
```

也不能：

```cpp
owner = ...
```

但仍然可以访问它指向的对象：

```cpp
owner->length();
```

注意：

```cpp
const std::unique_ptr<Line>&
```

和：

```cpp
std::unique_ptr<const Line>
```

不是一回事。

前者是：

```text
不能修改 owner
```

后者是：

```text
不能通过指针修改 Line
```

这是两个不同层次的 `const`。

---

# 常见模式

如果需要：

> 为多个对象分别建立独占 ownership

通常可以使用：

```cpp
std::vector<std::unique_ptr<T>> objects;

for (...) {
    objects.push_back(std::make_unique<T>(...));
}

for (const auto &object : objects) {
    object->someMethod();
}
```

---

# 本题最重要的知识点

记住这三个规则即可：

```cpp
std::vector<std::unique_ptr<T>>
```

表示：

> vector 中每个元素独占一个 `T`。

创建：

```cpp
owners.push_back(std::make_unique<T>(...));
```

遍历：

```cpp
for (const auto &owner : owners)
```

访问对象：

```cpp
owner->method();
```

不要写：

```cpp
for (auto owner : owners)
```

因为它会尝试复制 `unique_ptr`。

---

# 一个简单判断原则

看到：

```cpp
std::unique_ptr<T>
```

就先问自己：

> 这行代码是不是在创建第二个 `unique_ptr`？

如果是普通复制：

```cpp
auto x = owner;
```

❌ 不允许。

如果只是引用：

```cpp
const auto &x = owner;
```

✅ 可以。

如果把 ownership 转移：

```cpp
auto x = std::move(owner);
```

✅ 可以，但原来的 `owner` 将失去对象。

所以 `unique_ptr` 的核心规则可以浓缩成一句话：

> **不能复制，只能引用，或者移动 ownership。**
