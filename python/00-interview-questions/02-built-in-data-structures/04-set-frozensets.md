`set`（集合）是一种**无序、元素唯一，并且元素必须可哈希（hashable）**的集合类型。

如果你发现自己经常在问：

- “这个值之前出现过吗？”
- “这两个集合里有哪些共同的元素？”
- “如何快速去重？”

那么 `set` 通常就是非常合适的工具。

在 Python 面试中，`set` 也是常见考点，因为它可以考察你是否理解：

- **唯一性（uniqueness）**
- **成员检查（membership）的性能**
- 什么叫做 **可哈希（hashable）**

### 创建集合（Creating Sets）

可以使用花括号 `{}` 或 `set()` 构造函数创建集合。

```python
a = {1, 2, 3}
b = set([1, 2, 2, 3])   # {1, 2, 3}，重复元素会被删除
```

一个非常经典的坑：

`{}` 表示的是**空字典（dict）**，而不是空集合。

如果要创建一个空集合，必须使用 `set()`。

```python
type({})        # <class 'dict'>
type(set())     # <class 'set'>
```

`set()` 可以接受任何**可迭代对象（iterable）**。

例如：

```python
set("hello")
```

结果类似于：

```python
{'h', 'e', 'l', 'o'}
```

这是因为 `set()` 会遍历字符串中的每个字符，同时删除重复元素。

字符串 `"hello"` 中有两个 `l`，所以最终集合中只会保留一个 `l`。

### 唯一性与去重（Uniqueness and Deduplication）

一个 `set` 中永远不会保存两个相等的元素。

因此，把一个 `list` 转换成 `set` 是一种非常简单的**去重方法**。

```python
unique = set([3, 1, 2, 3, 1])   # {1, 2, 3}，顺序不保证
back = list(unique)              # 再转换回 list
```

需要注意：

**集合是无序的，因此不要依赖集合中的元素顺序。**

如果你希望：

> 去除重复元素，同时保留原来的顺序

通常可以使用：

```python
list(dict.fromkeys(nums))
```

这是一个常见的一行写法，因为 Python 的 `dict` 会保留插入顺序。

### 集合运算（Set Operations）

集合支持我们在数学集合中常见的各种运算。

```python
a = {1, 2, 3}
b = {2, 3, 4}

a | b   # 并集 union
        # {1, 2, 3, 4}

a & b   # 交集 intersection
        # {2, 3}

a - b   # 差集 difference
        # {1}

a ^ b   # 对称差集 symmetric difference
        # {1, 4}
        # 只存在于其中一个集合中，但不能同时存在于两个集合中
```

简单来说：

```text
|    两边所有元素
&    两边共同元素
-    左边有、右边没有
^    只在其中一边出现
```

这些运算也都有对应的方法形式，例如：

```python
a.union(b)
a.intersection(b)
a.difference(b)
a.symmetric_difference(b)
```

方法形式和运算符形式之间有一个重要区别：

**方法形式可以接受任何 iterable，而运算符形式要求两边都是集合。**

例如：

```python
a.union([5, 6])
```

这是可以的。

但是：

```python
a | [5, 6]
```

会抛出：

```python
TypeError
```

### 子集与超集（Subset and Superset）

`<=` 用于检查**子集（subset）**。

`>=` 用于检查**超集（superset）**。

而 `<` 和 `>` 用于检查严格的子集/超集，也就是**真子集（proper subset）**和**真超集（proper superset）**。

```python
{1, 2} <= {1, 2, 3}    # True，属于子集

{1, 2} < {1, 2}        # False
                        # 两个集合完全相等，所以不是真子集

{1, 2, 3} >= {1, 2}    # True，属于超集
```

另外：

```python
a.isdisjoint(b)
```

用于判断两个集合是否**完全没有共同元素**。

如果两个集合没有任何共同元素，则返回：

```python
True
```

### 添加和删除元素（Adding and Removing）

```python
s = {1, 2, 3}

s.add(4)        # {1, 2, 3, 4}

s.discard(9)    # 如果 9 不存在，也不会报错

s.remove(9)     # 如果 9 不存在，会抛出 KeyError

s.pop()         # 删除并返回一个任意元素
```

`discard()` 和 `remove()` 的区别是一个非常经典的 Python 面试题。

如果元素不存在：

```python
s.discard(value)
```

不会报错。

而：

```python
s.remove(value)
```

会抛出：

```python
KeyError
```

### 成员检查的性能（Membership Performance）

对于集合：

```python
x in s
```

平均时间复杂度是：

```text
O(1)
```

这是因为集合会对元素进行哈希（hash），然后直接定位到对应的位置。

相比之下，在 `list` 中进行成员检查：

```python
x in some_list
```

时间复杂度是：

```text
O(n)
```

因为 Python 可能需要从头到尾逐个检查元素。

例如：

```python
big = list(range(1_000_000))
big_set = set(big)

999_999 in big        # O(n)，较慢，需要扫描 list
999_999 in big_set    # 平均 O(1)，很快
```

因此，如果你需要对一个很大的集合进行**大量重复查询**，`set` 通常会比 `list` 快很多。

### 集合元素必须是可哈希的（Hashable Elements Only）

集合中的每一个元素都必须是：

**可哈希（hashable）的。**

常见的不可变内置类型通常可以哈希，例如：

```text
int
str
tuple（前提是里面的元素也都可哈希）
frozenset
```

而可变对象通常不可哈希，例如：

```text
list
dict
set
```

因此：

```python
{[1, 2]}
```

会抛出：

```python
TypeError: unhashable type: 'list'
```

但是：

```python
{(1, 2)}
```

是完全合法的，因为这里的 `tuple` 是可哈希的。

这也解释了为什么一个普通的 `set` 不能直接包含另外一个 `set`。

例如：

```python
{{1, 2}, {3, 4}}
```

是不允许的，因为内部的 `set` 本身是可变的，因此不可哈希。

但是可以使用 `frozenset`。

### Frozenset

`frozenset` 可以理解为：

**不可变版本的 `set`。**

因为 `frozenset` 是不可变的，所以它是**可哈希的**。

因此，它可以：

- 放到另一个 `set` 中
- 作为 `dict` 的 key

例如：

```python
fs = frozenset([1, 2, 3])
```

由于它不可变，所以不能调用：

```python
fs.add(4)
```

否则会抛出：

```python
AttributeError
```

但是可以这样：

```python
nested = {
    fs,
    frozenset([4, 5])
}
```

也就是说：

```python
# set of frozensets
nested = {
    frozenset([1, 2, 3]),
    frozenset([4, 5])
}
```

这是合法的。

`frozenset` 也可以作为字典的 key：

```python
matrix = {
    frozenset(["a", "b"]): 1
}
```

`frozenset` 支持所有**只读的集合操作**，例如：

```python
|
&
in
issubset()
```

但是它不支持修改自身的操作，例如：

```python
add()
remove()
```

当你需要：

- 把一个集合本身作为 `dict` 的 key
- 把一个集合作为另一个 `set` 的成员
- 创建一个不能被其他调用者修改的集合

这时候就可以考虑使用 `frozenset`。