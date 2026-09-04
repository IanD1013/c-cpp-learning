# Bash 脚本的执行权限、Shebang 与 Here Document

## 1. 为什么 `./greet.sh` 会出现 `Permission denied`

Linux/Unix 并不会因为文件名以 `.sh` 结尾，就把它当作“可执行的 Shell 脚本”。

`.sh` 只是文件名的一部分，主要方便人识别。真正决定：

```bash
./greet.sh
```

能否执行的，是文件的**执行权限（execute bit）**。

例如查看权限：

```bash
ls -l greet.sh
```

可能得到：

```text
-rw-r--r-- 1 user user 32 Sep 4 17:00 greet.sh
```

最前面的：

```text
-rw-r--r--
```

表示：

| 部分           | 权限        |
| ------------ | --------- |
| owner（所有者）   | `rw-`：读、写 |
| group（组）     | `r--`：读   |
| others（其他用户） | `r--`：读   |

这里完全没有 `x`，所以：

```bash
./greet.sh
```

通常会得到：

```text
bash: ./greet.sh: Permission denied
```

---

## 2. 读取权限和执行权限不是一回事

Unix 权限主要有三种：

| 字母  | 数值 | 含义         |
| --- | -: | ---------- |
| `r` |  4 | read，读取    |
| `w` |  2 | write，修改   |
| `x` |  1 | execute，执行 |

例如：

```text
rwx
```

对应：

```text
4 + 2 + 1 = 7
```

而：

```text
r--
```

对应：

```text
4
```

因此：

```bash
chmod 744 greet.sh
```

表示：

```text
owner   group   others
  7       4       4
 rwx     r--     r--
```

最终权限就是：

```text
-rwxr--r--
```

也就是：

* 文件所有者：可以读、写、执行
* 同组用户：只能读
* 其他用户：只能读

---

## 3. `chmod +x` 和 `chmod 744` 的区别

最常见的做法是：

```bash
chmod +x greet.sh
```

意思是：

> 给文件增加执行权限。

但是如果没有明确写：

```bash
u+x
g+x
o+x
```

那么具体哪些类别会受到影响，可能受到当前 `umask` 等规则影响。

因此，如果题目要求：

> 权限必须**精确地**是 `744`

最好直接使用：

```bash
chmod 744 greet.sh
```

这样不会依赖文件原来的权限状态。

### 更明确的符号写法

也可以写成：

```bash
chmod u=rwx,g=r,o=r greet.sh
```

它与：

```bash
chmod 744 greet.sh
```

效果相同。

---

## 4. 可执行权限并不等于脚本一定能被内核直接运行

需要区分两个问题：

1. **有没有权限执行这个文件？**
2. **应该由哪个程序解释这个文件？**

`x` 主要解决第一个问题。

对于 Shell 脚本，第二个问题通常由 **shebang** 解决：

```bash
#!/usr/bin/env bash
```

如果执行：

```bash
./greet.sh
```

操作系统会检查文件开头，从 shebang 得知：

> 这个文件应该交给 Bash 解释执行。

因此，一个标准 Bash 脚本通常同时需要：

```text
执行权限 + 正确的 shebang
```

---

## 5. Shebang 是什么

脚本第一行：

```bash
#!/usr/bin/env bash
```

称为 **shebang**。

其中：

```text
#!
```

是特殊标记。

后面的：

```text
/usr/bin/env bash
```

告诉系统使用什么解释器执行脚本。

完整过程可以理解为：

```text
./greet.sh
    ↓
操作系统读取第一行
    ↓
#!/usr/bin/env bash
    ↓
运行 /usr/bin/env
    ↓
env 在 PATH 中寻找 bash
    ↓
使用找到的 bash 执行 greet.sh
```

---

## 6. 为什么常见写法是 `/usr/bin/env bash`

可以直接写：

```bash
#!/bin/bash
```

这种方式指定 Bash 必须位于：

```text
/bin/bash
```

另一种方式是：

```bash
#!/usr/bin/env bash
```

这里 `env` 会根据当前：

```text
PATH
```

寻找 `bash`。

例如：

```text
PATH=/usr/local/bin:/usr/bin:/bin
```

`env` 会按照这些目录寻找 Bash。

因此：

```bash
#!/usr/bin/env bash
```

通常比硬编码 Bash 的具体安装位置更加灵活。

> 但它也意味着具体使用哪个 Bash 取决于当前 `PATH`。对于需要严格固定解释器位置的环境，固定路径可能更合适。

---

## 7. Shebang 必须位于第一行

正确：

```bash
#!/usr/bin/env bash
echo "Hello"
```

错误：

```bash

#!/usr/bin/env bash
echo "Hello"
```

也不要这样：

```bash
# My script
#!/usr/bin/env bash
echo "Hello"
```

对于通过：

