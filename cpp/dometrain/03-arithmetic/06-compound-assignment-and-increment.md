# C++ 复合赋值运算符与自增、自减运算符

## 1. 什么是复合赋值

在程序中，我们经常需要：

1. 读取变量当前的值
2. 对它做某种运算
3. 再把结果存回同一个变量

例如：

```cpp
int value = 10;
value = value + 5;
```

执行后：

```cpp
value == 15
```

因为这种写法非常常见，C++ 提供了更简洁的写法：

```cpp
value += 5;
```

它等价于：

```cpp
value = value + 5;
```

这种写法叫做：

> **复合赋值运算符（compound assignment operator）**

---

# 2. 常见复合赋值运算符

基本格式是：

```cpp
variable operator= value;
```

常见的有：

| 运算符  | 含义    | 等价写法        |
| ---- | ----- | ----------- |
| `+=` | 加后赋值  | `x = x + y` |
| `-=` | 减后赋值  | `x = x - y` |
| `*=` | 乘后赋值  | `x = x * y` |
| `/=` | 除后赋值  | `x = x / y` |
| `%=` | 取余后赋值 | `x = x % y` |

例如：

```cpp
int value = 10;

value += 5;   // 15
value *= 3;   // 45
value /= 4;   // 11
value -= 1;   // 10
```

一步一步看：

```text
初始值：10

10 + 5 = 15
15 * 3 = 45
45 / 4 = 11
11 - 1 = 10
```

所以最后：

```cpp
value == 10
```

---

# 3. 左边变量既会被读取，也会被修改

以：

```cpp
value += amount;
```

为例，它其实是：

```cpp
value = value + amount;
```

因此左边的 `value` 同时扮演两个角色：

* 读取它当前的值
* 把新的结果重新存进去

例如：

```cpp
int score = 20;

score += 5;
```

过程可以理解成：

```text
读取 score 当前值：20
计算 20 + 5：25
把 25 存回 score
```

最终：

```cpp
score == 25
```

---

# 4. 运算方向非常重要

复合赋值永远把左边变量的当前值当作左操作数。

例如：

```cpp
value /= divisor;
```

等价于：

```cpp
value = value / divisor;
```

而不是：

```cpp
value = divisor / value;
```

例如：

```cpp
int value = 20;
int divisor = 5;

value /= divisor;
```

得到：

```cpp
value == 4
```

因为计算的是：

```cpp
20 / 5
```

而不是：

```cpp
5 / 20
```

这一点对于：

```cpp
-=
/=
%=
```

尤其重要，因为这些运算不满足交换律。

---

# 5. 右边的表达式会先计算

例如：

```cpp
value *= factor + 1;
```

并不是：

```cpp
(value * factor) + 1
```

而是：

```cpp
value *= (factor + 1);
```

也就是：

```cpp
value = value * (factor + 1);
```

例如：

```cpp
int value = 10;
int factor = 2;

value *= factor + 1;
```

先算：

```cpp
factor + 1
```

得到：

```cpp
3
```

然后：

```cpp
10 * 3
```

所以：

```cpp
value == 30
```

---

# 6. `/=` 仍然遵守整数除法规则

这是一个非常重要的点。

如果变量和除数都是 `int`：

```cpp
int value = 45;

value /= 4;
```

等价于：

```cpp
value = value / 4;
```

而：

```cpp
45 / 4
```

是整数除法，所以结果是：

```cpp
11
```

而不是：

```text
11.25
```

因此：

```cpp
value == 11
```

---

# 7. 负数整数除法：向 0 截断

C++ 的整数除法不是“向下取整”，而是：

> **向 0 截断（truncate toward zero）**

例如：

```cpp
-18 / 4
```

数学结果是：

```text
-4.5
```

C++ 会舍掉小数部分，得到：

```cpp
-4
```

因此：

```cpp
int value = -18;
value /= 4;
```

执行后：

```cpp
value == -4
```

注意，它不是：

```cpp
-5
```

因为 `-5` 是向负无穷方向取整，而 C++ 整数除法是向 0 截断。

可以记住：

```text
 18 / 4  ->  4
-18 / 4  -> -4
```

---

# 8. `=+` 和 `+=` 完全不是一回事

这是非常常见的错误。

正确：

```cpp
value += addend;
```

意思是：

```cpp
value = value + addend;
```

但是如果写成：

```cpp
value =+ addend;
```

C++ 会把它解析成：

```cpp
value = +addend;
```

其中：

```cpp
+addend
```

只是一个带有一元正号 `+` 的值。

因此它本质上是：

```cpp
value = addend;
```

原来的 `value` 会直接被覆盖。

例如：

```cpp
int value = 10;
int addend = 5;

value =+ addend;
```

最后：

```cpp
value == 5
```

而不是：

```cpp
15
```

所以一定注意：

```cpp
+=
```

和：

```cpp
=+
```

不是同一个东西。

