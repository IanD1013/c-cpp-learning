### 在 Python 中写入文件

你已经知道如何使用 `open()`、`read()` 和 `readlines()` 来读取文件。写入文件的模式与之类似——打开一个文件，但这次使用不同的**模式（mode）**来告诉 Python 你想要写入数据而不是读取数据。文件写入对于生成报告、保存程序状态、导出数据以及创建配置文件至关重要。

### 工作原理

当你调用 `open(path, 'w')` 时，Python 会以**写入模式**打开位于 `path` 的文件。此时会自动发生两种重要行为：

1. **如果文件不存在**，Python 会自动为你创建它。
2. **如果文件已存在**，Python 会将其**截断（truncate）**——这意味着在开始写入之前，它会清空所有现有内容。请务必记住这个关键细节，因为使用 `'w'` 打开现有文件会抹去其中的所有内容。

打开文件后，可以使用 `f.write(text)` 方法将字符串写入文件。与 `print()` 不同，`write()` 方法**不会**自动添加换行符——它只会完全按照你提供的字符串进行写入。

### 语法

```python
# 写入文件的通用模式
with open(file_path, 'w') as f:
    f.write(some_string)
```

`with` 语句确保在代码块结束时文件会被正确关闭，即使发生错误也不例外。在处理文件时，你应**始终**使用 `with`——它可以防止资源泄漏和文件损坏。

### 示例

```python
# 示例 1：向日志文件写入单行内容
with open("status.log", "w") as f:
    f.write("System initialized successfully")
# status.log 现在包含：System initialized successfully

# 示例 2：写入多行内容（注意显式的 \n）
with open("shopping.txt", "w") as f:
    f.write("Eggs\n")
    f.write("Milk\n")
    f.write("Bread\n")
# shopping.txt 包含三行内容

# 示例 3：写入后重新读取以进行验证
with open("greeting.txt", "w") as f:
    f.write("Bonjour!")

with open("greeting.txt", "r") as f:
    result = f.read()  # result 为 "Bonjour!"
```

### 常见模式

* **写入并验证**：将数据写入文件，然后以读取模式再次打开以确认内容。这对于测试和数据完整性检查非常有用。
* **先构建内容**：在内存中构建好完整的字符串，然后通过单次 `f.write()` 调用完成写入，而不是进行多次小规模写入。
* **独立的 `with` 代码块**：使用一个 `with` 代码块进行写入，并使用另一个独立的 `with` 代码块进行读取。你无法从以 `'w'` 模式打开的文件中读取内容。

### 核心区别：`write()` 对比 `print()`

| **特性**    | `f.write(text)` | `print(text, file=f)` |
| --------- | --------------- | --------------------- |
| 添加换行符？    | 否               | 是                     |
| 接受非字符串类型？ | 否（仅限字符串）        | 是（自动转换）               |
| 返回值       | 写入的字符数          | None                  |
