# Python 元组（Tuple）

元组（tuple）是一种**有序（ordered）、不可变（immutable）的序列**。

在 Python 中，元组主要通过**逗号**来创建，圆括号 `()` 通常是可选的。

元组在 Python 中非常常见，例如：

- 函数返回多个值
- 作为字典的键（dictionary key）
- 表示小型、固定的数据记录

面试中也经常考察元组，因为它可以很好地检验你是否理解：

- **不可变性（immutability）**
- **打包（packing）**
- **解包（unpacking）**
- Python 简洁的赋值语法

---

## 创建元组以及尾随逗号陷阱

真正创建元组的是**逗号 `,`，而不是圆括号 `()`**。

```python
point = (3, 4)      # 包含两个元素的元组
also = 3, 4         # 同样是元组，圆括号可以省略

empty = ()          # 空元组

one = (5,)          # 只有一个元素的元组，注意最后的逗号

not_a_tuple = (5)   # 这只是整数 5，并不是元组
```

单元素元组是一个非常常见的陷阱。

```python
(5)
```

实际上等价于：

```python
5
```

因为这里的圆括号只是用来**分组（grouping）**，并不会创建元组。

如果想创建只有一个元素的元组，必须加上尾随逗号：

```python
(5,)
```

因此：

```python
a = (5)
b = (5,)

print(type(a))
# <class 'int'>

print(type(b))
# <class 'tuple'>
```

空元组是一个特殊情况：

```python
empty = ()
```

因为没有元素，也就没有逗号可以写，所以必须使用 `()`。

---

## 不可变性（Immutability）

元组一旦创建，就**不能修改**。

例如：

```python
t = (1, 2, 3)

t[0] = 99
```

会报错：

```text
TypeError: 'tuple' object does not support item assignment
```

也就是说，元组没有列表中的这些修改操作：

```python
append()
pop()
```

也不能：

```python
t[0] = ...
```

元组本身只有两个常用方法：

```python
count()
index()
```

例如：

```python
t = (10, 20, 20, 30)

print(t.count(20))
# 2

print(t.index(30))
# 3
```

元组的不可变性也是为什么元组可以作为：

- 字典的 key
- set 中的元素

而列表通常不可以。

不过需要注意一个细节：

**元组本身不可变，但如果元组里面包含一个可变对象，那么这个可变对象内部仍然可以发生变化。**

例如：

```python
t = ([1, 2], 3)

t[0].append(4)

print(t)
# ([1, 2, 4], 3)
```

这里并没有把：

```python
t[0]
```

替换成另一个对象。

只是修改了 `t[0]` 指向的那个 list 内部的内容。

简单理解就是：

> 元组中的元素引用是固定的，但如果某个元素本身是可变对象，那么那个对象内部仍然可以变化。

---

## 打包和解包（Packing & Unpacking）

### Packing：打包

把多个值收集到一个元组中，称为**打包（packing）**。

```python
packed = 1, 2, 3
```

实际上：

```python
packed
```

就是：

```python
(1, 2, 3)
```

---

### Unpacking：解包

解包就是把一个序列中的元素分别赋值给多个变量。

```python
packed = 1, 2, 3

a, b, c = packed
```

结果：

```python
a == 1
b == 2
c == 3
```

也可以显式写圆括号：

```python
x, y = (10, 20)
```

结果：

```python
x == 10
y == 20
```

而且解包并不是元组专属的。

任何可迭代对象都可以参与解包，例如 list：

```python
a, b = [4, 5]
```

结果：

```python
a == 4
b == 5
```

---

## 解包数量必须匹配

普通解包时，左边变量的数量必须和右边元素的数量匹配。

例如：

```python
a, b = (1, 2, 3)
```

左边只有两个变量：

```python
a, b
```

右边却有三个值：

```python
1, 2, 3
```

因此会报错：

```text
ValueError: too many values to unpack (expected 2)
```

---

## 星号解包（Star Unpacking）

如果你不知道中间有多少个元素，可以使用 `*`。

带 `*` 的变量会把多出来的元素全部收集起来。

例如：

```python
first, *rest = [1, 2, 3, 4]
```

结果：

```python
first == 1
rest == [2, 3, 4]
```

也可以让前面的变量收集多个元素：

```python
*head, last = [1, 2, 3, 4]
```

结果：

```python
head == [1, 2, 3]
last == 4
```

甚至可以放在中间：

```python
a, *mid, b = [1, 2, 3, 4, 5]
```

结果：

```python
a == 1
mid == [2, 3, 4]
b == 5
```

需要注意：

**一次解包最多只能有一个 `*` 目标。**

---

### 星号变量得到的一定是 list

即使你解包的是 tuple：

```python
a, *rest = (1, 2, 3, 4)
```

`rest` 仍然是：

```python
[2, 3, 4]
```

而不是：

```python
(2, 3, 4)
```

也就是说：

