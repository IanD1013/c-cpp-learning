# C++ 自定义排序：Lambda 比较器与 `std::sort`

## 1. 为什么需要自定义比较器

`std::sort` 默认按从小到大排序：

```cpp
std::sort(v.begin(), v.end());
```

如果想按普通的从大到小排序，也可以直接使用现成的比较器，例如：

```cpp
std::sort(v.begin(), v.end(), std::greater<int>());
```

但如果排序规则比较特殊，例如：

> 奇数排在偶数前面；奇数内部从小到大；偶数内部也从小到大。

标准库没有一个现成的比较器直接表达这个规则，这时就可以自己写一个 **lambda 表达式**。

---

## 2. Lambda 表达式基础

一个最简单的比较器可以写成：

```cpp
auto comesBefore = [](int a, int b) {
    return a < b;
};
```

它基本等价于一个小函数：

```cpp
bool comesBefore(int a, int b) {
    return a < b;
}
```

但 lambda 可以直接定义在需要它的位置。

### 结构

```cpp
auto name = [](参数) {
    函数体
};
```

例如：

```cpp
auto comesBefore = [](int a, int b) {
    return a < b;
};
```

各部分含义：

```cpp
[]
```

是 **capture list（捕获列表）**。

这里为空：

```cpp
[]
```

表示 lambda 不使用外部作用域中的变量。

---

```cpp
(int a, int b)
```

是参数列表。

对于排序比较器，`std::sort` 会不断拿两个元素来询问：

> `a` 是否应该排在 `b` 前面？

---

```cpp
return a < b;
```

返回一个 `bool`：

* `true`：`a` 应该排在 `b` 前面
* `false`：不能说 `a` 应该排在 `b` 前面

---

整个 lambda 是一个对象，所以可以用 `auto` 保存：

```cpp
auto comesBefore = [](int a, int b) {
    return a < b;
};
```

之后既可以直接调用：

```cpp
comesBefore(3, 4);
```

也可以交给 `std::sort`：

```cpp
std::sort(v.begin(), v.end(), comesBefore);
```

---

# 3. `std::sort` 的比较器到底在问什么？

理解自定义排序最重要的一点是：

```cpp
compare(a, b)
```

不是在问：

> `a` 是否比 `b` 小？

而是在问：

> **按照你定义的排序规则，`a` 是否应该排在 `b` 前面？**

例如：

```cpp
auto compare = [](int a, int b) {
    return a > b;
};
```

那么：

```cpp
compare(5, 3)
```

返回：

```cpp
true
```

意思是：

```text
5 应该排在 3 前面
```

于是排序结果就是从大到小。

---

# 4. 自定义规则：奇数优先，内部升序

现在要求：

1. 奇数全部放在偶数前面
2. 奇数内部从小到大
3. 偶数内部从小到大

例如：

```cpp
{4, 7, 2, 9, 6, 1}
```

排序后：

```cpp
{1, 7, 9, 2, 4, 6}
```

可以把比较过程分成两种情况。

---

## 情况 1：一个奇数，一个偶数

如果：

```cpp
a = 7
b = 4
```

因为奇数应该排前面：

```cpp
compare(7, 4) == true
```

反过来：

```cpp
compare(4, 7) == false
```

所以可以先判断奇偶性。

---

## 情况 2：两者同为奇数或同为偶数

例如：

```cpp
a = 7
b = 3
```

两者都是奇数。

这时候按照普通升序：

```cpp
return a < b;
```

所以：

```cpp
compare(3, 7) == true
compare(7, 3) == false
```

偶数同理。

---

# 5. 正确判断奇数

一个整数是奇数，可以写：

```cpp
value % 2 != 0
```

例如：

```cpp
3 % 2   // 1
5 % 2   // 1

-3 % 2  // -1
-5 % 2  // -1
```

因此不要写：

```cpp
value % 2 == 1
```

因为负奇数：

```cpp
-3 % 2
```

结果是：

```cpp
-1
```

所以：

```cpp
-3 % 2 == 1
```

是 `false`。

正确写法：

```cpp
value % 2 != 0
```

这样正奇数和负奇数都能正确识别。

---

# 6. 实现奇数优先比较器

可以先记录：

```cpp
bool aOdd = a % 2 != 0;
bool bOdd = b % 2 != 0;
```

然后判断。

如果一个是奇数，一个是偶数：

```cpp
if (aOdd != bOdd) {
    return aOdd;
}
```

