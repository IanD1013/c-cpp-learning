# C++ `namespace` 命名空间与作用域

## 1. 为什么需要 `namespace`

在大型 C++ 项目中，不同模块很可能使用相同的函数名、类名或变量名。

例如：

```cpp
int count();
```

如果另一个模块也想定义：

```cpp
int count();
```

就可能发生名字冲突。

C++ 使用 **命名空间（namespace）** 给名字划分作用域：

```cpp
namespace geo {

int perimeter(int side);

}
```

这里真正的完整函数名不是：

```cpp
perimeter
```

而是：

```cpp
geo::perimeter
```

因此，不同命名空间可以拥有同名函数：

```cpp
namespace geo {
int count();
}

namespace text {
int count();
}
```

它们分别是：

```cpp
geo::count
text::count
```

互不冲突。

---

# 2. `::`：作用域解析运算符

`::` 称为 **scope resolution operator（作用域解析运算符）**。

它用来明确指出某个名字属于哪个作用域。

例如：

```cpp
namespace geo {

int perimeter(int side);

}
```

调用时：

```cpp
geo::perimeter(5);
```

意思是：

> 使用 `geo` 命名空间中的 `perimeter`。

---

# 3. 嵌套 namespace

命名空间可以继续嵌套：

```cpp
namespace geo {

int perimeter(int side);

namespace units {

int toMillimeters(int meters);

}

}
```

于是完整名称分别是：

```cpp
geo::perimeter
geo::units::toMillimeters
```

可以把它理解成目录结构：

```text
geo
├── perimeter
└── units
    └── toMillimeters
```

每一个 `::` 都表示进入下一层作用域：

```cpp
geo::units::toMillimeters
```

即：

```text
global
  ↓
geo
  ↓
units
  ↓
toMillimeters
```

现代 C++ 也可以把嵌套 namespace 简写成：

```cpp
namespace geo::units {

int toMillimeters(int meters);

}
```

等价于：

```cpp
namespace geo {
namespace units {

int toMillimeters(int meters);

}
}
```

---

# 4. 全局作用域

如果一个函数不属于任何 namespace：

```cpp
int perimeter(int side);
```

那么它属于 **global scope（全局作用域）**。

因此下面两个函数完全可以同时存在：

```cpp
int perimeter(int side);

namespace geo {

int perimeter(int side);

}
```

它们实际上是两个不同的函数：

```cpp
::perimeter
geo::perimeter
```

其中：

```cpp
::perimeter
```

开头的 `::` 明确表示：

> 从全局作用域寻找 `perimeter`。

通常直接写：

```cpp
perimeter(5);
```

也可以调用全局函数，但：

```cpp
::perimeter(5);
```

更加明确。

---

# 5. 名字查找：裸名字从内向外寻找

假设有：

```cpp
int perimeter(int side);

namespace geo {

int perimeter(int side);

void test() {
    perimeter(5);
}

}
```

在 `test()` 中写：

```cpp
perimeter(5);
```

编译器会从当前作用域开始向外查找。

大致顺序：

```text
当前局部作用域
    ↓
geo namespace
    ↓
global scope
```

因为 `geo` 中已经存在：

```cpp
geo::perimeter
```

所以这里找到的是：

```cpp
geo::perimeter
```

而不是全局的：

```cpp
::perimeter
```

如果一定要调用全局版本，可以明确写：

```cpp
::perimeter(5);
```

---

# 6. namespace 可以重复打开

一个 namespace 不需要一次写完。

例如：

```cpp
namespace geo {

int perimeter(int side);

}
```

后面可以再次写：

```cpp
namespace geo {

int area(int side);

}
```

这不是创建两个不同的 `geo`。

两段内容都会属于同一个：

```cpp
namespace geo
```

最终相当于：

```cpp
namespace geo {

int perimeter(int side);
int area(int side);

}
```

这也是大型 C++ 项目能够把同一个命名空间分散到很多 `.hpp` 和 `.cpp` 文件里的原因。

例如标准库也可以理解为大量代码共同放进：

```cpp
namespace std
```

所以我们会看到：

```cpp
std::string
std::vector
std::cout
```

---

# 7. namespace 中的函数如何定义

假设头文件中声明：

```cpp
namespace geo {

int perimeter(int side);

}
```

一种最直观的定义方式是重新打开相同 namespace：

```cpp
namespace geo {

int perimeter(int side) {
    return side * 4;
}

}
```

函数的完整名字仍然是：

```cpp
geo::perimeter
```

也可以写成完全限定名称：

```cpp
int geo::perimeter(int side) {
    return side * 4;
}
```

但实际项目中，多个相关函数通常一起放进 namespace block，可读性更好：

```cpp
namespace geo {

int perimeter(int side) {
    return side * 4;
}

int area(int side) {
    return side * side;
}

}
```

---

# 8. `std` 本质上也是 namespace

例如：

```cpp
std::string
std::cout
std::vector
std::isspace
std::isalpha
```

这里的：

```cpp
std
```

就是 C++ 标准库使用的命名空间。

所以：

```cpp
std::string
```

意思就是：

> `std` namespace 中的 `string`。

---

# 9. 为什么不推荐 `using namespace std;`

有时会看到：

```cpp
using namespace std;
```

这样之后可以把：

