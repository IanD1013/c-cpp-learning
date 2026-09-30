## Python 描述符：`__get__`、`__set__` 和 `__delete__` 协议

描述符（Descriptor）是 Python 中最强大却又最常被误解的特性之一。**描述符**是指定义了 `__get__`、`__set__` 或 `__delete__` 中至少一个方法的任意对象。

当描述符被赋值为**类属性**时，Python 会拦截对实例上该属性的访问，并将其委托给描述符的方法，而不是进行常规的属性查找。

这就是 `property`、`classmethod`、`staticmethod` 背后的机制，甚至常规方法的工作原理也是如此。



## 描述符的工作原理

当你访问：

```python
obj.attr
```

Python 不仅仅会在 `obj.__dict__` 中查找。它首先会检查**类**（或其 MRO）中的 `attr` 是否是一个描述符。

如果是：

- 在**读取**属性（`obj.attr`）时调用 **`__get__(self, obj, objtype)`**
- 在**写入**属性（`obj.attr = value`）时调用 **`__set__(self, obj, value)`**
- 在**删除**属性（`del obj.attr`）时调用 **`__delete__(self, obj)`**

描述符分为两类：

- **数据描述符（Data descriptors）**：定义了 `__set__` 和/或 `__delete__` —— 它们的优先级**高于**实例的 `__dict__`
- **非数据描述符（Non-data descriptors）**：仅定义了 `__get__`（例如常规函数）—— 实例的 `__dict__` 可以覆盖它们


## `__set_name__` 钩子

Python 3.6+ 添加了：

```python
__set_name__(self, owner, name)
```

它会在类创建时被自动调用。

它会告知描述符其被分配到的属性名称，从而无需手动传入名称。

### 语法

```python
class MyDescriptor:
    def __set_name__(self, owner, name):
        # owner = the class that owns this descriptor
        # name = the attribute name it was assigned to
        self.storage_name = '_' + name

    def __get__(self, obj, objtype=None):
        if obj is None:
            return self  # accessed from the class, not an instance
        return getattr(obj, self.storage_name, None)

    def __set__(self, obj, value):
        # validate or transform before storing
        setattr(obj, self.storage_name, value)
```


## 示例

### 示例 1：强制执行字符串类型的描述符

```python
class StringField:
    def __set_name__(self, owner, name):
        self.storage_name = '_' + name

    def __get__(self, obj, objtype=None):
        if obj is None:
            return self
        return getattr(obj, self.storage_name, '')

    def __set__(self, obj, value):
        if not isinstance(value, str):
            raise TypeError(f"Expected str, got {type(value).__name__}")
        setattr(obj, self.storage_name, value)


class User:
    name = StringField()


u = User()

u.name = "Alice"      # works fine
print(u.name)         # "Alice"

u.name = 42           # raises TypeError
```

### 示例 2：将值限制在特定范围内的描述符

```python
class Clamped:
    def __init__(self, min_val, max_val):
        self.min_val = min_val
        self.max_val = max_val

    def __set_name__(self, owner, name):
        self.storage_name = '_' + name

    def __get__(self, obj, objtype=None):
        if obj is None:
            return self
        return getattr(obj, self.storage_name, self.min_val)

    def __set__(self, obj, value):
        clamped = max(self.min_val, min(self.max_val, value))
        setattr(obj, self.storage_name, clamped)


class Thermostat:
    temperature = Clamped(60, 80)


t = Thermostat()

t.temperature = 95     # clamped to 80
print(t.temperature)   # 80
```

---

## 检查描述符

你可以通过直接查看**类的 `__dict__`**（绕过描述符协议）并检查其类型定义了哪些方法来判断某个类属性是否为描述符：

```python
descriptor_obj = User.__dict__['name']

type(descriptor_obj).__name__                 # 'StringField'

hasattr(type(descriptor_obj), '__get__')      # True
hasattr(type(descriptor_obj), '__set__')      # True
```

> 注意：请在描述符的**类型（type）**上使用 `hasattr`，而不是描述符实例本身，因为 `__get__` 和 `__set__` 是在其类型上查找的。

---

## 描述符在哪里存储数据

一种常见的模式是将实际值使用私有属性名（如 `_balance`）存储在**实例**上。

`__set_name__` 钩子为你提供了原始属性名，以便你可以推导出存储名称。

使用：

```python
setattr(obj, self.storage_name, value)
```

和：

```python
getattr(obj, self.storage_name)
```

可以保持每个实例的数据相互独立。