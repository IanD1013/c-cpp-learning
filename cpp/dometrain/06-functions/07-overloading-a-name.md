# C++ 函数重载（Function Overloading）

## 1. 什么是函数重载

C++ 允许多个函数使用**同一个函数名**，只要它们的**参数列表不同**。

例如：

```cpp
static int total(int a, int b) {
    return a + b;
}

static int total(int a, int b, int c) {
    return total(a, b) + c;
}
```

这里有两个 `total`：

```cpp
total(int, int)
total(int, int, int)
```

它们名字相同，但参数数量不同，因此可以同时存在。

这种“同名但参数列表不同”的函数集合称为 **overload set（重载集合）**。

---

## 2. 什么样的区别可以构成重载

### 参数数量不同

```cpp
int f(int a);
int f(int a, int b);
```

这是合法重载。

### 参数类型不同

```cpp
int f(int a);
int f(double a);
```

也是合法重载。

例如：

```cpp
f(10);    // 调用 f(int)
f(3.14);  // 调用 f(double)
```

---

## 3. 返回值类型不能区分重载

下面是不合法的：

```cpp
int total(int a, int b);
double total(int a, int b);
```

虽然返回值不同，但参数列表完全相同。

C++ **不会使用返回值类型来决定调用哪个函数**，因此这会被视为重复定义。

原因很直观：

```cpp
total(2, 3);
```

编译器无法仅根据这行代码判断应该调用：

```cpp
int total(...)
```

还是：

```cpp
double total(...)
```

因此：

> 函数重载主要由参数列表决定，而不是返回值类型。

---

## 4. Overload Resolution：重载解析

当代码调用一个重载函数时，编译器需要决定到底调用哪个版本，这个过程称为：

> **overload resolution（重载解析）**

例如：

```cpp
static int total(int a, int b) {
    return a + b;
}

static int total(int a, int b, int c) {
    return total(a, b) + c;
}
```

调用：

```cpp
total(2, 3);
```

传入两个参数，所以匹配：

```cpp
total(int, int)
```

调用：

```cpp
total(2, 3, 4);
```

传入三个参数，所以匹配：

```cpp
total(int, int, int)
```

---

## 5. 重载解析发生在编译阶段

函数调用具体选择哪个版本，是在**编译时**决定的。

例如：

```cpp
int x = total(2, 3);
```

编译器看到两个参数后，就已经决定调用：

```cpp
total(int, int)
```

后面如何使用返回值不会改变这个决定。

---

## 6. 重载函数之间可以互相调用

这是非常常见的写法：

```cpp
static int total(int a, int b, int c) {
    return total(a, b) + c;
}
```

这里的：

```cpp
total(a, b)
```

仍然会正常进行一次 overload resolution。

因为只有两个参数，所以调用：

```cpp
total(int, int)
```

这种写法可以避免重复代码。

例如求三个数中的最小值，可以写成：

```cpp
static int minOf(int a, int b, int c) {
    return minOf(minOf(a, b), c);
}
```

其中三参数版本复用了二参数版本的逻辑。

---

## 7. 没有匹配的重载会发生编译错误

假设只有：

```cpp
int total(int a, int b);
int total(int a, int b, int c);
```

那么：

```cpp
total(7);
```

会编译失败。

因为：

* 两参数版本需要 2 个参数
* 三参数版本需要 3 个参数
* 当前只有 1 个参数

没有任何合法候选函数。

---

## 8. 如果多个重载同样匹配，也可能编译失败

如果编译器发现两个函数都可以匹配，而且无法判断哪个更好，就会产生：

> **ambiguous call（调用歧义）**

核心原则：

> 一个函数调用必须最终得到唯一的最佳匹配。

---

## 9. 为什么这道题不适合用默认参数代替重载

可能会想到：

```cpp
static int minOf(int a, int b, int c = ???);
```

然后：

```cpp
minOf(a, b);
```

省略第三个参数。

但这道题不能安全地这么做。

假设使用：

```cpp
c = 1000
```

如果：

```cpp
minOf(1000, 1000)
```

还勉强正常，但如果数据范围发生变化，就可能出问题。

如果使用：

```cpp
c = -1000
```

那么：

```cpp
minOf(8, 5)
```

会变成：

```cpp
minOf(8, 5, -1000)
```

答案错误地变成 `-1000`。

也就是说：

> 不存在一个固定的默认值，可以保证永远不影响“取最小值”的结果。

而且默认参数也不能简单写成另一个参数：

```cpp
int minOf(int a, int b, int c = a); // 不允许这样使用参数 a
```

因此，这种场景更适合使用**函数重载**。

---

## 10. 本题的设计

需要实现两个函数：

```cpp
static int minOf(int a, int b);

static int minOf(int a, int b, int c);
```

第一个：

```cpp
minOf(a, b)
```

返回两个数中的较小值。

第二个：

```cpp
minOf(a, b, c)
```

返回三个数中的最小值。

推荐让三参数版本复用两参数版本，而不是重复写比较逻辑。

例如：

```cpp
minOf(minOf(a, b), c)
```

执行过程：

```cpp
minOf(8, 5, 3)
```

首先：

```cpp
minOf(8, 5)
```

得到：

```text
5
```

然后：

```cpp
minOf(5, 3)
```

得到：

```text
3
```

最终结果：

```text
3
```

---

# 总结

函数重载的核心是：

```text
相同函数名
+
不同参数列表
```

例如：

```cpp
int minOf(int, int);
int minOf(int, int, int);
```

编译器会根据调用时的参数数量和类型进行 **overload resolution**：

```cpp
minOf(9, 4);       // 两参数版本
minOf(8, 5, 3);    // 三参数版本
```

返回值类型不能用来区分重载。

设计重载函数时，推荐让更复杂的版本复用简单版本：

```cpp
return minOf(minOf(a, b), c);
```

这样可以减少重复代码，并让逻辑更加清晰。
