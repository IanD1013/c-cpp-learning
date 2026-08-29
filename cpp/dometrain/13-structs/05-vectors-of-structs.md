# C++：`std::vector<struct>`——把一整行数据作为一个元素保存

当几项数据天然属于同一条记录时，通常应该先用 `struct` 把它们组合起来，再使用：

```cpp
std::vector<StructType>
```

保存多条记录。

例如：

```cpp
struct Row {
    std::string city;
    int visits;
};

std::vector<Row> rows;

rows.push_back(Row{"oslo", 3});
rows.push_back(Row{"lima", 8});
```

此时 `rows` 可以理解为：

```text
rows
├── Row{"oslo", 3}
└── Row{"lima", 8}
```

每个 vector 元素都是一个完整的 `Row`。

---

## 1. 为什么使用 `vector<struct>`

如果不用结构体，可能会写成两个平行 vector：

```cpp
std::vector<std::string> cities;
std::vector<int> visits;
```

然后规定：

```text
cities[0]  ↔ visits[0]
cities[1]  ↔ visits[1]
cities[2]  ↔ visits[2]
```

这种设计的问题是：

> 两个 vector 之间的对应关系只是程序员自己约定的。

一旦其中一个 vector 被插入、删除或修改，而另一个没有同步修改，数据就会错位。

例如：

```cpp
cities = {"oslo", "lima"};
visits = {3, 8};
```

逻辑上应该表示：

```text
oslo → 3
lima → 8
```

但如果其中一个 vector 被单独修改：

```cpp
cities.erase(cities.begin());
```

现在：

```text
cities = {"lima"}
visits = {3, 8}
```

原来的对应关系已经被破坏。

更好的设计是：

```cpp
struct Row {
    std::string city;
    int visits;
};

std::vector<Row> rows;
```

这样：

```cpp
Row{"oslo", 3}
```

本身就是一条完整记录。

城市和访问次数始终一起移动。

---

## 2. `push_back` 添加结构体元素

对于：

```cpp
std::vector<Row> rows;
```

可以写：

```cpp
rows.push_back(Row{"oslo", 3});
```

这里可以拆成两步理解：

```cpp
Row temp{"oslo", 3};
rows.push_back(temp);
```

也就是说：

1. `Row{"oslo", 3}` 创建一个 `Row`
2. `push_back(...)` 把它放到 vector 末尾

所以：

```cpp
rows.push_back(Row{"oslo", 3});
rows.push_back(Row{"lima", 8});
```

最后：

```cpp
rows[0].city
```

是：

```cpp
"oslo"
```

而：

```cpp
rows[0].visits
```

是：

```cpp
3
```

---

## 3. 遍历 `vector<struct>`

假设：

```cpp
std::vector<Row> rows;
```

可以这样遍历：

```cpp
for (const Row &row : rows) {
    // 使用 row
}
```

例如：

```cpp
int total = 0;

for (const Row &row : rows) {
    total = total + row.visits;
}
```

如果：

```cpp
rows = {
    Row{"oslo", 3},
    Row{"lima", 8}
};
```

最后：

```cpp
total == 11
```

---

# 4. 为什么推荐 `const Row &row`

推荐：

```cpp
for (const Row &row : rows)
```

而不是：

```cpp
for (Row row : rows)
```

两者最大的区别是：

```cpp
Row row
```

会复制当前元素。

而：

```cpp
const Row &row
```

直接引用 vector 中原来的元素。

---

## 使用值传递：会复制

```cpp
for (Row row : rows) {
    ...
}
```

每轮都会创建一个新的 `Row`。

由于：

```cpp
struct Row {
    std::string city;
    int visits;
};
```

里面还有 `std::string`，所以复制结构体时字符串也需要复制。

如果只是读取数据，这种复制通常没有必要。

---

## 使用 const reference：不复制

```cpp
for (const Row &row : rows) {
    ...
}
```

这里：

* `&`：引用 vector 中原来的元素
* `const`：不允许通过 `row` 修改它

例如可以：

```cpp
std::cout << row.city;
int x = row.visits;
```

但不能：

```cpp
row.visits = 100;
```

因为 `row` 是 `const`。

因此：

> 如果遍历 vector 时只读取元素，通常使用 `const T &`。

通用写法：

```cpp
for (const T &element : container) {
    ...
}
```

例如：

```cpp
for (const Row &row : rows)
```

以及：

```cpp
for (const Item &item : items)
```

---

# 5. 本题：把两个平行 vector 转成 `vector<Item>`

题目给出：

```cpp
const std::vector<std::string> &names
const std::vector<int> &quantities
```

其中：

```text
names[i]
```

和：

```text
quantities[i]
```

共同组成一条记录。

例如：

```cpp
names      = {"apple", "banana", "apple"}
quantities = {3,       5,        2}
```

逻辑上其实是：

```text
apple  → 3
banana → 5
apple  → 2
```

因此可以定义：

```cpp
struct Item {
    std::string name;
    int quantity;
};
```

然后转成：

```cpp
std::vector<Item>
```

得到：

```text
items
├── Item{"apple", 3}
├── Item{"banana", 5}
└── Item{"apple", 2}
```

---

# 6. `totalForName` 的题目规则

函数签名：

```cpp
static int totalForName(
    const std::vector<std::string> &names,
    const std::vector<int> &quantities,
    const std::string &wanted
)
```

它需要计算：

> 所有 `name == wanted` 的 Item 的 quantity 总和。

---

## 规则 1：两个 vector 长度必须相同

必须首先检查：

```cpp
if (names.size() != quantities.size()) {
    return -1;
}
```

因为只有长度相等时：

```cpp
names[i]
```

才能和：

```cpp
quantities[i]
```

