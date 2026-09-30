# Python 高级类机制综合练习

Python 的对象模型提供了强大的机制来控制类的创建、属性访问和继承解析。本练习整合了五项高级特性：**描述符（descriptors）**、**`__slots__`**、**`__init_subclass__`**、**包含 MRO 的多重继承**以及 **mixins** —— 它们将在一个微型组件框架中协同工作。

## 描述符（Descriptors）

描述符是定义了 `__get__`、`__set__` 或 `__delete__` 中任意方法的对象。当作为类属性放置时，它会拦截对该类实例上相应属性的访问。

```python
class PositiveNumber:
    def __init__(self):
        self.attr_name = None

    def __set_name__(self, owner, name):
        self.attr_name = name

    def __get__(self, obj, objtype=None):
        if obj is None:
            return self
        return getattr(obj, '_store_' + self.attr_name, None)

    def __set__(self, obj, value):
        if not isinstance(value, (int, float)) or value <= 0:
            raise ValueError(f"{self.attr_name} must be positive")
        object.__setattr__(obj, '_store_' + self.attr_name, value)

class Product:
    price = PositiveNumber()
    quantity = PositiveNumber()

p = Product()
p.price = 9.99      # 正常工作
p.quantity = -1      # 抛出 ValueError
```

请注意 `__set_name__` 是如何自动接收属性名称的，以及实际数据是如何存储在修饰后的名称下以避免递归调用的。

## `__slots__` 与 Mixins

`__slots__` 将实例限制为仅包含列出的属性，从而节省内存并防止创建任意属性。

```python
class Timestamped:
    __slots__ = ('created_at',)

    def get_slots(self):
        return self.__class__.__slots__
```

当将 `__slots__` 与继承结合使用时，作为 mixin 的父类应该使用 `__slots__ = ()` 以避免冲突。

## `__init_subclass__`

当一个类被子类化时，此钩子会自动运行。它是类注册模式中元类（metaclasses）的一种轻量级替代方案。

```python
class Plugin:
    _plugins = {}

    def __init_subclass__(cls, **kwargs):
        super().__init_subclass__(**kwargs)
        Plugin._plugins[cls.__name__] = cls

class AudioPlugin(Plugin):
    pass

class VideoPlugin(Plugin):
    pass

print(Plugin._plugins)  # {'AudioPlugin': <class ...>, 'VideoPlugin': <class ...>}
```

## 多重继承与 MRO

Python 使用 C3 线性化算法来解析方法查找顺序。你可以通过 `ClassName.__mro__` 或 `ClassName.mro()` 来查看它。

```python
class A:
    pass

class B(A):
    pass

class C(A):
    pass

class D(B, C):
    pass

[c.__name__ for c in D.__mro__]  # ['D', 'B', 'C', 'A', 'object']
```

## 将 `__slots__` 与描述符结合

当类使用 `__slots__` 时，描述符需要有存储数据的地方。存储属性（例如 `_desc_name`）必须与逻辑属性名称一起包含在 `__slots__` 中。

```python
class Typed:
    def __init__(self, tp):
        self.tp = tp
        self.attr_name = None

    def __set_name__(self, owner, name):
        self.attr_name = name

    def __get__(self, obj, objtype=None):
        if obj is None:
            return self
        return getattr(obj, '_desc_' + self.attr_name, None)

    def __set__(self, obj, value):
        if not isinstance(value, self.tp):
            raise TypeError(f"Wrong type for {self.attr_name}")
        object.__setattr__(obj, '_desc_' + self.attr_name, value)

class Record:
    __slots__ = ('label', '_desc_label')
    label = Typed(str)
```