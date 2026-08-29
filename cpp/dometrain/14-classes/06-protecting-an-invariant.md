# C++ 类不变量（Invariant）与封装

## 1. 什么是类不变量？

**Invariant（不变量）**是一个对象在正常可用状态下必须始终满足的条件。

例如，一个 `Percentage` 类表示百分比：

```cpp
0 <= storedValue_ <= 100
```

这就是 `Percentage` 的类不变量。

可以把它理解成：

> 类向外界保证：只要一个 `Percentage` 对象处于正常状态，它内部保存的值一定在 `[0, 100]` 范围内。

这个条件至少应该在以下时刻成立：

* 构造函数执行结束后
* 每个可能修改对象状态的成员函数执行结束后

---

## 2. 为什么成员变量应该设为 `private`？

假设：

```cpp
class Percentage {
private:
    int storedValue_;
};
```

因为 `storedValue_` 是 `private`，类外代码不能直接执行：

```cpp
percentage.storedValue_ = 500;  // 不允许
```

于是，所有修改 `storedValue_` 的代码都只能存在于类内部。

这样我们就可以集中控制：

```cpp
Percentage(...)
update(...)
store(...)
```

哪些地方允许修改状态。

### 但要注意

`private` 本身并不会自动保证数据合法。

例如：

```cpp
void update(int value) {
    storedValue_ = value;
}
```

即使 `storedValue_` 是私有的：

```cpp
update(500);
```

仍然会破坏：

```cpp
0 <= storedValue_ <= 100
```

因此：

> `private` 提供的是“控制修改入口”的能力，而真正维护 invariant 的是这些入口中的检查逻辑。

---

## 3. 所有修改入口都必须检查 invariant

假设一个类要求：

```cpp
0 <= level_ <= 100
```

那么构造函数和修改函数都必须执行同样的检查。

不推荐这样：

```cpp
Volume(int level) {
    if (level >= 0 && level <= 100) {
        level_ = level;
    }
}

void turnTo(int level) {
    if (level >= 0 && level <= 100) {
        level_ = level;
    }
}
```

因为同一个规则被写了两遍。

以后如果规则发生变化，例如改成：

```cpp
0 <= level <= 80
```

就可能只修改了一处，导致逻辑不一致。

---

## 4. 把检查集中到一个私有函数

更好的设计是：

```cpp
class Volume {
public:
    Volume(int level) {
        set(level);
    }

    void turnTo(int level) {
        set(level);
    }

private:
    void set(int level) {
        // 所有合法性检查统一放这里
    }

    int level_ = 0;
};
```

这样：

```text
constructor ─┐
             ├──> set() ──> 修改 level_
turnTo() ────┘
```

所有状态修改都经过同一个入口。

这种设计的重要好处是：

> **Single source of truth：一条规则只写一次。**

---

## 5. 非法输入应该怎么办？

假设合法范围为：

```cpp
0 <= value <= 100
```

那么：

```cpp
0
100
```

都是合法值。

非法值包括：

```cpp
-1
101
150
-1000
1100
```

对于非法输入，一个类可以采用不同策略：

### 策略一：Clamp

把值限制到最近的合法边界：

```text
-20  -> 0
150  -> 100
```

### 策略二：Reject

拒绝修改：

```text
当前值 = 40

update(150)

当前值仍然 = 40
```

本题采用第二种：

> 非法输入不能写入对象。

同时还要增加一次拒绝计数。

---

## 6. 为什么构造函数也必须检查？

构造函数同样会建立对象的初始状态：

```cpp
Percentage p(150);
```

如果构造函数直接：

```cpp
storedValue_ = 150;
```

那么对象刚创建出来 invariant 就已经被破坏了。

因此构造函数应该复用同一个检查函数：

```cpp
Percentage(int start) {
    store(start);
}
```

如果成员变量默认初始化为：

```cpp
int storedValue_ = 0;
```

那么：

```cpp
Percentage p(150);
```

执行过程是：

```text
storedValue_ = 0
        ↓
store(150)
        ↓
150 非法
        ↓
拒绝修改
        ↓
storedValue_ 仍然是 0
        ↓
拒绝次数 +1
```

最终：

```cpp
p.current()   // 0
p.refusals()  // 1
```

因此构造函数产生的非法输入也算一次拒绝。

---

## 7. `return` 很适合处理拒绝逻辑

典型代码结构：

```cpp
void store(int candidate) {
    if (candidate < 0 || candidate > 100) {
        ++refusalCount_;
        return;
    }

    storedValue_ = candidate;
}
```

逻辑非常清楚：

```text
candidate 非法？
    │
    ├─ 是 → refusalCount_++ → return
    │
    └─ 否 → storedValue_ = candidate
```

这里的：

```cpp
return;
```

表示立即结束 `void` 函数。

它不返回一个值，只是不再继续执行下面的代码。

因此非法数据永远不会运行到：

```cpp
storedValue_ = candidate;
```

---

# 核心知识总结

维护类 invariant 时，可以遵循这个模式：

```cpp
class Example {
public:
    Example(int value) {
        set(value);
    }

    void update(int value) {
        set(value);
    }

private:
    void set(int value) {
        if (/* value 非法 */) {
            // 处理非法情况
            return;
        }

        data_ = value;
    }

    int data_ = 合法默认值;
};
```

核心思想：

```text
private 数据
    ↓
限制修改入口
    ↓
所有修改入口复用同一个验证函数
    ↓
验证成功才修改状态
    ↓
保证 invariant 始终成立
```

一句话记忆：

> **Invariant 是类必须始终维护的规则；`private` 控制谁能修改状态，而统一的验证函数负责真正保证这个规则。**