```bash
./greet.sh
```

直接执行的脚本，shebang 应当从文件的**第一个字节**开始。

---

## 8. `./greet.sh` 和 `bash greet.sh` 的重要区别

这两个命令表面上很像：

```bash
./greet.sh
```

```bash
bash greet.sh
```

但执行机制不同。

### `./greet.sh`

这是在说：

> 把 `greet.sh` 本身作为一个可执行文件运行。

因此通常需要：

```text
执行权限 x
```

并且脚本应有正确 shebang。

---

### `bash greet.sh`

这是在说：

> 启动 Bash，让 Bash 打开并解释 `greet.sh`。

此时真正被执行的程序是：

```text
bash
```

`greet.sh` 本身通常只需要 Bash 能够读取，而不需要自己的 `x` 权限。

例如：

```bash
chmod 644 greet.sh
bash greet.sh
```

仍然可以正常工作。

但：

```bash
chmod 644 greet.sh
./greet.sh
```

通常会失败：

```text
Permission denied
```

### 核心区别

```text
bash greet.sh
     ↑
Bash 主动读取文件

./greet.sh
↑
操作系统直接执行文件
```

因此，测试一个脚本是否真正配置成“可执行脚本”时，应使用：

```bash
./greet.sh
```

而不是：

```bash
bash greet.sh
```

---

## 9. 为什么 Bash 脚本最好明确写 shebang

假设脚本使用 Bash 特有语法：

```bash
#!/usr/bin/env bash

names=("Alice" "Bob")
echo "${names[0]}"
```

数组：

```bash
names=("Alice" "Bob")
```

是 Bash 语法，并不是所有 `/bin/sh` 实现都支持。

如果某个程序最终使用 `/bin/sh` 来解释这个文件，在 Debian/Ubuntu 等系统中 `/bin/sh` 常常指向 `dash`，于是可能出现语法错误。

因此，只要脚本依赖 Bash，就应该明确写：

```bash
#!/usr/bin/env bash
```

而不要依赖调用程序“猜”应该使用哪个 Shell。

---

# 使用 Here Document 创建脚本

## 10. `cat > greet.sh` 是什么

下面的命令：

```bash
cat > greet.sh
```

可以拆成：

```text
cat
 ↓
读取标准输入 stdin

>
 ↓
把标准输出重定向到 greet.sh
```

因此，如果输入：

```text
hello
```

`cat` 会把它输出，而 `>` 又把这个输出写入：

```text
greet.sh
```

---

## 11. Here Document：`<<`

创建多行文件时，经常使用：

```bash
cat > greet.sh <<'SCRIPT'
#!/usr/bin/env bash
echo "Hello"
SCRIPT
```

其中：

```bash
<<'SCRIPT'
```

叫作 **here-document（Here Doc）**。

它表示：

> 从下一行开始，把内容作为当前命令的标准输入，直到遇到只包含 `SCRIPT` 的那一行为止。

可以理解为：

```text
#!/usr/bin/env bash
echo "Hello"
```

被送给：

```bash
cat
```

然后：

```bash
>
```

把 `cat` 的输出写进：

```text
greet.sh
```

最终文件内容为：

```bash
#!/usr/bin/env bash
echo "Hello"
```

---

## 12. 结束标记必须单独占一行

例如：

```bash
cat > greet.sh <<'SCRIPT'
#!/usr/bin/env bash
echo "Hello"
SCRIPT
```

最后的：

```text
SCRIPT
```

必须作为结束标记。

通常应当：

* 单独一行
* 前面没有普通缩进
* 后面没有其他文字

例如下面不是相同的结束标记：

```text
SCRIPT hello
```

---

## 13. 为什么推荐 `<<'SCRIPT'`

下面两种写法有重要区别：

```bash
<<SCRIPT
```

与：

```bash
<<'SCRIPT'
```

### 不加引号

例如：

```bash
name="Alice"

cat > greet.sh <<SCRIPT
echo "$name"
SCRIPT
```

当前 Shell 会先展开：

```bash
$name
```

所以实际写入文件的可能是：

```bash
echo "Alice"
```

---

### 加单引号

```bash
name="Alice"

cat > greet.sh <<'SCRIPT'
echo "$name"
SCRIPT
```

这里：

```bash
$name
```

不会在创建文件时展开。

实际写入：

```bash
echo "$name"
```

因此，当使用 Here Doc 创建 Shell 脚本、配置文件或代码文件时，通常更安全的写法是：

```bash
<<'EOF'
```

或者：

```bash
<<'SCRIPT'
```

这样能够尽可能把内容**原样写入文件**。

---

## 14. `>` 会覆盖已有文件

命令：

```bash
cat > greet.sh
```

中的：

```text
>
```

表示覆盖写入。

