# Bash 中合并标准输出与标准错误：`> combined.log 2>&1`

## 1. 核心问题：一个命令实际上有两条输出通道

在 Bash 中，一个程序通常至少有两条独立的输出流：

| 文件描述符 | 名称                          | 用途         |
| ----- | --------------------------- | ---------- |
| `1`   | standard output，标准输出 stdout | 正常结果       |
| `2`   | standard error，标准错误 stderr  | 错误、警告、诊断信息 |

例如：

```bash
printf 'reading notes.txt\n'
cat missing.txt
```

第一条命令的内容通常写入 stdout：

```text
reading notes.txt
```

而 `cat` 找不到文件时，会把错误信息写入 stderr：

```text
cat: missing.txt: No such file or directory
```

虽然两者默认都显示在同一个终端窗口里，但它们实际上是两个独立的数据流。

---

## 2. 为什么单独使用 `>` 不够

例如：

```bash
{
    printf 'reading %s\n' "$1"
    cat "$1"
    printf 'finished\n' >&2
} > combined.log
```

`>` 等价于：

```bash
1> combined.log
```

也就是：

> 只把文件描述符 `1`（stdout）重定向到文件。

因此：

* stdout → `combined.log`
* stderr → 仍然是终端

如果 `$1` 不存在，`cat` 的错误仍然会出现在屏幕上，而不会写进日志。

这会产生一个问题：以后查看 `combined.log` 时，看不到程序运行过程中完整的事件顺序。

---

# 3. `2>&1`：让 stderr 跟随 stdout 当前的目标

最常用的写法是：

```bash
> combined.log 2>&1
```

可以拆成两步理解。

首先：

```bash
> combined.log
```

等价于：

```bash
1> combined.log
```

此时：

```text
stdout (fd 1) ──────> combined.log
stderr (fd 2) ──────> terminal
```

然后执行：

```bash
2>&1
```

意思是：

> 把文件描述符 `1` **当前指向的位置**复制给文件描述符 `2`。

于是：

```text
stdout (fd 1) ─┐
               ├──> combined.log
stderr (fd 2) ─┘
```

最终 stdout 和 stderr 都写入同一个文件。

---

## 4. `2>&1` 中每个符号是什么意思

```bash
2>&1
```

可以理解为：

```text
2  >  &1
│     │
│     └── 文件描述符 1
└──────── 重定向文件描述符 2
```

其中：

* `2`：stderr
* `>`：重定向
* `&1`：这里的 `1` 是**文件描述符编号**

因此：

```bash
2>&1
```

不是“写入一个叫 `1` 的文件”，而是：

> 让 stderr 使用 stdout 当前正在使用的目标。

---

## 5. 为什么 `&` 不能省略

下面两个命令完全不同：

```bash
2>&1
```

和：

```bash
2>1
```

### `2>&1`

表示：

```text
stderr → stdout 当前的目标
```

### `2>1`

表示：

```text
stderr → 名字叫 1 的普通文件
```

例如：

```bash
cat missing.txt 2>1
```

会在当前目录创建一个文件：

```text
1
```

其中包含：

```text
cat: missing.txt: No such file or directory
```

所以这里的 `&` 非常重要：

> `&` 告诉 Bash：“后面的数字是文件描述符，不是文件名。”

---

# 6. 重定向顺序非常重要

Bash 按照**从左到右**的顺序处理重定向。

因此下面两种写法并不等价。

## 正确：stdout 和 stderr 都进入日志

```bash
command > combined.log 2>&1
```

执行过程：

### 第一步

```bash
> combined.log
```

得到：

```text
stdout ──> combined.log
stderr ──> terminal
```

### 第二步

```bash
2>&1
```

stderr 复制 stdout **当前的目标**：

```text
stdout ─┐
        ├──> combined.log
stderr ─┘
```

最终：

```text
stdout + stderr → combined.log
```

---

## 不同结果：`2>&1 > combined.log`

```bash
command 2>&1 > combined.log
```

执行过程相反。

### 第一步

```bash
2>&1
```

这时 stdout 还指向终端，所以：

```text
stdout ──> terminal
stderr ──> terminal
```

stderr 被设置为当前 stdout 的目标，也就是终端。

### 第二步

```bash
> combined.log
```

只改变 stdout：

