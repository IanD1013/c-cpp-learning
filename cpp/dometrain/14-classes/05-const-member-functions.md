# C++：`const` 成员函数与 `const` 引用

## 1. 什么是 `const` 成员函数？

在 C++ 中，如果 `const` 写在成员函数的参数列表之后：

```cpp
int reading() const {
    return reading_;
}
```

这里的 `const` 表示：

> **调用这个成员函数不会修改当前对象的状态。**

例如：

```cpp
class Meter {
public:
    Meter(int reading) : reading_(reading) {}

    int reading() const {
        return reading_;
    }

private:
    int reading_;
};
```

`reading()` 只是读取 `reading_`，因此应该声明为 `const`。

---

## 2. `const` 写在哪里非常重要

正确：

```cpp
int reading() const;
```

这里的 `const` 修饰的是 **成员函数对当前对象的操作**。

而：

```cpp
const int reading();
```

含义完全不同。

它只是说：

> 返回一个 `const int`。

它**并没有保证函数不会修改对象**。

因此，如果题目要求：

```cpp
degrees() const
quarterTurns() const
```

必须把 `const` 写在参数列表后面。

---

## 3. `const` 成员函数中不能修改成员变量

例如：

```cpp
class Meter {
private:
    int reading_;

public:
    int reading() const {
        reading_ = 0;  // ❌ 编译错误
        return reading_;
    }
};
```

因为：

```cpp
reading() const
```

已经承诺不会修改当前对象。

可以把它粗略理解成：在这个函数里面，当前对象 `*this` 被视为只读对象。

概念上类似：

```cpp
const Meter* this;
```

因此：

```cpp
reading_ = 0;
```

是不允许的。

---

## 4. `const` 成员函数仍然可以使用局部变量

`const` 并不是说函数内部什么都不能修改。

例如：

```cpp
int halves() const {
    int value = reading_;
    value /= 2;
    return value;
}
```

这里修改：

```cpp
value
```

完全合法，因为它只是局部变量。

限制的是：

> **不能修改当前对象的普通成员变量。**

---

## 5. `const` 成员函数只能调用其他 `const` 成员函数

例如：

```cpp
class Meter {
public:
    int reading() const {
        return reading_;
    }

    int halves() const {
        return reading() / 2;
    }

private:
    int reading_;
};
```

这里：

```cpp
halves() const
```

调用：

```cpp
reading()
```

是合法的，因为 `reading()` 也是 `const`。

---

如果把它改成：

```cpp
int reading() {
    return reading_;
}
```

那么：

```cpp
int halves() const {
    return reading();  // ❌
}
```

就不能编译。

原因是：

```cpp
halves() const
```

承诺不会修改对象。

但它调用的：

```cpp
reading()
```

没有作出这个承诺。

编译器无法确定 `reading()` 会不会偷偷修改对象，所以禁止调用。

---

# 6. 为什么 `const` 引用需要 `const` 成员函数？

考虑：

```cpp
int readHalves(const Meter &meter) {
    return meter.halves();
}
```

参数：

```cpp
const Meter &meter
```

表示：

> `readHalves()` 只能读取这个 `Meter`，不能通过 `meter` 修改它。

因此通过 `meter` 只能调用 `const` 成员函数。

如果：

```cpp
int halves() const;
```

则合法：

```cpp
meter.halves();
```

如果：

```cpp
int halves();
```

则：

```cpp
meter.halves();  // ❌
```

不能编译。

---

# 7. 为什么只读参数通常写成 `const T&`

例如：

```cpp
int readHalves(const Meter &meter)
```

这里同时使用了：

```cpp
&
```

和：

```cpp
const
```

分别解决两个问题。

### `&`：避免复制对象

如果写：

```cpp
int readHalves(Meter meter)
```

调用函数时会复制一个 `Meter`。

使用：

```cpp
const Meter &meter
```

则直接引用原来的对象，没有额外复制。

### `const`：保证函数不会修改对象

```cpp
const Meter &meter
```

意味着：

```cpp
meter
```

是只读的。

因此，对于只需要读取对象的函数，一个非常常见的写法是：

```cpp
void function(const SomeType &object);
```

可以记成：

> **不需要修改对象 → 优先考虑 `const T&`。**

---

# 8. 本题中的 `Angle`

题目要求：

```cpp
degrees() const
```

返回创建 `Angle` 时传入的角度：

```cpp
int degrees() const {
    return degrees_;
}
```

另一个函数：

```cpp
quarterTurns() const
```

计算：

```cpp
degrees() / 90
```

例如：

```text
89   / 90 → 0
90   / 90 → 1
359  / 90 → 3
-90  / 90 → -1
-45  / 90 → 0
```

---

# 9. C++ 整数除法：向 0 截断

这里：

```cpp
degrees() / 90
```

两边都是 `int`，所以执行的是整数除法。

C++ 的整数除法会：

> **向 0 截断小数部分。**

例如：

```cpp
89 / 90
```

数学结果：

```text
0.988...
```

得到：

```cpp
0
```

而：

```cpp
-45 / 90
```

数学结果：

```text
-0.5
```

C++ 向 0 截断，因此得到：

```cpp
0
```

而不是 `-1`。

注意它和数学里的 `floor()` 不一样：

```text
-0.5 向下取整 → -1
-0.5 向 0 截断 → 0
```

---

# 10. `const std::vector<int>&`

题目要求：

```cpp
static std::vector<int> quarterTurnsOf(
    const std::vector<int> &degrees
)
```

其中：

```cpp
const std::vector<int> &degrees
```

意味着：

* 不复制整个 `vector`
* 函数不能修改传入的 `vector`

这是读取容器参数时非常常见的写法。

例如：

```cpp
for (int degree : degrees) {
    ...
}
```

只是读取其中的每个数字。

---

# 11. 本题最重要的 `const` 关系

可以把调用关系记成：

```text
const Angle
    ↓
只能调用 const 成员函数
    ↓
quarterTurns() const
    ↓
调用
    ↓
degrees() const
```

以及：

```text
const Angle &
    ↓
只能调用 const 成员函数
    ↓
angle.quarterTurns()
```

所以这些 `const` 是互相关联的。

只要其中某个地方漏掉：

```cpp
degrees()
```

或者：

```cpp
quarterTurns()
```

而不是：

```cpp
degrees() const
quarterTurns() const
```

就可能导致整个调用链无法编译。

---

# 12. 核心记忆

```cpp
ReturnType function() const
```

中的 `const` 表示：

> **这个成员函数不会修改当前对象。**

最常见的场景：

```cpp
class A {
public:
    int getValue() const {
        return value_;
    }

private:
    int value_;
};
```

然后：

```cpp
void printValue(const A &a) {
    std::cout << a.getValue();
}
```

核心规则：

```text
const 对象 / const 引用
        ↓
只能调用
        ↓
const 成员函数
```

所以对于 getter、查询函数、计算但不改变对象状态的函数，通常都应该考虑加：

```cpp
const
```
