# C++ 函数定义：返回类型、函数名、参数与函数体

## 1. 一个函数定义的基本结构

C++ 中，一个普通函数定义可以理解为由以下几个核心部分组成：

```cpp
返回类型 函数名(参数列表) {
    函数体
}
```

例如：

```cpp
static int rangeWidth(int low, int high) {
    int width = high - low;
    return width;
}
```

这里可以拆成：

```text
static int rangeWidth(int low, int high)
│      │        │
│      │        └── 参数列表
│      └─────────── 函数名
└────────────────── 返回类型前的说明符 + 返回类型

{
    // 函数体
}
```

更准确地说：

* `int`：返回类型
* `rangeWidth`：函数名
* `(int low, int high)`：参数列表
* `{ ... }`：函数体
* `static`：额外的函数说明符，不属于返回类型本身

---

# 2. 返回类型 Return Type

```cpp
int rangeWidth(...)
```

最前面的 `int` 表示：

> 这个函数最终会返回一个 `int` 类型的值。

因此：

```cpp
return width;
```

中的 `width` 必须能够转换成 `int`。

例如：

```cpp
int getNumber() {
    return 10;
}
```

合法，因为 `10` 是 `int`。

如果函数不需要返回任何值，则可以使用：

```cpp
void printHello() {
    std::cout << "Hello";
}
```

`void` 表示：

> 这个函数没有返回值。

---

# 3. 函数名 Function Name

```cpp
rangeWidth
```

这是函数的名字。

以后调用它时：

```cpp
int result = rangeWidth(3, 10);
```

程序就是通过 `rangeWidth` 找到这个函数。

此时：

```cpp
result == 7
```

因为：

```cpp
10 - 3 == 7
```

---

# 4. 参数列表 Parameter List

函数定义：

```cpp
int rangeWidth(int low, int high)
```

参数列表是：

```cpp
(int low, int high)
```

这里有两个参数：

```cpp
int low
int high
```

每个参数通常写成：

```text
类型 参数名
```

也就是：

```cpp
int low
```

表示：

> 创建一个叫 `low` 的局部变量，它的类型是 `int`。

多个参数之间使用逗号：

```cpp
(int low, int high)
```

---

# 5. 参数和实参的区别

这是学习函数时非常重要的两个概念。

函数定义中的：

```cpp
int rangeWidth(int low, int high)
```

`low` 和 `high` 叫做：

> 参数（parameter / 形参）

调用函数时：

```cpp
rangeWidth(3, 10);
```

这里的：

```cpp
3
10
```

叫：

> 实参（argument）

调用发生时，可以粗略理解为：

```cpp
low = 3;
high = 10;
```

然后执行：

```cpp
int width = high - low;
```

也就是：

```cpp
int width = 10 - 3;
```

所以结果为：

```cpp
7
```

---

# 6. 参数按照位置匹配

参数非常重要的一点是：

> 实参按照从左到右的位置传给参数。

例如：

```cpp
int rangeWidth(int low, int high) {
    return high - low;
}
```

调用：

```cpp
rangeWidth(3, 10);
```

对应关系是：

```text
low  ← 3
high ← 10
```

因此：

```cpp
10 - 3 = 7
```

但如果调用：

```cpp
rangeWidth(10, 3);
```

那么：

```text
low  ← 10
high ← 3
```

结果就变成：

```cpp
3 - 10 = -7
```

所以：

> 参数的顺序会影响传入值分别进入哪个变量。

---

# 7. 参数名字由函数作者决定

例如：

```cpp
int rangeWidth(int low, int high)
```

也可以写成：

```cpp
int rangeWidth(int a, int b) {
    return b - a;
}
```

甚至：

```cpp
int rangeWidth(int x, int y) {
    return y - x;
}
```

它们的行为完全一样。

也就是说：

> 参数名只是函数内部为了方便理解而使用的变量名。

调用者并不知道这些参数内部叫什么。

调用者只需要：

```cpp
rangeWidth(3, 10);
```

