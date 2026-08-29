# C++ `std::map` 基础：键值对、`operator[]` 与默认插入

## 1. `std::map` 是什么？

`std::map` 用来保存一组 **键值对（key-value pairs）**。

基本写法：

```cpp
#include <map>

std::map<KeyType, ValueType> name;
```

尖括号中的两个类型依次表示：

```text
std::map<键类型, 值类型>
```

例如：

```cpp
#include <map>
#include <string>

std::map<std::string, int> counts;
```

表示：

* key 是 `std::string`
* value 是 `int`

可以把它理解成：

```text
"ana" -> 30
"bo"  -> 20
```

---

## 2. 一个 key 只能对应一个 value

例如：

```cpp
std::map<std::string, int> counts;

counts["ana"] = 10;
counts["bo"] = 20;
counts["ana"] = 30;
```

执行过程：

```text
第一次：
"ana" -> 10

第二次：
"ana" -> 10
"bo"  -> 20

第三次：
"ana" -> 30
"bo"  -> 20
```

因为 `"ana"` 已经存在，所以：

```cpp
counts["ana"] = 30;
```

不会创建第二个 `"ana"`，而是覆盖原来的值。

最终：

```cpp
counts.size() == 2
```

因此要记住：

> `map.size()` 统计的是不同 key 的数量，而不是执行过多少次赋值。

---

## 3. `map[key]` 不只是读取

`std::map` 中一个非常重要的行为是：

```cpp
map[key]
```

如果 key 已经存在：

```cpp
int x = counts["ana"];
```

直接返回对应的 value。

但如果 key **不存在**：

```cpp
int x = counts["zed"];
```

`std::map` 会自动：

1. 创建 `"zed"` 这个 key
2. 给它一个默认构造出来的 value
3. 返回这个 value 的引用

对于：

```cpp
std::map<std::string, int>
```

`int` 的默认值是：

```cpp
0
```

所以：

```cpp
counts["zed"];
```

等价于产生：

```text
"zed" -> 0
```

---

## 4. “读取”可能改变 map

假设：

```cpp
std::map<std::string, int> counts;

counts["ana"] = 30;
counts["bo"] = 20;
```

此时：

```cpp
counts.size() == 2
```

然后：

```cpp
int value = counts["zed"];
```

因为 `"zed"` 不存在，所以它会被插入：

```text
"ana" -> 30
"bo"  -> 20
"zed" -> 0
```

现在：

```cpp
value == 0
counts.size() == 3
```

这是 `std::map::operator[]` 最容易踩坑的地方：

> `map[key]` 对不存在的 key 进行“读取”时，也会修改 map。

---

## 5. 不同 value 类型的默认值

当 `map[key]` 创建一个不存在的 key 时，其 value 会进行默认初始化。

例如：

```cpp
std::map<std::string, int> a;
a["x"];
```

得到：

```text
"x" -> 0
```

而：

```cpp
std::map<std::string, std::string> b;
b["x"];
```

得到：

```text
"x" -> ""
```

也就是空字符串。

---

## 6. 本题最重要的执行顺序

题目要求：

1. 根据 `keys[i]` 和 `values[i]` 建立 map
2. 读取 `probe`
3. 返回：

   * probe 后 map 的大小
   * probe 对应的值

这里顺序非常重要。

应该：

```cpp
int probedValue = m[probe];
int entries = m.size();
```

而不能先：

```cpp
int entries = m.size();
int probedValue = m[probe];
```

因为如果 `probe` 不存在：

```cpp
m[probe]
```

会新增一个 entry。

例如最开始：

```text
"a" -> 10
"b" -> 20
```

大小是：

```text
2
```

如果：

```cpp
probe == "c"
```

执行：

```cpp
int value = m["c"];
```

以后变成：

```text
"a" -> 10
"b" -> 20
"c" -> 0
```

所以最终答案必须是：

```text
[3, 0]
```

而不是：

```text
[2, 0]
```

---

## 7. 重复 key 的处理

如果输入为：

```cpp
keys   = {"ana", "bo", "ana"};
values = {10, 20, 30};
```

依次执行：

```cpp
m[keys[i]] = values[i];
```

得到：

```text
"ana" -> 30
"bo"  -> 20
```

因此后出现的 value 会覆盖前面的 value。

---

## 8. 输入长度不一致

题目规定：

```cpp
keys.size() != values.size()
```

时应立即返回空的 `std::vector<int>`：

```cpp
return {};
```

例如：

```cpp
keys   = {"a", "b"};
values = {10};
```

不能正确组成 key-value pair，因此直接返回：

```cpp
{}
```

---

## 9. 空输入也是合法输入

例如：

```cpp
keys   = {};
values = {};
probe  = "abc";
```

一开始 map 是空的：

```text
size = 0
```

然后执行：

```cpp
int value = m["abc"];
```

由于 `"abc"` 不存在，会自动插入：

```text
"abc" -> 0
```

所以：

```cpp
m.size() == 1
value == 0
```

最终答案：

```cpp
{1, 0}
```

---

# 核心知识总结

```cpp
std::map<Key, Value>
```

表示：

```text
key -> value
```

其中：

* 一个 key 最多只有一个 entry
* 对同一个 key 再次赋值会覆盖旧 value
* `map.size()` 返回不同 key 的数量
* `map[key]`：

  * key 存在：返回对应 value
  * key 不存在：自动创建 key，并插入默认 value
* 因此 `map[key]` 可能改变 map 的大小

本题尤其要记住：

```cpp
int value = m[probe];   // 先 probe，因为可能插入新 key
int size = m.size();    // 再获取最终大小
```
