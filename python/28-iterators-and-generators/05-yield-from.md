### `yield from` 表达式

当你编写生成器函数并需要将其部分工作委托给另一个可迭代对象或子生成器时，你常常会发现自己需要编写重复的 `for item in sub: yield item` 循环。Python 的 `yield from` 表达式就是为了消除这种样板代码而引入的。它可以透明地将迭代委托给另一个可迭代对象，产生其中的每个值，就好像外层生成器直接产出它们一样。在编写遍历嵌套数据结构的递归生成器时，这一点尤其强大。

### 工作原理

当生成器遇到 `yield from some_iterable` 时，它会暂停自身的执行并将控制权交给 `some_iterable`。`some_iterable` 产出的每个值都会直接 yield 给外层生成器的调用方。一旦 `some_iterable` 耗尽，外层生成器将在 `yield from` 语句之后恢复执行。

关键在于，`yield from` 适用于**任何可迭代对象**——列表、元组、字符串、range、集合、字典以及其他生成器。当与递归结合使用时，它为遍历树状或深度嵌套的结构提供了一种优雅的模式。

### 语法

```python
def outer_generator():
    yield from some_iterable
    # execution continues here after some_iterable is exhausted
```

这等价于：

```python
def outer_generator():
    for item in some_iterable:
        yield item
```

但是 `yield from` 更为简洁，并且在委托给另一个生成器时，还能正确处理 `.send()`、`.throw()` 和 `.close()` 的传递。

### 示例

```python
# Example 1: Delegating to a simple iterable
def count_then_letters():
    yield from range(1, 4)
    yield from "abc"

list(count_then_letters())  # [1, 2, 3, 'a', 'b', 'c']


# Example 2: Chaining multiple generators
def evens(n):
    for i in range(0, n, 2):
        yield i

def odds(n):
    for i in range(1, n, 2):
        yield i

def all_numbers(n):
    yield from evens(n)
    yield from odds(n)

list(all_numbers(6))  # [0, 2, 4, 1, 3, 5]


# Example 3: Recursive generator traversing a tree
# Given a tree as (value, [children...]):
def walk_tree(node):
    value, children = node
    yield value
    for child in children:
        yield from walk_tree(child)

tree = ("root", [("a", [("a1", [])]), ("b", [])])
list(walk_tree(tree))  # ['root', 'a', 'a1', 'b']
```

请注意在示例 3 中，递归调用 `yield from walk_tree(child)` 是如何替代原本包含显式 yield 的嵌套循环的。每次递归调用都会产出其值，而 `yield from` 会将它们直接传递给最初的调用方。

### 递归生成器与嵌套结构

使用 `yield from` 递归处理嵌套结构的模式遵循一个常见的形式：

1. 遍历当前层级中的每个元素
2. 如果某个元素本身是一个需要深入遍历的集合，则使用 `yield from` 进行递归调用
3. 否则，直接 `yield` 该元素

关键的决策在于确定**何时递归**以及**何时 yield**。你需要一个条件来区分“容器”元素与“叶子”元素。Python 的 `isinstance()` 函数对于检查某个值是否为特定类型非常有用。
