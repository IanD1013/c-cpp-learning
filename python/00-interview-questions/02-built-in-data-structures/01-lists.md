# Python 列表（List）

列表（`list`）是 Python 中一种**有序（ordered）、可变（mutable）的序列（sequence）**。

列表可以：

- 保存不同类型的数据
- 在原地增加或删除元素
- 保持元素的顺序

当你需要一个**顺序很重要**并且**内容可能发生变化**的容器时，列表通常是最常用的选择。

在面试中，列表也是非常常见的考点。面试官经常通过列表来考察你是否理解：

- **可变性（mutability）**
- **切片（slicing）**
- **复制（copy）**
- **引用 / 别名（aliasing）**
- 「复制一个列表」和「让两个变量指向同一个列表」之间的区别

---

## 创建列表

你可以通过以下几种方式创建列表：

- 列表字面量 `[]`
- `list()` 构造函数
- 使用 `*` 重复元素

```python
a = [1, 2, 3]

b = list("abc")
# ['a', 'b', 'c']

c = list(range(3))
# [0, 1, 2]

d = [0] * 4
# [0, 0, 0, 0]
```

对字符串使用 `list()` 时，会把字符串拆成一个个字符：

```python
list("abc")
# ['a', 'b', 'c']
```

对字典使用 `list()` 时，会得到字典的 **key**：

```python
d = {"name": "Tom", "age": 20}

list(d)
# ['name', 'age']
```

`[x] * n` 会把同一个元素重复 `n` 次：

```python
[0] * 4
# [0, 0, 0, 0]
```

对于 `0` 这种**不可变对象（immutable object）**来说，这样做没有问题。

但是，如果元素本身是**可变对象（mutable object）**，例如列表，就可能产生陷阱。

后面的「`[[]] * n` 共享引用陷阱」会详细说明。

---

## 索引和负数索引

Python 的列表索引从 `0` 开始。

```python
nums = [10, 20, 30, 40]

nums[0]
# 10
```

负数索引表示**从列表末尾开始数**。

其中：

```text
-1 → 最后一个元素
-2 → 倒数第二个元素
-3 → 倒数第三个元素
```

例如：

```python
nums = [10, 20, 30, 40]

nums[0]
# 10

nums[-1]
# 40

nums[-2]
# 30
```

如果直接访问一个不存在的索引，会抛出：

```python
nums[4]
# IndexError: list index out of range
```

也就是说：

> 普通索引超出范围会产生 `IndexError`，但是切片超出范围通常不会报错。

---

## 切片：start、stop、step

列表切片的基本语法是：

```python
lst[start:stop:step]
```

其中：

- `start`：开始位置，**包含**
- `stop`：结束位置，**不包含**
- `step`：步长

可以理解为：

```text
[start, stop)
```

也就是**左闭右开**。

例如：

```python
nums = [0, 1, 2, 3, 4, 5]

nums[1:4]
# [1, 2, 3]

nums[:3]
# [0, 1, 2]

nums[::2]
# [0, 2, 4]

nums[::-1]
# [5, 4, 3, 2, 1, 0]

nums[10:20]
# []
```

如果省略：

```python
start
```

默认从开头开始。

如果省略：

```python
stop
```

默认到结尾。

如果省略：

```python
step
```

默认步长为 `1`。

---

### 使用 `[::-1]` 反转列表

一个非常常见的 Python 写法是：

```python
nums[::-1]
```

例如：

```python
nums = [1, 2, 3, 4]

reversed_nums = nums[::-1]

print(reversed_nums)
# [4, 3, 2, 1]
```

这会得到一个**新的列表**。

因此：

```python
lst[::-1]
```

通常表示：

> 创建一个倒序排列的列表副本。

---

### 切片超出范围不会报错

例如：

```python
nums = [0, 1, 2, 3]

nums[10:20]
# []
```

即使 `10` 和 `20` 都超过了列表范围，也不会产生 `IndexError`。

这和普通索引不同：

```python
nums[10]
# IndexError
```

---

### 使用 `[:]` 复制列表

因为切片会创建一个新的列表，所以：

```python
new_list = old_list[:]
```

是一种常见的**浅拷贝（shallow copy）**方式。

例如：

```python
a = [1, 2, 3]

b = a[:]

print(b)
# [1, 2, 3]
```

此时 `a` 和 `b` 是两个不同的列表对象。

---

## 切片赋值（Slice Assignment）

你不仅可以读取切片，还可以**给切片赋值**。

