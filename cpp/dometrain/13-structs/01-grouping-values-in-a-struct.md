# C++ `struct` 结构体基础

## 1. `struct` 是什么？

在 C++ 中，`struct` 可以把多个变量组合起来，定义成一个新的类型。

例如：

```cpp
struct Point {
    int x;
    int y;
};
```

这里定义了一个新类型：

```cpp
Point
```

它内部包含两个成员：

```cpp
int x;
int y;
```

可以把它理解成：

> 一个 `Point` 对象中同时保存一个 `x` 和一个 `y`。

---

## 2. 定义 `struct` 的基本语法

基本格式：

```cpp
struct 类型名 {
    类型 成员名;
    类型 成员名;
};
```

例如：

```cpp
struct Point {
    int x;
    int y;
};
```

注意结尾是：

```cpp
};
```

也就是右花括号 `}` 后面还需要一个分号 `;`。

因为整个 `struct` 定义本身是一条声明。

---

## 3. 定义类型 ≠ 创建变量

下面这段代码：

```cpp
struct Point {
    int x;
    int y;
};
```

只是定义了一个叫做 `Point` 的类型，并没有真正创建一个 `Point` 对象。

真正创建变量：

```cpp
Point origin;
```

此时 `origin` 内部才真正包含：

```cpp
origin.x
origin.y
```

两个 `int` 成员。

可以理解为：

```text
struct Point {...};
    ↓
设计图

Point origin;
    ↓
按照设计图创建一个实际对象
```

---

## 4. 使用 `.` 访问成员

对于普通结构体变量，可以使用：

```cpp
变量名.成员名
```

访问成员。

例如：

```cpp
Point origin;

origin.x = 3;
origin.y = 4;
```

之后：

```cpp
int sum = origin.x + origin.y;
```

等价于：

```cpp
int sum = 3 + 4;
```

因此：

```cpp
sum == 7
```

结构体成员本质上就是普通变量，所以可以进行正常操作：

```cpp
origin.x = 10;

if (origin.x > 5) {
    // ...
}

int result = origin.x * origin.y;
```

---

## 5. 每个对象都有自己独立的成员

例如：

```cpp
Point a;
Point b;

a.x = 1;
b.x = 100;
```

那么：

```cpp
a.x
```

和：

```cpp
b.x
```

是两个完全不同的变量。

可以想象为：

```text
a
├── x = 1
└── y

b
├── x = 100
└── y
```

因此 `struct` 特别适合表示“一组属于同一个东西的数据”。

例如：

```cpp
struct Student {
    int age;
    int score;
};
```

或者：

```cpp
struct Part {
    int unitPrice;
    int quantity;
};
```

---

## 6. 未初始化成员的问题

如果写：

```cpp
Point origin;
```

对于这里的普通局部变量，`origin.x` 和 `origin.y` 并不会自动获得一个可靠的值。

因此不能直接：

```cpp
Point origin;

int sum = origin.x + origin.y;  // 错误做法
```

因为成员还没有被初始化。

应该先赋值：

```cpp
Point origin;

origin.x = 3;
origin.y = 4;

int sum = origin.x + origin.y;
```

或者直接使用聚合初始化：

```cpp
Point origin{3, 4};
```

这样：

```cpp
origin.x == 3
origin.y == 4
```

---

## 7. 本题中的 `Part`

题目要求定义：

```cpp
struct Part {
    int unitPrice;
    int quantity;
};
```

它表示一个商品/零件的信息：

```text
Part
├── unitPrice   单价
└── quantity    数量
```

创建变量：

```cpp
Part part;
```

然后把函数参数保存进去：

```cpp
part.unitPrice = unitPrice;
part.quantity = quantity;
```

之后计算总价时，不直接使用函数参数，而是通过结构体成员：

```cpp
part.unitPrice * part.quantity
```

---

## 核心总结

### 定义结构体

```cpp
struct Point {
    int x;
    int y;
};
```

### 创建结构体变量

```cpp
Point point;
```

### 给成员赋值

```cpp
point.x = 3;
point.y = 4;
```

### 读取成员

```cpp
int result = point.x + point.y;
```

最重要的概念是：

```text
struct 定义一种新的数据类型
对象是真正的数据
`.` 用来访问对象中的成员
```
