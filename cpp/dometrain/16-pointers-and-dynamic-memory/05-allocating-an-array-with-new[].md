# C++ 动态数组：`new[]` 与 `delete[]`

## 1. 一次动态创建多个对象

前面我们见过创建单个动态对象：

```cpp
int *p = new int(7);

delete p;
```

如果想一次创建多个连续的对象，需要使用数组形式的 `new`：

```cpp
int size = 3;
int *buffer = new int[size];
```

这里：

```cpp
new int[size]
```

表示：

> 在动态内存中申请一块能够连续存放 `size` 个 `int` 的空间，并返回第一个元素的地址。

因此返回值仍然是：

```cpp
int *
```

也就是指向第一个元素的指针。

---

## 2. 使用 `[]` 访问动态数组

例如：

```cpp
int size = 3;
int *buffer = new int[size];

for (int i = 0; i < size; i++) {
    buffer[i] = i * 10;
}
```

最终：

```text
buffer[0] = 0
buffer[1] = 10
buffer[2] = 20
```

访问方式和普通数组、`std::vector` 很像：

```cpp
buffer[i]
```

可以读取：

```cpp
int x = buffer[1];
```

也可以修改：

```cpp
buffer[1] = 100;
```

从概念上可以理解为：

```text
buffer
  ↓
+----+----+----+
| 0  | 10 | 20 |
+----+----+----+
  0    1    2
```

`buffer` 保存的是第一个元素的地址。

---

## 3. 动态数组不会记录自己的长度

这是动态数组和 `std::vector` 的一个重要区别。

对于 vector：

```cpp
std::vector<int> values = {1, 2, 3};

values.size();
```

它自己知道有多少元素。

但是：

```cpp
int *buffer = new int[3];
```

`buffer` 只是一个指针。

它不知道：

```text
“我后面有 3 个 int。”
```

因此必须自己保存长度：

```cpp
int size = 3;
int *buffer = new int[size];
```

之后所有访问都必须确保：

```cpp
0 <= i < size
```

例如：

```cpp
for (int i = 0; i < size; i++) {
    buffer[i] = i * 10;
}
```

---

## 4. 越界访问非常危险

假设：

```cpp
int *buffer = new int[3];
```

合法位置只有：

```cpp
buffer[0]
buffer[1]
buffer[2]
```

下面都是越界：

```cpp
buffer[3] = 100;
buffer[10] = 200;
```

C++ 的原始数组通常不会自动检查越界。

程序可能：

* 崩溃
* 修改其他内存
* 得到错误结果
* 暂时看起来完全正常

也就是说：

> “程序没有崩溃”并不能说明访问是合法的。

这种行为属于 **Undefined Behavior（未定义行为）**。

因此动态数组必须自己保证索引合法。

---

# 5. `new[]` 必须对应 `delete[]`

动态数组创建：

```cpp
int *buffer = new int[size];
```

释放时必须：

```cpp
delete[] buffer;
```

对应关系是：

```text
new       → delete
new[]     → delete[]
```

例如：

```cpp
int *p = new int(7);
delete p;
```

而：

```cpp
int *buffer = new int[10];
delete[] buffer;
```

不能混用：

```cpp
int *buffer = new int[10];

delete buffer;   // 错误
```

即使某些情况下：

* 编译器没有报错
* 程序没有立即崩溃

它仍然是错误的内存管理。

---

# 6. 为什么数组需要 `delete[]`

考虑：

```cpp
Item *items = new Item[3];
```

实际上创建了三个 `Item`：

```text
Item
Item
Item
```

释放时：

```cpp
delete[] items;
```

需要正确结束整个数组中所有对象的生命周期。

因此 C++ 区分：

```cpp
delete
```

和：

```cpp
delete[]
```

---

# 7. 提前 `return` 前也必须释放内存

考虑：

```cpp
int *buffer = new int[size];

for (...) {
    if (发生错误) {
        return {-1};
    }
}

delete[] buffer;
```

这里存在问题。

如果执行：

```cpp
return {-1};
```

函数立即结束，因此：

```cpp
delete[] buffer;
```

永远不会运行。

这会产生：

```text
memory leak
内存泄漏
```

正确方式是：

```cpp
if (发生错误) {
    delete[] buffer;
    return {-1};
}
```

因此使用原始 `new[]` 时必须记住：

> 每一条可能结束函数的路径，都要考虑动态内存是否已经释放。

