### 使用 `for` 循环遍历字符串

在 Python 中，字符串是**可迭代的**——这意味着你可以使用 `for` 循环逐个字符直接遍历它们。这是处理文本最自然且最具可读性的方式之一，无论你是要搜索特定字符、统计出现次数，还是构建新字符串。

### 工作原理

当你编写 `for char in some_string:` 时，Python 会自动从左到右遍历该字符串，一次将一个字符赋值给循环变量。你不需要管理索引或调用 `len()`——Python 会为你处理好一切。与像 `for i in range(len(s))` 这样基于索引的方法相比，这种方式通常更简洁且更不易出错。

### 语法

```python
for variable in some_string:
    # variable holds one character per iteration
    # do something with variable
```

### 示例

```python
# Print each character of a word on its own line
for letter in "cat":
    print(letter)
# Output:
# c
# a
# t

# Count how many times the letter 'd' appears in a sentence
sentence = "dogs and ducks dance daily"
d_count = 0
for ch in sentence:
    if ch == "d":
        d_count += 1
print(d_count)  # 5

# Build a new string with only uppercase letters
mixed = "Hello World 123"
uppers = ""
for ch in mixed:
    if ch >= "A" and ch <= "Z":
        uppers += ch
print(uppers)  # "HW"
```

### 使用 `in` 检查成员资格

`in` 运算符可以检查单个字符是否存在于另一个字符串中。当你有一组想要匹配的字符时，这非常有用。

```python
# Check if a character is a digit
if "7" in "0123456789":
    print("It's a digit!")  # This prints

# Check if a character is a punctuation mark
char = "!"
if char in ".,!?;:":
    print("Punctuation found")  # This prints
```

注意 `in` 是如何让你简洁地测试成员资格的——无需使用很长的 `or` 比较链。

### 结合循环与条件语句

一种常见的模式是遍历字符串，并在循环内部使用 `if` 语句来选择性地处理某些字符。通常，你会在循环**之前**初始化一个计数器或累加器变量，在满足条件时在循环**内部**更新它，然后在循环**之后**使用该结果。

```python
# Count spaces in a string
text = "the quick brown fox"
spaces = 0
for ch in text:
    if ch == " ":
        spaces += 1
print(spaces)  # 3
```
