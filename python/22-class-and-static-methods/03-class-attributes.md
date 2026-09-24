### 类属性

在 Python 中，**类属性**（class attribute）是直接定义在类体中、位于任何方法之外的变量。与实例属性（通过 `self` 归属于某个特定对象）不同，类属性是由该类的**所有实例共享**的。它们存在于类本身上，非常适合用于在所有对象之间保持一致或进行累加的数据，例如配置默认值、计数器或注册表。

### 工作原理

当 Python 在实例上查找属性时，它会首先检查实例自身的 `__dict__`。如果在那里没有找到，它会向上查找到类并检查类的 `__dict__`。这意味着每个实例都可以*读取*类属性，但如果你通过 `self` 对其进行赋值，则会创建一个**新的实例属性**并遮蔽（shadow）类属性 —— 这是一个细微但至关重要的区别。

要真正修改共享的类属性，你应该通过类名（`ClassName.attr`）或在 `@classmethod` 内部通过 `cls` 来引用它。

### 语法

```python
class MyClass:
    shared_value = 42          # class attribute — shared by all instances

    def __init__(self, x):
        self.x = x             # instance attribute — unique per object

    @classmethod
    def get_shared(cls):
        return cls.shared_value  # access via cls inside a classmethod
```

### 示例

```python
# Example 1: A default setting shared across all instances
class Connection:
    timeout = 30  # seconds, shared default

    def __init__(self, host):
        self.host = host

c1 = Connection("db.local")
c2 = Connection("cache.local")
print(c1.timeout)          # 30 — reads from class
print(c2.timeout)          # 30 — same shared value

Connection.timeout = 60    # change affects ALL instances
print(c1.timeout)          # 60
print(c2.timeout)          # 60
```

```python
# Example 2: Counting instances with a classmethod
class Animal:
    population = 0

    def __init__(self, species):
        self.species = species
        Animal.population += 1   # modify via class name

    @classmethod
    def total(cls):
        return cls.population

Animal("cat")
Animal("dog")
Animal("parrot")
print(Animal.total())  # 3
```

```python
# Example 3: The shadowing trap
class Config:
    debug = False

c = Config()
c.debug = True       # creates an INSTANCE attribute on c!
print(c.debug)        # True  — instance attribute
print(Config.debug)   # False — class attribute is unchanged
```

### 常见模式

| **模式在何处修改示例**     |                                |                          |
| ----------------- | ------------------------------ | ------------------------ |
| 从实例读取             | `self.attr` 或 `ClassName.attr` | `print(self.timeout)`    |
| 修改共享值             | `ClassName.attr = ...`         | `Animal.population += 1` |
| 在 classmethod 中访问 | `cls.attr`                     | `return cls.population`  |
| 在测试之间重置           | `ClassName.attr = initial`     | `Animal.population = 0`  |
