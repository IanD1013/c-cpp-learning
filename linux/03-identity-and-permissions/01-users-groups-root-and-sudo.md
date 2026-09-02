# Linux 用户、用户组、root 与 `sudo` 基础

## 1. Linux 为什么需要用户和用户组

Linux 中，每个进程都以某个**用户（user）**的身份运行。

内核在判断一个进程是否可以读取文件、修改文件或执行系统操作时，会参考该进程对应的：

* 用户 ID：UID（User ID）
* 主组 ID：GID（Group ID）
* 附加用户组（supplementary groups）
* 文件权限和其他安全机制

因此，Linux 权限管理的核心关系可以理解为：

```text
进程
 ↓
用户 UID + 用户组 GID
 ↓
文件所有者 / 所属组 / 权限
 ↓
决定是否允许访问
```

用户组的作用是让多个用户共享一组权限。例如，可以创建一个 `reports` 组，让所有需要访问报表文件的用户都加入这个组。

---

## 2. 查看系统中的用户和用户组

### `/etc/passwd`：用户数据库

系统用户记录通常可以通过：

```bash
cat /etc/passwd
```

查看。

每个用户占一行，例如：

```text
analyst:x:2001:2001::/home/analyst:/bin/sh
```

常见字段依次为：

```text
用户名:密码占位符:UID:GID:备注:家目录:登录 Shell
```

例如：

```text
analyst:x:2001:2001::/home/analyst:/bin/sh
```

表示：

* 用户名：`analyst`
* UID：`2001`
* 主组 GID：`2001`
* 家目录：`/home/analyst`
* Shell：`/bin/sh`

> `/etc/passwd` 中的 `x` 不代表密码本身。现代 Linux 通常把密码哈希保存在权限更严格的 `/etc/shadow` 中。

---

### `/etc/group`：用户组数据库

查看系统用户组：

```bash
cat /etc/group
```

典型记录：

```text
reports:x:2001:
```

格式大致为：

```text
组名:密码占位符:GID:附加成员列表
```

例如：

```text
developers:x:3000:alice,bob
```

表示：

* 用户组：`developers`
* GID：`3000`
* `alice` 和 `bob` 是该组的附加成员

### 一个容易混淆的地方

用户的**主组**记录在 `/etc/passwd` 的 GID 字段中，因此不一定会同时出现在 `/etc/group` 最后的成员列表里。

所以判断用户属于哪些组时，更适合使用：

```bash
id analyst
```

而不是只观察 `/etc/group`。

---

## 3. 使用 `id` 查看用户身份

查看当前用户：

```bash
id
```

可能得到：

```text
uid=1000(learner) gid=1000(learner) groups=1000(learner),27(sudo)
```

其中：

* `uid`：用户 ID
* `gid`：主组 ID
* `groups`：该用户所属的全部用户组

查看指定用户：

```bash
id analyst
```

例如：

```text
uid=2001(analyst) gid=2001(reports) groups=2001(reports)
```

这说明 `analyst` 的 UID 是 `2001`，并且属于 `reports` 组。

---

# root 用户与 `sudo`

## 1. root 是什么

Linux 中：

```text
root
UID = 0
```

`root` 是系统超级用户，拥有极高权限，可以执行普通用户无法执行的系统管理操作，例如：

* 创建或删除用户
* 创建用户组
* 修改系统文件
* 修改其他用户的文件
* 修改文件所有权和权限
* 在 `/root` 等受保护目录中创建文件

日常操作不应该长期使用 root 身份，因为错误命令可能破坏整个系统。

---

## 2. `sudo` 的作用

`sudo` 可以让普通用户临时以更高权限执行**某一条命令**。

例如：

```bash
sudo groupadd reports
```

这里只是 `groupadd reports` 以 root 权限运行。

命令执行完成后，当前 shell 仍然是普通用户：

```text
普通用户
   ↓
sudo 某条命令
   ↓
该命令以 root 权限执行
   ↓
返回普通用户环境
```

