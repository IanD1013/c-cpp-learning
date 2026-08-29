# C++ `std::vector`：空 vector、`push_back()` 与动态构造元素

## 1. 声明一个空的 `std::vector`

如果 vector 中的元素需要在程序运行过程中逐个计算出来，通常可以先创建一个**空 vector**：

```cpp
std::vector<int> squares;
```

此时：

```cpp
squares.size() == 0
```

也就是说：

```text
squares
[]
```

这个 vector 已经是一个完整、可以正常使用的对象，只是暂时没有元素。

随后可以使用 `push_back()` 添加元素：

```cpp
squares.push_back(1);
squares.push_back(4);
```

现在：

```text
squares
[1, 4]
```

并且：

```cpp
squares.size() == 2
```

---

## 2. 不要混淆空 vector 和指定大小的 vector

下面两种写法含义完全不同。

### 空 vector

```cpp
std::vector<int> squares;
```

结果：

```text
[]
```

大小：

```cpp
squares.size() == 0
```

### 创建 4 个元素

```cpp
std::vector<int> squares(4);
```

这里的 `4` 表示：

> 创建一个包含 4 个 `int` 元素的 vector。

由于 `int` 会被值初始化，因此这 4 个元素都是 `0`：

```text
[0, 0, 0, 0]
```

大小：

```cpp
squares.size() == 4
```

如果随后执行：

```cpp
squares.push_back(1);
```

结果不是：

```text
[1]
```

而是：

```text
[0, 0, 0, 0, 1]
```

因为 `push_back()` 是在**已有元素后面追加元素**。

---

## 3. `push_back()` 是什么

`push_back()` 的作用是：

> 在 vector 的末尾增加一个元素。

例如：

```cpp
std::vector<int> numbers;

numbers.push_back(10);
numbers.push_back(20);
numbers.push_back(30);
```

最终：

```text
[10, 20, 30]
```

每调用一次：

```cpp
push_back(...)
```

vector 的 `size()` 都会增加 1。

---

## 4. `push_back()` 不需要指定位置

例如：

```cpp
numbers.push_back(20);
```

这里的 `20` 表示：

```text
要添加的值
```

而不是：

```text
添加的位置
```

`push_back()` 永远把元素放在**最后面**。

因此：

```cpp
numbers.push_back(4);
```

可以理解为：

> 把数字 `4` 加到 vector 的末尾。

---

## 5. 不需要提前给 `push_back()` 留空间

例如：

```cpp
std::vector<int> numbers;

numbers.push_back(1);
numbers.push_back(2);
numbers.push_back(3);
```

完全合法。

你不需要先写：

```cpp
std::vector<int> numbers(3);
```

事实上，如果写成：

```cpp
std::vector<int> numbers(3);
```

vector 已经存在三个元素：

```text
[0, 0, 0]
```

然后再 `push_back()`：

```cpp
numbers.push_back(1);
```

就会得到：

```text
[0, 0, 0, 1]
```

这通常不是你想要的结果。

---

# 用 `push_back()` 动态生成结果

假设有这样一个函数：

```cpp
static std::vector<int> multiplesOf(int step, int count)
```

它需要返回：

```text
step × 1
step × 2
step × 3
...
step × count
```

例如：

```cpp
multiplesOf(4, 3)
```

应该得到：

```text
[4, 8, 12]
```

因为：

```text
4 × 1 = 4
4 × 2 = 8
4 × 3 = 12
```

实现这类函数时，可以先创建空 vector：

```cpp
std::vector<int> result;
```

然后让计数器 `i` 从：

```text
1
```

一直运行到：

```text
count
```

每次计算：

```cpp
step * i
```

再使用：

```cpp
result.push_back(...)
```

加入结果。

最后返回：

```cpp
return result;
```

---

# 边界情况

这个函数有一些边界情况，其实可以直接由循环条件自然处理。

## `count == 0`

假设循环：

```cpp
for (int i = 1; i <= count; ++i)
```

如果：

```cpp
count == 0
```

第一次判断就是：

```cpp
1 <= 0
```

结果为 `false`。

因此循环一次也不会执行。

结果：

```text
[]
```

所以通常不需要额外写：

```cpp
if (count == 0)
```

---

## `count < 0`

例如：

```cpp
count == -3
```

第一次判断：

```cpp
1 <= -3
```

同样为 `false`。

所以结果仍然是：

```text
[]
```

也不需要额外处理。

---

## `step == 0`

例如：

```cpp
multiplesOf(0, 3)
```

计算过程：

```text
0 × 1 = 0
0 × 2 = 0
0 × 3 = 0
```

结果：

```text
[0, 0, 0]
```

---

## `step < 0`

例如：

```cpp
multiplesOf(-4, 3)
```

计算：

```text
-4 × 1 = -4
-4 × 2 = -8
-4 × 3 = -12
```

结果：

```text
[-4, -8, -12]
```

---

# 核心记忆

```cpp
std::vector<int> v;
```

表示：

```text
创建一个空 vector
```

而：

```cpp
std::vector<int> v(4);
```

表示：

```text
创建 4 个 int 元素
```

对于 `int`：

```text
[0, 0, 0, 0]
```

而：

```cpp
v.push_back(x);
```

表示：

```text
把 x 添加到 vector 末尾
```

所以当元素需要通过循环动态计算时，最常见的模式就是：

```cpp
std::vector<int> result;

for (...) {
    result.push_back(...);
}

return result;
```
