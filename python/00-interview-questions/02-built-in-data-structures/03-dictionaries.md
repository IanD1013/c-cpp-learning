`dict`（字典）用于将**键（key）映射到值（value）**。它是 Python 中最常用、最核心的容器之一：关键字参数、对象属性、类似 JSON 的数据以及计数器等，底层都大量依赖字典。

字典中的查找、插入和删除操作，平均时间复杂度都是 **O(1)**。因此，当你需要通过一个**标签（key）**来查找对应的值，而不是通过位置索引来查找时，通常就应该使用字典。

### 创建字典

字典字面量使用花括号 `{}`，其中包含 `key: value` 形式的键值对。

`dict()` 构造函数可以通过关键字参数，或者通过由键值对组成的可迭代对象来创建字典。

```python
a = {"x": 1, "y": 2}
b = dict(x=1, y=2)              # 关键字形式，key 必须是合法的标识符
c = dict([("x", 1), ("y", 2)]) # 从包含 2 元组的列表创建
empty = {}                     # 这是空字典，不是空集合
```

`{}` 永远表示一个空字典。

如果你想创建一个空集合，必须写：

```python
set()
```

还有一种常见方式，是使用两个平行的序列来创建字典：

```python
dict(zip(keys, values))
```

### 访问值：`[]` 与 `get`

使用 `[]` 访问字典时，如果 key 不存在，会抛出 `KeyError`。

而 `get()` 在 key 不存在时不会抛出异常，而是返回 `None`，或者返回你提供的默认值。

```python
d = {"a": 1}

d["a"]            # 1
d["b"]            # KeyError: 'b'

d.get("b")        # None
d.get("b", 0)     # 0，返回提供的默认值
```

`get()` **永远不会向字典中插入任何内容**。

如果你希望：

- 读取某个值
- 如果 key 不存在，则插入一个默认值

可以使用 `setdefault()`。

如果 key 已经存在，它会返回当前值。

如果 key 不存在，它会插入默认值，然后返回这个默认值。

```python
d = {}

d.setdefault("hits", 0)
# 返回 0，并插入 "hits": 0

d.setdefault("hits", 99)
# 返回 0
# 因为 key 已经存在，所以不会覆盖原来的值
```

### 成员测试和视图

对于字典来说，`in` 检查的是 **key，而不是 value**。

由于它使用哈希表进行查找，因此速度很快。

```python
d = {"a": 1, "b": 2}

"a" in d          # True
1 in d            # False，1 是 value，不是 key
1 in d.values()   # True
```

`keys()`、`values()` 和 `items()` 返回的是**视图对象（view objects）**。

视图可以理解为字典的一个**实时窗口**：如果原字典发生变化，视图也会自动反映这些变化。

`items()` 中的每一个元素都是一个 `(key, value)` 元组。

```python
d = {"a": 1}

v = d.keys()

d["b"] = 2

list(v)
# ['a', 'b']
# 因为 view 会随着原字典更新
```

### 插入顺序

从 Python 3.7 开始，字典会保留 key 的**插入顺序**。

遍历字典时，也会按照 key 被插入的顺序返回它们。

如果重新给一个已经存在的 key 赋值，只会更新它的 value，**不会把这个 key 移动到最后**。

但是，如果先删除一个 key，再重新添加它，那么这个 key 会被移动到最后。

```python
d = {"a": 1, "b": 2}

d["a"] = 99
# value 改变，但位置仍然是第一个

list(d)
# ['a', 'b']
```

直接遍历字典时，得到的是 key。

因此：

```python
for k in d:
```

等价于：

```python
for k in d.keys():
```

如果你同时需要 key 和 value，可以使用：

```python
d.items()
```

### 合并字典

从 Python 3.9 开始，可以使用 `|` 运算符合并两个字典，并返回一个新的字典。

`|=` 会直接修改原字典。

`update()` 方法同样会修改左边的字典。

如果两个字典中存在相同的 key，那么**右边字典中的 value 会覆盖左边的 value**。

```python
left = {"a": 1, "b": 2}
right = {"b": 3, "c": 4}

left | right
# {'a': 1, 'b': 3, 'c': 4}
# 对于 'b'，右边的值胜出

left.update(right)
# 直接修改 left，使其变成相同的结果
# update() 返回 None
```

### Key 必须是可哈希的

字典的 key 必须是 **hashable（可哈希）** 的。

这意味着它必须：

- 拥有一个稳定的 hash 值
- 支持相等性比较

不可变的内置类型通常可以作为 key，例如：

```python
str
int
float
bool
tuple   # 前提是 tuple 中的元素本身也都是可哈希的
frozenset
```

可变容器不能作为字典 key，例如：

```python
list
dict
set
```

如果尝试使用它们作为 key，会抛出 `TypeError`。

```python
{(1, 2): "ok"}
# tuple 可以作为 key，没有问题

{[1, 2]: "no"}
# TypeError: unhashable type: 'list'
```

有一个需要注意的陷阱：

```python
True == 1
```

结果是：

```python
True
```

并且：

```python
hash(True) == hash(1)
```

结果也是：

```python
True
```

因此：

```python
{1: "a", True: "b"}
```

最终只会得到一个键值对。

第一个 key 会被保留，而最后一次赋值的 value 会覆盖之前的 value，因此最终结果是：

```python
{1: "b"}
```