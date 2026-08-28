# C++ 循环控制：`continue` 与 `break`

在 C++ 的循环内部，有两个非常重要的控制语句：

* `continue;`：结束**当前这一轮循环**，继续下一轮。
* `break;`：直接结束**整个循环**。

它们看起来很像，但作用完全不同。

---

## 1. `continue`：跳过当前这一轮

例如：

```cpp
for (int n = 1; n <= 5; ++n) {
    if (n == 2) {
        continue;
    }

    std::cout << n << "\n";
}
```

输出：

```text
1
3
4
5
```

当：

```cpp
n == 2
```

时执行：

```cpp
continue;
```

意味着：

> 当前这一轮后面的代码全部跳过，直接进入下一轮循环。

所以：

```cpp
std::cout << n << "\n";
```

不会在 `n == 2` 时执行。

---

## 2. `for` 循环中执行 `continue` 后会发生什么？

考虑：

```cpp
for (int n = 1; n <= 5; ++n)
```

它本质上的执行顺序是：

```text
初始化
↓
检查条件
↓
执行循环体
↓
执行更新语句 ++n
↓
再次检查条件
```

因此，在 `for` 循环里面遇到：

```cpp
continue;
```

以后，并不是直接重新执行循环体。

而是：

```text
continue
↓
执行 ++n
↓
检查 n <= 5
↓
如果成立，开始下一轮
```

例如：

```cpp
for (int n = 1; n <= 5; ++n) {
    if (n == 2) {
        continue;
    }

    std::cout << n << "\n";
}
```

执行到 `n == 2` 时：

```text
n = 2
↓
continue
↓
跳过 cout
↓
执行 ++n
↓
n = 3
↓
继续下一轮
```

所以循环不会因为 `continue` 而卡住。

---

# 3. `break`：直接结束整个循环

例如：

```cpp
for (int n = 1; n <= 5; ++n) {
    if (n == 4) {
        break;
    }

    std::cout << n << "\n";
}
```

输出：

```text
1
2
3
```

当：

```cpp
n == 4
```

时执行：

```cpp
break;
```

意味着：

> 整个循环立即结束。

不会再：

* 执行当前循环体后面的代码
* 执行 `++n`
* 检查下一轮条件
* 执行 `n == 5` 的那一轮

程序会直接跳到循环后面的代码。

---

# 4. `continue` 和 `break` 的区别

可以把一个循环想象成不断重复处理任务。

## `continue`

意思类似：

> “这个任务不用继续处理了，去看下一个。”

例如：

```cpp
if (这个数据不符合要求) {
    continue;
}
```

程序仍然继续循环。

---

## `break`

意思类似：

> “已经找到答案了，不需要继续找了。”

例如：

```cpp
if (已经找到目标) {
    break;
}
```

整个循环结束。

---

## 对比

| 语句         | 当前这一轮 | 后续循环 |
| ---------- | ----- | ---- |
| `continue` | 立即结束  | 继续   |
| `break`    | 立即结束  | 全部取消 |

可以简单记成：

```text
continue = 下一轮
break    = 不循环了
```

---

# 5. 综合示例

```cpp
for (int n = 1; n <= 5; ++n) {
    if (n == 2) {
        continue;
    }

    if (n == 4) {
        break;
    }

    std::cout << n << "\n";
}
```

输出：

```text
1
3
```

逐轮分析：

| `n` | 发生什么       | 是否打印      |
| --: | ---------- | --------- |
|   1 | 正常执行       | `1`       |
|   2 | `continue` | 不打印       |
|   3 | 正常执行       | `3`       |
|   4 | `break`    | 不打印，并结束循环 |
|   5 | 永远不会执行     | —         |

因此最终输出只有：

```text
1
3
```

---

# 6. 一个很常见的 `continue` 使用模式：过滤数据

假设只处理偶数：

```cpp
for (int value = 1; value <= 10; ++value) {
    if (value % 2 != 0) {
        continue;
    }

    std::cout << value << "\n";
}
```

这里：

```cpp
value % 2 != 0
```

表示：

> `value` 不能被 2 整除，也就是奇数。

于是：

```cpp
continue;
```

把所有奇数过滤掉。

只有偶数能够继续执行下面的：

```cpp
std::cout << value << "\n";
```

这种写法非常常见：

```cpp
if (不符合条件) {
    continue;
}

// 从这里开始，只处理符合条件的数据
```

相比于把大量代码全部包在：

```cpp
if (符合条件) {
    ...
}
```

里面，通常更加清晰。

---

# 7. 一个很常见的 `break` 使用模式：找到答案后立即停止

例如寻找第一个能被 7 整除的数：

```cpp
for (int value = 1; value <= 100; ++value) {
    if (value % 7 == 0) {
        std::cout << value;
        break;
    }
}
```

