# 使用 `awk` 按条件筛选并累加字段

## 1. 适用场景

假设有一个制表符分隔的文件 `~/orders.tsv`，每行代表一笔订单，包含三个字段：

```text
订单ID    金额    状态
```

例如：

```text
1001	120	paid
1002	80	refunded
1003	250	paid
1004	60	failed
1005	100	paid
```

目标是：

* 只选择状态为 `paid` 的订单；
* 将这些订单的金额相加；
* 最终输出一行：

```text
successful revenue: 470
```

并把结果保存到：

```text
/home/learner/revenue.txt
```

这种任务同时包含**条件筛选**和**数值累加**，非常适合使用 `awk`。

---

## 2. `awk` 如何理解一行数据

`awk` 默认一次读取一行输入。

在 `awk` 中：

* 一行叫做一个 **record（记录）**；
* 一行中的各列叫做 **field（字段）**；
* `$1` 表示第 1 个字段；
* `$2` 表示第 2 个字段；
* `$3` 表示第 3 个字段；
* `$0` 表示整行。

对于：

```text
1001	120	paid
```

如果字段分隔符是 Tab，那么：

```text
$1 = 1001
$2 = 120
$3 = paid
```

---

## 3. 使用 `-F` 指定字段分隔符

文件使用 Tab 分隔，因此可以写：

```bash
awk -F '\t' '...' ~/orders.tsv
```

其中：

```text
-F '\t'
```

表示：

> 使用 Tab 作为字段分隔符。

明确指定分隔符通常比依赖 `awk` 默认行为更安全。

### 默认分隔规则为什么可能有问题？

`awk` 默认会把一段连续的空白字符当成字段分隔符，包括：

* 空格；
* Tab；
* 连续多个空白字符。

如果数据严格规定为 TSV（Tab-Separated Values），显式使用：

```bash
-F '\t'
```

可以让程序准确表达：

> 这里就是按照 Tab 分列。

---

## 4. `awk` 的核心结构：`pattern { action }`

典型的 `awk` 程序由若干规则组成：

```awk
pattern { action }
```

含义是：

> 如果当前这一行满足 `pattern`，就执行 `{ action }`。

例如：

```awk
$3 == "paid" { total += $2 }
```

可以拆成两部分。

### 条件：`$3 == "paid"`

```awk
$3 == "paid"
```

检查第 3 个字段是否等于字符串：

```text
paid
```

因此只有已支付订单会满足条件。

### 动作：`total += $2`

```awk
total += $2
```

等价于：

```awk
total = total + $2
```

也就是把当前订单金额加入累计总额。

---

## 5. `awk` 变量不需要提前声明

第一次执行：

```awk
total += $2
```

时，并不需要先写：

```awk
total = 0
```

因为对于数值运算，未初始化的 `awk` 变量可以当作 `0` 使用。

因此：

```awk
$3 == "paid" { total += $2 }
```

就足以完成累计。

当然，如果希望代码意图更加明确，也可以使用：

```awk
BEGIN { total = 0 }
```

但这个例子没有必要。

---

## 6. 为什么需要 `END`

处理完所有订单以后才能得到最终收入，所以不能在每一行处理时直接打印最终结果。

`awk` 提供特殊规则：

```awk
END { ... }
```

它会在**所有输入读取完成之后执行一次**。

因此可以写：

```awk
END { print "successful revenue:", total }
```

执行顺序相当于：

```text
读取第 1 行
→ 如果 paid，则累计

读取第 2 行
→ 如果 paid，则累计

...

读取最后一行
→ 如果 paid，则累计

END
→ 输出最终 total
```

---

## 7. 完整命令

```bash
awk -F '\t' '$3 == "paid" { total += $2 } END { print "successful revenue:", total }' ~/orders.tsv > /home/learner/revenue.txt
```

这条命令完成了整个任务。

---

## 8. 命令逐部分解释

```bash
awk
```

启动 `awk`。

```bash
-F '\t'
```

指定字段由 Tab 分隔。

