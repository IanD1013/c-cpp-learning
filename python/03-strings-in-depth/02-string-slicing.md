### 字符串切片

字符串切片（String slicing）允许你在不改变原字符串的情况下提取其一部分（即“切片”）。这是 Python 中最实用的特性之一，在解析文本、提取子字符串以及转换数据时，你会经常用到它。

### 工作原理

你已经了解了如何使用索引读取单个字符：`s[0]` 返回第一个字符，`s[-1]` 返回最后一个字符。切片扩展了这一概念，使你可以一次性获取一个字符**范围**。Python 会根据你选取的字符构建一个全新的字符串并将其返回，因此你可以将其返回或存储在变量中。

关键细节：**start** 索引是包含在内的，但 **end** 索引是**不包含**的。可以把它想象成数学中的左闭右开区间：`[start, end)`。

### 语法

```python
s[start:end]       # Characters from index start up to (but not including) end
s[start:end:step]  # Same, but take every 'step'-th character
```

* **start**：切片开始的位置（包含）。若省略，默认为 `0`。
* **end**：切片结束的位置（不包含）。若省略，默认为 `len(s)`。
* **step**：字符之间的步长。若省略，默认为 `1`。

### 示例

```python
word = "banana"

# Basic slicing
word[0:3]    # "ban"   characters at index 0, 1, 2
word[2:5]    # "nan"   characters at index 2, 3, 4

# Omitting start or end
word[:4]     # "bana"  from the beginning up to index 4
word[3:]     # "ana"   from index 3 to the end
word[:]      # "banana" a copy of the entire string

# Using step
word[::2]    # "bnn"   picks indices 0, 2, 4
word[1::2]   # "aaa"   picks indices 1, 3, 5
```

### 切片中的负索引

负数在切片中的用法与在索引中一样。它们从末尾向前倒数：

```python
greeting = "goodbye"

greeting[-3:]     # "bye"   last 3 characters
greeting[:-2]     # "goodb" everything except the last 2
```

### 负步长

**负步长**会反向遍历字符串：

```python
digits = "0123456789"

digits[7:2:-1]    # "76543"  from index 7 down to (not including) index 2
digits[8::-2]     # "86420"  from index 8, going backward, every other character
digits[::-1]      # "9876543210"  the entire string, reversed
```

当步长为负数且省略了 start 和 end 时，Python 会自动从最后一个字符开始并向第一个字符移动。

### 返回值

此函数需要将结果返回给调用者。使用 `return` 关键字将反转后的字符串从函数中返回：

```python
def first_two(s):
    return s[:2]   # sends the first two characters back to the caller

first_two("hello")   # gives back "he"
```

注意这里使用的是 `return`，而不是 `print`。返回值可以被其他代码使用，而不仅仅是显示在屏幕上。
