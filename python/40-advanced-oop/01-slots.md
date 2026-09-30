# 用于内存优化和属性限制的 `__slots__`

默认情况下，每个 Python 对象都将其实例属性存储在一个 `__dict__` 字典中。这具有极佳的灵活性——你可以随时添加任何属性——但这种灵活性是有代价的。每个 `__dict__` 都是一个完整的哈希表，会消耗大量内存。当你创建数百万个实例时，这种开销会迅速累积。Python 的 `__slots__` 机制允许你声明一组固定的允许属性，将每个实例的字典替换为紧凑的、类似于结构体的布局。

## 工作原理

当你在类体中将 `__slots__` 定义为属性名称的元组（或列表）时，Python 会做两件事：

1. **消除 `__dict__`** — 实例不再携带实例字典。相反，Python 会分配一个固定大小的内存块，每个声明的属性对应一个槽位（slot）。
2. **限制属性** — 任何尝试设置未在 `__slots__` 中列出的属性的操作都会引发 `AttributeError`。

这会降低每个实例的内存占用（通常减少 30–50%），并且由于 Python 不需要执行字典查找，属性访问速度也会稍微加快。

## 语法

```python
class MyClass:
    __slots__ = ('attr1', 'attr2', 'attr3')

    def __init__(self, attr1, attr2, attr3):
        self.attr1 = attr1
        self.attr2 = attr2
        self.attr3 = attr3
```

`__slots__` 声明是一个类级别的变量——通常是一个包含该类将使用的所有实例属性名称的字符串元组。

## 示例

```python
# Example 1: Basic __slots__ usage
class Point:
    __slots__ = ('x', 'y')

    def __init__(self, x, y):
        self.x = x
        self.y = y

p = Point(3.0, 4.0)
print(p.x)          # 3.0
print(hasattr(p, '__dict__'))  # False — no instance dictionary!

# Trying to add an undeclared attribute fails:
# p.z = 5.0  # Raises AttributeError: 'Point' object has no attribute 'z'
```

```python
# Example 2: Comparing with a regular class
class RegularPoint:
    def __init__(self, x, y):
        self.x = x
        self.y = y

rp = RegularPoint(3.0, 4.0)
print(hasattr(rp, '__dict__'))  # True — has a full dictionary
rp.z = 5.0  # Works fine — no restriction
print(rp.__dict__)  # {'x': 3.0, 'y': 4.0, 'z': 5.0}
```

```python
# Example 3: Accessing the __slots__ tuple from the class
class Sensor:
    __slots__ = ('id', 'value', 'timestamp')

print(Sensor.__slots__)  # ('id', 'value', 'timestamp')
```

## 重要细节

- **`hasattr(obj, '__dict__')`** 是检查对象是否具有实例字典的标准方法。
- 使用 try/except 块**捕获 `AttributeError`** 可以让你以编程方式验证是否强制执行了属性限制。
- **`__slots__` 是一个类属性**，因此你应该通过 `ClassName.__slots__` 访问它，而不是通过实例访问。
- 如果你在 `__slots__` 中包含 `'__dict__'`，实例将在保留 slots 的同时重新获得字典——这在混合方案中很有用。
- 子类必须定义自己的 `__slots__`（即使为空）以保持优化；否则它们将再次获得 `__dict__`。