> `*` 解包收集到的结果始终是 `list`。

而且这个 list 可以为空：

```python
a, *rest = [9]
```

结果：

```python
a == 9
rest == []
```

这不会报错。

---

## 不使用临时变量交换两个值

Python 可以非常方便地交换两个变量：

```python
a, b = 1, 2

a, b = b, a
```

执行之后：

```python
a == 2
b == 1
```

不需要传统写法中的临时变量：

```python
temp = a
a = b
b = temp
```

这是因为 Python 会先计算整个右侧，然后再进行左侧赋值。

例如：

```python
a, b = b, a + b
```

右边的：

```python
b
a + b
```

都会使用**赋值之前的旧 `a` 和旧 `b`**。

不会出现 `a` 已经更新了一半，然后 `a + b` 又使用新值的问题。

---

## 元组作为字典的 Key

如果一个元组中的所有元素都是不可变、可哈希（hashable）的，那么这个元组也可以作为：

- dictionary key
- set element

例如，可以使用 `(x, y)` 坐标作为字典的 key：

```python
grid = {}

grid[(0, 0)] = "start"
grid[(1, 2)] = "wall"

print(grid[(1, 2)])
# wall
```

这种方式非常适合表示组合键，例如：

```text
(x, y)
(latitude, longitude)
(row, column)
(first_name, last_name)
```

而 list 不能作为字典 key：

```python
d = {}

d[[1, 2]] = "hello"
```

会报错：

```text
TypeError: unhashable type: 'list'
```

同样需要注意：

**如果 tuple 里面包含 list，那么这个 tuple 也不能作为字典 key。**

例如：

```python
key = ([1, 2], 3)
```

因为里面的：

```python
[1, 2]
```

是不可哈希的，所以整个 tuple 也不可哈希。

也就是说，想让 tuple 可以被 hash，它内部的元素也必须满足 hashability 的要求。

---

## Tuple vs List

什么时候应该使用 tuple？

当数据是一个**固定的记录（fixed record）**，并且不同位置具有不同含义时，可以考虑 tuple。

例如坐标：

```python
(x, y)
```

或者函数返回：

```python
(value, error)
```

这里不同位置代表不同含义：

```text
第一个位置 → value
第二个位置 → error
```

这种数据通常不会不断增加或删除元素，因此 tuple 很合适。

---

什么时候应该使用 list？

如果数据是：

- 会增长的
- 会缩小的
- 会修改的
- 一组同类型的数据

通常使用 list。

例如：

```python
scores = [88, 92, 75, 100]
```

之后可能：

```python
scores.append(95)
scores.remove(75)
```

因此 list 更合适。

可以简单理解：

```text
Tuple → 固定记录
List  → 可变化的集合
```

Tuple 本身也向代码阅读者传达了一种意图：

> “这些数据组成一个固定结构，我不打算修改它。”

这也是使用 tuple 的重要原因之一。

---

## namedtuple：更容易阅读的记录

Python 的：

```python
collections.namedtuple
```

可以创建一种特殊的 tuple。

它仍然具有 tuple 的特性：

- immutable
- 可以通过索引访问
- 可以 unpack
- 可以作为 tuple 使用

但它额外允许你通过**字段名称**访问数据。

例如：

```python
from collections import namedtuple

Point = namedtuple("Point", ["x", "y"])

p = Point(3, 4)
```

现在可以使用：

```python
print(p.x, p.y)
# 3 4
```

同时仍然可以像普通 tuple 一样使用索引：

```python
print(p[0])
# 3
```

也可以解包：

```python
x, y = p
```

打印时也更加容易理解：

```python
print(p)
# Point(x=3, y=4)
```

相比普通 tuple：

```python
p = (3, 4)

print(p[0])
print(p[1])
```

使用 `namedtuple`：

```python
print(p.x)
print(p.y)
```

代码的含义更加清楚。

---

### namedtuple 仍然是不可变的

虽然可以通过：

```python
p.x
```

访问字段，但并不意味着可以修改它。

例如：

```python
p.x = 9
```

会报：

```text
AttributeError
```

因为 `namedtuple` 仍然是 immutable 的。

---

## 什么时候使用 namedtuple？

当一个普通 tuple 开始变得难以理解时，可以考虑使用 `namedtuple`。

例如：

```python
user = ("Ian", 30, "Wellington")
```

过一段时间之后，你可能忘记：

```python
user[0]
user[1]
user[2]
```

分别代表什么。

这时可以使用：

```python
from collections import namedtuple

User = namedtuple("User", ["name", "age", "city"])

user = User("Ian", 30, "Wellington")
```

然后：

```python
user.name
user.age
user.city
```

代码就会更加容易阅读。

因此可以简单理解：

```text
tuple
    ↓
适合简单、固定的数据

namedtuple
    ↓
仍然是 tuple
但给每个位置增加了名字
让代码更容易阅读
```