如果 `greet.sh` 已经存在，它原本的内容会被截断，然后写入新内容。

如果想追加内容，则使用：

```bash
>>
```

例如：

```bash
echo 'echo "Bye"' >> greet.sh
```

---

# 一个完整的 Bash 脚本创建流程

```bash
cat > greet.sh <<'SCRIPT'
#!/usr/bin/env bash
echo "Hello"
SCRIPT

chmod 744 greet.sh

./greet.sh
```

执行过程：

```text
Here Doc
   ↓
创建 greet.sh
   ↓
chmod 744
   ↓
增加正确的执行权限
   ↓
./greet.sh
   ↓
根据 shebang 找到 Bash
   ↓
Bash 执行 echo "Hello"
   ↓
输出 Hello
```

---

# 常见误区

## 误区 1：`.sh` 代表文件可以执行

错误。

文件名：

```text
greet.sh
```

并不会自动获得 Shell 脚本属性或执行权限。

系统主要关心：

```text
权限位
文件格式 / shebang
```

---

## 误区 2：能读取文件就一定能执行

错误。

例如：

```text
-rw-r--r--
```

文件是可读的，但没有：

```text
x
```

所以不能通过：

```bash
./greet.sh
```

直接执行。

---

## 误区 3：`chmod +x` 一定得到 `744`

错误。

`+x` 是在**现有权限基础上增加执行权限**，不是把整个权限设置成固定值。

如果要求精确权限：

```text
744
```

应该使用：

```bash
chmod 744 greet.sh
```

---

## 误区 4：`bash greet.sh` 可以运行，就说明权限正确

错误。

```bash
bash greet.sh
```

运行的是 Bash，Bash 再读取脚本。

它不能证明：

```bash
./greet.sh
```

能够成功执行。

测试执行权限应该直接：

```bash
./greet.sh
```

---

## 误区 5：shebang 可以放在注释下面

错误。

应该：

```bash
#!/usr/bin/env bash
# comment
```

而不是：

```bash
# comment
#!/usr/bin/env bash
```

---

# 知识点总结

一个可以通过：

```bash
./script.sh
```

直接运行的 Bash 脚本，通常需要同时满足：

```text
1. 正确的执行权限
2. 第一行正确的 shebang
3. 脚本内容符合指定解释器的语法
```

例如：

```bash
#!/usr/bin/env bash
echo "Hello"
```

设置精确权限：

```bash
chmod 744 script.sh
```

Here Document 适合一次创建多行脚本：

```bash
cat > script.sh <<'SCRIPT'
#!/usr/bin/env bash
echo "Hello"
SCRIPT
```

其中给结束标记加引号：

```bash
<<'SCRIPT'
```

可以避免创建文件时提前进行 `$变量`、命令替换等 Shell 展开。

---

# 问题解答：创建满足要求的 `greet.sh`

## 问题

在 `bash-lab` 中创建 `greet.sh`，要求：

* 第一行必须是：

```bash
#!/usr/bin/env bash
```

* 文件权限必须精确为 `744`
* 必须能够通过：

```bash
./greet.sh
```

直接运行

* 输出必须恰好为：

```text
Hello
```

并带一个换行

* 退出状态必须为 `0`

## 答案

执行：

```bash
cat > greet.sh <<'SCRIPT'
#!/usr/bin/env bash
echo "Hello"
SCRIPT

chmod 744 greet.sh
```

然后运行：

```bash
./greet.sh
```

输出：

```text
Hello
```

## 为什么满足要求

### 1. Shebang 正确

文件第一行：

```bash
#!/usr/bin/env bash
```

因此直接执行：

```bash
./greet.sh
```

时会使用 Bash 解释脚本。

### 2. 权限精确为 `744`

```bash
chmod 744 greet.sh
```

产生：

```text
-rwxr--r--
```

即：

```text
owner  = rwx = 7
group  = r-- = 4
others = r-- = 4
```

### 3. 输出恰好是 `Hello` 加换行

```bash
echo "Hello"
```

默认输出：

```text
Hello\n
```

因此没有额外文字。

### 4. 退出状态为 `0`

脚本最后执行的是：

```bash
echo "Hello"
```

正常执行时 `echo` 返回：

```text
0
```

脚本没有显式 `exit` 时，退出状态默认是最后一条命令的退出状态，因此整个脚本返回：

```text
0
```

## 验证

检查权限：

```bash
stat -c '%a' greet.sh
```

应输出：

```text
744
```

检查运行结果：

```bash
./greet.sh
```

应输出：

```text
Hello
```

检查退出状态：

```bash
echo $?
```

应输出：

```text
0
```

因此完整答案可以写成：

```bash
cat > greet.sh <<'SCRIPT'
#!/usr/bin/env bash
echo "Hello"
SCRIPT
chmod 744 greet.sh
./greet.sh
```
