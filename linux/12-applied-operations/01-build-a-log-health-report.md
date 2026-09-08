# Bash 日志统计：用 `awk`、`sort`、`uniq` 构建错误分类报表

## 1. 任务目标

假设目录：

```text
/var/log/course/app
```

中有两个服务产生的日志文件。每行日志格式固定：

```text
时间戳 日志级别 分类 消息...
```

例如：

```text
2026-09-08T10:15:01 ERROR database connection timeout
2026-09-08T10:15:02 INFO api request completed
2026-09-08T10:15:03 ERROR auth invalid token
2026-09-08T10:15:04 ERROR database query failed
```

现在需要统计：

> 每种错误分类出现了多少次，并按照出现次数从多到少排列。

最终输出格式为 TSV：

```text
database	2
auth	1
```

其中：

* 第一列：错误分类；
* 第二列：出现次数；
* 两列之间使用一个 Tab；
* 不保留 `uniq -c` 产生的对齐空格。

---

## 2. 整体处理思路

整个任务可以拆成 5 个阶段：

```text
读取所有日志
    ↓
只保留 ERROR 行
    ↓
提取 category 字段
    ↓
统计每个 category 出现次数
    ↓
按次数降序排列并格式化
```

典型管道：

```bash
awk '$2 == "ERROR" { print $3 }' /var/log/course/app/* \
    | sort \
    | uniq -c \
    | sort -nr \
    | awk '{ printf "%s\t%d\n", $2, $1 }'
```

每个工具只负责一个明确的任务。

---

## 3. 为什么使用字段比较，而不是 `grep ERROR`

如果日志结构固定：

```text
$1 = 时间戳
$2 = 日志级别
$3 = 分类
$4... = 消息
```

那么判断一行是不是错误日志，真正应该检查的是：

```text
第二个字段是否等于 ERROR
```

因此：

```bash
awk '$2 == "ERROR" { print $3 }'
```

比：

```bash
grep ERROR
```

更准确。

### `grep ERROR` 的问题

例如日志：

```text
2026-09-08T10:20:00 INFO api previous ERROR was recovered
```

这里日志级别实际上是：

```text
INFO
```

但：

```bash
grep ERROR
```

仍然会匹配，因为消息文本中出现了 `ERROR`。

而：

```bash
awk '$2 == "ERROR"'
```

只检查第二个字段，因此不会误判。

### 原则

> 当数据具有明确字段结构时，优先比较字段，而不是搜索整行文本。

---

## 4. 使用 `awk` 同时筛选和提取字段

命令：

```bash
awk '$2 == "ERROR" { print $3 }' /var/log/course/app/*
```

含义：

```text
$2 == "ERROR"
```

表示：

> 第二个字段必须等于字符串 `ERROR`。

而：

```text
{ print $3 }
```

表示：

> 对满足条件的行，只输出第三个字段。

例如输入：

```text
2026-09-08T10:15:01 ERROR database connection timeout
2026-09-08T10:15:02 INFO api request completed
2026-09-08T10:15:03 ERROR auth invalid token
2026-09-08T10:15:04 ERROR database query failed
```

经过：

```bash
awk '$2 == "ERROR" { print $3 }'
```

得到：

```text
database
auth
database
```

后续命令不再需要关心完整日志内容，只处理 category。

这体现了 Shell pipeline 的核心思想：

> 每一步尽量把数据变成下一步最容易处理的形式。

---

## 5. 为什么 `uniq -c` 前面必须先 `sort`

假设目前数据是：

```text
database
auth
database
network
auth
database
```

不能直接：

```bash
uniq -c
```

因为 `uniq` 只处理**相邻的重复行**。

例如：

```text
database
auth
database
```

两个 `database` 没有相邻，因此不会被统计到同一个组。

### 第一步：排序

```bash
sort
```

得到：

```text
auth
auth
database
database
database
network
```

相同值被排列到了一起。

### 第二步：计数

```bash
uniq -c
```

得到：

```text
      2 auth
      3 database
      1 network
```

因此经典组合是：

```bash
sort | uniq -c
```

而不是单独使用：

```bash
uniq -c
```

---

## 6. 两次 `sort` 的作用完全不同

完整管道里有两个 `sort`：

```bash
... | sort | uniq -c | sort -nr
```

它们虽然是同一个命令，但承担不同任务。

### 第一次 `sort`

```bash
sort
```

目的：

> 把相同 category 放到一起，让 `uniq -c` 能正确统计。

排序依据是 category 本身。

---

### 第二次 `sort`

经过：

```bash
uniq -c
```

数据变成：

```text
      2 auth
      3 database
      1 network
```

此时需要按照第一列的数字排序：

```bash
sort -nr
```

参数：

| 参数   | 含义                  |
| ---- | ------------------- |
| `-n` | numeric，按数字而不是字符串比较 |
| `-r` | reverse，逆序，即从大到小    |

得到：

```text
      3 database
      2 auth
      1 network
```

### 为什么需要 `-n`

普通字符串排序可能把：

```text
10
2
9
```

按照字符顺序处理，而不是按照真正的数值大小。

统计数量时应明确使用：

```bash
sort -n
```

或降序：

```bash
sort -nr
```

