# 多重继承与方法解析顺序（MRO）

Python 支持多重继承，这意味着一个类可以继承自多个父类。当多个基类定义了相同的方法时，Python 需要一种确定性的方法来决定调用哪个版本。这就是**方法解析顺序（Method Resolution Order，简称 MRO）**发挥作用的地方——它定义了 Python 在解析方法调用时搜索基类的精确顺序。

## 工作原理

Python 使用 **C3 线性化算法（C3 linearization algorithm）**来计算 MRO。该算法保证：

1. **子类先于父类**——子类总是出现在其基类之前。
2. **保持从左到右的顺序**——如果定义了 `class C(A, B)`，则在搜索 `B` 之前先搜索 `A`。
3. **每个类仅出现一次**——在方法解析过程中，任何类都不会被访问两次。

经典的挑战是**菱形继承问题（diamond problem）**：当两个父类共享一个共同的祖先类时。如果没有严密的顺序，祖父类的方法可能会被调用多次或以错误的顺序被调用。

## 语法

```python
# Multiple inheritance
class Child(Parent1, Parent2):
    pass

# Inspect the MRO
Child.__mro__        # Returns a tuple of classes
Child.mro()          # Returns a list of classes

# super() follows the MRO, NOT just the direct parent
class Parent1(GrandParent):
    def greet(self):
        return "Parent1 -> " + super().greet()  # Calls next in MRO
```

## 示例

考虑一个日志系统的继承层次结构：

```python
class Logger:
    def log(self):
        return ["Logger"]

class FileLogger(Logger):
    def log(self):
        return ["FileLogger"] + super().log()

class NetworkLogger(Logger):
    def log(self):
        return ["NetworkLogger"] + super().log()

class HybridLogger(FileLogger, NetworkLogger):
    def log(self):
        return ["HybridLogger"] + super().log()

# MRO: HybridLogger -> FileLogger -> NetworkLogger -> Logger -> object
print([c.__name__ for c in HybridLogger.__mro__])
# ['HybridLogger', 'FileLogger', 'NetworkLogger', 'Logger', 'object']

print(HybridLogger().log())
# ['HybridLogger', 'FileLogger', 'NetworkLogger', 'Logger']
```

注意，`FileLogger.log()` 内的 `super()` 并没有**直接**调用 `Logger.log()`——它调用的是 `NetworkLogger.log()`，因为那是 MRO 中的下一个类。这是一个关键认知：`super()` 是遵循 MRO 的，而不是仅仅指向直接父类。

另一个展示 MRO 检查的示例：

```python
class Animal:
    pass

class Flyer(Animal):
    pass

class Swimmer(Animal):
    pass

class Duck(Flyer, Swimmer):
    pass

print(len(Duck.__mro__))  # 5: Duck, Flyer, Swimmer, Animal, object
print(Duck.__mro__[-1].__name__)  # 'object' — always last
```

## 常见模式

| **模式** | **描述** |
| :--- | :--- |
| `super().method()` | 调用 MRO 中的下一个类，不一定是直接父类 |
| `[c.__name__ for c in Cls.__mro__]` | 获取易于阅读的 MRO 类名字符串列表 |
| `len(Cls.__mro__)` | 计算解析链中有多少个类 |
| `list.count(item) == 1` | 验证某项在列表中是否恰好出现一次 |

## 核心认知：菱形继承问题

在一个 `D` 继承自 `B` 和 `C`，且 `B` 与 `C` 均继承自 `A` 的菱形层次结构中，C3 线性化确保 `A` 在 MRO 中仅出现**一次**——位于 `B` 和 `C` 之后。

当每个类都使用 `super()` 进行委托调用时，尽管有两条路径通向 `A`，`A` 中的方法也仅会被精确调用一次。这种协作式多重继承是 Python 解决菱形继承问题的优雅方案。