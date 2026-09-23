### 访问和修改实例属性

创建对象后，它的旅程并未结束。实例属性是每个对象内部的动态数据——你可以读取它们、修改它们，甚至在对象创建后添加全新的属性。这就是对象随时间维护和演变其**状态**的方式，这对于在代码中建模现实世界的事物至关重要。

### 工作原理

你在 `__init__` 中使用 `self.attribute_name = value` 设置的每个属性，都可以通过**点号表示法**在对象上进行访问。创建对象后，你可以使用相同的点号表示法来读取或覆盖这些值。修改一个对象上的属性对同一类的其他对象**完全没有影响**——每个对象都维护着自己属性的独立副本。

你甚至可以在 `__init__` 外部为对象附加全新的属性，不过最佳实践是在 `__init__` 内部定义所有属性，以便每个实例都以可预测且一致的数据集开始。

### 语法

```python
# Reading an attribute
value = obj.attribute_name

# Modifying an existing attribute
obj.attribute_name = new_value

# Incrementing a numeric attribute
obj.attribute_name += 1

# Adding a new attribute after creation (works but not recommended)
obj.new_attribute = some_value
```

### 示例

```python
class Car:
    def __init__(self, make, mileage):
        self.make = make
        self.mileage = mileage

my_car = Car("Toyota", 50000)
print(my_car.mileage)       # 50000

# Drive 200 miles
my_car.mileage += 200
print(my_car.mileage)       # 50200

# Another car is completely independent
your_car = Car("Honda", 30000)
print(your_car.mileage)     # 30000 — unaffected by changes to my_car
```

```python
class BankAccount:
    def __init__(self, owner, balance):
        self.owner = owner
        self.balance = balance

account = BankAccount("Dana", 1000.0)
account.balance -= 150.0    # Withdraw
print(account.balance)      # 850.0
account.balance += 500.0    # Deposit
print(account.balance)      # 1350.0
```

### 核心要点

* **读取**：`obj.attr` 返回该属性的当前值。
* **修改**：`obj.attr = new_value` 或 `obj.attr += delta` 会就地更新该属性。
* **独立性**：每个对象分别独立跟踪其自身的属性值。
* **最佳实践**：在 `__init__` 中定义所有属性，使每个实例都以相同的结构开始。
