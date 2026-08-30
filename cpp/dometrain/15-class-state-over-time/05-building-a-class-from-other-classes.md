# C++ 类的组合：把一个类作为另一个类的成员

## 1. 成员变量的类型也可以是一个类

C++ 中，一个类的成员变量不一定只能是 `int`、`double`、`bool` 等基本类型，也可以直接是另一个类的对象。

例如：

```cpp
class Clock {
public:
    Clock(int start) : minutes_(start) {}

    int minutes() const {
        return minutes_;
    }

private:
    int minutes_;
};

class Session {
public:
    Session(int start) : started_(start) {}

    int startMinute() const {
        return started_.minutes();
    }

private:
    Clock started_;
};
```

这里：

```cpp
Clock started_;
```

表示：

> 每一个 `Session` 对象内部，都包含一个完整的 `Clock` 对象。

这种设计通常叫做 **组合（composition）**。

可以理解成：

```text
Session
└── Clock started_
    └── int minutes_
```

也就是一种 **has-a（拥有）关系**：

```text
Session has a Clock
```

---

## 2. 类类型成员必须先被构造

当创建：

```cpp
Session session(10);
```

时，C++ 并不是先进入 `Session` 构造函数的函数体，再创建 `started_`。

实际顺序是：

```text
1. 构造 Session 的成员 started_
2. 然后才进入 Session 构造函数体
```

所以：

```cpp
Session(int start) : started_(start) {}
```

中的：

```cpp
started_(start)
```

就是在告诉 C++：

> 使用 `Clock(int)` 构造函数来创建成员 `started_`。

等价于概念上的：

```cpp
Clock started_(start);
```

但注意，不能真的把它写进构造函数体。

---

## 3. 为什么不能在构造函数体里初始化？

假设写成：

```cpp
Session(int start) {
    // started_ = Clock(start);
}
```

问题在于：

在进入 `{}` 之前，C++ 就已经要求所有成员完成初始化。

但是 `Clock` 只有：

```cpp
Clock(int start)
```

并没有：

```cpp
Clock()
```

也就是没有默认构造函数。

因此 C++ 无法自动执行：

```cpp
Clock started_;
```

编译就会失败。

所以类类型成员通常应该直接写在 **成员初始化列表**中：

```cpp
Session(int start)
    : started_(start) {
}
```

---

## 4. 成员初始化列表不是“赋值”

下面两件事情概念上不同：

```cpp
Session(int start)
    : started_(start) {
}
```

这是：

```text
直接构造 started_
```

而：

```cpp
Session(int start) {
    started_ = Clock(start);
}
```

如果这种代码能够成立，它表示的是：

```text
先构造 started_
→ 再给 started_ 赋一个新值
```

所以对于类类型成员，优先使用初始化列表。

尤其是以下成员，经常必须使用初始化列表：

```cpp
const 成员
引用成员
没有默认构造函数的类类型成员
```

---

## 5. “拥有一个对象”不代表可以访问它的 private 成员

虽然 `Session` 内部有：

```cpp
Clock started_;
```

但是 `Session` 仍然不能直接访问：

```cpp
started_.minutes_
```

因为：

```cpp
minutes_
```

是 `Clock` 的 `private` 成员。

所以必须通过 `Clock` 提供的 public 接口：

```cpp
started_.minutes()
```

例如：

```cpp
int startMinute() const {
    return started_.minutes();
}
```

这体现了 C++ 的封装原则：

> 一个类应该通过自己的 public 接口管理自己的内部状态。

组合关系不会破坏这种封装。

---

# 用对象表示一个规则，而不是复制数据

假设已经有一个 `Range` 类：

```cpp
class Range {
public:
    Range(int low, int high)
        : low_(low), high_(high) {}

    bool contains(int value) const {
        return value >= low_ && value <= high_;
    }

private:
    int low_;
    int high_;
};
```

`Range` 自己负责维护“一个数是否位于区间内”这个规则。

如果 `Selector` 需要判断数字是否位于范围内，可以直接包含：

```cpp
Range allowed_;
```

而不是重新保存：

```cpp
int low_;
int high_;
```

更推荐：

```cpp
class Selector {
private:
    Range allowed_;
};
```

而不是：

```cpp
class Selector {
private:
    int low_;
    int high_;
};
```

---

## 为什么不要重复保存 `low` 和 `high`？

如果已经有：

```cpp
Range allowed_;
```

再保存：

```cpp
int low_;
int high_;
```

就会产生两份相同信息。

例如未来可能出现：

```text
allowed_ 表示 [3, 10]

但是

low_ = 4
high_ = 10
```

那么两个地方已经不一致了。

这类问题通常叫做：

> **重复状态（duplicated state）**

更好的设计原则是：

> 哪个类拥有某条规则，就让哪个类成为这条规则的唯一来源。

因此判断范围应该交给：

```cpp
allowed_.contains(value)
```

而不是让 `Selector` 自己再次写：

```cpp
value >= low && value <= high
```

---

# 组合的核心思想

例如：

```cpp
Range allowed_;
```

意味着：

```text
Selector
└── Range allowed_
    ├── low_
    └── high_
```

`Selector` 不需要知道 `Range` 内部到底如何存储范围。

它只需要知道：

```cpp
allowed_.contains(value)
```

能够回答：

```text
这个 value 是否被允许？
```

这就是面向对象设计中一个很重要的思想：

> 不直接操作别人的内部数据，而是让负责该数据的对象完成对应工作。

也就是：

```text
不要问别人要数据再自己处理，
而是直接让那个对象完成它负责的事情。
```

---

# 本节需要记住

```cpp
class A {
private:
    B member_;
};
```

表示类 `A` **拥有一个 `B` 对象**。

如果 `B` 需要构造参数：

```cpp
A(int x)
    : member_(x) {
}
```

访问 `B` 的功能时：

```cpp
member_.somePublicMethod()
```

而不能因为 `B` 是自己的成员，就直接访问：

```cpp
member_.privateMember
```

最重要的三个关键词：

```text
组合（composition）
成员初始化列表（member initializer list）
封装（encapsulation）
```
