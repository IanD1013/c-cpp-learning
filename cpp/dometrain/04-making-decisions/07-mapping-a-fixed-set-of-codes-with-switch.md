# C++ `switch` 语句：处理固定的精确值匹配

## 1. `switch` 适合解决什么问题

当一个变量需要和一组**固定的、精确的值**进行比较时，可以使用 `switch`。

例如：

```cpp
static int sideCount(char shape) {
    int sides = 0;

    switch (shape) {
        case 't':
        case 'T':
            sides = 3;
            break;

        case 's':
            sides = 4;
            break;

        default:
            sides = -1;
            break;
    }

    return sides;
}
```

这里程序根据 `shape` 的值决定 `sides`：

```text
't' 或 'T' → 3
's'        → 4
其他值     → -1
```

这种“一个值对应一个结果”的场景，非常适合 `switch`。

---

# 2. `switch` 的基本结构

最基本的形式：

```cpp
switch (expression) {
    case value1:
        // statements
        break;

    case value2:
        // statements
        break;

    default:
        // statements
        break;
}
```

可以理解成：

```text
计算 expression

如果等于 value1
    执行第一组代码

否则如果等于 value2
    执行第二组代码

否则
    执行 default
```

例如：

```cpp
int day = 2;

switch (day) {
    case 1:
        std::cout << "Monday";
        break;

    case 2:
        std::cout << "Tuesday";
        break;

    default:
        std::cout << "Unknown";
        break;
}
```

由于：

```cpp
day == 2
```

所以执行：

```cpp
std::cout << "Tuesday";
```

---

# 3. `switch` 后面的值只计算一次

例如：

```cpp
switch (shape) {
```

其中：

```cpp
shape
```

叫做：

> controlling expression，控制表达式

程序进入 `switch` 时，会先计算一次这个表达式的值，然后根据结果寻找匹配的 `case`。

例如：

```cpp
char symbol = 'V';

switch (symbol) {
    case 'I':
        ...
    case 'V':
        ...
    case 'X':
        ...
}
```

程序会使用：

```cpp
symbol
```

当前的值 `'V'`，然后找到：

```cpp
case 'V':
```

并从那里开始执行。

---

# 4. `case` 表示“精确匹配”

例如：

```cpp
case 'I':
```

只表示：

```text
symbol == 'I'
```

它不会表示一个范围。

也就是说，下面是可以的：

```cpp
case 1:
case 2:
case 3:
```

但不能直接写成类似：

```cpp
case 1 to 10:
```

来表示一个范围。

`switch` 最擅长的是：

> 一个变量是不是正好等于某几个确定值。

---

# 5. `case` 后面必须是编译期已知的常量

例如：

```cpp
case 'I':
```

可以。

```cpp
case 10:
```

也可以。

因为 `'I'` 和 `10` 都是编译器提前知道的固定值。

但一般不能这样写：

```cpp
int target = 10;

switch (value) {
    case target:
        ...
}
```

因为普通变量 `target` 不是一个用于 `case` 的固定编译期常量。

初学阶段可以记住：

```text
case 后面通常放：
- 整数字面量
- 字符字面量
- 编译期常量
```

---

# 6. `switch` 不是用来比较字符串的

典型的 `switch` 匹配对象是：

* 整数类型
* `char`
* `enum`

例如：

```cpp
switch (symbol)
```

其中：

```cpp
symbol
```

是一个 `char`，非常适合。

但字符串通常不能像这样直接使用：

```cpp
std::string command = "start";

switch (command) {
    case "start":
        ...
}
```

这种写法不成立。

对于字符串，一般使用：

```cpp
if (command == "start") {
    ...
} else if (command == "stop") {
    ...
}
```

所以可以粗略记成：

```text
固定整数 / 字符代码 → switch 很合适

字符串比较 → 通常用 if / else if
```

---

# 7. `default` 表示“其他所有情况”

例如：

```cpp
default:
    sides = -1;
    break;
```

含义是：

> 如果前面的所有 `case` 都没有匹配，就执行这里。

例如：

```cpp
switch (symbol) {
    case 'I':
        value = 1;
        break;

    case 'V':
        value = 5;
        break;

    default:
        value = -1;
        break;
}
```

