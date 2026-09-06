# Linux 挂载点、块设备与文件系统信息：`findmnt`、`lsblk`、`stat -f`

## 1. Linux 路径背后可能属于不同文件系统

Linux 看起来只有一棵统一的目录树：

```text
/
├── home
├── var
├── boot
└── mnt
```

但这棵树实际上可以由多个不同的文件系统拼接而成。

例如：

```text
/dev/sda2  -> /
/dev/sdb1  -> /data
/dev/sdc1  -> /backup
```

访问：

```text
/data/project/file.txt
```

时，路径本身并不会告诉你 `/data` 是另一个文件系统的挂载点。

这种“把一个文件系统接到目录树某个目录上”的过程叫做 **挂载（mount）**。

因此，排查存储问题时要区分三个问题：

| 问题                     | 命令        |
| ---------------------- | --------- |
| 某个路径属于哪个挂载的文件系统？       | `findmnt` |
| 系统有哪些磁盘、分区等块设备？        | `lsblk`   |
| 某个路径背后的文件系统有什么属性和剩余资源？ | `stat -f` |

---

## 2. `findmnt`：查询路径属于哪个挂载

`findmnt` 读取系统的挂载信息，回答的是：

> **什么文件系统，被挂载到了什么位置？**

例如：

```bash
findmnt /
```

可能得到：

```text
TARGET SOURCE    FSTYPE OPTIONS
/      /dev/sda2 ext4   rw,relatime
```

这里：

* `TARGET`：挂载点；
* `SOURCE`：文件系统来源；
* `FSTYPE`：文件系统类型；
* `OPTIONS`：挂载选项。

### 查询某个路径实际属于哪个挂载

例如：

```bash
findmnt /home/learner
```

`findmnt` 会找到包含该路径的文件系统，而不要求这个路径本身就是挂载点。

例如：

```text
TARGET SOURCE    FSTYPE
/      /dev/sda2 ext4
```

说明：

```text
/home/learner
```

实际上仍然位于根文件系统 `/` 中。

---

## 3. 用 `-o` 和 `-n` 控制 `findmnt` 输出

### `-o`：指定输出字段

例如：

```bash
findmnt -o SOURCE,TARGET,FSTYPE /
```

输出：

```text
SOURCE    TARGET FSTYPE
/dev/sda2 /      ext4
```

只显示：

```text
SOURCE
TARGET
FSTYPE
```

### `-n`：取消表头

```bash
findmnt -n -o SOURCE,TARGET,FSTYPE /
```

输出可能变成：

```text
/dev/sda2 / ext4
```

这种形式非常适合：

* shell 脚本；
* 重定向到文件；
* 作为其他程序的输入。

### 常用写法

```bash
findmnt -n -o SOURCE,TARGET,FSTYPE /
```

可以理解为：

> 查找 `/` 所属的挂载，只输出来源、挂载点和文件系统类型，并且不要表头。

---

## 4. `lsblk`：查看系统中的块设备

`lsblk` 的重点不是挂载点，而是 **块设备（block device）**。

块设备主要包括：

* 硬盘；
* SSD；
* 虚拟磁盘；
* 分区；
* LVM 逻辑卷等。

执行：

```bash
lsblk
```

可能得到：

```text
NAME   MAJ:MIN RM  SIZE RO TYPE MOUNTPOINTS
sda      8:0    0   40G  0 disk
├─sda1   8:1    0    1G  0 part /boot
└─sda2   8:2    0   39G  0 part /
sdb      8:16   0  100G  0 disk
└─sdb1   8:17   0  100G  0 part
```

这里可以看到：

```text
sdb1
```

虽然存在，但没有挂载点。

这正是 `lsblk` 和 `findmnt` 的重要区别：

* `findmnt`：主要看**已经挂载的文件系统**；
* `lsblk`：看内核发现的**块设备，无论是否挂载**。

因此，如果你怀疑：

> “磁盘明明存在，为什么目录树里找不到？”

应该先看：

```bash
lsblk
```

---

## 5. `findmnt` 与 `lsblk` 的区别

例如系统中存在：

```text
/dev/sdb1
```

但它尚未挂载。

此时：

```bash
lsblk
```

可以看到它。

但：

```bash
findmnt
```

通常不会把它作为一个已挂载文件系统列出来。

可以简单记忆：

```text
lsblk
  ↓
系统有哪些磁盘/分区？

findmnt
  ↓
这些文件系统挂载到了哪里？
```

---

## 6. `stat -f`：查询路径背后的文件系统

普通的：

```bash
stat file.txt
```

查看的是**文件本身**的信息，例如：

* 大小；
* inode；
* 权限；
* 修改时间。

而：

```bash
stat -f file.txt
```

查看的是：

> **这个文件所在的文件系统。**

例如：

```bash
stat -f /
```

可能输出：

```text
  File: "/"
    ID: ... Namelen: 255     Type: ext2/ext3
Block size: 4096       Fundamental block size: 4096
Blocks: Total: 10240000   Free: 5000000   Available: 4500000
Inodes: Total: 2621440    Free: 2500000
```

