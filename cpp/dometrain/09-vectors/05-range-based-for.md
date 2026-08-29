# C++ `range-based for`：范围 `for` 循环

## 1. 基本语法

如果只是想**依次访问容器中的每个元素**，C++ 可以使用范围 `for` 循环（range-based for loop）：

```cpp
std::vector<double> v = {1.5, 2.25, 0.75};

for (double value : v) {
    std::cout << value << " ";
}
```

输出：

```text
1.5 2.25 0.75
```

基本结构：

```cpp
for (元素类型 变量名 : 容器) {
    // 使用当前元素
}
```

例如：

```cpp
for (double value : v)
```

可以理解为：

> 对 `v` 中的每一个元素，依次把它放到 `value` 中，然后执行循环体。

---

## 2. 为什么使用范围 `for`

传统索引循环：

```cpp
for (std::size_t i = 0; i < v.size(); ++i) {
    std::cout << v[i] << " ";
}
```

范围 `for`：

```cpp
for (double value : v) {
    std::cout << value << " ";
}
```

如果你**不需要元素的位置（index）**，范围 `for` 通常更简单。

它不需要自己处理：

```cpp
i = 0
i < v.size()
++i
v[i]
```

因此也减少了：

* 写错循环边界
* 越界访问
* 忘记递增索引

这类问题。

---

## 3. `double value`：复制元素

```cpp
for (double value : v)
```

这里的 `value` 是当前元素的**副本**。

例如：

```cpp
std::vector<double> v = {1.0, 2.0, 3.0};

for (double value : v) {
    value = 100;
}
```

循环结束后：

```cpp
v == {1.0, 2.0, 3.0}
```

因为修改的是：

```text
vector 中的元素
      ↓ copy
    value
```

而不是原来的元素。

对于 `int`、`double` 等很小的基本类型，这种复制成本通常可以忽略。

---

## 4. `const auto &value`：只读引用

如果元素比较大，不希望每次循环都复制，可以写：

```cpp
for (const auto &value : v) {
    std::cout << value << " ";
}
```

这里：

```cpp
&
```

表示 `value` 是元素的**引用**，不会复制。

```cpp
const
```

表示不能通过 `value` 修改这个元素。

因此：

```cpp
for (const auto &value : v)
```

可以理解为：

> 直接读取容器里的元素，不复制，也不允许修改。

这是一种非常常见的只读遍历写法。

---

## 5. 三种常见形式

### 复制元素

```cpp
for (auto value : values) {
}
```

适合：

* `int`
* `double`
* `char`
* 其他复制成本很低的类型

---

### 只读引用

```cpp
for (const auto &value : values) {
}
```

适合：

* `std::string`
* 大型对象
* 不希望发生复制
* 只需要读取元素

---

### 可修改引用

```cpp
for (auto &value : values) {
    value *= 2;
}
```

这里直接修改容器中的元素。

例如：

```cpp
std::vector<int> values = {1, 2, 3};

for (auto &value : values) {
    value *= 2;
}
```

结果：

```cpp
{2, 4, 6}
```

---

## 6. 什么时候仍然需要索引循环

范围 `for` 很适合：

```text
“对每个元素做某件事”
```

但如果你需要知道：

```text
当前元素是第几个？
```

索引循环通常更合适：

```cpp
for (std::size_t i = 0; i < values.size(); ++i) {
    std::cout << i << ": " << values[i] << '\n';
}
```

因此可以记住：

```text
只关心元素
    ↓
range-based for

关心元素的位置
    ↓
index-based for
```

---

# 核心记忆

```cpp
for (double value : values)
```

表示：

> 依次取得 `values` 中的每个元素，并把它复制给 `value`。

```cpp
for (const auto &value : values)
```

表示：

> 依次直接引用每个元素，只读取，不复制、不修改。

```cpp
for (auto &value : values)
```

表示：

> 依次直接引用每个元素，并允许修改原始元素。
