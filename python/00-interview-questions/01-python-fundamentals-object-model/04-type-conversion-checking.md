Python 是一种**强类型（strongly typed）**但同时也是**动态类型（dynamically typed）**的语言。

- **强类型**意味着 Python 不会悄悄地把字符串和整数加在一起。
- **动态类型**意味着一个变量名可以指向任何类型的对象，并且你可以在运行时检查或转换类型。

面试官在这方面通常会考察两个能力：

1. 如何主动把一种类型转换成另一种类型，以及知道什么时候转换会失败。
2. 如何正确地检查一个对象是什么类型。

本节会介绍这两个主题，并初步了解 Python 中非常重要的概念——**鸭子类型（Duck Typing）**。

---

## 使用内置构造函数进行显式类型转换

Python 的内置类型名称本身也可以作为**类型转换函数**使用。

例如：

`int()`、`float()`、`str()`、`bool()`、`list()`、`tuple()`、`set()` 和 `dict()` 都可以根据传入的参数创建对应类型的新对象。

```python
int("42")       # 42
int(3.9)        # 3   （向 0 截断，不是四舍五入）
int("3.9")      # ValueError：int() 无法直接解析浮点数字符串
float("3.14")   # 3.14
str(42)         # "42"
list("abc")     # ['a', 'b', 'c']   遍历字符串中的字符
tuple([1, 2])   # (1, 2)
set([1, 1, 2])  # {1, 2}            去除重复元素
```

对一个 `float` 使用 `int()` 时，会**向 0 截断（truncate toward zero）**，而不是进行四舍五入。

例如：

```python
int(3.9)
# 3

int(-3.9)
# -3
```

而：

```python
int("3.9")
```

会抛出异常，因为字符串 `"3.9"` 并不是一个合法的整数字面量。

虽然：

```python
int(3.9)
```

可以正常工作，但这里传入的是一个真正的 `float` 对象，而不是字符串。

### 一个常见陷阱：创建 dict

创建 `dict` 时，需要提供**键值对（key/value pairs）**，而不是一个普通的扁平序列。

```python
dict([("a", 1), ("b", 2)])
# {'a': 1, 'b': 2}

dict([1, 2, 3])
# TypeError：无法转换成 dict
```

---

## 类型转换什么时候会抛出异常？

当文本不是对应类型的合法数字时，`int()` 和 `float()` 会抛出 `ValueError`。

如果传入的对象类型本身就不适合进行这种转换，则通常会抛出 `TypeError`。

```python
int("x")         # ValueError
int("")          # ValueError
float("abc")     # ValueError
int(None)        # TypeError
int("  10  ")    # 10   前后的空格是允许的
int("0x1A", 16)  # 26   可以通过 base 参数解析其他进制
```

例如：

```python
int("x")
```

这里 `"x"` 是字符串，但是它无法被解释成整数，因此是：

```text
ValueError
```

而：

```python
int(None)
```

`None` 这种类型根本不适合转换成整数，因此是：

```text
TypeError
```

### `bool()` 几乎不会抛出异常

`bool()` 会根据 Python 的**真值规则（truthiness）**判断一个对象是真还是假。

```python
bool("")         # False
bool("False")    # True
bool(0)          # False
bool([])         # False
```

这里非常容易出现一个陷阱：

```python
bool("False")
# True
```

因为 `"False"` 是一个**非空字符串**。

Python 并不会理解这个字符串里面写的是英文单词 `"False"`。

对于 `bool()` 来说：

```python
""          # 空字符串 → False
"False"     # 非空字符串 → True
"hello"     # 非空字符串 → True
"0"         # 非空字符串 → True
```

---

## `str()` vs `repr()`

`str()` 用来生成一个**方便最终用户阅读的字符串**。

`repr()` 用来生成一个**更加明确、主要面向开发者的字符串表示形式**，通常希望这个表示能够清楚地展示对象本身。

对于很多对象来说，`str()` 和 `repr()` 的结果看起来一样，但字符串是一个很好的例外。

```python
str("hi")
# 'hi'

repr("hi")
# "'hi'"
```

`repr()` 会把字符串本身的引号也表示出来。

再看一个更明显的例子：

```python
print(str("a\nb"))
```

输出：

```text
a
b
```

因为 `\n` 被解释成了换行。

但是：

```python
print(repr("a\nb"))
```

输出：

```text
'a\nb'
```

这里 `repr()` 会把 `\n` 显示出来，而不是直接把它表现成换行。

### `print()` 使用 `str()`

`print()` 默认使用对象的 `str()` 表示。

而 Python 交互式环境以及容器内部展示对象时，经常会使用 `repr()`。

