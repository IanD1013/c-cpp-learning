# C++ 类放在头文件：类内定义、`public/private` 与 `const` 成员函数

## 1. 多个 `.cpp` 都要使用的类，通常放在头文件

如果一个类需要被多个源文件使用，可以把完整的类定义放进头文件：

```cpp
// Meter.hpp
#pragma once

class Meter {
public:
    Meter(int start) : reading_(start) {}

    void bump() {
        reading_ += 1;
    }

    int reading() const {
        return reading_;
    }

private:
    int reading_;
};
```

其他文件只需要：

```cpp
#include "Meter.hpp"
```

就可以创建和使用 `Meter`：

```cpp
Meter m(5);
m.bump();

int n = m.reading();
```

---

## 2. `#pragma once`：防止同一个头文件被重复包含

```cpp
#pragma once
```

通常写在头文件第一行。

它的作用是：

> 在同一个 `.cpp` 的编译过程中，一个头文件即使通过不同路径被 `#include` 多次，也只处理一次。

例如：

```cpp
// program.cpp
#include "Meter.hpp"
#include "report.hpp"
```

而：

```cpp
// report.hpp
#include "Meter.hpp"
```

这样 `Meter.hpp` 实际上被间接包含了两次。

有：

```cpp
#pragma once
```

之后，就不会因为同一个类被重复展开而发生 redefinition。

---

## 3. `class` 的基本结构

```cpp
class Meter {
public:
    // 对外接口

private:
    // 内部实现
};
```

注意类定义最后必须有分号：

```cpp
};
```

这是 C++ 类定义语法的一部分。

---

## 4. `public:` 和 `private:`

### `public:`

`public` 成员可以被类外代码访问。

```cpp
class Meter {
public:
    void bump();
};
```

于是：

```cpp
Meter m;
m.bump();   // 可以
```

---

### `private:`

`private` 成员只能由类自己的成员函数等内部代码直接访问。

```cpp
class Meter {
private:
    int reading_;
};
```

外部不能：

```cpp
Meter m;
m.reading_ = 10;   // 编译错误
```

但类自己的成员函数可以：

```cpp
void bump() {
    reading_ += 1;
}
```

---

## 5. 访问控制标签会一直生效

例如：

```cpp
class Example {
public:
    void foo();
    void bar();

private:
    int x;
    int y;
};
```

`public:` 会一直持续到 `private:`。

所以：

```cpp
foo()
bar()
```

都是 public。

而：

```cpp
x
y
```

都是 private。

---

## 6. `class` 默认就是 `private`

对于：

```cpp
class Counter {
    int count_;
};
```

即使没有写：

```cpp
private:
```

`count_` 默认也是 private。

也就是说：

```cpp
class Counter {
private:
    int count_;
};
```

在访问权限上是一样的。

不过实际项目和练习中通常还是推荐显式写：

```cpp
private:
```

因为代码意图更清楚。

另外：

* `class` 默认访问权限是 `private`
* `struct` 默认访问权限是 `public`

例如：

```cpp
struct A {
    int x;   // public
};
```

而：

```cpp
class B {
    int x;   // private
};
```

---

# 7. 成员函数可以直接定义在类里面

例如：

```cpp
class Meter {
public:
    void bump() {
        reading_ += 1;
    }

private:
    int reading_;
};
```

这里既完成了函数声明，也完成了函数定义。

不需要再去 `.cpp` 里面写：

```cpp
void Meter::bump() {
    ...
}
```

---

# 8. 类内定义的成员函数自动是 `inline`

例如：

```cpp
class Meter {
public:
    void bump() {
        reading_ += 1;
    }
};
```

`bump()` 在类定义内部定义，因此它自动具有 `inline` 属性。

可以理解为类似：

```cpp
inline void bump() {
    reading_ += 1;
}
```

所以当类定义放在头文件中，而这个头文件又被多个 `.cpp` 包含时，不会因为这些类内成员函数而违反普通函数的“一处定义”规则。

