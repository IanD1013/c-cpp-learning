# C++ `static` 类成员：属于整个类的共享数据

## 1. 普通成员属于对象

普通成员变量是**每个对象各自拥有一份**的。

```cpp
class Counter {
public:
    Counter() : value_(0) {}

private:
    int value_;
};
```

如果创建：

```cpp
Counter a;
Counter b;
```

那么实际上有两份独立的 `value_`：

```text
a
└── value_

b
└── value_
```

修改 `a.value_` 不会影响 `b.value_`。

---

# 2. `static` 成员属于整个类

如果成员变量前面加上 `static`：

```cpp
class Machine {
private:
    static int limit_;
};
```

它就不再属于某一个对象，而是**整个 `Machine` 类共享一份**。

可以理解成：

```text
Machine 类
└── static limit_ = 10

Machine a ─┐
Machine b ─┼── 都访问同一个 limit_
Machine c ─┘
```

无论创建多少个 `Machine` 对象，都只有一个 `limit_`。

因此：

> 普通成员描述“这个对象的数据”，
> `static` 成员描述“整个类共享的数据”。

---

# 3. `static` 成员变量需要定义

经典 C++ 写法通常分成两步。

## 第一步：在类中声明

```cpp
class Machine {
private:
    static int limit_;
};
```

这里的：

```cpp
static int limit_;
```

主要是在告诉编译器：

> `Machine` 类有一个叫 `limit_` 的静态整数成员。

---

## 第二步：在类外定义

```cpp
int Machine::limit_ = 10;
```

这里才真正为这个变量提供定义和初始值。

完整代码：

```cpp
class Machine {
public:
    static int limit() {
        return limit_;
    }

private:
    static int limit_;
};

int Machine::limit_ = 10;
```

其中：

```cpp
Machine::limit_
```

表示：

> `Machine` 类作用域里的 `limit_`。

---

# 4. 为什么缺少定义会出现 linker error？

如果只写：

```cpp
class Machine {
private:
    static int limit_;
};
```

但没有：

```cpp
int Machine::limit_ = 10;
```

代码可能可以完成**编译（compile）**，但最终在**链接（link）**阶段失败，例如：

```text
undefined reference to Machine::limit_
```

原因是：

```text
编译器：
“我知道 Machine::limit_ 存在。”

链接器：
“但是它到底定义在哪里？”
```

因此要区分：

```text
声明 declaration
        ↓
告诉编译器它存在

定义 definition
        ↓
真正提供这个变量
```

---

# 5. `static` 成员函数

成员函数也可以声明为 `static`：

```cpp
class Machine {
public:
    static int limit() {
        return limit_;
    }

private:
    static int limit_;
};
```

调用时不需要创建对象：

```cpp
int x = Machine::limit();
```

而不是必须：

```cpp
Machine machine;
machine.limit();
```

因为 `limit()` 属于类，而不是某一个具体对象。

典型写法：

```cpp
ClassName::function()
```

例如：

```cpp
Machine::limit();
Tracked::created();
```

---

# 6. `static` 成员函数没有具体对象

普通成员函数：

```cpp
machine.run();
```

执行时知道自己正在操作哪个对象，因此内部可以访问：

```cpp
value_
```

本质上相当于：

```text
machine.value_
```

但：

```cpp
Machine::limit();
```

调用时根本没有 `machine` 对象。

所以 `static` 成员函数没有“当前对象”，也就没有普通成员函数中的 `this`。

因此静态函数不能直接访问普通成员变量：

```cpp
class Machine {
private:
    int speed_;

public:
    static int getSpeed() {
        return speed_;   // ❌
    }
};
```

因为问题是：

> 到底是哪一个 Machine 对象的 `speed_`？

没有答案。

但是它可以访问静态成员：

```cpp
class Machine {
private:
    static int limit_;

public:
    static int limit() {
        return limit_;   // ✅
    }
};
```

因为整个类只有一份 `limit_`。

---

# 7. 为什么对象计数器适合使用 `static`？

假设希望统计：

> 到目前为止创建过多少个 `Tracked` 对象？

这个数字不是某一个对象自己的属性，而是整个类的历史。

例如：

```cpp
Tracked a;
Tracked b;
Tracked c;
```

