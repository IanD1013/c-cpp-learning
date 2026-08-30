# C++ 头文件重复包含、`#pragma once` 与 `inline`

## 1. 问题：为什么明明只定义了一次 `Tally`，却报 `redefinition`？

假设项目的包含关系如下：

```text
program.cpp
├── #include "tally.hpp"
└── #include "report.hpp"
        └── #include "tally.hpp"
```

也就是说，`program.cpp` 会通过两条路径得到 `tally.hpp`：

```cpp
#include "tally.hpp"
#include "report.hpp"
```

而 `report.hpp` 内部又有：

```cpp
#include "tally.hpp"
```

如果 `tally.hpp` 中定义了：

```cpp
struct Tally {
    int hits;
    int misses;
};
```

那么经过预处理以后，效果类似于：

```cpp
struct Tally {
    int hits;
    int misses;
};

// report.hpp 展开后又来了一个

struct Tally {
    int hits;
    int misses;
};
```

于是编译器看到同一个 `struct Tally` 被定义两次，就会报：

```text
redefinition of 'Tally'
```

关键点是：

> `#include` 本质上可以理解成：把另一个文件的内容“复制粘贴”到当前位置。

所以即使项目源代码里只有一个 `Tally` 定义，经过多层 `#include` 展开后，它仍然可能在同一个 `.cpp` 中出现多次。

---

# 2. 使用 `#pragma once` 防止头文件重复包含

解决方式是在 `tally.hpp` 第一行加入：

```cpp
#pragma once
```

例如：

```cpp
#pragma once

#include <vector>

struct Tally {
    int hits;
    int misses;
};
```

它的含义是：

> 在编译当前 `.cpp` 文件时，这个头文件最多只处理一次。

因此：

```text
program.cpp
├── tally.hpp        ← 第一次：正常展开
└── report.hpp
        └── tally.hpp ← 第二次：跳过
```

第二次 `#include "tally.hpp"` 并不会报错，只是不会再次展开文件内容。

于是 `Tally` 最终只会出现一次。

---

# 3. `#pragma once` 属于预处理阶段

C++ 编译可以粗略理解为：

```text
源代码
  ↓
预处理
  ↓
编译
  ↓
链接
```

`#include`、`#pragma once`、`#define` 等都属于**预处理器（preprocessor）**处理的内容。

因此：

```cpp
#pragma once
```

是在真正进行 C++ 编译之前生效的。

可以把它理解成预处理器记住：

```text
tally.hpp 已经展开过了
```

再次遇到时就直接跳过。

---

# 4. `#pragma once` 只解决“同一个编译单元中的重复包含”

这一点非常重要。

`#pragma once` 能解决：

```text
program.cpp
   ↓
tally.hpp 被间接 include 两次
```

但它不能解决另一类问题：

```text
a.cpp ── include ── tally.hpp
b.cpp ── include ── tally.hpp
```

因为：

```text
a.cpp
b.cpp
```

通常会分别编译。

也就是说：

```text
a.cpp → a.o
b.cpp → b.o
```

此时每个 `.cpp` 都有自己独立的预处理过程。

所以 `#pragma once` 的作用范围可以理解为：

> 防止一个头文件在**同一个编译单元（translation unit）**中被展开多次。

它并不会让整个程序中所有 `.cpp` 共享一次展开结果。

---

# 5. 为什么头文件中的普通函数定义可能有问题？

例如你把下面的函数直接写进头文件：

```cpp
int netScore(const Tally &tally) {
    return tally.hits - tally.misses;
}
```

然后：

```cpp
// a.cpp
#include "tally.hpp"
```

```cpp
// b.cpp
#include "tally.hpp"
```

经过编译后，相当于：

```text
a.o 中有一个 netScore()
b.o 中又有一个 netScore()
```

最终链接时，链接器可能看到多个相同函数定义，从而违反 **One Definition Rule（ODR，单一定义规则）**。

可能得到类似：

```text
multiple definition of `netScore`
```

注意这和前面的 `Tally redefinition` 是不同阶段的问题：

