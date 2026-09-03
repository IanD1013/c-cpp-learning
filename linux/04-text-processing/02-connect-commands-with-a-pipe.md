# Bash 管道 `|`、`grep` 与 `wc -l`

## 1. 管道解决什么问题

在 Shell 中，一个常见需求是：

1. 用一个命令筛选数据；
2. 再用另一个命令处理筛选结果。

例如，要统计日志中包含 `FAILED` 的记录数，可以先这样做：

```bash
grep 'FAILED' /var/log/course/auth.log > /tmp/failed.log
wc -l /tmp/failed.log
rm /tmp/failed.log
```

这种方法虽然可行，但临时文件只是为了在两个命令之间传递数据，没有必要真正写入磁盘。

Bash 的**管道（pipeline）**可以直接把两个命令连接起来：

```bash
grep 'FAILED' /var/log/course/auth.log | wc -l
```

核心含义是：

```text
grep 的 stdout
       |
       v
wc 的 stdin
```

也就是：

> `|` 将左侧命令的标准输出（stdout）连接到右侧命令的标准输入（stdin）。

数据可以直接在命令之间流动，不需要临时文件。

---

## 2. `grep`：筛选需要的行

命令：

```bash
grep 'FAILED' /var/log/course/auth.log
```

表示：

* 从 `/var/log/course/auth.log` 读取内容；
* 找出包含 `FAILED` 的行；
* 将这些行输出到标准输出。

例如日志内容：

```text
SUCCESS alice
FAILED bob
SUCCESS carol
FAILED dave
FAILED eve
```

执行：

```bash
grep 'FAILED' auth.log
```

得到：

```text
FAILED bob
FAILED dave
FAILED eve
```

因此，`grep` 可以理解为：

> 从文本中筛选符合条件的行。

---

## 3. `wc -l`：统计行数

`wc` 是 **word count** 命令，可以统计输入中的行数、单词数和字节数。

其中：

```bash
wc -l
```

只统计**行数（lines）**。

例如：

```bash
printf 'a\nb\nc\n' | wc -l
```

输出：

```text
3
```

因此：

```bash
grep 'FAILED' /var/log/course/auth.log | wc -l
```

实际上完成了两个步骤：

```text
auth.log
   |
   v
grep 'FAILED'
   |
   | 只剩失败记录
   v
wc -l
   |
   v
失败记录数量
```

---

## 4. 为什么通过管道使用 `wc -l` 很方便

下面两种写法的输出格式存在一个重要区别。

### 直接把文件名传给 `wc`

```bash
wc -l auth.log
```

可能输出：

```text
42 auth.log
```

因为 `wc` 不仅显示数量，还会显示它统计的是哪个文件。

### 从标准输入读取

```bash
cat auth.log | wc -l
```

输出通常只有：

```text
42
```

因为 `wc` 没有接收到文件名，它只是在读取 stdin，因此没有文件名可以附加到结果后面。

所以在：

```bash
grep 'FAILED' /var/log/course/auth.log | wc -l
```

中，`wc -l` 接收到的是来自管道的 stdin，最终得到的是一个适合继续保存或处理的纯数字：

```text
3
```

---

## 5. 管道与重定向的组合

如果不仅要统计，还要把最终结果保存到文件，可以在整个管道的最后使用输出重定向：

```bash
grep 'FAILED' input.log | wc -l > count.txt
```

执行顺序可以理解为：

```text
input.log
   |
   v
grep
   |
   v
wc -l
   |
   v
count.txt
```

其中：

* `grep` 的 stdout 被 `|` 接走；
* `wc -l` 从 stdin 读取这些内容；
* `wc -l` 的 stdout 再被 `>` 写入文件。

因此：

> 重定向应该放在真正产生最终结果的命令之后。

---

## 6. 为什么不能在管道中间重定向

错误思路：

```bash
grep 'FAILED' input.log > matches.txt | wc -l
```

这里：

```bash
> matches.txt
```

已经把 `grep` 的标准输出重定向到了文件。

于是本来应该流入：

```bash
wc -l
```

的数据被截走了。

可以理解为两个操作都想控制 `grep` 的 stdout：

```text
                -> matches.txt
grep stdout ----X
                -> wc -l
```

如果目标是让 `wc` 处理 `grep` 的结果，就应该保留管道：

```bash
grep 'FAILED' input.log | wc -l
```

如果最后还需要保存结果，再重定向 `wc` 的 stdout：

```bash
grep 'FAILED' input.log | wc -l > count.txt
```

---

## 7. `>` 会覆盖目标文件

命令：

```bash
command > file
```

表示：

> 把 `command` 的标准输出写入 `file`。

如果文件已经存在，其原有内容通常会被覆盖。

例如：

```bash
grep 'FAILED' auth.log | wc -l > failed-count
```

运行后，`failed-count` 中只保存本次统计结果。

如果输出是：

```text
3
```

文件实际包含：

```text
3\n
```

也就是数字 `3` 后跟一个换行符。

---

## 8. 管道的重要思维方式

管道的关键不是记住某个特定命令组合，而是理解：

> 一个命令负责产生数据，另一个命令负责消费数据。

通用结构：

```bash
producer | processor
```

例如：

```bash
grep 'ERROR' app.log | wc -l
```

```bash
ps aux | grep nginx
```

```bash
printf '%s\n' apple banana orange | grep 'an'
```

复杂一些时，还可以连接多个命令：

```bash
command1 | command2 | command3
```

数据流为：

```text
command1 stdout
      ↓
command2 stdin
      ↓
command2 stdout
      ↓
command3 stdin
```

这种设计是 Unix Shell 的核心思想之一：让每个小工具只负责一件事情，再通过管道组合起来。

---

## 9. 核心总结

```bash
grep 'FAILED' file | wc -l
```

可以拆解为：

| 部分                   | 作用                     |
| -------------------- | ---------------------- |
| `grep 'FAILED' file` | 筛选包含 `FAILED` 的行       |
| `\|`                 | 把左侧 stdout 连接到右侧 stdin |
| `wc -l`              | 统计收到的行数                |
| `> output`           | 把最终结果写入文件              |

典型模式：

```bash
筛选命令 | 处理命令 > 最终文件
```

管道的最大价值是：

* 不需要临时文件；
* 数据可以直接在进程之间传递；
* 可以把多个简单命令组合成完整的数据处理流程。

---

# 问题解答：统计失败登录次数并写入文件

## 问题

统计 `/var/log/course/auth.log` 中包含 `FAILED` 的记录数量，并将数字写入：

```text
/home/learner/failed-count
```

要求：

* 文件只保存统计数字和一个换行符；
* 不使用临时文件。

## 命令

```bash
grep 'FAILED' /var/log/course/auth.log | wc -l > /home/learner/failed-count
```

## 工作过程

```text
/var/log/course/auth.log
        ↓
grep 'FAILED'
        ↓
筛选出所有 FAILED 行
        ↓
wc -l
        ↓
统计行数
        ↓
> /home/learner/failed-count
```

由于 `wc -l` 的输入来自管道，而不是直接指定的文件名，因此输出只有数字，不会附带文件名。

例如有 2 条失败记录时，目标文件内容为：

```text
2
```

其中末尾包含正常的换行符。

最终所需命令就是：

```bash
grep 'FAILED' /var/log/course/auth.log | wc -l > /home/learner/failed-count
```
