Python 中的每一个值都可以用于**布尔上下文（boolean context）**，例如：

- `if`
- `while`
- `and` / `or` 表达式

Python 会根据一套简单且可预测的规则，判断一个值应该被视为“真（True）”还是“假（False）”。

这是 Python 面试中很常见的知识点，因为规则本身并不复杂，但有几个地方特别容易让人混淆：

- `and` 和 `or` **不一定返回 `True` / `False`**
- `is` 和 `==` **不是一回事**
- 空列表 `[]` 虽然是一个真实存在的对象，但在布尔上下文中仍然属于 **falsy（假值）**

---

## 什么是 Falsy，什么是 Truthy

如果一个值在布尔上下文中被 Python 当作 `False`，那么这个值就是 **falsy（假值）**。

你需要记住这些常见的 falsy 内置值：

```python
bool(None)      # False
bool(False)     # False
bool(0)         # False
bool(0.0)       # False
bool("")        # False  （空字符串）
bool([])        # False  （空列表）
bool({})        # False  （空字典）
bool(())        # False  （空元组）
bool(set())     # False  （空集合）
```

其他值基本上都属于 **truthy（真值）**，例如：

- 非空容器
- 非零数字
- 非空字符串

即使字符串的内容看起来像 `"0"` 或 `"False"`，它们仍然是 truthy，因为它们**不是空字符串**。

```python
bool("0")       # True   （字符串不是空的）
bool("False")   # True
bool(-1)        # True   （非零）
bool([0])       # True   （列表中有一个元素）
bool(" ")       # True   （空格也是一个字符）
```

正因为如此，在 Python 中通常不会写：

```python
if len(items) > 0:
    ...
```

更符合 Python 风格的写法是：

```python
if items:
    ...
```

---

## `bool()` 与 `int` 的关系

在 Python 中，`bool` 实际上是 `int` 的子类。

因此：

- `True` 等于 `1`
- `False` 等于 `0`

它们甚至可以像数字一样参与算术运算：

```python
True + True     # 2
True == 1       # True
False == 0      # True

sum([True, False, True])  # 2
```

最后这个例子：

```python
sum([True, False, True])
```

实际上相当于：

```python
sum([1, 0, 1])
```

所以结果是：

```python
2
```

这也解释了为什么：

```python
True == 1
```

结果是：

```python
True
```

但是：

```python
True is 1
```

结果是：

```python
False
```

因为它们虽然**值相等**，但并不是**同一个对象**。

---

## `is` 和 `==` 的区别

`==` 问的是：

> “这两个对象的**值是否相等**？”

而 `is` 问的是：

> “这两个变量是否指向**完全同一个对象**？”

例如：

```python
a = [1, 2, 3]
b = [1, 2, 3]

a == b          # True
a is b          # False
```

为什么？

因为：

```python
a == b
```

比较的是两个列表的**内容**。

它们的内容都是：

```python
[1, 2, 3]
```

所以：

```python
a == b   # True
```

但是 `a` 和 `b` 是分别创建出来的两个列表对象，因此：

```python
a is b   # False
```

---

### 判断 `None` 时使用 `is`

有一个非常重要的情况：判断 `None`。

判断一个变量是不是 `None` 时，应该使用：

```python
if value is None:
    ...
```

或者：

```python
if value is not None:
    ...
```

不要写：

```python
if value == None:
    ...
```

虽然很多情况下它也能工作，但一个类可以自定义 `__eq__`，从而影响 `==` 的行为。

因此，判断 `None` 时，标准且符合 Python 风格的写法是：

```python
is None
```

和：

```python
is not None
```

类似地，判断布尔值时通常也不要写：

```python
if flag == True:
    ...
```

而应该直接写：

```python
if flag:
    ...
```

---

## `and`、`or`、`not`

这是最容易让人意外的地方之一。

### `and` 和 `or` 不一定返回 `True` / `False`

`and` 和 `or` 不会简单地把操作数转换成 `True` 或 `False`。

它们会进行**短路求值（short-circuit evaluation）**，然后返回其中一个**原始操作数**。

---

### `a and b`

规则是：

> 如果 `a` 是 falsy，返回 `a`；否则返回 `b`。

例如：

```python
0 and 5
```

因为：

```python
bool(0) == False
```

所以直接返回：

```python
0
```

再例如：

```python
3 and 5
```

因为 `3` 是 truthy，所以继续看第二个值，并返回：

```python
5
```

因此：

```python
0 and 5         # 0
3 and 5         # 5
```

可以简单记成：

```text
a and b

a 是假的 → 返回 a
a 是真的 → 返回 b
```

---

### `a or b`

规则正好相反：

