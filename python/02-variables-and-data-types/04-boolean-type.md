# Python 布尔类型 `bool`：`True`、`False` 与比较运算

## 1. 什么是布尔类型

Python 中的布尔类型（Boolean，`bool`）用于表示一个判断的结果。

布尔值只有两个：

```python
True
False
```

可以把它们理解为：

* `True`：是、成立、正确
* `False`：否、不成立、错误

例如：

```python
print(10 > 5)
```

输出：

```text
True
```

因为 `10 > 5` 这个判断成立。

再例如：

```python
print(10 < 5)
```

输出：

```text
False
```

因为 `10 < 5` 不成立。

布尔值非常重要，因为程序经常需要根据某个条件是否成立来决定下一步做什么，例如：

* 密码是否正确
* 用户是否已经登录
* 年龄是否达到要求
* 商品是否有库存
* 一个数字是否大于另一个数字

---

## 2. `True` 和 `False` 的大小写

Python 中必须写成：

```python
True
False
```

首字母必须大写。

下面是错误的：

```python
true
false
```

例如：

```python
is_logged_in = True
```

这是合法的。

而：

```python
is_logged_in = true
```

通常会产生：

```text
NameError
```

因为 Python 会把 `true` 当成一个变量名，而不是布尔值。

---

## 3. 比较运算会产生布尔值

在 Python 中，对两个值进行比较时，结果通常就是 `True` 或 `False`。

例如：

```python
20 > 10
```

可以理解为在问：

> 20 是否大于 10？

答案是：

```python
True
```

因此：

```python
result = 20 > 10

print(result)
```

输出：

```text
True
```

变量 `result` 保存的不是数字，而是一个 `bool` 类型的值。

可以使用 `type()` 查看：

```python
result = 20 > 10

print(type(result))
```

输出：

```text
<class 'bool'>
```

---

## 4. Python 的比较运算符

Python 最常见的比较运算符有：

| 运算符  | 含义    | 示例        | 结果      |
| ---- | ----- | --------- | ------- |
| `>`  | 大于    | `20 > 10` | `True`  |
| `<`  | 小于    | `20 < 10` | `False` |
| `==` | 等于    | `7 == 7`  | `True`  |
| `!=` | 不等于   | `7 != 7`  | `False` |
| `>=` | 大于或等于 | `5 >= 5`  | `True`  |
| `<=` | 小于或等于 | `3 <= 8`  | `True`  |

---

## 5. `>`：大于

```python
print(8 > 3)
```

输出：

```text
True
```

因为：

```text
8 > 3
```

成立。

如果反过来：

```python
print(3 > 8)
```

结果：

```text
False
```

---

## 6. `<`：小于

```python
print(3 < 8)
```

输出：

```text
True
```

而：

```python
print(8 < 3)
```

输出：

```text
False
```

---

## 7. `==`：判断两个值是否相等

`==` 表示：

> 左边和右边的值是否相等？

例如：

```python
print(7 == 7)
```

结果：

```text
True
```

因为两个值相同。

```python
print(7 == 8)
```

结果：

```text
False
```

---

## 8. `=` 和 `==` 完全不同

这是 Python 初学者非常容易混淆的地方。

### `=`：赋值

```python
x = 5
```

意思是：

> 把值 `5` 保存到变量 `x` 中。

---

### `==`：比较是否相等

```python
x == 5
```

意思是：

> 判断 `x` 的值是否等于 `5`。

结果是：

```python
True
```

或者：

```python
False
```

例如：

```python
x = 5

print(x == 5)
```

输出：

```text
True
```

可以记忆为：

```text
=   → 给变量一个值
==  → 问两个值是否相等
```

---

## 9. `!=`：判断两个值是否不相等

`!=` 表示：

> 两个值是否不同？

例如：

```python
print(5 != 10)
```

输出：

```text
True
```

因为：

```text
5 和 10 不相等
```

而：

```python
print(5 != 5)
```

输出：

```text
False
```

因为 `5` 和 `5` 实际上相等，所以“不相等”这个判断是错误的。

---

## 10. `>=` 和 `<=`

### `>=`：大于或等于

```python
print(10 >= 5)
```

结果：

```text
True
```

因为 `10 > 5`。

但相等也可以：

```python
print(10 >= 10)
```

结果仍然是：

```text
True
```

所以：

```text
>=
```

意味着满足以下任意一种情况即可：

```text
大于
或者
等于
```

---

### `<=`：小于或等于

```python
print(3 <= 8)
```

结果：

```text
True
```

相等时也成立：

```python
print(8 <= 8)
```

结果：

```text
True
```

---

## 11. 可以直接打印比较结果

不一定要先创建变量。

例如：

```python
print(42 == 42)
print(10 > 20)
print(5 != 3)
```

输出：

```text
True
False
True
```

Python 的执行过程可以理解为：

```text
print(42 == 42)
      ↓
    True
      ↓
print(True)
```

所以最终打印：

```text
True
```

---

## 12. 也可以把布尔值保存到变量中

比较表达式的结果可以保存在变量里：

