# Bash 脚本：Shebang、执行权限与 PATH

## 1. 什么是 Bash 脚本

Bash 脚本本质上是一个文本文件，其中保存原本需要手动逐条执行的 Shell 命令。

例如：

```bash
#!/bin/bash
echo "hello from Linux"
```

保存后，可以把整组操作当成一个命令执行，而不必每次重新输入。

---

## 2. Shebang：声明使用哪个解释器

脚本第一行通常写：

```bash
#!/bin/bash
```

这一行叫 **shebang**。

格式是：

```text
#!解释器路径
```

例如：

```bash
#!/bin/bash
```

表示：

> 当这个文件被直接执行时，使用 `/bin/bash` 来解释其中的内容。

执行：

```bash
./script.sh
```

时，Linux 会读取文件开头的 `#!`，然后按照指定的解释器运行脚本。

可以把它近似理解为：

```bash
/bin/bash ./script.sh
```

### 为什么不能依赖 Bash 自动处理

没有 shebang 的脚本有时仍然会在 Bash 中“看起来能运行”，因为 Bash 遇到某些直接执行失败的文本文件时，可能尝试把它当作 Shell 脚本继续执行。

但这不是脚本应该依赖的行为。

脚本可能由其他程序、服务或调度器直接执行，因此可执行脚本应该明确声明解释器：

```bash
#!/bin/bash
```

---

## 3. 两种运行脚本的方式并不相同

假设存在：

```text
script.sh
```

### 方式一：让 Bash 读取它

```bash
bash script.sh
```

这里真正执行的是：

```text
bash
```

`script.sh` 只是 Bash 需要读取的输入文件。

因此 `script.sh` 本身通常**不需要执行权限**，只需要 Bash 有权限读取它。

---

### 方式二：直接执行文件

```bash
./script.sh
```

这时 Linux 把 `script.sh` 本身当作可执行文件，因此通常需要：

1. 文件具有执行权限；
2. 文件中有正确的 shebang。

例如：

```bash
#!/bin/bash
```

这是理解 Bash 脚本权限问题最重要的区别之一：

| 命令                            | 脚本需要执行权限吗 | 是否依赖 shebang |
| ----------------------------- | --------: | -----------: |
| `bash script.sh`              |         否 |            否 |
| `./script.sh`                 |         是 |            是 |
| `/home/learner/bin/script.sh` |         是 |            是 |

---

## 4. Execute Bit：执行权限

Linux 文件权限包含：

* `r`：read，读取
* `w`：write，写入
* `x`：execute，执行

例如：

```bash
chmod 755 script.sh
```

`755` 可以拆成：

```text
7 5 5
│ │ │
│ │ └─ other
│ └─── group
└───── owner
```

数字对应的权限值：

```text
r = 4
w = 2
x = 1
```

所以：

```text
7 = 4 + 2 + 1 = rwx
5 = 4 + 1     = r-x
5 = 4 + 1     = r-x
```

因此：

```bash
chmod 755 script.sh
```

产生：

```text
rwxr-xr-x
```

含义是：

| 用户    | 权限     |
| ----- | ------ |
| 文件所有者 | 读、写、执行 |
| 同组用户  | 读、执行   |
| 其他用户  | 读、执行   |

可以使用：

```bash
ls -l script.sh
```

检查：

```text
-rwxr-xr-x
```

---

## 5. 为什么 `644` 的脚本不能直接执行

权限：

```text
644
```

相当于：

```text
rw-r--r--
```

没有任何 `x` 权限。

因此：

```bash
./script.sh
```

通常会得到：

```text
Permission denied
```

但是：

```bash
bash script.sh
```

仍然可能正常工作，因为 Bash 只需要**读取**脚本，而不是把脚本本身直接执行。

因此：

```text
可读取 ≠ 可直接执行
```

---

## 6. `~/bin`：个人命令目录

目录：

```text
~/bin
```

通常用于保存只属于当前用户的个人命令和脚本。

假设：

```text
/home/learner/bin/course-greet
```

它也可以写成：

```text
~/bin/course-greet
```

因为：

```text
~
```

表示当前用户的 home directory（家目录）。

对于用户 `learner`：

```text
~ = /home/learner
```

所以：

```text
~/bin/course-greet
```

等价于：

```text
/home/learner/bin/course-greet
```

---

## 7. PATH 决定能否只写命令名

Shell 执行：

```bash
course-greet
```

