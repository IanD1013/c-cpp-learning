# Linux 分区格式化、文件系统与挂载

## 1. 分区不等于文件系统

创建 `/dev/sda1` 后，仍然不能直接在其中创建文件。

原因是：

* **分区（partition）**只是磁盘上一段被划分出来的扇区范围；
* 它只定义“这部分磁盘空间属于谁”；
* 它本身并不知道什么是：

  * 文件；
  * 目录；
  * 文件名；
  * 权限；
  * 空闲空间。

例如：

```text
/dev/sda
└── /dev/sda1
```

这里只表示 `/dev/sda1` 是磁盘 `/dev/sda` 上的第一个分区。

如果希望在 `/dev/sda1` 中保存文件，还必须先在这个分区上创建**文件系统（filesystem）**。

---

## 2. `mkfs`：在分区上创建文件系统

`mkfs` 是 **make filesystem** 的缩写，用于在块设备上创建文件系统。

例如创建 ext4：

```bash
sudo mkfs.ext4 /dev/sda1
```

执行后，`/dev/sda1` 就拥有了 ext4 文件系统。

### ext4 会创建什么

格式化时会在分区中建立文件系统所需要的数据结构，例如：

* **superblock（超级块）**

  * 记录整个文件系统的信息；
  * 例如大小、状态、块数量等。

* **inode**

  * 保存文件的元数据；
  * 例如权限、所有者、大小以及数据块的位置。

* **journal（日记）**

  * 记录文件系统修改；
  * 如果写入过程中突然断电，可以帮助恢复文件系统的一致性。

可以简单理解为：

```text
分区
↓
mkfs.ext4
↓
ext4 文件系统
↓
可以保存目录和文件
```

---

## 3. 格式化是破坏性操作

`mkfs` 会重新创建文件系统结构，因此通常意味着：

> 原设备中的已有数据会被破坏。

例如：

```bash
sudo mkfs.ext4 /dev/sda1
```

绝对不能在没有确认设备身份的情况下随便执行。

尤其需要注意：

```text
/dev/sda1
```

在真实 Linux 机器中经常可能是系统磁盘的一部分。

在执行前应该确认设备：

```bash
lsblk
```

例如：

```text
NAME   SIZE TYPE MOUNTPOINTS
sda     20G disk
└─sda1  20G part
```

还应确认目标分区没有挂载重要数据。

---

## 4. `mkfs.ext4 -F` 中的 `-F`

有时会看到：

```bash
sudo mkfs.ext4 -F /dev/sda1
```

`-F` 表示强制执行。

它可能跳过某些原本用于防止误操作的检查或确认。

因此：

```bash
mkfs.ext4 -F /dev/sda1
```

比普通的：

```bash
mkfs.ext4 /dev/sda1
```

风险更高。

### 使用原则

在实验环境或自动化脚本中可能需要 `-F`，但真实机器上不要机械复制。

核心原则是：

> 先确认设备，再执行格式化命令。

---

## 5. 有文件系统之后仍然不能直接通过普通路径访问

假设已经执行：

```bash
sudo mkfs.ext4 /dev/sda1
```

虽然 `/dev/sda1` 已经拥有 ext4 文件系统，但它还没有接入 Linux 的目录树。

Linux 程序通常不会这样访问文件：

```text
/dev/sda1/some-file
```

因为 `/dev/sda1` 是一个**块设备文件**，而不是普通目录。

需要先进行：

```text
mount
```

也就是**挂载**。

---

## 6. 什么是挂载（mount）

Linux 使用一棵统一的目录树：

```text
/
├── etc
├── home
├── var
└── mnt
```

不同磁盘和文件系统通过**挂载（mount）**被接入这棵目录树。

例如：

```bash
sudo mount /dev/sda1 /mnt/course-data
```

表示：

> 把 `/dev/sda1` 中的文件系统连接到 `/mnt/course-data`。

之后：

```text
/mnt/course-data
```

中的内容实际上来自：

```text
/dev/sda1
```

例如：

```bash
sudo touch /mnt/course-data/test.txt
```

这个文件的数据最终会存放在 `/dev/sda1` 的文件系统中。

---

## 7. 什么是挂载点（mount point）

被用来接入文件系统的目录叫：

> **mount point（挂载点）**

例如：

```text
/mnt/course-data
```

