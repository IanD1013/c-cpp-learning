# C++ `struct` 的大括号初始化（Brace Initialization）

## 1. 基本语法

定义一个结构体：

```cpp
struct Span {
    int start;
    int length;
};
```

创建结构体变量时，可以直接使用 `{}` 给成员赋初值：

```cpp
int firstIndex = 4;

Span span{firstIndex, 9};
```

此时等价于：

```cpp
span.start = 4;
span.length = 9;
```

---

## 2. 初始化值按照“成员声明顺序”匹配

最重要的一点：

> `{}` 中的值是按照结构体成员的声明顺序填入的，而不是根据变量名字匹配。

例如：

```cpp
struct Span {
    int start;
    int length;
};
```

成员顺序是：

1. `start`
2. `length`

所以：

```cpp
Span span{4, 9};
```

表示：

```cpp
span.start == 4;
span.length == 9;
```

即使写成：

```cpp
int firstIndex = 4;

Span span{9, firstIndex};
```

也完全合法，但含义变成：

```cpp
span.start == 9;
span.length == 4;
```

编译器并不会因为变量叫 `firstIndex`，就自动把它放进 `start`。

---

## 3. 初始化时保存的是值的副本

例如：

```cpp
int firstIndex = 4;

Span span{firstIndex, 9};

firstIndex = 100;
```

最后：

```cpp
span.start == 4
```

而不是：

```cpp
span.start == 100
```

原因是：

```cpp
Span span{firstIndex, 9};
```

执行时，会把 `firstIndex` 当前的值 `4` **复制**到 `span.start`。

可以理解为：

```text
firstIndex ──值 4──► span.start
```

复制完成以后，两者就是两个独立的 `int`。

修改：

```cpp
firstIndex = 100;
```

不会影响：

```cpp
span.start
```

---

## 4. `struct` 类型和 `struct` 变量要区分

假设：

```cpp
struct Range {
    int low;
    int high;
};
```

这里：

```cpp
Range
```

是一个**类型（type）**。

而：

```cpp
Range range{3, 10};
```

中的：

```cpp
range
```

才是一个真正的**变量（object / variable）**。

因此读取成员时应该写：

```cpp
range.low
range.high
```

不能写：

```cpp
Range.low
Range.high
```

因为 `.` 左边必须是一个实际对象。

可以类比：

```cpp
int number = 10;
```

`int` 是类型，`number` 是变量。

同样：

```cpp
Range range{3, 10};
```

`Range` 是类型，`range` 是变量。

---

## 5. `{}` 初始化非常依赖成员顺序

例如：

```cpp
struct Range {
    int low;
    int high;
};
```

那么：

```cpp
Range range{low, high};
```

表示：

```cpp
range.low = low;
range.high = high;
```

如果反过来：

```cpp
Range range{high, low};
```

则表示：

```cpp
range.low = high;
range.high = low;
```

所以看到：

```cpp
Type variable{a, b};
```

时，应该去查看 `struct` 的定义：

```cpp
struct Type {
    ...
};
```

然后按照成员声明顺序解释 `{a, b}`。

---

## 6. 核心总结

```cpp
struct Range {
    int low;
    int high;
};

Range range{3, 9};
```

应该理解为：

```text
Range
 ├── low  = 3
 └── high = 9
```

记住三个重点：

1. `{}` 中的值按照成员**声明顺序**匹配。
2. 普通类型成员保存的是值的**副本**。
3. 使用 `.` 时，左边必须是实际变量，例如：

```cpp
range.low
```

而不是：

```cpp
Range.low
```
