# gzip、bzip2 与 xz：Linux 单文件压缩格式

## 1. 核心概念

在 Debian/Linux 系统中，常见的三种单文件压缩工具是：

* `gzip`
* `bzip2`
* `xz`

它们解决的是同一个问题：

> **把一个文件压缩成更小的文件。**

它们不会像 `tar` 一样把多个文件或整个目录结构打包到一起。

---

## 2. 三种工具的基本行为

### gzip

```bash
gzip data.txt
```

执行后：

```text
data.txt
↓
data.txt.gz
```

默认情况下，原始的 `data.txt` 会被删除，只留下压缩后的：

```text
data.txt.gz
```

解压可以使用：

```bash
gunzip data.txt.gz
```

或者：

```bash
gzip -d data.txt.gz
```

---

### bzip2

压缩：

```bash
bzip2 data.txt
```

结果：

```text
data.txt
↓
data.txt.bz2
```

解压：

```bash
bunzip2 data.txt.bz2
```

或者：

```bash
bzip2 -d data.txt.bz2
```

---

### xz

压缩：

```bash
xz data.txt
```

结果：

```text
data.txt
↓
data.txt.xz
```

解压：

```bash
unxz data.txt.xz
```

或者：

```bash
xz -d data.txt.xz
```

---

## 3. 默认会删除原文件

这三个工具默认都有一个很重要的行为：

```bash
gzip data.txt
```

并不是生成一份额外的压缩副本，而是把：

```text
data.txt
```

替换为：

```text
data.txt.gz
```

因此压缩后通常不会同时存在：

```text
data.txt
data.txt.gz
```

而只会留下：

```text
data.txt.gz
```

如果需要保留原文件，可以使用相应的保留选项。例如：

```bash
gzip -k data.txt
bzip2 -k data.txt
xz -k data.txt
```

其中 `-k` 表示 **keep，保留原文件**。

---

## 4. 压缩工具与 tar 的分工

`gzip`、`bzip2` 和 `xz` 本身都只处理一个文件。

例如不能依靠 `gzip` 本身保存这样的目录结构：

```text
project/
├── config.ini
├── README.md
└── src/
    └── main.py
```

这是 `tar` 的工作。

可以把它们的职责理解为：

```text
tar
↓
把多个文件和目录结构打包成一个文件

gzip / bzip2 / xz
↓
把一个文件压缩得更小
```

因此：

```text
多个文件
    ↓ tar
archive.tar
    ↓ gzip
archive.tar.gz
```

这里：

* `.tar` 表示它是一个 tar 归档文件；
* `.gz` 表示这个 tar 文件又经过了 gzip 压缩。

---

## 5. 如何理解 `.tar.gz`、`.tar.bz2` 和 `.tar.xz`

常见文件名：

```text
archive.tar.gz
archive.tar.bz2
archive.tar.xz
```

可以从右向左理解。

### `archive.tar.gz`

```text
原始文件树
→ tar
archive.tar
→ gzip
archive.tar.gz
```

### `archive.tar.bz2`

```text
原始文件树
→ tar
archive.tar
→ bzip2
archive.tar.bz2
```

### `archive.tar.xz`

```text
原始文件树
→ tar
archive.tar
→ xz
archive.tar.xz
```

所以文件扩展名实际上记录了处理过程。

---

## 6. gzip、bzip2、xz 的主要差异

一般可以粗略理解为：

| 格式    | 压缩速度 | 压缩率 | 常见扩展名  |
| ----- | ---- | --- | ------ |
| gzip  | 快    | 较低  | `.gz`  |
| bzip2 | 中等   | 中等  | `.bz2` |
| xz    | 较慢   | 较高  | `.xz`  |

也就是说，大体趋势是：

```text
压缩速度：

gzip > bzip2 > xz
```

而压缩效果通常相反：

```text
压缩率：

xz > bzip2 > gzip
```

这里的“压缩率更高”意味着最终文件通常更小，但具体结果取决于文件内容。

对于很小的文件，这些差异往往没有实际意义。

---

## 7. 为什么不能永远选择 xz

即使 `xz` 经常可以得到更小的文件，也不代表它永远是最佳选择。

现实中压缩格式通常由接收方决定。

例如某个系统要求：

```text
.gz
```

那么即使 `.xz` 更小，也不能随意改成 `.xz`。

因此应优先考虑：

> **下游程序、服务器、协议或用户要求什么格式。**

压缩格式经常是接口的一部分，而不是单纯的个人偏好。

---

## 8. 常用命令总结

### 压缩

```bash
gzip file
bzip2 file
xz file
```

结果分别为：

```text
file.gz
file.bz2
file.xz
```

### 解压

```bash
gunzip file.gz
bunzip2 file.bz2
unxz file.xz
```

或者：

```bash
gzip -d file.gz
bzip2 -d file.bz2
xz -d file.xz
```

### 保留原文件

```bash
gzip -k file
bzip2 -k file
xz -k file
```

---

## 9. 关键记忆

```text
tar = 打包多个文件和目录结构
gzip / bzip2 / xz = 压缩一个文件
```

典型流程：

```text
目录树
→ tar
一个 .tar 文件
→ gzip / bzip2 / xz
压缩后的归档文件
```

不要把“打包”和“压缩”混为一谈。

---

# 操作题：分别使用 gzip、bzip2 和 xz 压缩三个文件

## 题目

需要进行以下操作：

* `/home/learner/data-a.txt` 使用 `gzip`
* `/home/learner/data-b.txt` 使用 `bzip2`
* `/home/learner/data-c.txt` 使用 `xz`

压缩后的三个文件都应留在 `/home/learner` 目录中。

---

## 正确命令

```bash
gzip /home/learner/data-a.txt
bzip2 /home/learner/data-b.txt
xz /home/learner/data-c.txt
```

执行后会得到：

```text
/home/learner/data-a.txt.gz
/home/learner/data-b.txt.bz2
/home/learner/data-c.txt.xz
```

原来的：

```text
/home/learner/data-a.txt
/home/learner/data-b.txt
/home/learner/data-c.txt
```

默认都会被删除。

---

## 也可以先进入 home 目录

```bash
cd /home/learner

gzip data-a.txt
bzip2 data-b.txt
xz data-c.txt
```

结果完全相同：

```text
/home/learner/data-a.txt.gz
/home/learner/data-b.txt.bz2
/home/learner/data-c.txt.xz
```

---

## 验证结果

可以运行：

```bash
ls -l /home/learner
```

应当能看到类似：

```text
data-a.txt.gz
data-b.txt.bz2
data-c.txt.xz
```

三个文件的扩展名分别对应所要求的压缩格式。