> 如果 `a` 是 truthy，返回 `a`；否则返回 `b`。

例如：

```python
0 or 5
```

`0` 是 falsy，所以返回第二个值：

```python
5
```

而：

```python
3 or 5
```

`3` 已经是 truthy，因此直接返回：

```python
3
```

例如：

```python
0 or 5          # 5
3 or 5          # 3
"" or "default" # "default"
None or 0       # 0
```

最后一个例子：

```python
None or 0
```

虽然 `None` 和 `0` 都是 falsy，但 `or` 在第一个值为 falsy 时会返回第二个操作数，因此最终结果是：

```python
0
```

---

## 短路求值（Short-circuiting）

`and` 和 `or` 还有一个非常重要的特性：

**右边的表达式可能根本不会被执行。**

例如：

```python
def boom():
    raise ValueError
```

如果运行：

```python
False and boom()
```

Python 看到左边：

```python
False
```

已经知道整个 `and` 表达式不可能是 truthy，因此不会继续执行：

```python
boom()
```

所以：

```python
False and boom()   # False
```

`boom()` 根本不会被调用。

类似地：

```python
True or boom()
```

Python 看到左边已经是 truthy，就不需要再执行右边，因此：

```python
True or boom()     # True
```

`boom()` 同样不会被调用。

---

## `not` 一定返回真正的 bool

与 `and` 和 `or` 不同，`not` **一定返回 `True` 或 `False`**。

例如：

```python
not 5           # False
not 0           # True
not []          # True
```

因为：

```python
bool(5)         # True
```

所以：

```python
not 5           # False
```

而：

```python
bool(0)         # False
```

因此：

```python
not 0           # True
```

---

## 使用 `or` 设置默认值

Python 中一个很常见的写法是：

```python
name = user_name or "guest"
```

意思是：

如果：

```python
user_name
```

是 truthy，就使用 `user_name`。

否则使用：

```python
"guest"
```

例如：

```python
user_name = "Ian"

name = user_name or "guest"

print(name)
```

结果：

```text
Ian
```

但是如果：

```python
user_name = ""
```

那么：

```python
name = user_name or "guest"
```

结果就是：

```text
guest
```

### 这里有一个陷阱

`or` 替换的是**所有 falsy 值**，而不仅仅是 `None`。

也就是说：

```python
"" or "guest"      # "guest"
0 or "guest"       # "guest"
None or "guest"    # "guest"
False or "guest"   # "guest"
```

如果你的真正需求是：

> “只有当值是 `None` 时才使用默认值。”

那么应该明确判断：

```python
if user_name is None:
    name = "guest"
else:
    name = user_name
```

---

## 链式比较（Chained Comparisons）

Python 允许你把多个比较操作连接起来。

例如：

```python
x = 5

1 < x < 10
```

结果：

```python
True
```

它表达的就是数学中的：

```text
1 < x < 10
```

也就是：

> `x` 大于 `1`，同时小于 `10`。

---

### Python 如何理解链式比较

表达式：

```python
a < b < c
```

可以理解为：

```python
(a < b) and (b < c)
```

但是有一个重要区别：

**`b` 只会被求值一次。**

它并不是：

```python
(a < b) < c
```

例如：

```python
x = 5

1 < x < 10
```

可以理解为：

```python
1 < x and x < 10
```

两边都是 `True`，所以最终结果是：

```python
True
```

---

## 比较运算符也可以混合使用

Python 的链式比较并不要求所有比较符都一样。

例如：

```python
a = 1

a == a == 1     # True
3 > 2 > 1       # True
1 < 2 > 0       # True
```

最后一个：

```python
1 < 2 > 0
```

可以理解为：

```python
1 < 2 and 2 > 0
```

也就是：

```python
True and True
```

所以结果是：

```python
True
```

---

## 总结

Python 的布尔判断主要需要记住以下规则：

- `None`、`False`、数字 `0`、空字符串和空容器都是 **falsy**
- 非零数字、非空字符串、非空容器通常都是 **truthy**
- `bool` 是 `int` 的子类，因此 `True == 1`、`False == 0`
- `==` 比较的是**值**
- `is` 比较的是**对象身份**
- 判断 `None` 应该使用 `is None` / `is not None`
- `and` 和 `or` 返回的是**操作数本身**，不一定是 `True` / `False`
- `and`：左边 falsy → 返回左边；否则返回右边
- `or`：左边 truthy → 返回左边；否则返回右边
- `and` / `or` 会进行**短路求值**
- `not` 一定返回真正的 `True` 或 `False`
- `a or default` 会替换**所有 falsy 值**，而不仅仅是 `None`
- `1 < x < 10` 是 Python 支持的链式比较，可以理解为 `1 < x and x < 10`