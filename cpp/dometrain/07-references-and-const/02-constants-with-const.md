# C++ `const` 常量与浮点除法

## 1. `const` 是什么？

在变量声明前加上 `const`，表示：

> 变量必须在创建时初始化，并且初始化之后不能再被赋值。

```cpp
const int daysPerWeek = 7;
const double litersPerGallon = 3.785;
```

这些变量通常称为**常量（constant）**或**命名常量（named constant）**。

之后再修改它：

```cpp
daysPerWeek = 8;
```

会产生编译错误，因为 `daysPerWeek` 已经是 `const`。

---

## 2. `const` 变量为什么必须初始化？

下面这种写法不允许：

```cpp
const int limit;
```

原因是：

* `limit` 创建时没有值
* 之后又因为 `const` 不能赋值
* 那么这个变量永远无法得到有效值

因此通常必须：

```cpp
const int limit = 100;
```

可以把初始化理解成：

> `const` 变量只有创建时这一次机会得到值。

---

## 3. `const` 不等于“编译期常量”

一个常见误区是：

> `const` 的值是不是必须在编译时就知道？

不是。

例如：

```cpp
double example(double seconds) {
    const double half = seconds / 2.0;
    return half;
}
```

这里 `seconds` 是函数运行时传入的，所以 `half` 的值也要到**程序运行时**才能算出来。

但这完全合法。

`const` 真正表达的是：

> 初始化以后不能再修改。

而不是：

> 必须在编译时知道它的值。

---

# 4. 用命名常量表达单位转换

假设要把秒转换成小时。

已知：

```text
1 分钟 = 60 秒
1 小时 = 60 分钟
```

可以定义：

```cpp
const int secondsPerMinute = 60;
const int minutesPerHour = 60;
```

然后计算：

```cpp
const double secondsPerHour =
    secondsPerMinute * minutesPerHour;
```

这样：

```text
secondsPerHour = 60 × 60 = 3600
```

相比直接写：

```cpp
seconds / 3600
```

这种方式更容易理解：

```cpp
seconds / secondsPerHour
```

因为代码本身已经表达了：

> 一小时包含多少秒。

---

# 5. Magic Number（魔法数字）

代码中直接出现：

```cpp
3600
```

虽然程序员可能知道它代表“一小时的秒数”，但读代码的人未必知道。

这种没有解释来源的数字通常称为：

> **Magic Number（魔法数字）**

例如：

```cpp
return seconds / 3600.0;
```

通常不如：

```cpp
const int secondsPerMinute = 60;
const int minutesPerHour = 60;

const double secondsPerHour =
    secondsPerMinute * minutesPerHour;

return seconds / secondsPerHour;
```

后者更清楚，也更容易维护。

---

# 6. 为什么 `secondsPerHour` 要是 `double`？

这是这道题真正重要的地方。

假设：

```cpp
int seconds = 5400;
```

如果写：

```cpp
int secondsPerHour = 3600;

seconds / secondsPerHour;
```

实际进行的是：

```cpp
5400 / 3600
```

由于两边都是 `int`，C++ 会执行**整数除法**：

```text
5400 / 3600 = 1
```

小数部分 `.5` 会被直接丢掉。

---

## 使用 `double`

如果 divisor（除数）是 `double`：

```cpp
double secondsPerHour = 3600;
```

那么：

```cpp
seconds / secondsPerHour
```

相当于：

```text
5400 / 3600.0
```

此时 C++ 会进行浮点除法：

```text
1.5
```

所以：

```cpp
const double secondsPerHour =
    secondsPerMinute * minutesPerHour;
```

非常重要。

---

# 7. `int * int` 为什么能赋值给 `double`？

这里：

```cpp
const int secondsPerMinute = 60;
const int minutesPerHour = 60;

const double secondsPerHour =
    secondsPerMinute * minutesPerHour;
```

右边：

```cpp
secondsPerMinute * minutesPerHour
```

实际上是：

```cpp
int * int
```

所以乘法结果先是：

```cpp
3600   // int
```

然后赋值给：

```cpp
double secondsPerHour
```

C++ 会自动转换成：

```cpp
3600.0
```

因此后面的：

```cpp
seconds / secondsPerHour
```

就变成：

```text
int / double
```

C++ 会把 `seconds` 转换成 `double` 后再进行浮点除法。

---

# 8. `static_cast<double>` 与本题方法的区别

之前可能学过：

```cpp
static_cast<double>(seconds) / secondsPerHour
```

例如：

```cpp
int a = 5;
int b = 2;

double result =
    static_cast<double>(a) / b;
```

得到：

```text
2.5
```

这是主动把 `a` 转换成 `double`。

但本题禁止使用：

```cpp
static_cast
```

因为本题想训练另一种方法：

```cpp
double divisor
```

例如：

```cpp
const double secondsPerHour = 3600;

return seconds / secondsPerHour;
```

由于除数本身已经是 `double`，C++ 会自动进行浮点运算。

---

# 9. 零和负数也可以正常转换

这里不需要特殊处理。

### 0 秒

```cpp
0 / 3600.0
```

得到：

```text
0.0
```

### -3600 秒

```cpp
-3600 / 3600.0
```

得到：

```text
-1.0
```

因此不需要写：

```cpp
if (seconds < 0) ...
```

也不需要拒绝负数。

---

# 核心总结

### `const`

```cpp
const int x = 10;
```

表示：

```text
创建时赋值
      ↓
之后不能修改
```

它不意味着值必须在编译时确定。

---

### 避免整数除法

```cpp
int / int
```

会进行整数除法：

```cpp
5400 / 3600    // 1
```

而：

```cpp
int / double
```

会进行浮点除法：

```cpp
5400 / 3600.0  // 1.5
```

因此可以通过：

```cpp
const double secondsPerHour = ...;
```

让整个除法自然变成浮点除法。

---

## 最值得记住的一句话

> `const` 决定的是“这个变量之后能不能修改”，而变量的类型决定了表达式如何进行计算。
