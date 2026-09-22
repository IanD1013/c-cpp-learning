### 默认参数值

在定义函数时，你通常希望某些参数是可选的——如果调用者没有提供这些参数，函数会自动使用合理的默认备选值。默认参数值正是用于实现这一功能的。它们减少了调用者需要记住的参数数量，同时为需要自定义设置的用户保留了灵活性。

### 工作原理

在函数定义中，你可以使用 `=` 为参数指定默认值。当调用函数时没有提供该参数，Python 就会使用默认值。当调用者*确实*提供了值时，它会覆盖默认值。带有默认值的参数必须出现在所有没有默认值的参数**之后**——否则 Python 无法确定哪些实参对应哪些形参。

### 语法

```python
def function_name(required_param, optional_param=default_value):
    # use required_param and optional_param normally
    pass
```

你可以设置多个默认参数：

```python
def function_name(a, b=10, c=20):
    pass
```

### 示例

```python
# A function with one default parameter
def power(base, exponent=2):
    return base ** exponent

power(5)        # returns 25  (exponent defaults to 2)
power(5, 3)     # returns 125 (exponent overridden to 3)

# Multiple default parameters
def describe_pet(name, species="dog", age=1):
    return f"{name} is a {species}, age {age}"

describe_pet("Max")                # "Max is a dog, age 1"
describe_pet("Whiskers", "cat")    # "Whiskers is a cat, age 1"
describe_pet("Polly", "parrot", 5) # "Polly is a parrot, age 5"
```

### 可变默认参数陷阱

这是 Python 中最常见的陷阱之一。**切勿使用可变对象**（例如列表、字典或集合）作为默认值。默认值在函数定义时只会被求值**一次**，而不是每次调用函数时都求值。这意味着每次调用都会共享*同一个*对象：

```python
# DANGEROUS — don't do this!
def add_item(item, items=[]):
    items.append(item)
    return items

add_item("apple")   # ['apple']
add_item("banana")  # ['apple', 'banana']  — the list persists!
```

安全的做法是将 `None` 作为默认值，并在函数内部创建新对象：

```python
# SAFE — do this instead
def add_item(item, items=None):
    if items is None:
        items = []
    items.append(item)
    return items

add_item("apple")   # ['apple']
add_item("banana")  # ['banana']  — a new list each time
```
