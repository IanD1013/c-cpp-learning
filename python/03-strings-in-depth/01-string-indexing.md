# Python 中的字符串索引

Python 中的字符串是**字符序列**。字符串中的每个字符都位于一个被称为**索引**（index）的编号位置上。

理解索引可以让你从任意字符串中提取特定字符——这在解析数据、验证输入，或用现有字符串的某些部分构建新字符串时非常有用。

## 工作原理

Python 使用**基于零的索引**（zero-based indexing），这意味着：

* 第一个字符位于位置 `0`
* 第二个字符位于位置 `1`
* 第三个字符位于位置 `2`
* 依此类推

Python 还支持**负索引**（negative indexing），即从字符串末尾倒序计数：

* `-1` 是最后一个字符
* `-2` 是倒数第二个字符
* `-3` 是倒数第三个字符
* 依此类推

你可以通过在字符串或变量名后面的方括号 `[]` 中填写索引来访问字符。

## 语法

```python
# Positive indexing (from the start)
character = some_string[index]

# Negative indexing (from the end)
character = some_string[-index]

# Getting the length of a string
length = len(some_string)
```

## 示例

```python
city = "Tokyo"

# Positive indexing
city[0]    # "T" — first character
city[1]    # "o" — second character
city[4]    # "o" — fifth character (last one)

# Negative indexing
city[-1]   # "o" — last character
city[-2]   # "y" — second to last
city[-5]   # "T" — same as city[0]

# Using len()
len(city)  # 5

# Building a new string from indexed characters
greet = "Morning"
result = greet[0] + greet[3]  # "Mn" — first character and fourth character
```

## 正向与负向索引参考

对于字符串 `"Tokyo"`：

| 字符   |    T |    o |    k |    y |    o |
| ---- | ---: | ---: | ---: | ---: | ---: |
| 正向索引 |  `0` |  `1` |  `2` |  `3` |  `4` |
| 负向索引 | `-5` | `-4` | `-3` | `-2` | `-1` |

请注意：

```python
string[0]
```

和

```python
string[-len(string)]
```

始终指向**第一个字符**。

而：

```python
string[len(string) - 1]
```

和

```python
string[-1]
```

始终指向**最后一个字符**。