```python
is_positive = 7 > 0

print(is_positive)
```

输出：

```text
True
```

执行过程相当于：

```python
is_positive = 7 > 0
```

先计算：

```python
7 > 0
```

得到：

```python
True
```

然后相当于：

```python
is_positive = True
```

---

## 13. 布尔变量的命名

表示布尔状态的变量，通常使用容易回答“是/否”的名称。

例如：

```python
is_logged_in = True
is_admin = False
has_permission = True
is_available = False
```

这些变量名读起来就像问题：

```text
is_logged_in
→ 是否已经登录？

has_permission
→ 是否拥有权限？
```

这是非常常见且推荐的命名方式。

常见前缀包括：

```text
is_
has_
can_
should_
```

例如：

```python
is_valid = True
has_access = False
can_edit = True
should_retry = False
```

---

## 14. 数字可以进行比较

例如：

```python
print(8 < 15)
print(100 == 99)
print(4 != 4)
```

输出：

```text
True
False
False
```

第三个例子容易看错：

```python
4 != 4
```

意思不是：

> 4 是否等于 4？

而是：

> 4 是否**不等于** 4？

由于 `4` 确实等于 `4`，所以“不等于”这个判断是错误的：

```text
False
```

---

## 15. 字符串也可以比较

字符串同样可以使用 `==` 和 `!=`。

例如：

```python
print("cat" == "cat")
```

输出：

```text
True
```

而：

```python
print("cat" == "dog")
```

输出：

```text
False
```

字符串比较区分大小写：

```python
print("Python" == "python")
```

输出：

```text
False
```

因为：

```text
"Python"
```

和：

```text
"python"
```

并不是完全相同的字符串。

---

## 16. 不同类型通常不会因为“看起来一样”就相等

例如：

```python
print(5 == "5")
```

结果：

```text
False
```

因为：

```python
5
```

是整数 `int`，而：

```python
"5"
```

是字符串 `str`。

虽然它们看起来都像数字 5，但类型和含义不同。

```python
print(type(5))
print(type("5"))
```

输出：

```text
<class 'int'>
<class 'str'>
```

如果想比较它们的数值，可以先转换类型：

```python
print(5 == int("5"))
```

结果：

```text
True
```

---

## 17. 布尔值为什么重要

布尔值真正重要的地方在于：**控制程序是否执行某段代码。**

例如：

```python
age = 20

if age >= 18:
    print("You are an adult")
```

这里：

```python
age >= 18
```

会产生：

```python
True
```

因此 `if` 中的代码会执行。

如果：

```python
age = 15
```

那么：

```python
age >= 18
```

结果是：

```python
False
```

对应代码就不会执行。

程序中的很多逻辑，本质上都是：

```text
提出一个条件
    ↓
得到 True / False
    ↓
根据结果决定下一步
```

例如：

```python
password_correct = True

if password_correct:
    print("Login successful")
```

---

## 18. 常见错误

### 错误 1：写成小写 `true` / `false`

错误：

```python
is_ready = true
```

正确：

```python
is_ready = True
```

Python 的布尔值固定为：

```python
True
False
```

---

### 错误 2：混淆 `=` 和 `==`

赋值：

```python
x = 10
```

比较：

```python
x == 10
```

记住：

```text
=   assignment（赋值）
==  equality comparison（相等比较）
```

---

### 错误 3：把布尔值写成字符串

下面两个值并不一样：

```python
True
"True"
```

第一个是：

```python
bool
```

第二个是：

```python
str
```

可以验证：

```python
print(type(True))
print(type("True"))
```

输出：

```text
<class 'bool'>
<class 'str'>
```

---

### 错误 4：忘记比较表达式本身就有结果

不需要这样绕一圈：

```python
age = 20

if age >= 18:
    is_adult = True
else:
    is_adult = False
```

因为：

```python
age >= 18
```

本身就已经是一个布尔值。

可以直接写：

```python
age = 20

is_adult = age >= 18
```

这是更简洁、更常见的写法。

---

## 19. 一个完整示例

```python
age = 20
username = "Alice"

is_adult = age >= 18
is_alice = username == "Alice"

print(is_adult)
print(is_alice)
```

输出：

```text
True
True
```

执行过程：

```text
age >= 18
20 >= 18
True
```

所以：

```python
is_adult = True
```

同时：

```text
username == "Alice"
"Alice" == "Alice"
True
```

所以：

```python
is_alice = True
```

---

## 20. 核心总结

Python 的布尔类型：

```python
bool
```

只有两个值：

```python
True
False
```

比较表达式通常会产生布尔结果：

```python
10 > 5      # True
10 < 5      # False
5 == 5      # True
5 != 5      # False
5 >= 5      # True
3 <= 8      # True
```

最需要记住的区别是：

```text
=   → 赋值
==  → 判断是否相等
```

比较结果可以直接保存到变量：

```python
is_positive = number > 0
```

布尔值是程序进行条件判断和决策的基础：

```python
if condition:
    ...
```

其核心思路就是：

```text
条件表达式
    ↓
True / False
    ↓
程序决定接下来执行什么
```