应该得到：

```text
created = 3
```

如果使用普通成员：

```cpp
int created_;
```

那么：

```text
a.created_ = ?
b.created_ = ?
c.created_ = ?
```

每个对象都有自己的一份计数器，无法自然表示整个类总共创建了多少对象。

因此应该使用：

```cpp
static int created_;
```

所有对象共享同一个数字：

```text
Tracked 类
└── created_ = 3

a ─┐
b ─┼── 共用 created_
c ─┘
```

---

# 8. Constructor 正好负责增加计数

构造函数：

```cpp
Tracked();
```

每创建一个对象就执行一次。

所以：

```cpp
Tracked::Tracked() {
    created_ += 1;
}
```

天然就可以统计对象创建次数。

例如：

```cpp
Tracked a;   // created_ = 1
Tracked b;   // created_ = 2
Tracked c;   // created_ = 3
```

然后通过：

```cpp
Tracked::created()
```

读取共享计数：

```cpp
static int created() {
    return created_;
}
```

---

# 9. 完整结构

这种“统计创建对象数量”的类通常写成：

```cpp
class Tracked {
public:
    Tracked();

    static int created();

private:
    static int created_;
};
```

类外定义静态成员：

```cpp
int Tracked::created_ = 0;
```

构造函数：

```cpp
Tracked::Tracked() {
    created_ += 1;
}
```

查询函数：

```cpp
int Tracked::created() {
    return created_;
}
```

---

# 10. 执行过程

假设：

```cpp
Tracked a;
Tracked b;
Tracked c;
```

过程是：

```text
程序开始

created_ = 0

构造 a
created_ = 1

构造 b
created_ = 2

构造 c
created_ = 3

Tracked::created()
→ 3
```

核心思想就是：

```text
所有对象
   ↓
运行自己的 constructor
   ↓
修改同一个 static created_
```

---

# 11. `static` 最重要的判断方式

看到一个数据时，可以问：

> 这个数据描述的是“某一个对象”，还是“整个类”？

如果属于单个对象：

```cpp
int health_;
int speed_;
std::string name_;
```

使用普通成员。

如果属于整个类：

```cpp
static int created_;
static int maxObjects_;
static int globalLimit_;
```

考虑使用 `static`。

例如：

| 数据          | 更适合      |
| ----------- | -------- |
| 某辆汽车的速度     | 普通成员     |
| 某个用户的名字     | 普通成员     |
| 某个账户的余额     | 普通成员     |
| 总共创建过多少辆汽车  | `static` |
| 所有对象共享的最大限制 | `static` |
| 全部实例共享的配置   | `static` |

---

# 12. 一个容易混淆的地方：`static` 和 `const`

这两个关键字解决的是完全不同的问题。

```cpp
static
```

回答：

> 这个东西属于对象还是属于整个类？

而成员函数后的：

```cpp
const
```

回答：

> 这个函数会不会修改当前对象？

例如：

```cpp
int value() const;
```

是普通对象成员函数，只是承诺不修改对象。

而：

```cpp
static int created();
```

是属于整个类的函数，甚至没有当前对象。

因此：

```text
static → 所有权 / 是否属于对象

const → 是否允许修改当前对象
```

不要把两者混在一起。

---

# 13. 现代 C++：`inline static`

从 C++17 开始，还可以直接写：

```cpp
class Machine {
private:
    inline static int limit_ = 10;
};
```

这样通常不再需要额外写：

```cpp
int Machine::limit_ = 10;
```

不过如果课程正在训练经典的：

```cpp
static int value_;
```

加上：

```cpp
int ClassName::value_ = 0;
```

这种写法，按照课程要求写即可。

---

# 总结

最需要记住的是：

```text
普通成员变量
→ 每个对象各自一份

static 成员变量
→ 整个类共享一份
```

以及：

```text
普通成员函数
→ 通过对象调用
→ 有具体对象
→ 有 this

static 成员函数
→ 可以通过 ClassName::function() 调用
→ 不依赖具体对象
→ 没有 this
```

对象计数器是 `static` 最经典的用途之一：

```cpp
constructor 被调用一次
        ↓
static created_ 加 1
        ↓
Tracked::created()
        ↓
得到整个类累计创建的对象数量
```
