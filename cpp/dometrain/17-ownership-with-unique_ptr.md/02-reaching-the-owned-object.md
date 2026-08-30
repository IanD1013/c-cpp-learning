# C++ `std::unique_ptr`：访问所管理对象的三种方式

假设有一个 `std::unique_ptr<std::string>`：

```cpp
std::unique_ptr<std::string> owner =
    std::make_unique<std::string>("ab");
```

这里：

* `owner` 是一个 `std::unique_ptr<std::string>`
* 它拥有一个位于堆上的 `std::string`
* 字符串当前内容为 `"ab"`

访问这个字符串主要有三种形式：

```cpp
owner->member()
*owner
owner.get()
```

它们用途不同，不应混淆。

---

## 1. `owner->member()`：访问对象的成员

例如：

```cpp
std::size_t n = owner->size();
```

`owner->size()` 的意思是：

> 访问 `owner` 所拥有的 `std::string`，然后调用这个字符串的 `size()`。

可以把它理解为：

```cpp
(*owner).size()
```

因此：

```cpp
owner->size()
```

和：

```cpp
(*owner).size()
```

效果相同。

### 为什么不能写 `owner.size()`？

因为：

```cpp
owner.size()
```

是在询问：

> `std::unique_ptr` 自己有没有 `size()`？

但 `std::unique_ptr` 没有这个成员。

真正拥有 `size()` 的是里面的 `std::string`。

所以：

```cpp
owner->size();   // ✅ string 的 size()
owner.size();    // ❌ unique_ptr 没有 size()
```

---

## 2. `*owner`：访问整个对象

`*owner` 表示：

> `owner` 所指向的那个完整 `std::string` 对象。

例如：

```cpp
*owner += "cd";
```

原来字符串是：

```text
ab
```

执行后变成：

```text
abcd
```

这里修改的是堆上的那个 `std::string` 本身。

也可以：

```cpp
std::string copy = *owner;
```

这时：

* `*owner` 得到原字符串对象
* `copy` 会复制它的内容

原来的堆对象仍然由 `owner` 管理。

### 什么时候使用 `*owner`？

当代码需要的是：

```cpp
std::string
```

本身，而不是它的某个成员时。

例如：

```cpp
std::cout << *owner;

std::string result = "value=" + *owner;
```

---

## 3. `owner.get()`：取得普通裸指针

例如：

```cpp
const std::string *borrowed = owner.get();
```

`owner.get()` 返回：

```cpp
std::string *
```

也就是一个普通的 raw pointer（裸指针）。

但非常重要：

```text
get() 只借出地址，不转移所有权。
```

对象仍然属于：

```cpp
owner
```

因此不能这样做：

```cpp
delete owner.get();   // ❌
```

因为之后 `owner` 销毁时还会再次释放该对象，从而产生 double free / undefined behavior。

---

## `get()` 的典型用途

最常见情况是某个旧接口要求：

```cpp
const std::string *
```

例如：

```cpp
bool isEmptyText(const std::string *text);
```

如果手里有：

```cpp
std::unique_ptr<std::string> owner;
```

就可以：

```cpp
bool empty = isEmptyText(owner.get());
```

这里 `isEmptyText` 只是暂时借用这个对象。

所有权仍然属于：

```cpp
owner
```

---

# 三种访问方式对比

| 写法              | 得到什么     | 典型用途      |
| --------------- | -------- | --------- |
| `owner->size()` | 对象成员     | 调成员函数     |
| `*owner`        | 整个对象     | 读取或修改对象   |
| `owner.get()`   | 裸指针 `T*` | 传给需要指针的函数 |

例如：

```cpp
std::unique_ptr<std::string> owner =
    std::make_unique<std::string>("ab");

std::size_t n = owner->size();       // 调 string::size()

*owner += "cd";                      // 修改整个 string

const std::string *p = owner.get();  // 借出 string*
```

---

# 一个很重要的记忆方式

假设：

```cpp
std::unique_ptr<T> owner;
```

那么：

```cpp
owner
```

是：

```text
智能指针对象
```

而：

```cpp
*owner
```

是：

```text
T 对象
```

而：

```cpp
owner.get()
```

是：

```text
T*
```

因此可以记成：

```text
owner         → unique_ptr<T>
*owner        → T
owner.get()   → T*
owner->foo()  → 调用 T::foo()
```

---

# `get()` 不会延长对象生命周期

例如：

```cpp
const std::string *borrowed;

{
    auto owner = std::make_unique<std::string>("hello");
    borrowed = owner.get();
}
```

离开 `{}` 后：

```cpp
owner
```

被销毁，它管理的字符串也被释放。

此时：

```cpp
borrowed
```

虽然还保存着地址，但这个地址已经失效，成为 dangling pointer（悬空指针）。

所以：

> 从 `get()` 得到的指针不能比拥有者 `unique_ptr` 活得更久。

---

# 本题的核心思路

题目要求实现：

```cpp
static std::string appendAndDescribe(
    const std::string &text,
    const std::string &suffix
)
```

处理过程可以分成四步。

### 第一步：创建 owner

把 `text` 放进堆上的字符串：

```cpp
auto owner = std::make_unique<std::string>(text);
```

---

### 第二步：追加 suffix 之前检查是否为空

题目已经提供：

```cpp
isEmptyText(const std::string *)
```

因此需要：

```cpp
owner.get()
```

例如：

```cpp
std::string state =
    isEmptyText(owner.get()) ? "empty" : "filled";
```

注意检查发生在追加 `suffix` **之前**。

例如：

```cpp
text = ""
suffix = "end"
```

初始字符串为空，所以状态必须是：

```text
empty
```

虽然追加以后字符串变成了 `"end"`。

---

### 第三步：使用 `*owner` 修改字符串

题目明确要求：

```cpp
*owner += suffix;
```

例如：

```text
"ab" + "cd"
```

变成：

```text
"abcd"
```

---

### 第四步：生成结果

结果格式为：

```text
状态|长度|字符串
```

长度必须在追加以后读取：

```cpp
owner->size()
```

然后使用：

```cpp
std::to_string(...)
```

转换为字符串。

最终：

```cpp
state
+ "|"
+ std::to_string(owner->size())
+ "|"
+ *owner
```

---

# 边界情况

## `text` 为空

输入：

```cpp
text = ""
suffix = "end"
```

首先检查：

```text
text 是空的
```

所以：

```text
state = empty
```

追加以后：

```text
end
```

长度：

```text
3
```

结果：

```text
empty|3|end
```

---

## 两个字符串都为空

```cpp
text = ""
suffix = ""
```

结果：

```text
empty|0|
```

注意最后的 `|` 后面没有字符，但这个分隔符仍然存在。

---

## 初始字符串非空

例如：

```cpp
text = "ab"
suffix = "cd"
```

状态在追加之前判断：

```text
filled
```

追加后：

```text
abcd
```

长度：

```text
4
```

结果：

```text
filled|4|abcd
```
