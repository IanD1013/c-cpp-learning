# C++ `void` 函数：没有返回值的函数

## 1. 什么是 `void` 函数？

前面我们见过这种函数：

```cpp
static int doubled(int value) {
    return value * 2;
}
```

这里的 `int` 是函数的**返回类型（return type）**。

它表示：

> 调用这个函数之后，会得到一个 `int` 类型的值。

例如：

```cpp
int result = doubled(5);
```

执行后：

```cpp
result == 10
```

但是，并不是所有函数都需要产生一个值。

例如，一个函数的目的可能只是：

* 打印内容
* 修改某个对象
* 写入文件
* 记录日志
* 执行某个动作

这时候可以使用：

```cpp
void
```

例如：

```cpp
static void printTimes(int count) {
    std::cout << count << " times\n";
}
```

这里：

```cpp
void
```

表示：

> 这个函数执行完以后，不会返回任何值。

---

# 2. `void` 所在的位置

函数定义的一般结构是：

```cpp
返回类型 函数名(参数列表) {
    函数体
}
```

例如：

```cpp
static int doubled(int value) {
    return value * 2;
}
```

而 `void` 函数只是把返回类型换成了：

```cpp
void
```

因此：

```cpp
static void printTimes(int count) {
    std::cout << count << " times\n";
}
```

可以拆成：

```text
static
void
printTimes
(int count)
{
    ...
}
```

其中：

* `static`：这里用于限制函数在当前源文件中的可见范围
* `void`：函数不返回值
* `printTimes`：函数名
* `int count`：参数
* `{ ... }`：函数体

所以：

```cpp
static void
```

和：

```cpp
static int
```

的整体函数结构其实完全一样。

区别只是：

```cpp
int
```

表示“函数会返回一个整数”，而：

```cpp
void
```

表示“函数不返回任何值”。

---

# 3. `void` 函数中的 `return;`

考虑：

```cpp
static void printTimes(int count) {
    if (count <= 0) {
        return;
    }

    std::cout << count << " times\n";
}
```

注意这里写的是：

```cpp
return;
```

而不是：

```cpp
return 某个值;
```

这是因为 `void` 函数没有返回值。

因此：

```cpp
return;
```

的含义只是：

> 立刻结束这个函数。

例如：

```cpp
printTimes(-3);
```

进入函数后：

```cpp
count <= 0
```

成立，于是：

```cpp
return;
```

直接结束函数。

因此下面这行不会执行：

```cpp
std::cout << count << " times\n";
```

---

# 4. `return;` 与 `return value;` 的区别

对于有返回值的函数：

```cpp
static int doubled(int value) {
    return value * 2;
}
```

必须返回一个符合返回类型的值：

```cpp
return value * 2;
```

这里返回的是一个 `int`。

但是对于：

```cpp
static void printTimes(int count)
```

不能写：

```cpp
return 10;
```

因为函数声明已经说明：

```cpp
void
```

也就是：

> 我不会返回任何值。

因此只能：

```cpp
return;
```

或者根本不写 `return`。

---

# 5. `void` 函数不一定需要写 `return`

例如：

```cpp
static void printTimes(int count) {
    std::cout << count << " times\n";
}
```

这是完全合法的。

因为程序运行到：

```cpp
}
```

也就是函数的最后一个右花括号时，函数自然结束。

对于 `void` 函数来说：

```cpp
static void hello() {
    std::cout << "Hello\n";
}
```

相当于：

```cpp
static void hello() {
    std::cout << "Hello\n";
    return;
}
```

通常没有必要专门在最后写：

```cpp
return;
```

---

# 6. 那什么时候需要在 `void` 函数中写 `return;`？

最常见的用途是**提前退出（early return）**。

例如：

```cpp
static void printTimes(int count) {
    if (count <= 0) {
        return;
    }

    std::cout << count << " times\n";
}
```

这里的逻辑可以理解成：

```text
如果输入不符合条件：
    直接结束函数

否则：
    继续执行后面的代码
```

这种写法非常常见。

例如：

```cpp
static void printPositive(int value) {
    if (value <= 0) {
        return;
    }

    std::cout << value << "\n";
}
```

如果：

```cpp
value = -5
```

就直接退出。

如果：

```cpp
value = 8
```

才会输出：

```text
8
```

---

# 7. `void` 函数通常是为了“副作用”

对于：

```cpp
static int doubled(int value) {
    return value * 2;
}
```

函数最重要的结果是：

```text
返回一个值
```

而对于：

```cpp
static void printTimes(int count) {
    std::cout << count << " times\n";
}
```

函数没有返回值。

它存在的目的，是：

```cpp
std::cout
```

产生的输出。

这种函数执行后对外界产生的效果，通常可以称为：

> **副作用（side effect）**

常见副作用包括：

```text
输出到终端
修改对象
修改引用参数
写文件
写数据库
发送网络请求
记录日志
```

