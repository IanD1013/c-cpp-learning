### 使用 `*args` 处理可变位置参数

有时你需要一个可以接收任意数量参数的函数——可能是两个、二十个，或者根本没有参数。Python 的 `*args` 语法允许你定义具有灵活参数列表的函数，这在构建工具函数、包装器（wrapper）或任何由调用者决定传入多少个值的函数时至关重要。

### 工作原理

当你在函数定义中的参数名前加上星号（`*`）时，Python 会将所有额外的位置参数收集到一个 **tuple** 中。在函数体内部，该参数的行为与普通 tuple 完全一致——你可以遍历它、通过索引访问它、检查其长度，以及使用你已知的所有 tuple 操作。

名称 `args` 纯粹是一种约定。你也可以写成 `*numbers`、`*items` 或 `*values`——起关键作用的是 `*`，而不是名称本身。

### 语法

```python
# Collecting all positional arguments
def my_function(*args):
    # args is a tuple of whatever was passed
    print(type(args))  # <class 'tuple'>

# Mixing regular parameters with *args
def my_function(first, second, *rest):
    # first and second are required
    # rest captures everything else as a tuple
    pass
```

关键规则：

* `*args` 必须位于所有常规位置参数**之后**
* 每个函数只允许有**一个** `*args` 参数
* 如果没有传递额外的参数，`args` 是一个空 tuple `()`

### 示例

```python
# Example 1: A function that finds the longest string
def longest(*words):
    if not words:
        return ""
    result = words[0]
    for w in words:
        if len(w) > len(result):
            result = w
    return result

longest("cat", "elephant", "dog")  # "elephant"
longest("hi")                      # "hi"
longest()                           # ""

# Example 2: Regular param before *args
def greet(greeting, *names):
    messages = []
    for name in names:
        messages.append(f"{greeting}, {name}!")
    return messages

greet("Hello", "Alice", "Bob")  # ["Hello, Alice!", "Hello, Bob!"]
greet("Hi")                     # [] (no names passed, *names is empty)

# Example 3: Counting how many arguments were passed
def count_args(*args):
    return len(args)

count_args(10, 20, 30)  # 3
count_args()            # 0
```

请注意在每个示例中，`*args`（或 `*words`、`*names`）都只是一个 tuple。你可以使用 `len()`、用 `for` 循环遍历、用 `[0]` 索引，或者应用任何 tuple 操作。

### 常见模式

* **空值检查**：当没有传入参数时，`args` 为 `()`——一个布尔值为假的空 tuple。你可以通过检查 `if not args:` 来处理这种情况。
* **遍历**：由于 `args` 是一个 tuple，使用简单的 `for` 循环即可处理每个元素。
* **与常规参数结合**：先放置必填参数，再使用 `*args` 捕获其余参数：`def f(required, *optional):`
