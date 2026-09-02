# Linux 目录所有权与权限管理：`chown`、`chgrp` 与 `chmod`

## 场景目标

目录 `/srv/uploads` 当前由 `root` 创建，因此默认状态类似：

```bash
ls -ld /srv/uploads
```

可能看到：

```text
drwxr-xr-x  root root  ... /srv/uploads
```

这表示：

* 所有者：`root`
* 所属组：`root`
* 权限：`755`
* 所有人都可以进入并查看目录内容
* 只有 `root` 可以在其中创建、删除或修改文件

现在希望把它作为分析团队的共享目录：

* 所有者改为用户 `analyst`
* 所属组改为 `reports`
* 权限改为 `770`

最终执行：

```bash
sudo chown analyst:reports /srv/uploads
sudo chmod 770 /srv/uploads
```

然后验证：

```bash
ls -ld /srv/uploads
```

期望类似：

```text
drwxrwx---  analyst reports  ... /srv/uploads
```

---

## 1. Linux 文件的 owner 和 group

Linux 中，每个文件或目录都关联两个重要身份：

1. **owner（所有者）**
2. **group（所属组）**

例如：

```text
drwxr-xr-x root root /srv/uploads
```

其中：

```text
root root
│    │
│    └── group
└─────── owner
```

权限则按照三类用户分别控制：

```text
rwx rwx rwx
│   │   │
│   │   └── other：其他用户
│   └────── group：所属组成员
└────────── owner：所有者
```

---

## 2. `chown`：修改所有者

`chown` 是 **change owner** 的缩写。

基本语法：

```bash
chown USER FILE
```

例如：

```bash
sudo chown analyst /srv/uploads
```

把 `/srv/uploads` 的 owner 改成：

```text
analyst
```

但不会主动把 group 改成 `reports`。

---

## 3. 同时修改 owner 和 group

`chown` 更常见的形式是：

```bash
chown USER:GROUP FILE
```

例如：

```bash
sudo chown analyst:reports /srv/uploads
```

一次完成：

```text
owner: analyst
group: reports
```

因此相比连续执行两个命令：

```bash
sudo chown analyst /srv/uploads
sudo chgrp reports /srv/uploads
```

通常可以直接写：

```bash
sudo chown analyst:reports /srv/uploads
```

---

## 4. `chgrp`：只修改 group

`chgrp` 是 **change group** 的缩写。

语法：

```bash
chgrp GROUP FILE
```

例如：

```bash
sudo chgrp reports /srv/uploads
```

只会把 group 改为：

```text
reports
```

owner 保持不变。

### `chown` 与 `chgrp` 对比

| 命令                      | 修改内容          |
| ----------------------- | ------------- |
| `chown user file`       | owner         |
| `chown user:group file` | owner + group |
| `chgrp group file`      | group         |

如果 owner 和 group 都需要改变，通常直接使用：

```bash
chown user:group file
```

---

## 5. 为什么修改 owner 通常需要 `sudo`

修改文件内容和修改文件**所有权**不是同一件事。

即使一个用户能够写某个文件，也不能随意执行：

```bash
chown someone file
```

把文件送给另一个用户。

普通用户随意改变 owner 会带来很多问题，例如：

* 绕过磁盘配额（quota）
* 混淆文件真正的责任人
* 影响审计记录
* 破坏系统权限模型

因此，重新指定文件 owner 通常需要 root 权限：

```bash
sudo chown ...
```

在这个场景中， `/srv/uploads` 本身也是：

```text
root root
```

所以修改其权限也需要：

```bash
sudo chmod ...
```

---

## 6. `chmod 770` 的含义

目标权限：

```bash
sudo chmod 770 /srv/uploads
```

数字权限的每一位分别对应：

```text
owner group other
  7     7     0
```

其中：

```text
r = 4
w = 2
x = 1
```

因此：

```text
7 = 4 + 2 + 1 = rwx
0 = ---
```

所以：

```text
770
```

等价于：

```text
rwx rwx ---
```

