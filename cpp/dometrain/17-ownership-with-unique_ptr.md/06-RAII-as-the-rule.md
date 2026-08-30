# C++ RAII 与对象销毁顺序

## 1. 什么是 RAII？

RAII（Resource Acquisition Is Initialization，资源获取即初始化）是 C++ 管理资源最核心的思想之一：

> **构造函数负责获得资源，析构函数负责释放资源。**

例如：

```cpp
class File {
public:
    File() {
        // 获取资源
    }

    ~File() {
        // 释放资源
    }
};
```

程序不需要显式写：

```cpp
release();
cleanup();
free();
```

而是利用 C++ 自身的对象生命周期规则：

```cpp
{
    File f;
} // 离开作用域，自动调用 ~File()
```

这意味着：

> **对象什么时候死亡，就决定了资源什么时候释放。**

---

# 2. 局部变量：逆声明顺序销毁

同一个作用域中的局部变量：

```cpp
{
    A a;
    B b;
    C c;
}
```

构造顺序：

```text
a
b
c
```

销毁顺序：

```text
c
b
a
```

因此可以记成：

```text
构造：从上往下
析构：从下往上
```

也就是：

> **后构造的对象，先销毁。**

---

# 3. 类成员也遵循相同规则

例如：

```cpp
class Example {
private:
    A a_;
    B b_;
    C c_;
};
```

创建 `Example` 时：

```text
a_
b_
c_
```

销毁 `Example` 时：

```text
c_
b_
a_
```

需要特别注意：

## 成员构造顺序由「声明顺序」决定

不是 initializer list 的书写顺序。

例如：

```cpp
class Example {
public:
    Example()
        : b_(),
          a_() {
    }

private:
    A a_;
    B b_;
};
```

虽然 initializer list 写的是：

```cpp
b_
a_
```

真实构造顺序仍然是：

```text
a_
b_
```

因为成员声明顺序是：

```cpp
A a_;
B b_;
```

所以最佳实践是：

```cpp
Example()
    : a_(),
      b_() {
}
```

让 initializer list 和成员声明保持一致，避免误导。

---

# 4. 析构函数与成员析构的关系

假设：

```cpp
class Example {
public:
    ~Example() {
        // destructor body
    }

private:
    A a_;
    B b_;
};
```

销毁过程是：

```text
1. 执行 Example::~Example() 的函数体
2. 销毁 b_
3. 销毁 a_
```

也就是说：

> **先执行对象自己的析构函数体，再自动销毁成员。**

成员不需要自己手动销毁。

---

# 5. `std::unique_ptr` 如何参与 RAII？

例如：

```cpp
class Holder {
private:
    std::unique_ptr<Tracked> owned_;
};
```

如果构造：

```cpp
owned_(std::make_unique<Tracked>(...))
```

那么 `unique_ptr` 是这个 `Tracked` 对象的唯一 owner。

当：

```cpp
Holder
```

被销毁时：

```text
Holder
  ↓
owned_ 被销毁
  ↓
unique_ptr 自动销毁它拥有的 Tracked
```

因此不需要：

```cpp
delete
```

也不应该手动：

```cpp
delete owned_.get();
```

这正是 RAII 的意义。

---

# 6. `make_unique` 创建的对象不会在构造函数结束时消失

例如：

```cpp
Holder(...)
    : owned_(std::make_unique<Tracked>(...)) {
}
```

这里创建的 `Tracked` 位于堆上。

它的生命周期不是：

```text
Holder 构造函数结束
→ Tracked 销毁
```

而是：

```text
Holder 构造
→ unique_ptr 持续拥有 Tracked
→ Holder 生命周期结束
→ unique_ptr 被销毁
→ Tracked 被销毁
```

因此：

> **资源的生命周期跟随 owner，而不是跟随创建它的函数。**

---

# 7. 用日志观察构造和析构

假设 `Tracked`：

```cpp
Tracked("ann", record);
```

构造时写入：

```text
+ann
```

析构时写入：

```text
-ann
```

如果：

```cpp
{
    Tracked a("ann", record);
    Tracked b("bob", record);

    record += "|";
}
```

最终就是：

```text
+ann+bob|-bob-ann
```

`|` 前面表示构造阶段：

```text
+ann+bob
```

`|` 后面：

```text
-bob-ann
```

全部来自自动调用的析构函数。

程序没有显式调用：

```cpp
a.~Tracked();
b.~Tracked();
```

---

# 8. 嵌套对象如何判断销毁顺序？

假设：

```cpp
class Holder {
private:
    Tracked first_;
    std::unique_ptr<Tracked> second_;
};
```

构造：

```text
first_
second_
```

销毁：

```text
second_
first_
```

其中销毁：

```cpp
second_
```

实际上意味着：

```text
unique_ptr 被销毁
→ 它拥有的 Tracked 被销毁
```

再考虑：

```cpp
class Group {
private:
    Holder holder_;
    Tracked third_;
};
```

构造：

```text
holder_
    first_
    second_

third_
```

因此整体构造：

```text
first
second
third
```

销毁必须完全反过来：

```text
third
second
first
```

可以把嵌套对象展开来看：

```text
构造：

Group
├── Holder
│   ├── first
│   └── second
└── third
```

销毁：

```text
third
Holder
    second
    first
```

所以：

> **一个作用域生命周期的后半段，基本上就是前半段倒着执行。**

---

# 9. 本题最核心的规律

如果要求：

```text
+ann+bob+cat|-cat-bob-ann
```

那么构造顺序必须是：

```text
ann
bob
cat
```

销毁顺序自然就是：

```text
cat
bob
ann
```

需要控制两个地方：

```text
Holder 的成员声明顺序
Group 的成员声明顺序
```

正确结构：

```text
Group
├── Holder
│   ├── ann：普通 Tracked
│   └── bob：unique_ptr<Tracked>
└── cat：普通 Tracked
```

这样 C++ 自己就会产生：

```text
构造：
ann → bob → cat

析构：
cat → bob → ann
```

---

# 10. 一个非常实用的记忆方式

看到：

```cpp
{
    A a;
    B b;
    C c;
}
```

直接在脑中翻转：

```text
创建
↓
A
B
C
|
C
B
A
↑
销毁
```

类成员也是完全相同的思路。

对于：

```cpp
std::unique_ptr<T>
```

则把它理解成：

```text
一个会在自己死亡时
顺便销毁 T 的成员
```

---

# 11. RAII 的真正价值

RAII 不只是为了少写几行 `delete`。

它真正解决的问题是：

```text
资源释放
```

不再依赖：

```text
程序员有没有记得 cleanup
```

而依赖：

```text
语言保证的对象生命周期
```

例如函数提前：

```cpp
return;
```

甚至发生异常：

```cpp
throw ...
```

局部对象仍然会正常析构。

因此现代 C++ 通常更推荐：

```cpp
std::unique_ptr<T>
std::vector<T>
std::string
```

这样的 RAII 类型，而不是：

```cpp
new
delete
```

手动维护资源生命周期。

---

## 核心总结

记住这四条基本就够了：

```text
1. RAII：构造时获取资源，析构时释放资源。

2. 局部变量：
   按声明顺序构造，
   按声明顺序的逆序销毁。

3. 类成员：
   按成员声明顺序构造，
   按成员声明顺序的逆序销毁。

4. unique_ptr：
   自己销毁时，
   自动销毁它拥有的对象。
```

因此：

```text
先构造
A → B → C

后销毁
C → B → A
```
