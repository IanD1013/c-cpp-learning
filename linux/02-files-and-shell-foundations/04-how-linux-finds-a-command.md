# Bash 如何找到并执行命令：Built-in、PATH 与命令定位

## 1. 输入命令后，Bash 到底做了什么？

当你输入：

```bash
ls
```

Bash 不会直接假设 `ls` 是 `/usr/bin/ls`。它需要先判断 **`ls` 这个名字代表什么**。

常见情况包括：

* **Alias（别名）**
* **Shell Built-in（Shell 内建命令）**
* **外部可执行程序**
* Shell 函数等其他 Bash 可识别的命令形式

因此：

> 一个“命令名”不一定对应磁盘上的某个可执行文件。

---

## 2. Shell Built-in：命令本身属于 Bash

有些命令直接由 Bash 实现，而不是单独存放在某个可执行文件中。

典型例子：

```bash
cd
```

`cd` 是 Bash built-in。

因此一般不能期待找到类似：

```text
/usr/bin/cd
```

这样的独立程序。

### 为什么 `cd` 必须由 Shell 自己实现？

`cd` 的作用是修改 **当前 Shell 进程的工作目录**。

如果 `cd` 只是启动一个外部程序：

```text
Bash
 └── cd 子进程
```

那么只能改变子进程自己的目录；程序结束后，Bash 本身仍然停留在原目录。

所以 `cd` 必须由 Shell 自己执行。

---

## 3. 外部命令：磁盘上的可执行文件

很多命令实际上是磁盘上的独立程序，例如：

```bash
/usr/bin/ls
```

如果直接输入：

```bash
ls
```

Bash 就需要找到这个程序的位置。

这通常依赖 **PATH**。

---

# PATH：Bash 的命令搜索路径

`PATH` 是一个环境变量，其中保存了一组目录。

可以查看：

```bash
echo "$PATH"
```

典型结果类似：

```text
/usr/local/bin:/usr/bin:/bin
```

这些目录使用 `:` 分隔。

当 Bash 需要寻找外部命令时，会按照顺序搜索：

```text
/usr/local/bin
↓
/usr/bin
↓
/bin
```

找到第一个匹配且可执行的程序后就停止搜索。

例如：

```bash
ls
```

可能最终找到：

```text
/usr/bin/ls
```

---

## PATH 中目录的顺序很重要

假设：

```text
PATH=/home/user/bin:/usr/bin
```

而两个目录中都存在：

```text
ls
```

Bash 会优先执行：

```text
/home/user/bin/ls
```

因为它排在 `/usr/bin` 前面。

因此：

> PATH 不只是“去哪些地方找命令”，还决定了同名程序的优先级。

---

# PATH 找不到程序时怎么办？

假设某个程序位于：

```text
/opt/course/bin/course-status
```

但 `/opt/course/bin` 不在 `PATH` 中。

此时输入：

```bash
course-status
```

可能得到：

```text
command not found
```

这并不代表程序不存在。

只是 Bash 无法通过 PATH 找到它。

可以直接提供完整路径：

```bash
/opt/course/bin/course-status
```

只要文件存在并具有执行权限，就可以运行。

---

# 命令名 vs 可执行文件路径

这两个概念不要混淆。

### 使用命令名

```bash
ls
```

需要 Bash：

1. 判断是否为 alias / built-in 等
2. 必要时搜索 PATH
3. 找到程序
4. 执行

### 使用完整路径

```bash
/usr/bin/ls
```

已经明确告诉 Bash 要运行哪个文件，因此不需要 PATH 搜索。

---

# 如何查看 Bash 实际会执行什么？

不要靠猜，可以直接询问 Shell。

## `type`

```bash
type cd
```

可能得到：

```text
cd is a shell builtin
```

检查 `ls`：

```bash
type ls
```

如果 `ls` 被设置成 alias，可能看到：

```text
ls is aliased to `ls --color=auto'
```

`type` 特别适合回答：

> “Bash 会如何解释这个名字？”

它可以识别：

* alias
* built-in
* function
* 外部程序

因此在 Bash 中，`type` 是非常实用的诊断工具。

---

# `command -v`

```bash
command -v ls
```

它用于查看 Bash 会解析到什么命令。

对于普通外部程序，可能输出：

```text
/usr/bin/ls
```

对于 built-in：

```bash
command -v cd
```

可能输出：

```text
cd
```

如果命令是 alias，结果也可能显示 alias 的定义。

### 推荐用途

在 Shell 脚本中判断命令是否存在时，经常使用：

```bash
command -v git
```

例如：

```bash
if command -v git >/dev/null 2>&1; then
    echo "git is installed"
