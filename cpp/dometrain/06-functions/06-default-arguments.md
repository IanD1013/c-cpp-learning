# C++ 默认参数（Default Arguments）

## 1. 什么是默认参数

C++ 允许给函数参数指定一个**默认值**。

当调用函数时，如果调用者省略了这个参数，编译器就会自动使用默认值。

```cpp
static int shifted(int value, int offset = 1) {
    return value + offset;
}
```

调用：

```cpp
shifted(4, 3);  // 7
shifted(4);     // 5
```

第二次调用：

```cpp
shifted(4);
```

等价于：

```cpp
shifted(4, 1);
```

这里：

* `value = 4`
* `offset = 1`（使用默认值）

---

## 2. 默认参数必须从右往左连续出现

函数参数按照**位置**匹配，因此调用者只能省略参数列表末尾的参数。

合法：

```cpp
void func(int a, int b = 10);
```

可以：

```cpp
func(5);      // a = 5, b = 10
func(5, 20);  // a = 5, b = 20
```

如果某个参数有默认值，那么它右边的参数通常也必须有默认值。

合法：

```cpp
void func(int a, int b = 10, int c = 20);
```

非法：

```cpp
void func(int a = 10, int b);
```

因为：

```cpp
func(5);
```

编译器无法判断这个 `5` 是想传给 `a` 还是绕过 `a` 传给 `b`。

---

## 3. 默认参数通常只声明一次

例如：

```cpp
int func(int x, int y = 10);

int func(int x, int y) {
    return x + y;
}
```

默认值已经在声明中写过：

```cpp
y = 10
```

定义时不要再次重复：

```cpp
// 不要这样重复写
int func(int x, int y = 10) {
    ...
}
```

实际项目中默认参数通常放在头文件中的函数声明里，因为**调用函数的位置需要知道默认值**。

如果题目直接要求：

```cpp
static int countDigits(int value, int base = 10)
```

并且没有单独的声明，那么就在这个函数定义中保留 `= 10`。

---

# `countDigits` 问题

函数：

```cpp
static int countDigits(int value, int base = 10)
```

作用是计算整数 `value` 使用 `base` 进制表示时需要多少位。

例如：

```cpp
countDigits(1000);      // 4，默认十进制
countDigits(255, 16);   // 2，因为 255 = FF
countDigits(255, 2);    // 8，因为 255 = 11111111
```

---

## 1. 参数检查

题目规定：

```cpp
value < 0
```

或者：

```cpp
base < 2
```

返回：

```cpp
-1
```

因此应该在任何除法之前检查：

```cpp
if (value < 0 || base < 2) {
    return -1;
}
```

尤其是：

```cpp
base == 1
```

必须提前排除。

否则不断执行：

```cpp
value /= 1;
```

`value` 永远不会变成 0，循环就不会结束。

---

## 2. 为什么 `value == 0` 返回 1

数字：

```text
0
```

本身占一位。

因此无论什么进制：

```cpp
countDigits(0) == 1
```

例如：

```text
十进制：0
二进制：0
十六进制：0
```

都是一位。

---

## 3. 为什么不断除以 `base` 可以计算位数

以：

```cpp
countDigits(1000, 10)
```

为例：

```text
1000
100
10
1
0
```

每除一次 `10`，相当于去掉最右边的一位数字。

因此：

```text
1000 → 4 位
```

二进制也是一样：

```cpp
countDigits(255, 2)
```

不断除以 2：

```text
255
127
63
31
15
7
3
1
0
```

一共 8 位。

整数除法会自动丢弃小数部分：

```cpp
255 / 2 == 127
```

而不是：

```text
127.5
```

这正是这个算法需要的行为。

---

# 推荐算法

可以先假设数字至少有一位：

```cpp
int count = 1;
```

然后先除一次：

```cpp
value /= base;
```

只要剩余的值仍然大于 0，就继续：

```cpp
while (value > 0) {
    value /= base;
    count++;
}
```

这种写法自然处理：

```cpp
value == 0
```

因为初始的：

```cpp
count = 1
```

已经表示数字 `0` 占一位。

---

## 示例：`1000`

初始：

```text
count = 1
value = 1000
```

第一次：

```text
1000 / 10 = 100
```

然后循环：

```text
100 / 10 = 10    count = 2
10 / 10 = 1      count = 3
1 / 10 = 0       count = 4
```

最终：

```cpp
return 4;
```

---

# 这道题最重要的几个点

1. 保留函数签名：

```cpp
static int countDigits(int value, int base = 10)
```

2. `base` 的默认值是 `10`，因此：

```cpp
countDigits(1000)
```

等价于：

```cpp
countDigits(1000, 10)
```

3. 非法输入：

```cpp
value < 0 || base < 2
```

返回：

```cpp
-1
```

4. `value == 0` 返回 `1`。

5. 每除一次 `base`，相当于去掉一位数字。

6. 不要自己写 `main()`。

题目的测试程序会提供自己的：

```cpp
main()
```

如果你再定义一个 `main()`，程序会因为存在两个 `main` 而无法编译。
