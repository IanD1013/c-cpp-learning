# C++ 参数传递：Pass by Value（值传递）

## 1. 核心结论

在下面这种普通函数参数中：

```cpp
static int shrink(int amount)
```

参数 `amount` **不是调用者变量本身**。

当函数被调用时：

> 实参（argument）的值会被复制一份，用来初始化形参（parameter）。

因此，函数内部修改参数：

```cpp
amount = amount - 1;
```

修改的只是这个函数自己的那份副本，不会修改调用者原来的变量。

这种机制称为：

```text
Pass by Value
值传递
```

---

# 2. 最基本的例子

```cpp
static int shrink(int amount) {
    amount = amount - 1;
    return amount;
}
```

调用：

```cpp
int size = 10;
int smaller = shrink(size);
```

最终结果：

```cpp
size == 10
smaller == 9
```

为什么？

调用：

```cpp
shrink(size)
```

可以理解成函数执行前发生了类似这样的事情：

```cpp
int amount = size;
```

此时：

```text
size
↓
10

amount
↓
10
```

虽然两个变量当前保存的值都是 `10`，但它们是两个不同的变量。

之后执行：

```cpp
amount = amount - 1;
```

结果变成：

```text
size
↓
10

amount
↓
9
```

所以：

```cpp
return amount;
```

返回 `9`。

调用者将它保存：

```cpp
int smaller = shrink(size);
```

最终：

```cpp
size == 10
smaller == 9
```

---

# 3. 参数可以理解成函数自己的局部变量

这是理解值传递最重要的方法之一。

例如：

```cpp
static int shrink(int amount) {
    amount = amount - 1;
    return amount;
}
```

这里的：

```cpp
amount
```

本质上就是 `shrink` 函数内部的一个局部变量。

区别只是：

普通局部变量可能这样初始化：

```cpp
int amount = 10;
```

而函数参数是在调用函数时初始化：

```cpp
shrink(size);
```

相当于：

```cpp
int amount = size;
```

所以在进入函数以后，可以把参数当成普通局部变量使用：

```cpp
amount = 3;
amount++;
amount *= 10;
```

这些操作默认都不会修改调用者的变量。

---

# 4. 修改形参 ≠ 修改实参

例如：

```cpp
static int change(int x) {
    x = 100;
    return x;
}
```

调用：

```cpp
int number = 5;

int result = change(number);
```

最终：

```cpp
number == 5
result == 100
```

执行过程：

```text
调用前：

number = 5

调用 change(number)

复制：

x = 5

函数内部：

x = 100

返回：

100
```

但：

```cpp
number
```

从来没有被修改。

---

# 5. `return` 才是普通值传递函数把结果交还给调用者的方式

例如：

```cpp
static int shrink(int amount) {
    amount = amount - 1;
    return amount;
}
```

调用：

```cpp
int size = 10;
shrink(size);
```

虽然 `shrink` 算出了 `9`，但如果你没有保存返回值：

```cpp
shrink(size);
```

那么这个 `9` 就被丢掉了。

`size` 仍然是：

```cpp
10
```

如果想保存结果，需要：

```cpp
int smaller = shrink(size);
```

或者修改原变量：

```cpp
size = shrink(size);
```

这时 `size` 才会变成：

```cpp
9
```

注意：

```cpp
size = shrink(size);
```

并不是 `shrink` 在函数内部修改了 `size`。

真正发生的是：

```text
1. size 的值 10 被复制给 amount

2. shrink 计算并 return 9

3. 调用表达式 shrink(size) 的结果变成 9

4. 外面的赋值执行：

   size = 9
```

所以修改 `size` 的其实是调用者中的：

```cpp
size = ...
```

---

# 6. 参数名和调用者变量名没有关系

例如：

```cpp
static int shrink(int amount) {
    amount--;
    return amount;
}
```

调用：

```cpp
int size = 10;

int result = shrink(size);
```

参数叫：

```cpp
amount
```

调用者变量叫：

```cpp
size
```

名字完全不同，没有问题。

甚至这样也可以：

