# C++ 类的成员函数（Member Function）

## 1. 什么是成员函数？

定义在 `class` 内部的函数，叫做 **成员函数（member function）**。

```cpp
class Tally {
public:
    void bump(int amount) {
        count_ += amount;
    }

    int count() {
        return count_;
    }

private:
    int count_ = 0;
};
```

这里：

* `bump()` 是成员函数
* `count()` 是成员函数
* `count_` 是成员变量
* `count_` 是 `private`，类外不能直接访问

成员函数有一个重要特点：

> **成员函数可以直接访问该类对象的 private 成员，不需要把成员变量作为参数传进去。**

例如：

```cpp
void bump(int amount) {
    count_ += amount;
}
```

虽然 `count_` 是 `private`，但 `bump()` 属于 `Tally`，因此可以直接修改它。

---

## 2. `public` / `private` 决定的是“谁可以调用”

```cpp
class Tally {
public:
    void bump(int amount);
    int count();

private:
    int count_ = 0;
};
```

`public:` 表示：

```cpp
Tally tally;
tally.bump(3);
tally.count();
```

类外代码可以调用这些成员。

而 `private:` 下的函数或变量，只能被类内部代码访问。

例如：

```cpp
class Example {
private:
    void helper() {
        // 可以访问 private 成员
    }
};
```

`helper()` 虽然也是成员函数，也可以访问其他 `private` 成员，但：

```cpp
Example e;
e.helper();   // 编译错误
```

因为它本身是 `private`。

因此要区分两个问题：

* **这个函数能不能访问 private 成员？**

  * 成员函数可以。
* **类外代码能不能调用这个函数？**

  * 取决于函数是在 `public:` 还是 `private:` 下。

---

## 3. 成员函数是“在某个对象上运行”的

普通函数可以直接调用：

```cpp
foo();
```

而非 `static` 成员函数通常需要通过一个对象调用：

```cpp
Tally tally;

tally.bump(3);
```

这里可以理解为：

> 在 `tally` 这个对象上执行 `bump(3)`。

因此函数内部写：

```cpp
count_ += amount;
```

实际上修改的是 **这个 `tally` 对象自己的 `count_`**。

例如：

```cpp
Tally a;
Tally b;

a.bump(3);
b.bump(10);
```

此时：

```cpp
a.count()   // 3
b.count()   // 10
```

两个对象各自拥有自己的成员变量。

---

## 4. 成员函数为什么不用传入对象？

例如：

```cpp
tally.bump(3);
```

`bump()` 内部可以直接写：

```cpp
count_ += amount;
```

因为成员函数隐含地知道：

> “我现在正在操作哪个对象。”

可以把它粗略理解为编译器内部知道当前对象是 `tally`。

C++ 中这个当前对象可以通过 `this` 指针表示，例如：

```cpp
void bump(int amount) {
    this->count_ += amount;
}
```

和：

```cpp
void bump(int amount) {
    count_ += amount;
}
```

效果相同。

通常没有必要显式写 `this->`。

---

## 5. 调用成员函数必须写 `()`

假设：

```cpp
int count() {
    return count_;
}
```

正确调用：

```cpp
int x = tally.count();
```

错误：

```cpp
int x = tally.count;
```

因为：

```cpp
tally.count()
```

表示：

> 调用 `count` 函数，并得到它返回的 `int`。

而：

```cpp
tally.count
```

只是提到了这个函数本身，并没有执行它。

因此：

> **函数调用永远要写括号，即使没有参数。**

```cpp
counter.total();
```

而不是：

```cpp
counter.total;
```

---

## 6. 类外不能直接写成员函数名

例如：

```cpp
Tally tally;
tally.bump(3);
```

正确。

但：

```cpp
bump(3);
```

在类外通常不成立。

原因有两个：

1. `bump` 属于 `Tally`
2. 没有指定在哪个 `Tally` 对象上执行

成员函数必须通过对象调用：

```cpp
object.memberFunction();
```

---

# 典型设计：private 状态 + public 操作

类常见的一种设计方式是：

```cpp
class Counter {
public:
    void add(int amount) {
        total_ += amount;
    }

    int total() {
        return total_;
    }

private:
    int total_ = 0;
};
```

这里：

```cpp
total_
```

是内部状态。

外部不能：

```cpp
counter.total_ = 100;   // 错误
```

而是通过公开接口：

```cpp
counter.add(100);
```

读取时：

```cpp
counter.total();
```

这种设计叫做 **封装（encapsulation）**：

> 隐藏内部数据，只允许外部通过类提供的成员函数操作它。

---

# `add` 为什么必须使用 `+=`

如果目标是累计：

```cpp
counter.add(7);
counter.add(7);
counter.add(7);
```

最终应该是：

```text
21
```

因此：

```cpp
total_ += amount;
```

等价于：

```cpp
total_ = total_ + amount;
```

不能写：

```cpp
total_ = amount;
```

否则每次都会覆盖之前的结果：

```text
7
7
7
```

最终仍然只有：

```text
7
```

---

# `void` 成员函数

例如：

```cpp
void add(int amount) {
    total_ += amount;
}
```

`void` 表示：

> 这个函数不返回值。

因此不能依靠：

```cpp
counter.add(amount)
```

获得最终总数。

真正返回结果的是：

```cpp
counter.total();
```

这体现了一种常见模式：

```text
操作函数 → 修改对象状态
查询函数 → 返回对象状态
```

例如：

```cpp
counter.add(5);
counter.add(10);

int answer = counter.total();
```

---

# 一个完整调用过程

假设：

```cpp
std::vector<int> amounts{7, 7, -2};
```

创建：

```cpp
Counter counter;
```

初始：

```text
total_ = 0
```

第一次：

```cpp
counter.add(7);
```

变成：

```text
total_ = 7
```

第二次：

```cpp
counter.add(7);
```

变成：

```text
total_ = 14
```

第三次：

```cpp
counter.add(-2);
```

变成：

```text
total_ = 12
```

最后：

```cpp
return counter.total();
```

返回：

```text
12
```

---

# 空 vector 为什么自然得到 0？

如果：

```cpp
std::vector<int> amounts;
```

循环：

```cpp
for (int amount : amounts) {
    counter.add(amount);
}
```

一次都不会执行。

而成员一开始就是：

```cpp
int total_ = 0;
```

因此最后：

```cpp
counter.total();
```

自然返回：

```text
0
```

这也是为什么为成员变量设置合理的初始值非常重要。

---

# 核心记忆

```cpp
object.memberFunction(arguments);
```

成员函数：

* 属于某个类
* 通常通过对象调用
* 可以直接访问该对象的 `private` 成员
* `public/private` 决定外部是否能访问它

常见封装结构：

```cpp
class Counter {
public:
    void add(int amount) {
        total_ += amount;
    }

    int total() {
        return total_;
    }

private:
    int total_ = 0;
};
```

可以把它理解为：

```text
private 成员变量
    ↓
保存对象内部状态

public 成员函数
    ↓
提供安全的操作接口
```
