### 生成器函数与 `yield` 关键字

你已经了解了如何使用 `__iter__` 和 `__next__` 手动构建迭代器。生成器函数能以更少的样板代码提供相同的功能。生成器函数看起来与普通函数类似，但它使用 `yield` 代替了 `return`。这个单一的关键字将函数转换为**生成器对象（generator objects）**的工厂——即惰性（lazy）生成值的迭代器，每次生成一个值并在每次产出值之间暂停执行。

在处理计算开销大、可能无限或过大而无法一次性保存在内存中的序列时，生成器至关重要。

### 工作原理

当你调用生成器函数时，**函数体不会立即执行**。相反，Python 会返回一个生成器对象。每次对该对象调用 `next()` 时：

1. 执行从上次暂停的位置恢复（或在首次调用时从头开始）。
2. 代码一直运行，直到遇到 `yield` 表达式。
3. 将 yield 产出的值返回给调用方。
4. 函数的整个状态——局部变量、指令指针——都会就地冻结。

当函数体结束时（执行到末尾或遇到不带返回值的 `return`），会自动引发 `StopIteration`，就像手写的 `__next__` 一样。

这与构建列表并返回有着本质的不同。列表会一次性将所有元素实例化到内存中。而生成器仅在被请求时才生成每个元素。

### 语法

```python
def my_generator():
    # Any function containing 'yield' becomes a generator function
    yield value_1
    yield value_2
    # ... execution pauses at each yield, resumes on next()
```

你可以通过 `next()`、`for` 循环或将其传递给类似 `list()` 的构造函数来消耗生成器：

```python
gen = my_generator()
next(gen)   # value_1
next(gen)   # value_2
next(gen)   # raises StopIteration
```

### 示例

**有限生成器——倒计时：**

```python
def countdown(start):
    while start > 0:
        yield start
        start -= 1

for num in countdown(3):
    print(num)
# Output: 3, 2, 1
```

**无限生成器——自然数：**

```python
def naturals():
    n = 1
    while True:
        yield n
        n += 1

gen = naturals()
print(next(gen))  # 1
print(next(gen))  # 2
print(next(gen))  # 3
# This never ends on its own — the caller decides when to stop
```

**从无限生成器中收集数据：**

因为无限生成器本身永远不会引发 `StopIteration`，所以由你来控制获取多少个值。请注意，运行中的值是在每次 `yield` *之后*更新的，因此下一次恢复时会生成下一项：

```python
def powers_of_two():
    n = 1
    while True:
        yield n
        n *= 2

gen = powers_of_two()
first_five = [next(gen) for _ in range(5)]
print(first_five)  # [1, 2, 4, 8, 16]
```

### 常见模式

| **模式描述**                |                          |
| ----------------------- | ------------------------ |
| `while True` 中的 `yield` | 创建无限生成器——由调用方控制长度        |
| `next(gen)`             | 将生成器推进单步                 |
| `list(gen)`             | 耗尽有限生成器并转换为列表            |
| `for x in gen`          | 持续迭代直到遇到 `StopIteration` |
| 从无限生成器中收集前 N 个值         | 在已知长度的循环中使用 `next()`     |
