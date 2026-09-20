### `for` 循环中的 `break` 语句

你已经了解了 `break` 在 `while` 循环中的工作原理 —— 它会立即退出循环，跳过所有剩余的迭代。`break` 语句在 `for` 循环中的工作方式**完全相同**。这在“搜索并停止”模式中特别强大：当你需要在序列中查找某个内容时，一旦找到就可以立即提前退出，而不是毫无意义地继续扫描剩余部分。

### 工作原理

当 Python 在 `for` 循环内部遇到 `break` 时，它会立即跳出循环体，并继续执行循环**之后**的第一条语句。不会再进行后续的迭代。当继续循环会造成无用功时，这非常有用 —— 一旦找到了要找的内容，就无需继续检查了。

### 语法

```python
for item in sequence:
    if some_condition:
        # do something with the found item
        break
```

`break` 只会退出**最内层的封闭循环**。循环之后的代码会正常运行。

### 示例

假设你想在列表中找到第一个负数：

```python
numbers = [4, 7, 2, -3, 8, -1]
first_negative = None
for n in numbers:
    if n < 0:
        first_negative = n
        break  # Found it — no need to check 8 and -1
# first_negative is -3
```

或者检查一个名字列表中是否包含以 `"Z"` 开头的名字：

```python
names = ["Alice", "Bob", "Zara", "Charlie"]
has_z_name = False
for name in names:
    if name.startswith("Z"):
        has_z_name = True
        break  # Stop immediately — we found one
# has_z_name is True
```

如果没有 `break`，即使答案已经确定，这两个循环也依然会继续遍历每一个剩余的元素。

### 常见模式：标志变量 (Flag) + Break

解决搜索问题的典型方法是将**布尔标志变量**与 `break` 结合使用：

1. 在循环之前将标志变量设为 `False`。
2. 遍历序列。
3. 当找到要找的内容时，将标志设为 `True` 并执行 `break`。
4. 循环结束后，该标志会告诉你搜索是否成功。

### 常用参考

| **方法 / 函数**     | **描述**                  | **示例**                   |
| --------------- | ----------------------- | ------------------------ |
| `str.isdigit()` | 如果字符是数字（0–9），则返回 `True` | `"7".isdigit()` → `True` |
| `str.isalpha()` | 如果字符是字母，则返回 `True`      | `"a".isalpha()` → `True` |
| `str.isupper()` | 如果字符是大写字母，则返回 `True`    | `"A".isupper()` → `True` |
