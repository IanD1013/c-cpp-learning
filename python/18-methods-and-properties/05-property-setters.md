### 使用 `@name.setter` 的属性设置器（Property Setters）

你已经知道如何使用 `@property` 来创建 getter —— 即在*读取*属性时运行的方法。**属性 setter** 是与之对应的部分：它会在你为该属性*赋值*时运行自定义逻辑。这对于强制执行关于对象允许持有什么值的规则至关重要。

### 工作原理

当你定义了一个 setter 时，Python 会拦截赋值操作。你的 setter 方法会优先运行，而不是直接存储该值，从而让你有机会验证、转换或拒绝传入的值。如果该值不符合你的标准，你可以直接跳过更新内部属性，保持其不变。

Setter 是使用装饰器 `@property_name.setter` 定义的，其中 `property_name` 与你的 `@property` getter 名称相匹配。该方法本身也使用相同的名称。

### 语法

```python
class MyClass:
    def __init__(self, value):
        self._value = some_default    # Initialize the internal attribute first
        self.value = value            # This triggers the setter

    @property
    def value(self):
        return self._value

    @value.setter
    def value(self, new_value):
        # Custom logic here — validate, transform, or reject
        if some_condition(new_value):
            self._value = new_value
        # If condition fails, self._value stays unchanged
```

### 示例

```python
# Example 1: A Temperature class that rejects values below absolute zero
class Temperature:
    def __init__(self, celsius):
        self._celsius = 0
        self.celsius = celsius

    @property
    def celsius(self):
        return self._celsius

    @celsius.setter
    def celsius(self, value):
        if value >= -273.15:
            self._celsius = value

t = Temperature(25)
print(t.celsius)    # 25
t.celsius = -300
print(t.celsius)    # 25 (unchanged — rejected by setter)


# Example 2: A Product class that ensures price is positive
class Product:
    def __init__(self, name, price):
        self.name = name
        self._price = 0.0
        self.price = price

    @property
    def price(self):
        return self._price

    @price.setter
    def price(self, value):
        if isinstance(value, (int, float)) and value > 0:
            self._price = value

p = Product("Widget", 9.99)
print(p.price)      # 9.99
p.price = -5
print(p.price)      # 9.99 (unchanged — negative rejected)
```

### 关键模式：在 Setter 之前初始化

注意在上面的两个示例中，内部属性（例如 `self._celsius` 或 `self._price`）都是在 `__init__` 中调用 setter **之前**被初始化为默认值的。这很重要，因为 setter 可能会拒绝初始值，如果 `self._value` 尚不存在，稍后读取它将引发 `AttributeError`。

```python
def __init__(self, value):
    self._value = default   # Step 1: Set a safe default
    self.value = value       # Step 2: Trigger setter (may or may not update)
```
