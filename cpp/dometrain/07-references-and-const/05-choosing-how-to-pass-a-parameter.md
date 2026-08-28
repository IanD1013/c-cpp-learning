# C++ 函数参数选择：值传递、引用、const 引用

在设计函数参数时，可以先问两个问题：

1. **函数需不需要修改调用者的变量？**
2. **这个参数复制起来是否昂贵？**

基本判断规则：

```text
需要修改调用者变量
    ↓
使用 T &

不需要修改
    ↓
复制是否昂贵？
    ├─ 是 → 使用 const T &
    └─ 否 → 使用 T
```

---

## 1. 普通引用 `T &`

如果函数需要直接修改调用者传入的变量，就使用引用参数。

```cpp
static void addOne(int &counter) {
    counter = counter + 1;
}
```

调用：

```cpp
int n = 5;
addOne(n);
```

执行后：

```cpp
n == 6
```

原因是：

```cpp
int &counter
```

不会创建一个新的 `int`，而是让 `counter` 成为调用者变量 `n` 的另一个名字。

所以：

```cpp
counter = counter + 1;
```

实际上修改的就是：

```cpp
n
```

### 适用场景

当函数的目的之一就是修改调用者的数据时：

```cpp
void addTo(int &total, int amount);
void swap(int &a, int &b);
void reset(Player &player);
```

---

## 2. const 引用 `const T &`

如果函数：

* 不需要修改参数
* 但复制参数比较昂贵

通常使用：

```cpp
const T &name
```

例如：

```cpp
static std::string repeated(const std::string &text) {
    return text + text;
}
```

这里：

```cpp
const std::string &text
```

有两个作用。

### `&`

不复制整个字符串，而是直接引用调用者的字符串。

### `const`

保证函数不能通过 `text` 修改原来的字符串。

例如下面的代码不能编译：

```cpp
text = "hello";
```

因此：

```cpp
const std::string &
```

可以理解成：

> **只读地引用调用者的数据，不进行昂贵复制。**

---

# 3. 值传递 `T`

对于很小的数据类型，通常直接复制即可：

```cpp
int
double
char
bool
```

例如：

```cpp
static int tripled(int value) {
    return value * 3;
}
```

调用：

```cpp
int x = 4;
int result = tripled(x);
```

函数内部会得到自己的：

```cpp
value = 4
```

修改 `value` 不会影响 `x`。

这些类型通常只有几个字节，因此为了避免一次很小的复制而使用引用，往往没有必要。

---

# 4. 三种参数方式对比

| 参数写法             | 是否复制 | 能否修改调用者变量 | 常见用途                |
| ---------------- | ---- | --------- | ------------------- |
| `T value`        | 是    | 否         | `int`、`double` 等小对象 |
| `T &value`       | 否    | 是         | 函数需要修改调用者变量         |
| `const T &value` | 否    | 否         | `string`、大型对象，只读    |

例如：

```cpp
static void addOne(int &counter) {
    counter = counter + 1;
}

static std::string repeated(const std::string &text) {
    return text + text;
}

static int tripled(int value) {
    return value * 3;
}
```

分别对应：

```text
addOne
→ 修改调用者
→ int &

repeated
→ 不修改
→ string 复制较贵
→ const std::string &

tripled
→ 不修改
→ int 很小
→ int
```

---

# 5. 不要把引用当成“性能开关”

下面这种写法通常不好：

```cpp
static int tripled(int &value) {
    return value * 3;
}
```

因为函数根本没有修改 `value`。

看到：

```cpp
int &
```

别人通常会理解成：

> 这个函数可能修改我传进去的变量。

但实际上并没有。

因此引用不仅是性能问题，它还表达了函数接口的语义。

比较：

```cpp
void update(int &value);
```

意味着：

> `value` 可能被修改。

而：

```cpp
int calculate(int value);
```

意味着：

> 函数只是使用这个值进行计算。

---

# 6. `const int value` 通常没有必要

例如：

```cpp
static int doubled(const int value) {
    return value * 2;
}
```

这里的：

```cpp
const
```

并不能保护调用者的数据。

因为在进入函数之前：

```cpp
value
```

已经是调用者参数的副本了。

例如：

```cpp
int x = 5;
doubled(x);
```

实际上类似：

```text
x
│
│ copy
▼
value
```

`const` 只意味着：

```cpp
value = 10;
```

在函数内部不允许。

但是无论有没有 `const`：

```cpp
x
```

本来就不会被修改。

所以对于简单值传递参数：

```cpp
int value
double value
bool value
```

一般不需要写：

```cpp
const int value
```

---

# 7. 综合例子

假设有：

```cpp
static int totalDoubledOnMatch(
    int start,
    int amount,
    const std::string &first,
    const std::string &second
);
```

它内部使用三个辅助函数：

```cpp
addTo(...)
sameText(...)
doubled(...)
```

## `addTo`

需要修改调用者的总数：

```cpp
static void addTo(int &running, int amount) {
    running += amount;
}
```

这里：

```cpp
running
```

必须是引用，因为修改必须保留下来。

---

## `sameText`

只比较两个字符串：

```cpp
static bool sameText(
    const std::string &left,
    const std::string &right
) {
    return left == right;
}
```

字符串不需要修改，也不希望进行复制：

```cpp
const std::string &
```

最合适。

---

## `doubled`

只是读取一个小整数：

```cpp
static int doubled(int value) {
    return value * 2;
}
```

`int` 很小，所以直接值传递即可。

---

# 核心判断法

以后看到函数参数，可以快速使用下面的规则：

```text
函数需要修改调用者变量？
│
├─ 是
│   ↓
│   T &
│
└─ 否
    ↓
参数复制昂贵？
    │
    ├─ 是
    │   ↓
    │   const T &
    │
    └─ 否
        ↓
        T
```

最常见的 C++ 风格就是：

```cpp
int value
double value
bool value

const std::string &text
const std::vector<int> &values

int &result
std::string &output
```

一句话总结：

> **要修改调用者 → `T &`；只读大对象 → `const T &`；只读小对象 → `T`。**