```text
stdout ──> combined.log
stderr ──> terminal
```

最终结果：

* stdout → 文件
* stderr → 屏幕

所以关键原则是：

> `2>&1` 复制的是 stdout **执行到这一刻时的目标**，不是建立一个永久的“stderr 跟随 stdout”关系。

---

# 7. 使用 `{ ...; }` 给多个命令统一重定向

如果多个命令都需要使用相同的重定向，可以使用 Bash 的 **brace group（花括号命令组）**：

```bash
{
    command1
    command2
    command3
} > combined.log 2>&1
```

这样最后的：

```bash
> combined.log 2>&1
```

会作用于整个命令组。

例如：

```bash
{
    printf 'reading %s\n' "$1"
    cat "$1"
    printf 'finished\n' >&2
} > combined.log 2>&1
```

这里三个命令的 stdout 和 stderr 最终都会进入：

```text
combined.log
```

---

## 8. 一行写花括号命令组时的语法

可以写成：

```bash
{ first; second; third; } > combined.log 2>&1
```

需要注意几个 Bash 语法细节：

```bash
{ first; second; third; }
^                      ^
```

### `{` 后面必须有空格

正确：

```bash
{ first;
```

错误：

```bash
{first;
```

### 最后一个命令后必须有 `;`

正确：

```bash
third; }
```

不能直接写：

```bash
third }
```

因为 Bash 必须先知道 `third` 命令已经结束，然后才能把 `}` 识别为命令组结束符。

### `}` 前也需要分隔

通常写：

```bash
third; }
```

所以最常见的一行形式是：

```bash
{ first; second; third; } > combined.log 2>&1
```

---

# 9. `>&2`：主动把某条输出发送到 stderr

假设有：

```bash
printf 'finished\n' >&2
```

`printf` 默认把结果写到 stdout：

```text
printf → fd 1
```

加上：

```bash
>&2
```

等价于：

```bash
1>&2
```

意思是：

> 把这个命令的 stdout 改成 stderr 当前的目标。

所以：

```bash
printf 'finished\n' >&2
```

实际上让：

```text
finished
```

通过文件描述符 `2` 输出。

---

## `2>&1` 与 `>&2` 的区别

这两个很容易混淆：

```bash
2>&1
```

表示：

```text
stderr → stdout 的目标
```

而：

```bash
>&2
```

等价于：

```bash
1>&2
```

表示：

```text
stdout → stderr 的目标
```

可以记成：

```text
A>&B
```

意思是：

> 让文件描述符 A 使用文件描述符 B 当前的目标。

---

# 10. 完整脚本示例

创建：

```text
capture-all.sh
```

内容：

```bash
#!/bin/bash

{
    printf 'reading %s\n' "$1"
    cat "$1"
    printf 'finished\n' >&2
} > combined.log 2>&1
```

也可以按照练习要求写成一行：

```bash
#!/bin/bash

{ printf 'reading %s\n' "$1"; cat "$1"; printf 'finished\n' >&2; } > combined.log 2>&1
```

运行：

```bash
bash capture-all.sh notes.txt
```

假设：

```text
notes.txt
```

内容为：

```text
queue drained
worker idle
```

那么：

```text
combined.log
```

会包含：

```text
reading notes.txt
queue drained
worker idle
finished
```

---

# 11. 文件不存在时会发生什么

运行：

```bash
bash capture-all.sh missing.txt
```

执行顺序仍然是：

```bash
printf
cat
printf
```

因此日志可能类似：

```text
reading missing.txt
cat: missing.txt: No such file or directory
finished
```

关键点是：

```text
cat: ...
```

虽然来自 stderr，但因为整个命令组使用了：

```bash
> combined.log 2>&1
```

所以它也进入日志，而不是留在屏幕上。

---

# 12. 参数是目录时

如果传给 `cat` 的不是普通文件，而是目录：

```bash
bash capture-all.sh my_directory
```

`cat` 也会失败，不过诊断信息与“不存在的文件”不同。

日志可能类似：

```text
reading my_directory
cat: my_directory: Is a directory
finished
```

这正是保留 stderr 的价值：

> 不要自己猜测或替换错误信息，而是记录程序实际产生的诊断信息。

不同系统或不同 `cat` 实现的具体文字可能略有不同，因此脚本应直接保存 `cat` 的 stderr。

