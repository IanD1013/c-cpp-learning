### `.format()` 方法和格式规范

在 Python 3.6 引入 f-string 之前，`.format()` 方法是构建格式化字符串的主要方式。它在代码库、日志配置以及模板字符串与数据分开存储的场景中仍然被广泛使用。理解 `.format()` 和格式规范可以让你精确控制值在输出中的呈现方式。

### `.format()` 的工作原理

`.format()` 方法在包含花括号占位符 `{}` 的字符串上调用。每个占位符都会被对应的参数替换：

```python
"Hello, {}!".format("Alice")        # "Hello, Alice!"
"{} + {} = {}".format(3, 4, 7)       # "3 + 4 = 7"
```

你还可以使用位置参数或关键字参数：

```python
"{0} likes {1}, and {1} likes {0}".format("Sam", "Jo")  # "Sam likes Jo, and Jo likes Sam"
"{name} is {age}".format(name="Lee", age=30)             # "Lee is 30"
```

### 格式规范

在花括号内，冒号 `:` 引入了一个**格式规范**（format specification），用于控制值的显示方式。这些规范在 `.format()` 和 f-string 中的用法完全相同。

#### 数字格式化

使用 `:.Nf` 可以显示保留恰好 `N` 位小数的浮点数：

```python
"{:.2f}".format(3.14159)    # "3.14"
"{:.4f}".format(2.5)        # "2.5000"
f"{7:.1f}"                  # "7.0"
```

请注意，当显示较少的小数位数时，Python 会对数值进行四舍五入；当要求更多的小数位数时，则会用零填充。

#### 对齐和填充

你可以控制字段的宽度以及值在其中的对齐方式：

| 规范        | 含义             | 示例                       | 结果             |
| --------- | -------------- | ------------------------ | -------------- |
| `{:>10}`  | 在 10 个字符宽内右对齐  | `"{:>10}".format("hi")`  | `" hi"`        |
| `{:<10}`  | 在 10 个字符宽内左对齐  | `"{:<10}".format("hi")`  | `"hi "`        |
| `{:^10}`  | 在 10 个字符宽内居中对齐 | `"{:^10}".format("hi")`  | `" hi "`       |
| `{:*^10}` | 居中对齐，使用 `*` 填充 | `"{:*^10}".format("hi")` | `"****hi****"` |

#### 组合规范

你可以将对齐和数字格式化结合使用：

```python
"{:>10.2f}".format(42.5)    # "     42.50"  (right-aligned, 2 decimals, 10-wide)
"{:0>8.2f}".format(3.7)     # "00003.70"    (zero-padded, 2 decimals, 8-wide)
```

### 常见模式

以下是 `.format()` 与格式规范在实际中的一些用法：

```python
# Currency-style output
template = "Total: EUR {:.2f}"
print(template.format(249.5))       # "Total: EUR 249.50"

# Table-like alignment
for name, score in [("Ada", 97.3), ("Bo", 100)]:
    print("{:<8} {:>6.1f}".format(name, score))
# Ada       97.3
# Bo       100.0

# Percentage formatting
"{:.1%}".format(0.856)              # "85.6%"
```