就是挂载点。

挂载前，这个目录必须存在：

```bash
sudo mkdir -p /mnt/course-data
```

然后才能：

```bash
sudo mount /dev/sda1 /mnt/course-data
```

`mount` 本身不会替你创建这个目录。

---

## 8. 挂载会“遮住”目录原来的内容

假设：

```text
/mnt/course-data/
└── old.txt
```

然后执行：

```bash
sudo mount /dev/sda1 /mnt/course-data
```

此时 `/mnt/course-data` 显示的是 `/dev/sda1` 文件系统中的内容。

原来的：

```text
old.txt
```

不会被删除，但暂时看不到了。

结构可以理解为：

```text
原目录内容
/mnt/course-data
└── old.txt

          ↓ mount /dev/sda1

现在看到
/mnt/course-data
└── /dev/sda1 中的内容
```

如果卸载：

```bash
sudo umount /mnt/course-data
```

原来的：

```text
old.txt
```

就会重新出现。

因此：

> 不要随意把文件系统挂载到已经包含重要文件的目录上。

---

## 9. `mount` 为什么通常不需要指定文件系统类型

可以显式指定：

```bash
sudo mount -t ext4 /dev/sda1 /mnt/course-data
```

其中：

```text
-t ext4
```

表示文件系统类型是 ext4。

但很多情况下直接这样即可：

```bash
sudo mount /dev/sda1 /mnt/course-data
```

因为 `mount` 可以根据设备上的文件系统签名识别类型。

也就是说：

```text
mkfs.ext4
   ↓
写入 ext4 文件系统标识
   ↓
mount 检测文件系统
   ↓
识别为 ext4
```

因此这里的：

```bash
-t ext4
```

通常是可选的。

---

## 10. 没有文件系统时为什么无法挂载

如果 `/dev/sda1` 只是一个刚创建的裸分区：

```text
/dev/sda1
```

但还没有执行：

```bash
mkfs.ext4
```

那么：

```bash
sudo mount /dev/sda1 /mnt/course-data
```

通常会失败。

因为内核虽然知道 `/dev/sda1` 是一个块设备，但不知道其中的数据应该按照什么文件系统结构解释。

因此正确顺序是：

```text
创建分区
    ↓
创建文件系统
    ↓
创建挂载点
    ↓
挂载
    ↓
通过目录访问文件
```

例如：

```bash
sudo mkfs.ext4 /dev/sda1
sudo mkdir -p /mnt/course-data
sudo mount /dev/sda1 /mnt/course-data
```

---

## 11. `umount`：卸载文件系统

卸载使用：

```bash
sudo umount /mnt/course-data
```

注意命令叫：

```text
umount
```

而不是：

```text
unmount
```

也可以使用设备：

```bash
sudo umount /dev/sda1
```

卸载的含义是：

> 解除文件系统与目录树之间的连接。

它不会删除文件系统，也不会删除其中的数据。

---

## 12. 为什么会出现 `target is busy`

如果文件系统仍然被某个程序使用，`umount` 会拒绝卸载。

例如：

```bash
cd /mnt/course-data
sudo umount /mnt/course-data
```

可能得到类似：

```text
target is busy
```

原因是当前 shell 自己的工作目录就在这个文件系统内部。

解决方法：

```bash
cd /
sudo umount /mnt/course-data
```

其他常见原因还包括：

* 某个程序正在读取其中的文件；
* 某个程序打开了其中的日志；
* 另一个 shell 当前位于该目录中；
* 某个进程仍持有其中的文件。

可以使用：

```bash
lsof +D /mnt/course-data
```

或：

```bash
fuser -m /mnt/course-data
```

辅助寻找正在使用该文件系统的进程。

---

## 13. 如何确认文件系统已经挂载

### 方法一：`findmnt`

```bash
findmnt /mnt/course-data
```

可能看到：

```text
TARGET           SOURCE    FSTYPE OPTIONS
/mnt/course-data /dev/sda1 ext4   rw,relatime
```

关键关系是：

```text
SOURCE       → /dev/sda1
TARGET       → /mnt/course-data
FSTYPE       → ext4
```

说明：

> `/mnt/course-data` 当前由 `/dev/sda1` 上的 ext4 文件系统提供内容。

### 方法二：`lsblk`