```cpp
std::string
std::cout
```

写成：

```cpp
string
cout
```

虽然少写几个字符，但大型项目中并不推荐，尤其不要写在头文件中。

原因是它把 `std` 中大量名字带进当前名字查找范围。

例如你自己定义：

```cpp
int count();
```

而某个库里恰好也存在：

```cpp
count
```

就容易形成冲突或歧义。

因此更安全、更清晰的写法是：

```cpp
std::string
std::cout
std::isspace
std::isalpha
```

而不是：

```cpp
using namespace std;
```

---

# 10. 本题中的 namespace 结构

题目中的 `text_tools.hpp` 声明了三个函数。

结构可以理解为：

```text
global scope
│
├── wordCount
│
└── text
    │
    ├── wordCount
    │
    └── letters
        │
        └── count
```

因此三个完整函数名分别是：

```cpp
text::wordCount
text::letters::count
::wordCount
```

虽然两个函数都叫：

```cpp
wordCount
```

但是它们不冲突，因为一个属于：

```cpp
text
```

另一个属于：

```cpp
global scope
```

---

# 11. `text::wordCount`：按所有 whitespace 分词

要求：

```cpp
text::wordCount(const std::string &line)
```

把任何 whitespace 都视为分隔符。

例如：

```cpp
"one two three"   // 3
" spaced out "    // 2
""                // 0
"a\tb c"          // 3
```

这里：

```cpp
\t
```

是 tab，也属于 whitespace。

题目要求使用：

```cpp
std::isspace(
    static_cast<unsigned char>(line[i])
)
```

常见 whitespace 包括：

```text
' '   空格
'\t'  tab
'\n'  换行
'\r'  carriage return
```

---

# 12. 如何正确统计“单词 run”

不能简单地：

> 每遇到一个空格就 +1。

因为：

```cpp
"  hello   world  "
```

包含很多空格，但只有两个词。

正确思路是检测：

> 当前字符是不是一个新单词的开始。

可以维护：

```cpp
bool inWord = false;
```

遍历字符。

遇到 whitespace：

```cpp
inWord = false;
```

遇到非 whitespace，并且之前不在单词里：

```cpp
count++;
inWord = true;
```

例如：

```text
"  one   two "
```

状态变化：

```text
空格 → 不在词中
空格 → 不在词中

o → 新词开始 → count = 1
n → 已经在词中
e → 已经在词中

空格 → 离开单词
...

t → 新词开始 → count = 2
```

这样无论有几个连续分隔符，都不会重复计数。

---

# 13. 全局 `wordCount` 的不同之处

全局函数：

```cpp
::wordCount
```

只把普通空格：

```cpp
' '
```

视为分隔符。

所以：

```cpp
"a\tb c"
```

其中：

```cpp
a\tb
```

不会被 tab 分开。

因此实际被分成：

```text
"a\tb"
"c"
```

结果：

```cpp
2
```

而：

```cpp
text::wordCount("a\tb c")
```

会把 tab 也视为 whitespace，因此得到：

```cpp
3
```

这很好地展示了：

```cpp
text::wordCount
```

和：

```cpp
::wordCount
```

虽然函数名相同，却是两个不同函数。

---

# 14. `text::letters::count`

第三个函数：

```cpp
text::letters::count
```

统计字符串中有多少个字母。

例如：

```cpp
"a1 b!"
```

只有：

```text
a
b
```

所以：

```cpp
2
```

而：

```cpp
"Mix 42 ok"
```

字母为：

```text
M i x o k
```

所以：

```cpp
5
```

题目要求使用：

```cpp
std::isalpha(
    static_cast<unsigned char>(line[i])
)
```

大小写字母都会计数。

---

# 15. 为什么 `isspace` / `isalpha` 前要转成 `unsigned char`

题目特意要求：

```cpp
std::isspace(static_cast<unsigned char>(line[i]))
```

以及：

```cpp
std::isalpha(static_cast<unsigned char>(line[i]))
```

而不是简单写：

```cpp
std::isspace(line[i])
std::isalpha(line[i])
```

原因在于：

```cpp
char
```

在某些系统上可能是有符号类型。

某些非 ASCII 字符可能变成负数，而 `<cctype>` 中这些函数通常要求参数必须能够表示为：

```cpp
unsigned char
```

或者是：

```cpp
EOF
```

所以更稳妥的固定模式是：

```cpp
std::isspace(static_cast<unsigned char>(ch))
std::isalpha(static_cast<unsigned char>(ch))
```

这是值得记住的 C++ 习惯写法。

---

# 16. 本节核心知识总结

最重要的是理解：

```cpp
namespace name {
    ...
}
```

建立一个命名作用域。

完整名称使用：

```cpp
namespaceName::identifier
```

例如：

```cpp
text::wordCount
text::letters::count
std::string
```

嵌套 namespace：

```cpp
namespace a {
namespace b {
    int f();
}
}
```

对应：

```cpp
a::b::f
```

全局作用域可以明确写成：

```cpp
::name
```

因此：

```cpp
::wordCount
```

和：

```cpp
text::wordCount
```

可以同时存在。

另外，namespace 可以在不同位置、不同文件重复打开：

```cpp
namespace text {
    ...
}
```

每次都是继续向同一个 namespace 添加内容，而不是创建新的 namespace。