```awk
$3 == "paid"
```

只选择第 3 列为 `paid` 的记录。

```awk
{ total += $2 }
```

把第 2 列的金额加入 `total`。

```awk
END
```

所有输入处理结束后执行。

```awk
{ print "successful revenue:", total }
```

打印最终结果。

```bash
~/orders.tsv
```

读取订单文件。

```bash
> /home/learner/revenue.txt
```

把 `awk` 的标准输出重定向到目标文件。

最终文件内容类似：

```text
successful revenue: 470
```

---

## 9. `print` 为什么会自动加空格？

下面的代码：

```awk
print "successful revenue:", total
```

向 `print` 传递了两个参数：

```text
"successful revenue:"
total
```

`awk` 默认会用一个空格连接由逗号分隔的 `print` 参数，因此输出：

```text
successful revenue: 470
```

而不是：

```text
successful revenue:470
```

如果需要完全控制格式，也可以使用 `printf`：

```awk
printf "successful revenue: %d\n", total
```

例如完整写法：

```bash
awk -F '\t' '$3 == "paid" { total += $2 } END { printf "successful revenue: %d\n", total }' ~/orders.tsv
```

---

## 10. 为什么这里使用 `awk`，而不是 `cut`、`grep` 或 `sed`

不同命令擅长解决不同问题：

| 工具     | 主要用途             |
| ------ | ---------------- |
| `grep` | 根据模式筛选整行         |
| `cut`  | 提取指定字段           |
| `sed`  | 对文本进行匹配和替换       |
| `awk`  | 按字段处理、条件判断、计算和汇总 |

这个任务需要：

```text
按第 3 列筛选
        ↓
读取第 2 列
        ↓
进行数值加法
        ↓
最后输出汇总结果
```

因此 `awk` 可以在一个程序中自然地完成整个过程。

---

## 11. 常见错误

### 错误一：把字段写成 Bash 变量

错误理解：

```bash
$3
```

是 Bash 的变量。

实际上，这里的：

```awk
$3
```

属于 `awk` 语言，表示当前记录的第 3 个字段。

因为整个 `awk` 程序放在单引号中：

```bash
'$3 == "paid" { ... }'
```

Bash 不会展开 `$3`，而是把它原样交给 `awk`。

这也是为什么 `awk` 程序通常使用单引号包起来。

---

### 错误二：每次匹配都直接打印

例如：

```bash
awk -F '\t' '$3 == "paid" { total += $2; print total }' ~/orders.tsv
```

这样会在每一笔 paid 订单后都打印一次累计值：

```text
120
370
470
```

如果目标只需要最终总数，就应该把打印操作放进：

```awk
END { ... }
```

---

### 错误三：忘记重定向

下面的命令：

```bash
awk -F '\t' '$3 == "paid" { total += $2 } END { print "successful revenue:", total }' ~/orders.tsv
```

只会把结果打印到终端。

如果要求写入文件，需要：

```bash
> /home/learner/revenue.txt
```

完整形式：

```bash
awk -F '\t' '$3 == "paid" { total += $2 } END { print "successful revenue:", total }' ~/orders.tsv > /home/learner/revenue.txt
```

---

## 12. 核心知识总结

最值得记住的是 `awk` 的这个通用模式：

```bash
awk -F '分隔符' '条件 { 累加 } END { 输出结果 }' 文件
```

例如：

```bash
awk -F '\t' '$3 == "paid" { total += $2 } END { print "successful revenue:", total }' ~/orders.tsv
```

其中：

```text
$1、$2、$3      → 第 1、2、3 个字段
-F '\t'         → 使用 Tab 分隔字段
pattern         → 决定哪些记录需要处理
total += $2     → 累加数值
END             → 所有输入处理结束后执行一次
print           → 输出最终结果
> file          → 把结果写入文件
```

`awk` 最核心的思维方式可以概括为：

> **逐行读取 → 按字段判断 → 对匹配记录处理 → 在 `END` 中输出最终汇总结果。**