如果：

```cpp
symbol == 'Z'
```

因为没有：

```cpp
case 'Z':
```

所以会进入：

```cpp
default:
```

并得到：

```cpp
value = -1;
```

---

# 8. `default` 很适合处理无效输入

假设一个函数只认识：

```text
I
V
X
```

那么其他字符都应该被视为无效输入。

例如：

```text
'Z'
'0'
'?'
'A'
```

都可以统一交给：

```cpp
default:
```

处理。

例如：

```cpp
default:
    value = -1;
    break;
```

这里：

```cpp
-1
```

就是一个 sentinel value，也就是：

> 哨兵值 / 特殊失败值

它告诉调用者：

```text
这个字符不是函数认识的合法输入。
```

---

# 9. `break` 的作用非常重要

来看：

```cpp
case 'V':
    value = 5;
    break;
```

其中：

```cpp
break;
```

表示：

> 立刻结束当前 `switch`，跳到整个 `switch` 后面的代码。

例如：

```cpp
switch (symbol) {
    case 'V':
        value = 5;
        break;
}

return value;
```

如果匹配 `'V'`：

```text
value = 5
↓
break
↓
离开 switch
↓
return value
```

最终返回：

```text
5
```

---

# 10. 忘记 `break` 会发生 fallthrough

这是 `switch` 最重要的陷阱之一。

例如：

```cpp
switch (symbol) {
    case 'I':
        value = 1;

    case 'V':
        value = 5;
        break;
}
```

假设：

```cpp
symbol == 'I'
```

程序先进入：

```cpp
case 'I':
```

执行：

```cpp
value = 1;
```

但因为没有：

```cpp
break;
```

程序不会离开 `switch`。

它会继续往下执行：

```cpp
case 'V':
    value = 5;
```

于是最后：

```cpp
value
```

变成：

```text
5
```

而不是预期的：

```text
1
```

这叫：

> fallthrough，贯穿 / 继续落入下一个 case

---

# 11. fallthrough 本身不是语法错误

重要的是：

```text
忘记 break 通常不会导致编译失败。
```

代码可能完全可以编译，但逻辑结果是错的。

例如：

```cpp
case 1:
    result = 10;

case 2:
    result = 20;
    break;
```

如果匹配：

```cpp
case 1:
```

程序仍然会继续执行：

```cpp
case 2:
```

最终 `result` 变成 `20`。

所以：

> `switch` 代码中要特别检查 `break;`。

---

# 12. 但 fallthrough 有时是故意的

虽然忘记 `break` 经常是 bug，但有一种非常常见的正确用法：

```cpp
case 't':
case 'T':
    sides = 3;
    break;
```

注意：

```cpp
case 't':
```

下面没有任何语句。

所以如果匹配：

```cpp
't'
```

程序自然继续进入：

```cpp
case 'T':
```

然后执行共同的代码：

```cpp
sides = 3;
break;
```

于是：

```text
't' → 3
'T' → 3
```

这叫：

> stacked case labels，堆叠的 `case` 标签

---

# 13. 堆叠 `case` 非常适合大小写字符

例如：

```cpp
case 'I':
case 'i':
    value = 1;
    break;
```

意思是：

```text
'I' 或 'i' 都执行同一个代码块
```

同理：

```cpp
case 'V':
case 'v':
    value = 5;
    break;
```

以及：

```cpp
case 'X':
case 'x':
    value = 10;
    break;
```

这比重复写两遍：

```cpp
case 'I':
    value = 1;
    break;

case 'i':
    value = 1;
    break;
```

更加简洁。

---

# 14. “故意不写 break”和“忘记写 break”的区别

## 正确：堆叠标签

```cpp
case 'I':
case 'i':
    value = 1;
    break;
```

这里：

```cpp
case 'I':
```

和：

```cpp
case 'i':
```

之间没有任何要执行的代码。

意思非常明确：

```text
两个 case 共用一个 body
```

---

## 容易出错：执行代码后没有 break

```cpp
case 'I':
    value = 1;

case 'V':
    value = 5;
    break;
```

这里：

```cpp
value = 1;
```

执行之后还继续掉进下一个 `case`。

通常这就是 bug。

所以初学阶段可以记：