| 问题                   | 阶段       | 常见解决方法                |
| -------------------- | -------- | --------------------- |
| 一个 `.cpp` 中头文件被展开多次  | 预处理 / 编译 | `#pragma once`        |
| 多个 `.cpp` 都包含同一个函数定义 | 链接       | `inline` 或把实现放 `.cpp` |

---

# 6. `inline` 在这里真正解决什么？

例如：

```cpp
inline int netScore(const Tally &tally) {
    return tally.hits - tally.misses;
}
```

如果多个 `.cpp` 都通过头文件得到完全相同的这个定义：

```text
a.cpp → netScore 定义
b.cpp → netScore 定义
c.cpp → netScore 定义
```

`inline` 允许这种情况存在。

因此：

> 定义在头文件中的普通函数通常需要 `inline`。

这里不要把 `inline` 主要理解成：

> “让编译器把函数调用展开，提高性能。”

现代 C++ 中，是否真的做函数内联优化主要由编译器决定。

在头文件设计中，`inline` 更重要的作用是：

> 允许完全相同的函数定义出现在多个编译单元中，而不会违反 ODR。

---

# 7. `#pragma once` 和 `inline` 不要混淆

两者解决的是完全不同的问题。

## `#pragma once`

解决：

```text
一个 .cpp
  ↓
同一个头文件被 include 多次
```

例如：

```text
program.cpp
├── tally.hpp
└── report.hpp
      └── tally.hpp
```

---

## `inline`

解决：

```text
多个 .cpp
 ↓     ↓
共同 include 一个含函数定义的头文件
```

例如：

```text
a.cpp ─┐
       ├── tally.hpp → inline netScore(...)
b.cpp ─┘
```

因此可以记成：

```text
#pragma once
    ↓
防止“一个编译单元里重复展开头文件”

inline
    ↓
允许“多个编译单元里存在相同函数定义”
```

---

# 8. 本题中的 `Tally`

`Tally` 保存两个统计值：

```cpp
struct Tally {
    int hits;
    int misses;
};
```

含义：

```text
hits   = 达到 threshold 的值的数量
misses = 没达到 threshold 的值的数量
```

例如：

```cpp
values = {10, 3, 8, 2}
threshold = 8
```

判断：

```text
10 >= 8 → hit
3  < 8 → miss
8  >= 8 → hit
2  < 8 → miss
```

因此：

```cpp
hits = 2
misses = 2
```

并且永远有：

```cpp
hits + misses == values.size()
```

因为每个元素只能属于两类之一：

```text
value >= threshold
```

或者：

```text
value < threshold
```

---

# 9. `tallyOf`

函数签名：

```cpp
inline Tally tallyOf(
    const std::vector<int> &values,
    int threshold
);
```

作用：

> 遍历所有数字，根据 `threshold` 统计 hit 和 miss。

核心逻辑：

```cpp
if (value >= threshold) {
    ++tally.hits;
} else {
    ++tally.misses;
}
```

例如：

```cpp
tallyOf({10, 3, 8, 2}, 8)
```

返回：

```cpp
Tally{
    2,
    2
}
```

---

# 10. `netScore`

函数签名：

```cpp
inline int netScore(const Tally &tally);
```

计算：

```text
netScore = hits - misses
```

例如：

```cpp
hits = 5
misses = 2
```

那么：

```cpp
netScore = 3
```

如果：

```cpp
hits = 2
misses = 5
```

则：

```cpp
netScore = -3
```

所以结果完全可以是负数。

---

# 11. 本题最值得记住的知识点

```text
#include
≈ 把文件内容复制到当前位置
```

因此复杂的 include 关系可能导致头文件被展开多次。

解决头文件重复展开：

```cpp
#pragma once
```

如果函数实现直接写在 `.hpp` 中，并且这个头文件未来可能被多个 `.cpp` 包含：

```cpp
inline
```

典型写法：

```cpp
#pragma once

#include <vector>

struct Tally {
    int hits;
    int misses;
};

inline Tally tallyOf(
    const std::vector<int> &values,
    int threshold
) {
    // ...
}

inline int netScore(const Tally &tally) {
    // ...
}
```

一句话总结：

> `#pragma once` 防止同一个头文件在一个编译单元里被重复展开；`inline` 则让头文件中的函数定义可以安全地出现在多个编译单元中。
