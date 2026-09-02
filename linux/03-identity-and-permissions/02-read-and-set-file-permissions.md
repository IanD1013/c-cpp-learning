# Linux 文件权限与 `chmod`：用数字模式和符号模式设置 `640`

## 目标权限

假设有两个普通文件：

```text
/home/learner/lab/numeric.txt
/home/learner/lab/symbolic.txt
```

要求它们最终都具有以下权限：

* 文件所有者（owner）：可读、可写
* 所属组（group）：只读
* 其他用户（other）：无任何权限

对应权限模式：

```text
640
```

用 `ls -l` 查看时应显示：

```text
-rw-r-----
```

---

## 1. 理解 `ls -l` 的权限字段

执行：

```bash
ls -l /home/learner/lab/numeric.txt /home/learner/lab/symbolic.txt
```

可能看到类似：

```text
-rw-r--r-- 1 learner learner ... numeric.txt
-rw-r--r-- 1 learner learner ... symbolic.txt
```

第一列：

```text
-rw-r--r--
```

共有 10 个字符：

```text
- rw- r-- r--
│ │   │   │
│ │   │   └── other：其他用户
│ │   └────── group：所属组
│ └────────── owner：文件所有者
└──────────── 文件类型
```

### 第一个字符：文件类型

常见值：

| 字符  | 含义   |
| --- | ---- |
| `-` | 普通文件 |
| `d` | 目录   |

因此：

```text
-rw-r--r--
```

开头的 `-` 表示这是一个普通文件。

---

## 2. `r`、`w`、`x` 的含义

每一类用户都有三个权限位：

```text
rwx
```

含义：

| 权限  | 英文      | 含义     |
| --- | ------- | ------ |
| `r` | read    | 读取     |
| `w` | write   | 写入、修改  |
| `x` | execute | 执行     |
| `-` | —       | 没有对应权限 |

例如：

```text
rw-
```

表示：

* 可以读取
* 可以写入
* 不可以执行

---

## 3. 如何读取 `-rw-r--r--`

将它拆开：

```text
- rw- r-- r--
```

表示：

| 类别    | 权限    | 含义    |
| ----- | ----- | ----- |
| 文件类型  | `-`   | 普通文件  |
| owner | `rw-` | 可读、可写 |
| group | `r--` | 只读    |
| other | `r--` | 只读    |

因此：

```text
-rw-r--r--
```

的问题在于：

> 其他所有用户仍然可以读取这个文件。

如果文件包含内部报告，希望其他用户完全无法访问，就需要把最后一个：

```text
r--
```

改成：

```text
---
```

最终得到：

```text
-rw-r-----
```

---

# 4. `chmod` 的作用

`chmod` 用来修改文件或目录的权限：

```bash
chmod MODE FILE
```

例如：

```bash
chmod 640 report.txt
```

`chmod` 支持两种常见写法：

1. 数字模式（numeric mode）
2. 符号模式（symbolic mode）

它们最终修改的是同一组权限位。

---

# 5. 数字模式：为什么是 `640`

数字权限使用以下数值：

| 权限          | 数值 |
| ----------- | -: |
| read `r`    |  4 |
| write `w`   |  2 |
| execute `x` |  1 |

一个用户类别的权限数字，就是把需要的权限加起来。

例如：

```text
rw- = 4 + 2 = 6
r-- = 4
--- = 0
```

目标权限为：

```text
owner: rw-
group: r--
other: ---
```

所以：

```text
6 4 0
```

即：

```text
640
```

---

## 数字权限常见组合

|  数字 | 权限    |
| --: | ----- |
| `0` | `---` |
| `1` | `--x` |
| `2` | `-w-` |
| `3` | `-wx` |
| `4` | `r--` |
| `5` | `r-x` |
| `6` | `rw-` |
| `7` | `rwx` |

因此：

```bash
chmod 640 file.txt
```

等价于：

```text
owner = rw-
group = r--
other = ---
```

---

# 6. 使用数字模式修改文件

对 `numeric.txt` 使用数字形式：

```bash
chmod 640 /home/learner/lab/numeric.txt
```

执行后，它的权限应变成：

```text
-rw-r-----
```

这里不需要 `sudo`，因为文件本身属于当前用户 `learner`。

普通用户通常可以修改**自己拥有的文件**的权限。

---

# 7. 符号模式：`u=rw,g=r,o=`

符号模式不用数字，而是直接描述不同用户类别应该拥有什么权限。

用户类别：

| 字符  | 含义          |
| --- | ----------- |
| `u` | user，文件所有者  |
| `g` | group，所属组   |
| `o` | others，其他用户 |
| `a` | all，所有类别    |