---

## 7. 为什么最后还需要一次 `awk`

`uniq -c` 默认输出：

```text
      3 database
      2 auth
      1 network
```

但目标格式要求：

```text
database	3
auth	2
network	1
```

区别有两个：

1. category 应该在前；
2. count 应该在后；
3. 两者之间必须是一个 Tab；
4. 不需要前导空格。

因此使用：

```bash
awk '{ printf "%s\t%d\n", $2, $1 }'
```

这里：

* `$1`：计数；
* `$2`：category；
* `\t`：Tab；
* `\n`：换行。

例如：

```text
      3 database
```

被转换为：

```text
database	3
```

---

## 8. 完整脚本

创建：

```text
/home/learner/bin/log-health
```

内容：

```bash
#!/bin/bash

awk '$2 == "ERROR" { print $3 }' /var/log/course/app/* \
    | sort \
    | uniq -c \
    | sort -nr \
    | awk '{ printf "%s\t%d\n", $2, $1 }'
```

也可以写成单行：

```bash
#!/bin/bash
awk '$2 == "ERROR" { print $3 }' /var/log/course/app/* | sort | uniq -c | sort -nr | awk '{ printf "%s\t%d\n", $2, $1 }'
```

多行版本通常更容易阅读和维护。

---

## 9. 设置执行权限

运行：

```bash
chmod 755 /home/learner/bin/log-health
```

`755` 表示：

```text
owner:  rwx
group:  r-x
others: r-x
```

即：

* 文件所有者可以读、写、执行；
* 其他用户可以读和执行；
* 只有所有者可以修改脚本。

检查：

```bash
ls -l /home/learner/bin/log-health
```

应类似：

```text
-rwxr-xr-x 1 learner learner ... /home/learner/bin/log-health
```

如果需要确认当前文件所有者：

```bash
whoami
```

脚本应由当前用户拥有。

---

## 10. 生成最终报表

执行脚本，并把标准输出重定向到：

```text
/home/learner/error-summary.tsv
```

命令：

```bash
/home/learner/bin/log-health > /home/learner/error-summary.tsv
```

这里：

```text
>
```

表示：

> 将命令的标准输出写入文件，并覆盖文件原来的内容。

查看结果：

```bash
cat /home/learner/error-summary.tsv
```

为了明确观察 Tab，可以使用：

```bash
cat -T /home/learner/error-summary.tsv
```

Tab 通常会显示成：

```text
^I
```

例如：

```text
database^I7
auth^I4
network^I2
```

实际文件中的分隔符仍然是 Tab。

---

## 11. Pipeline 中每一步的数据变化

假设错误分类依次是：

```text
database
auth
database
network
auth
database
```

### 第一步：筛选错误并提取分类

```bash
awk '$2 == "ERROR" { print $3 }'
```

得到：

```text
database
auth
database
network
auth
database
```

### 第二步：按分类排序

```bash
sort
```

得到：

```text
auth
auth
database
database
database
network
```

### 第三步：统计

```bash
uniq -c
```

得到：

```text
      2 auth
      3 database
      1 network
```

### 第四步：按照次数降序排列

```bash
sort -nr
```

得到：

```text
      3 database
      2 auth
      1 network
```

### 第五步：交换字段并输出 TSV

```bash
awk '{ printf "%s\t%d\n", $2, $1 }'
```

得到：

```text
database	3
auth	2
network	1
```

---

## 12. 常见错误

### 错误 1：使用 `grep ERROR`

```bash
grep ERROR /var/log/course/app/*
```

问题：

> 会匹配消息正文中的 `ERROR`，而不仅仅是日志级别字段。

更准确：

```bash
awk '$2 == "ERROR" { print $3 }' /var/log/course/app/*
```

---

### 错误 2：直接使用 `uniq -c`

```bash
awk ... | uniq -c
```

问题：

> 相同 category 如果不相邻，就会被分开统计。

正确：

```bash
awk ... | sort | uniq -c
```

---

### 错误 3：最后使用普通 `sort`

```bash
uniq -c | sort
```

这会主要按照文本排序，而不是按照出现次数。

正确：

```bash
uniq -c | sort -nr
```

---

### 错误 4：直接保存 `uniq -c` 的结果

```bash
uniq -c
```

默认格式是：

```text
      3 database
```

但要求是：

```text
database	3
```

因此需要最后重新格式化：

```bash
awk '{ printf "%s\t%d\n", $2, $1 }'
```

---

## 13. 核心知识总结

这类日志统计任务可以记成：

```text
筛选 → 提取 → 排序 → 计数 → 排名 → 格式化
```

对应 Bash 工具：

```bash
awk | sort | uniq -c | sort -nr | awk
```

关键原则：

* 日志具有固定字段时，用 `awk` 按字段判断，比整行 `grep` 更可靠。
* `uniq` 只能合并相邻重复项，所以通常先执行 `sort`。
* 第一次 `sort` 是为了**分组计数**。
* 第二次 `sort -nr` 是为了**按照数量排名**。
* `printf "%s\t%d\n"` 可以精确生成 TSV 格式。
* Shell pipeline 的重点不是某一个复杂命令，而是让多个简单命令逐步缩小、转换数据。
