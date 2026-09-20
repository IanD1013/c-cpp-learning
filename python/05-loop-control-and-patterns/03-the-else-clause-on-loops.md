### 循环中的 `else` 子句

Python 拥有一个让即使来自其他语言的资深程序员也感到惊讶的特性：你可以为 `for` 或 `while` 循环附加一个 `else` 块。`else` 块**仅在循环正常完成时**运行——这意味着它从未被 `break` 语句中断。如果循环因 `break` 而提前退出，`else` 块将被完全跳过。

这在集合中查找特定内容的“搜索”模式下非常有用。

### 工作原理

可以将循环上的 `else` 理解为：**“如果循环没有 break。”**

* 循环正常遍历所有项 → `else` 块**运行**
* 循环遇到 `break` 并提前退出 → `else` 块被**跳过**
* 循环体从未执行（例如空 range）→ `else` 块**运行**（因为没有发生 `break`）

这消除了在大多数其他语言中需要使用的笨拙的“found”标志变量。

### 语法

```python
for item in collection:
    if some_condition:
        break
else:
    # This runs only if the loop finished without hitting break
```

```python
while condition:
    if some_condition:
        break
else:
    # This runs only if the loop finished without hitting break
```

### 示例

```python
# Example 1: Searching for a negative number in a list
def has_negative(numbers):
    for num in numbers:
        if num < 0:
            print("Found a negative number!")
            break
    else:
        print("No negative numbers found.")

has_negative([3, 5, 8])    # prints: No negative numbers found.
has_negative([3, -1, 8])   # prints: Found a negative number!
```

```python
# Example 2: Checking if a username is available
def check_username(desired, taken_names):
    for name in taken_names:
        if name == desired:
            return "Username is taken"
    else:
        return "Username is available"

check_username("alice", ["bob", "carol"])   # returns: "Username is available"
check_username("bob", ["bob", "carol"])     # returns: "Username is taken"
```

```python
# Example 3: Without for-else, you'd need a flag variable
def check_username_old_way(desired, taken_names):
    found = False
    for name in taken_names:
        if name == desired:
            found = True
            break
    if found:
        return "Username is taken"
    else:
        return "Username is available"
# The for-else version above is cleaner!
```

### 常见模式

`for...else` 模式在验证和搜索任务中大放异彩：

* **搜索**集合中的匹配项 —— 找到时 `break`，`else` 处理“未找到”的情况
* **验证**所有项是否满足某个条件 —— 首次失败时 `break`，`else` 确认全部通过
* **检查整除性** —— 遍历可能的除数，如果能整除则 `break`，`else` 表示没有除数能整除

### 实用参考

| **概念**        | **描述**                                        |
| ------------- | --------------------------------------------- |
| `range(a, b)` | 生成从 `a` 开始到（不包括）`b` 为止的整数                     |
| `n % i == 0`  | 如果 `n` 能被 `i` 整除则为 True                       |
| `break`       | 立即退出循环，跳过 `else` 块                            |
| 空 range       | `range(2, 2)` 不生成任何值 —— 循环体从未运行，但 `else` 仍会运行 |