---

# 13. 为什么要把 stdout 和 stderr 合并到同一个日志

假设程序依次执行：

```text
输出状态
发生错误
输出结束状态
```

如果 stdout 和 stderr 分开保存：

```bash
command > output.log 2> error.log
```

你会得到：

```text
output.log:
status
finished
```

和：

```text
error.log:
error message
```

单独查看两个文件时，很难判断错误究竟发生在：

```text
status
```

之后，还是：

```text
finished
```

之后。

而使用：

```bash
command > combined.log 2>&1
```

通常可以得到：

```text
status
error message
finished
```

因此日志更容易重建程序执行过程。

---

# 14. 分开保存和合并保存的区别

## 分开保存

```bash
command > output.log 2> error.log
```

结果：

```text
stdout → output.log
stderr → error.log
```

适合希望：

* 单独处理正常数据；
* 单独分析错误日志。

---

## 合并保存

```bash
command > combined.log 2>&1
```

结果：

```text
stdout ─┐
        ├──> combined.log
stderr ─┘
```

适合希望：

* 保存完整运行日志；
* 同时看到正常输出和错误；
* 尽可能保留两类输出发生的顺序。

---

# 15. `&>`：Bash 的简写

在 Bash 中：

```bash
command &> combined.log
```

通常等价于：

```bash
command > combined.log 2>&1
```

也就是同时重定向 stdout 和 stderr。

不过：

```bash
> combined.log 2>&1
```

更值得学习，因为它清楚展示了文件描述符的工作方式，而且在很多 shell 脚本中都非常常见。

---

# 16. 退出状态与重定向是两件不同的事

重定向只决定：

> 输出写到哪里。

它不会自动改变命令成功或失败的状态。

例如：

```bash
cat missing.txt > combined.log 2>&1
```

即使错误信息成功写入了日志：

```text
combined.log
```

`cat` 本身依然失败。

查看退出码：

```bash
echo $?
```

通常会得到非零值。

因此需要区分：

```text
重定向是否成功
```

和：

```text
被执行的命令是否成功
```

它们是两个不同的问题。

---

# 17. `{ ...; }` 命令组的退出状态

例如：

```bash
{
    printf 'reading %s\n' "$1"
    cat "$1"
    printf 'finished\n' >&2
}
```

如果最后一条：

```bash
printf 'finished\n' >&2
```

成功，那么整个 brace group 默认使用**最后一条命令的退出状态**。

因此即使：

```bash
cat "$1"
```

失败，后面的 `printf` 成功后，整个命令组仍然可能返回：

```text
0
```

例如：

```bash
{ cat missing.txt; printf 'finished\n'; }
echo $?
```

可能输出：

```text
0
```

这不是因为 `cat` 成功了，而是因为命令组最后执行的是成功的 `printf`。

这是 shell 脚本中非常重要的细节：

> “日志记录了错误”不代表脚本最终一定返回失败状态。

如果需要脚本在 `cat` 失败时也返回失败，需要额外设计退出状态处理。

---

# 18. 最重要的记忆模型

把：

```bash
command > combined.log 2>&1
```

理解成两个连续动作：

```bash
1>combined.log
```

然后：

```bash
2>&1
```

也就是：

```text
第 1 步：
stdout ─────> combined.log
stderr ─────> terminal

第 2 步：
stderr 复制 stdout 当前的目标

最终：
stdout ─┐
        ├────> combined.log
stderr ─┘
```

核心规则只有一句：

> **Bash 从左到右执行重定向，而 `n>&m` 表示让文件描述符 `n` 使用文件描述符 `m` 在那一刻所指向的目标。**

---

# 知识点速查

```bash
command > file
```

只保存 stdout：

```text
fd 1 → file
```

```bash
command 2> file
```

只保存 stderr：

```text
fd 2 → file
```

```bash
command > output.log 2> error.log
```

分别保存：

```text
stdout → output.log
stderr → error.log
```

```bash
command > combined.log 2>&1
```

合并保存：

```text
stdout + stderr → combined.log
```

```bash
command >&2
```

把该命令原本的 stdout 发送到 stderr：

```text
fd 1 → fd 2 当前目标
```

```bash
{ command1; command2; command3; } > combined.log 2>&1
```

对整个命令组统一应用重定向。