时，并不会搜索整个磁盘，而是只搜索环境变量：

```bash
PATH
```

中列出的目录。

可以查看：

```bash
echo "$PATH"
```

例如可能得到：

```text
/usr/local/bin:/usr/bin:/bin:/home/learner/bin
```

如果：

```text
/home/learner/bin
```

在 `PATH` 中，那么：

```bash
course-greet
```

就可以找到：

```text
/home/learner/bin/course-greet
```

如果它不在 `PATH` 中，则仍然可以明确指定文件路径：

```bash
~/bin/course-greet
```

因此：

```text
PATH 决定的是能不能省略路径，
而不是脚本本身能不能执行。
```

---

## 8. 标准输出与重定向

脚本：

```bash
echo "hello from Linux"
```

默认把内容发送到 **标准输出（stdout）**，通常就是终端。

例如：

```bash
~/bin/course-greet
```

终端显示：

```text
hello from Linux
```

如果使用：

```bash
~/bin/course-greet > ~/greeting.txt
```

`>` 会把标准输出写入文件。

于是：

```text
/home/learner/greeting.txt
```

内容变成：

```text
hello from Linux
```

### `>` 会覆盖文件

如果目标文件已经存在：

```bash
command > file
```

会先清空原内容，再写入新的输出。

而：

```bash
command >> file
```

则是在文件末尾追加内容。

---

## 9. 一个脚本能够直接执行的完整条件

典型 Bash 脚本：

```bash
#!/bin/bash
echo "hello"
```

如果希望这样运行：

```bash
./script.sh
```

通常需要同时满足：

1. 有正确的 shebang；
2. 有执行权限：

```bash
chmod 755 script.sh
```

3. 使用正确的路径找到文件：

   * `./script.sh`
   * `/完整/路径/script.sh`
   * 或脚本所在目录已经加入 `PATH`。

可以记成：

```text
直接执行脚本
    │
    ├── 谁解释？ → shebang
    ├── 能执行吗？ → x 权限
    └── 去哪里找？ → 路径 / PATH
```

---

# Bash 脚本创建任务：`course-greet`

## 问题

创建：

```text
/home/learner/bin/course-greet
```

要求：

* 是 Bash 脚本；
* 只输出：

```text
hello from Linux
```

* 输出后有一个换行；
* 文件权限为 `755`；
* 文件保持当前用户所有；
* 执行脚本；
* 将输出保存到：

```text
/home/learner/greeting.txt
```

最终文件中只能有这一行。

---

## 完整操作

### 1. 创建 `bin` 目录

```bash
mkdir -p /home/learner/bin
```

`-p` 表示目录已经存在时不会报错。

---

### 2. 创建脚本

```bash
cat > /home/learner/bin/course-greet <<'EOF'
#!/bin/bash
echo "hello from Linux"
EOF
```

最终脚本内容是：

```bash
#!/bin/bash
echo "hello from Linux"
```

其中：

```bash
#!/bin/bash
```

指定 Bash 作为解释器。

```bash
echo "hello from Linux"
```

输出文本，并自动在末尾添加换行。

---

### 3. 设置执行权限

```bash
chmod 755 /home/learner/bin/course-greet
```

权限应成为：

```text
rwxr-xr-x
```

因为没有使用 `chown` 修改所有者，所以文件仍然属于创建它的当前用户。

---

### 4. 执行脚本并重定向输出

```bash
/home/learner/bin/course-greet > /home/learner/greeting.txt
```

这里：

```bash
/home/learner/bin/course-greet
```

直接执行脚本。

而：

```bash
> /home/learner/greeting.txt
```

把脚本的标准输出写入文件。

---

## 验证结果

检查脚本权限：

```bash
ls -l /home/learner/bin/course-greet
```

应看到类似：

```text
-rwxr-xr-x 1 learner learner ... /home/learner/bin/course-greet
```

检查输出文件：

```bash
cat /home/learner/greeting.txt
```

应该只显示：

```text
hello from Linux
```

也可以执行：

```bash
wc -l /home/learner/greeting.txt
```

应得到一行：

```text
1 /home/learner/greeting.txt
```

最终核心命令可以概括为：

```bash
mkdir -p /home/learner/bin

cat > /home/learner/bin/course-greet <<'EOF'
#!/bin/bash
echo "hello from Linux"
EOF

chmod 755 /home/learner/bin/course-greet
/home/learner/bin/course-greet > /home/learner/greeting.txt
```
