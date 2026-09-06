# Linux 磁盘分区基础：使用 `fdisk` 创建 `/dev/sda1`

## 1. 磁盘、分区与文件系统的关系

Linux 存储通常可以理解为三个层次：

```text
磁盘
/dev/sda
   ↓ 分区
/dev/sda1
   ↓ 创建文件系统
ext4 / xfs / ...
   ↓ 挂载
/mnt/data
   ↓
文件和目录
```

这几个概念不能混为一谈：

| 层次   | 示例          | 作用                |
| ---- | ----------- | ----------------- |
| 磁盘   | `/dev/sda`  | 一整块块设备            |
| 分区   | `/dev/sda1` | 从磁盘中划出的一段空间       |
| 文件系统 | ext4、XFS    | 定义文件如何存储和管理       |
| 挂载点  | `/mnt/data` | 把文件系统接入 Linux 目录树 |

因此：

> **创建分区并不等于创建文件系统，也不等于已经可以存文件。**

本阶段只处理：

```text
/dev/sda → /dev/sda1
```

---

## 2. 如何判断一块磁盘目前没有被使用

可以使用：

```bash
lsblk
```

例如：

```text
NAME MAJ:MIN RM SIZE RO TYPE MOUNTPOINTS
sda    8:0    0  10G  0 disk
```

这里有几个重要信号：

* `sda` 的类型是 `disk`
* 下面没有 `sda1`、`sda2` 等分区
* `MOUNTPOINTS` 为空

说明 `/dev/sda` 当前没有分区，也没有挂载。

如果系统的根目录 `/` 并不是来自 `/dev/sda`，那么 `/dev/sda` 可能是一块额外的数据盘或练习盘。

在实际机器上执行分区命令前，应再次确认目标磁盘：

```bash
lsblk
```

因为：

> 对错误的磁盘执行分区操作可能破坏原有数据。

---

## 3. 什么是分区表

一块原始磁盘本质上可以看成大量连续的 **sector（扇区）**：

```text
sector 0
sector 1
sector 2
...
sector N
```

Linux 如果不知道哪些扇区属于哪个用途，就只能把它看成一整块原始设备。

**分区表（partition table）** 用来记录：

> 哪一段扇区属于哪个分区。

例如：

```text
/dev/sda
┌───────────────────────────────────────┐
│ partition table                       │
├───────────────────────────────────────┤
│ /dev/sda1                             │
│ sector 2048 → sector 20971519         │
└───────────────────────────────────────┘
```

于是系统就能看到：

```text
/dev/sda
└─/dev/sda1
```

需要注意：

> 分区表只记录分区的边界，并不负责决定分区里面采用什么文件系统。

创建 `/dev/sda1` 后，还需要类似：

```bash
mkfs.ext4 /dev/sda1
```

才能创建 ext4 文件系统。

---

## 4. `/dev/sda` 与 `/dev/sda1`

设备名中的数字代表分区编号：

```text
/dev/sda      整块磁盘
/dev/sda1     第 1 个分区
/dev/sda2     第 2 个分区
/dev/sda3     第 3 个分区
```

可以把它类比为：

```text
整块硬盘
└── 分区 1
```

因此不要把：

```bash
/dev/sda
```

和：

```bash
/dev/sda1
```

当成同一个东西。

通常：

* `fdisk` 操作的是整块磁盘 `/dev/sda`
* `mkfs` 操作的是具体分区 `/dev/sda1`
* `mount` 通常也挂载具体分区 `/dev/sda1`

---

## 5. `fdisk` 是什么

`fdisk` 是 Linux 中用于管理磁盘分区表的工具。

进入交互模式：

```bash
sudo fdisk /dev/sda
```

这里指定：

```text
/dev/sda
```

而不是：

```text
/dev/sda1
```

因为现在要修改的是整块磁盘上的分区表。

进入后会出现类似：

```text
Command (m for help):
```

然后通过单个字母输入命令。

---

