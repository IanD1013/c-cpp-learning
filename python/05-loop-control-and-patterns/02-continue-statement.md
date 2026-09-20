### `continue` 语句

在遍历数据时，你经常需要跳过某些项，而不是完全停止循环。`continue` 语句正是用于实现这一功能——它会跳过当前迭代的剩余部分，并直接跳转到下一次迭代。这与完全退出循环的 `break` 不同。使用 `continue` 时，循环会继续执行；它只是不会为该特定迭代执行 `continue` 下方的任何代码。

### 工作原理

当 Python 在循环内部遇到 `continue` 时，它会立即停止执行当前迭代的剩余代码并移动到下一项。可以把它想象成翻过书中不想读的一页——你并没有合上书（`break`），只是跳过了那一页继续阅读。

如果没有 `continue`，你通常需要将处理逻辑嵌套在 `if` 代码块中。`continue` 允许你提前处理“跳过”条件，并将主要逻辑保持在较低的缩进级别，使你的代码更整洁、更易读。

### 语法

```python
for item in collection:
    if some_condition:
        continue
    # This code only runs when some_condition is False
    do_something(item)
```

### 示例

```python
# Print only even numbers from 1 to 8
for i in range(1, 9):
    if i % 2 != 0:
        continue
    print(i)
# Output: 2, 4, 6, 8 (each on its own line)

# Build a list of non-empty words from a sentence
words = []
for word in "hello  world  python".split(" "):
    if word == "":
        continue
    words.append(word)
# words is now ["hello", "world", "python"]

# Count lowercase vowels in a string
count = 0
for ch in "Bravo Echo":
    if ch not in "aeiou":
        continue
    count += 1
# count is 3: the 'a' and 'o' in "Bravo", plus the 'o' in "Echo"
# (the uppercase 'E' is skipped, since "aeiou" holds only lowercase vowels)
```

请注意 `continue` 是如何将“主要路径”代码保持在主缩进级别的。如果没有它，你就必须将逻辑包裹在 `if` 代码块中，随着条件的增加，代码会变得混乱。

### 常见模式

* **过滤不需要的值**：在循环体开头尽早跳过空值、负数或无效条目。
* **避免深层嵌套**：与其使用 `if valid: ... (indented block)`，不如使用 `if not valid: continue` 来保持主逻辑扁平化。
* **使用累加器进行处理**：在求和、计数或收集结果时，`continue` 可以干净利落地跳过不应计入的项。