组成完整的一行。

例如：

```cpp
names      = {"apple", "banana"}
quantities = {3}
```

第二个名字没有对应数量，因此输入无效。

返回：

```cpp
-1
```

---

## 规则 2：任何 quantity < 0 都属于无效输入

例如：

```cpp
quantities = {3, -2, 5}
```

存在负数，所以应该：

```cpp
return -1;
```

注意：

> 必须检查整个输入。

即使负数对应的名字不是 `wanted`，依然应该返回 `-1`。

例如：

```cpp
names      = {"apple", "banana"}
quantities = {3, -5}
wanted     = "apple"
```

虽然只查 `"apple"`，仍然返回：

```cpp
-1
```

因为输入本身非法。

---

# 7. 为什么 `-1` 可以作为错误值

合法 quantity 都满足：

```cpp
quantity >= 0
```

因此合法总和一定：

```cpp
total >= 0
```

所以：

```cpp
-1
```

永远不会和一个正常计算结果冲突。

这是一种常见设计：

```text
合法结果：0 或正数
错误结果：-1
```

---

# 8. 必须创建 `std::vector<Item>`

题目明确要求：

> Build a `std::vector<Item>` and fill it using `push_back`.

因此不能直接：

```cpp
for (size_t i = 0; i < names.size(); ++i) {
    if (names[i] == wanted) {
        total += quantities[i];
    }
}
```

虽然这种代码逻辑上能够得到答案，但没有完成题目要求练习的核心内容。

应该先：

```cpp
std::vector<Item> items;
```

然后：

```cpp
items.push_back(Item{names[i], quantities[i]});
```

---

# 9. 构造 `vector<Item>`

典型写法：

```cpp
std::vector<Item> items;

for (std::size_t i = 0; i < names.size(); ++i) {
    items.push_back(Item{names[i], quantities[i]});
}
```

这里：

```cpp
Item{names[i], quantities[i]}
```

按照 struct 成员的声明顺序初始化。

例如：

```cpp
struct Item {
    std::string name;
    int quantity;
};
```

那么：

```cpp
Item{"apple", 3}
```

等价于：

```text
name     = "apple"
quantity = 3
```

---

# 10. 再遍历 `vector<Item>` 计算答案

题目要求使用：

```cpp
const Item &item
```

所以：

```cpp
int total = 0;

for (const Item &item : items) {
    if (item.name == wanted) {
        total = total + item.quantity;
    }
}
```

注意：

> 同一个名字可能出现多次。

例如：

```cpp
items = {
    Item{"apple", 3},
    Item{"banana", 5},
    Item{"apple", 2}
};
```

如果：

```cpp
wanted == "apple"
```

结果应该：

```cpp
3 + 2 = 5
```

不能找到第一个以后就停止。

因此不能：

```cpp
if (item.name == wanted) {
    return item.quantity;
}
```

而应该一直累加。

---

# 11. 找不到名字时为什么返回 0

初始化：

```cpp
int total = 0;
```

如果没有任何元素满足：

```cpp
item.name == wanted
```

那么这一句永远不会执行：

```cpp
total = total + item.quantity;
```

所以最后自然：

```cpp
return 0;
```

这正是正确答案。

例如：

```cpp
names      = {"apple", "banana"}
quantities = {3, 5}
wanted     = "orange"
```

结果：

```cpp
0
```

---

# 12. 空 vector 也返回 0

如果：

```cpp
names = {}
quantities = {}
```

两个长度相同，并且没有负数。

所以输入是合法的。

构造出的：

```cpp
items
```

也是空的。

之后：

```cpp
int total = 0;
```

循环执行 0 次，最终：

```cpp
return 0;
```

因此：

```text
空输入 → 0
```

而不是：

```text
空输入 → -1
```

因为 `-1` 表示的是：

> 输入非法

而空输入只是：

> 没有任何数据需要累加。

---

# 13. 需要特别注意的边界情况

### 情况 1：长度不同

```cpp
names      = {"a", "b"}
quantities = {1}
```

结果：

```cpp
-1
```

---

### 情况 2：存在负数

```cpp
names      = {"a", "b"}
quantities = {1, -2}
```

结果：

```cpp
-1
```

---

### 情况 3：名字重复

```cpp
names      = {"a", "b", "a"}
quantities = {2, 5, 7}
wanted     = "a"
```

结果：

```cpp
2 + 7 = 9
```

---

### 情况 4：没有匹配

```cpp
names      = {"a", "b"}
quantities = {2, 5}
wanted     = "c"
```

结果：

```cpp
0
```

---

### 情况 5：空输入

```cpp
names      = {}
quantities = {}
wanted     = "anything"
```

结果：

```cpp
0
```

---

### 情况 6：空字符串也是正常名字

```cpp
names      = {"", "apple", ""}
quantities = {2, 5, 3}
wanted     = ""
```

结果：

```cpp
2 + 3 = 5
```

不要把空字符串自动当成错误输入。

---

# 14. 核心知识总结

本题真正练习的是下面这套模式：

```cpp
struct Item {
    ...
};

std::vector<Item> items;

for (...) {
    items.push_back(Item{...});
}

for (const Item &item : items) {
    ...
}
```

可以记成：

```text
多列相关数据
    ↓
struct 组成一行
    ↓
vector<struct> 保存多行
    ↓
const Struct & 遍历
```

最重要的几个原则：

```text
struct              → 把相关字段组成一条记录
vector<struct>      → 保存很多条记录
push_back           → 添加一条完整记录
const T &           → 只读遍历且避免复制
size 不一致         → 无法组成完整记录
total 从 0 开始     → 没有匹配自然得到 0
-1                  → 表示不可能与合法结果冲突的错误值
```
