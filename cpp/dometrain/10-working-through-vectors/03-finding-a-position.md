# C++ Vector：查找元素第一次出现的位置

## 1. 问题目标

有时我们查找一个元素，不只是想知道“有没有这个值”，而是想知道：

> **这个值第一次出现在哪个索引位置？**

例如：

```cpp
std::vector<int> v = {5, 9, 5};
```

如果查找 `9`，答案应该是：

```text
1
```

因为：

```text
索引:  0  1  2
元素:  5  9  5
```

---

## 2. 为什么这里适合使用索引 `for` 循环？

范围 `for`：

```cpp
for (int value : numbers) {
    ...
}
```

很适合处理：

> “每个元素是什么？”

但它默认不会告诉你：

> “这个元素位于哪个索引？”

而当前问题的答案本身就是一个索引，因此更适合使用普通索引循环：

```cpp
for (std::size_t i = 0; i < numbers.size(); ++i) {
    ...
}
```

这里：

```cpp
i
```

表示当前正在检查的索引。

```cpp
numbers[i]
```

表示这个索引对应的元素。

---

## 3. 基本查找写法

```cpp
for (std::size_t i = 0; i < numbers.size(); ++i) {
    if (numbers[i] == target) {
        return static_cast<int>(i);
    }
}
```

核心逻辑是：

```cpp
if (numbers[i] == target)
```

也就是：

> 如果当前位置的元素等于目标值，那么当前位置 `i` 就是答案。

---

## 4. 为什么直接在循环里面 `return`？

例如：

```cpp
std::vector<int> numbers = {5, 9, 3, 9};
```

查找：

```cpp
target = 9;
```

执行过程：

```text
i = 0 → numbers[0] = 5 → 不匹配
i = 1 → numbers[1] = 9 → 匹配
```

此时立即：

```cpp
return 1;
```

整个函数立刻结束。

后面的第二个 `9` 根本不会继续检查。

因此这种写法天然得到：

> **第一个匹配元素的索引。**

---

## 5. 为什么循环条件必须是 `i < numbers.size()`？

标准写法：

```cpp
for (std::size_t i = 0; i < numbers.size(); ++i)
```

假设：

```cpp
numbers.size() == 3;
```

合法索引只有：

```text
0
1
2
```

所以：

```cpp
i < numbers.size()
```

允许：

```text
0 < 3
1 < 3
2 < 3
```

但：

```text
3 < 3
```

为 `false`，循环结束。

因此永远不会执行：

```cpp
numbers[3]
```

这正是访问 `vector` 时最重要的边界保证。

不要写成：

```cpp
i <= numbers.size()
```

因为最后会尝试访问：

```cpp
numbers[numbers.size()]
```

这是越界访问。

---

## 6. 为什么索引使用 `std::size_t`？

`vector::size()` 返回的类型是：

```cpp
std::size_t
```

因此最好让循环变量也使用：

```cpp
std::size_t i
```

例如：

```cpp
for (std::size_t i = 0; i < numbers.size(); ++i)
```

这样：

```cpp
i
```

和：

```cpp
numbers.size()
```

类型一致。

---

## 7. 为什么返回时需要 `static_cast<int>`？

题目要求函数返回：

```cpp
int
```

但是：

```cpp
i
```

的类型是：

```cpp
std::size_t
```

因此可以显式转换：

```cpp
return static_cast<int>(i);
```

意思是：

> 把 `i` 转换成 `int` 再返回。

所以：

```cpp
std::size_t
```

用于安全地遍历 `vector`。

而：

```cpp
int
```

是这个函数规定的返回类型。

---

## 8. 找不到时为什么返回 `-1`？

循环结束以后：

```cpp
return -1;
```

意味着：

> 整个 vector 都检查完了，但是没有找到目标元素。

这里 `-1` 可以安全地表示“没有找到”，因为合法索引只能是：

```text
0, 1, 2, 3, ...
```

永远不会出现负数索引。

因此：

```text
>= 0 → 找到了，对应索引
-1   → 没找到
```

例如：

```cpp
std::vector<int> numbers = {5, 9, 5};
```

```cpp
firstIndexOf(numbers, 9);   // 1
firstIndexOf(numbers, 5);   // 0
firstIndexOf(numbers, 100); // -1
```

---

## 9. 为什么返回“位置”时可以用 `-1`，返回“元素”时不一定可以？

这是一个很重要的设计区别。

### 返回索引

如果函数返回：

```text
元素的位置
```

合法答案永远不会是负数。

因此：

```cpp
-1
```

可以明确表示：

```text
没有找到
```

### 返回元素本身

如果函数返回的是：

```text
找到的元素值
```

那么 `-1` 可能本身就是一个合法元素。

例如：

```cpp
std::vector<int> numbers = {4, -1, 8};
```

此时：

```cpp
return -1;
```

到底表示：

```text
找到了元素 -1
```

还是：

```text
没有找到
```

就无法判断。

所以使用特殊值表示失败之前，必须确认：

> **这个特殊值不可能和正常答案冲突。**

---

## 10. 空 Vector 不需要特殊处理

假设：

```cpp
std::vector<int> numbers;
```

此时：

```cpp
numbers.size() == 0
```

循环：

```cpp
for (std::size_t i = 0; i < numbers.size(); ++i)
```

第一次判断就是：

```cpp
0 < 0
```

结果为：

```cpp
false
```

因此循环体一次都不会执行，直接来到：

```cpp
return -1;
```

所以空 vector 会自然得到：

```text
没找到 → -1
```

不需要额外写：

```cpp
if (numbers.empty()) {
    return -1;
}
```

虽然这样写也没错，但属于没有必要的重复判断。

---

# 完整思维模型

对于“查找某个元素第一次出现的位置”，可以记成：

```text
从索引 0 开始
        ↓
索引是否还在 vector 范围内？
        ↓
检查 numbers[i]
        ↓
是否等于 target？
   ┌────┴────┐
   是        否
   ↓         ↓
返回 i      i++
             ↓
          继续查找
```

如果所有元素都检查完：

```text
返回 -1
```

---

# 通用模板

```cpp
for (std::size_t i = 0; i < container.size(); ++i) {
    if (container[i] == target) {
        return static_cast<int>(i);
    }
}

return -1;
```

这个模式可以概括为：

> **遍历所有合法索引 → 找到后立即返回 → 遍历结束仍没找到则返回失败标记。**

---

# 重点总结

* 查找“在哪里”时，索引循环通常比范围 `for` 更合适。
* `i` 表示当前位置，`numbers[i]` 表示当前位置的元素。
* 使用 `i < numbers.size()` 保证索引不会越界。
* 第一次匹配时立即 `return`，自然得到第一个匹配位置。
* `std::size_t` 与 `vector::size()` 的类型一致。
* 函数要求返回 `int` 时，可以使用 `static_cast<int>(i)`。
* `-1` 可以表示“没有找到”，因为合法 vector 索引不会是负数。
* 空 vector 不需要单独处理：循环自动跳过并返回 `-1`。
