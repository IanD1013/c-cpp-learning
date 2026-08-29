# C++ `class`、访问控制与成员函数

## 1. `class` 和 `struct` 本质上都能定义自定义类型

在 C++ 中，`class` 和 `struct` 都可以：

* 定义成员变量
* 定义成员函数
* 创建对象
* 控制成员的访问权限

例如：

```cpp
class Box {
public:
    int fill(int value) {
        contents_ = value;
        return contents_;
    }

private:
    int contents_ = 0;
};
```

这里定义了一个 `Box` 类型，其中包含：

* 一个公开成员函数 `fill`
* 一个私有成员变量 `contents_`

---

## 2. `class` 和 `struct` 的关键区别：默认访问权限

最需要记住的区别是：

```cpp
struct
```

成员默认是：

```cpp
public
```

而：

```cpp
class
```

成员默认是：

```cpp
private
```

例如：

```cpp
class Box {
    int value;
};
```

等价于：

```cpp
class Box {
private:
    int value;
};
```

所以外部代码不能这样写：

```cpp
Box box;
box.value = 10;   // 编译错误
```

因为 `value` 是私有成员。

---

## 3. `public:` 和 `private:`

`public:` 和 `private:` 是**访问控制标签**。

它们会影响后面声明的所有成员，直到遇到下一个访问标签。

例如：

```cpp
class Example {
public:
    int getValue();
    void setValue(int value);

private:
    int value_;
    std::string name_;
};
```

这里：

```cpp
getValue()
setValue()
```

都是公开成员。

而：

```cpp
value_
name_
```

都是私有成员。

---

## 4. `public` 成员

`public` 成员可以被类外部访问。

例如：

```cpp
class Box {
public:
    int fill(int value) {
        return value;
    }
};
```

外部可以：

```cpp
Box box;
int result = box.fill(4);
```

成员函数的调用格式是：

```cpp
对象.成员函数(参数)
```

例如：

```cpp
box.fill(4);
```

---

## 5. `private` 成员

`private` 成员只能直接被这个类自己的成员函数访问。

例如：

```cpp
class Box {
public:
    int fill(int value) {
        contents_ = value;
        return contents_;
    }

private:
    int contents_ = 0;
};
```

在 `fill()` 内部：

```cpp
contents_ = value;
```

是合法的，因为 `fill()` 属于 `Box`。

但是外部：

```cpp
Box box;
box.contents_ = 10;
```

会编译失败。

---

## 6. 为什么要使用 `private`

把数据放在 `private` 中，可以防止外部代码随意修改对象内部状态。

例如：

```cpp
class BankAccount {
public:
    void deposit(int amount) {
        if (amount > 0) {
            balance_ += amount;
        }
    }

private:
    int balance_ = 0;
};
```

如果 `balance_` 是公开的：

```cpp
account.balance_ = -100000;
```

外部就可以直接制造不合理的状态。

但设置为 `private` 后，外部只能通过：

```cpp
account.deposit(100);
```

来修改余额。

这种思想叫做：

**封装（Encapsulation）**

即：

> 对象内部的数据由对象自己的成员函数负责管理，而不是让外部代码任意修改。

---

## 7. 成员函数可以直接访问成员变量

考虑：

```cpp
class Box {
public:
    int fill(int value) {
        contents_ = value;
        return contents_;
    }

private:
    int contents_ = 0;
};
```

调用：

```cpp
Box box;
box.fill(4);
```

进入 `fill()` 后：

```cpp
contents_ = value;
```

实际上修改的是当前这个 `box` 对象自己的 `contents_`。

可以把它理解成：

```text
box.fill(4)
    ↓
在 box 内部执行 fill
    ↓
box.contents_ = 4
```

成员函数天然知道自己正在操作哪个对象，因此不需要把 `contents_` 额外作为参数传进去。

---

## 8. 成员变量常用 `_` 后缀

代码中：

```cpp
int contents_;
```

末尾的 `_` 不是 C++ 语法要求，只是一种常见命名风格。

通常用来表示：

```text
这是一个成员变量
```

例如：

```cpp
class User {
private:
    std::string name_;
    int age_;
};
```

这样可以很容易区分：

```cpp
void setAge(int age) {
    age_ = age;
}
```

其中：

```cpp
age_
```

是成员变量，

而：

```cpp
age
```

是函数参数。

---

## 9. 类定义最后仍然需要分号

和 `struct` 一样：

```cpp
class Box {
};
```

最后必须写：

```cpp
;
```

也就是：

```cpp
};
```

而不是：

```cpp
}
```

漏掉分号会导致编译错误。

---

## 10. 私有成员会影响初始化方式

对于简单的 `struct`：

```cpp
struct Point {
    int x;
    int y;
};
```

通常可以写：

```cpp
Point p{3, 4};
```

但是如果类型包含私有成员，例如：

```cpp
class Box {
private:
    int contents_ = 0;
};
```

不能直接依靠这种外部聚合初始化：

```cpp
Box box{4};   // 不允许
```

因为外部代码不能直接初始化那个私有成员。

可以先创建对象：

```cpp
Box box;
```

然后通过公开成员函数修改：

```cpp
box.fill(4);
```

另一种更常见的正式做法，是以后学习**构造函数**：

```cpp
class Box {
public:
    Box(int value) : contents_(value) {}

private:
    int contents_;
};
```

这样就可以：

```cpp
Box box{4};
```

但这里能够写 `{4}`，是因为调用了构造函数，而不是因为外部可以直接访问私有成员。

---

# 本节核心模型

可以把一个 `class` 理解成：

```text
class
│
├── private 数据
│   └── 对象内部状态
│
└── public 函数
    └── 外部操作对象的接口
```

例如：

```cpp
class Box {
public:
    int fill(int value) {
        contents_ = value;
        return contents_;
    }

private:
    int contents_ = 0;
};
```

外部：

```cpp
Box box;
box.fill(4);
```

而不是：

```cpp
box.contents_ = 4;   // 不允许
```

---

# `struct` 与 `class` 对比

| 特性                    | `struct` | `class`   |
| --------------------- | -------- | --------- |
| 可以定义成员变量              | 是        | 是         |
| 可以定义成员函数              | 是        | 是         |
| 可以使用 `public/private` | 是        | 是         |
| 默认成员访问权限              | `public` | `private` |
| 常见用途                  | 简单数据对象   | 封装数据和行为   |

因此不要把它们理解成：

```text
struct = 只有变量
class = 才能有函数
```

这是错误的。

真正最基本的语言层面区别是：

```cpp
struct → 默认 public
class  → 默认 private
```

---

# 本节最需要掌握的语法

```cpp
class TypeName {
public:
    ReturnType method(ParameterType parameter) {
        // 可以访问 private 成员
    }

private:
    MemberType member_;
};
```

使用：

```cpp
TypeName object;
object.method(argument);
```

核心思想：

> `private` 保存对象内部状态，`public` 提供外部使用对象的接口。
