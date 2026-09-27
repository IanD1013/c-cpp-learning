### `Enum` 类

Python 的 `enum` 模块提供了 `Enum` 基类，用于创建枚举（enumeration）——即绑定到唯一常量值的一组符号名称。枚举使代码更具可读性，并且比在代码库中随处散落魔法数字（magic numbers）或字符串更不容易出错。与其编写 `status = 3` 并期望自己能记住 `3` 代表什么，不如编写 `status = Status.ACTIVE`。

### 工作原理

`Enum` 子类将成员定义为具有赋值的类属性。每个成员都是枚举类的单例（singleton）实例，这意味着内存中恰好只有一个 `Color.RED` 对象。成员有两个关键属性：`.name`（你赋予它的字符串名称）和 `.value`（你赋予它的值）。因为成员是单例，所以使用 `is` 进行同一性比较是可靠有效的。

### 语法

```python
from enum import Enum

class Direction(Enum):
    NORTH = 1
    SOUTH = 2
    EAST = 3
    WEST = 4
```

### 访问成员

有三种方法可以获取枚举成员：

```python
# 通过属性访问
Direction.NORTH          # <Direction.NORTH: 1>

# 按名称（方括号表示法）——当名称为字符串变量时非常有用
Direction['SOUTH']       # <Direction.SOUTH: 2>

# 按值（调用表示法）——从值反向查找成员
Direction(3)             # <Direction.EAST: 3>
```

### 成员属性与遍历

```python
# .name 返回字符串名称，.value 返回赋予的值
member = Direction.WEST
print(member.name)       # "WEST"
print(member.value)      # 4

# 按定义顺序遍历所有成员
for d in Direction:
    print(d.name, d.value)
# NORTH 1
# SOUTH 2
# EAST 3
# WEST 4

# 将所有成员名称收集到一个列表中
all_names = [d.name for d in Direction]  # ['NORTH', 'SOUTH', 'EAST', 'WEST']
```

### 单例同一性

枚举成员是单例。无论你通过哪种方式访问它们，得到的始终是同一个对象：

```python
a = Direction.NORTH
b = Direction['NORTH']
c = Direction(1)

print(a is b)   # True
print(b is c)   # True
```

这意味着对于枚举成员，`is` 比较可以正常工作，这与常规类实例不同。

### 别名与 `@unique`

如果给两个名称赋相同的值，第二个名称就会成为第一个名称的别名。别名在遍历时不会出现。如果你想完全防止重复值的出现，可以应用 `@unique` 装饰器：

```python
from enum import Enum, unique

@unique
class Priority(Enum):
    LOW = 1
    MEDIUM = 2
    HIGH = 3
    # CRITICAL = 3  # 使用 @unique 会引发 ValueError
```