即：

| 用户类别            | 权限    |
| --------------- | ----- |
| owner `analyst` | `rwx` |
| group `reports` | `rwx` |
| other           | `---` |

---

## 7. 目录上的 `r`、`w`、`x`

目录权限和普通文件稍有不同。

### `r`：读取目录列表

允许查看目录中有哪些文件，例如：

```bash
ls /srv/uploads
```

### `w`：修改目录内容

允许：

* 创建文件
* 删除文件
* 重命名文件

### `x`：进入或穿过目录

允许：

```bash
cd /srv/uploads
```

并访问其中的具体路径。

因此：

```text
rwx
```

对于共享工作目录通常意味着：

> 可以进入目录、查看文件名，并创建、删除或重命名其中的文件。

---

## 8. 为什么使用 group 管理团队访问

假设只把目录交给一个用户：

```text
owner = analyst
group = root
```

那么只有 `analyst` 得到 owner 权限。

将来再来一个分析师，就可能需要重新修改权限甚至再次：

```bash
sudo chown ...
```

更好的做法是让权限跟随**角色或团队**：

```text
owner = analyst
group = reports
mode  = 770
```

然后团队成员加入：

```text
reports
```

组即可。

例如：

```bash
sudo usermod -aG reports alice
```

之后 `alice` 可以通过 group 的：

```text
rwx
```

权限访问 `/srv/uploads`，而无需重新修改目录本身。

这种设计体现了一个重要原则：

> 多人共享资源时，通常让访问权限绑定到 group，而不是不断绑定到具体个人。

---

## 9. `-R`：递归修改整个目录树

`chown` 和 `chgrp` 都支持：

```bash
-R
```

表示递归处理目录中的所有内容。

例如：

```bash
sudo chown -R analyst:reports /srv/uploads
```

不仅修改：

```text
/srv/uploads
```

本身，还会修改其中：

```text
/srv/uploads/file1
/srv/uploads/file2
/srv/uploads/subdir/
/srv/uploads/subdir/file3
...
```

### 不需要递归时不要随便使用 `-R`

如果任务只要求修改目录本身：

```bash
sudo chown analyst:reports /srv/uploads
```

就足够了。

不要习惯性写：

```bash
sudo chown -R ...
```

因为这可能无意中改变大量文件的 ownership。

---

## 10. 完整操作流程

### 查看当前状态

```bash
ls -ld /srv/uploads
```

例如：

```text
drwxr-xr-x root root ... /srv/uploads
```

### 修改 owner 和 group

```bash
sudo chown analyst:reports /srv/uploads
```

### 设置权限

```bash
sudo chmod 770 /srv/uploads
```

### 验证结果

```bash
ls -ld /srv/uploads
```

期望：

```text
drwxrwx--- analyst reports ... /srv/uploads
```

其中：

```text
d          directory
rwx        analyst 拥有完整权限
rwx        reports 组拥有完整权限
---        其他用户无权限
analyst    owner
reports    group
```

---

## 核心命令速查

```bash
# 修改 owner
sudo chown analyst /srv/uploads

# 同时修改 owner 和 group
sudo chown analyst:reports /srv/uploads

# 只修改 group
sudo chgrp reports /srv/uploads

# 设置 owner/group 完整权限，其他用户无权限
sudo chmod 770 /srv/uploads

# 递归修改整个目录树的 ownership
sudo chown -R analyst:reports /srv/uploads

# 查看目录自身的权限和 ownership
ls -ld /srv/uploads
```

## 核心要点

* `chown` 修改 owner，也可以用 `user:group` 同时修改 group。
* `chgrp` 只修改 group。
* 改变文件 owner 通常需要 root 权限，因此常配合 `sudo`。
* `770` 表示 `rwxrwx---`：owner 和 group 完整访问，其他用户完全无权访问。
* 团队共享目录应尽量通过 **group** 管理访问权限，而不是反复给不同个人重新 `chown`。
* `-R` 会影响整个目录树，只有确实需要递归修改时才使用。
