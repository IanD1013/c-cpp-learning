# C++ `std::unique_ptr` 与 `std::make_unique`

## 1. 为什么需要 `unique_ptr`

传统动态内存通常写成：

```cpp
Meter *p = new Meter(4);

// 使用对象
p->step();

delete p;
```

问题是：程序员必须自己保证 `delete` 一定执行。

例如：

```cpp
Meter *p = new Meter(4);

if (somethingWrong) {
    return -1;  // 忘记 delete，发生内存泄漏
}

delete p;
```

现代 C++ 更推荐：

```cpp
std::unique_ptr<Meter> owner = std::make_unique<Meter>(4);
```

`unique_ptr` 会在自己生命周期结束时，**自动销毁它拥有的堆对象**。

因此通常不需要手写：

```cpp
new
delete
```

---

## 2. 基本写法

需要：

```cpp
#include <memory>
```

例如：

```cpp
std::unique_ptr<Meter> owner = std::make_unique<Meter>(4);
```

可以简单理解为：

```text
unique_ptr<Meter>
    ↓
拥有一个 Meter 对象

make_unique<Meter>(4)
    ↓
在堆上创建 Meter(4)
并把所有权交给 unique_ptr
```

---

## 3. `unique_ptr<T>` 中的 `T` 是对象类型

正确：

```cpp
std::unique_ptr<Meter>
```

不是：

```cpp
std::unique_ptr<Meter *>
```

因为：

```cpp
std::unique_ptr<Meter>
```

表示：

> 一个拥有 `Meter` 对象的智能指针。

而：

```cpp
std::unique_ptr<Meter *>
```

表示：

> 一个拥有 `Meter*` 指针对象的智能指针。

这完全不是同一个东西。

因此通常写：

```cpp
auto owner = std::make_unique<Meter>(4);
```

或者：

```cpp
std::unique_ptr<Meter> owner =
    std::make_unique<Meter>(4);
```

---

## 4. `make_unique` 会调用构造函数

假设：

```cpp
class Meter {
public:
    Meter(int start) : reading_(start) {}

private:
    int reading_;
};
```

那么：

```cpp
std::make_unique<Meter>(4)
```

相当于要求创建：

```cpp
Meter(4)
```

所以 `4` 会传给：

```cpp
Meter(int start)
```

如果类有默认构造函数：

```cpp
class Accumulator {
public:
    Accumulator() : total_(0) {}
};
```

那么可以写：

```cpp
std::make_unique<Accumulator>()
```

括号中的参数，本质上就是传给构造函数的参数。

---

## 5. 使用 `->` 访问对象成员

虽然：

```cpp
owner
```

本身是一个 `std::unique_ptr<Meter>` 对象，但它使用起来很像普通指针。

例如：

```cpp
owner->step();
```

相当于访问它所拥有的 `Meter`：

```cpp
Meter::step()
```

也可以：

```cpp
int value = owner->reading();
```

因此：

```cpp
owner->method()
```

是最常见的智能指针成员访问方式。

---

## 6. 自动释放内存

例如：

```cpp
int twoSteps() {
    auto owner = std::make_unique<Meter>(4);

    owner->step();
    owner->step();

    return owner->reading();
}
```

执行到函数结束时：

```cpp
}
```

局部变量：

```cpp
owner
```

生命周期结束。

`unique_ptr` 会自动：

1. 销毁它拥有的 `Meter`
2. 释放对应的堆内存

因此不需要：

```cpp
delete owner;
```

事实上这段代码本身就不会编译：

```cpp
delete owner;
```

因为 `owner` 不是原始指针。

---

## 7. Early Return 也会自动清理

这是 `unique_ptr` 非常重要的优势。

例如：

```cpp
int example(bool failed) {
    auto owner = std::make_unique<Meter>(4);

    if (failed) {
        return -1;
    }

    owner->step();

    return owner->reading();
}
```

无论走：

```cpp
return -1;
```

还是：

```cpp
return owner->reading();
```

`owner` 离开作用域时都会自动释放它拥有的对象。

这体现了 C++ 很重要的设计思想：

```text
RAII
Resource Acquisition Is Initialization
```

即：

> 让资源的生命周期绑定到对象生命周期。

资源对象离开作用域时，资源自动释放。

---

## 8. `unique_ptr` 的核心含义：唯一所有权

`unique_ptr` 中的 `unique` 表示：

> 同一个动态对象，在同一时间只有一个 `unique_ptr` 负责拥有它。

因此不能普通复制：

```cpp
auto a = std::make_unique<Meter>(4);

auto b = a;  // ❌ 不允许
```

否则 `a` 和 `b` 都会认为自己拥有同一个对象，最终就可能重复释放。

如果确实需要转移所有权，可以使用：

```cpp
auto b = std::move(a);
```

之后：

```text
a → 不再拥有对象
b → 成为新的 owner
```

不过在当前题目中不需要使用 `std::move`。

---

## 9. 和 `new/delete` 的关系

旧式：

```cpp
Accumulator *owner = new Accumulator();

owner->add(10);

int result = owner->total();

delete owner;
```

现代 C++：

```cpp
auto owner = std::make_unique<Accumulator>();

owner->add(10);

int result = owner->total();
```

最大的区别是：

```text
new/delete
    → 手动管理生命周期

make_unique/unique_ptr
    → 自动管理生命周期
```

通常应优先使用智能指针。

---

## 10. 本题真正考察的知识点

题目要求：

```cpp
static int accumulateValues(
    const std::vector<int> &values
)
```

并且希望：

1. 创建一个 `Accumulator`
2. 使用 `std::make_unique`
3. 不能自己用 `new/delete`
4. `Accumulator` 必须真正参与计算
5. 每个元素都调用：

```cpp
owner->add(value);
```

6. 最后：

```cpp
return owner->total();
```

因此程序结构应该是：

```cpp
创建 Accumulator
        ↓
遍历 vector
        ↓
每个 value 调用 add(value)
        ↓
读取 total()
        ↓
return
```

而不是自己另外创建：

```cpp
int sum = 0;
```

然后绕过 `Accumulator` 做普通求和。

---

## 11. 一个完整的小例子

```cpp
#include <memory>
#include <vector>

class Accumulator {
public:
    Accumulator() : total_(0) {}

    void add(int value) {
        total_ += value;
    }

    int total() const {
        return total_;
    }

private:
    int total_;
};

int accumulateValues(const std::vector<int> &values) {
    auto owner = std::make_unique<Accumulator>();

    for (int value : values) {
        owner->add(value);
    }

    return owner->total();
}
```

例如：

```cpp
values = {3, 5, -2}
```

执行过程：

```text
初始 total = 0

add(3)
total = 3

add(5)
total = 8

add(-2)
total = 6
```

最终：

```cpp
return 6;
```

如果：

```cpp
values = {}
```

循环一次都不会执行，因此：

```cpp
owner->total()
```

仍然是构造时的：

```text
0
```

---

# 核心总结

```cpp
auto owner = std::make_unique<Accumulator>();
```

表示：

> 在堆上创建一个 `Accumulator`，并让 `owner` 独占管理它。

访问对象：

```cpp
owner->add(value);
owner->total();
```

当：

```cpp
owner
```

离开作用域时，对象会自动销毁：

```text
不需要 delete
```

记住最常见模式：

```cpp
auto ptr = std::make_unique<Type>(constructor_arguments);

ptr->method();
```

对于现代 C++，如果一个动态对象只需要一个所有者：

```text
优先考虑 std::unique_ptr + std::make_unique
```
