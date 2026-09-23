### 让自定义类像内置容器一样工作

Python 的内置容器——列表（list）、集合（set）、字典（dict）——都支持 `len()` 和 `in` 运算符。当你构建用于容纳数据集合的自定义类时，可以通过实现两个双下划线方法（dunder methods）：`__len__` 和 `__contains__`，让它们的使用体验同样自然。这意味着操作这些对象的代码无需了解其内部实现细节——直接使用熟悉的 Python 语法即可。

### 工作原理

当你调用 `len(obj)` 时，Python 内部会调用 `obj.__len__()`。当你编写 `item in obj` 时，Python 内部会调用 `obj.__contains__(item)`。通过在类中定义这些方法，你就接入了 Python 用于实现类容器行为的协议。这与让 `len([1, 2, 3])` 返回 `3` 以及 `"apple" in ["apple", "banana"]` 返回 `True` 的机制是完全相同的。

如果你没有定义 `__contains__`，Python 会回退到通过 `__iter__`（如果已定义）进行遍历查找，但显式定义它能让你拥有更好的控制力和更高的性能。

### 语法

```python
class MyContainer:
    def __init__(self):
        self._items = []

    def __len__(self):
        # Return an integer representing the "size" of this container
        return len(self._items)

    def __contains__(self, item):
        # Return True if item is found, False otherwise
        return item in self._items
```

### 示例

```python
# A custom bookshelf that supports len() and 'in'
class Bookshelf:
    def __init__(self, owner):
        self.owner = owner
        self._books = []

    def place_book(self, title):
        self._books.append(title)

    def __len__(self):
        return len(self._books)

    def __contains__(self, title):
        return title in self._books

shelf = Bookshelf("Alice")
shelf.place_book("Dune")
shelf.place_book("1984")

print(len(shelf))          # 2
print("Dune" in shelf)     # True
print("Hamlet" in shelf)   # False
```

```python
# A shopping cart with case-insensitive search
class ShoppingCart:
    def __init__(self):
        self._items = []

    def add_item(self, item):
        self._items.append(item.lower())

    def __len__(self):
        return len(self._items)

    def __contains__(self, item):
        return item.lower() in self._items

cart = ShoppingCart()
cart.add_item("Milk")
print(len(cart))          # 1
print("milk" in cart)     # True (case-insensitive)
print("Bread" in cart)    # False
```

### 常见模式

* `__len__` 应始终返回一个非负整数。
* `__contains__` 应始终返回一个布尔值（`True` 或 `False`）。
* 这两个方法通常委托给内部数据结构处理——你不需要手动循环或计数；直接借助底层的 list、set 或 dict 即可。
* 这些方法可以与 `__iter__` 和 `__getitem__` 很好地结合，构建出完全支持迭代和索引访问的容器。
