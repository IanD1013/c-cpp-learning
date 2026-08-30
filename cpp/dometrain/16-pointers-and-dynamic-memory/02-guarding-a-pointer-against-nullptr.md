# C++ 指针：`nullptr`、空指针检查与 `const int *`

## 1. `nullptr`：表示“没有指向任何对象”

指针保存的是某个对象的地址。

```cpp
int value = 10;
int *p = &value;
```

这里 `p` 指向 `value`。

但有时候我们需要表示：

> 这个指针目前没有指向任何有效对象。

C++ 使用 `nullptr`：

```cpp
int *p = nullptr;
```

此时：

```cpp
p == nullptr
```

为 `true`。

---

## 2. 空指针绝对不能解引用

解引用：

```cpp
*p
```

表示：

> 访问指针指向的那个对象。

例如：

```cpp
int x = 10;
int *p = &x;

int y = *p;
```

`y` 得到 `10`。

但如果：

```cpp
int *p = nullptr;
int y = *p;
```

这是**未定义行为（Undefined Behavior）**。

不要依赖某次运行表现出来的结果。程序可能：

* 崩溃
* 得到 `0`
* 得到垃圾值
* 看起来正常运行
* 在不同编译器或优化级别下表现不同

因此核心规则是：

> **解引用指针之前，必须先确认它不是 `nullptr`。**

正确写法：

```cpp
const int *found = nullptr;

if (found == nullptr) {
    return -1;
}

int value = *found;
```

错误写法：

```cpp
int value = *found;

if (found == nullptr) {
    return -1;
}
```

因为执行到检查时，危险的解引用已经发生了。

---

## 3. Guard Clause：先处理失败情况

处理可能为空的指针时，一个非常常见的写法是 **guard clause（守卫语句）**：

```cpp
if (found == nullptr) {
    return -1;
}

return *found;
```

逻辑是：

1. 先处理异常/失败情况
2. 提前返回
3. 后面的代码就可以默认指针有效

相比：

```cpp
if (found != nullptr) {
    return *found;
} else {
    return -1;
}
```

guard clause 通常更加清晰。

---

# 4. `const int *` 是什么意思？

例如：

```cpp
const int *found;
```

这里的 `const` 修饰的是：

```cpp
int
```

所以它的意思是：

> `found` 是一个指向 `const int` 的指针。

也可以理解为：

> 可以通过 `found` 读取整数，但不能通过 `found` 修改整数。

例如：

```cpp
const int *p = &value;

int x = *p;    // 可以读取
```

但是：

```cpp
*p = 20;       // 编译错误
```

---

## 5. 注意：指针本身并不是 const

```cpp
const int *p;
```

这里不能修改的是：

> `p` 指向的整数。

但 `p` 自己仍然可以改成指向其他地方：

```cpp
int a = 10;
int b = 20;

const int *p = &a;

p = &b;        // 可以
```

因此：

```cpp
const int *p
```

可以记成：

> **pointer to const int**

即：

> 指向只读 `int` 的指针。

---

# 6. 返回值类型必须匹配

假设函数：

```cpp
const int *firstAtLeast(...);
```

返回的是：

```cpp
const int *
```

那么接收时应该写：

```cpp
const int *found = firstAtLeast(values, threshold);
```

不能写：

```cpp
int *found = firstAtLeast(values, threshold);
```

原因是：

```cpp
int *
```

意味着调用者可以这样做：

```cpp
*found = 100;
```

但原函数返回的是：

```cpp
const int *
```

它只允许读取，不允许修改。

因此编译器不会允许你把：

```cpp
const int *
```

偷偷转换成：

```cpp
int *
```

因为这相当于获得了原本不存在的修改权限。

---

# 7. 指针声明时应该初始化

危险写法：

```cpp
const int *found;
```

此时 `found` 没有初始化。

它保存的是一个**不确定值（indeterminate value）**。

不能假设它自动等于：

```cpp
nullptr
```

因此下面的代码本身就不可靠：

```cpp
const int *found;

if (found == nullptr) {
    ...
}
```

正确做法：

```cpp
const int *found = nullptr;
```

或者直接让它获得一个有效返回值：

```cpp
const int *found = firstAtLeast(values, threshold);
```

可以记住：

> **指针声明时尽量立即初始化。**

---

# 8. “没有找到”与合法数据必须能够区分

假设函数查找：

> 第一个大于等于 `threshold` 的元素。

如果没有找到，需要返回一个特殊值。

如果使用：

```cpp
return 0;
```

但 `0` 本身可能也是合法数据，那么调用者就无法判断：

```text
返回 0
```

究竟表示：

* 找到了真正的 `0`
* 还是没有找到

因此这道题规定：

```cpp
return -1;
```

并保证数据范围为：

```text
1 ~ 500
```

所以：

```text
-1 = 明确的“没有找到”
```

---

# 9. 典型搜索函数的使用模式

假设已经有：

```cpp
const int *firstAtLeast(
    const std::vector<int> &values,
    int threshold
);
```

它的行为是：

```text
找到：
    返回对应元素的地址

没找到：
    返回 nullptr
```

调用者应该按照下面的模式处理：

```cpp
const int *found = firstAtLeast(values, threshold);

if (found == nullptr) {
    return -1;
}

return *found;
```

这是一种非常常见的 C++ 指针 API 使用方式：

```text
调用函数
   ↓
得到指针
   ↓
检查 nullptr
   ↓
失败 → 处理失败
   ↓
成功 → 解引用
```

---

# 10. 本节最重要的规则

## Rule 1：永远不要解引用 `nullptr`

```cpp
*p
```

之前必须确保：

```cpp
p != nullptr
```

---

## Rule 2：检查必须发生在解引用之前

正确：

```cpp
if (p == nullptr) {
    return -1;
}

return *p;
```

错误：

```cpp
int value = *p;

if (p == nullptr) {
    return -1;
}
```

---

## Rule 3：指针应该初始化

推荐：

```cpp
int *p = nullptr;
```

不要：

```cpp
int *p;
```

---

## Rule 4：`const int *` 表示只能通过指针读取

```cpp
const int *p;
```

允许：

```cpp
int value = *p;
```

禁止：

```cpp
*p = 100;
```

---

## Rule 5：函数返回 `const int *`，接收方也应使用 `const int *`

```cpp
const int *found = firstAtLeast(...);
```

不能擅自去掉 `const`。

---

# 一句话总结

对于可能返回 `nullptr` 的指针函数，标准使用模式是：

```cpp
const int *p = function();

if (p == nullptr) {
    // 处理失败
}

return *p;
```

即：

> **先判断指针是否存在，再解引用读取对象。**
