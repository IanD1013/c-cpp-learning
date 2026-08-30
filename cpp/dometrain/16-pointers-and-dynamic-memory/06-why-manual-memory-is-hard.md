# C++ 动态内存常见错误：Leak、Double Free、Dangling Pointer 与 delete 不匹配

在 C++ 中使用 `new` / `delete` 管理动态内存时，有一类错误非常危险：

> **程序可能正常编译、正常运行，甚至返回看起来完全正确的结果，但代码本身依然是错误的。**

因此，不能用“程序有没有崩溃”来判断动态内存操作是否合法。

核心原则是：

```text
动态内存错误主要靠分析代码中的：
1. 谁分配了内存
2. 谁持有地址
3. 什么时候释放
4. 是否释放了一次且仅一次
5. new 和 delete 的形式是否匹配
6. 释放以后是否还继续使用该地址
```

---

## 1. Memory Leak：内存泄漏

### 定义

如果一块由 `new` 创建的内存，在程序失去它之前没有执行对应的 `delete`，就发生了 **memory leak（内存泄漏）**。

例如：

```cpp
int *p = new int(10);

return;
delete p;
```

`return` 发生后，`delete p` 永远不会执行。

因此：

```text
new 创建的内存仍然存在
↓
但程序已经无法再释放它
↓
memory leak
```

---

### 常见形式 1：提前 return

```cpp
int *p = new int(10);

if (condition) {
    return 5;
}

delete p;
```

如果 `condition == true`：

```cpp
new
↓
return
↓
delete 被跳过
```

因此产生泄漏。

---

### 常见形式 2：覆盖唯一的指针

```cpp
int *p = new int(10);

p = new int(20);
```

第一次 `new` 得到的地址原本保存在 `p` 中。

第二次赋值后：

```text
p
↓
第二块内存
```

第一块内存的地址已经丢失。

因此再也无法：

```cpp
delete 第一块内存;
```

这也是内存泄漏。

---

### 识别口诀

看到：

```cpp
new
```

就立刻问：

> 对应的 `delete` 是否在所有执行路径上都一定会执行？

如果答案是否定的，就可能是 leak。

---

# 2. Double Free：重复释放

### 定义

同一块动态内存只能释放一次。

例如：

```cpp
int *p = new int(10);

delete p;
delete p;
```

第二个：

```cpp
delete p;
```

就是 **double free**。

---

### 更隐蔽的情况

```cpp
int *p = new int(10);

if (condition) {
    delete p;
}

delete p;
```

如果：

```cpp
condition == true
```

执行过程为：

```text
delete p
↓
delete p
```

同一地址被释放两次。

---

### 两个指针也可能导致 double free

```cpp
int *p = new int(10);
int *q = p;

delete p;
delete q;
```

虽然变量名不同：

```text
p
 \
  → 同一块内存
 /
q
```

所以：

```cpp
delete p;
delete q;
```

仍然是对同一地址释放两次。

---

## 注意

double free 属于 **Undefined Behavior（未定义行为）**。

程序可能：

* 崩溃
* 不崩溃
* 看起来完全正常
* 在不同机器上表现不同

因此不能依赖运行结果判断。

---

# 3. Dangling Pointer：悬空指针

## 定义

如果指针仍然保存一个地址，但那个地址对应的对象已经死亡，这个指针就叫：

> **dangling pointer（悬空指针）**

例如：

```cpp
int *p = new int(10);

delete p;

int x = *p;
```

执行：

```cpp
delete p;
```

之后：

```text
p 中的地址仍然存在
```

但是：

```text
那个地址上的对象已经不存在了
```

因此：

```cpp
*p
```

是在访问已经释放的对象。

这叫：

> **use-after-free / read through a released pointer**

---

## 为什么它特别危险？

因为释放内存后，那块内存中的旧数据不一定马上消失。

例如：

```cpp
int *p = new int(42);

delete p;

cout << *p;
```

有时可能仍然打印：

```text
42
```

但这并不意味着代码正确。

只是那块内存暂时还没有被其他数据覆盖。

下一次运行可能得到：

```text
0
垃圾值
不同数字
崩溃
```

因此：

> **读取到了正确值，也不能证明指针仍然有效。**

---

## 另一种 dangling pointer

局部变量离开作用域后：

```cpp
int *p;

{
    int x = 10;
    p = &x;
}

cout << *p;
```

离开 `{}` 后：

```cpp
x
```

已经死亡。

所以 `p` 也变成 dangling pointer。

