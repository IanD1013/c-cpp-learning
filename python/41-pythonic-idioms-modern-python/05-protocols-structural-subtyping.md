### Protocol：用于类型安全鸭子类型的结构子类型化 (Structural Subtyping)

Python 一直崇尚鸭子类型（duck typing）——如果一个对象走起路来像鸭子，叫起来也像鸭子，那它就是鸭子。但从历史上看，这种哲学缺乏类型安全性。`typing` 模块中的 `Protocol` 类通过允许你基于**结构**而非**继承**来定义接口，从而使鸭子类型形式化。一个类只需拥有正确的方法和属性即可满足 Protocol 的要求——无需显式地写 `class Foo(MyProtocol)`。

### 工作原理

当你定义一个 Protocol 时，你声明了一个对象必须拥有的方法签名（以及可选的属性）集合。任何恰好实现了这些方法的类都会自动被视为兼容——类型检查器会静态地验证这一点，并且配合 `@runtime_checkable`，你甚至可以在运行时使用 `isinstance()`。

这与抽象基类（ABC）有着本质的不同：使用 ABC 时，类必须显式继承自基类；而使用 Protocol 时，兼容性纯粹基于结构。

### 语法

```python
from typing import Protocol, runtime_checkable

# 定义一个 Protocol — 注意方法体只是 `...` (Ellipsis)
class Flyable(Protocol):
    def fly(self) -> str: ...

# 添加 @runtime_checkable 可以启用 isinstance() 检查
@runtime_checkable
class Sized(Protocol):
    def __len__(self) -> int: ...
```

### 示例

```python
from typing import Protocol, runtime_checkable

@runtime_checkable
class Greetable(Protocol):
    def greet(self) -> str: ...

# 这个类在没有继承 Greetable 的情况下满足了 Greetable 的要求
class FriendlyRobot:
    def greet(self) -> str:
        return "Beep boop, hello!"

class ShyAlien:
    def greet(self) -> str:
        return "...*waves tentacle*..."

robot = FriendlyRobot()
alien = ShyAlien()

# 得益于 @runtime_checkable，两者都能通过 isinstance 检查
print(isinstance(robot, Greetable))  # True
print(isinstance(alien, Greetable))  # True

# 但这两个类都没有继承自 Greetable
print(Greetable in type(robot).__mro__)  # False
print(FriendlyRobot.__bases__)           # (object,)
```

```python
@runtime_checkable
class Serializable(Protocol):
    def to_json(self) -> str: ...
    def byte_size(self) -> int: ...

# 一个类必须拥有全部方法才能满足 protocol 的要求
class Config:
    def to_json(self) -> str:
        return '{"debug": true}'

    def byte_size(self) -> int:
        return 16

class PartialConfig:
    def to_json(self) -> str:
        return '{}'  # 缺少 byte_size！

print(isinstance(Config(), Serializable))         # True
print(isinstance(PartialConfig(), Serializable))  # False — 缺少 byte_size
```

### 验证基于结构（而非继承）的子类型化

要确认一个对象仅通过结构（而非继承）满足了 Protocol 的要求，你可以检查该类的方法解析顺序（Method Resolution Order，简称 MRO）：

```python
# type(obj).__mro__ 以元组形式返回继承链
print(type(robot).__mro__)  # (<class 'FriendlyRobot'>, <class 'object'>)

# 如果 Greetable 不在 MRO 中，则说明匹配纯粹是基于结构的
print(Greetable not in type(robot).__mro__)  # True — 鸭子类型！
```

### 关键要点

| **概念** | **描述** |
|---|---|
| `Protocol` | 用于定义结构化接口的基类 |
| `@runtime_checkable` | 允许在运行时进行 `isinstance()` 检查的装饰器 |
| 方法体 `...` | 在 Protocol 方法声明中使用 Ellipsis 作为方法体 |
| `type(obj).__mro__` | 继承链中各类组成的元组 |
| 结构子类型化 | 基于结构而非继承的兼容性 |