因此可以把两类函数粗略理解成：

```text
有返回值函数：
输入 → 计算 → 返回结果

void 函数：
输入 → 执行动作
```

---

# 8. `void` 函数的调用方式

假设：

```cpp
static void printTimes(int count) {
    std::cout << count << " times\n";
}
```

调用时通常单独写一行：

```cpp
printTimes(5);
```

输出：

```text
5 times
```

这里：

```cpp
printTimes(5);
```

本身是一条完整的语句。

---

# 9. 为什么不能把 `void` 函数赋值给变量？

假设：

```cpp
static void printTimes(int count) {
    std::cout << count << " times\n";
}
```

不能写：

```cpp
int result = printTimes(5);
```

原因是：

```cpp
printTimes(5)
```

没有产生任何值。

而：

```cpp
int result = ...
```

要求右边必须产生一个可以赋给 `int` 的值。

可以把它想象成：

```text
int result = ???;
```

但是 `void` 函数没有任何东西可以填进 `???`。

因此这是错误的。

---

# 10. `void` 函数也不能放进需要“值”的表达式中

例如：

```cpp
std::cout << printTimes(5);
```

也是错误的。

因为：

```cpp
operator<<
```

需要右边提供一个可以输出的值。

但：

```cpp
printTimes(5)
```

没有返回任何值。

正确做法是：

```cpp
printTimes(5);
```

因为 `printTimes` 自己已经负责输出。

---

# 11. 有返回值函数和 `void` 函数的核心区别

可以这样总结：

| 函数            | 调用后得到值吗？ | 常见用途  |
| ------------- | -------: | ----- |
| `int`         |        是 | 计算整数  |
| `double`      |        是 | 计算小数  |
| `std::string` |        是 | 生成字符串 |
| `bool`        |        是 | 判断条件  |
| `void`        |        否 | 执行动作  |

例如：

```cpp
static int add(int a, int b) {
    return a + b;
}
```

可以：

```cpp
int result = add(3, 4);
```

因为：

```cpp
add(3, 4)
```

最终会被它的返回值替代：

```cpp
7
```

即：

```cpp
int result = 7;
```

而：

```cpp
static void printSum(int a, int b) {
    std::cout << a + b << "\n";
}
```

只能这样使用：

```cpp
printSum(3, 4);
```

它会输出：

```text
7
```

但不会把 `7` 返回给调用者。

---

# 12. 一个非常重要的区别：打印 ≠ 返回

初学 C++ 时很容易混淆：

```cpp
std::cout << value;
```

和：

```cpp
return value;
```

这两个操作完全不同。

## `std::cout`

```cpp
std::cout << value;
```

表示：

> 把内容显示在终端中。

例如：

```cpp
static void showNumber(int value) {
    std::cout << value;
}
```

调用：

```cpp
showNumber(10);
```

屏幕看到：

```text
10
```

但是调用者没有得到 `10`。

---

## `return`

```cpp
return value;
```

表示：

> 把值交回调用函数的地方。

例如：

```cpp
static int getNumber() {
    return 10;
}
```

调用：

```cpp
int x = getNumber();
```

得到：

```cpp
x == 10
```

这里甚至不需要产生任何屏幕输出。

因此：

```text
cout = 显示给人看
return = 返回给程序使用
```

这是两个不同概念。

---

# 13. 本题中的辅助函数 `printLabelledValue`

题目设计了一个辅助函数：

```cpp
printLabelledValue(...)
```

它接受两个参数：

```text
第一个：label
第二个：int value
```

它负责打印：

```text
标签: 数值
```

例如：

```cpp
printLabelledValue("Total", 12);
```

应该产生：

```text
Total: 12
```

输出结构必须是：

```text
label
+
:
+
一个空格
+
value
+
换行
```

因此核心输出逻辑是：

```cpp
std::cout << label << ": " << value << "\n";
```

---

# 14. 为什么输出格式必须非常精确？

自动测试通常不是“看起来差不多就算正确”。

它可能直接比较两个字符串：

```text
期望：
First: 7
Second: 4
```

和程序真正输出的内容。

因此下面这些都可能导致测试失败。

多一个空格：

```text
First:  7
```

少一个空格：

```text
First:7
```

大小写错误：

```text
first: 7
```

少冒号：

```text
First 7
```

两个结果跑到同一行：

```text
First: 7Second: 4
```

都和正确答案不同。

所以输出题要特别注意：

> **空格、标点、大小写、换行都是程序输出的一部分。**

---

# 15. `"\n"` 的作用

例如：

```cpp
std::cout << "First: " << first << "\n";
std::cout << "Second: " << second << "\n";
```

第一次输出后：

```cpp
"\n"
```

让光标移动到下一行。

所以最终是：

```text
First: 7
Second: 4
```

如果没有第一个：

```cpp
"\n"
```

就可能变成：

```text
First: 7Second: 4
```

