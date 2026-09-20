### 原地修改列表

由于列表在 Python 中是可变（mutable）的，你可以在不创建全新列表的情况下更改它们的内容。这非常强大且高效——尤其是在处理大型数据集时，复制列表会浪费内存。修改列表有三种核心方法：通过索引更改元素、追加到末尾以及在特定位置插入。

### 工作原理

当你原地修改列表时，内存中的原始列表对象会被直接更新。不会创建新的列表。这意味着指向同一个列表的任何其他变量也会看到这一更改。

**通过索引赋值**会替换该位置上的元素：

```python
colors = ["red", "green", "blue"]
colors[1] = "yellow"
# colors is now ["red", "yellow", "blue"]
```

**追加（Appending）**会将单个值添加到列表末尾：

```python
scores = [88, 92]
scores.append(75)
# scores is now [88, 92, 75]
```

**插入（Inserting）**会将值放置在指定索引处，并将其后的所有内容向右移动一位：

```python
letters = ["a", "c", "d"]
letters.insert(1, "b")
# letters is now ["a", "b", "c", "d"]
```

### 添加多个元素

要一次性添加多个元素，可以使用 `extend()` 或 `+=` 运算符：

```python
fruits = ["apple"]
fruits.extend(["banana", "cherry"])
# fruits is now ["apple", "banana", "cherry"]

nums = [1, 2]
nums += [3, 4]
# nums is now [1, 2, 3, 4]
```

这两种方式都会原地修改原始列表——它们**不会**创建新列表。

### 原地修改与创建新列表

一个关键区别：上述所有操作都会更改**同一个**列表对象。将此与使用 `+` 进行拼接对比，后者会创建一个**新**列表：

```python
a = [1, 2]
b = a + [3]   # b is a NEW list [1, 2, 3]; a is still [1, 2]

c = [1, 2]
c.append(3)   # c is modified IN PLACE to [1, 2, 3]
```

### 使用循环计算累加总和

你可以使用循环和累加器变量从列表中计算值：

```python
prices = [4.50, 3.25, 7.00]
total = 0
for price in prices:
    total += price
# total is now 14.75
```