```cpp
int amount = 10;

int result = shrink(amount);
```

虽然两边都叫 `amount`，它们仍然是两个不同的变量：

```text
caller 的 amount
≠
shrink 的 amount
```

名字相同不代表它们是同一个变量。

---

# 7. 值传递不只是 `int`

这种规则并不只适用于整数。

例如：

```cpp
static void change(double value) {
    value = 3.14;
}
```

调用：

```cpp
double x = 10.0;
change(x);
```

调用完成后：

```cpp
x == 10.0
```

同样：

```cpp
static void change(std::string text) {
    text = "hello";
}
```

调用：

```cpp
std::string name = "Ian";

change(name);
```

调用结束后：

```cpp
name == "Ian"
```

因为这里：

```cpp
std::string text
```

仍然是值传递。

可以理解为复制了一份字符串：

```text
name
↓
"Ian"

复制

text
↓
"Ian"
```

随后修改：

```cpp
text = "hello";
```

只会修改副本。

---

# 8. 但复制复杂对象可能有成本

从语义上来说：

```cpp
int
double
std::string
```

作为普通参数时都是值传递。

不过成本可能不同。

例如：

```cpp
static void process(std::string text)
```

可能需要复制整个字符串。

如果字符串非常大，这种复制可能没有必要。

所以以后会经常看到：

```cpp
static void process(const std::string& text)
```

这里的：

```cpp
&
```

表示引用。

它就不再是普通的值传递了。

这是后面非常重要的 C++ 概念：

```text
Pass by Value
值传递

vs

Pass by Reference
引用传递
```

当前这道题只需要掌握前者。

---

# 9. `tripleValue` 示例

题目要求实现：

```cpp
tripleValue
```

它接受一个整数：

```cpp
value
```

然后将它变成原来的三倍，再返回。

可以写成：

```cpp
static int tripleValue(int value) {
    value = value * 3;
    return value;
}
```

例如：

```cpp
int x = 5;

int result = tripleValue(x);
```

执行过程：

```text
x = 5

调用：
tripleValue(x)

复制：
value = 5

修改：
value = 15

返回：
15
```

最终：

```cpp
x == 5
result == 15
```

`x` 不会变成 `15`。

---

# 10. 用调用栈思维理解 `gapAfterTripling`

假设函数：

```cpp
static int gapAfterTripling(int start)
```

需要：

1. 把 `start` 交给 `tripleValue`
2. 保存得到的三倍值
3. 返回：

```cpp
start - tripled
```

例如：

```cpp
static int gapAfterTripling(int start) {
    int tripled = tripleValue(start);
    return start - tripled;
}
```

注意调用：

```cpp
tripleValue(start)
```

不会修改当前函数自己的：

```cpp
start
```

因为 `tripleValue` 的参数：

```cpp
int value
```

仍然是一个副本。

---

# 11. 示例：`start = 5`

调用：

```cpp
gapAfterTripling(5)
```

首先：

```text
start = 5
```

调用：

```cpp
tripleValue(start)
```

于是：

```text
value = 5
```

然后：

```cpp
value = value * 3;
```

得到：

```text
value = 15
```

返回：

```text
15
```

于是：

```cpp
tripled = 15
```

但是：

```cpp
start
```

仍然是：

```cpp
5
```

最后：

```cpp
return start - tripled;
```

即：

```cpp
return 5 - 15;
```

得到：

```cpp
-10
```

---

# 12. 示例：`start = 0`

```cpp
tripleValue(0)
```

返回：

```cpp
0
```

所以：

```cpp
0 - 0 = 0
```

答案：

```cpp
0
```

---

# 13. 示例：`start = -7`

首先：

```cpp
start = -7
```

调用：

```cpp
tripleValue(-7)
```

得到：

```cpp
-21
```

原本的：

```cpp
start
```

仍然是：

```cpp
-7
```

因此：

```cpp
-7 - (-21)
```

等于：

```cpp
-7 + 21
```

得到：

```cpp
14
```

---

# 14. 数学上可以进一步化简

题目的结果其实是：

```text
start - 3 × start
```

