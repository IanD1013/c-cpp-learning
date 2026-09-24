### 类方法、静态方法与共享类状态

Python 类可以在**类级别**（class level）保存状态——由所有实例共享——并通过三种不同的方法类型公开行为：普通实例方法、`@classmethod` 方法和 `@staticmethod` 方法。这是构建配置管理器、注册表和工厂对象的强大模式，适用于需要集中状态而无需单独实例的场景。

### 工作原理

**类属性**是直接在类体中定义的变量（而不是在 `__init__` 中）。它们属于类本身，因此每个访问 `cls.<attribute>` 的方法都会共享相同的数据。

**`@classmethod`** 接收类（`cls`）作为其第一个参数，而不是实例（`self`）。这使得它可以读取和修改类级别状态，甚至构造新实例（工厂模式）。

**`@staticmethod`** 既不接收 `cls` 也不接收 `self`。它是一个为了组织代码而挂载在类上的普通实用工具函数——它无法访问类状态或实例状态。

### 语法

```python
class MyClass:
    shared_data = []          # class attribute — shared by all

    @classmethod
    def add_item(cls, item):
        cls.shared_data.append(item)

    @classmethod
    def from_list(cls, items):
        cls.shared_data = list(items)   # replace with a new list
        return cls()                    # optionally return a new instance

    @staticmethod
    def is_valid(item):
        return isinstance(item, str) and item != ""
```

### 示例

```python
# Example 1: A counter registry using class methods
class Counter:
    _counts = {}

    @classmethod
    def increment(cls, name):
        cls._counts[name] = cls._counts.get(name, 0) + 1

    @classmethod
    def get_count(cls, name):
        return cls._counts.get(name, 0)

Counter.increment("page_views")
Counter.increment("page_views")
Counter.get_count("page_views")  # 2
Counter.get_count("clicks")      # 0 (default)

# Example 2: Static method as a utility validator
class UserForm:
    @staticmethod
    def is_valid_email(email):
        return isinstance(email, str) and "@" in email

UserForm.is_valid_email("a@b.com")   # True
UserForm.is_valid_email(42)           # False

# Example 3: Factory classmethod loading from external data
class Palette:
    _colors = []

    @classmethod
    def load(cls, color_list):
        cls._colors = list(color_list)   # replace entirely

    @classmethod
    def all_colors(cls):
        return cls._colors.copy()        # return a safe copy

Palette.load(["red", "green"])
Palette.all_colors()  # ["red", "green"] — a copy, not the original
```

### 常见模式

| 模式用途关键细节                                     |           |                        |
| -------------------------------------------- | --------- | ---------------------- |
| `cls._data.clear()` 然后 `cls._data.update(d)` | 就地替换字典内容  | 保留相同的字典对象              |
| `return cls._data.copy()`                    | 安全读取访问    | 防止外部修改                 |
| `isinstance(val, str)`                       | 验证器中的类型检查 | 在 `@staticmethod` 中很有用 |
| `cls._data.get(key, default)`                | 安全的字典查找   | 如果键不存在则返回默认值           |
