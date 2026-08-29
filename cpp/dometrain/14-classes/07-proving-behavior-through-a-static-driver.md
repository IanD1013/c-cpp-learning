# C++ 自动评分器中的类测试：用静态 Driver 间接测试对象

## 1. 自动评分器能比较什么？

这一类自动评分系统通常只能直接比较一些简单类型，例如：

* 数字：`int`、`double` 等
* `bool`
* `char`
* `std::string`
* 上述类型的一维 `std::vector`

例如下面这些返回值通常可以直接评分：

```cpp
static int answer();
static bool valid();
static std::string name();
static std::vector<int> results();
```

但如果返回：

```cpp
MyClass
std::pair<int, int>
std::tuple<int, int>
std::vector<std::vector<int>>
```

代码本身可能仍然可以正常编译，但评分器不知道如何比较这些类型，因此测试会失败。

---

# 2. 为什么不能直接返回自己写的对象？

假设有：

```cpp
class Peak {
public:
    void offer(int value);
    int highest() const;
};
```

理想情况下可能想写：

```cpp
static Peak buildPeak(...);
```

然后让评分器检查 `Peak`。

但评分器不能理解自定义类，所以正确做法是：

> 在函数内部创建对象、操作对象，最后只返回评分器能够比较的简单数据。

这种函数可以看成一个 **driver（驱动函数）**。

例如：

```cpp
static int highestSeen(const std::vector<int>& values) {
    Peak peak;

    for (int value : values) {
        peak.offer(value);
    }

    return peak.highest();
}
```

流程是：

```text
输入数据
   ↓
创建对象
   ↓
调用对象的方法
   ↓
读取对象状态
   ↓
返回普通类型
   ↓
评分器比较
```

因此，虽然评分器不能直接测试一个对象，但可以通过它公开的方法来**间接验证这个类是否正确**。

---

# 3. 为什么 Driver 应该真正使用这个类？

不要绕过类自己计算答案，例如：

```cpp
static int highestSeen(const std::vector<int>& values) {
    int result = ...;   // 自己重新实现算法
    return result;
}
```

这样只能证明 Driver 算对了，并不能证明 `Peak` 类本身实现正确。

正确方式应该是：

```cpp
Peak peak;

for (...) {
    peak.offer(...);
}

return peak.highest();
```

也就是说：

> Driver 的作用是“操作被测试对象并读取状态”，而不是替这个对象完成工作。

有些评分器还会提供隐藏 Driver，直接创建你的类并调用它的方法。

因此即使你自己的 Driver 偷偷计算出了正确答案，只要类本身没有正确维护状态，隐藏测试仍然会失败。

---

# 4. Driver 返回什么，评分器就只能看到什么

这是自动测试设计中非常重要的一点：

> **返回值就是测试能够获得的全部证据。**

假设有一个类同时维护：

```cpp
current   // 当前连续长度
best      // 历史最长连续长度
```

如果 Driver 只返回：

```cpp
best
```

那么评分器只能验证 `best`。

即使 `current` 完全错误，也可能无法被发现。

所以如果题目要求两个状态都正确，Driver 最好把两个状态都返回：

```cpp
return {run.current(), run.best()};
```

例如对于：

```cpp
{1, 1, 1, 0, 2}
```

正确状态应该是：

```text
current = 1
best    = 3
```

如果只检查 `best`，某些错误实现可能蒙混过关。

---

# 5. `Streak` 类需要维护什么状态？

这一题中的 `Streak` 用来记录：

> 连续出现正数（`> 0`）的长度。

需要维护两个成员变量：

```cpp
int current_;
int best_;
```

含义分别是：

```text
current_
    当前这一段连续正数的长度

best_
    到目前为止出现过的最长连续正数长度
```

初始状态：

```text
current_ = 0
best_ = 0
```

---

# 6. `record()` 的状态更新规则

每次：

```cpp
run.record(reading);
```

只处理一个 reading。

## 情况 1：`reading > 0`

说明当前连续正数还在继续：

```cpp
++current_;
```

然后检查是否创造了新纪录：

```cpp
if (current_ > best_) {
    best_ = current_;
}
```

---

## 情况 2：`reading <= 0`

包括：

```text
0
负数
```

