# Linux 磁盘空间分析：`du`、`df` 与按大小排序

## 1. `du` 和 `df` 解决的是两个不同问题

磁盘空间不足时，通常需要回答两个问题：

1. **空间被哪些目录或文件占用了？**
2. **整个文件系统还剩多少空间？**

对应的工具分别是：

| 命令   | 关注对象  | 主要用途         |
| ---- | ----- | ------------ |
| `du` | 文件和目录 | 找出“空间去哪了”    |
| `df` | 文件系统  | 查看“磁盘还剩多少空间” |

### `du`：统计目录占用空间

`du`（Disk Usage）会遍历目录树，统计其中可见文件占用的空间。

例如：

```bash
du -sh /srv/course/*
```

可能得到：

```text
420M    /srv/course/data
1.8G    /srv/course/logs
```

常用参数：

* `-s`：summary，只输出每个参数的总大小，不继续展示内部子目录。
* `-h`：human-readable，以 `K`、`M`、`G` 等容易阅读的单位显示。

---

## 2. Shell 通配符 `*` 为什么能得到“一目录一行”

命令：

```bash
du -sh /srv/course/*
```

其中的：

```text
/srv/course/*
```

并不是 `du` 自己处理的。

Shell 会先展开 `*`。例如目录中存在：

```text
/srv/course/data
/srv/course/logs
```

Shell 实际执行的效果类似：

```bash
du -sh /srv/course/data /srv/course/logs
```

因此 `du -s` 会分别统计每个参数，最终得到一行一个目录。

> `*` 通常只匹配当前层级中的内容，不会自动递归展开所有子目录；目录内部的统计则由 `du` 完成。

---

## 3. 按目录大小排序：使用 `sort -h`

如果希望快速找出最大的目录，可以把 `du` 的输出交给 `sort`：

```bash
du -sh /srv/course/* | sort -hr
```

其中：

```text
|
```

是管道（pipe），表示把左边命令的标准输出作为右边命令的标准输入。

### 为什么必须使用 `sort -h`

普通的：

```bash
sort
```

主要按照文本字符排序，并不真正理解：

```text
41K
401K
2M
1G
```

这些值代表的实际大小。

使用：

```bash
sort -h
```

后，`sort` 会理解 `K`、`M`、`G` 等单位，从而按照实际数值排序。

### `-r`：最大值优先

```bash
sort -hr
```

等价于：

```bash
sort -h -r
```

含义：

* `-h`：理解人类可读的大小单位；
* `-r`：reverse，反向排序。

因此最大的目录排在最前面。

例如：

```text
2.1G    /srv/course/logs
420M    /srv/course/data
```

这非常适合排查磁盘空间问题，因为通常首先需要检查最大的目录。

---

## 4. `df`：查看文件系统剩余容量

`df`（Disk Free）查看的是**文件系统整体的容量统计**。

例如：

```bash
df -h /
```

可能得到：

```text
Filesystem      Size  Used Avail Use% Mounted on
/dev/sda1        40G   31G  7.0G  82% /
```

主要字段：

| 字段           | 含义      |
| ------------ | ------- |
| `Size`       | 文件系统总容量 |
| `Used`       | 已使用空间   |
| `Avail`      | 可用空间    |
| `Use%`       | 使用率     |
| `Mounted on` | 挂载位置    |

这里：

```bash
df -h /
```

并不是只统计 `/` 目录中的文件，而是找到**承载 `/` 的文件系统**，然后报告整个文件系统的容量。

---

## 5. 为什么 `du` 和 `df` 的结果可能不同

这是排查磁盘问题时非常重要的现象。

### `du` 的视角

`du` 遍历当前目录树，只统计它能够看到的文件。

### `df` 的视角

`df` 从文件系统本身读取空间分配情况。

因此可能出现：

```text
df：磁盘几乎满了
du：目录中却没有那么多文件
```

一个常见原因是：

> 文件已经被删除，但仍然被某个正在运行的进程打开。

Linux 中删除文件时，如果仍有进程持有该文件的打开句柄：

* 文件名可以已经从目录树消失；
* `du` 无法再看到它；
* 但文件占用的数据块暂时不能释放；
* `df` 因此仍然认为这些空间正在被使用。

所以：

```text
df 显示空间不足
+
du 找不到对应的大文件
```

通常意味着应该进一步检查被删除但仍被进程打开的文件。

例如常见的进一步排查命令是：

```bash
lsof +L1
```

---

## 6. 输出重定向：`>` 与 `>>`

### `>`：覆盖文件

```bash
command > file
```

表示把命令输出写入 `file`。

如果文件原来存在，其内容会被覆盖。

例如：

```bash
du -sh /srv/course/* | sort -hr > /home/learner/disk-usage
```

执行后，文件中保存目录空间统计。

### `>>`：追加到文件末尾

```bash
command >> file
```

表示把输出追加到已有文件末尾，而不会删除之前的内容。

例如：

```bash
df -h / >> /home/learner/disk-usage
```

这会把文件系统容量信息添加到已有的目录统计之后。

因此，当任务要求：

1. 先写入第一部分；
2. 再添加第二部分；
3. 第二部分不能覆盖第一部分；

典型做法就是：

```bash
first_command > file
second_command >> file
```

---

## 7. 典型磁盘空间排查思路

遇到磁盘空间不足时，可以按以下顺序检查：

```bash
df -h /
```

先确认整个文件系统是否真的接近满载。

然后使用：

```bash
du -sh /path/* | sort -hr
```

找出最大的目录。

核心思路是：

```text
df → 还有多少空间？
du → 空间被谁用了？
sort -hr → 最大的目录是谁？
```

如果：

```text
df 显示磁盘满
但 du 找不到足够大的文件
```

则需要考虑：

```text
已删除但仍被进程打开的文件
```

而不是简单认为 `du` 或 `df` 出错。

---

# 问题解答：生成 `/home/learner/disk-usage`

## 问题

创建文件：

```text
/home/learner/disk-usage
```

内容依次包含：

1. `/srv/course` 下两个已有目录的磁盘占用；
2. 按大小从大到小排列；
3. 然后添加承载 `/` 的文件系统容量信息；
4. 第二部分必须追加，不能覆盖第一部分。

## 命令

首先生成目录空间统计：

```bash
du -sh /srv/course/* | sort -hr > /home/learner/disk-usage
```

然后追加 `/` 所在文件系统的容量：

```bash
df -h / >> /home/learner/disk-usage
```

## 命令拆解

第一条：

```bash
du -sh /srv/course/* | sort -hr > /home/learner/disk-usage
```

含义：

```text
du -sh /srv/course/*
│  │        │
│  │        └─ Shell 展开为 /srv/course 下的各个目录
│  └─ 汇总每个目录，并使用 K/M/G 等易读单位
└─ 统计磁盘占用
```

然后：

```text
| sort -hr
```

表示：

* `-h`：按实际容量排序；
* `-r`：从大到小排列。

最后：

```text
> /home/learner/disk-usage
```

创建或覆盖目标文件，写入第一部分。

第二条：

```bash
df -h / >> /home/learner/disk-usage
```

其中：

* `df -h /`：显示承载 `/` 的文件系统容量；
* `>>`：把结果追加到文件末尾。

关键区别：

```text
>   覆盖
>>  追加
```

因此第二条命令必须使用 `>>`，否则第一条命令生成的目录统计会被删除。

最终核心命令为：

```bash
du -sh /srv/course/* | sort -hr > /home/learner/disk-usage
df -h / >> /home/learner/disk-usage
```