而不需要知道参数名字是：

```cpp
low
high
```

---

# 8. 参数的作用域

例如：

```cpp
int rangeWidth(int low, int high) {
    int width = high - low;
    return width;
}
```

这里：

```cpp
low
high
width
```

都只存在于这个函数内部。

函数执行完以后，这些局部变量就结束生命周期。

因此外部代码不能直接访问：

```cpp
low
high
width
```

例如下面通常是不合法的：

```cpp
int rangeWidth(int low, int high) {
    int width = high - low;
    return width;
}

std::cout << width;   // 错误
```

因为 `width` 只存在于 `rangeWidth` 内部。

---

# 9. 函数体 Function Body

函数体由花括号包围：

```cpp
{
    ...
}
```

例如：

```cpp
int rangeWidth(int low, int high) {
    int width = high - low;
    return width;
}
```

函数被调用以后，就会从函数体的第一条语句开始执行。

执行顺序：

```cpp
int width = high - low;
```

然后：

```cpp
return width;
```

---

# 10. `return` 的作用

例如：

```cpp
return width;
```

有两个重要作用。

第一：

> 把一个值返回给调用者。

例如：

```cpp
int result = rangeWidth(3, 10);
```

函数：

```cpp
return 7;
```

以后，相当于：

```cpp
int result = 7;
```

第二：

> `return` 会立即结束当前函数。

例如：

```cpp
int example() {
    return 10;

    std::cout << "hello";
}
```

`return 10;` 执行以后函数已经结束，因此后面的代码不会正常执行。

可以把：

```cpp
return value;
```

理解为：

```text
把 value 交给调用者
+
立刻结束这个函数
```

---

# 11. `return` 和 `std::cout` 完全不同

初学时很容易混淆：

```cpp
return value;
```

和：

```cpp
std::cout << value;
```

二者完全不是一回事。

## `return`

```cpp
return 150;
```

表示：

> 函数的计算结果是 `150`，把它返回给调用者。

例如：

```cpp
int x = totalSeconds(2, 30);
```

得到：

```cpp
x == 150
```

---

## `std::cout`

```cpp
std::cout << 150;
```

只是：

> 把 `150` 打印到屏幕。

它不会自动成为函数返回值。

例如：

```cpp
int totalSeconds(int minutes, int seconds) {
    std::cout << minutes * 60 + seconds;
}
```

虽然屏幕可能显示：

```text
150
```

但这个函数并没有正确地：

```cpp
return 150;
```

如果在线判题系统比较的是函数返回值，那么打印结果通常没有帮助。

---

# 12. `static` 有不同含义

原文中特别出现了两个 `static`，这是值得区分的。

## 情况一：文件作用域的 `static` 函数

例如：

```cpp
static int rangeWidth(int low, int high) {
    return high - low;
}
```

如果这个函数定义在类、struct 和其他函数之外，那么这里的 `static` 大致表示：

> 这个函数只在当前源文件中可见。

也就是说，它具有内部链接（internal linkage）。

例如：

```text
a.cpp
b.cpp
```

如果 `rangeWidth` 是：

```cpp
static int rangeWidth(...)
```

那么通常只有 `a.cpp` 自己能够直接使用这个函数名，不会作为一个普通的外部函数暴露给其他 `.cpp` 文件。

现代 C++ 中，对于这种需求也经常使用匿名命名空间：

```cpp
namespace {

int rangeWidth(int low, int high) {
    return high - low;
}

}
```

---

# 13. struct / class 中的 `static` 成员函数

假设：

```cpp
struct Solution {
    static int totalSeconds(int minutes, int seconds) {
        // ...
    }
};
```

这里的 `static` 是另一种含义。

它表示：

> `totalSeconds` 属于 `Solution` 这个类型本身，而不是某一个具体的 `Solution` 对象。

普通成员函数通常需要先创建对象：

```cpp
Solution s;
s.someFunction();
```

但静态成员函数可以：

```cpp
Solution::totalSeconds(2, 30);
```

