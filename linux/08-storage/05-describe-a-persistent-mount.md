# Linux 持久化挂载：使用 `/etc/fstab` 和 UUID

## 1. 临时挂载与持久化挂载

使用 `mount` 命令手动挂载文件系统，例如：

```bash
mount /dev/sda1 /mnt/course-data
```

这种挂载通常只在当前系统运行期间有效。

机器重启后，挂载关系会消失。

如果希望系统启动时自动挂载文件系统，需要把挂载配置写入：

```text
/etc/fstab
```

`/etc/fstab` 可以理解为 Linux 的**静态文件系统挂载配置表**。

系统启动时会读取这个文件，根据其中的配置挂载文件系统。

---

## 2. `/etc/fstab` 的基本格式

`/etc/fstab` 中通常每个文件系统占一行，共有六个字段：

```text
<设备> <挂载点> <文件系统类型> <挂载选项> <dump> <fsck顺序>
```

例如：

```text
UUID=12345678-abcd-1234-abcd-123456789abc /mnt/course-data ext4 defaults 0 2
```

六个字段分别表示：

| 字段      | 示例                 | 含义                |
| ------- | ------------------ | ----------------- |
| 设备标识    | `UUID=...`         | 要挂载哪个文件系统         |
| 挂载点     | `/mnt/course-data` | 挂载到目录树中的哪里        |
| 文件系统类型  | `ext4`             | 文件系统格式            |
| 挂载选项    | `defaults`         | 使用哪些挂载参数          |
| dump    | `0`                | 是否供 `dump` 备份工具使用 |
| fsck 顺序 | `2`                | 启动时文件系统检查顺序       |

---

## 3. `defaults 0 2` 是什么意思

常见配置：

```text
defaults 0 2
```

### `defaults`

表示采用一组常用的默认挂载选项。

对于普通本地 ext4 数据分区，通常直接使用：

```text
defaults
```

即可。

### 第一个数字：`0`

```text
0
```

与传统的 `dump` 备份工具有关。

`0` 表示：

> 不使用 `dump` 对这个文件系统进行自动备份。

现代 Linux 系统中通常都写 `0`。

### 第二个数字：`2`

```text
2
```

表示系统启动时 `fsck` 检查文件系统的顺序。

常见约定：

```text
0  不检查
1  优先检查，通常用于根文件系统 /
2  根文件系统之后检查
```

因此普通 ext4 数据分区常见配置为：

```text
0 2
```

可以理解为：

> 不参与 `dump`，启动时在根文件系统之后进行文件系统检查。

---

# 4. 为什么不应该在 `/etc/fstab` 中依赖 `/dev/sda1`

可以写：

```text
/dev/sda1 /mnt/course-data ext4 defaults 0 2
```

但通常不推荐这样做。

原因是：

```text
/dev/sda1
/dev/sdb1
/dev/sdc1
```

这些名称描述的是 Linux 当前识别出来的**设备位置和枚举顺序**。

它们不一定永远不变。

例如原来：

```text
/dev/sda1
```

是数据盘。

后来新增了一块磁盘，系统下次启动时可能变成：

```text
/dev/sdb1
```

如果 `/etc/fstab` 仍然写着：

```text
/dev/sda1
```

系统可能：

* 找不到目标文件系统；
* 挂载失败；
* 极端情况下挂载到错误的磁盘。

因此，长期配置通常应该使用更稳定的文件系统标识。

---

# 5. UUID：给文件系统一个稳定身份

UUID 是：

> Universally Unique Identifier，通用唯一标识符。

创建文件系统时，例如：

```bash
mkfs.ext4 /dev/sda1
```

文件系统内部通常会获得一个 UUID。

可以使用：

```bash
blkid /dev/sda1
```

查看。

例如：

```text
/dev/sda1: UUID="5a4f0181-b17a-4ec4-b879-78c437ab8792" TYPE="ext4"
```

这里：

