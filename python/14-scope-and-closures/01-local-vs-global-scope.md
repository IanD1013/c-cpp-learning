### 局部变量与全局变量作用域

Python 中的每个变量都存在于特定的**作用域**（scope）中——即该变量可以被访问的代码区域。理解作用域至关重要，因为它决定了程序的哪些部分可以查看和修改哪些数据。对作用域理解有误会导致一些隐蔽的 bug，比如变量出现意外的值，或者在你认为它们存在时却根本不存在。

### 工作原理

在现阶段，Python 主要有两种作用域需要理解：

* **全局作用域**（Global scope）：在任何函数外部创建的变量。它们在程序的整个生命周期内存在，并且可以在函数内部被**读取**。
* **局部作用域**（Local scope）：在函数内部创建的变量。它们仅在函数执行期间存在，并在函数返回时被销毁。

一个关键规则：如果你在函数内部给一个变量**赋值**，Python 会将其视为**局部**变量——即使存在同名的全局变量也是如此。这被称为**遮蔽**（shadowing）。局部变量在该函数内部隐藏了全局变量，但全局变量本身完全不受影响。

### 语法

```python
# Global variable — accessible everywhere (for reading)
greeting = "Hello"

def some_function():
    # Local variable — only exists inside this function
    message = "World"
    return message

# greeting is still accessible here
# message does NOT exist here — it was local to some_function
```

### 示例

```python
# Example 1: Global variable remains unchanged after function call
count = 10

def reset_count():
    count = 0  # This creates a NEW local variable named 'count'
    print(count)  # Prints: 0

reset_count()
print(count)  # Prints: 10 — the global 'count' was never touched!


# Example 2: Local variables don't exist outside their function
def compute_area(length, width):
    area = length * width  # 'area' is local
    return area

result = compute_area(5, 3)  # result is 15
# print(area)  # NameError! 'area' only existed inside compute_area


# Example 3: Reading a global variable inside a function (no assignment)
base_tax = 0.08

def calculate_tax(price):
    # We can READ base_tax here because we're not assigning to it
    return price * base_tax

print(calculate_tax(100))  # Prints: 8.0
```

### 需要记住的关键规则

| **规则行为**      |                        |
| ------------- | ---------------------- |
| 在函数内部赋值的变量    | 始终是该函数的**局部**变量        |
| 在所有函数外部定义的变量  | **全局**变量——可从任何地方读取     |
| 给与全局变量同名的变量赋值 | 创建一个**新的局部**变量（遮蔽全局变量） |
| 在函数外部访问局部变量   | 引发 **NameError**       |
| 函数参数          | 被视为**局部**变量            |
