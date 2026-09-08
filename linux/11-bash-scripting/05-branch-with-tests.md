# Bash 条件判断：读取数值并按阈值分类

## 目标

假设文件：

```text
/var/lib/course/capacity
```

只保存一个整数，表示数据文件系统当前已使用的百分比，例如：

```text
82
```

需要把这个数值转换成便于运维人员理解的状态：

|     使用率 | 状态         |
| ------: | ---------- |
|  `< 70` | `ok`       |
| `70–89` | `warning`  |
| `>= 90` | `critical` |

例如：

```text
65  -> ok
82  -> warning
94  -> critical
```

---

## 1. Bash 的 `if` 本质上判断的是退出状态

Bash 的 `if` 并不是直接理解“条件表达式”，而是：

> 执行一个命令，然后根据这个命令的退出状态决定是否进入分支。

基本结构：

```bash
if command; then
    ...
fi
```

如果 `command`：

* 返回 `0`：条件成立；
* 返回非 `0`：条件不成立。

例如：

```bash
if [ "$used" -ge 90 ]; then
    echo "critical"
fi
```

这里真正被 `if` 执行的是：

```bash
[ "$used" -ge 90 ]
```

---

## 2. `[` 实际上是一个命令

下面的代码：

```bash
[ "$used" -ge 90 ]
```

并不是类似其他语言中的括号语法。

`[` 本质上是 `test` 命令的一种写法：

```bash
test "$used" -ge 90
```

基本等价于：

```bash
[ "$used" -ge 90 ]
```

因此空格非常重要：

```bash
[ "$used" -ge 90 ]
```

正确。

```bash
["$used" -ge 90]
```

错误，因为 Bash 会试图寻找一个名为 `[` 后面直接连着内容的命令。

最后的 `]` 也不是普通标点，而是 `[` 命令要求的最后一个参数。

---

## 3. Bash 中常见的三类测试

选择正确的比较运算符非常重要。

### 文件测试

```bash
[ -f "$path" ]   # 是否为普通文件
[ -d "$path" ]   # 是否为目录
[ -e "$path" ]   # 路径是否存在
```

例如：

```bash
if [ -f /var/lib/course/capacity ]; then
    echo "file exists"
fi
```

---

### 字符串比较

```bash
[ "$a" = "$b" ]
[ "$a" != "$b" ]
```

例如：

```bash
status="ok"

if [ "$status" = "ok" ]; then
    echo "healthy"
fi
```

字符串比较用于文本，不应拿来做数值大小判断。

---

### 整数比较

Bash 的传统整数比较运算符如下：

| 运算符   | 含义                         |
| ----- | -------------------------- |
| `-eq` | equal，等于                   |
| `-ne` | not equal，不等于              |
| `-lt` | less than，小于               |
| `-le` | less than or equal，小于等于    |
| `-gt` | greater than，大于            |
| `-ge` | greater than or equal，大于等于 |

例如：

```bash
[ "$used" -lt 70 ]
[ "$used" -ge 90 ]
```

分别表示：

```text
used < 70
used >= 90
```

---

## 4. 不要把数字当字符串比较

假设：

```bash
used=9
```

阈值是：

```text
85
```

如果错误地按照字符串进行大小比较，比较过程可能按字符顺序进行：

```text
"9"
"85"
```

第一个字符就是：

```text
9 > 8
```

于是字符串意义上的结果可能认为：

```text
"9" > "85"
```

这显然不是我们想要的数值结果：

```text
9 < 85
```

因此阈值判断应使用整数比较：

```bash
[ "$used" -ge 90 ]
```

而不是依赖字符串比较。

---

## 5. 使用 `if` / `elif` 创建多个区间

对于三个互斥的状态，可以使用：

```bash
if condition1; then
    ...
elif condition2; then
    ...
else
    ...
fi
```

例如：

```bash
if [ "$used" -ge 90 ]; then
    status="critical"
elif [ "$used" -ge 70 ]; then
    status="warning"
else
    status="ok"
fi
```

这里的判断顺序非常重要。

### 为什么先判断 `>= 90`

对于：

```text
95
```

它同时满足：

```text
>= 90
>= 70
```

但 `if` / `elif` 只执行**第一个成立的分支**。

因此应从最严格、最高的阈值开始：

```bash
if [ "$used" -ge 90 ]; then
    ...
elif [ "$used" -ge 70 ]; then
    ...
fi
```

这样：

```text
95 -> critical
82 -> warning
65 -> ok
```

---

## 6. 错误的判断顺序

下面的写法有逻辑错误：

```bash
if [ "$used" -ge 70 ]; then
    echo "warning"
elif [ "$used" -ge 90 ]; then
    echo "critical"
fi
```

如果：

```bash
used=95
```

第一个条件：

```bash
[ "$used" -ge 70 ]
```

