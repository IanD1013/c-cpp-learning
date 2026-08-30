# C++：在类中使用 `std::unique_ptr` 管理堆对象

## 1. 核心思想

一个类不仅可以拥有普通成员变量，也可以通过 `std::unique_ptr` **独占地拥有一个堆对象**。

例如：

```cpp
class Session {
public:
    Session(const std::string &label)
        : label_(label),
          hits_(std::make_unique<Tally>()) {
    }

    void record() {
        hits_->bump();
    }

    int recorded() const {
        return hits_->total();
    }

private:
    std::string label_;
    std::unique_ptr<Tally> hits_;
};
```

这里：

```cpp
std::unique_ptr<Tally> hits_;
```

表示：

> `Session` 对象拥有一个 `Tally` 对象，而这个 `Tally` 存放在堆上。

---

## 2. `unique_ptr` 成员本身不等于“已经创建对象”

写：

```cpp
std::unique_ptr<Tally> hits_;
```

只是声明了一个智能指针成员。

它类似于：

```cpp
std::unique_ptr<Tally> p;
```

默认情况下它并没有指向任何 `Tally`。

也就是说，此时概念上是：

```text
hits_
  |
  └── nullptr
```

真正创建 `Tally` 的地方是：

```cpp
std::make_unique<Tally>()
```

---

## 3. 推荐在构造函数初始化列表中创建对象

```cpp
Session(const std::string &label)
    : label_(label),
      hits_(std::make_unique<Tally>()) {
}
```

这里：

```cpp
hits_(std::make_unique<Tally>())
```

做了两件事情：

1. 在堆上创建一个 `Tally`
2. 让 `hits_` 获得这个对象的所有权

可以理解成：

```text
Session
├── label_ = "abc"
└── hits_
      |
      v
    Tally       ← heap
```

这种写法比先默认构造、再在构造函数体中赋值更自然：

```cpp
Session(...)
    : label_(label) {
    hits_ = std::make_unique<Tally>();
}
```

后者虽然通常也可以运行，但初始化列表能让对象从一开始就处于完整状态。

---

## 4. 访问 `unique_ptr` 所拥有的对象

如果：

```cpp
std::unique_ptr<Tally> hits_;
```

那么访问 `Tally` 的成员时使用：

```cpp
hits_->bump();
hits_->total();
```

也就是：

```cpp
pointer->member
```

因为 `unique_ptr` 表现得类似一个指向所拥有对象的指针。

等价地，也可以写：

```cpp
(*hits_).bump();
```

但通常：

```cpp
hits_->bump();
```

更加简洁。

---

## 5. 不需要自己写 `delete`

这是 `std::unique_ptr` 最重要的优势之一。

假设：

```cpp
class Session {
private:
    std::unique_ptr<Tally> hits_;
};
```

当 `Session` 被销毁时：

1. `Session` 的成员开始被销毁
2. `hits_` 的析构函数执行
3. `unique_ptr` 自动销毁它拥有的 `Tally`
4. 堆内存自动释放

因此不需要：

```cpp
~Session() {
    // 不需要手动 delete
}
```

更不能写：

```cpp
~Session() {
    delete hits_;   // 错误
}
```

原因是：

```cpp
hits_
```

的类型是：

```cpp
std::unique_ptr<Tally>
```

而不是：

```cpp
Tally *
```

`delete` 是给裸指针使用的，而 `unique_ptr` 自己已经负责释放资源。

---

# 6. RAII：对象拥有资源，生命周期自动绑定

这种设计体现了 C++ 中非常重要的思想：

> RAII（Resource Acquisition Is Initialization）

简单理解：

```text
对象活着
↓
资源存在

对象销毁
↓
资源自动释放
```

例如：

```cpp
class Session {
    std::unique_ptr<Tally> hits_;
};
```

那么：

```text
Session 生命周期
        │
        └── 控制 Tally 生命周期
```

不需要程序员在每个 `return`、异常路径或者分支中手动考虑：

```cpp
delete ...
```

这也是现代 C++ 更推荐智能指针，而不是直接使用 `new` / `delete` 的原因之一。

---

# 7. 普通成员 vs `unique_ptr` 成员

假设我们需要一个 `Counter`。

完全可以直接写：

```cpp
class DigitScan {
private:
    Counter seen_;
};
```

也可以写：

```cpp
class DigitScan {
private:
    std::unique_ptr<Counter> seen_;
};
```

两者都可以实现计数。

对于这种很小、生命周期完全跟随宿主对象的对象：

```cpp
Counter seen_;
```

实际上往往更简单。

使用：

```cpp
std::unique_ptr<Counter>
```

并不会自动变得：

* 更快
* 更省内存
* 更高级

这里使用堆对象主要是为了练习：

```text
class
  ↓ owns
unique_ptr
  ↓ owns
heap object
```

真实项目中是否需要 `unique_ptr`，应该根据所有权和生命周期设计决定。

---

# 8. `std::isdigit` 的正确使用方式

如果要判断一个字符是不是数字，可以使用：

```cpp
std::isdigit(...)
```

但推荐写成：

```cpp
std::isdigit(static_cast<unsigned char>(ch))
```

例如：

```cpp
for (char ch : text_) {
    if (std::isdigit(static_cast<unsigned char>(ch))) {
        ...
    }
}
```

不要简单依赖：

```cpp
std::isdigit(ch)
```

因为某些平台上 `char` 是有符号类型。

如果字符值为负数，直接把它传给 `<cctype>` 中的一些函数可能产生未定义行为。

因此形成习惯：

```cpp
std::isdigit(static_cast<unsigned char>(ch))
```

---

# 9. 本节应掌握的代码模式

以后看到：

```cpp
class Something {
private:
    std::unique_ptr<Widget> widget_;
};
```

通常应该想到：

```cpp
Something(...)
    : widget_(std::make_unique<Widget>()) {
}
```

使用：

```cpp
widget_->method();
```

而不是：

```cpp
new Widget
```

或者：

```cpp
delete widget_;
```

完整模式：

```cpp
class Owner {
public:
    Owner()
        : resource_(std::make_unique<Resource>()) {
    }

    void work() {
        resource_->doSomething();
    }

private:
    std::unique_ptr<Resource> resource_;
};
```

其中资源释放完全自动完成。

---

# 10. 一句话总结

```text
std::unique_ptr<T> 作为类成员
        ↓
表示这个类独占拥有一个 T
        ↓
构造时用 make_unique 创建
        ↓
使用时通过 ->
        ↓
宿主对象销毁时自动释放
```

关键原则：

> **拥有资源的成员应该负责资源生命周期，而使用 `unique_ptr` 时通常不需要手写析构函数。**