---

# 4. new / delete 形式不匹配

C++ 有两组动态内存操作。

## 单个对象

```cpp
int *p = new int(10);

delete p;
```

对应关系：

```text
new
↓
delete
```

---

## 动态数组

```cpp
int *p = new int[10];

delete[] p;
```

对应关系：

```text
new[]
↓
delete[]
```

因此必须记住：

```cpp
new      → delete
new[]    → delete[]
```

不能写成：

```cpp
int *p = new int[10];

delete p;      // 错误
```

正确的是：

```cpp
delete[] p;
```

---

## 为什么对象数组尤其重要？

假设：

```cpp
Item *items = new Item[5];
```

实际上创建了：

```text
Item
Item
Item
Item
Item
```

释放：

```cpp
delete[] items;
```

才能正确结束整个数组中所有对象的生命周期。

使用错误的：

```cpp
delete items;
```

属于 **Undefined Behavior**。

即使某次运行只调用了一个析构函数、程序也没有崩溃，都不能认为它是合法代码。

---

# 5. `p = nullptr` 不能修复之前的错误

经常会看到：

```cpp
delete p;
p = nullptr;
```

这是一个不错的习惯，因为它可以防止继续把 `p` 当成有效地址使用。

但是：

> 设置 `nullptr` 只能防止之后误用这个指针，不能修复已经发生的内存错误。

例如：

```cpp
delete p;
delete p;
```

已经发生 double free。

之后再写：

```cpp
p = nullptr;
```

没有意义。

同样：

```cpp
delete p;

int x = *p;

p = nullptr;
```

`*p` 已经发生 use-after-free。

最后设置 `nullptr` 无法让之前的访问变正确。

---

# 6. 不要通过运行结果判断这些错误

下面这些行为都属于 Undefined Behavior：

```text
double free
use-after-free
new[] 与 delete 不匹配
```

Undefined Behavior 的意思不是：

```text
程序一定崩溃
```

而是：

```text
C++ 不再保证程序会发生什么
```

所以可能：

```text
正常运行
返回正确结果
返回错误结果
崩溃
不同机器结果不同
优化后结果改变
```

因此这类题目的正确方法是：

> **静态阅读代码，而不是运行代码看结果。**

---

# 7. 做题时的检查方法

对于每一个函数，可以按照下面顺序检查。

## Step 1：找到所有 `new`

例如：

```cpp
Node *p = new Node;
```

记录：

```text
创建了一块动态内存 A
```

---

## Step 2：追踪所有指向 A 的指针

例如：

```cpp
Node *q = p;
```

此时：

```text
p ─┐
   ├──→ A
q ─┘
```

变量不同，不代表内存不同。

---

## Step 3：找到对应的 delete

检查：

```cpp
delete p;
```

或者：

```cpp
delete[] p;
```

---

## Step 4：检查所有执行路径

尤其注意：

```cpp
if
else
return
break
```

例如：

```cpp
Node *p = new Node;

if (condition) {
    return 1;
}

delete p;
```

存在：

```text
new → return
```

但没有：

```text
delete
```

因此是 leak。

---

## Step 5：检查 delete 是否出现两次

例如：

```cpp
delete p;
delete q;
```

如果：

```cpp
q == p
```

则为 double free。

---

## Step 6：检查 delete 后是否继续解引用

例如：

```cpp
delete p;

return p->value;
```

这里：

```cpp
p->value
```

等价于通过已经释放的指针读取数据。

属于：

```text
dangling pointer / use-after-free
```

---

## Step 7：检查 new/delete 是否配对

最后检查：

```text
new      ↔ delete
new[]    ↔ delete[]
```

---

# 8. 本题的缺陷编号

题目规定：

| 返回值  | 含义                     |
| ---- | ---------------------- |
| `0`  | 没有缺陷                   |
| `1`  | Memory leak            |
| `2`  | Double free            |
| `3`  | 读取已经释放的指针              |
| `4`  | `new` / `delete` 形式不匹配 |
| `-1` | 不存在这个函数编号              |

有效函数编号只有：

```text
1
2
3
4
5
```

因此：

```cpp
0
负数
6
7
...
```

都应该返回：

```cpp
-1
```

---

# 核心记忆

动态内存题可以浓缩成四句话：

```text
new 之后必须 delete。
同一块内存只能 delete 一次。
delete 之后不能再 dereference。
new[] 必须使用 delete[]。
```

以及最重要的一条：

> **程序没崩溃，不代表动态内存代码正确。**
