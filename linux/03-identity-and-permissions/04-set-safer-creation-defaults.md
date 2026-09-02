# Linux `umask`：让新文件在创建时自动获得正确权限

## 目标

让用户 `analyst` 创建 `report.txt` 时，文件**从创建的那一刻起**就具有以下权限：

```text
-rw-r-----
```

对应数字权限：

```text
640
```

权限含义：

| 对象              | 权限    | 含义    |
| --------------- | ----- | ----- |
| owner：`analyst` | `rw-` | 可读、可写 |
| group：`reports` | `r--` | 只读    |
| others          | `---` | 无任何权限 |

要实现这一点，可以在创建文件之前设置：

```bash
umask 027
```

---

## 使用 `umask 027` 创建文件

以 `analyst` 用户启动一个 Bash，并在同一个 shell 中设置 `umask` 后创建文件：

```bash
sudo -u analyst bash -c 'umask 027 && touch ~/report.txt'
```

这条命令可以拆成三部分理解：

```bash
sudo -u analyst
```

表示：

> 以 `analyst` 用户身份执行后面的命令。

```bash
bash -c '...'
```

表示：

> 启动一个 Bash，让 Bash 执行引号中的命令。

内部执行：

```bash
umask 027 && touch ~/report.txt
```

其中 `&&` 表示只有前面的命令成功后，才执行后面的命令。

因此实际过程是：

```text
切换为 analyst
        ↓
设置 umask 027
        ↓
创建 ~/report.txt
```

由于 `~` 是在 `analyst` 的 shell 中展开的，因此这里通常对应：

```text
/home/analyst/report.txt
```

---

## `umask` 的核心原理

`umask`（user file-creation mode mask）控制的是：

> **创建新文件或目录时，需要从程序请求的权限中清除哪些权限位。**

它不是直接指定最终权限。

### 普通文件的默认请求权限

普通文件通常以：

```text
666
```

作为最大基础权限：

```text
rw-rw-rw-
```

也就是：

* owner：读 + 写
* group：读 + 写
* others：读 + 写

普通文件默认不带执行权限，因此不是 `777`。

### 目录的默认请求权限

目录通常请求：

```text
777
```

即：

```text
rwxrwxrwx
```

目录需要 `x` 权限才能进入和访问其中的文件，所以目录和普通文件的基础权限不同。

---

## `umask 027` 表示什么

将 `027` 分成三组：

```text
0 2 7
│ │ │
│ │ └─ others
│ └─── group
└───── owner
```

每一位表示需要**清除**的权限。

### Owner：`0`

```text
0 = ---
```

什么都不清除。

所以 owner 原来的：

```text
rw-
```

仍然是：

```text
rw-
```

### Group：`2`

数字权限中：

```text
read    = 4
write   = 2
execute = 1
```

因此：

```text
2 = -w-
```

表示清除 group 的写权限。

原本：

```text
rw-
```

变成：

```text
r--
```

### Others：`7`

```text
7 = rwx
```

表示清除：

* read
* write
* execute

因此 others 最终没有任何权限：

```text
---
```

---

## 最终权限为什么是 `640`

普通文件的基础权限：

```text
666
```

对应：

```text
rw-rw-rw-
```

应用：

```text
umask 027
```

后：

```text
owner:  rw-   → 不清除       → rw-
group:  rw-   → 清除 write   → r--
others: rw-   → 全部清除     → ---
```

最终得到：

```text
rw-r-----
```

即：

```text
640
```

可以概念化为：

```text
666
mask 027
↓
640
```

---

## `umask` 不是普通减法

不要把它简单理解成：

```text
最终权限 = 666 - umask
```

`umask` 实际做的是：

> **按位清除权限。**

更准确的表达是：

```text
最终权限 = 请求权限 AND NOT(umask)
```

例如：

```text
umask 023
```

如果错误地做数学减法：

```text
666 - 023 = 643
```

但实际结果是：

```text
644
```

因为普通文件的基础权限是：

```text
rw-rw-rw-
```

`umask 023` 要清除：

```text
group:  -w-
others: -wx
```

但是普通文件本来就没有 execute 权限，因此“清除 execute”不会产生额外变化。

最终：

```text
rw-r--r--
```

即：

```text
644
```

所以理解 `umask` 时，应始终使用“**清除权限位**”，而不是“减去一个数字”。

---

## 检查文件权限

创建文件后可以运行：

```bash
ls -l /home/analyst/report.txt
```

期望看到类似：

```text
-rw-r----- 1 analyst reports ... /home/analyst/report.txt
```

其中：

```text
-rw-r-----
││  │  │
││  │  └─ others: ---
││  └──── group: r--
│└─────── owner: rw-
└──────── 普通文件
```

如果想直接查看数字权限、owner 和 group，可以使用：

```bash
stat -c '%a %U %G' /home/analyst/report.txt
```

期望输出：

```text
640 analyst reports
```

其中：

* `%a`：数字权限，例如 `640`
* `%U`：owner 用户名
* `%G`：group 名称

---

## 为什么必须在创建文件之前设置 `umask`

`umask` 只影响：

> **之后新创建的文件和目录。**

例如：

```bash
touch report.txt
umask 027
```

不会自动把已经存在的 `report.txt` 改成 `640`。

如果文件已经创建，应使用：

```bash
chmod 640 report.txt
```

因此两者解决的问题不同：

| 命令      | 作用             |
| ------- | -------------- |
| `umask` | 控制以后新创建文件的默认权限 |
| `chmod` | 修改已经存在的文件或目录权限 |

如果目标是让文件**创建时就安全**，应该先设置 `umask`。

---

## `umask` 的作用范围

执行：

```bash
umask 027
```

通常只会修改**当前 shell 以及它启动的子进程**的 `umask`。

例如：

```bash
umask 027
touch report.txt
```

`report.txt` 会受到这个 `umask` 影响。

但是关闭这个 shell 后，该临时设置通常也就消失了。

这也是为什么下面的命令把两个操作放在同一个 Bash 中：

```bash
sudo -u analyst bash -c 'umask 027 && touch ~/report.txt'
```

这样可以保证：

```text
touch
```

运行时继承的正是：

```text
umask 027
```

---

## 关键命令

创建权限为 `640` 的 `report.txt`：

```bash
sudo -u analyst bash -c 'umask 027 && touch ~/report.txt'
```

检查权限：

```bash
ls -l /home/analyst/report.txt
```

精确检查数字权限、owner 和 group：

```bash
stat -c '%a %U %G' /home/analyst/report.txt
```

预期：

```text
640 analyst reports
```

## 核心记忆

```text
普通文件基础权限：666
目录基础权限：    777

umask = 要清除的权限
```

对于：

```bash
umask 027
```

含义是：

```text
owner  → 不清除
group  → 清除 write
others → 清除全部权限
```

因此普通文件通常最终得到：

```text
666 + umask 027
        ↓ 按位清除
640
```

即：

```text
rw-r-----
```
