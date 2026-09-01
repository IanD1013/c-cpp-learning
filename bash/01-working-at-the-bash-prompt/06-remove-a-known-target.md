# Bash `rm`：安全删除文件

## 1. `rm` 的作用

`rm`（remove）用于删除文件：

```bash
rm 文件路径
```

例如：

```bash
rm notes.txt
```

会删除当前工作目录中的 `notes.txt`。

与图形界面的“删除到回收站”不同，命令行中的普通 `rm`：

* 不会把文件移动到回收站；
* 默认不会保留副本；
* 通常不会询问是否确认；
* 删除后没有 `rm` 自带的撤销功能。

因此，使用 `rm` 时最重要的原则是：

> **尽量精确地指定要删除的文件，而不是依赖范围过大的通配符。**

---

## 2. 为什么删除时要谨慎使用通配符

假设目录中有：

```text
archive/
├── commands.txt
├── session.tmp
└── discard.tmp
```

如果只想删除：

```text
discard.tmp
```

不应该执行：

```bash
rm archive/*.tmp
```

因为 `*.tmp` 不是一个具体文件名，而是 Bash 的 **glob（通配符模式）**。

在执行 `rm` 之前，Shell 会先展开：

```bash
archive/*.tmp
```

如果当前匹配到：

```text
archive/session.tmp
archive/discard.tmp
```

那么实际执行效果相当于：

```bash
rm archive/session.tmp archive/discard.tmp
```

于是两个 `.tmp` 文件都会被删除。

更危险的写法是：

```bash
rm archive/*
```

它可能匹配目录中的所有普通可匹配条目。

### 核心原因

文件扩展名只是命名约定：

```text
.tmp
.log
.txt
```

并不能保证某类扩展名的所有文件都可以删除。

例如：

```text
session.tmp
```

虽然以 `.tmp` 结尾，却可能保存正在进行的工作。

因此，当明确只需要删除一个文件时，最安全、最清晰的做法通常是直接写出该文件的准确路径：

```bash
rm /home/learner/bash-lab/archive/discard.tmp
```

---

## 3. `rm -- 文件路径` 的意义

一种更稳妥的写法是：

```bash
rm -- /path/to/file
```

例如：

```bash
rm -- /home/learner/bash-lab/archive/discard.tmp
```

这里的：

```text
--
```

表示：

> **命令选项到这里结束，后面的内容全部作为普通参数处理。**

这主要用于防止以 `-` 开头的文件名被误认为命令选项。

例如目录中存在一个文件：

```text
-report.txt
```

直接执行：

```bash
rm -report.txt
```

可能让 `rm` 尝试把其中的字符解释为选项。

使用：

```bash
rm -- -report.txt
```

则明确表示：

```text
-report.txt
```

是文件名，而不是选项。

如果文件使用绝对路径：

```bash
/home/learner/file.txt
```

因为它以 `/` 开头，本身通常不会被误认为选项，所以 `--` 并不是必须的：

```bash
rm /home/learner/file.txt
```

同样可以正常工作。

不过养成使用：

```bash
rm -- 文件路径
```

的习惯，可以让处理特殊文件名时更加安全。

---

## 4. 删除后如何验证

删除操作完成后，可以使用：

```bash
ls 目录路径
```

查看目录当前包含哪些条目。

例如：

```bash
ls /home/learner/bash-lab/archive
```

如果删除前目录包含：

```text
commands.txt
discard.tmp
session.tmp
```

只删除 `discard.tmp` 后，应该看到：

```text
commands.txt
session.tmp
```

这是一种简单但重要的操作习惯：

```text
明确目标 → 执行删除 → 检查结果
```

---

## 5. 安全删除的基本原则

### 原则一：只删除一个文件时，直接写文件名

推荐：

```bash
rm -- /path/to/discard.tmp
```

而不是：

```bash
rm *.tmp
```

除非你确实确认所有匹配文件都应该删除。

### 原则二：执行前理解 glob 会匹配什么

例如：

```bash
*.tmp
```

意味着：

> 当前目录中所有名称符合 `*.tmp` 的文件。

它并不意味着：

> 所有“不重要的临时文件”。

Shell 只根据文件名匹配，不理解文件的业务含义。

### 原则三：删除后检查目录

例如：

```bash
ls /path/to/directory
```

确认：

* 应删除的文件已经消失；
* 应保留的文件仍然存在。

---

## 6. 常见误区

### 误区：`.tmp` 文件都可以删除

错误。

`.tmp` 只是文件名的一部分。程序完全可能正在使用某个 `.tmp` 文件。

### 误区：`rm *.tmp` 只会删除自己心里想到的那个文件

错误。

Shell 根据目录当前实际存在的文件展开通配符。

今天：

```bash
*.tmp
```

可能匹配一个文件；明天可能匹配十个文件。

### 误区：`rm` 会像桌面系统一样把文件放进回收站

通常错误。

标准 `rm` 直接删除目录项，不提供普通意义上的“撤销”或“回收站恢复”机制。

---

## 7. 核心总结

安全使用 `rm` 的核心不是记住更多参数，而是**控制删除范围**：

```bash
rm -- 精确文件路径
```

当只需要删除一个已知文件时，优先使用准确路径，不要为了省几个字符而扩大匹配范围。

特别要记住：

> **Shell 的 glob 只理解文件名模式，不理解哪些文件“应该被删除”。**
