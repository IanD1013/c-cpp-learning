# C++ 指针基础：地址、取地址与解引用

## 1. 什么是地址？

程序中的每个变量都会存放在内存中的某个位置，这个位置可以用一个**内存地址（address）**表示。

例如：

```cpp
int count = 4;
```

可以把它想象成：

```text
某个内存地址
0x1234
   ↓
+------+
|  4   |   ← count
+------+
```

`count` 是变量名，`4` 是变量保存的值，而 `0x1234` 可以理解为它在内存中的地址。

---

## 2. 什么是指针？

**指针（pointer）本身也是一个变量，只不过它保存的不是普通数据，而是另一个变量的地址。**

```cpp
int count = 4;
int *p = &count;
```

这里：

```cpp
count
```

保存的是：

```text
4
```

而：

```cpp
p
```

保存的是：

```text
count 的地址
```

可以理解成：

```text
p
+--------+
| 0x1234 | ----+
+--------+     |
               v
            count
           +------+
           |  4   |
           +------+
```

因此我们说：

> `p` 指向 `count`。

---

# 3. `&`：取地址运算符

在变量前面写：

```cpp
&变量
```

表示：

> 获取这个变量的内存地址。

例如：

```cpp
int count = 4;

&count
```

得到的是 `count` 的地址。

因此：

```cpp
int *p = &count;
```

就是：

> 创建一个指向 `int` 的指针 `p`，让它保存 `count` 的地址。

---

# 4. `*` 有两种不同含义

C++ 里的 `*` 在指针中有两个非常容易混淆的用途。

## 用法一：声明指针

```cpp
int *p;
```

这里的 `*` 表示：

> `p` 是一个指向 `int` 的指针。

也就是说：

```cpp
int *p;
```

可以理解为：

```text
p 的类型 = int 指针
```

它应该保存某个 `int` 对象的地址，例如：

```cpp
int count = 4;

int *p = &count;
```

---

## 用法二：解引用 pointer dereference

当指针已经保存了一个地址之后：

```cpp
*p
```

表示：

> 找到 `p` 指向的那个对象。

例如：

```cpp
int count = 4;
int *p = &count;

cout << *p;
```

结果是：

```text
4
```

因为：

```cpp
p
```

保存的是 `count` 的地址，而：

```cpp
*p
```

表示：

> 地址里面那个 `int`。

也就是 `count` 本身。

---

# 5. 通过指针读取变量

例如：

```cpp
int count = 4;
int *p = &count;

int x = *p;
```

这里：

```cpp
*p
```

读取的是 `count` 的值。

因此：

```cpp
x == 4
```

---

# 6. 通过指针修改变量

更重要的是，解引用之后也可以作为赋值目标：

```cpp
int count = 4;
int *p = &count;

*p = 14;
```

这相当于：

```cpp
count = 14;
```

所以最后：

```cpp
count == 14
```

---

## 一个完整例子

```cpp
int count = 4;
int *p = &count;

*p = *p + 10;
```

执行过程：

```text
*p
↓
count
↓
4
```

所以：

```cpp
*p + 10
```

得到：

```text
14
```

然后：

```cpp
*p = 14;
```

实际上修改的是：

```cpp
count
```

最终：

```cpp
count == 14
```

---

# 7. `count` 和 `*p` 不是两个变量

这是理解指针最重要的地方之一。

```cpp
int count = 4;
int *p = &count;
```

这里并没有复制出第二个 `int`。

不是：

```text
count = 4
*p    = 4
```

而是：

```text
count
   ↑
   |
  *p
```

`count` 和 `*p` 都是在访问**同一个对象**。

因此：

```cpp
count = 10;
```

之后：

```cpp
*p == 10
```

反过来：

```cpp
*p = 20;
```

之后：

```cpp
count == 20
```

---

# 8. 修改指针和修改指针指向的对象是两回事

这是另一个非常重要的区别。

假设：

