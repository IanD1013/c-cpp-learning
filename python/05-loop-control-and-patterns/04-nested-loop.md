### 嵌套循环

嵌套循环是指放置在另一个循环体内部的循环。外层循环的**每一次迭代**，内层循环都会完整地执行其所有迭代。每当需要处理网格、坐标对等二维结构，或者需要组合来自两个序列的元素时，这种模式都是必不可少的。

### 工作原理

当 Python 遇到外层循环时，它会开始第一次迭代。在该次迭代中，它会遇到内层循环并将其完整运行完毕。接着外层循环进入下一次迭代，内层循环重新从头开始执行。如果外层循环运行 `m` 次，内层循环运行 `n` 次，那么内层循环内部的代码总共会执行 `m × n` 次。

可以把它想象成一个时钟：时针（外层循环）每走一格，分针（内层循环）就会完整转动一圈。

### 语法

```python
for outer_item in outer_sequence:
    # code that runs once per outer iteration
    for inner_item in inner_sequence:
        # code that runs m * n times total
    # back to outer loop body
```

### 示例

```python
# Example 1: Print all coordinate pairs on a 2x3 grid
for row in range(2):
    for col in range(3):
        print(f"({row}, {col})", end="  ")
    print()  # newline after each row
# Output:
# (0, 0)  (0, 1)  (0, 2)
# (1, 0)  (1, 1)  (1, 2)

# Example 2: Build a string of all two-letter combos from "AB" and "XY"
result = ""
for first in "AB":
    for second in "XY":
        result += first + second + " "
# result is "AX AY BX BY "

# Example 3: Accumulator pattern — build rows, then join them
colors = ["red", "blue"]
sizes = ["S", "M", "L"]
lines = []
for color in colors:
    items = []
    for size in sizes:
        items.append(f"{color}-{size}")
    lines.append(", ".join(items))
output = "\n".join(lines)
# output is:
# "red-S, red-M, red-L\nblue-S, blue-M, blue-L"
```

### 用于构建字符串的累加器模式

当你需要构建结构化字符串（例如表格）时，一种常见的方法是：

1. 为**行**创建一个空列表。
2. 在外层循环中，为当前行的**列**创建一个空列表。
3. 在内层循环中，计算每个值并将其追加到列列表中。
4. 内层循环结束后，使用列分隔符拼接这些列，并追加到行列表中。
5. 外层循环结束后，使用行分隔符拼接所有行。

这可以避免末尾多余分隔符带来的棘手问题。

### 常用参考

| 函数 / 方法             | 说明                 | 示例                                |
| ------------------- | ------------------ | --------------------------------- |
| `range(1, n+1)`     | 生成从 1 到 n（包含 n）的整数 | `range(1, 4)` → 1, 2, 3           |
| `str(value)`        | 将数字转换为字符串          | `str(12)` → `"12"`                |
| `"\t".join(list)`   | 使用制表符拼接列表元素        | `"\t".join(["a","b"])` → `"a\tb"` |
| `"\n".join(list)`   | 使用换行符拼接列表元素        | `"\n".join(["x","y"])` → `"x\ny"` |
| `list.append(item)` | 向列表末尾添加一个元素        | `items.append("hello")`           |