这样可以直接修改原来的列表。

例如：

```python
x = [1, 2, 3, 4, 5]

x[1:3] = [20, 30, 40]

print(x)
# [1, 20, 30, 40, 4, 5]
```

这里：

```python
x[1:3]
```

原本对应：

```python
[2, 3]
```

现在被：

```python
[20, 30, 40]
```

替换。

需要注意：

> 替换进去的元素数量不需要和原来的切片长度相同。

因此，切片赋值可以让列表：

- 变长
- 变短
- 保持相同长度

---

### 使用切片删除元素

例如：

```python
x = [1, 2, 3, 4, 5]

x[1:3] = []

print(x)
# [1, 4, 5]
```

相当于把索引 `1` 和 `2` 的元素删除。

---

### 带 step 的切片赋值

如果切片包含 `step`：

```python
x[::2] = ...
```

那么左右两边的元素数量必须相同，否则会产生：

```text
ValueError
```

---

## 可变性（Mutability）和列表方法

列表是**可变对象（mutable object）**。

这意味着列表创建之后，可以直接修改自身内容。

例如：

```python
xs = [3, 1, 2]

xs.append(9)

print(xs)
# [3, 1, 2, 9]
```

很多修改列表的方法都是**原地修改（in-place）**。

并且这些方法通常返回：

```python
None
```

例如：

```python
xs = [3, 1, 2]

result = xs.append(9)

print(result)
# None

print(xs)
# [3, 1, 2, 9]
```

---

## 常见列表方法

### `append(x)`

在列表末尾添加**一个元素**：

```python
xs = [1, 2]

xs.append(3)

print(xs)
# [1, 2, 3]
```

---

### `extend(iterable)`

把一个可迭代对象中的**每一个元素**添加到列表末尾：

```python
xs = [1, 2]

xs.extend([3, 4])

print(xs)
# [1, 2, 3, 4]
```

注意 `append()` 和 `extend()` 的区别。

```python
xs = [1, 2]

xs.append([3, 4])

print(xs)
# [1, 2, [3, 4]]
```

而：

```python
xs = [1, 2]

xs.extend([3, 4])

print(xs)
# [1, 2, 3, 4]
```

所以：

```text
append → 把整个东西作为一个元素加入

extend → 把里面的元素一个一个加入
```

---

### `insert(i, x)`

在索引 `i` **之前**插入元素：

```python
xs = [1, 3]

xs.insert(1, 2)

print(xs)
# [1, 2, 3]
```

---

### `pop()`

删除并返回最后一个元素：

```python
xs = [1, 2, 3]

value = xs.pop()

print(value)
# 3

print(xs)
# [1, 2]
```

也可以指定索引：

```python
xs.pop(1)
```

表示删除并返回索引 `1` 的元素。

---

### `remove(x)`

删除**第一个匹配的值**：

```python
xs = [1, 2, 2, 3]

xs.remove(2)

print(xs)
# [1, 2, 3]
```

如果找不到这个值，会产生：

```text
ValueError
```

---

### `index(x)`

返回某个值**第一次出现的位置**：

```python
xs = [10, 20, 30]

xs.index(20)
# 1
```

如果不存在，同样会产生：

```text
ValueError
```

---

### `count(x)`

计算某个值出现了多少次：

```python
xs = [1, 2, 2, 2, 3]

xs.count(2)
# 3
```

---

### `reverse()`

原地反转列表：

```python
xs = [1, 2, 3]

xs.reverse()

print(xs)
# [3, 2, 1]
```

---

### `sort()`

原地排序：

```python
xs = [3, 1, 2]

xs.sort()

print(xs)
# [1, 2, 3]
```

---

## `sort()` 和 `sorted()` 的区别

这是一个非常常见的 Python 面试问题。

### `list.sort()`

`sort()`：

- 修改原列表
- 返回 `None`

例如：

```python
xs = [3, 1, 2]

result = xs.sort()

print(xs)
# [1, 2, 3]

print(result)
# None
```

---

### `sorted()`

`sorted()`：

- 不修改原来的数据
- 创建并返回一个新的排序列表

例如：

```python
xs = [3, 1, 2]

ys = sorted(xs)

print(ys)
# [1, 2, 3]

print(xs)
# [3, 1, 2]
```

两者都支持：

```python
key=
```

和：

```python
reverse=
```

例如：

```python
xs = [3, 1, 2]

xs.sort(reverse=True)

print(xs)
# [3, 2, 1]
```

