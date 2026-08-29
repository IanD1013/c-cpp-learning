# C++ `std::map`：使用 `count()` 和 `find()` 查询键

## 1. `count(key)`：判断 key 是否存在

对于 `std::map`：

```cpp
map.count(key)
```

只会返回：

* `1`：key 存在
* `0`：key 不存在

例如：

```cpp
#include <map>
#include <string>

std::map<std::string, int> counts;

counts["ana"] = 10;

if (counts.count("ana") == 1) {
    // "ana" 存在
}
```

原因是：

> `std::map` 中每个 key 最多只能出现一次。

所以：

```cpp
counts.count("ana")
```

永远不可能得到 `2`、`3` 等结果。

---

## 2. `count()` 不会修改 map

这是 `count()` 很重要的特点。

例如：

```cpp
std::map<std::string, int> counts;

counts["ana"] = 10;

int before = counts.size();

counts.count("bo");

int after = counts.size();
```

此时：

```cpp
before == after
```

因为 `count()` 只是检查，并不会插入 `"bo"`。

---

## 3. 不要用 `map[key] != 0` 判断 key 是否存在

例如：

```cpp
if (counts["bo"] != 0) {
    ...
}
```

这并不是一个好的“是否存在”检查。

### 问题一：`operator[]` 会自动插入不存在的 key

假设：

```cpp
std::map<std::string, int> counts;
```

然后执行：

```cpp
counts["bo"];
```

即使 `"bo"` 原本不存在，它也会被自动插入，并得到 `int` 的默认值：

```cpp
0
```

因此：

```cpp
counts.size()
```

也会发生变化。

---

### 问题二：合法的值可能本来就是 0

例如：

```cpp
counts["ana"] = 0;
```

此时：

```cpp
counts["ana"] != 0
```

为 `false`。

但 `"ana"` 明明存在。

所以：

```cpp
map[key] != 0
```

无法正确区分：

```text
key 不存在
```

和：

```text
key 存在，但 value == 0
```

正确做法应该使用：

```cpp
count()
```

或者：

```cpp
find()
```

---

# 4. `find(key)`：同时寻找 key 和 value

如果不仅想知道 key 是否存在，还想取得对应的 value，可以使用：

```cpp
map.find(key)
```

例如：

```cpp
auto found = counts.find("ana");
```

`find()` 返回的是一个 **iterator（迭代器）**。

---

## 5. 使用 `end()` 判断有没有找到

标准写法：

```cpp
auto found = counts.find("ana");

if (found == counts.end()) {
    // 没找到
} else {
    // 找到了
}
```

可以理解成：

```text
find(key)
    ↓
找到       → 返回指向该元素的 iterator
没找到     → 返回 end()
```

因此：

```cpp
found == counts.end()
```

表示：

> 这个 key 不存在。

而：

```cpp
found != counts.end()
```

表示：

> 这个 key 存在。

---

# 6. iterator 中的 `first` 和 `second`

`std::map` 中的每个元素可以近似理解为：

```cpp
std::pair<Key, Value>
```

例如：

```cpp
std::map<std::string, int> counts;

counts["ana"] = 10;
```

其中这一项可以理解为：

```cpp
("ana", 10)
```

因此：

```cpp
found->first
```

得到：

```cpp
"ana"
```

而：

```cpp
found->second
```

得到：

```cpp
10
```

即：

```text
first  = key
second = value
```

---

# 7. 为什么使用 `->` 而不是 `.`？

错误：

```cpp
found.second
```

正确：

```cpp
found->second
```

因为：

```cpp
found
```

本身并不是 map 中的元素，而是一个 **iterator**。

它指向一个 map 元素。

所以可以把：

```cpp
found->second
```

大致理解为：

```cpp
(*found).second
```

也就是：

1. `*found`：取得 iterator 指向的元素
2. `.second`：取得该元素的 value

因此：

```cpp
found->second
```

实际上就是：

```cpp
(*found).second
```

---

# 8. `count()` 和 `find()` 应该怎么选？

如果只想检查：

> 这个 key 存不存在？

可以使用：

```cpp
if (m.count(key) == 1) {
    ...
}
```

如果还需要 value：

```cpp
auto it = m.find(key);

if (it != m.end()) {
    int value = it->second;
}
```

通常不要这样写：

```cpp
if (m.count(key)) {
    int value = m[key];
}
```

虽然逻辑上可以工作，但这相当于进行了两次查找。

更自然的写法是：

```cpp
auto it = m.find(key);

if (it != m.end()) {
    int value = it->second;
}
```

---

# 9. 核心记忆

```cpp
m.count(key)
```

用于：

```text
检查 key 是否存在
```

返回：

```text
0 或 1
```

不会修改 map。

---

```cpp
m.find(key)
```

用于：

```text
寻找 key，并取得对应元素
```

判断是否找到：

```cpp
it == m.end()     // 没找到
it != m.end()     // 找到了
```

取得 key：

```cpp
it->first
```

取得 value：

```cpp
it->second
```

---

## 最常见模板

### 只检查存在性

```cpp
if (m.count(key) == 1) {
    // key exists
}
```

### 检查并取得 value

```cpp
auto it = m.find(key);

if (it != m.end()) {
    int value = it->second;
}
```

### 不推荐

```cpp
if (m[key] != 0) {
    ...
}
```

因为：

1. 不存在的 key 会被自动插入；
2. value 为 `0` 时无法判断 key 是否真的存在。
