# Bash 文本字段处理：用 `cut` 选择列，用 `tr` 转换分隔符

## 1. 场景

假设文件 `~/accounts` 每行包含一条账户记录，格式为：

```text
账户名:登录 Shell:最后登录日期
```

例如：

```text
alice:/bin/bash:2026-08-30
bob:/bin/zsh:2026-09-01
```

现在希望生成一个只有两列的报告：

```text
alice	/bin/bash
bob	/bin/zsh
```

要求：

* 保留第 1 列：账户名
* 保留第 2 列：登录 Shell
* 删除第 3 列：最后登录日期
* 两列之间使用一个 Tab，而不是冒号

这个任务适合组合使用：

```text
cut → tr
```

核心原则是：

> **先选择需要的字段，再转换字段之间的分隔符。**

---

## 2. `cut`：按照字段位置选择列

`cut` 用于从文本中提取指定字段。

基本形式：

```bash
cut -d':' -f1,2 file
```

其中：

* `-d':'`：指定字段分隔符为 `:`
* `-f1,2`：保留第 1 和第 2 个字段
* `file`：要处理的文件

例如：

```bash
cut -d':' -f1,2 ~/accounts
```

输入：

```text
alice:/bin/bash:2026-08-30
bob:/bin/zsh:2026-09-01
```

输出：

```text
alice:/bin/bash
bob:/bin/zsh
```

第三个字段已经被删除。

### 为什么输出仍然使用冒号？

因为 `cut` 的职责主要是**选择字段**。

指定：

```bash
-d':'
```

以后，`cut` 不仅使用 `:` 识别字段，也会默认使用相同的分隔符连接保留下来的字段。

因此：

```bash
cut -d':' -f1,2
```

得到的是：

```text
field1:field2
```

而不是：

```text
field1<TAB>field2
```

---

## 3. `tr`：逐字符转换

`tr`（translate）用于把一种字符转换成另一种字符。

例如：

```bash
tr ':' '\t'
```

表示：

```text
冒号 → Tab
```

例如输入：

```text
alice:/bin/bash
```

输出：

```text
alice	/bin/bash
```

### `tr` 的特点

`tr` 并不理解：

* 字段
* 列
* CSV/TSV
* 第几个字段

它只处理字符。

因此：

```bash
tr ':' '\t'
```

本质上就是：

> 遇到一个 `:`，就把它换成一个 Tab。

这正适合在字段已经筛选完成之后转换分隔符。

---

## 4. 为什么必须先 `cut`，再 `tr`

正确的数据处理流程是：

```text
原始三列数据
      │
      ▼
cut：只留下第 1、2 列
      │
      ▼
两列、冒号分隔
      │
      ▼
tr：把冒号转换成 Tab
      │
      ▼
两列、Tab 分隔
```

对应命令：

```bash
cut -d':' -f1,2 ~/accounts | tr ':' '\t'
```

例如：

```text
alice:/bin/bash:2026-08-30
```

经过 `cut`：

```text
alice:/bin/bash
```

再经过 `tr`：

```text
alice	/bin/bash
```

---

## 5. 为什么不能直接使用 `tr`

一个常见错误是：

```bash
tr ':' '\t' < ~/accounts
```

如果原始数据为：

```text
alice:/bin/bash:2026-08-30
```

那么所有冒号都会被转换：

```text
alice	/bin/bash	2026-08-30
```

结果仍然有 **3 列**。

虽然看起来已经成为 TSV 格式，但最后登录日期并没有被删除。

因此：

> `tr` 能改变分隔符，却不能选择字段。

正确思路应该是：

```bash
cut -d':' -f1,2 ~/accounts | tr ':' '\t'
```

即：

1. `cut` 负责**选择列**
2. `tr` 负责**修改分隔符**

---

## 6. 管道 `|` 的作用

命令：

```bash
cut -d':' -f1,2 ~/accounts | tr ':' '\t'
```

中的：

```bash
|
```

表示把左侧命令的标准输出连接到右侧命令的标准输入。

数据流可以理解为：

```text
~/accounts
    │
    ▼
cut -d':' -f1,2
    │
    │ stdout
    ▼
tr ':' '\t'
    │
    ▼
最终结果
```

中间结果不需要保存到临时文件。

这也是 Unix 命令行常见的设计方式：

> 每个工具只完成一个简单任务，再通过管道组合起来。

---

## 7. `cut`、`tr` 和 `paste` 的区别

这三个命令都经常用于简单的表格型文本处理，但职责不同。