这也是为什么：

```python
print(["a", "b"])
```

显示：

```python
['a', 'b']
```

列表中的字符串会带有引号。

---

## `type()` vs `isinstance()`

`type(x)` 返回对象 `x` 的**确切类型（exact class）**。

而：

```python
isinstance(x, C)
```

检查的是：

> `x` 是否是 `C` 的实例，或者是否是 `C` 的某个子类的实例。

通常进行类型检查时，更推荐使用 `isinstance()`，因为它能够正确处理**继承关系**。

```python
isinstance(True, int)   # True   bool 是 int 的子类

type(True) is int       # False  True 的确切类型是 bool

type(True) is bool      # True

isinstance(3, (int, float))
# True
```

这里有一个非常经典的 Python 面试陷阱：

```python
isinstance(True, int)
# True
```

原因是：

```python
bool
```

实际上是：

```python
int
```

的子类。

所以：

```python
isinstance(True, int)
```

会返回：

```python
True
```

但是：

```python
type(True) is int
```

返回：

```python
False
```

因为 `True` 的**确切类型**是：

```python
bool
```

因此：

```python
type(True) is bool
# True
```

只有当你**真的需要检查确切类型，并且明确想排除子类**时，才应该使用：

```python
type(x) is int
```

一般情况下，更推荐：

```python
isinstance(x, int)
```

### `isinstance()` 可以接受多个类型

`isinstance()` 的第二个参数可以是一个**类型组成的 tuple**。

例如：

```python
isinstance(3, (int, float))
# True
```

意思是：

> `3` 是不是 `int` 或者 `float`？

只要匹配其中任何一种类型，就会返回 `True`。

注意这里需要使用 **tuple**，而不是 list。

```python
isinstance(x, (int, float))   # 正确
```

---

## 隐式数值类型提升

通常我们会主动进行类型转换，但在混合数值运算中，Python 也会自动进行一定程度的**数值类型提升（numeric promotion）**。

当 `int` 和 `float` 一起参与运算时，结果通常会变成 `float`。

```python
1 + 2.0
# 3.0
```

因为：

```text
int + float → float
```

另外，真正除法 `/` **总是返回 `float`**，即使两个操作数都是整数，并且能够整除。

```python
4 / 2
# 2.0
```

而：

```python
4 // 2
# 2
```

如果两个操作数都是整数，整数的地板除法 `//` 结果仍然是整数。

例如：

```python
type(3 * 2.0)
# <class 'float'>
```

还有一个和前面 `bool` 相关的特殊情况：

```python
True + 1
# 2
```

因为 `bool` 是 `int` 的子类。

在数值运算中：

```python
True  == 1
False == 0
```

因此：

```python
True + 1
```

相当于：

```python
1 + 1
```

结果就是：

```python
2
```

---

## 鸭子类型：关注行为，而不是类型

Python 中有一个非常重要的思想叫做**鸭子类型（Duck Typing）**。

它来自一句经典的话：

> "If it walks like a duck and quacks like a duck, treat it as a duck."

意思是：

> 如果它走路像鸭子，叫起来也像鸭子，那就把它当成鸭子。

换成 Python 的思维就是：

> 与其先检查一个对象到底是什么类型，不如直接尝试使用它。只要它支持我们需要的操作，就可以使用它。

例如：

```python
def total(items):
    # 适用于任何包含数字的可迭代对象：
    # list、tuple、set、generator 等
    return sum(items)
```

我们可以传入 list：

```python
total([1, 2, 3])
# 6
```

也可以传入 tuple：

```python
total((1, 2, 3))
# 6
```

甚至可以传入 generator：

```python
total(x for x in range(4))
# 6
```

这个函数从来没有检查：

```python
isinstance(items, list)
```

它并不关心：

> "`items` 到底是不是一个 list？"

它真正关心的是：

> "`items` 能不能被迭代，并且里面的元素能不能被 `sum()` 相加？"

只要答案是可以，那么这个对象就可以正常使用。

这就是**鸭子类型（Duck Typing）**带来的灵活性。

Pythonic 的代码通常不会习惯性地对每个参数都先进行类型检查。

当你确实需要根据不同类型执行不同逻辑时，再使用 `isinstance()`。

例如，一个函数允许用户传入：

```python
"hello"
```

或者：

```python
["hello", "world"]
```

这时候可能确实需要：

```python
if isinstance(value, str):
    ...
else:
    ...
```

所以可以简单理解为：

```text
鸭子类型：
不要先问“你是什么类型？”

而是先问：
“你能不能做我需要你做的事情？”
```
