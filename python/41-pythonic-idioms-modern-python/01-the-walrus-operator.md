# 海象运算符 `:=`（赋值表达式）

Python 3.8 引入了海象运算符 `:=`，正式名称为**赋值表达式**。它允许你在**表达式内部**为变量赋值，而不像常规的 `=` 那样是一个语句。当你需要同时计算一个值并立即使用它时，这非常强大 —— 从而消除了冗余计算或重复的函数调用。

## 工作原理

海象运算符会计算右侧的值，将其结果赋给左侧的变量，然后整个表达式的值等于该赋的值。这意味着你可以将赋值嵌入到 `if` 条件、`while` 循环、列表推导式以及生成器表达式中。

核心原则：**在能消除实际冗余并提高清晰度时使用 `:=`**。不要为了用而用 —— 如果它使代码更难阅读，请坚持使用常规赋值。

## 语法

```python
# General form — parentheses are often needed for precedence
(variable := expression)

# In an if statement
if (result := some_function()) is not None:
    process(result)

# In a while loop
while (chunk := file.read(1024)):
    handle(chunk)

# In a list comprehension
[transformed for item in collection if (transformed := transform(item)) > threshold]
```

## 示例

```python
# Example 1: Avoid calling an expensive function twice
import re

text = "Order #12345 confirmed"

if (match := re.search(r'#(\d+)', text)):
    print(f"Found order: {match.group(1)}")  # Found order: 12345

# Without walrus, you'd need:
# match = re.search(r'#(\d+)', text)
# if match:
#     print(f"Found order: {match.group(1)}")


# Example 2: Filter and transform in one pass
words = ["hello", "hi", "greetings", "yo", "hey"]

long_upper = [upper for w in words if len(upper := w.upper()) > 3]

# Result: ['HELLO', 'GREETINGS']
# Here, upper is computed once and used for both filtering and the output


# Example 3: While loop with sentinel value
import io

stream = io.StringIO("line1\nline2\nline3\n")
lines = []

while (line := stream.readline().strip()):
    lines.append(line)

# lines is now ['line1', 'line2', 'line3']


# Example 4: Counting with a generator expression
scores = [88, 42, 95, 67, 73, 55]

high_count = sum(1 for s in scores if (adjusted := s + 5) >= 80)

# adjusted >= 80 matches:
# 88+5=93,
# 95+5=100,
# 67+5=72(no),
# 73+5=78(no),
# 55+5=60(no),
# 42+5=47(no)
#
# high_count = 2
```

## 常见模式

| **模式** | **用例** |
| :--- | :--- |
| `if (x := expr):` | 一步完成计算与判断 |
| `while (x := next_item()):` | 循环直到遇到假值/哨兵值 |
| `[y for ... if (y := f(x)) > t]` | 根据转换后的值进行过滤，并保留该转换值 |
| `sum(1 for ... if (y := f(x)) > t)` | 统计计算值满足条件的元素数量 |

## 重要注意事项

- 海象运算符**不能**替代单行顶层的常规赋值语句 —— `(x := 5)` 虽然有效但毫无意义；直接写 `x = 5` 即可。
- 在推导式中，通过海象运算符赋值的变量会**泄漏到外层作用域**（与迭代变量不同）。这是设计使然，但可能会让你感到意外。
- 如果需要保留原始数据，请务必在可变数据的**副本**上操作 —— `.pop()` 会就地修改列表。