```text
5a4f0181-b17a-4ec4-b879-78c437ab8792
```

就是文件系统 UUID。

在 `/etc/fstab` 中可以写：

```text
UUID=5a4f0181-b17a-4ec4-b879-78c437ab8792 /mnt/course-data ext4 defaults 0 2
```

---

## 6. UUID 为什么比 `/dev/sda1` 更可靠

关键区别是：

```text
/dev/sda1
```

描述：

> 这个设备当前位于什么位置。

而：

```text
UUID=...
```

描述：

> 我要的是这个具体文件系统。

即使设备名称从：

```text
/dev/sda1
```

变成：

```text
/dev/sdb1
```

只要文件系统本身没有被重新格式化，它的 UUID 通常仍然不变。

所以：

```text
UUID=...
```

会继续找到正确的文件系统。

可以简单记成：

```text
/dev/sda1 = 地址
UUID       = 身份证
```

对于 `/etc/fstab` 这种长期配置，通常应该使用“身份证”，而不是临时“地址”。

---

# 7. 使用 `blkid` 获取 UUID

查看设备信息：

```bash
blkid /dev/sda1
```

如果只想获取 UUID：

```bash
blkid -s UUID -o value /dev/sda1
```

例如输出：

```text
5a4f0181-b17a-4ec4-b879-78c437ab8792
```

参数含义：

```text
-s UUID
```

表示只选择 `UUID` 字段。

```text
-o value
```

表示只输出值本身。

这样得到的结果特别适合脚本使用。

---

# 8. Shell 命令替换：`$(...)`

Shell 可以使用：

```bash
$(command)
```

执行命令，然后把命令的输出插入当前位置。

例如：

```bash
echo "$(blkid -s UUID -o value /dev/sda1)"
```

Shell 会先执行：

```bash
blkid -s UUID -o value /dev/sda1
```

假设结果是：

```text
5a4f0181-b17a-4ec4-b879-78c437ab8792
```

那么实际效果相当于：

```bash
echo "5a4f0181-b17a-4ec4-b879-78c437ab8792"
```

因此可以自动构造 `/etc/fstab` 条目，例如：

```bash
echo "UUID=$(blkid -s UUID -o value /dev/sda1) /mnt/course-data ext4 defaults 0 2"
```

输出类似：

```text
UUID=5a4f0181-b17a-4ec4-b879-78c437ab8792 /mnt/course-data ext4 defaults 0 2
```

这样可以避免手动复制很长的 UUID 时输错字符。

---

# 9. 配置 `/etc/fstab`

假设：

* 文件系统设备：`/dev/sda1`
* 文件系统：`ext4`
* 挂载点：`/mnt/course-data`

首先确认挂载目录存在：

```bash
mkdir -p /mnt/course-data
```

获取 UUID：

```bash
blkid -s UUID -o value /dev/sda1
```

然后编辑：

```bash
/etc/fstab
```

加入：

```text
UUID=<实际UUID> /mnt/course-data ext4 defaults 0 2
```

例如：

```text
UUID=5a4f0181-b17a-4ec4-b879-78c437ab8792 /mnt/course-data ext4 defaults 0 2
```

---

# 10. 修改 `/etc/fstab` 后为什么要运行 `mount -a`

修改 `/etc/fstab` 后，不应该直接等到下次重启才发现配置错误。

可以立即执行：

```bash
mount -a
```

它会重新读取：

```text
/etc/fstab
```

并尝试挂载其中尚未挂载、且符合条件的文件系统。

因此它是修改 `/etc/fstab` 后非常重要的检查命令。

---

## 11. `mount -a` 能发现哪些问题

例如：

### UUID 写错

```text
UUID=错误值 /mnt/course-data ext4 defaults 0 2
```

可能出现类似：

```text
can't find UUID=...
```

### 文件系统类型写错

例如真实格式是 ext4，却写成：

```text
xfs
```

则挂载可能失败。

### 挂载目录有问题

如果挂载点配置不正确，也可能导致失败。