权限仍然使用：

```text
r
w
x
```

因此：

```bash
chmod u=rw,g=r,o= file.txt
```

表示：

```text
u=rw
```

文件所有者拥有：

```text
rw-
```

```text
g=r
```

所属组拥有：

```text
r--
```

```text
o=
```

其他用户没有任何权限：

```text
---
```

合起来就是：

```text
-rw-r-----
```

也就是数字权限：

```text
640
```

---

# 8. 使用符号模式修改文件

对 `symbolic.txt`：

```bash
chmod u=rw,g=r,o= /home/learner/lab/symbolic.txt
```

这与下面的命令最终效果相同：

```bash
chmod 640 /home/learner/lab/symbolic.txt
```

区别只是写法不同。

---

# 9. 完整操作

```bash
ls -l /home/learner/lab/numeric.txt /home/learner/lab/symbolic.txt

chmod 640 /home/learner/lab/numeric.txt

chmod u=rw,g=r,o= /home/learner/lab/symbolic.txt

ls -l /home/learner/lab/numeric.txt /home/learner/lab/symbolic.txt
```

最终应该看到类似：

```text
-rw-r----- 1 learner learner ... numeric.txt
-rw-r----- 1 learner learner ... symbolic.txt
```

---

# 10. `=` 与 `+`、`-` 的区别

符号模式中，不只有 `=`。

## `=`：精确设置

```bash
chmod u=rw file.txt
```

表示：

> owner 的权限必须恰好是 `rw-`。

如果之前有执行权限 `x`，也会被清除。

---

## `+`：增加权限

例如：

```bash
chmod g+r file.txt
```

表示：

> 给 group 增加读取权限，其他已有权限保持不变。

---

## `-`：删除权限

例如：

```bash
chmod o-r file.txt
```

表示：

> 删除 other 的读取权限。

因此，如果只想解决：

```text
-rw-r--r--
```

中“其他用户可以读取”的问题，也可以写：

```bash
chmod o-r file.txt
```

但如果策略明确规定完整权限状态，使用：

```bash
chmod u=rw,g=r,o= file.txt
```

更加明确，因为它直接规定最终结果。

---

# 11. 为什么不要随便使用 `chmod 777`

`777` 表示：

```text
rwxrwxrwx
```

也就是：

* owner 可以读、写、执行
* group 可以读、写、执行
* other 也可以读、写、执行

对于普通报告文件，这通常不仅没有必要，而且会造成严重的权限过度开放。

例如：

```bash
chmod 777 report.txt
```

意味着机器上的其他用户也可能：

* 修改报告
* 删除或覆盖内容
* 获得不必要的执行权限

正确原则是：

> 根据实际需求授予最少权限，而不是遇到权限问题就使用 `777`。

这就是 **最小权限原则（Principle of Least Privilege）**。

---

# 12. 为什么这里不需要 `sudo`

文件权限能否被修改，主要取决于你是不是文件所有者，而不是命令本身是不是“系统命令”。

例如：

```text
-rw-r--r-- 1 learner learner ... numeric.txt
```

这里：

```text
learner learner
```

通常分别表示：

```text
owner group
```

如果当前用户就是文件所有者 `learner`，那么：

```bash
chmod 640 numeric.txt
```

通常可以直接执行。

没有必要写：

```bash
sudo chmod 640 numeric.txt
```

原则是：

> 普通权限足够时，不要使用 `sudo`。

---

# 13. `chmod` 与文件所有权不是一回事

需要区分两个概念：

## `chmod`

修改**权限**：

```bash
chmod 640 file.txt
```

改变的是：

```text
rw-r-----
```

---

## `chown`

修改**所有者或所属组**：

```bash
chown user:group file.txt
```

例如：

```bash
sudo chown learner:learner file.txt
```

改变的是：

```text
owner / group
```

因此：

```text
-rw-r-----  learner learner
^^^^^^^^^^  ^^^^^^^ ^^^^^^^
权限         owner   group
```

这是两套不同的信息。

---

# 14. 快速记忆

目标：

```text
owner  → read + write
group  → read
other  → nothing
```

数字形式：

```text
rw- = 6
r-- = 4
--- = 0
```

所以：

```bash
chmod 640 file.txt
```

符号形式：

```bash
chmod u=rw,g=r,o= file.txt
```

最终：

```text
-rw-r-----
```

核心对应关系：

```text
640
│││
││└── other = 0 = ---
│└─── group = 4 = r--
└──── owner = 6 = rw-
```