| 命令      | 主要用途           | 典型问题           |
| ------- | -------------- | -------------- |
| `cut`   | 从一行中选择字段       | “只要第 1、2 列”    |
| `tr`    | 逐字符替换          | “把 `:` 换成 Tab” |
| `paste` | 把多份按行排列的数据横向拼接 | “把两个文件变成两列”    |

### `cut`

适合从已有记录中抽取列：

```bash
cut -d':' -f1,2 file
```

### `tr`

适合字符转换：

```bash
tr ':' '\t'
```

### `paste`

假设：

```text
names.txt
```

内容：

```text
alice
bob
```

以及：

```text
shells.txt
```

内容：

```text
/bin/bash
/bin/zsh
```

执行：

```bash
paste names.txt shells.txt
```

得到：

```text
alice	/bin/bash
bob	/bin/zsh
```

因此：

> `paste` 适合“列原本存在于不同文件”的情况，而不是从同一条记录中删除某些字段。

---

## 8. TSV 文件

TSV 是 **Tab-Separated Values（制表符分隔值）**。

例如：

```text
alice	/bin/bash
bob	/bin/zsh
```

虽然肉眼看起来像若干空格，但列之间实际是一个 Tab 字符：

```text
\t
```

因此：

```bash
tr ':' '\t'
```

可以把冒号分隔的数据转换为 Tab 分隔数据。

要注意：

```text
\t
```

表示 Tab，而普通空格：

```text
 
```

并不等价于 Tab。

---

## 9. 常见误区

### 误区一：直接把所有冒号换成 Tab

```bash
tr ':' '\t' < ~/accounts
```

问题：

* 分隔符确实变成了 Tab
* 但是没有删除第 3 列
* 最终仍然是三列数据

---

### 误区二：认为 `cut -f1,2` 默认处理冒号

```bash
cut -f1,2 ~/accounts
```

`cut` 默认字段分隔符是 **Tab**，不是冒号。

如果源文件使用冒号，必须显式指定：

```bash
cut -d':' -f1,2 ~/accounts
```

---

### 误区三：混淆“选择字段”和“修改字符”

下面两个操作本质不同：

```text
选择第 1、2 个字段
```

属于结构操作，应该使用：

```bash
cut
```

而：

```text
把 : 改成 Tab
```

属于字符转换，应该使用：

```bash
tr
```

不要因为 `tr` 能改变分隔符，就把它当成字段选择工具。

---

## 10. 核心记忆

处理类似文本时，可以先判断自己究竟要做哪一种操作：

```text
从一行中选列        → cut
字符 A 换成字符 B   → tr
多个文件横向拼列    → paste
```

对于：

```text
name:shell:date
```

转换成：

```text
name<TAB>shell
```

正确思路是：

```text
先 cut 删除不需要的字段
再 tr 修改剩余字段之间的分隔符
```

即：

```bash
cut -d':' -f1,2 ~/accounts | tr ':' '\t'
```

---

# Bash 实战：生成 `/home/learner/accounts.tsv`

## 任务

已知：

```text
~/accounts
```

每行格式为：

```text
账户名:登录Shell:最后登录日期
```

需要创建：

```text
/home/learner/accounts.tsv
```

文件中只保留：

1. 账户名
2. 登录 Shell

两列之间使用一个 Tab。

---

## 正确命令

```bash
cut -d':' -f1,2 ~/accounts | tr ':' '\t' > /home/learner/accounts.tsv
```

## 命令拆解

```bash
cut -d':' -f1,2 ~/accounts
```

从冒号分隔的数据中选择第 1、2 个字段。

然后：

```bash
|
```

把结果传给下一条命令。

接着：

```bash
tr ':' '\t'
```

把剩余的冒号转换成 Tab。

最后：

```bash
> /home/learner/accounts.tsv
```

把最终结果写入目标文件。

---

## 数据流

假设输入：

```text
alice:/bin/bash:2026-08-30
bob:/bin/zsh:2026-09-01
```

经过：

```bash
cut -d':' -f1,2 ~/accounts
```

得到：

```text
alice:/bin/bash
bob:/bin/zsh
```

经过：

```bash
tr ':' '\t'
```

得到：

```text
alice	/bin/bash
bob	/bin/zsh
```

最后通过：

```bash
> /home/learner/accounts.tsv
```

保存到文件。

完整命令：

```bash
cut -d':' -f1,2 ~/accounts | tr ':' '\t' > /home/learner/accounts.tsv
```

如果 `~/accounts` 中有两条记录，那么生成的 `/home/learner/accounts.tsv` 也会有两行，每行正好两列。