因此：

```text
= -2 × start
```

例如：

```text
start = 5

-2 × 5 = -10
```

以及：

```text
start = -7

-2 × (-7) = 14
```

不过即使数学上可以直接写：

```cpp
return -2 * start;
```

如果题目明确要求练习：

```cpp
tripleValue
```

通常仍然应该调用它：

```cpp
int tripled = tripleValue(start);
return start - tripled;
```

因为自动测试可能会单独测试 `tripleValue`。

---

# 15. Helper Function 可能会被单独测试

这是在线编程题里很重要的一点。

如果题目提供：

```cpp
static int tripleValue(int value)
```

不要因为最终函数可以绕过它，就随便实现，例如：

```cpp
static int tripleValue(int value) {
    return value;
}
```

然后：

```cpp
static int gapAfterTripling(int start) {
    return -2 * start;
}
```

虽然：

```cpp
gapAfterTripling
```

可能返回正确答案，但 grader 很可能会单独测试：

```cpp
tripleValue(5)
```

正确答案应该是：

```cpp
15
```

而你的函数却返回：

```cpp
5
```

于是仍然会失败。

所以原则是：

> 题目要求实现的每一个函数，都应该满足自己的函数契约，而不只是让最终函数碰巧返回正确答案。

---

# 16. 为什么 `tripleValue` 的位置很重要

题目说明：

```cpp
tripleValue
```

应该放在：

```cpp
struct Solution
```

外面。

例如：

```cpp
static int tripleValue(int value) {
    value = value * 3;
    return value;
}

struct Solution {
    static int gapAfterTripling(int start) {
        int tripled = tripleValue(start);
        return start - tripled;
    }
};
```

这里：

```cpp
tripleValue
```

是一个独立函数。

grader 可以直接调用：

```cpp
tripleValue(5);
```

如果你把它移进去：

```cpp
struct Solution {
    static int tripleValue(int value) {
        ...
    }
};
```

它现在变成了：

```cpp
Solution::tripleValue
```

调用方式必须变成：

```cpp
Solution::tripleValue(5);
```

而如果 grader 期待：

```cpp
tripleValue(5);
```

编译就会失败。

因此：

> 不要随意改变 starter code 中函数所属的作用域。

---

# 17. 为什么不要自己写 `main`

在线评测系统通常会偷偷提供类似：

```cpp
int main() {
    // 调用你的 Solution
    // 执行测试
}
```

所以你的任务通常只是实现：

```cpp
struct Solution
```

以及题目指定的 helper functions。

如果你自己又写：

```cpp
int main() {
}
```

那么程序里会出现两个：

```cpp
main
```

于是可能产生编译或链接错误。

因此：

> 如果题目没有要求你实现 `main`，通常不要自行添加 `main`。

---

# 18. Method Signature 是什么意思

题目给出的：

```cpp
static int gapAfterTripling(int start)
```

叫做函数签名 / 方法签名要求。

它告诉你 grader 预计找到一个：

```text
函数名：gapAfterTripling

参数数量：1

参数类型：int

返回类型：int

static：是
```

所以不要随意改成：

```cpp
int gapAfterTripling(double start)
```

或者：

```cpp
static double gapAfterTripling(int start)
```

或者：

```cpp
static int gap(int start)
```

因为 grader 是按照指定接口调用你的代码的。

---

# 19. 本节最重要的执行模型

看到：

```cpp
int x = 10;
int y = someFunction(x);
```

如果函数是：

```cpp
static int someFunction(int value)
```

应该在脑中理解为：

```text
x
│
│ 当前值 10
│
▼
复制
│
▼
value = 10

函数只操作 value

return 某个值

返回值替换函数调用表达式

y = 返回值
```

而不是：

```text
value 就是 x 的另一个名字
```

后者只有在使用引用等其他机制时才可能成立。

---

# 20. 一句话总结

对于普通参数：

```cpp
void f(int x)
```

记住：

> `x` 是调用者传入值的一份副本；修改 `x` 不会修改调用者变量，函数计算出的结果通常通过 `return` 返回。
