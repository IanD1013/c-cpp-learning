# `match/case` 中的类模式（Class Patterns）

你已经了解了如何匹配字面量、捕获变量、使用 OR 模式以及应用守卫（guards）。**类模式（Class patterns）**将结构化模式匹配提升到了一个新的水平，它允许你匹配特定类的实例，并在单一步骤中解构其属性。在处理对象层次结构、数据模型或任何行为取决于对象的类型和内容的场景时，这非常强大。

## 工作原理

类模式使用类名后接包含属性模式的圆括号。当 Python 遇到类模式时，它会检查两件事：

1. 该对象是否为该类的实例？
2. 这些属性是否匹配括号内的子模式？

对于 **dataclasses**（以及带有 `__match_args__` 的类），Python 知道属性顺序，因此你可以进行**位置匹配**。

对于**关键字匹配**，你需要显式指定属性名称——这适用于任何类。

## 语法

```python
match some_object:
    case ClassName(attr1=pattern1, attr2=pattern2):
        # Matches if some_object is a ClassName instance
        # AND attr1 matches pattern1, attr2 matches pattern2

    case ClassName(pattern1, pattern2):
        # Positional matching (requires __match_args__)
        # pattern1 binds to the first attribute, pattern2 to the second
```

## 示例

```python
from dataclasses import dataclass

@dataclass
class Vector:
    x: float
    y: float

@dataclass
class Color:
    r: int
    g: int
    b: int

def describe(obj):
    match obj:
        # Keyword matching — captures y into variable dy
        case Vector(x=0, y=dy):
            return f"Vertical vector with y={dy}"

        # Positional matching — captures both attributes
        case Vector(vx, vy):
            return f"Vector({vx}, {vy})"

        # Matching with a guard
        case Color(r=r, g=g, b=b) if r == g == b:
            return f"Grayscale color: {r}"

        case Color(r=r, g=g, b=b):
            return f"Color({r}, {g}, {b})"


describe(Vector(0, 5))          # -> "Vertical vector with y=5"
describe(Vector(3, 4))          # -> "Vector(3, 4)"
describe(Color(128, 128, 128))  # -> "Grayscale color: 128"
describe(Color(255, 0, 0))      # -> "Color(255, 0, 0)"
```

## 内置类型匹配

类模式也适用于内置类型，这对于根据类型进行分发非常有用：

```python
def classify(value):
    match value:
        case int(n) if n > 0:
            return f"Positive integer: {n}"

        case int(n):
            return f"Non-positive integer: {n}"

        case str(s):
            return f"String of length {len(s)}"

        case float(f):
            return f"Float: {f}"


classify(42)     # -> "Positive integer: 42"
classify(-3)     # -> "Non-positive integer: -3"
classify("hi")   # -> "String of length 2"
```

## 将类模式与守卫结合使用

你可以在类模式后添加 `if` 守卫来实现额外的约束：

```python
match shape:
    case Circle(radius=r) if r > 100:
        # Only matches circles with radius greater than 100
```

## 无捕获匹配

使用不带参数的 `case ClassName()` 可以在不绑定任何属性的情况下检查类型：

```python
match obj:
    case Vector():
        return "It's some kind of vector"
```