# C++：引用（Reference）和指针（Pointer）作为函数参数

## 1. 核心区别

引用和指针都可以让函数直接访问调用者原来的对象，而不需要复制对象。

```cpp
void bump(int &value) {
    value += 1;
}

void bumpIfPresent(int *value) {
    if (value != nullptr) {
        *value += 1;
    }
}
```

它们最重要的区别是：

> **引用表示“这里一定有一个对象”；指针可以表示“可能没有对象”。**

---

## 2. 引用参数：对象一定存在

```cpp
void bump(int &value) {
    value += 1;
}
```

调用：

```cpp
int total = 10;
bump(total);
```

执行后：

```cpp
total == 11
```

这里：

```cpp
int &value
```

表示 `value` 是调用者变量的别名。

因此：

```cpp
value += 1;
```

实际上就是修改：

```cpp
total += 1;
```

### 引用的重要性质

引用：

* 必须绑定到一个真实对象
* 不能表示“没有对象”
* 不需要 `nullptr` 检查
* 绑定之后不能重新绑定到另一个对象

所以如果函数的意思是：

> “调用者一定会给我一个对象，我需要操作它。”

通常适合使用引用。

---

# 3. 指针参数：对象可能不存在

```cpp
void bumpIfPresent(int *value) {
    if (value != nullptr) {
        *value += 1;
    }
}
```

调用时可以传入一个对象：

```cpp
int total = 10;

bumpIfPresent(&total);
```

也可以明确表示“没有对象”：

```cpp
bumpIfPresent(nullptr);
```

因此指针参数通常表达：

> “这里可能有一个对象，也可能没有。”

---

## 4. 指针必须先检查，再解引用

如果一个指针可能是 `nullptr`：

```cpp
int *p = nullptr;
```

就不能直接：

```cpp
*p
```

正确模式是：

```cpp
if (p != nullptr) {
    // 现在才能安全使用 *p
}
```

例如：

```cpp
void bumpIfPresent(int *value) {
    if (value != nullptr) {
        *value += 1;
    }
}
```

原则可以记成：

```text
pointer
   ↓
检查 nullptr
   ↓
dereference (*pointer)
```

不要写成：

```cpp
*value += 1;   // value 可能是 nullptr
```

解引用空指针属于 **Undefined Behavior（未定义行为）**。

程序可能：

* 崩溃
* 输出奇怪结果
* 看起来“正常运行”
* 在不同编译器中表现不同

所以不能依赖“这里空指针读出来好像是 0”这种行为。

---

# 5. `&` 在参数声明中的含义

这里：

```cpp
void bump(int &value)
```

`&` 属于类型声明。

可以理解成：

```text
value 的类型 = int 的引用
```

调用时仍然直接写：

```cpp
bump(total);
```

而不是：

```cpp
bump(&total);   // 错误
```

这和指针不同。

指针参数：

```cpp
void f(int *value);
```

调用时通常需要传地址：

```cpp
f(&total);
```

---

# 6. 引用和指针的调用对比

假设：

```cpp
int total = 10;
```

### 引用

```cpp
void change(int &x) {
    x = 20;
}

change(total);
```

直接传变量：

```cpp
change(total);
```

---

### 指针

```cpp
void change(int *x) {
    if (x != nullptr) {
        *x = 20;
    }
}
```

传变量地址：

```cpp
change(&total);
```

或者：

```cpp
change(nullptr);
```

---

# 7. `const` 指针：只能读取目标对象

例如：

```cpp
const int *factor
```

这里的 `const` 修饰的是：

```text
factor 指向的 int
```

所以：

```cpp
int value = *factor;   // 可以
```

但是：

```cpp
*factor = 10;          // 不可以
```

这种参数特别适合表达：

> “这个对象可能不存在；如果存在，我只读取它，不修改它。”

例如：

```cpp
void applyFactor(int &total, const int *factor) {
    if (factor != nullptr) {
        total *= *factor;
    }
}
```

其中：

* `total`：一定存在，而且需要修改 → `int &`
* `factor`：可能不存在，只需要读取 → `const int *`

---

# 8. 普通值、引用、指针如何选择

函数参数经常可以按照下面的思路判断。

## 情况 1：只是读取一个很小的值

例如：

```cpp
int floorValue
bool enabled
double rate
```

通常直接传值：

```cpp
void f(int value);
```

因为复制这些类型成本很低。

---

## 情况 2：对象一定存在，而且函数需要访问或修改原对象

使用引用：

```cpp
void f(int &value);
```

如果只读：

```cpp
void f(const std::string &text);
```

例如：

```cpp
void print(const std::string &text);
```

这样：

* 不复制整个 `std::string`
* 又不允许函数修改它

---

## 情况 3：对象可能不存在

使用指针：

```cpp
void f(const int *value);
```

其中：

```cpp
nullptr
```

可以自然表示：

```text
没有这个值
```

---

# 9. 一个很好用的判断规则

可以把参数设计理解成三个问题。

### 第一个问题：只是读取一个小值吗？

是：

```cpp
int value
```

---

### 第二个问题：需要访问调用者原来的对象，而且它一定存在吗？

是：

```cpp
T &
```

或者只读：

```cpp
const T &
```

---

### 第三个问题：对象可能不存在吗？

是：

```cpp
T *
```

或者只读：

```cpp
const T *
```

因此可以简单记成：

| 情况          | 常见参数类型           |
| ----------- | ---------------- |
| 小对象，只读取     | `T value`        |
| 一定存在，需要修改   | `T &value`       |
| 一定存在，只读取大对象 | `const T &value` |
| 可能不存在，需要修改  | `T *value`       |
| 可能不存在，只读取   | `const T *value` |

最重要的设计依据通常不是“哪个更快”，而是：

> **这个参数在语义上到底能不能缺失？**

---

# 10. 本题中的参数设计

题目有三个 helper。

## `applyFactor`

需求：

* 修改调用者的 `total`
* `total` 一定存在
* `factor` 可能不存在
* 如果存在，只读取 factor

因此：

```cpp
void applyFactor(int &total, const int *factor)
```

逻辑：

```cpp
if (factor != nullptr) {
    total *= *factor;
}
```

---

## `raiseToFloor`

需求：

* 修改调用者的 `total`
* `floorValue` 只是一个小整数，只读取

因此：

```cpp
void raiseToFloor(int &total, int floorValue)
```

逻辑：

```cpp
if (total < floorValue) {
    total = floorValue;
}
```

---

## `labelWeight`

字符串通常不希望复制，所以使用：

```cpp
const std::string &label
```

只需要返回字符串长度：

```cpp
return static_cast<int>(label.size());
```

这里显式转换成 `int`，是因为：

```cpp
std::string::size()
```

返回的类型通常是：

```cpp
std::size_t
```

而函数要求返回 `int`。

---

# 11. 最终记忆口诀

```text
小值只读 → 传值

一定存在 → 引用
可能不存在 → 指针

只读 → const
需要修改 → 不加 const
```

例如：

```cpp
int value
```

表示：

> 给我一个值。

```cpp
int &value
```

表示：

> 给我一个一定存在的 int，我可能修改它。

```cpp
const int &value
```

表示：

> 给我一个一定存在的 int，我只读取它。

```cpp
int *value
```

表示：

> 可能有一个 int，也可能没有；如果有，我可能修改它。

```cpp
const int *value
```

表示：

> 可能有一个 int，也可能没有；如果有，我只读取它。

```
```