## 6. `fdisk` 中最重要的几个命令

| 命令  | 含义               |
| --- | ---------------- |
| `p` | Print，显示当前分区表    |
| `n` | New，创建新分区        |
| `d` | Delete，删除分区      |
| `w` | Write，把修改写入磁盘并退出 |
| `q` | Quit，不保存修改并退出    |
| `m` | 显示帮助             |

其中最重要的是理解：

```text
w = 保存
q = 放弃
```

---

## 7. `fdisk` 为什么相对适合交互式操作

`fdisk` 中的大多数修改在操作过程中只是暂存在内存里。

例如：

```text
打开 fdisk
    ↓
创建分区
    ↓
删除分区
    ↓
重新创建
```

在输入：

```text
w
```

之前，这些修改通常还没有真正写入磁盘分区表。

因此可以先查看磁盘：

```bash
sudo fdisk /dev/sda
```

输入：

```text
p
```

查看当前状态，然后：

```text
q
```

退出。

这种情况下不会保存修改。

但是不要因此认为 `fdisk` “没有风险”。一旦执行：

```text
w
```

修改就会真正写入磁盘。

---

## 8. 创建一个占满整块磁盘的分区

目标：

```text
/dev/sda
└── /dev/sda1
```

并让 `/dev/sda1` 使用几乎全部可用空间。

首先进入：

```bash
sudo fdisk /dev/sda
```

### 第一步：创建新分区

输入：

```text
n
```

即：

```text
n = new partition
```

---

### 第二步：选择分区类型

在传统 MBR/DOS 分区表中，可能出现：

```text
Partition type
   p   primary
   e   extended
Select (default p):
```

直接按 Enter：

```text
Enter
```

表示接受默认值：

```text
primary
```

即创建主分区。

> 在 GPT 分区表下，交互提示可能有所不同，也通常不再强调 primary/extended 的区别。

---

### 第三步：选择分区编号

可能出现：

```text
Partition number (1-4, default 1):
```

直接按 Enter：

```text
Enter
```

于是创建：

```text
/dev/sda1
```

如果选择 `2`，则会得到：

```text
/dev/sda2
```

---

### 第四步：选择起始扇区

可能出现：

```text
First sector (..., default 2048):
```

直接按 Enter。

通常应该接受默认值，因为 `fdisk` 会自动选择合理对齐的位置。

---

### 第五步：选择结束扇区

随后可能看到：

```text
Last sector, +/-sectors or +/-size{K,M,G,T,P} (...):
```

如果希望让这个分区占据剩余全部磁盘空间，直接：

```text
Enter
```

接受默认的最后扇区。

因此，一个“整盘单分区”的典型操作基本就是：

```text
n
Enter
Enter
Enter
Enter
```

具体提示取决于磁盘当前的分区表类型。

---

## 9. 写入修改

创建完成后，可以先输入：

```text
p
```

检查即将保存的分区表。

确认无误后：

```text
w
```

`w` 表示：

```text
write
```

它会：

1. 把新的分区表写入磁盘；
2. 通知内核重新读取分区信息；
3. 退出 `fdisk`。

这一步之后修改才真正生效。

---

## 10. 如果发现操作错误

只要还没有执行：

```text
w
```

通常可以输入：

```text
q
```

直接退出。

`q` 表示：

```text
quit without saving
```

例如：

```bash
sudo fdisk /dev/sda
```

进入后：

```text
p
n
...
```

突然发现磁盘选错了，此时不要执行 `w`，而应：

```text
q
```

---

## 11. 完整操作示例

创建一个占满 `/dev/sda` 的第一分区：

```bash
sudo fdisk /dev/sda
```

交互过程可以概括为：

```text
Command: n
Partition type: Enter
Partition number: Enter
First sector: Enter
Last sector: Enter

Command: p
Command: w
```

逻辑相当于：