因此推荐流程是：

```text
修改 /etc/fstab
        ↓
运行 mount -a
        ↓
确认没有报错
        ↓
确认文件系统确实已挂载
```

---

# 12. 如何确认挂载成功

可以使用：

```bash
findmnt /mnt/course-data
```

例如：

```text
TARGET           SOURCE     FSTYPE OPTIONS
/mnt/course-data /dev/sda1  ext4   rw,...
```

也可以使用：

```bash
df -h /mnt/course-data
```

查看该路径所属文件系统的容量信息。

---

# 13. `mount -a` 不等于完整的启动测试

这是一个容易误解的地方。

执行：

```bash
mount -a
```

成功，只能说明：

> 当前运行环境下，`/etc/fstab` 中的挂载配置能够被解析并成功挂载。

它可以验证很多重要问题，例如：

* UUID 是否存在；
* 文件系统类型是否正确；
* 挂载点是否有效；
* 挂载参数是否基本正确。

但是：

```bash
mount -a
```

不能完全证明：

> 下次系统启动一定不会因为这个配置出现问题。

因为真正的启动过程还涉及：

* systemd 启动顺序；
* 设备出现的时间；
* 网络文件系统依赖；
* 服务之间的依赖关系；
* boot-time filesystem check；
* 其他启动阶段行为。

因此：

```text
mount -a 成功
```

和：

```text
经过实际重启验证
```

不是完全相同的测试。

---

# 14. 推荐操作流程

对于一个新的 ext4 数据分区，可以按照下面的思路完成持久化挂载。

### 1. 确认文件系统

```bash
blkid /dev/sda1
```

确认类似：

```text
TYPE="ext4"
```

### 2. 创建挂载点

```bash
mkdir -p /mnt/course-data
```

### 3. 获取 UUID

```bash
blkid -s UUID -o value /dev/sda1
```

### 4. 写入 `/etc/fstab`

```text
UUID=<UUID> /mnt/course-data ext4 defaults 0 2
```

### 5. 测试配置

```bash
mount -a
```

### 6. 验证结果

```bash
findmnt /mnt/course-data
```

---

# 15. 常见误区

## 误区一：`mount` 之后重启也会保留

不会。

```bash
mount /dev/sda1 /mnt/course-data
```

只是建立当前运行期间的挂载关系。

想要重启后自动恢复，需要配置：

```text
/etc/fstab
```

---

## 误区二：`/dev/sda1` 永远代表同一块分区

不能保证。

设备名称可能因为硬件、驱动或枚举顺序发生变化。

长期配置优先使用：

```text
UUID=...
```

---

## 误区三：UUID 是磁盘的编号

更准确地说，这里使用的 UUID 通常属于：

> 文件系统。

例如执行：

```bash
mkfs.ext4 /dev/sda1
```

重新创建文件系统后，通常会生成新的 UUID。

因此 UUID 与文件系统本身联系更紧密，而不是单纯与磁盘插槽绑定。

---

## 误区四：`mount -a` 成功就证明系统一定能重启

不一定。

它是非常重要的配置验证，但不能完全模拟系统启动过程。

真正验证 boot-time 行为需要实际重启。

---

# 16. 核心知识总结

持久化挂载的完整逻辑是：

```text
文件系统
   │
   │ UUID 标识它是谁
   ▼
/etc/fstab
   │
   │ 描述挂到哪里、使用什么类型和选项
   ▼
系统启动自动挂载
```

典型 `/etc/fstab` 配置：

```text
UUID=<UUID> /mnt/course-data ext4 defaults 0 2
```

修改后立即测试：

```bash
mount -a
```

然后验证：

```bash
findmnt /mnt/course-data
```

最重要的原则是：

> `/dev/sda1` 更像设备当前的“地址”，UUID 更像文件系统稳定的“身份”。对于需要跨重启长期有效的 `/etc/fstab` 配置，通常应优先使用 UUID。
