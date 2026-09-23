# 类：对象的蓝图

到目前为止，你一直使用函数来组织代码，并将数据存储在变量、列表和字典中。但随着程序规模的扩大，你经常会发现自己需要将同一组相关联的值传递给许多函数——这里传递一个姓名，那里传递一个年龄，再传递一个余额。**类（class）**允许你将相关联的数据和行为打包成一个名为**对象（object）**的可复用包。你可以把类想象成饼干模具，而每个对象就是用它印出的饼干。

## 运作原理

当你定义一个类时，你是在描述一个对象将包含*什么数据*以及它可以执行*什么操作*。你可以像调用函数一样调用类来创建一个新对象——接着 Python 会：

1. 在内存中创建一个全新的空对象。
2. 在该对象上调用名为 `__init__` 的特殊方法，并传入你提供的所有参数。
3. 将完全初始化后的对象返回给你。

`self` 参数是每个方法引用其所属特定对象的方式。Python 会自动传递它——在调用方法时你永远不需要手动提供它。

## 语法

```python
class ClassName:
    def __init__(self, param1, param2):
        self.attribute1 = param1
        self.attribute2 = param2

# Creating an object (instance)
obj = ClassName(value1, value2)

# Accessing attributes
print(obj.attribute1)
print(obj.attribute2)
```

## 示例

```python
# A simple Dog class
class Dog:
    def __init__(self, breed, weight):
        self.breed = breed
        self.weight = weight

rex = Dog("Labrador", 30)
print(rex.breed)    # Labrador
print(rex.weight)   # 30

# A Book class with three attributes
class Book:
    def __init__(self, title, author, pages):
        self.title = title
        self.author = author
        self.pages = pages

novel = Book("Dune", "Frank Herbert", 412)
print(f"{novel.title} by {novel.author}")  # Dune by Frank Herbert

# You can create as many objects from one class as you like
textbook = Book("Clean Code", "Robert Martin", 464)
print(textbook.pages)  # 464
```

请注意每个对象是如何保留其各自独立的属性副本的——修改 `rex.weight` 不会影响任何其他 `Dog` 对象。

## 核心要点

| **概念描述**     |                           |
| ------------ | ------------------------- |
| `class` 关键字  | 开始类的定义                    |
| `__init__`   | 创建对象时自动调用的特殊方法            |
| `self`       | 引用当前对象；始终作为方法的第一个参数       |
| `self.x = y` | 在对象上将 `y` 存储为名为 `x` 的实例属性 |
| `obj.x`      | 访问对象 `obj` 上的属性 `x`       |