```text
n
→ 创建新分区

默认分区类型
→ 使用默认值

默认编号 1
→ /dev/sda1

默认起始 sector
→ 使用 fdisk 推荐位置

默认结束 sector
→ 使用剩余全部空间

p
→ 检查结果

w
→ 正式保存
```

---

## 12. 如何确认 `/dev/sda1` 已经创建

完成后运行：

```bash
lsblk
```

预期看到类似：

```text
NAME   MAJ:MIN RM SIZE RO TYPE MOUNTPOINTS
sda      8:0    0  10G  0 disk
└─sda1   8:1    0  10G  0 part
```

关键是：

```text
sda
└─sda1
```

并且：

```text
TYPE
disk
part
```

分别说明：

* `/dev/sda` 是磁盘；
* `/dev/sda1` 是分区。

也可以直接检查设备节点：

```bash
ls -l /dev/sda1
```

---

## 13. 创建分区后为什么还不能直接存文件

此时完成的只是：

```text
raw disk
   ↓
partition table
   ↓
/dev/sda1
```

`/dev/sda1` 目前只是“一段磁盘空间”。

还没有：

```text
目录
文件名
文件内容组织结构
inode
空闲空间管理
```

这些功能由**文件系统**提供。

因此下一阶段通常是：

```bash
sudo mkfs.ext4 /dev/sda1
```

然后再创建挂载点，例如：

```bash
sudo mkdir -p /mnt/data
```

最后：

```bash
sudo mount /dev/sda1 /mnt/data
```

完整流程是：

```text
/dev/sda
   │
   │ fdisk
   ▼
/dev/sda1
   │
   │ mkfs.ext4
   ▼
ext4 文件系统
   │
   │ mount
   ▼
/mnt/data
   │
   ▼
可以正常创建文件
```

---

## 14. 常见误区

### 误区一：`/dev/sda1` 是另一块硬盘

不是。

```text
/dev/sda  = 整块磁盘
/dev/sda1 = /dev/sda 上的第一个分区
```

---

### 误区二：分区完成就可以写文件

不一定。

创建分区只是划分空间：

```bash
fdisk
```

还需要创建文件系统：

```bash
mkfs.ext4 /dev/sda1
```

然后挂载：

```bash
mount /dev/sda1 /mnt/data
```

---

### 误区三：`fdisk` 每输入一个命令就立即修改磁盘

通常不是。

交互中的修改先保存在内存中，直到执行：

```text
w
```

才写入磁盘。

要放弃修改：

```text
q
```

---

### 误区四：起始和结束 sector 应该自己计算

对于“创建一个占满整块磁盘的普通分区”，通常没有必要。

接受 `fdisk` 默认值即可：

```text
First sector: Enter
Last sector:  Enter
```

默认值通常已经考虑了合理的分区对齐。

---

### 误区五：看到 `/dev/sda` 就可以直接操作

设备名本身不能证明这是一块安全的空盘。

执行危险操作前至少检查：

```bash
lsblk
```

最好进一步确认：

```bash
findmnt
```

确认系统当前正在使用哪些设备。

---

## 15. 核心命令速查

```bash
# 查看磁盘和分区
lsblk

# 编辑 /dev/sda 的分区表
sudo fdisk /dev/sda
```

`fdisk` 交互命令：

```text
p    查看分区表
n    创建分区
d    删除分区
w    保存并退出
q    不保存退出
```

创建单个占满磁盘的分区后再次确认：

```bash
lsblk
```

目标结果：

```text
/dev/sda
└── /dev/sda1
```

## 16. 核心记忆模型

```text
磁盘 ≠ 分区 ≠ 文件系统 ≠ 挂载点
```

具体对应：

```text
/dev/sda
   ↓ fdisk
/dev/sda1
   ↓ mkfs
ext4
   ↓ mount
/mnt/data
```

其中 `fdisk` 最重要的两个安全意识是：

```text
w = 真正写入修改
q = 放弃修改
```

在执行 `w` 之前，应始终确认自己操作的是正确磁盘。
