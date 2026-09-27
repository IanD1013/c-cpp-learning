### 向枚举添加方法和属性

Python 的枚举（enum）是完备的类（full-fledged classes），这意味着你可以向它们添加实例方法、属性、类方法和静态方法——就像对任何其他类一样。这非常强大，因为它允许你将行为直接附加到枚举成员上，将相关逻辑封装在一起，而不是分散在整个代码库中。

### 工作原理

当你在枚举类中定义一个方法时，该枚举的每个成员都可以调用它。`self` 参数指向具体的枚举成员。你还可以使用 `@property` 创建计算属性，使其使用起来就像普通的属性访问一样；使用 `@classmethod` 创建用于操作枚举类本身的备用构造函数或查找方法。

### 语法

```python
from enum import Enum

class MyEnum(Enum):
    MEMBER_A = "value_a"
    MEMBER_B = "value_b"

    # Regular instance method — self is the enum member
    def some_method(self):
        return f"I am {self.name} with value {self.value}"

    # Property — accessed like an attribute, no parentheses needed
    @property
    def computed_attribute(self):
        return len(self.value)

    # Class method — cls is the enum class, useful for lookups
    @classmethod
    def find_by_criteria(cls, criteria):
        for member in cls:
            if member.value == criteria:
                return member
        raise ValueError(f"No match for {criteria}")
```

### 示例

```python
from enum import Enum

class Season(Enum):
    SPRING = 1
    SUMMER = 2
    AUTUMN = 3
    WINTER = 4

    def is_warm(self):
        return self in (Season.SPRING, Season.SUMMER)

    @property
    def next_season(self):
        members = list(Season)
        idx = members.index(self)
        return members[(idx + 1) % len(members)]

    @classmethod
    def from_month(cls, month):
        mapping = {
            (3, 4, 5): cls.SPRING,
            (6, 7, 8): cls.SUMMER,
            (9, 10, 11): cls.AUTUMN,
            (12, 1, 2): cls.WINTER,
        }
        for months, season in mapping.items():
            if month in months:
                return season
        raise ValueError(f"Invalid month: {month}")

# Instance method
Season.SUMMER.is_warm()          # True
Season.WINTER.is_warm()          # False

# Property — no parentheses
Season.AUTUMN.next_season        # Season.WINTER
Season.WINTER.next_season        # Season.SPRING

# Class method — called on the class
Season.from_month(7)             # Season.SUMMER
```

```python
class HttpStatus(Enum):
    OK = 200
    NOT_FOUND = 404
    SERVER_ERROR = 500

    @property
    def is_error(self):
        return self.value >= 400

    @classmethod
    def from_code(cls, code):
        for member in cls:
            if member.value == code:
                return member
        raise ValueError(f"Unknown status code: {code}")

HttpStatus.OK.is_error             # False
HttpStatus.NOT_FOUND.is_error      # True
HttpStatus.from_code(500)          # HttpStatus.SERVER_ERROR
```

### 常见模式

- **在方法内使用映射字典**：当某个方法需要将成员关联到其他成员（例如相反方向或配对）时，可以定义一个将 `self` 映射到结果的字典。
- **遍历成员**：使用 `for member in cls`（在 classmethod 中）或 `for member in ClassName` 遍历所有枚举成员——这在过滤或搜索时非常有用。
- **访问 `.name` 和 `.value`**：每个枚举成员都具有 `.name`（字符串名称）和 `.value`（分配的值）。可以将它们用于查找和输出。