```bash
lsblk -f
```

可能看到：

```text
NAME   FSTYPE MOUNTPOINTS
sda
└─sda1 ext4   /mnt/course-data
```

这可以同时看到：

* 分区；
* 文件系统类型；
* 挂载点。

---

## 14. 完整关系

整个存储流程可以理解为：

```text
物理磁盘
/dev/sda
    │
    │ 创建分区
    ▼
分区
/dev/sda1
    │
    │ mkfs.ext4
    ▼
ext4 文件系统
    │
    │ mount
    ▼
挂载点
/mnt/course-data
    │
    ▼
普通文件路径
/mnt/course-data/file.txt
```

三个概念必须分清：

| 概念   | 示例                 | 作用                |
| ---- | ------------------ | ----------------- |
| 磁盘   | `/dev/sda`         | 提供原始存储空间          |
| 分区   | `/dev/sda1`        | 划分磁盘空间            |
| 文件系统 | ext4               | 组织文件和目录           |
| 挂载点  | `/mnt/course-data` | 把文件系统接入 Linux 目录树 |

---

## 15. 核心命令

```bash
# 查看磁盘和分区
lsblk

# 在分区上创建 ext4 文件系统
sudo mkfs.ext4 /dev/sda1

# 创建挂载点
sudo mkdir -p /mnt/course-data

# 挂载
sudo mount /dev/sda1 /mnt/course-data

# 查看挂载关系
findmnt /mnt/course-data

# 查看设备、文件系统和挂载点
lsblk -f

# 卸载
sudo umount /mnt/course-data
```

最重要的操作顺序：

```text
partition
    ↓
filesystem
    ↓
mount point
    ↓
mount
    ↓
files
```

---

# 实操：将 `/dev/sda1` 格式化为 ext4 并挂载到 `/mnt/course-data`

## 目标

完成以下操作：

1. 将 `/dev/sda1` 格式化为 ext4；
2. 创建 `/mnt/course-data`；
3. 将 `/dev/sda1` 挂载到该目录；
4. 验证挂载结果。

> **警告：格式化会破坏 `/dev/sda1` 上已有的数据。执行前必须确认 `/dev/sda1` 确实是允许清空的目标分区。**

## 1. 确认设备

```bash
lsblk
```

确认目标确实是：

```text
/dev/sda1
```

并确保它不是正在使用的重要系统分区。

## 2. 创建 ext4 文件系统

```bash
sudo mkfs.ext4 -F /dev/sda1
```

其中：

* `mkfs.ext4`：创建 ext4 文件系统；
* `-F`：强制执行；
* `/dev/sda1`：要格式化的分区。

执行后：

```text
/dev/sda1
```

就包含一个新的 ext4 文件系统。

## 3. 创建挂载点

```bash
sudo mkdir -p /mnt/course-data
```

`-p` 可以确保路径不存在时创建目录，同时在目录已经存在时不会因此报错。

## 4. 挂载文件系统

```bash
sudo mount /dev/sda1 /mnt/course-data
```

含义是：

```text
/dev/sda1
    ↓ mount
/mnt/course-data
```

此后访问：

```text
/mnt/course-data
```

就是在访问 `/dev/sda1` 上的文件系统。

## 5. 使用 `findmnt` 验证

```bash
findmnt /mnt/course-data
```

预期结果类似：

```text
TARGET           SOURCE    FSTYPE OPTIONS
/mnt/course-data /dev/sda1 ext4   rw,relatime
```

只要能够看到：

```text
SOURCE = /dev/sda1
TARGET = /mnt/course-data
FSTYPE = ext4
```

就说明挂载成功。

## 6. 也可以使用 `lsblk` 验证

```bash
lsblk -f
```

预期结构类似：

```text
NAME   FSTYPE MOUNTPOINTS
sda
└─sda1 ext4   /mnt/course-data
```

## 最终命令

```bash
sudo mkfs.ext4 -F /dev/sda1
sudo mkdir -p /mnt/course-data
sudo mount /dev/sda1 /mnt/course-data
findmnt /mnt/course-data
```

或使用：

```bash
lsblk -f
```

再次确认：

```text
/dev/sda1 → ext4 → /mnt/course-data
```

这表示 `/dev/sda1` 已经拥有 ext4 文件系统，并成功接入 Linux 的目录树。
