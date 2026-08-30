# C++：每个对象都有自己独立的成员变量

## 1. 普通成员变量属于“某一个具体对象”

在 C++ 中，类里声明的普通成员变量（非 `static`）是**每个对象各自拥有一份**的。

例如：

```cpp
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

创建两个对象：

```cpp
Meter left(4);
Meter right(4);

left.bump();
```

虽然 `left` 和 `right` 都是 `Meter` 类型，并且都用 `4` 初始化，但它们实际上拥有两份互相独立的 `reading_`：

```text
left
└── reading_ = 5

right
└── reading_ = 4
```

因此：

```cpp
left.reading();   // 5
right.reading();  // 4
```

修改 `left` 不会影响 `right`。

---

## 2. 可以把对象理解成“自带一份数据的实例”

类可以看作一个模板：

```cpp
class Meter {
    int reading_;
};
```

而每创建一个对象，就会根据这个模板创建一套新的成员变量。

例如：

```cpp
Meter a(10);
Meter b(20);
Meter c(30);
```

可以粗略理解为：

```text
a.reading_ = 10
b.reading_ = 20
c.reading_ = 30
```

这三个 `reading_` 是三个不同的变量。

因此：

```cpp
a.bump();
```

只修改：

```text
a.reading_
```

不会修改：

```text
b.reading_
c.reading_
```

---

## 3. 成员函数操作的是“调用它的那个对象”

考虑：

```cpp
void bump() {
    reading_ += 1;
}
```

调用：

```cpp
left.bump();
```

这里函数内部的：

```cpp
reading_
```

实际上指的是：

```cpp
left.reading_
```

如果调用：

```cpp
right.bump();
```

那么同样的代码：

```cpp
reading_ += 1;
```

操作的就是：

```cpp
right.reading_
```

也就是说：

> 成员函数中的成员变量名称，默认指向“当前调用这个函数的对象”的成员。

更深入一点，可以把：

```cpp
left.bump();
```

概念上理解成：

```cpp
// 伪代码
bump(&left);
```

成员函数内部实际上存在一个隐含的当前对象，也就是 C++ 中的 `this` 指针。

例如：

```cpp
void bump() {
    this->reading_ += 1;
}
```

与：

```cpp
void bump() {
    reading_ += 1;
}
```

在这里效果相同。

---

## 4. 构造函数负责给每个对象自己的成员赋初始值

例如：

```cpp
Meter(int start) : reading_(start) {}
```

创建：

```cpp
Meter left(4);
Meter right(7);
```

相当于分别得到：

```text
left.reading_  = 4
right.reading_ = 7
```

即使之后从未调用 `right` 的任何函数，它也不是“空的”。

它仍然保存着构造函数赋给它的值。

例如：

```cpp
Meter right(4);

right.reading();
```

结果仍然是：

```cpp
4
```

---

## 5. 对象的状态彼此独立

所谓对象的“状态”，通常就是它当前成员变量中保存的值。

例如：

```cpp
class Tally {
private:
    int total_;
};
```

创建：

```cpp
Tally first(10);
Tally second(100);
```

可以理解为：

```text
first 的状态：
total_ = 10

second 的状态：
total_ = 100
```

随后：

```cpp
first.add(5);
```

只会得到：

```text
first.total_ = 15
second.total_ = 100
```

这种“每个对象维护自己状态”的机制，是面向对象编程最基础的概念之一。

---

# 本节核心知识

## 普通成员变量是 per-object 的

例如：

```cpp
class Example {
private:
    int value_;
};
```

如果：

```cpp
Example a;
Example b;
```

那么实际上有：

```text
a.value_
b.value_
```

两份独立的数据。

---

## 成员函数操作当前对象

```cpp
a.doSomething();
```

成员函数中的：

```cpp
value_
```

指的是：

```cpp
a.value_
```

而：

```cpp
b.doSomething();
```

中的 `value_` 则指：

```cpp
b.value_
```

---

## 构造函数初始化每一个新对象

```cpp
Tally first(10);
Tally second(20);
```

意味着两个对象分别保存自己的初始状态：

```text
first.total_  = 10
second.total_ = 20
```

---

## 一个对象发生变化，不会自动影响另一个对象

```cpp
first.add(5);
```

不会改变：

```cpp
second
```

除非程序显式地让两个对象共享某些数据。

普通成员变量默认不会共享。

---

# 与 `static` 成员的区别

本节讨论的是普通成员变量。

普通成员：

```cpp
class Example {
    int value_;
};
```

每个对象都有一份。

而如果写成：

```cpp
class Example {
    static int value_;
};
```

那么 `value_` 属于整个类，而不是某个具体对象，所有对象共享同一份。

因此可以记住：

```text
普通成员变量
→ 每个对象一份

static 成员变量
→ 整个类共享一份
```

这两种情况不要混淆。