因此应遵循一个重要原则：

> **只在操作确实需要管理员权限时使用 `sudo`。**

---

# 什么时候需要 `sudo`

判断标准不是“这个命令看起来是不是系统命令”，而是：

> **这次操作是否需要修改当前普通用户无权修改的系统状态。**

## 不需要 `sudo` 的读取操作

普通用户通常可以直接执行：

```bash
cat /etc/passwd
cat /etc/group
id
id analyst
```

这些操作只是读取公开的系统身份信息。

---

## 通常需要 `sudo` 的修改操作

例如：

```bash
sudo groupadd reports
sudo useradd ...
```

这些命令会修改系统用户和用户组数据库。

向 `/root` 写入文件也需要管理员权限：

```bash
sudo install ...
```

---

# 创建用户组

创建名为 `reports` 的用户组：

```bash
sudo groupadd reports
```

如果需要指定 GID：

```bash
sudo groupadd -g 2001 reports
```

其中：

```text
-g 2001
```

表示把该用户组的 GID 设置为 `2001`。

执行后，可以检查：

```bash
grep '^reports:' /etc/group
```

可能看到：

```text
reports:x:2001:
```

---

# 创建用户

假设需要创建：

```text
用户名：analyst
UID：2001
主组：reports
家目录：/home/analyst
```

可以执行：

```bash
sudo useradd -m -u 2001 -g reports analyst
```

各参数含义：

| 参数           | 含义      |
| ------------ | ------- |
| `-m`         | 创建用户家目录 |
| `-u 2001`    | 指定 UID  |
| `-g reports` | 指定主组    |
| `analyst`    | 创建的用户名  |

执行后通常会创建：

```text
/home/analyst
```

并让 `analyst` 的主组成为：

```text
reports
```

---

## `-g` 和 `-G` 不要混淆

这是用户管理中非常常见的区别。

### `-g`：主组

```bash
useradd -g reports analyst
```

表示：

```text
analyst 的 primary group = reports
```

### `-G`：附加组

例如：

```bash
useradd -g analysts -G reports analyst
```

表示：

```text
主组：analysts
附加组：reports
```

如果要求 `reports` 直接作为用户的主组，应使用：

```bash
-g reports
```

---

# 创建仅 root 可访问的文件

假设需要：

```text
/root/provisioned
```

并且要求：

* 文件存在
* 所有者是 root
* 权限为 `600`

可以使用：

```bash
sudo install -m 600 /dev/null /root/provisioned
```

---

## 1. `/dev/null` 是什么

`/dev/null` 是一个特殊设备。

读取它时不会得到任何数据，因此：

```bash
install /dev/null destination
```

可以用来创建一个空文件。

---

## 2. `install -m 600`

这里的：

```bash
-m 600
```

表示新文件直接使用权限：

```text
600
```

所以一条命令就完成了：

1. 创建文件；
2. 设置权限；
3. 因为通过 `sudo` 执行，所以文件由 root 创建并拥有。

---

# Linux 文件权限 `600`

Linux 常见权限由三组组成：

```text
owner | group | others
```

每一组使用三个权限：

```text
r = read
w = write
x = execute
```

数字对应：

| 权限  | 数值 |
| --- | -: |
| `r` |  4 |
| `w` |  2 |
| `x` |  1 |

因此：

```text
6 = 4 + 2 = rw-
0 = ---
0 = ---
```

所以：

```text
600
```

等价于：

```text
rw-------
```

含义是：

| 对象    | 权限  |
| ----- | --- |
| 文件所有者 | 读、写 |
| 所属组   | 无权限 |
| 其他用户  | 无权限 |

如果文件属于 root：

```text
-rw------- root root /root/provisioned
```

那么实际上只有 root 可以读取和修改它。

---

# `install` 与 `touch + chmod` 的区别

除了：

```bash
sudo install -m 600 /dev/null /root/provisioned
```

也可以分开完成：