因此第一个换行非常重要。

---

# 16. 为什么应该让 `printLabelledPair` 调用辅助函数？

题目要求：

```cpp
printLabelledPair(int first, int second)
```

调用：

```cpp
printLabelledValue
```

两次。

逻辑应该是：

```text
第一次：
label = "First"
value = first

第二次：
label = "Second"
value = second
```

也就是：

```cpp
printLabelledValue("First", first);
printLabelledValue("Second", second);
```

这体现了一个很重要的编程思想：

> **把重复逻辑抽取到函数中，然后复用这个函数。**

如果直接写：

```cpp
std::cout << "First: " << first << "\n";
std::cout << "Second: " << second << "\n";
```

虽然输出可能一样，但没有按照题目要求使用辅助函数。

---

# 17. 参数是按位置匹配的

假设辅助函数定义是：

```cpp
static void printLabelledValue(
    const std::string& label,
    int value
)
```

那么：

```cpp
printLabelledValue("First", first);
```

参数匹配关系是：

```text
"First" → label
first   → value
```

因为函数参数是按照**位置**匹配的。

因此不能写反：

```cpp
printLabelledValue(first, "First");
```

因为第一个参数应该是字符串，而不是整数。

---

# 18. 调用顺序决定输出顺序

题目要求：

```text
First
```

在：

```text
Second
```

前面。

因此必须先调用：

```cpp
printLabelledValue("First", first);
```

再调用：

```cpp
printLabelledValue("Second", second);
```

如果反过来：

```cpp
printLabelledValue("Second", second);
printLabelledValue("First", first);
```

输出会变成：

```text
Second: 4
First: 7
```

测试就会失败。

---

# 19. 辅助函数的位置为什么重要？

题目特别说明：

```text
printLabelledValue 要留在 struct Solution 外面。
```

例如：

```cpp
static void printLabelledValue(...) {
    ...
}

struct Solution {
    static void printLabelledPair(int first, int second) {
        ...
    }
};
```

这是正确结构。

不要擅自改成：

```cpp
struct Solution {
    static void printLabelledValue(...) {
        ...
    }
};
```

原因是测试程序可能直接调用：

```cpp
printLabelledValue(...);
```

而不是：

```cpp
Solution::printLabelledValue(...);
```

如果你把函数移动到 `Solution` 里面，它的名字和调用方式就发生了变化。

测试代码仍然写：

```cpp
printLabelledValue(...);
```

就可能产生编译错误。

这是自动评测题中非常重要的一条原则：

> **不要随意修改 starter code 已经给定的函数位置、函数名和函数签名。**

---

# 20. 为什么不能自己写 `main()`？

题目明确说明：

> runner 自己提供 `main()`。

也就是说测试系统内部可能类似：

```cpp
int main() {
    Solution::printLabelledPair(7, 4);
}
```

所以你只需要完成题目要求的函数。

不要再写：

```cpp
int main() {
    ...
}
```

否则整个程序可能同时存在两个：

```cpp
main()
```

导致编译或链接失败。

LeetCode、在线课程自动评测器等环境中都经常采用这种模式。

---

# 21. 本节最重要的知识点

## `void`

```cpp
void
```

表示函数：

```text
不返回任何值
```

---

## `return;`

在 `void` 函数中：

```cpp
return;
```

表示：

```text
立刻结束函数
```

没有返回值。

---

## 到达 `}` 也会结束 `void` 函数

所以：

```cpp
static void hello() {
    std::cout << "Hello\n";
}
```

完全合法。

---

## `void` 函数不能作为一个值使用

错误：

```cpp
int x = printTimes(5);
```

错误：

```cpp
std::cout << printTimes(5);
```

正确：

```cpp
printTimes(5);
```

---

## 打印和返回不是一回事

```cpp
std::cout
```

是：

```text
把内容输出到终端
```

而：

```cpp
return
```

是：

```text
把值返回给调用者
```

---

## 自动评测题要严格遵守接口

不要随意修改：

```text
函数名
参数
参数顺序
返回类型
函数所在位置
```

除非题目明确要求。

---

# 22. 一个完整的思维模型

看到：

```cpp
static void printLabelledPair(int first, int second)
```

可以在脑中读成：

> 定义一个叫 `printLabelledPair` 的函数。
> 它接受两个 `int` 参数 `first` 和 `second`。
> 它执行某些操作，但不会返回任何值。

调用：

```cpp
printLabelledPair(7, 4);
```

参数变成：

```text
first  = 7
second = 4
```

然后函数执行：

```cpp
printLabelledValue("First", first);
printLabelledValue("Second", second);
```

于是：

```text
"First", 7
    ↓
First: 7

"Second", 4
    ↓
Second: 4
```

最终输出：

```text
First: 7
Second: 4
```

函数结束后：

```text
没有任何值返回给调用者
```

因为它的返回类型是：

```cpp
void
```