都会结束当前连续段：

```cpp
current_ = 0;
```

但：

```cpp
best_
```

不能清零，因为历史最长记录仍然存在。

---

# 7. 为什么 `best` 必须在每次正数出现时立即更新？

错误思路：

```cpp
for (...) {
    ...
}

best = std::max(best, current);
```

如果只在循环结束时更新，就可能丢掉之前已经结束的最长连续段。

例如：

```cpp
{4, 5, 0}
```

过程：

```text
4 → current = 1
5 → current = 2
0 → current = 0
```

如果最后才看：

```cpp
current == 0
```

就已经看不到之前的长度 `2` 了。

因此每当：

```cpp
current_
```

增长时，就应该立即：

```cpp
best_ = max(best_, current_);
```

这样 `best_` 才是真正的：

> 截止当前为止的历史最大值。

---

# 8. 一个重要的不变量

`Streak` 应始终满足：

```cpp
best_ >= current_
```

原因很简单：

* `current_` 表示当前连续长度
* `best_` 表示历史上见过的最大连续长度

因此当前连续长度不可能比“历史最大值”更大而不更新 `best_`。

例如：

```cpp
if (reading > 0) {
    ++current_;

    if (current_ > best_) {
        best_ = current_;
    }
}
```

正是在维护这个不变量。

---

# 9. `runLengths()` Driver

要求的函数签名：

```cpp
static std::vector<int> runLengths(
    const std::vector<int>& readings
)
```

函数应该：

1. 创建一个局部 `Streak`
2. 按顺序读取所有 reading
3. 每个 reading 恰好调用一次 `record()`
4. 最后返回：

```cpp
{
    run.current(),
    run.best()
}
```

注意顺序：

```text
第 0 个元素：current
第 1 个元素：best
```

---

# 10. 示例

## 示例 1

输入：

```cpp
{4, 5, 0}
```

状态变化：

| reading | current | best |
| ------: | ------: | ---: |
|      初始 |       0 |    0 |
|       4 |       1 |    1 |
|       5 |       2 |    2 |
|       0 |       0 |    2 |

返回：

```cpp
{0, 2}
```

---

## 示例 2

输入：

```cpp
{1, 1, 1, 0, 2}
```

状态变化：

```text
1 → current 1, best 1
1 → current 2, best 2
1 → current 3, best 3
0 → current 0, best 3
2 → current 1, best 3
```

返回：

```cpp
{1, 3}
```

---

## 示例 3：空输入

```cpp
{}
```

没有调用过 `record()`，所以：

```text
current = 0
best = 0
```

返回：

```cpp
{0, 0}
```

---

# 11. 为什么对象应该放在 Driver 的局部变量中？

应该写：

```cpp
static std::vector<int> runLengths(...) {
    Streak run;
    ...
}
```

而不是：

```cpp
Streak run;   // 全局对象
```

Driver 的每次执行应该拥有一个全新的对象状态：

```text
调用 Driver
→ 创建新 Streak
→ 处理输入
→ 返回结果
→ 对象销毁
```

这样测试之间不会通过对象状态产生不必要的耦合。

即使某些评分环境会为每个测试启动新的程序，也没有必要依赖这种行为。

---

# 12. 核心知识总结

这部分真正需要掌握的不是 `Streak` 这个具体算法，而是：

### ① 自定义类通常不能直接交给简单自动评分器比较

因此使用：

```text
Driver → 创建对象 → 调用方法 → 返回普通值
```

---

### ② 测试只能验证它能够观察到的状态

```text
没有返回 / 暴露的状态
≈ 自动测试无法直接验证的状态
```

因此 Driver 应该返回足够的信息来验证题目要求的行为。

---

### ③ Driver 不应该重新实现类本身的逻辑

应该：

```cpp
run.record(...)
run.current()
run.best()
```

而不是 Driver 自己计算连续长度。

---

### ④ 有状态类通常需要维护不变量

本题最重要的不变量之一：

```cpp
best_ >= current_
```

并且：

```text
reading > 0 → current 增长并更新 best
reading <= 0 → current 清零，best 保留
```

这种“输入一次 → 更新一次内部状态”的设计，是很多状态机、计数器和流式算法的基本模式。