```text
case 标签连续堆叠
→ 可以故意不写 break

已经执行了真正的代码
→ 通常应该立刻检查是否需要 break
```

---

# 15. `switch` 和 `if...else if` 的区别

这两者有一些重叠场景，但用途不完全一样。

## `switch` 更适合固定值

例如：

```cpp
switch (symbol) {
    case 'I':
        ...
    case 'V':
        ...
    case 'X':
        ...
}
```

这里判断的是：

```text
symbol 是不是精确等于某个字符
```

---

## `if` 更适合范围和复杂条件

例如：

```cpp
if (score >= 90) {
    ...
} else if (score >= 80) {
    ...
}
```

这里判断的是范围：

```text
score >= 90
score >= 80
```

这种条件无法自然地写成普通 `case`。

因此可以这样记：

```text
精确匹配固定值
→ switch

范围、大小比较、多个逻辑条件
→ if / else if
```

---

# 16. `switch` 中的 `case` 本质上不是一个新的作用域

例如：

```cpp
switch (value) {
    case 1:
        int x = 10;
        break;

    case 2:
        ...
}
```

在更复杂的代码里，直接在不同 `case` 中声明变量有时会遇到作用域和初始化问题。

比较安全的写法是为复杂 `case` 加花括号：

```cpp
switch (value) {
    case 1: {
        int x = 10;
        std::cout << x;
        break;
    }

    case 2: {
        int y = 20;
        std::cout << y;
        break;
    }
}
```

初学题通常不会碰到这个问题，但以后写较复杂的 `switch` 时值得注意。

---

# 17. 本题中的 Roman numeral 字符

题目要求函数识别三个罗马数字字符：

```text
I → 1
V → 5
X → 10
```

而且大小写都接受：

```text
'I' → 1
'i' → 1

'V' → 5
'v' → 5

'X' → 10
'x' → 10
```

其他所有字符：

```text
'Z'
'0'
'A'
'?'
...
```

都返回：

```text
-1
```

---

# 18. 为什么 `'0'` 不是数字 0

这是一个很重要的细节。

```cpp
'0'
```

是：

> 字符 `'0'`

而：

```cpp
0
```

是：

> 整数零

它们不是一回事。

例如：

```cpp
char c = '0';
int n = 0;
```

`c` 保存的是字符编码对应的 `'0'`，而 `n` 保存的是数字 0。

因此本题中：

```cpp
romanDigitValue('0')
```

不应该返回 `0`。

因为：

```text
'0'
```

不是罗马数字字符。

它应该进入：

```cpp
default:
```

最终返回：

```text
-1
```

---

# 19. 本题的整体思路

题目已经给出：

```cpp
int value = 0;
```

函数最后：

```cpp
return value;
```

所以 `switch` 中每个分支的任务就是：

```text
给 value 赋正确的值
然后 break
```

结构应当是：

```cpp
int value = 0;

switch (symbol) {
    case ...:
    case ...:
        value = ...;
        break;

    case ...:
    case ...:
        value = ...;
        break;

    case ...:
    case ...:
        value = ...;
        break;

    default:
        value = -1;
        break;
}

return value;
```

分别填入：

```text
I/i → 1
V/v → 5
X/x → 10
其他 → -1
```

---

# 20. 核心记忆模板

最值得记住的是：

```cpp
switch (value) {
    case constant1:
        result = ...;
        break;

    case constant2:
        result = ...;
        break;

    default:
        result = ...;
        break;
}
```

如果多个值共享同一个结果：

```cpp
case value1:
case value2:
    result = ...;
    break;
```

例如：

```cpp
case 'A':
case 'a':
    result = 1;
    break;
```

---

# 21. 一句话总结

`switch` 最适合表达：

> “拿一个整数、字符或枚举值，与一组固定常量做精确匹配，然后根据匹配结果执行对应代码。”

其中最重要的三个部分是：

```cpp
case
```

表示一个精确匹配值；

```cpp
break;
```

在完成当前分支后离开 `switch`；

```cpp
default:
```

处理所有没有被任何 `case` 匹配到的值。

特别要记住：

```text
忘记 break → 可能发生意外 fallthrough

连续堆叠 case → 可以故意让多个值共享同一段代码
```
