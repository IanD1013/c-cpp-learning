# `match/case` 中的守卫子句（Guard Clauses）

你已经知道了如何匹配字面量值、捕获变量以及使用 OR 模式组合备选分支。但是，当你需要表达的条件超出了简单的相等性或结构检查时——比如检查一个数字是否在某个范围内——该怎么办呢？

这就是 **守卫子句（guard clauses）** 发挥作用的地方。

守卫是附加在 `case` 上的 `if` 条件，它增加了一个额外的约束：

> 模式必须首先匹配成功，*然后*才会评估守卫条件。

如果守卫条件为 `False`，Python 会直接进入下一个 `case`，就像模式根本没有匹配一样。

## 工作原理

当 Python 遇到带有守卫的 `case` 时，它会按顺序执行两项检查：

1. **模式匹配**  
   该值在结构上是否与模式匹配？  
   对于像 `case x` 这样的捕获模式，这总是成功的并将值绑定到 `x`。

2. **守卫评估**  
   `if` 条件是否为 `True`？  
   如果不是，将跳过当前 `case` 并尝试下一个 `case`。

这个两步过程意味着你可以将捕获模式的灵活性与任意布尔逻辑结合起来。

守卫是自上而下进行评估的，因此 **第一个模式和守卫都成功的 `case`** 将会生效。

## 语法

```python
match subject:
    case pattern if condition:
        # runs only if pattern matches AND condition is True
    case pattern if other_condition:
        # tried next if the previous guard was False
    case _:
        # fallback if nothing else matched
```

## 示例

考虑一个对温度读数进行分类的函数：

```python
def describe_temp(temp):
    match temp:
        case t if t < 0:
            return "freezing"
        case t if t < 15:
            return "cold"
        case t if t < 25:
            return "comfortable"
        case t if t < 35:
            return "warm"
        case _:
            return "hot"


describe_temp(-5)   # "freezing"
describe_temp(10)   # "cold"
describe_temp(22)   # "comfortable"
describe_temp(40)   # "hot"
```

注意每个 `case t` 是如何将值捕获到 `t` 中的，但守卫限制了哪个 `case` 实际触发。

顺序至关重要——`case t if t < 15` 排在 `case t if t < 25` 之前，这样 `10` 就不会意外匹配到 `"comfortable"` 分支。

你还可以将守卫与更复杂的表达式结合使用：

```python
def classify_score(score):
    match score:
        case s if s >= 90 and s <= 100:
            return "A"
        case s if s >= 80:
            return "B"
        case s if s >= 70:
            return "C"
        case s if s >= 0:
            return "F"
        case _:
            return "invalid"
```

## 关键细节

- 守卫对捕获的变量拥有完全的访问权限——你可以对其调用函数，使用 `and` / `or` 组合多个条件等。
- 如果守卫失败，捕获的变量将被丢弃，并从头开始尝试下一个 `case`。
- 你可以在同一个函数中使用多个 `match` 语句——没有任何规则限制你只能写一个。
- 守卫适用于任何模式类型，而不仅仅是捕获模式。