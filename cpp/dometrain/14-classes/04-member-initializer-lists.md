# C++ 构造函数初始化列表

## 1. 什么是成员初始化列表

在构造函数的参数列表和函数体 `{}` 之间，可以使用冒号 `:` 写一个**成员初始化列表（member initializer list）**：

```cpp
class Rectangle {
public:
    Rectangle(int width, int height)
        : width_(width),
          height_(height),
          area_(width_ * height_) {
    }

private:
    const int width_;
    const int height_;
    const int area_;
};
```

基本语法：

```cpp
ClassName(parameters)
    : member1(value1),
      member2(value2),
      member3(value3) {
}
```

例如：

```cpp
Rectangle(int width, int height)
    : width_(width), height_(height) {
}
```

其中：

```cpp
width_(width)
```

表示：

* `width_`：类的数据成员
* `width`：构造函数参数
* 用参数 `width` 初始化成员 `width_`

---

## 2. 初始化和赋值不是一回事

下面两种代码看起来类似，但含义不同。

### 初始化列表

```cpp
Rectangle(int width)
    : width_(width) {
}
```

这里 `width_` **直接被初始化为 `width`**。

### 构造函数体中赋值

```cpp
Rectangle(int width) {
    width_ = width;
}
```

这里通常是：

1. `width_` 先完成初始化
2. 然后进入构造函数体
3. 再执行一次赋值

所以初始化列表通常更加直接，也通常应该优先使用。

---

## 3. `const` 成员必须使用初始化列表

例如：

```cpp
class Example {
private:
    const int value_;
};
```

`value_` 是 `const`，对象创建之后不能再修改。

因此下面不能编译：

```cpp
Example(int value) {
    value_ = value;   // ❌ 这是赋值
}
```

因为等执行到构造函数体时，`value_` 已经必须完成初始化了。

正确写法：

```cpp
Example(int value)
    : value_(value) {
}
```

因此可以记住：

> `const` 数据成员必须在构造阶段确定值，通常通过成员初始化列表完成。

---

## 4. 真正的初始化顺序由成员声明顺序决定

这是初始化列表中非常重要的一点。

假设：

```cpp
class Example {
public:
    Example(int x)
        : b_(a_ * 2),
          a_(x) {
    }

private:
    int a_;
    int b_;
};
```

虽然初始化列表写的是：

```cpp
b_(...)
a_(...)
```

但真正的初始化顺序仍然是：

```text
a_
↓
b_
```

因为类中声明顺序是：

```cpp
int a_;
int b_;
```

所以：

> 成员初始化顺序由它们在类中的**声明顺序**决定，而不是初始化列表中的书写顺序。

为了避免混乱，最好让初始化列表也按照成员声明顺序书写。

---

## 5. 一个成员依赖另一个成员时要特别注意

例如：

```cpp
class Rectangle {
private:
    const int width_;
    const int height_;
    const int area_;
};
```

然后：

```cpp
Rectangle(int width, int height)
    : width_(width),
      height_(height),
      area_(width_ * height_) {
}
```

这里是安全的，因为声明顺序为：

```text
width_
height_
area_
```

所以计算 `area_` 时，`width_` 和 `height_` 已经初始化。

如果反过来声明：

```cpp
private:
    const int area_;
    const int width_;
    const int height_;
```

即使初始化列表写：

```cpp
: width_(width),
  height_(height),
  area_(width_ * height_)
```

真正的初始化顺序仍然是：

```text
area_
width_
height_
```

此时计算 `area_` 时，后两个成员还没有初始化，程序可能产生错误结果。

---

## 6. 更简单的做法：直接使用构造函数参数

如果某个计算本来就来自构造函数参数，可以直接写：

```cpp
Rectangle(int width, int height)
    : width_(width),
      height_(height),
      area_(width * height) {
}
```

这通常比：

```cpp
area_(width_ * height_)
```

依赖更少，也更加清晰。

---

## 7. 初始化列表的推荐使用场景

以下情况尤其应该使用初始化列表：

```cpp
const int value_;
```

`const` 成员。

```cpp
SomeType &ref_;
```

引用成员。

```cpp
SomeObject object_;
```

需要调用特定构造函数的对象成员。

普通成员也推荐直接使用初始化列表：

```cpp
Example(int x)
    : x_(x) {
}
```

而不是：

```cpp
Example(int x) {
    x_ = x;
}
```

---

# 核心总结

构造函数推荐写成：

```cpp
ClassName(parameters)
    : member1(value1),
      member2(value2) {
}
```

重点记住：

1. 初始化列表用于**初始化成员**。
2. 它不是构造函数体中赋值的简写。
3. `const` 成员必须在初始化阶段获得值。
4. 成员真正的初始化顺序取决于**类中的声明顺序**。
5. 初始化列表最好按照成员声明顺序书写。
6. 如果计算可以直接使用构造函数参数，通常直接使用参数会更安全。
