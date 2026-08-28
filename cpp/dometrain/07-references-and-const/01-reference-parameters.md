# C++ 引用参数：让函数直接修改调用者的变量

## 1. 普通参数默认是“复制”

如果函数参数写成：

```cpp
void increaseBy(int value, int amount)
```

这里的 `value` 是调用者变量的一份**副本**。

例如：

```cpp
void increaseBy(int value, int amount) {
    value += amount;
}

int score = 10;
increaseBy(score, 5);
```

调用函数时，大致相当于：

```cpp
value = score;   // 复制出一个新的 int
```

然后函数修改的是这个副本：

```cpp
value += 5;
```

因此函数结束后：

```cpp
score == 10
```

原来的 `score` 不会改变。

---

## 2. `&`：引用参数

如果希望函数直接修改调用者的变量，可以把参数声明成**引用（reference）**：

```cpp
void doubleIt(int &n) {
    n = n * 2;
}
```

调用：

```cpp
int score = 21;
doubleIt(score);
```

执行后：

```cpp
score == 42
```

这里的：

```cpp
int &n
```

可以理解为：

> `n` 是调用者变量的另一个名字。

调用：

```cpp
doubleIt(score);
```

以后，在函数内部：

```cpp
n
```

实际上指向的就是：

```cpp
score
```

所以：

```cpp
n = n * 2;
```

等价于直接修改：

```cpp
score = score * 2;
```

---

## 3. 值参数 vs 引用参数

### 值参数

```cpp
void f(int value)
```

调用：

```cpp
int x = 10;
f(x);
```

可以理解为：

```text
x
│
│ 复制
▼
value
```

`value` 和 `x` 是两个不同的变量。

修改：

```cpp
value = 20;
```

不会影响 `x`。

---

### 引用参数

```cpp
void f(int &value)
```

调用：

```cpp
int x = 10;
f(x);
```

关系更像：

```text
x ──────┐
        │
value ──┘
```

`x` 和 `value` 指向同一个变量。

所以：

```cpp
value = 20;
```

会导致：

```cpp
x == 20
```

---

## 4. `&` 写在参数声明中，不写在调用处

函数声明：

```cpp
void doubleIt(int &n)
```

调用仍然正常写：

```cpp
doubleIt(score);
```

不要写：

```cpp
doubleIt(&score);
```

因为这里的：

```cpp
&score
```

不是“使用引用”，而是 C++ 中的**取地址运算符**，得到的是 `score` 的内存地址，类型为：

```cpp
int*
```

而函数要求的是：

```cpp
int&
```

两者不是同一种类型。

因此记住：

```cpp
void f(int &x);  // & 写在参数声明中

f(value);        // 调用时正常传变量
```

---

## 5. 非 const 引用通常不能绑定到字面量

下面是合法的：

```cpp
int score = 21;
doubleIt(score);
```

因为 `score` 是一个真实存在、可以被修改的变量。

但是：

```cpp
doubleIt(21);
```

通常无法编译。

因为：

```cpp
21
```

是一个临时值/字面量，而：

```cpp
int &n
```

要求绑定到一个可以被修改的 `int` 对象。

因此：

```cpp
int &x
```

通常意味着：

> 这个函数可能修改调用者传入的变量。

---

## 6. 修改会立即发生

引用参数并不是在函数结束时才“同步”回调用者。

例如：

```cpp
void change(int &x) {
    x = 20;

    if (x > 10) {
        return;
    }

    x = 30;
}
```

调用：

```cpp
int n = 5;
change(n);
```

执行：

```cpp
x = 20;
```

时，`n` 就已经变成了 `20`。

随后函数即使：

```cpp
return;
```

前面已经发生的修改也不会撤销。

---

## 7. 常见用途：通过参数输出结果

引用参数常用于让函数修改外部变量：

```cpp
void increaseBy(int &value, int amount) {
    value += amount;
}
```

例如：

```cpp
int x = 10;

increaseBy(x, 5);

cout << x;
```

结果：

```text
15
```

负数也一样：

```cpp
int x = 10;

increaseBy(x, -3);
```

得到：

```cpp
x == 7
```

如果：

```cpp
increaseBy(x, 0);
```

则：

```cpp
x
```

保持不变。

---

# 核心总结

普通参数：

```cpp
int value
```

表示：

> 把调用者的值复制一份给函数。

引用参数：

```cpp
int &value
```

表示：

> `value` 成为调用者变量的另一个名字。

因此：

```cpp
void change(int value)
```

通常不能修改调用者的变量。

而：

```cpp
void change(int &value)
```

可以直接修改调用者的变量。

典型写法：

```cpp
void increaseBy(int &value, int amount) {
    value += amount;
}
```

调用时仍然写：

```cpp
increaseBy(value, amount);
```

而不是：

```cpp
increaseBy(&value, amount);
```
