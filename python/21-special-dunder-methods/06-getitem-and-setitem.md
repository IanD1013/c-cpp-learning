### 使用 `__getitem__` 和 `__setitem__` 实现基于方括号的索引

Python 的方括号表示法——也就是你在列表和字典上使用的 `[]`——并不是什么魔法。它是由两个可以在你自己的类中实现的双下划线方法（dunder methods）所支持的。这使你的自定义对象能够支持与内置类型相同的直观 `obj[key]` 语法，从而让你的 API 感觉自然且符合 Python 风格（Pythonic）。

### 工作原理

当 Python 遇到 `obj[key]` 时，它会在后台调用 `obj.__getitem__(key)`。当遇到 `obj[key] = value` 时，它会调用 `obj.__setitem__(key, value)`。通过定义这些方法，可以让你的类完全控制方括号读取和写入时发生的操作。

`key` 参数可以是任何内容——整数索引、字符串键、切片，甚至是元组。具体如何处理完全取决于你的实现。

### 语法

```python
class MyCollection:
    def __getitem__(self, key):
        # Called for: my_collection[key]
        # Return the value associated with key
        ...

    def __setitem__(self, key, value):
        # Called for: my_collection[key] = value
        # Store value at the given key
        ...
```

### 示例

```python
# A simple wrapper around a dictionary
class Config:
    def __init__(self):
        self._data = {}

    def __getitem__(self, key):
        return self._data[key]

    def __setitem__(self, key, value):
        self._data[key] = value

config = Config()
config["theme"] = "dark"      # calls __setitem__("theme", "dark")
print(config["theme"])        # calls __getitem__("theme") -> "dark"
```

```python
# A grid that stores values by (row, col) tuples
class Grid:
    def __init__(self, width, height):
        self._cells = {}
        self.width = width
        self.height = height

    def __getitem__(self, pos):
        return self._cells.get(pos, 0)

    def __setitem__(self, pos, value):
        self._cells[pos] = value

g = Grid(3, 3)
g[(1, 2)] = 42                # __setitem__ with a tuple key
print(g[(1, 2)])              # __getitem__ -> 42
print(g[(0, 0)])              # __getitem__ -> 0 (default)
```

### 链式方括号访问

一个需要理解的重要模式：当你编写 `obj[a][b]` 时，Python 会从左到右对其求值。首先 `obj[a]` 调用 `__getitem__` 并返回一个结果——然后针对**该返回值**调用 `[b]`。如果 `__getitem__` 返回一个列表，则第二个方括号将使用该列表自身的 `__getitem__`。

```python
class Shelf:
    def __init__(self):
        self._rows = [["book1", "book2"], ["book3", "book4"]]

    def __getitem__(self, index):
        return self._rows[index]

shelf = Shelf()
print(shelf[1][0])  # shelf.__getitem__(1) returns ["book3", "book4"]
                    # then ["book3", "book4"][0] returns "book3"
```
