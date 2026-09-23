### 从同一个类创建多个对象

面向对象编程中最强大的思想之一是类充当创建对象的**蓝图**。每次调用一个类时，Python 都会创建一个**全新的、独立的对象**，并拥有自己独立的一组属性。修改一个对象对任何其他对象完全没有影响——即使它们是从同一个类创建的也是如此。这使你能够对共享相同结构但保存不同数据的现实世界实体进行建模。

### 工作原理

可以将类想象成饼干模具，将对象想象成饼干。模具定义了形状，但每块饼干都是独立的个体——你可以装饰其中一块而不会改变其他饼干。当你调用 `MyClass(...)` 时，Python 会：

1. 为新对象分配内存
2. 运行 `__init__` 以通过 `self` 初始化该特定对象的属性
3. 返回新对象

以这种方式创建的每个对象都存在于其自己的内存空间中。`__init__` 内部的 `self` 参数始终引用此时正在创建的**特定实例**。

### 语法

```python
class ClassName:
    def __init__(self, param1, param2):
        self.param1 = param1
        self.param2 = param2

# Creating multiple independent objects
obj_a = ClassName("value_a", 10)
obj_b = ClassName("value_b", 20)

# Each object has its own data
print(obj_a.param1)  # "value_a"
print(obj_b.param1)  # "value_b"
```

### 示例

```python
# A simple BankAccount class
class BankAccount:
    def __init__(self, owner, balance):
        self.owner = owner
        self.balance = balance

# Create two separate accounts
account1 = BankAccount("Maya", 500)
account2 = BankAccount("Liam", 1200)

# Modifying one does NOT affect the other
account1.balance -= 100
print(account1.balance)  # 400
print(account2.balance)  # 1200 — completely unaffected

# Comparing attributes across objects
if account2.balance > account1.balance:
    print(f"{account2.owner} has a higher balance")
# Output: Liam has a higher balance
```

```python
# A Book class demonstrating independent instances
class Book:
    def __init__(self, title, pages):
        self.title = title
        self.pages = pages

book_a = Book("Dune", 412)
book_b = Book("1984", 328)
book_c = Book("Hamlet", 104)

# Each object holds its own state
books = [book_a, book_b, book_c]
longest = max(books, key=lambda b: b.pages)
print(longest.title)  # "Dune"
```

### 关键认知：对象是相互独立的

一个常见的误解是从同一个类创建的对象会以某种方式共享数据。事实并非如此。对类构造函数的每次调用都会生成一个完全独立的实例。你可以创建数十个对象，并随心所欲地比较、组合或处理它们的属性——每个对象都独立维护其自身的状态。

```python
class Temperature:
    def __init__(self, city, degrees):
        self.city = city
        self.degrees = degrees

t1 = Temperature("Oslo", -5)
t2 = Temperature("Cairo", 35)

# Objects can be compared via their attributes
if t1.degrees > t2.degrees:
    print(f"{t1.city} is warmer")
else:
    print(f"{t2.city} is warmer")
# Output: Cairo is warmer
```
