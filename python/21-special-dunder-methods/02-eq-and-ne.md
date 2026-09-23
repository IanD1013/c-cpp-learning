### 使用 `__eq__` 和 `__ne__` 进行相等性比较

默认情况下，Python 的 `==` 运算符检查的是**对象标识（object identity）**——只有当两个变量指向内存中完全相同的对象时，它才会返回 `True`，而不是当两个不同的对象恰好具有相同的属性值时。对于自定义类来说，这通常不是你想要的结果。`__eq__` 双下划线方法（dunder method）允许你为类的实例重新定义“相等”的含义。

### 工作原理

当你编写 `a == b` 时，Python 会在后台调用 `a.__eq__(b)`。如果你没有定义 `__eq__`，它会回退到标识比较（等同于 `a is b`）。通过重写 `__eq__`，你可以改为比较属性值。

一个重要的额外好处：当你定义了 `__eq__` 时，Python 会**自动**提供 `__ne__`（即 `!=` 运算符）作为其逻辑反操作。因此，你通常只需要实现 `__eq__`。

### 语法

```python
class MyClass:
    def __eq__(self, other):
        # 1. Check that 'other' is the right type
        if not isinstance(other, MyClass):
            return False
        # 2. Compare the relevant attributes
        return self.some_attr == other.some_attr
```

`isinstance` 检查非常重要——如果没有它，将你的对象与不相关的类型（如整数或字符串）进行比较可能会引发 `AttributeError`，而不是简单地返回 `False`。

### 示例

```python
# Example 1: A Temperature class where equality means the same degrees
class Temperature:
    def __init__(self, degrees):
        self.degrees = degrees

    def __eq__(self, other):
        if not isinstance(other, Temperature):
            return False
        return self.degrees == other.degrees

t1 = Temperature(72)
t2 = Temperature(72)
t3 = Temperature(68)

print(t1 == t2)  # True — same degrees
print(t1 == t3)  # False — different degrees
print(t1 != t3)  # True — __ne__ provided automatically
print(t1 == 72)  # False — 72 is not a Temperature instance
```

```python
# Example 2: A Color class comparing multiple attributes
class Color:
    def __init__(self, r, g, b):
        self.r = r
        self.g = g
        self.b = b

    def __eq__(self, other):
        if not isinstance(other, Color):
            return False
        return self.r == other.r and self.g == other.g and self.b == other.b

red1 = Color(255, 0, 0)
red2 = Color(255, 0, 0)
blue = Color(0, 0, 255)

print(red1 == red2)   # True — all three channels match
print(red1 == blue)   # False — channels differ
print(red1 == "red")  # False — string is not a Color
```

### 为什么 `isinstance` 检查很重要

如果没有类型检查，将对象与非预期类型进行比较会导致程序崩溃：

```python
# Without isinstance guard:
def __eq__(self, other):
    return self.x == other.x  # AttributeError if other has no 'x'!

# With isinstance guard:
def __eq__(self, other):
    if not isinstance(other, MyClass):
        return False  # Safe — just returns False
    return self.x == other.x
```
