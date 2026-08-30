# C++ `std::unique_ptr`：为什么不能按值传参

## 1. 问题的核心

假设有：

```cpp
std::unique_ptr<Threshold> gate =
    std::make_unique<Threshold>(limit);

return countClearing(samples, gate);
```

而辅助函数写成：

```cpp
int countClearing(
    const std::vector<double> &samples,
    std::unique_ptr<Threshold> gate
);
```

这里会**编译失败**。

原因不是 `Threshold` 不能复制，而是：

> `std::unique_ptr` 不能复制。

---

## 2. 为什么按值传参会触发复制

函数参数：

```cpp
void f(SomeType x);
```

表示 `x` 是函数内部一个新的局部变量。

调用：

```cpp
SomeType a;
f(a);
```

通常意味着：

```text
a
↓ 复制
x
```

所以：

```cpp
countClearing(samples, gate);
```

如果参数是：

```cpp
std::unique_ptr<Threshold> gate
```

就相当于要求产生第二个 `unique_ptr`。

但 `unique_ptr` 表示：

> **唯一所有权（unique ownership）**

只能有一个 `unique_ptr` 负责某个对象的生命周期。

如果允许复制：

```cpp
owner1 ──► Threshold
owner2 ──► Threshold
```

那么两个 owner 都会认为自己应该释放同一个对象，最终可能导致重复释放。

因此 C++ 直接禁止：

```cpp
std::unique_ptr<T> a = ...;
std::unique_ptr<T> b = a;   // ❌
```

---

## 3. 编译器为什么会说 `deleted constructor`

`std::unique_ptr` 的复制构造函数被明确删除：

```cpp
unique_ptr(const unique_ptr&) = delete;
```

因此编译错误中经常能看到类似关键词：

```text
use of deleted function
```

以及：

```text
constructor
```

这里的 `deleted` 不是说什么东西在运行时被删除了。

它表示：

> 这个函数在 C++ 类型定义中被明确声明为禁止使用。

---

# 4. 真正应该传的是“对象”，不是“所有权”

辅助函数实际上只是想：

```text
读取 Threshold
```

它并不需要：

```text
拥有 Threshold
```

所以不应该传：

```cpp
std::unique_ptr<Threshold>
```

而应该传：

```cpp
const Threshold &
```

例如：

```cpp
static int countClearing(
    const std::vector<double> &samples,
    const Threshold &gate
)
```

这里：

```cpp
const Threshold &
```

表示：

* 不复制 `Threshold`
* 不接管它的生命周期
* 只引用已有对象
* 不能通过这个引用修改对象

这正符合“只读 helper”的需求。

---

# 5. 调用时使用 `*gate`

如果：

```cpp
std::unique_ptr<Threshold> gate =
    std::make_unique<Threshold>(limit);
```

那么：

```cpp
gate
```

是：

```cpp
std::unique_ptr<Threshold>
```

而：

```cpp
*gate
```

是：

```cpp
Threshold
```

所以函数参数是：

```cpp
const Threshold &
```

时，应该：

```cpp
countClearing(samples, *gate);
```

可以理解成：

```text
gate
│
│ unique_ptr
▼
Threshold 对象
```

而：

```cpp
*gate
```

就是“取出 gate 所拥有的那个 `Threshold` 对象”。

---

# 6. 为什么函数内部从 `->` 变成 `.`

如果参数原来是：

```cpp
std::unique_ptr<Threshold> gate
```

可能会写：

```cpp
gate->passes(sample);
```

因为 `gate` 是智能指针。

改成：

```cpp
const Threshold &gate
```

之后，`gate` 已经直接表示 `Threshold` 对象，因此写：

```cpp
gate.passes(sample);
```

总结：

```cpp
pointer->method()
object.method()
```

所以：

```cpp
std::unique_ptr<Threshold> gate;
gate->passes(x);
```

但：

```cpp
const Threshold &gate;
gate.passes(x);
```

---

# 7. 所有权和对象本身是两个不同概念

这是理解智能指针非常重要的一点。

```cpp
std::unique_ptr<Threshold> gate;
```

这里实际上涉及两样东西：

```text
gate                     Threshold
unique_ptr 对象           真正业务对象
负责“谁来释放”            保存 threshold 数据和行为
```

`unique_ptr` 管的是：

> **ownership / 生命周期责任**

而 helper 真正需要使用的是：

> **Threshold 对象本身**

因此，如果函数只是读取对象，应优先传：

```cpp
const Threshold &
```

而不是把管理对象生命周期的 `unique_ptr` 传进去。

---

# 8. 为什么不能改成 `unique_ptr<Threshold>&`

技术上可以写：

```cpp
std::unique_ptr<Threshold> &gate
```

它不会复制 `unique_ptr`。

但这里仍然不是好的接口设计。

因为 helper 实际上不关心：

```text
这个 Threshold 是不是由 unique_ptr 管理
```

它只关心：

```text
给我一个 Threshold，我读取它
```

所以：

```cpp
const Threshold &
```

表达的依赖更准确。

一个很重要的设计原则是：

> 函数参数应该描述函数真正需要的东西，而不是调用方碰巧使用的存储方式。

更好的：

```cpp
void inspect(const Threshold &gate);
```

而不是：

```cpp
void inspect(const std::unique_ptr<Threshold> &gate);
```

前者既可以接受：

```cpp
Threshold local;
```

也可以接受：

```cpp
std::unique_ptr<Threshold> ptr;
inspect(*ptr);
```

接口更加通用。

---

# 9. 为什么这里不应该用 `std::move`

`unique_ptr` 虽然不能复制，但可以移动：

```cpp
f(std::move(gate));
```

移动意味着：

```text
所有权从 gate 转移给函数参数
```

例如：

```text
调用前：

gate ──► Threshold

调用后：

gate = nullptr
        Threshold
            ▲
            │
      函数参数拥有
```

但本题 helper 只是读取数据，没有理由拿走所有权。

所以：

```cpp
std::move(gate)
```

虽然涉及合法的 `unique_ptr` 使用方式，但在这里是错误的设计方向。

判断标准：

```text
函数只是观察对象
→ const T &

函数需要修改已有对象
→ T &

函数明确接管对象所有权
→ std::unique_ptr<T>
```

---

# 10. 一个实用的参数设计规则

面对一个对象 `T`，可以先问：

### 只需要读取？

```cpp
const T &
```

例如：

```cpp
void print(const Threshold &t);
```

### 需要修改调用者的对象？

```cpp
T &
```

例如：

```cpp
void reset(Threshold &t);
```

### 参数可以不存在？

```cpp
const T *
```

或：

```cpp
T *
```

例如：

```cpp
void inspect(const Threshold *t);
```

### 函数需要接管对象生命周期？

```cpp
std::unique_ptr<T>
```

例如：

```cpp
void store(std::unique_ptr<Threshold> t);
```

---

# 11. 本题最重要的思维方式

看到：

```cpp
std::unique_ptr<T>
```

不要只把它理解成：

> “一种更安全的指针”。

更重要的是：

> `unique_ptr<T>` 表示“我拥有这个 `T`”。

因此设计函数参数时应该先问：

```text
这个函数真的需要拥有它吗？
```

如果只是读取：

```cpp
const T &
```

通常才是正确答案。