已经成立，于是输出：

```text
warning
```

后面的：

```bash
[ "$used" -ge 90 ]
```

永远没有机会执行。

因此对于这种等级判断，通常采用：

> 从最高阈值向最低阈值判断。

---

## 7. 从文件读取数值

可以使用命令替换：

```bash
used=$(cat /var/lib/course/capacity)
```

`$(...)` 会：

1. 执行里面的命令；
2. 捕获标准输出；
3. 去掉输出末尾的换行；
4. 把结果作为字符串返回。

例如文件内容实际是：

```text
82\n
```

执行：

```bash
used=$(cat /var/lib/course/capacity)
```

之后变量实际上得到：

```text
82
```

因此可以直接用于整数比较：

```bash
[ "$used" -ge 90 ]
```

### 更简单的 Bash 写法

读取整个小文件时，也可以写：

```bash
used=$(< /var/lib/course/capacity)
```

它不需要额外运行 `cat` 进程，因此在 Bash 脚本中更加直接。

---

## 8. `[ ]` 与 `[[ ]]`

Bash 中经常看到两种条件判断：

```bash
[ ... ]
```

和：

```bash
[[ ... ]]
```

它们不是完全相同的东西。

### `[ ... ]`

```bash
[ "$status" = "ok" ]
```

`[` 本质上是一个命令，因此变量引用和空格规则尤其重要。

---

### `[[ ... ]]`

```bash
[[ $status = "ok" ]]
```

`[[ ]]` 是 Bash 自己的条件表达式语法。

它在进行字符串处理时通常更安全，因为里面不会像普通命令参数一样发生普通的单词拆分和 pathname expansion。

因此 Bash 脚本中经常优先使用：

```bash
if [[ $status = "ok" ]]; then
    ...
fi
```

不过对于简单的整数比较：

```bash
[ "$used" -ge 90 ]
```

和：

```bash
[[ $used -ge 90 ]]
```

都可以正常工作。

---

## 9. 完整脚本

目标脚本：

```text
/home/learner/bin/capacity-check
```

内容：

```bash
#!/bin/bash

used=$(< /var/lib/course/capacity)

if [ "$used" -ge 90 ]; then
    echo "critical"
elif [ "$used" -ge 70 ]; then
    echo "warning"
else
    echo "ok"
fi
```

假设：

```text
/var/lib/course/capacity
```

内容为：

```text
82
```

执行：

```bash
/home/learner/bin/capacity-check
```

输出：

```text
warning
```

---

## 10. 设置脚本权限

要求脚本权限为：

```text
755
```

可以执行：

```bash
chmod 755 /home/learner/bin/capacity-check
```

`755` 表示：

| 身份    | 权限           |
| ----- | ------------ |
| 文件所有者 | `rwx`：读、写、执行 |
| 所属组   | `r-x`：读、执行   |
| 其他用户  | `r-x`：读、执行   |

可以检查：

```bash
ls -l /home/learner/bin/capacity-check
```

典型结果：

```text
-rwxr-xr-x 1 learner learner ... capacity-check
```

---

## 11. 把输出写入状态文件

要求把脚本输出保存到：

```text
/home/learner/capacity-status
```

使用输出重定向：

```bash
/home/learner/bin/capacity-check > /home/learner/capacity-status
```

其中：

```text
>
```

表示把标准输出写入文件，并覆盖该文件原有内容。

例如当前容量为：

```text
92
```

执行后：

```bash
cat /home/learner/capacity-status
```

得到：

```text
critical
```

---

## 12. 边界值检查

阈值代码：

```bash
if [ "$used" -ge 90 ]; then
    echo "critical"
elif [ "$used" -ge 70 ]; then
    echo "warning"
else
    echo "ok"
fi
```

几个最值得测试的值是边界附近的数字：

|   输入 | 输出         |
| ---: | ---------- |
| `69` | `ok`       |
| `70` | `warning`  |
| `89` | `warning`  |
| `90` | `critical` |

边界测试可以快速发现 `-gt`、`-ge`、`-lt`、`-le` 写错的问题。

---

## 13. 核心记忆

```bash
# 整数比较
-eq   # ==
-ne   # !=
-lt   # <
-le   # <=
-gt   # >
-ge   # >=
```

多级阈值判断通常从最高级开始：

```bash
if [ "$value" -ge HIGH ]; then
    ...
elif [ "$value" -ge MEDIUM ]; then
    ...
else
    ...
fi
```

完整的数据流可以理解为：

```text
capacity 文件
      ↓
读取数字
      ↓
整数阈值比较
      ↓
ok / warning / critical
      ↓
capacity-status 文件
```

关键原则是：**数字使用整数比较，多级范围从严格条件向宽松条件判断，`if` 根据命令的退出状态决定执行哪个分支。**