不需要：

```cpp
Solution s;
```

这也是很多在线编程环境、测试框架或者 runner 使用静态成员函数的原因。

可以简单理解成：

```text
普通成员函数
对象.函数()

static 成员函数
类型::函数()
```

例如：

```cpp
struct Math {
    static int add(int a, int b) {
        return a + b;
    }
};
```

调用：

```cpp
int result = Math::add(3, 4);
```

得到：

```cpp
7
```

---

# 14. 同一个 `static`，为什么含义不同？

因为 `static` 是一个上下文相关的关键字。

例如：

```cpp
static int helper();
```

如果它位于文件作用域，主要影响：

```text
函数的链接性 / 可见范围
```

而：

```cpp
struct Solution {
    static int totalSeconds(...);
};
```

如果它位于类或 struct 中，则表示：

```text
这是静态成员函数
不依赖某一个具体对象
```

所以不能把所有 `static` 都理解成完全相同的意思。

---

# 15. 本题中的函数签名

题目要求：

```cpp
static int totalSeconds(int minutes, int seconds)
```

可以拆解为：

```text
static
│
├── static 成员函数

int
│
├── 返回类型是 int

totalSeconds
│
├── 函数名

(int minutes, int seconds)
│
├── 第一个参数：minutes
└── 第二个参数：seconds
```

换句话说，这个函数接受：

```text
分钟数
秒数
```

然后返回：

```text
总秒数
```

---

# 16. 分钟转换成秒

基础换算关系：

```text
1 分钟 = 60 秒
```

所以：

```cpp
minutes
```

分钟对应：

```cpp
minutes * 60
```

秒。

然后再加上：

```cpp
seconds
```

得到：

```cpp
minutes * 60 + seconds
```

---

# 17. 示例

## 示例 1

```cpp
totalSeconds(2, 30)
```

计算：

```text
2 × 60 + 30
= 120 + 30
= 150
```

所以返回：

```cpp
150
```

---

## 示例 2

```cpp
totalSeconds(0, 45)
```

计算：

```text
0 × 60 + 45
= 45
```

返回：

```cpp
45
```

---

## 示例 3

```cpp
totalSeconds(0, 0)
```

计算：

```text
0 × 60 + 0
= 0
```

返回：

```cpp
0
```

---

## 示例 4：负数

题目明确说明不需要检查输入是否合法。

例如：

```cpp
totalSeconds(-2, -30)
```

仍然直接执行：

```text
-2 × 60 + (-30)
= -120 - 30
= -150
```

所以：

```cpp
return -150;
```

这里不需要写：

```cpp
if (minutes < 0)
```

之类的输入校验。

原则是：

> 只实现题目要求的行为，不额外改变输入规则。

---

# 18. 这类函数题的通用思考方法

遇到类似题目时，可以按照下面的顺序分析：

```text
1. 看函数返回类型
2. 看函数名
3. 看参数数量、类型和顺序
4. 明确输入分别代表什么
5. 找出输入与输出之间的计算关系
6. 使用 return 返回结果
7. 不修改题目没有要求修改的 main / runner
```

例如本题：

```text
输入：
minutes
seconds

关系：
1 minute = 60 seconds

输出：
minutes * 60 + seconds
```

因此核心代码实际上只有：

```cpp
return minutes * 60 + seconds;
```

---

# 19. 最重要的几个知识点

```text
函数定义：

返回类型 函数名(参数列表) {
    函数体
}
```

例如：

```cpp
int add(int a, int b) {
    return a + b;
}
```

调用：

```cpp
add(3, 5);
```

参数按照位置匹配：

```text
a ← 3
b ← 5
```

`return`：

```text
返回结果
+
结束当前函数
```

而：

```cpp
std::cout
```

只是打印，不等于返回。

最后，要特别区分两种 `static`：

```text
文件作用域 static 函数
→ 主要限制函数在当前源文件内使用

class / struct 中的 static 成员函数
→ 不需要创建对象即可调用
```