```cpp
int first = 10;
int second = 20;

int *p = &first;
```

此时：

```text
p → first
```

如果写：

```cpp
*p = 100;
```

修改的是：

```cpp
first
```

于是：

```cpp
first == 100
```

---

但如果写：

```cpp
p = &second;
```

修改的是：

```cpp
p 自己保存的地址
```

现在：

```text
p → second
```

但：

```cpp
first
second
```

本身都没有被这个操作修改。

所以可以记住：

```cpp
p = &second;
```

表示：

> 让指针改为指向 `second`

而：

```cpp
*p = 100;
```

表示：

> 修改 `p` 当前指向的对象

---

# 9. 一个很好用的理解方式

看到：

```cpp
p
```

可以读成：

> 地址

看到：

```cpp
*p
```

可以读成：

> 这个地址指向的东西

看到：

```cpp
&p
```

则是：

> 指针变量 `p` 自己的地址

这三者完全不同。

例如：

```cpp
int value = 10;
int *p = &value;
```

可以理解成：

```text
value
↓
10

&value
↓
value 的地址

p
↓
value 的地址

*p
↓
10
```

所以：

```cpp
p == &value
```

而：

```cpp
*p == value
```

---

# 10. 指针类型必须匹配

例如：

```cpp
int value = 10;

int *p = &value;
```

这里 `p` 是：

```cpp
int *
```

因此它应该保存：

```cpp
int
```

对象的地址。

例如下面通常是不允许的：

```cpp
double x = 3.14;

int *p = &x;   // 错误
```

因为：

```cpp
&x
```

的类型是：

```cpp
double *
```

而不是：

```cpp
int *
```

---

# 11. 指针经常用来“选择要操作哪个对象”

指针一个很常见的用途是：

> 根据条件，让一个变量指向不同对象，然后用同一段代码操作被选中的对象。

例如：

```cpp
int first = 10;
int second = 20;

int *target;

if (someCondition) {
    target = &first;
} else {
    target = &second;
}

*target += 5;
```

如果：

```cpp
target == &first
```

那么：

```cpp
*target += 5;
```

相当于：

```cpp
first += 5;
```

如果：

```cpp
target == &second
```

那么相当于：

```cpp
second += 5;
```

这是一种非常典型的指针使用方式：

```text
先决定“操作谁”
        ↓
让指针指向它
        ↓
统一通过 *pointer 操作
```

---

# 12. 指针与局部变量的生命周期

例如函数参数：

```cpp
void f(int first, int second) {
    int *p = &first;
}
```

这里：

```cpp
first
second
```

都是这个函数调用期间存在的局部变量。

因此在函数执行期间：

```cpp
&first
```

是有效地址。

只要函数还没有返回：

```cpp
p
```

就可以安全地指向 `first`。

但是函数结束之后，局部变量生命周期也结束，所以不能把这样的地址拿出去继续使用。

---

# 13. 本节最核心的四个表达式

建议直接记住：

```cpp
int value = 10;
int *p = &value;
```

那么：

```cpp
value
```

表示：

> `value` 的值

```cpp
&value
```

表示：

> `value` 的地址

```cpp
p
```

表示：

> `p` 中保存的地址，也就是 `value` 的地址

```cpp
*p
```

表示：

> `p` 指向的对象，也就是 `value`

因此：

```cpp
p == &value
```

并且：

```cpp
*p == value
```

---

# 14. 一个最值得记住的心智模型

```cpp
int x = 10;
int *p = &x;
```

可以画成：

```text
       p
+-------------+
| address of x|
+-------------+
       |
       |
       v
      x
+-------------+
|     10      |
+-------------+
```

因此：

```cpp
p
```

是箭头本身保存的地址。

```cpp
*p
```

则是：

> 顺着箭头走过去，找到那个对象。

所以：

```cpp
*p = 20;
```

可以理解成：

> 顺着 `p` 这根箭头找到 `x`，然后把 `x` 改成 20。
