# C++ Vector 遍历：计数器（Counter）与累加器（Running Total）

在遍历 `std::vector` 时，**计数（count）** 和 **求和（sum）** 的代码看起来很像，但它们回答的是两个完全不同的问题。

---

## 1. 计数：有多少个元素满足条件？

例如：

```cpp
std::vector<int> v = {4, 7, 4, 2};

int matches = 0;

for (std::size_t i = 0; i < v.size(); ++i) {
    if (v[i] == 4) {
        ++matches;
    }
}
```

最终：

```cpp
matches == 2
```

因为 vector 中一共有两个元素等于 `4`。

关键代码是：

```cpp
++matches;
```

它表示：

> 每发现一个满足条件的元素，就让计数器增加 1。

因此，元素本身是多少并不重要。

比如：

```cpp
{1000, 1000, 3}
```

如果条件是：

```cpp
v[i] == 1000
```

那么结果仍然只是：

```cpp
2
```

因为满足条件的元素有两个。

---

## 2. 求和：满足条件的元素加起来是多少？

如果把：

```cpp
++matches;
```

改成：

```cpp
matches += v[i];
```

那么变量就不再表示“数量”，而表示“总和”。

例如：

```cpp
std::vector<int> v = {4, 7, 4, 2};

int total = 0;

for (std::size_t i = 0; i < v.size(); ++i) {
    if (v[i] == 4) {
        total += v[i];
    }
}
```

最终：

```cpp
total == 8
```

因为：

```text
4 + 4 = 8
```

所以要注意：

```cpp
++count;
```

表示：

> 满足条件的元素数量 +1

而：

```cpp
total += v[i];
```

表示：

> 把满足条件的元素值加入总和

---

## 3. 为什么计数器要写在循环外？

正确：

```cpp
int count = 0;

for (...) {
    if (...) {
        ++count;
    }
}
```

这样 `count` 会保存前面所有循环留下来的结果。

错误思路：

```cpp
for (...) {
    int count = 0;

    if (...) {
        ++count;
    }
}
```

这里的 `count` 每次循环都会重新创建，并重新变成：

```cpp
0
```

所以前一次循环得到的结果会全部丢失。

### 核心原则

需要跨多次循环累计结果的变量，通常应该声明在循环外：

```cpp
int count = 0;
int sum = 0;

for (...) {
    // 更新 count / sum
}
```

---

## 4. 为什么从 0 开始？

计数器应该初始化为：

```cpp
int count = 0;
```

因为在还没有检查任何元素之前：

> 已经找到的匹配元素数量就是 0。

这样也能自然处理特殊情况。

### 情况 1：没有任何元素满足条件

例如：

```cpp
{1, 2, 3}
```

如果统计等于 `100` 的元素：

```cpp
count == 0
```

### 情况 2：vector 是空的

```cpp
std::vector<int> v;
```

此时：

```cpp
v.size() == 0
```

循环条件：

```cpp
i < v.size()
```

一开始就是 false，因此循环一次也不会运行。

最终：

```cpp
count == 0
```

不需要额外写：

```cpp
if (v.empty()) {
    return 0;
}
```

正常的循环逻辑已经能够处理空 vector。

---

## 5. Vector 遍历时的边界

标准索引循环：

```cpp
for (std::size_t i = 0; i < v.size(); ++i) {
    // 使用 v[i]
}
```

这里：

```cpp
i < v.size()
```

非常重要。

如果：

```cpp
v.size() == 4
```

合法索引只有：

```text
0 1 2 3
```

最后一次循环：

```cpp
i == 3
```

然后：

```cpp
i == 4
```

此时：

```cpp
4 < 4
```

为 false，循环停止。

因此不会访问：

```cpp
v[4]
```

这种越界位置。

---

## 6. 一个通用的“条件计数”模板

很多题目都可以写成：

```cpp
int count = 0;

for (std::size_t i = 0; i < numbers.size(); ++i) {
    if (/* numbers[i] 满足某个条件 */) {
        ++count;
    }
}

return count;
```

例如：

### 数正数

```cpp
if (numbers[i] > 0) {
    ++count;
}
```

### 数偶数

```cpp
if (numbers[i] % 2 == 0) {
    ++count;
}
```

### 数大于某个 limit 的元素

```cpp
if (numbers[i] > limit) {
    ++count;
}
```

---

# 总结

条件计数题最重要的模式是：

```cpp
int count = 0;

for (...) {
    if (...) {
        ++count;
    }
}

return count;
```

记住：

* `count` 表示“有几个”
* `sum` 表示“加起来是多少”
* 计数使用 `++count`
* 求和通常使用 `sum += value`
* 累计变量应该声明在循环外
* 初始值通常是 `0`
* `i < vector.size()` 保证索引不会越界
* 空 vector 会自然得到计数 `0`