---

# 9. 自增运算符 `++`

如果只需要给一个变量加 `1`：

```cpp
value += 1;
```

C++ 还有更简洁的写法：

```cpp
value++;
```

例如：

```cpp
int steps = 7;

steps++;
```

执行后：

```cpp
steps == 8
```

因此，在只关心变量最终值的时候：

```cpp
steps++;
```

可以看作：

```cpp
steps += 1;
```

进一步等价于：

```cpp
steps = steps + 1;
```

---

# 10. 自减运算符 `--`

类似地：

```cpp
value--;
```

表示：

```cpp
value -= 1;
```

也就是：

```cpp
value = value - 1;
```

例如：

```cpp
int steps = 7;

steps--;
```

最后：

```cpp
steps == 6
```

---

# 11. 前置 `++value` 与后置 `value++`

C++ 中有两种自增写法：

```cpp
++value;
value++;
```

如果它们只是单独作为一条语句：

```cpp
++value;
```

和：

```cpp
value++;
```

最终效果相同：都给 `value` 加 1。

例如：

```cpp
int x = 5;

x++;
```

得到：

```cpp
x == 6
```

而：

```cpp
int x = 5;

++x;
```

同样得到：

```cpp
x == 6
```

所以在：

```cpp
x++;
```

这种独立语句中，可以暂时把它简单理解为：

```cpp
x += 1;
```

---

# 12. 前置和后置真正的区别

区别出现在：

> 自增表达式的结果本身还会被其他代码使用时。

例如后置自增：

```cpp
int x = 5;
int y = x++;
```

执行后：

```cpp
x == 6
y == 5
```

可以粗略理解为：

```text
先把旧的 x 值交给 y
再把 x 加 1
```

而前置自增：

```cpp
int x = 5;
int y = ++x;
```

结果是：

```cpp
x == 6
y == 6
```

可以粗略理解为：

```text
先把 x 加 1
再把新的 x 值交给 y
```

所以：

```cpp
x++;
```

和：

```cpp
++x;
```

作为独立语句时通常没有实际区别。

但是：

```cpp
y = x++;
```

和：

```cpp
y = ++x;
```

有明显区别。

---

# 13. 一组完整示例

假设：

```cpp
int value = 10;
```

依次执行：

```cpp
value += 5;
value *= 3;
value /= 4;
value -= 1;
value++;
```

计算过程：

```text
value = 10

value += 5
10 + 5 = 15

value *= 3
15 * 3 = 45

value /= 4
45 / 4 = 11
整数除法舍掉小数部分

value -= 1
11 - 1 = 10

value++
10 + 1 = 11
```

最终：

```cpp
value == 11
```

---

# 14. 最重要的等价关系

复习时建议直接记住这些：

```cpp
x += y;
```

等价于：

```cpp
x = x + y;
```

---

```cpp
x -= y;
```

等价于：

```cpp
x = x - y;
```

---

```cpp
x *= y;
```

等价于：

```cpp
x = x * y;
```

---

```cpp
x /= y;
```

等价于：

```cpp
x = x / y;
```

---

```cpp
x %= y;
```

等价于：

```cpp
x = x % y;
```

---

```cpp
x++;
```

在单独作为一条语句时，可以理解成：

```cpp
x += 1;
```

---

```cpp
x--;
```

在单独作为一条语句时，可以理解成：

```cpp
x -= 1;
```

---

# 15. 常见错误

## 错误 1：把 `+=` 写成 `=+`

错误：

```cpp
value =+ addend;
```

它实际上相当于：

```cpp
value = addend;
```

正确应该是：

```cpp
value += addend;
```

---

## 错误 2：忘记整数除法会截断

```cpp
int value = 18;

value /= 4;
```

结果：

```cpp
4
```

而不是：

```cpp
4.5
```

---

## 错误 3：把负数整数除法理解成向下取整

```cpp
-18 / 4
```

结果是：

```cpp
-4
```

而不是：

```cpp
-5
```

C++ 整数除法向 `0` 截断。

---

## 错误 4：误解右边表达式的计算顺序

```cpp
value *= factor + 1;
```

应该理解成：

```cpp
value = value * (factor + 1);
```

而不是：

```cpp
value = value * factor + 1;
```

---

# 16. 核心总结

复合赋值用于：

> **根据变量当前值计算出新值，然后重新存回该变量。**

例如：

```cpp
score += 10;
```

表示：

```cpp
score = score + 10;
```

最常见的运算符是：

```cpp
+=
-=
*=
/=
%=
```

而：

```cpp
++
--
```

分别表示加 1 和减 1。

尤其需要注意：

```cpp
value /= divisor;
```

如果两者都是 `int`，依然执行整数除法，因此会向 `0` 截断小数部分。

最后务必区分：

```cpp
+=
```

和：

```cpp
=+
```

前者是“加后赋值”，后者只是普通赋值加上一元正号。