这里关注的是整个文件系统，而不是 `/` 这个目录本身。

---

## 7. `stat` 和 `stat -f` 不要混淆

这是一个非常常见的错误。

### 查看文件本身

```bash
stat /etc/passwd
```

回答：

> `/etc/passwd` 这个文件是什么情况？

### 查看文件所在的文件系统

```bash
stat -f /etc/passwd
```

回答：

> `/etc/passwd` 所在的文件系统是什么情况？

可以记成：

```text
stat PATH
    → 文件

stat -f PATH
    → filesystem（文件系统）
```

---

## 8. `stat -f` 中的重要信息

`stat -f` 通常会提供：

### 文件系统类型

例如：

```text
Type: ext2/ext3
```

或其他文件系统名称。

### Block size

```text
Block size: 4096
```

文件系统以块为单位管理存储空间。

### Blocks

例如：

```text
Blocks:
    Total:     ...
    Free:      ...
    Available: ...
```

可以了解：

* 总空间；
* 空闲空间；
* 普通用户可用空间。

### Inodes

例如：

```text
Inodes:
    Total: ...
    Free:  ...
```

inode 用于记录文件和目录的元数据。

因此磁盘即使还有容量，也可能因为 inode 用光而无法创建新文件。

---

## 9. `findmnt` 和 `stat -f` 的文件系统类型名称可能不同

有时会出现这种情况：

```bash
findmnt /
```

显示一种文件系统类型名称，而：

```bash
stat -f /
```

显示另一个名称。

这不一定表示系统出现异常。

原因是两个工具获取和展示文件系统类型的方式不同，同一个底层文件系统或虚拟文件系统可能使用不同的名称表示。

因此，不应该简单地认为：

```text
两个名称不完全相同
=
两个不同的文件系统
```

应该结合：

* `SOURCE`
* `TARGET`
* 系统环境
* 文件系统实现

综合判断。

---

## 10. 根文件系统的来源不一定是 `/dev/...`

在传统 Linux 主机上，根文件系统经常来自磁盘分区，例如：

```text
/dev/sda2
```

但这不是绝对的。

在以下环境中：

* 容器；
* 虚拟机；
* 沙箱；
* 网络文件系统；
* 主机共享文件系统；

根文件系统的 `SOURCE` 可能不是：

```text
/dev/sda1
/dev/vda1
/dev/nvme0n1p2
```

而是某个名称或虚拟来源。

因此：

```bash
findmnt /
```

如果显示：

```text
SOURCE
some-name
```

而不是 `/dev/...`，并不意味着结果错误。

它可能表示该文件系统由外部环境共享、虚拟化或动态提供。

---

## 11. 三个命令的选择方法

遇到存储相关问题时，可以按问题选择工具。

### “这个路径到底在哪个文件系统上？”

```bash
findmnt /some/path
```

### “系统能看到哪些磁盘和分区？”

```bash
lsblk
```

### “这个路径所在的文件系统还有多少块和 inode？”

```bash
stat -f /some/path
```

核心区别：

```text
findmnt  → 挂载关系
lsblk    → 块设备
stat -f  → 文件系统属性
```

---

# 操作题解答：记录根文件系统挂载信息

## 题目

将根文件系统 `/` 的以下信息写入：

```text
/home/learner/root-mount
```

文件内容要求：

1. 第一行写入 `findmnt` 得到的：

   * source
   * target
   * filesystem type
2. 不包含表头；
3. 随后追加完整的：

```bash
stat -f /
```

输出。

## 正确命令

```bash
findmnt -n -o SOURCE,TARGET,FSTYPE / > /home/learner/root-mount
stat -f / >> /home/learner/root-mount
```

也可以写成一行：

```bash
findmnt -n -o SOURCE,TARGET,FSTYPE / > /home/learner/root-mount && stat -f / >> /home/learner/root-mount
```

## 命令解析

第一条：

```bash
findmnt -n -o SOURCE,TARGET,FSTYPE / > /home/learner/root-mount
```

其中：

* `findmnt /`：查询根路径所属的挂载；
* `-o SOURCE,TARGET,FSTYPE`：只输出来源、目标和文件系统类型；
* `-n`：不输出表头；
* `>`：创建或覆盖目标文件。

第二条：

```bash
stat -f / >> /home/learner/root-mount
```

其中：

* `stat -f /`：输出根路径所在文件系统的完整信息；
* `>>`：追加到现有文件末尾，而不是覆盖它。

## 为什么第二条必须使用 `>>`

如果错误地写成：

```bash
stat -f / > /home/learner/root-mount
```

第二条命令会重新覆盖文件，之前写入的 `findmnt` 结果将消失。

因此这里必须区分：

```text
>   覆盖文件
>>  追加到文件末尾
```

## 检查结果

执行完成后可以查看：

```bash
cat /home/learner/root-mount
```

文件结构应该类似：

```text
<source> / <filesystem-type>
  File: "/"
    ID: ...
    ...
```

第一行来自 `findmnt`，后面的完整内容来自：

```bash
stat -f /
```
