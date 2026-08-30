# C++ 成员函数：Mutator、Accessor 与 `const`

## 1. 对象的状态

类中的成员变量保存对象当前的**状态（state）**。

例如：

```cpp
class Level {
public:
    Level() : value_(0) {}

private:
    int value_;
};
```

这里：

```cpp
value_
```

就是 `Level` 对象的状态。

刚创建时：

```cpp
Level level;
```

内部状态为：

```text
value_ = 0
```

---

## 2. Mutator：修改对象状态的成员函数

会修改成员变量的成员函数称为：

> **mutator（修改器）**

例如：

```cpp
void raise(int by) {
    if (value_ + by <= 10) {
        value_ += by;
    }
}
```

这里：

```cpp
value_ += by;
```

修改了对象状态，因此 `raise()` 是 mutator。

### Mutator 通常不能写 `const`

下面是不允许的：

```cpp
void raise(int by) const {
    value_ += by;   // 编译错误
}
```

因为成员函数后面的：

```cpp
const
```

表示：

> 这个函数承诺不会修改当前对象。

但 `raise()` 明明修改了 `value_`，所以违反承诺。

---

## 3. Accessor：只读取对象状态的成员函数

只读取成员变量、不会修改对象状态的函数称为：

> **accessor（访问器 / getter）**

例如：

```cpp
int value() const {
    return value_;
}
```

这里没有修改 `value_`，只是把它返回出去，因此适合写成：

```cpp
const
```

完整语法：

```cpp
返回类型 函数名(参数) const {
    ...
}
```

例如：

```cpp
int value() const
```

注意：

```cpp
const
```

写在**参数列表之后**，不是写在返回类型前面。

---

## 4. 成员函数后的 `const` 是什么？

例如：

```cpp
int value() const {
    return value_;
}
```

这里的 `const` 可以理解为：

> “调用这个函数不会改变这个对象。”

编译器会检查这个承诺。

例如下面会编译失败：

```cpp
int value() const {
    value_ = 0;   // 错误
    return value_;
}
```

因为 `value()` 已经声明自己不会修改对象。

---

## 5. 为什么 accessor 应该写 `const`？

不仅仅是代码风格问题。

如果一个对象通过 `const` 引用访问：

```cpp
const Level &ref = level;
```

那么只能调用它的 `const` 成员函数。

例如：

```cpp
ref.value();
```

要求：

```cpp
int value() const;
```

如果写成：

```cpp
int value() {
    return value_;
}
```

那么：

```cpp
const Level &ref = level;
ref.value();  // 编译错误
```

因此一个只负责读取状态的函数，通常应该声明成：

```cpp
const
```

---

# 6. `private` 与状态保护

例如：

```cpp
class Level {
public:
    void raise(int by) {
        if (value_ + by <= 10) {
            value_ += by;
        }
    }

private:
    int value_;
};
```

因为：

```cpp
value_
```

是 `private`，类外不能这样做：

```cpp
level.value_ = 999;   // 编译错误
```

调用者只能通过：

```cpp
level.raise(...);
```

修改状态。

因此类可以在 mutator 中统一维护规则。

例如：

```cpp
if (value_ + by <= 10)
```

保证：

```text
value_ <= 10
```

---

# 7. 为什么规则应该放在类里面？

假设规定：

```text
Level 不能超过 10
```

正确做法：

```cpp
void raise(int by) {
    if (value_ + by <= 10) {
        value_ += by;
    }
}
```

这样所有调用者：

```cpp
a.raise(3);
b.raise(20);
c.raise(5);
```

都会经过同一个规则。

如果把检查交给调用者：

```cpp
if (...) {
    level.raise(...);
}
```

那么另一个调用者可能忘记检查。

因此一个重要的面向对象原则是：

> **让类自己维护自己的合法状态。**

即：

```text
private data
    ↓
public mutator
    ↓
检查规则
    ↓
修改状态
```

---

# 8. Mutator 和 Accessor 的典型结构

```cpp
class Example {
public:
    void changeSomething() {
        // 修改成员变量
    }

    int something() const {
        // 只读取成员变量
        return value_;
    }

private:
    int value_;
};
```

可以简单记成：

| 类型       | 作用     | 是否通常 `const` |
| -------- | ------ | ------------ |
| Mutator  | 修改对象状态 | 否            |
| Accessor | 读取对象状态 | 是            |

---

# 9. 状态机思想

`Toggle` 这道题实际上是在实现一个简单的：

> **有限状态机（Finite State Machine）**

对象只有三个合法状态：

```text
off
on
locked
```

操作会触发状态转换。

例如：

```text
off --turnOn--> on
on  --turnOff--> off
off --lock----> locked
on  --lock----> locked
locked --unlock--> off
```

但有些操作会被拒绝：

```text
locked --turnOn--> locked
locked --turnOff--> locked
```

也就是说：

> 当前状态 + 操作 → 新状态

可以用状态转换表表示：

| 当前状态     | turnOn   | turnOff  | lock     | unlock |
| -------- | -------- | -------- | -------- | ------ |
| `off`    | `on`     | `off`    | `locked` | `off`  |
| `on`     | `on`     | `off`    | `locked` | `on`   |
| `locked` | `locked` | `locked` | `locked` | `off`  |

写这种题时，先画状态表通常比直接写代码更容易。

---

# 10. 核心知识总结

这一节最重要的几个概念是：

```text
成员变量
    ↓
保存对象状态

Mutator
    ↓
修改状态
    ↓
通常不能 const

Accessor
    ↓
只读取状态
    ↓
通常应该 const

private
    ↓
阻止外部直接破坏状态

public mutator
    ↓
成为修改状态的唯一入口
    ↓
统一维护规则
```

其中最重要的一条设计原则是：

> **对象应该负责维护自己的状态和规则，而不是依赖调用者记住这些规则。**