fi
```

相比 `which`，`command -v` 通常更符合 Shell 自身的命令解析规则。

---

# `which`

```bash
which ls
```

典型结果：

```text
/usr/bin/ls
```

`which` 主要搜索 `PATH` 中的可执行文件。

它关心的是：

> PATH 搜索会找到哪个程序文件？

因此，如果目标只是获取外部程序路径，`which` 很直观。

不过在 Shell 脚本中通常更推荐：

```bash
command -v
```

因为它能更准确反映 Shell 自身如何解析命令。

---

# `whereis`

```bash
whereis ls
```

它搜索的范围比 `which` 更广。

可能返回：

```text
ls: /usr/bin/ls /usr/share/man/man1/ls.1.gz
```

除了程序本身，它还可能找到：

* executable
* manual page
* 其他相关系统文件

因此：

> `whereis` 更适合寻找一个程序相关文件“在哪里”。

---

# `type`、`command -v`、`which`、`whereis` 的区别

| 命令           | 主要用途                | 能识别 alias / built-in | 查找程序文件 |
| ------------ | ------------------- | -------------------: | -----: |
| `type`       | 查看 Bash 如何解释命令      |                    ✅ |      ✅ |
| `command -v` | 查看 Shell 最终会使用什么    |                    ✅ |      ✅ |
| `which`      | 搜索 PATH 中的可执行文件     |              通常不作为重点 |      ✅ |
| `whereis`    | 搜索程序、man page 等相关位置 |                    ❌ |      ✅ |

日常排查优先考虑：

```bash
type command-name
command -v command-name
```

---

# Alias 为什么会影响结果？

例如系统可能定义：

```bash
alias ls='ls --color=auto'
```

那么输入：

```bash
ls
```

Bash 实际上会先展开为类似：

```bash
ls --color=auto
```

这也是为什么：

```bash
type ls
```

和：

```bash
which ls
```

可能显示不同的信息。

前者关注：

```text
Bash 如何理解 ls
```

后者主要关注：

```text
PATH 中 ls 程序文件在哪里
```

---

# 一个重要排查思路

如果输入：

```bash
some-command
```

得到：

```text
command not found
```

可以依次检查：

```bash
type some-command
command -v some-command
```

如果你已经知道程序的完整路径，则可以直接运行：

```bash
/path/to/some-command
```

因此：

> `command not found` 通常意味着 Shell 无法通过当前命令解析规则找到它，并不一定意味着文件不存在。

---

# 与输出重定向结合使用

命令的标准输出可以通过：

```bash
>
```

写入文件。

例如：

```bash
which ls > ~/lab/ls-command.path
```

如果：

```bash
which ls
```

输出：

```text
/usr/bin/ls
```

文件中就会保存：

```text
/usr/bin/ls
```

命令本身输出的换行也会一起写入文件。

---

## 运行 PATH 找不到的程序并保存输出

如果：

```bash
course-status
```

无法找到，但程序实际位于：

```text
/opt/course/bin/course-status
```

可以：

```bash
/opt/course/bin/course-status > ~/lab/course-status.txt
```

这里同时应用了两个知识点：

1. **完整路径可以绕过 PATH 搜索**
2. **`>` 可以把标准输出写入文件**

---

# 本节需要记住的核心结论

1. **命令名不一定对应可执行文件。**

   * 它可能是 alias、built-in 或外部程序。

2. **`cd` 是 Bash built-in。**

   * 它不是普通磁盘程序。

3. **外部命令通常通过 `PATH` 查找。**

4. **PATH 按从左到右的顺序搜索。**

   * 第一个匹配程序会被使用。

5. **程序不在 PATH 中，不代表程序不存在。**

   * 可以直接使用完整路径运行：

   ```bash
   /path/to/program
   ```

6. 想知道 Bash 如何解释命令，优先使用：

   ```bash
   type command
   command -v command
   ```

7. `which` 主要用于寻找 PATH 中的可执行文件：

   ```bash
   which ls
   ```

8. `whereis` 搜索范围更广，还可能找到 man page 等相关文件。

9. 可以把命令输出直接保存到文件：

   ```bash
   command > file
   ```
