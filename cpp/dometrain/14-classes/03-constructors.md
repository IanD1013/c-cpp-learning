# C++ 构造函数（Constructor）

## 1. 什么是构造函数？

**构造函数（constructor）** 是对象被创建时自动执行的特殊成员函数。

它最重要的作用通常是：

> 在对象刚创建时，把成员变量初始化到一个合法、可用的状态。

例如：

```cpp
class Point {
public:
    Point(int x, int y) {
        x_ = x;
        y_ = y;
    }

    int x() {
        return x_;
    }

    int sum() {
        return x_ + y_;
    }

private:
    int x_ = 0;
    int y_ = 0;
};
```

创建对象：

```cpp
Point corner(3, 4);
```

此时会自动调用：

```cpp
Point(int x, int y)
```

相当于把：

```text
x = 3
y = 4
```

传入构造函数。

最后对象内部保存：

```text
corner.x_ = 3
corner.y_ = 4
```

---

## 2. 构造函数的语法特点

构造函数有两个非常重要的特点。

### 特点一：名字必须和类名完全相同

例如类叫：

```cpp
class Point
```

那么构造函数必须叫：

```cpp
Point(...)
```

不能写成：

```cpp
point(...)
```

也不能写成：

```cpp
createPoint(...)
```

---

### 特点二：构造函数没有返回类型

正确：

```cpp
Point(int x, int y) {
}
```

错误：

```cpp
void Point(int x, int y) {
}
```

即使写 `void` 也不行。

构造函数不是一个“返回 Point 的函数”，而是：

> 创建 `Point` 对象时自动执行的初始化过程。

---

# 3. 创建对象时调用构造函数

假设：

```cpp
class Point {
public:
    Point(int x, int y) {
        x_ = x;
        y_ = y;
    }

private:
    int x_;
    int y_;
};
```

创建：

```cpp
Point p(3, 4);
```

参数按照**位置**匹配：

```text
3 → x
4 → y
```

于是：

```cpp
x_ = 3;
y_ = 4;
```

和普通函数调用的参数规则完全一样。

---

# 4. 参数和成员变量不要混淆

常见写法：

```cpp
Point(int x, int y) {
    x_ = x;
    y_ = y;
}
```

这里：

```text
x   → 构造函数参数
x_  → 对象成员变量
```

例如：

```cpp
x_ = x;
```

表示：

> 把传进来的参数 `x` 保存到对象自己的 `x_` 中。

如果错误地写成：

```cpp
x = x;
```

实际上只是：

```text
把参数 x 赋值给参数 x 自己
```

成员变量 `x_` 完全没有改变。

因此很多 C++ 项目会使用这种命名习惯：

```cpp
x_
y_
name_
count_
```

用尾部 `_` 表示成员变量。

---

# 5. 更推荐的写法：成员初始化列表

虽然下面的代码可以工作：

```cpp
Point(int x, int y) {
    x_ = x;
    y_ = y;
}
```

现代 C++ 通常更推荐：

```cpp
Point(int x, int y)
    : x_(x), y_(y) {
}
```

这里的：

```cpp
: x_(x), y_(y)
```

叫做 **member initializer list（成员初始化列表）**。

含义是：

```text
用参数 x 初始化 x_
用参数 y 初始化 y_
```

完整例子：

```cpp
class Point {
public:
    Point(int x, int y)
        : x_(x), y_(y) {
    }

private:
    int x_;
    int y_;
};
```

这种写法通常更好，因为成员是在对象创建过程中**直接初始化**，而不是先初始化再执行赋值。

以后写构造函数时，可以优先考虑这种形式。

---

# 6. 构造函数通常为什么放在 `public:`

例如：

```cpp
class Point {
public:
    Point(int x, int y) {
        x_ = x;
        y_ = y;
    }

private:
    int x_;
    int y_;
};
```

外部代码需要执行：

```cpp
Point p(3, 4);
```

而创建对象本质上需要访问：

```cpp
Point(int x, int y)
```

因此构造函数通常放在：

```cpp
public:
```

下面。

如果构造函数是 `private`，普通外部代码就不能直接创建该对象。

---

# 7. 一个经典陷阱：`Point corner();`

下面看起来像是在创建对象：

```cpp
Point corner();
```

但它**不是创建对象**。

它实际上声明了一个函数：

```text
函数名：corner
参数：无
返回值：Point
```

这属于 C++ 中著名的：

> Most Vexing Parse

如果想创建一个无参数对象，应写：

```cpp
Point corner;
```

或者现代 C++ 更推荐：

```cpp
Point corner{};
```

---

# 8. 自己声明构造函数后，默认构造函数可能消失

例如：

```cpp
class Point {
public:
    Point(int x, int y) {
        x_ = x;
        y_ = y;
    }

private:
    int x_;
    int y_;
};
```

现在：

```cpp
Point p(3, 4);
```

可以。

但是：

```cpp
Point p;
```

通常不能编译。

原因是类只有：

```cpp
Point(int x, int y)
```

没有：

```cpp
Point()
```

也就是没有“零参数构造函数”。

如果希望两种方式都支持，可以自己定义：

```cpp
class Point {
public:
    Point()
        : x_(0), y_(0) {
    }

    Point(int x, int y)
        : x_(x), y_(y) {
    }

private:
    int x_;
    int y_;
};
```

这样：

```cpp
Point a;
Point b(3, 4);
```

都可以。

---

# 9. 构造函数和普通成员函数的区别

例如：

```cpp
class Point {
public:
    Point(int x, int y)
        : x_(x), y_(y) {
    }

    int x() {
        return x_;
    }

    int sum() {
        return x_ + y_;
    }

private:
    int x_;
    int y_;
};
```

这里有三种函数：

```cpp
Point(int x, int y)
```

是构造函数。

它在：

```cpp
Point p(3, 4);
```

时自动执行。

而：

```cpp
x()
sum()
```

属于普通成员函数，需要显式调用：

```cpp
p.x();
p.sum();
```

---

# 10. 本节核心记忆

可以把构造函数理解成：

```text
创建对象
   ↓
自动执行 constructor
   ↓
初始化成员变量
   ↓
得到可以正常使用的对象
```

例如：

```cpp
Interval span(9, 2);
```

如果构造函数是：

```cpp
Interval(int start, int end)
    : start_(start), end_(end) {
}
```

那么对象保存：

```text
start_ = 9
end_   = 2
```

之后：

```cpp
span.start()
```

应该返回：

```text
9
```

而：

```cpp
span.difference()
```

应该计算：

```cpp
end_ - start_
```

因此：

```text
2 - 9 = -7
```

不会自动排序，也不会取绝对值。

---

## 一句话总结

> 构造函数是在对象创建时自动执行的特殊成员函数，用来初始化对象；它与类同名，没有返回类型，通常通过成员初始化列表保存构造参数。
