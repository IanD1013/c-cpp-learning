### 冻结数据类 — 使用 `@dataclass(frozen=True)` 实现不可变数据

在实际应用中，通常需要创建在初始化后永远不应被修改的数据对象 — 例如配置记录、坐标点、缓存键或事件标识符。Python 的 `@dataclass` 装饰器通过 `frozen=True` 参数提供了对该特性的支持，从而生成行为类似于不可变值（immutable values）的实例。

## 工作原理

当向 `@dataclass` 传递 `frozen=True` 时，Python 会在类上生成特殊的 `__setattr__` 和 `__delattr__` 方法。当有代码试图在对象构造完成后修改或删除任何属性时，这些方法会抛出 `FrozenInstanceError`（`AttributeError` 的子类）。

由于该对象保证不会被修改，Python 还可以安全地生成 `__hash__` 方法 — 这意味着冻结数据类实例可以用作字典键并存储在集合中。

常规（可变）数据类默认是**不可哈希**的，因为它们的值可以被修改，这会破坏哈希值必须保持稳定的契约。

## 语法

```python
from dataclasses import dataclass

@dataclass(frozen=True)
class ClassName:
    field1: Type
    field2: Type
```

## 示例

```python
from dataclasses import dataclass

# A frozen Color dataclass
@dataclass(frozen=True)
class Color:
    r: int
    g: int
    b: int

red = Color(255, 0, 0)
print(red.r)        # 255

# Attempting mutation raises FrozenInstanceError
try:
    red.r = 128
except Exception as e:
    print(type(e).__name__)  # FrozenInstanceError

# Frozen instances are hashable — usable as dict keys
color_names = {
    Color(255, 0, 0): "red",
    Color(0, 0, 255): "blue"
}

print(color_names[red])  # "red"

# Frozen instances can be added to sets
palette = {
    Color(255, 0, 0),
    Color(0, 255, 0)
}

print(Color(255, 0, 0) in palette)  # True
```

```python
# Checking hashability safely with try/except
@dataclass(frozen=True)
class Point:
    label: str
    value: int

p = Point("origin", 0)

try:
    h = hash(p)
    print(f"Hash: {h}")  # succeeds for frozen dataclasses
except TypeError:
    print("Not hashable")
```

## 常见模式

| **操作** | **冻结数据类** | **常规数据类** |
| :--- | :--- | :--- |
| 读取属性 | ✅ 支持 | ✅ 支持 |
| 设置属性 | ❌ 抛出 `FrozenInstanceError` | ✅ 支持 |
| 删除属性 | ❌ 抛出 `FrozenInstanceError` | ✅ 支持 |
| `hash()` | ✅ 支持 | ❌ 抛出 `TypeError` |
| 用作字典键 | ✅ 支持 | ❌ 不允许 |
| 添加到 `set` | ✅ 支持 | ❌ 不允许 |

测试哈希性或不可变性等属性的常见模式是将操作包裹在 `try`/`except` 块中，并根据是否发生异常返回布尔值。