```bash
sudo touch /root/provisioned
sudo chmod 600 /root/provisioned
```

两种方法最终可以达到相同结果。

但：

```bash
sudo install -m 600 /dev/null /root/provisioned
```

可以一次完成创建和权限设置，因此更紧凑。

---

# 完整示例：创建分析员账户

目标：

```text
创建 reports 组
GID = 2001

创建 analyst 用户
UID = 2001
主组 = reports
创建家目录

创建 /root/provisioned
所有者 = root
权限 = 600
```

可以执行：

```bash
sudo groupadd -g 2001 reports
sudo useradd -m -u 2001 -g reports analyst
sudo install -m 600 /dev/null /root/provisioned
```

---

# 验证配置

## 检查用户身份

```bash
id analyst
```

预期结果类似：

```text
uid=2001(analyst) gid=2001(reports) groups=2001(reports)
```

---

## 检查 `/etc/passwd`

```bash
grep '^analyst:' /etc/passwd
```

可能看到：

```text
analyst:x:2001:2001::/home/analyst:/bin/sh
```

---

## 检查 `/etc/group`

```bash
grep '^reports:' /etc/group
```

可能看到：

```text
reports:x:2001:
```

即使最后没有显示 `analyst`，也不一定意味着配置错误，因为 `reports` 可能是 `analyst` 的**主组**。

使用：

```bash
id analyst
```

判断用户组关系通常更直接。

---

## 检查文件权限和所有者

```bash
sudo ls -l /root/provisioned
```

预期类似：

```text
-rw------- 1 root root 0 Sep 2 11:00 /root/provisioned
```

关键部分是：

```text
-rw-------
root root
```

即：

```text
权限 = 600
所有者 = root
```

---

# 常见误区

## 误区 1：所有系统命令都需要 `sudo`

错误。

例如：

```bash
cat /etc/passwd
cat /etc/group
id
```

只是读取普通用户有权读取的信息，不需要 `sudo`。

真正的判断标准是：

> 当前操作是否需要普通用户没有的权限。

---

## 误区 2：一直使用 root 更方便

虽然 root 可以减少权限错误，但风险很大。

普通用户下的错误命令可能只是：

```text
Permission denied
```

root 下执行相同的错误命令却可能真的删除或覆盖重要系统文件。

更安全的习惯是：

```text
日常使用普通用户
        ↓
确实需要管理员权限
        ↓
sudo 单条命令
```

---

## 误区 3：`/etc/group` 没列出用户名就说明用户不属于该组

不一定。

Linux 区分：

```text
主组
附加组
```

主组通过 `/etc/passwd` 中的 GID 表示，因此用户不一定出现在 `/etc/group` 的成员列表里。

优先使用：

```bash
id 用户名
```

查看完整的用户组关系。

---

## 误区 4：UID 和 GID 必须等于 2001

不一定。

UID 和 GID 本质上只需要在系统中满足相应唯一性要求。

例如：

```text
UID = 2001
GID = 2001
```

可能只是实验或课程约定。

真实系统中不应盲目硬编码数字，应先确认是否已经被占用：

```bash
getent passwd 2001
getent group 2001
```

---

# 核心知识总结

Linux 权限管理的核心是：

```text
进程以用户身份运行
        ↓
用户具有 UID
        ↓
用户属于一个主组和若干附加组
        ↓
内核结合身份与文件权限决定是否允许操作
```

常见身份管理命令：

```bash
cat /etc/passwd
cat /etc/group
id
id USER
```

创建用户组：

```bash
sudo groupadd -g GID GROUP
```

创建用户并设置主组：

```bash
sudo useradd -m -u UID -g GROUP USER
```

创建只有 root 可以读写的空文件：

```bash
sudo install -m 600 /dev/null FILE
```

使用 `sudo` 时最重要的原则是：

> **读取普通用户有权访问的信息时不需要 `sudo`；修改受保护的系统状态时才临时使用 `sudo`。**