---

### 常见错误

不要这样写：

```python
xs = xs.sort()
```

因为：

```python
xs.sort()
```

返回的是：

```python
None
```

所以最终：

```python
xs
```

会变成：

```python
None
```

---

## 把列表当作栈（Stack）

Python 列表可以非常方便地作为 **LIFO 栈**使用。

LIFO 表示：

> Last In, First Out  
> 后进先出

使用：

```python
append()
```

进行 push。

使用：

```python
pop()
```

进行 pop。

例如：

```python
stack = []

stack.append(1)
stack.append(2)

stack.pop()
# 2
```

过程可以理解为：

```text
[]

append(1)
↓
[1]

append(2)
↓
[1, 2]

pop()
↓
[1]
```

返回：

```python
2
```

在列表末尾进行 `append()` 和 `pop()` 都非常快。

---

## Copy vs Aliasing：复制和引用

这是列表中非常重要的概念。

```python
b = a
```

**不会复制列表。**

它只是让变量 `b` 也指向 `a` 所指向的那个列表对象。

例如：

```python
a = [1, 2, 3]

b = a

b.append(4)

print(a)
# [1, 2, 3, 4]
```

为什么修改 `b`，`a` 也变了？

因为：

```text
a ─────┐
       ↓
    [1, 2, 3]
       ↑
b ─────┘
```

`a` 和 `b` 指向的是**同一个列表对象**。

所以：

```python
b.append(4)
```

实际上修改的是它们共同指向的那个列表。

---

## 创建独立的列表副本

如果希望创建一个新的列表，可以使用：

```python
b = a[:]
```

或者：

```python
b = a.copy()
```

或者：

```python
b = list(a)
```

例如：

```python
a = [1, 2, 3]

b = a.copy()

b.append(4)

print(a)
# [1, 2, 3]

print(b)
# [1, 2, 3, 4]
```

此时可以理解为：

```text
a ───→ [1, 2, 3]

b ───→ [1, 2, 3, 4]
```

它们是两个不同的列表对象。

---

## 浅拷贝（Shallow Copy）

需要特别注意：

```python
a.copy()
```

```python
a[:]
```

以及：

```python
list(a)
```

都属于**浅拷贝（shallow copy）**。

也就是说：

> 最外层列表是新的，但是里面嵌套的对象仍然可能是共享的。

例如：

```python
a = [[1, 2], [3, 4]]

b = a.copy()
```

最外层：

```text
a 和 b
```

是两个不同的列表。

但是内部的：

```python
[1, 2]
```

和：

```python
[3, 4]
```

仍然是共享对象。

---

## `[[]] * n` 的共享引用陷阱

这是 Python 列表非常经典的陷阱。

例如：

```python
grid = [[0] * 3] * 2
```

看起来你可能认为创建了：

```python
[
    [0, 0, 0],
    [0, 0, 0]
]
```

然后：

```python
grid[0][0] = 9
```

你可能期待：

```python
[
    [9, 0, 0],
    [0, 0, 0]
]
```

但实际结果是：

```python
print(grid)

# [[9, 0, 0], [9, 0, 0]]
```

**两行都发生了变化。**

原因是：

```python
[[0] * 3] * 2
```

并没有创建两个独立的内部列表。

它实际上是让两个位置指向**同一个内部列表对象**。

可以理解成：

```text
grid
 │
 ├─────────┐
 │         │
 ↓         ↓
同一个 [0, 0, 0]
```

因此：

```python
grid[0]
```

和：

```python
grid[1]
```

实际上引用的是同一个列表。

修改：

```python
grid[0][0] = 9
```

相当于修改了这个共享对象，所以两个位置看到的内容都会改变。

---

## 正确创建独立的二维列表

正确做法是每次创建一个新的内部列表。

例如使用循环：

```python
grid = []

for _ in range(2):
    grid.append([0] * 3)
```

这样得到的每一行都是独立对象。

也可以使用列表推导式：

```python
grid = [[0] * 3 for _ in range(2)]
```

现在：

```python
grid[0][0] = 9

print(grid)
# [[9, 0, 0], [0, 0, 0]]
```

此时每一行都是独立的：

```text
grid
 │
 ├──→ [9, 0, 0]
 │
 └──→ [0, 0, 0]
```

使用深拷贝（deep copy）也可以把嵌套对象分开，但对于这种情况，通常最好的做法是：

> **从一开始就创建彼此独立的内部列表。**