---

# 8. `new int[0]` 是允许的

下面是合法代码：

```cpp
int *buffer = new int[0];
```

但是它表示：

```text
0 个可以访问的元素
```

所以绝对不能：

```cpp
buffer[0]
```

但是这块动态分配仍然应该使用：

```cpp
delete[] buffer;
```

释放：

```cpp
int *buffer = new int[0];

delete[] buffer;
```

因此：

```cpp
keep == 0
```

并不意味着一定不能调用 `new[]`。

---

# 9. 本题的核心控制流程

题目要求实现：

```cpp
static std::vector<int> doubledPrefix(
    const std::vector<int> &values,
    int keep
)
```

目标是：

> 只处理 `values` 前 `keep` 个元素，将它们乘以 2。

例如：

```cpp
values = {3, 1, 4, 1}
keep = 3
```

只处理：

```text
3, 1, 4
```

得到：

```text
6, 2, 8
```

第四个：

```text
1
```

完全不应该访问。

---

## 10. 第一阶段：先验证 `keep`

如果：

```cpp
keep < 0
```

或者：

```cpp
keep > values.size()
```

必须立即：

```cpp
return {-1};
```

而且题目特别要求：

> 这一步必须发生在动态内存分配之前。

逻辑应该是：

```text
检查 keep
    ↓
不合法 → return {-1}
    ↓
合法
    ↓
new int[keep]
```

这样错误输入不会产生没必要的动态内存。

---

# 11. 第二阶段：只检查前 `keep` 个元素

假设：

```cpp
values = {3, -7, 4};
keep = 3;
```

需要检查：

```text
3
-7
4
```

因为 `-7` 位于前 `keep` 个元素内，因此失败：

```text
[-1]
```

但是：

```cpp
values = {3, 4, -7};
keep = 2;
```

只需要看：

```text
3
4
```

第三个：

```text
-7
```

根本不属于处理范围。

结果应该是：

```text
[6, 8]
```

因此循环范围必须是：

```cpp
for (int i = 0; i < keep; i++)
```

而不是：

```cpp
for (int i = 0; i < values.size(); i++)
```

---

# 12. 遇到负数时先释放，再返回

已经执行：

```cpp
int *buffer = new int[keep];
```

以后，如果发现：

```cpp
values[i] < 0
```

不能直接：

```cpp
return {-1};
```

必须：

```cpp
delete[] buffer;
return {-1};
```

顺序非常重要：

```text
发现错误
   ↓
delete[] buffer
   ↓
return {-1}
```

否则会产生内存泄漏。

---

# 13. `keep == 0` 和错误必须能够区分

题目规定：

```cpp
keep == 0
```

返回：

```cpp
{}
```

也就是空 vector。

因此不能用：

```cpp
{}
```

代表错误。

错误专门使用：

```cpp
{-1}
```

于是：

```text
{}      → 合法，只是要求保留 0 个元素

{-1}    → 输入不合法 / 前 keep 个元素存在负数
```

---

# 14. 整个算法

可以把本题记成：

```text
1. 检查 keep 是否合法
      ↓
   不合法 → {-1}

2. new int[keep]
      ↓

3. 遍历 i = 0 ... keep-1
      ↓
   values[i] < 0 ?
      ↓ yes
   delete[] buffer
   return {-1}

4. buffer[i] = values[i] * 2

5. 把 buffer 中的数据复制进 vector

6. delete[] buffer

7. return vector
```

其中最值得掌握的是：

> 一旦成功执行了 `new[]`，之后每一条退出函数的路径都必须考虑对应的 `delete[]`。

---

# 15. 现代 C++ 中通常不应该这样写

这道题是在练习：

```cpp
new[]
delete[]
```

所以必须使用原始动态数组。

但真实项目中，如果只是需要动态长度的整数数组，通常直接使用：

```cpp
std::vector<int>
```

例如现实代码通常可以写成：

```cpp
std::vector<int> result;

for (int i = 0; i < keep; i++) {
    result.push_back(values[i] * 2);
}
```

这样就不需要自己写：

```cpp
new[]
delete[]
```

也减少：

* 内存泄漏
* `delete` / `delete[]` 混用
* 提前 `return` 忘记释放
* 异常发生时资源没有清理

因此可以把这一课理解为：

> 学习底层动态数组的生命周期和资源管理规则，而不是推荐以后大量手写 `new[]`。
