# 元类（Metaclasses）—— 创建类的类

在 Python 中，万物皆对象 —— 包括类本身。当你编写 `class Foo:` 时，Python 并不只是执行类主体然后就结束了。它实际上*调用了某种机制*来制造类对象。这个机制就是**元类（metaclass）**。默认的元类是 `type`。理解元类能让你拥有拦截和自定义类创建过程本身的能力 —— 这一控制层级高于装饰器和 `__init_subclass__`。

## 类的真正创建方式

当 Python 遇到类似如下的类声明时：

```python
class Dog:
    species = "Canis familiaris"

    def bark(self):
        return "Woof!"
```

它本质上被转换为：

```python
Dog = type(
    'Dog',
    (object,),
    {
        'species': 'Canis familiaris',
        'bark': bark_function
    }
)
```

调用 `type` 时传入了三个参数：

1. **name** — 类的字符串名称
2. **bases** — 基类的元组
3. **namespace** — 包含类属性和方法的字典

其结果是一个新的类对象。由于 `type` 本身也是一个类（并且也是它自己的元类！），你可以*子类化* `type` 来改变类的构建方式。

## 编写自定义元类

元类继承自 `type` 并重写 `__new__` 和/或 `__init__`：

- `__new__(mcs, name, bases, namespace)` — 在类对象存在*之前*被调用。你可以修改 `namespace`（类字典）并控制创建何种类对象。
- `__init__(cls, name, bases, namespace)` — 在类创建*之后*被调用，用于进一步对其进行配置。

## 语法

```python
class MyMeta(type):
    def __new__(mcs, name, bases, namespace):
        # Modify namespace or validate before class creation
        cls = super().__new__(mcs, name, bases, namespace)
        return cls


# Using the metaclass:
class MyClass(metaclass=MyMeta):
    pass


# Or programmatically:
MyClass = MyMeta('MyClass', (), {})
```

> 注意：按照惯例，`__new__` 的第一个参数通常命名为 `mcs`（metaclass 的缩写）而不是 `cls`，以便与常规类方法区分开来。

## 示例

### 示例 1：强制所有子类定义必需的方法

```python
class InterfaceMeta(type):
    def __new__(mcs, name, bases, namespace):
        # Skip validation for the base class itself 
        # 如果当前正在创建的类有父类，就执行 execute 方法检查；如果没有父类，就跳过检查。这样做的目的是避免检查作为“接口模板”的基础类 Plugin 自己。
        if bases:
            if 'execute' not in namespace:
                raise TypeError(f"{name} must implement 'execute' method")

        return super().__new__(mcs, name, bases, namespace)


class Plugin(metaclass=InterfaceMeta):
    pass


# This works:
class GoodPlugin(Plugin):
    def execute(self):
        return "running"


# This raises TypeError: BadPlugin must implement 'execute' method
# class BadPlugin(Plugin):
#     pass
```

### 示例 2：在全局注册表中自动注册类

```python
registry = {}


class RegisterMeta(type):
    def __new__(mcs, name, bases, namespace):
        cls = super().__new__(mcs, name, bases, namespace)

        if bases:  # don't register the base class itself
            registry[name] = cls

        return cls


class Handler(metaclass=RegisterMeta):
    pass


class EmailHandler(Handler):
    pass


class SMSHandler(Handler):
    pass


print(registry)
# {'EmailHandler': <class 'EmailHandler'>, 'SMSHandler': <class 'SMSHandler'>}
```

### 示例 3：向每个类注入属性

```python
class TimestampMeta(type):
    def __new__(mcs, name, bases, namespace):
        import time

        namespace['_created_at'] = time.time()

        return super().__new__(mcs, name, bases, namespace)


class Record(metaclass=TimestampMeta):
    pass


print(hasattr(Record, '_created_at'))  # True
```

## 需要理解的核心机制

| **概念** | **详情** |
|---|---|
| `type(obj)` | 返回 `obj` 的类 |
| `type(cls)` | 返回 `cls` 的元类 |
| `super().__new__(mcs, name, bases, namespace)` | 创建实际的类对象 |
| `namespace` 字典 | 包含在类主体中定义的所有内容 |
| 检查现有方法 | 检查 `'method_name' in namespace` 以查看类是否*显式*定义了某些内容 |
| `instance.__dict__` | 包含每个实例属性的字典 |

## 动态构建 `__repr__`

一种有用的模式是自动生成 `__repr__` 方法。实例的 `__repr__` 通常显示类名及其属性。你可以从 `instance.__dict__` 构建此字符串：

```python
# If an instance of class "Point" has __dict__ == {'x': 3, 'y': 4}
# A good repr would be: "Point(x=3, y=4)"
# You can get the class name via type(self).__name__
```

## 何时使用元类

元类功能强大但很少是必需的。对于较简单的自定义，应优先使用 `__init_subclass__` 或类装饰器。

在需要执行以下操作时使用元类：

- 在类对象创建*之前*修改类的命名空间
- 控制实际的 `type.__new__` 调用
- 构建类能够自注册或自验证的框架