# `__init_subclass__` — 钩入子类创建过程

Python 的 `__init_subclass__` 是一个类方法，每当创建一个新的子类时，它都会在**父类**上被自动调用。它提供了一种简洁的、无需使用元类（metaclass）的方式，可以在类定义时自定义、验证或注册子类。

这对于插件系统、框架钩子，以及任何需要让基类感知其子类的场景都非常有用。

## 工作原理

当你写：

```python
class Child(Parent):
    pass
```

Python 不仅仅会建立继承关系——它还会调用：

```python
Parent.__init_subclass__(Child)
```

父类会接收到刚刚创建的新子类，并将它作为 `cls` 参数传入，因此父类可以检查这个新子类、修改它，或者保存对它的引用。

如果类定义中包含关键字参数，例如：

```python
class Child(Parent, key="value"):
    pass
```

那么这些关键字参数也会被传递给 `__init_subclass__`。

关键点是：这个钩子是在**类定义时（class definition time）**触发的，而不是在创建实例时触发的。

也就是说，即使这个子类从来没有被实例化，`__init_subclass__` 仍然会执行。

## 语法

```python
class Base:
    def __init_subclass__(cls, **kwargs):
        super().__init_subclass__(**kwargs)
        # 在这里添加自定义逻辑
        # cls 就是新创建的子类
```

从类定义中传递关键字参数：

```python
class Base:
    def __init_subclass__(cls, tag=None, **kwargs):
        super().__init_subclass__(**kwargs)
        cls.tag = tag

class Child(Base, tag="important"):
    pass

# Child.tag == "important"
```

## 示例

### 示例 1：自动注册 Handler 类

```python
class EventHandler:
    handlers = []

    def __init_subclass__(cls, **kwargs):
        super().__init_subclass__(**kwargs)
        EventHandler.handlers.append(cls)

class ClickHandler(EventHandler):
    pass

class KeyHandler(EventHandler):
    pass

print(EventHandler.handlers)
# [<class 'ClickHandler'>, <class 'KeyHandler'>]
```

### 示例 2：从类定义中传递关键字参数

```python
class Serializer:
    format_map = {}

    def __init_subclass__(cls, fmt=None, **kwargs):
        super().__init_subclass__(**kwargs)
        if fmt:
            Serializer.format_map[fmt] = cls

class JsonSerializer(Serializer, fmt="json"):
    pass

class XmlSerializer(Serializer, fmt="xml"):
    pass

print(Serializer.format_map)
# {'json': <class 'JsonSerializer'>, 'xml': <class 'XmlSerializer'>}
```

### 示例 3：使用 `type()` 动态创建类也会触发 `__init_subclass__`

```python
class Validator:
    registry = {}

    def __init_subclass__(cls, **kwargs):
        super().__init_subclass__(**kwargs)
        Validator.registry[cls.__name__] = cls

# type(name, bases, namespace) 可以动态创建一个类
Dynamic = type("EmailValidator", (Validator,), {})

print("EmailValidator" in Validator.registry)
# True
```

## 重要细节

- 始终调用：

  ```python
  super().__init_subclass__(**kwargs)
  ```

  这样可以支持协作式多重继承（cooperative multiple inheritance），并避免由于未预期的关键字参数而产生 `TypeError`。

- 如果某个参数定义了默认值，而类定义时没有提供对应的关键字参数，那么就会使用该默认值。

- `type(name, bases, dict)` 可以在运行时创建一个新的类。如果它的某个基类定义了 `__init_subclass__`，那么这个方法同样会被触发——因为动态创建出来的类也是一个真正的子类。

- `issubclass(A, B)` 用于判断 `A` 是否是 `B` 的子类。如果是，则返回 `True`。