为什么直接：

```cpp
return aOdd;
```

就可以？

因为进入这个 `if` 时，两者奇偶性一定不同。

如果：

```cpp
aOdd == true
bOdd == false
```

说明 `a` 是奇数，因此应该排前面：

```cpp
return true;
```

如果：

```cpp
aOdd == false
bOdd == true
```

说明 `a` 是偶数，因此不应该排前面：

```cpp
return false;
```

于是：

```cpp
return aOdd;
```

刚好表达整个逻辑。

完整比较器：

```cpp
auto comesBefore = [](int a, int b) {
    bool aOdd = a % 2 != 0;
    bool bOdd = b % 2 != 0;

    if (aOdd != bOdd) {
        return aOdd;
    }

    return a < b;
};
```

---

# 7. 比较器必须是 Strict Comparison

`std::sort` 要求比较器形成一种严格排序关系。

最重要的一条就是：

> 一个值不能排在它自己前面。

也就是说必须保证：

```cpp
compare(a, a) == false
```

例如：

```cpp
compare(4, 4)
```

必须返回：

```cpp
false
```

---

## 为什么应该使用 `<`，不能使用 `<=`

正确：

```cpp
return a < b;
```

错误：

```cpp
return a <= b;
```

因为：

```cpp
4 <= 4
```

是：

```cpp
true
```

于是比较器相当于告诉 `std::sort`：

```text
4 应该排在 4 前面
```

这在逻辑上是矛盾的。

因此排序比较器通常应该具有这种性质：

```cpp
compare(a, a) == false
```

这也是为什么排序器通常使用：

```cpp
<
```

而不是：

```cpp
<=
```

---

# 8. 为什么排序结果看起来正确也不能证明比较器正确

假设错误地写：

```cpp
return a <= b;
```

有时候运行结果看起来仍然像正常排序：

```cpp
{1, 2, 3, 4}
```

但这并不代表比较器合法。

`std::sort` 假定你的比较器满足严格排序规则。

如果这个前提被破坏，程序行为就不再有可靠保证。

所以检查比较器时，可以重点测试：

```cpp
compare(x, x)
```

是否为：

```cpp
false
```

例如：

```cpp
ordersBefore(4, 4)
```

应该返回：

```cpp
false
```

---

# 9. 用 `std::sort` 排序

假设已经有：

```cpp
auto comesBefore = [](int a, int b) {
    bool aOdd = a % 2 != 0;
    bool bOdd = b % 2 != 0;

    if (aOdd != bOdd) {
        return aOdd;
    }

    return a < b;
};
```

就可以：

```cpp
std::sort(result.begin(), result.end(), comesBefore);
```

例如：

```cpp
std::vector<int> result = {4, 7, 2, 9, 6, 1};

std::sort(result.begin(), result.end(), comesBefore);
```

结果：

```cpp
{1, 7, 9, 2, 4, 6}
```

---

# 10. 不修改原始 `const vector`

如果函数参数是：

```cpp
const std::vector<int> &values
```

这里的 `const` 表示不能修改：

```cpp
values
```

因此不能直接：

```cpp
std::sort(values.begin(), values.end(), comesBefore);
```

应该先复制：

```cpp
std::vector<int> result = values;
```

然后排序副本：

```cpp
std::sort(result.begin(), result.end(), comesBefore);
```

最后：

```cpp
return result;
```

这样：

```text
values   → 保持不变
result   → 排序后的副本
```

---

# 11. 本题最值得记住的模式

自定义排序通常可以按下面的思路写：

```cpp
auto compare = [](int a, int b) {

    // 1. 先处理最高优先级规则

    // 2. 如果最高优先级相同，
    //    再处理第二优先级规则

    return ...;
};
```

例如：

```cpp
auto comesBefore = [](int a, int b) {
    bool aOdd = a % 2 != 0;
    bool bOdd = b % 2 != 0;

    // 第一优先级：奇数在偶数之前
    if (aOdd != bOdd) {
        return aOdd;
    }

    // 第二优先级：数值从小到大
    return a < b;
};
```

这其实就是多条件排序的基本思想：

```text
先比较第一排序条件
    ↓
如果无法决定先后
    ↓
再比较第二排序条件
    ↓
如果仍然无法决定
    ↓
继续比较后续条件
```

这个模式以后处理：

* 按年龄再按姓名排序
* 按分数再按学号排序
* 按类别再按价格排序
* 按奇偶再按数值排序

都非常常见。