这也是为什么这种小型成员函数经常直接写在头文件中。

---

# 9. 构造函数初始化列表

例如：

```cpp
Meter(int start) : reading_(start) {}
```

其中：

```cpp
: reading_(start)
```

叫做 **member initializer list（成员初始化列表）**。

它表示：

```cpp
reading_
```

在对象创建时直接用 `start` 初始化。

概念上可以读成：

```text
创建 Meter 对象时：
reading_ = start
```

虽然严格来说它不是“先创建再赋值”，而是直接初始化。

---

# 10. `const` 成员函数

观察：

```cpp
int reading() const {
    return reading_;
}
```

这里参数列表后面的：

```cpp
const
```

非常重要。

它表示：

> 这个成员函数承诺不会修改当前对象的普通成员状态。

这种函数叫：

**const member function（const 成员函数）**

---

## 为什么需要它？

假设：

```cpp
const Meter &m = someMeter;
```

这里通过一个：

```cpp
const Meter &
```

访问对象。

此时只能调用 `const` 成员函数：

```cpp
m.reading();   // 可以
```

因为：

```cpp
int reading() const;
```

承诺不会修改对象。

---

如果写成：

```cpp
int reading() {
    return reading_;
}
```

那么：

```cpp
const Meter &m = someMeter;

m.reading();   // 编译错误
```

即使函数实际上只读取数据，编译器也只看函数签名中的承诺。

---

# 11. Getter 通常应该写成 `const`

像下面这种只读取状态的函数：

```cpp
int value() const;
bool empty() const;
int size() const;
bool atLimit() const;
```

通常都应该加：

```cpp
const
```

因为它们只是“查询对象状态”，不应该改变对象。

一个很实用的判断方法：

> 如果函数只是回答“这个对象现在是什么状态？”，通常应该考虑写成 `const`。

例如：

```cpp
class Counter {
public:
    int value() const;
    bool atLimit() const;

private:
    int count_;
    int limit_;
};
```

---

# 12. 修改对象的函数通常不是 `const`

例如：

```cpp
void add(int amount) {
    count_ += amount;
}
```

它会修改：

```cpp
count_
```

所以不能写：

```cpp
void add(int amount) const;
```

普通情况下这样会导致编译错误。

因此可以简单区分：

```cpp
add()        // 修改状态
value()      // 查询状态
atLimit()    // 查询状态
```

通常对应：

```cpp
void add(int amount);

int value() const;

bool atLimit() const;
```

---

# 13. 一个典型类的设计

```cpp
#pragma once

class Counter {
public:
    Counter(int limit);

    void add(int amount);

    int value() const;

    bool atLimit() const;

private:
    int count_;
    int limit_;
};
```

可以把类理解成两个部分：

```text
public
↓
别人可以怎么使用这个对象
也就是类的接口

private
↓
对象内部如何保存自己的状态
也就是实现细节
```

良好的面向对象设计通常会隐藏内部数据：

```cpp
private:
    int count_;
    int limit_;
```

然后通过公开方法：

```cpp
add()
value()
atLimit()
```

控制外部如何访问和修改这些数据。

---

# 核心总结

一个典型的头文件类：

```cpp
#pragma once

class Example {
public:
    Example(...);

    void modify(...);

    int query() const;

private:
    int data_;
};
```

重点记住：

1. 多个文件都需要使用的类通常定义在头文件。
2. 头文件使用 `#pragma once` 防止重复包含。
3. `public:` 是对外接口。
4. `private:` 是内部实现。
5. `class` 默认成员权限就是 `private`。
6. 类内定义的成员函数自动具有 `inline` 属性。
7. 只读取对象状态的成员函数通常应该加尾部 `const`。
8. `const T&` 只能调用 `const` 成员函数。
9. 修改成员变量的方法通常不能声明为 `const`。
