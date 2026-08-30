# C++ 析构函数（Destructor）与对象生命周期

## 1. 什么是析构函数？

**析构函数（destructor）** 是当一个对象的生命周期结束时，由 C++ 自动调用的成员函数。

基本语法：

```cpp
class Handle {
public:
    Handle() {
        // 构造函数
    }

    ~Handle() {
        // 析构函数
    }
};
```

析构函数有几个固定特点：

```cpp
~ClassName()
```

* 名字是 `~` 加类名
* 没有返回类型
* 没有参数
* 通常由 C++ 自动调用

例如：

```cpp
class Marker {
public:
    ~Marker() {
    }
};
```

一般情况下，不需要手动写：

```cpp
obj.~Marker();
```

而是让对象生命周期结束时自动执行析构函数。

---

# 2. 局部对象什么时候被销毁？

对于局部对象：

```cpp
void test() {
    Marker a;
}
```

对象 `a` 的生命周期到函数的 `}` 为止。

执行到：

```cpp
}
```

时，C++ 会自动调用：

```cpp
a.~Marker();
```

因此可以理解成：

```text
进入作用域
    ↓
构造对象
    ↓
使用对象
    ↓
离开作用域
    ↓
自动调用析构函数
```

---

# 3. 同一作用域中的对象：逆序销毁

如果对象按照：

```cpp
{
    Marker one;
    Marker two;
}
```

的顺序构造，那么销毁顺序是：

```text
构造：
one
two

销毁：
two
one
```

也就是：

> **后构造，先析构（Last In, First Out）**

可以记成一个栈：

```text
构造：

one
two  ← 最后进入

析构：

two  ← 最先出去
one
```

例如：

```cpp
{
    Marker a("A");
    Marker b("B");
    Marker c("C");
}
```

构造顺序：

```text
A
B
C
```

析构顺序：

```text
C
B
A
```

---

# 4. 嵌套作用域会提前结束对象生命周期

考虑：

```cpp
{
    Marker a("A");

    {
        Marker b("B");
    }

    Marker c("C");
}
```

执行过程不是：

```text
A
B
C
C
B
A
```

而是：

```text
构造 A
构造 B
析构 B
构造 C
析构 C
析构 A
```

因为 `B` 属于内部作用域：

```cpp
{
    Marker b("B");
}
```

内部 `}` 一到，`B` 就已经死亡。

所以最终顺序：

```text
+A+B-B+C-C-A
```

这里非常重要：

> 对象的生命周期由它所在的作用域决定，而不是由整个函数决定。

---

# 5. 如何观察构造和析构顺序？

对象析构本身不会自动打印任何东西。

例如：

```cpp
Marker a;
```

当 `a` 被销毁时，默认不会出现：

```text
a destroyed
```

如果想观察生命周期，可以让构造函数和析构函数主动记录信息。

例如：

```cpp
class Marker {
public:
    Marker(const std::string &label)
        : label_(label) {
        record_ += "+" + label_;
    }

    ~Marker() {
        record_ += "-" + label_;
    }

private:
    std::string label_;

    static std::string record_;
};
```

假设：

```cpp
{
    Marker a("A");
}
```

构造时：

```text
+A
```

析构时：

```text
-A
```

最终：

```text
+A-A
```

---

# 6. 为什么这里要用 static 成员？

如果日志是普通成员：

```cpp
std::string record_;
```

那么每个 `Marker` 都会拥有自己的 `record_`。

例如：

```cpp
Marker a("A");
Marker b("B");
```

会变成：

```text
a.record_
b.record_
```

两个不同变量。

但我们希望：

```text
所有 Marker
    ↓
共同写入同一个日志
```

因此使用：

```cpp
static std::string record_;
```

`static` 成员属于整个类，而不是某一个对象。

例如：

```cpp
class Marker {
private:
    static std::string record_;
};
```

整个程序只有：

```text
Marker::record_
```

这一份。

所以：

```cpp
Marker a("A");
Marker b("B");
```

可以共同记录：

```text
+A+B-B-A
```

---

# 7. 析构顺序示例

假设构造函数记录：

```text
+标签
```

析构函数记录：

```text
-标签
```

## 一个对象

```cpp
{
    Marker a("A");
}
```

结果：

```text
+A-A
```

---

## 两个对象

```cpp
{
    Marker a("A");
    Marker b("B");
}
```

构造：

```text
+A+B
```

析构：

```text
-B-A
```

最终：

```text
+A+B-B-A
```

---

## 三个对象

```cpp
{
    Marker a("A");
    Marker b("B");
    Marker c("C");
}
```

最终：

```text
+A+B+C-C-B-A
```

---

## B 位于嵌套作用域

```cpp
{
    Marker a("A");

    {
        Marker b("B");
    }

    Marker c("C");
}
```

执行：

```text
+A
+B
-B
+C
-C
-A
```

最终：

```text
+A+B-B+C-C-A
```

与普通三个对象：

```text
+A+B+C-C-B-A
```

明显不同。

原因就是：

```cpp
{
    Marker b("B");
}
```

的作用域在创建 `C` 之前已经结束。

---

# 8. 为什么要在函数返回之后读取记录？

这是一个很容易忽略的生命周期问题。

假设：

```cpp
std::string test() {
    Marker a("A");

    return Marker::record();
}
```

在计算：

```cpp
Marker::record()
```

的时候，`a` 仍然活着。

也就是说此时日志可能只有：

```text
+A
```

然后函数开始退出，才析构：

```text
-A
```

但是返回值已经计算完成了。

因此返回出去的可能只是：

```text
+A
```

而不是：

```text
+A-A
```

更安全的设计是：

```cpp
void build() {
    Marker a("A");
}

std::string test() {
    build();

    return Marker::record();
}
```

执行顺序：

```text
进入 build
+A
离开 build
-A

回到 test

读取 record
```

这时候得到完整结果：

```text
+A-A
```

---

# 9. 核心规律总结

### 构造

对象创建时调用：

```cpp
ClassName(...)
```

### 析构

对象生命周期结束时调用：

```cpp
~ClassName()
```

### 同一作用域

```cpp
A
B
C
```

构造顺序：

```text
A → B → C
```

析构顺序：

```text
C → B → A
```

即：

> **构造正序，析构逆序。**

### 嵌套作用域

内部作用域中的对象：

```cpp
{
    ...
}
```

会在内部 `}` 到达时立即析构。

---

# 10. 更重要的实际意义：RAII

析构函数真正重要的地方，并不只是“打印对象什么时候消失”。

C++ 中非常核心的思想叫：

> **RAII（Resource Acquisition Is Initialization）**

简单理解：

```text
对象创建
→ 获得资源

对象销毁
→ 自动释放资源
```

例如：

```cpp
{
    std::ofstream file("data.txt");

    // 使用文件
}
```

离开作用域后：

```cpp
file
```

被析构，文件资源会自动关闭。

类似思想还用于：

```text
动态内存
文件
锁
socket
数据库连接
系统句柄
```

因此析构函数的核心价值是：

> **把资源的释放绑定到对象生命周期上。**

这样就不用依赖程序员记住：

```cpp
open();
...
close();
```

而可以依赖：

```text
对象活着 → 资源有效
对象死亡 → 自动清理
```

这也是 C++ 对象生命周期设计中最重要的思想之一。