第一次满足条件的是：

```text
7
```

既然已经找到第一个答案，就没有必要继续检查：

```text
8
9
10
...
100
```

所以使用：

```cpp
break;
```

可以直接结束循环。

---

# 8. `continue` + `break` 的典型组合

很多搜索问题都有这样的结构：

```cpp
for (...) {

    if (当前数据不是候选对象) {
        continue;
    }

    // 当前数据确实是候选对象
    进行计数或处理;

    if (当前数据就是最终答案) {
        break;
    }
}
```

也就是：

```text
不是我要的
→ continue
→ 跳过

可能是我要的
→ 处理

已经确定是答案
→ break
→ 结束
```

这正是本题 `multiplesUntilShared` 使用的模式。

---

# 9. `%`：判断是否整除

本题还需要使用取余运算符：

```cpp
%
```

例如：

```cpp
value % 3
```

表示：

> `value` 除以 3 后的余数。

如果：

```cpp
value % 3 == 0
```

说明：

> `value` 可以被 3 整除。

例如：

```cpp
12 % 3 == 0
12 % 4 == 0
```

所以 12 同时是：

* 3 的倍数
* 4 的倍数

---

# 10. 本题涉及的核心逻辑

函数：

```cpp
static int multiplesUntilShared(
    int firstFactor,
    int secondFactor,
    int limit
)
```

要从：

```text
1
```

一直检查到：

```text
limit
```

但真正需要“计数”的只有：

```text
firstFactor 的倍数
```

例如：

```cpp
firstFactor = 3
```

需要关注：

```text
3
6
9
12
15
...
```

而不是：

```text
1
2
3
4
5
6
...
```

因此遇到一个不是 `firstFactor` 倍数的值时：

```cpp
if (value % firstFactor != 0) {
    continue;
}
```

意思是：

> 这个数字连第一个条件都不满足，不用处理它。

---

# 11. `checked` 统计的不是“检查过多少个整数”

这是本题最容易理解错的地方。

假设：

```cpp
multiplesUntilShared(2, 5, 100)
```

需要寻找同时能被：

```text
2
5
```

整除的数。

第一个是：

```text
10
```

但是答案不是：

```text
10
```

而是：

```text
5
```

因为 `checked` 统计的是：

> 已经遇到了多少个 `firstFactor` 的倍数。

2 的倍数依次是：

```text
2   → 第 1 个
4   → 第 2 个
6   → 第 3 个
8   → 第 4 个
10  → 第 5 个
```

所以答案是：

```text
5
```

---

# 12. 为什么计数必须发生在第二次判断之前？

正确的逻辑应该是：

```cpp
if (value % firstFactor != 0) {
    continue;
}

++checked;

if (value % secondFactor == 0) {
    ...
}
```

原因是：

如果当前数字同时满足两个条件，它自己也属于：

```text
firstFactor 的倍数
```

因此必须把它算进去。

例如：

```cpp
multiplesUntilShared(7, 7, 7)
```

第一个候选值就是：

```text
7
```

它既是 7 的倍数，同时也是两个因子的共同倍数。

所以：

```text
7 → 第 1 个 7 的倍数
```

答案应该是：

```text
1
```

如果先判断共同倍数再增加 `checked`：

```cpp
if (value % secondFactor == 0) {
    ...
}

++checked;
```

就有可能得到错误答案 `0`。

---

# 13. 输入合法性检查

题目规定：

如果：

```text
firstFactor <= 0
```

或者：

```text
secondFactor <= 0
```

或者：

```text
limit <= 0
```

应该返回：

```cpp
-1
```

因此一般在循环之前使用 guard clause：

```cpp
if (firstFactor <= 0 ||
    secondFactor <= 0 ||
    limit <= 0) {
    return -1;
}
```

这种写法称为：

```text
guard clause
```

即：

> 如果输入本身无效，尽早返回，不让错误输入进入后面的主要逻辑。

---

# 14. 本节最重要的知识点

记住下面几个模式即可。

### 跳过不需要的数据

```cpp
if (不符合条件) {
    continue;
}
```

---

### 找到答案后停止搜索

```cpp
if (找到答案) {
    break;
}
```

---

### 判断整除

```cpp
value % factor == 0
```

表示：

```text
value 是 factor 的倍数
```

---

### 判断不整除

```cpp
value % factor != 0
```

表示：

```text
value 不是 factor 的倍数
```

---

### 常见搜索循环结构

```cpp
for (...) {
    if (不是候选对象) {
        continue;
    }

    处理候选对象;

    if (找到最终答案) {
        break;
    }
}
```

一句话总结：

> `continue` 用来过滤，`break` 用来结束搜索。
