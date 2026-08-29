# C++ `vector` 的逐元素转换（Transformation / Mapping）

## 1. 什么叫“转换”一个 `vector`

所谓 **transformation（转换）**，就是：

> 对输入 `vector` 中的每一个元素做一次处理，并为它生成一个对应的输出元素。

例如：

```cpp
std::vector<int> v = {3, 0, -1};

std::vector<int> result;
for (int value : v) {
    result.push_back(value + 10);
}
```

最终：

```cpp
result == {13, 10, 9}
```

对应关系是：

```text
3   -> 13
0   -> 10
-1  -> 9
```

最重要的特点是：

```text
一个输入元素 -> 一个输出元素
```

因此：

```cpp
result.size() == v.size()
```

---

## 2. 转换的基本模板

处理 `vector` 转换题时，可以直接记住下面这个结构：

```cpp
std::vector<类型> result;

for (元素类型 value : input) {
    result.push_back(转换后的值);
}

return result;
```

例如，把每个数字平方：

```cpp
std::vector<int> result;

for (int value : numbers) {
    result.push_back(value * value);
}

return result;
```

如果：

```cpp
numbers = {2, -3, 0};
```

那么：

```cpp
result = {4, 9, 0};
```

---

## 3. 为什么转换后的大小通常和输入相同

例如：

```cpp
for (int value : v) {
    result.push_back(value + 10);
}
```

循环会执行：

```text
v.size()
```

次。

而每一次循环都会执行一次：

```cpp
result.push_back(...)
```

所以最终一定有：

```cpp
result.size() == v.size()
```

这也是判断一道题是不是“转换题”的重要特征：

> 每个输入都必须产生一个输出。

---

## 4. 空 `vector` 不需要特殊处理

例如：

```cpp
std::vector<int> numbers;
std::vector<int> result;

for (int value : numbers) {
    result.push_back(value * 2);
}
```

因为 `numbers` 是空的，所以循环执行：

```text
0 次
```

最终：

```cpp
result == {}
```

因此通常完全不需要写：

```cpp
if (numbers.empty()) {
    return {};
}
```

原来的循环自然就能正确处理空输入。

---

## 5. 不要把“转换”和“过滤”混在一起

这是非常重要的区别。

### 转换 Transformation

每个输入元素都产生一个输出：

```cpp
for (int value : numbers) {
    result.push_back(value * 2);
}
```

例如：

```text
{3, 0, -1}
    ↓
{6, 0, -2}
```

大小不变。

---

### 过滤 Filtering

只有满足条件的元素才进入结果：

```cpp
for (int value : numbers) {
    if (value != 0) {
        result.push_back(value);
    }
}
```

例如：

```text
{3, 0, -1}
    ↓
{3, -1}
```

大小可能变化。

所以可以简单记成：

```text
Transformation:
每个元素都 push_back

Filtering:
满足条件才 push_back
```

---

## 6. 常见错误：跳过某些元素

假设题目要求：

> 把每个元素加 10。

错误写法：

```cpp
for (int value : numbers) {
    if (value != 0) {
        result.push_back(value + 10);
    }
}
```

如果输入：

```cpp
{3, 0, -1}
```

结果变成：

```cpp
{13, 9}
```

但正确答案应该是：

```cpp
{13, 10, 9}
```

因为 `0` 也是一个合法输入元素，它同样需要产生对应的输出。

---

## 7. 常见错误：提前创建大小，再使用 `push_back`

错误写法：

```cpp
std::vector<int> result(numbers.size());

for (int value : numbers) {
    result.push_back(value * 2);
}
```

问题在于：

```cpp
std::vector<int> result(numbers.size());
```

已经创建了 `numbers.size()` 个元素。

对于 `int`，这些元素初始值通常是：

```cpp
0
```

例如：

```cpp
numbers = {1, 2, 3};
```

执行：

```cpp
std::vector<int> result(numbers.size());
```

之后实际上已经是：

```cpp
result = {0, 0, 0};
```

再执行三次：

```cpp
push_back(...)
```

最终会变成：

```cpp
{0, 0, 0, 2, 4, 6}
```

大小变成输入的两倍。

---

## 8. 使用 `push_back` 时，结果应该从空 `vector` 开始

推荐写法：

```cpp
std::vector<int> result;

for (int value : numbers) {
    result.push_back(value * 2);
}
```

也就是：

```text
空 result
    ↓
逐个计算
    ↓
逐个 push_back
```

如果你决定使用索引赋值：

```cpp
result[i] = ...
```

那么才应该提前创建大小：

```cpp
std::vector<int> result(numbers.size());

for (std::size_t i = 0; i < numbers.size(); ++i) {
    result[i] = numbers[i] * 2;
}
```

两种写法都可以，但不要混合：

```cpp
result(numbers.size())
```

和：

```cpp
push_back(...)
```

---

## 9. 为什么修改 `value` 不会修改原来的 `vector`

考虑：

```cpp
for (int value : numbers) {
    value *= 2;
}
```

这里：

```cpp
int value
```

意味着每次循环都会把元素复制一份给 `value`。

所以实际上类似：

```text
numbers 中的元素
       ↓ copy
     value
```

修改：

```cpp
value *= 2;
```

只是修改这个副本，并不会修改 `numbers`。

例如：

```cpp
std::vector<int> numbers = {1, 2, 3};

for (int value : numbers) {
    value *= 2;
}
```

循环结束后：

```cpp
numbers == {1, 2, 3}
```

仍然没有变化。

---

## 10. `const std::vector<int> &numbers` 的含义

题目中的参数：

```cpp
const std::vector<int> &numbers
```

可以拆成三个部分理解。

### `std::vector<int>`

参数是：

```cpp
vector<int>
```

也就是整数向量。

### `&`

表示使用引用：

```cpp
&
```

函数不需要复制整个 `vector`。

对于较大的 `vector`，这通常更加高效。

### `const`

表示函数不能通过这个引用修改输入：

```cpp
const
```

例如下面的代码不允许：

```cpp
numbers[0] = 100;
```

因此：

```cpp
const std::vector<int> &numbers
```

可以理解成：

> 只读取调用者传入的 `vector`，不复制它，也不修改它。

---

## 11. 转换题的思维模型

看到下面这样的要求：

```text
把每个数字乘 2
把每个数字加 10
把每个数字平方
把每个字符串变成它的长度
```

可以马上想到：

```cpp
std::vector<输出类型> result;

for (输入类型 value : input) {
    result.push_back(/* 对 value 进行转换 */);
}

return result;
```

核心只有一句：

> **每循环一次，就产生一个结果。**

---

## 12. 和前面几种 Vector 操作的区别

可以把常见的 `vector` 循环题归纳成下面几类：

| 类型  | 核心操作              |    输出数量 |
| --- | ----------------- | ------: |
| 计数  | `++count`         |    一个数字 |
| 累加  | `total += value`  |    一个数字 |
| 查找  | 找到后 `return`      | 一个位置/结果 |
| 最大值 | 更新 `best`         |    一个数字 |
| 过滤  | 条件成立才 `push_back` |   0 ~ n |
| 转换  | 每次都 `push_back`   |    恰好 n |

尤其需要记住：

```text
过滤：
if (...) {
    result.push_back(...);
}

转换：
result.push_back(...);
```

转换通常没有一个用来决定“要不要添加元素”的 `if`。
