# Bash 中使用 `mv` 移动与重命名文件

## `mv` 的作用

`mv`（move）用于：

* 移动文件或目录；
* 重命名文件或目录。

基本语法：

```bash
mv SOURCE DESTINATION
```

其中：

* `SOURCE`：文件或目录当前所在的路径；
* `DESTINATION`：操作完成后希望它所在的路径或名称。

例如：

```bash
mv old.txt new.txt
```

执行后：

```text
old.txt      → 不再存在
new.txt      → 存在，内容与原 old.txt 相同
```

这里并不是创建了一份新文件，而是把原来的文件改成了新的名字。

---

## `mv` 和 `cp` 的核心区别

`cp` 和 `mv` 都接受“源路径 + 目标路径”，但结果完全不同。

### `cp`：复制

```bash
cp today.txt commands.txt
```

结果是：

```text
today.txt
commands.txt
```

两个文件都会存在。

如果以后修改其中一个，另一个不会自动同步。

### `mv`：移动或重命名

```bash
mv today.txt commands.txt
```

结果是：

```text
commands.txt
```

原来的 `today.txt` 不再存在。

因此，如果目标是**更改文件名，而不是保留两个副本**，应该使用 `mv`。

---

## 为什么重命名应该使用 `mv`

假设目录中存在：

```text
archive/
└── today.txt
```

实际上这个文件保存的是命令相关内容，因此希望改成：

```text
archive/
└── commands.txt
```

如果使用：

```bash
cp archive/today.txt archive/commands.txt
```

会得到：

```text
archive/
├── today.txt
└── commands.txt
```

旧名字依然存在，造成两个内容相同但名字不同的文件。

正确方式是：

```bash
mv archive/today.txt archive/commands.txt
```

结果：

```text
archive/
└── commands.txt
```

文件内容保持不变，只是访问它的路径变了。

---

## 移动和重命名本质上是同一个操作

`mv` 是否表现为“重命名”还是“移动”，取决于源路径和目标路径。

### 同一目录：重命名

```bash
mv archive/today.txt archive/commands.txt
```

文件仍然位于 `archive` 中，只是名字改变：

```text
today.txt → commands.txt
```

### 不同目录：移动

```bash
mv inbox/today.txt archive/today.txt
```

文件名不变，但位置发生变化：

```text
inbox/today.txt → archive/today.txt
```

### 同时移动并重命名

```bash
mv inbox/today.txt archive/commands.txt
```

位置和名字同时改变：

```text
inbox/today.txt → archive/commands.txt
```

因此可以把 `mv` 理解为：

> 把一个文件从“当前路径”变成“目标路径”。

路径改变可以只涉及文件名，也可以涉及目录，或者两者同时改变。

---

## `mv` 只操作明确指定的路径

假设目录结构是：

```text
bash-lab/
├── inbox/
│   └── today.txt
└── archive/
    └── today.txt
```

虽然两个文件都叫 `today.txt`，但它们是两个完全独立的路径：

```text
inbox/today.txt
archive/today.txt
```

执行：

```bash
mv archive/today.txt archive/commands.txt
```

只会修改：

```text
archive/today.txt
```

不会影响：

```text
inbox/today.txt
```

操作后：

```text
bash-lab/
├── inbox/
│   └── today.txt
└── archive/
    └── commands.txt
```

文件名相同并不意味着两个文件之间存在任何关联。Shell 和文件系统主要根据**完整路径**区分它们。

---

## 相对路径与绝对路径

如果当前工作目录是：

```text
/home/learner/bash-lab
```

可以使用相对路径：

```bash
mv archive/today.txt archive/commands.txt
```

也可以使用绝对路径：

```bash
mv /home/learner/bash-lab/archive/today.txt \
   /home/learner/bash-lab/archive/commands.txt
```

两者效果相同。

相对路径通常更简洁，但它依赖当前工作目录。

可以先使用：

```bash
pwd
```

确认当前位置。

---

## 目标是目录时的行为

如果目标路径已经是一个目录：

```bash
mv today.txt archive/
```

那么文件会被移动进去，并保留原文件名：

```text
archive/today.txt
```

这和：

```bash
mv today.txt archive/today.txt
```

效果基本相同。

因此要注意：

```bash
mv SOURCE DIRECTORY
```

和：

```bash
mv SOURCE NEW_FILENAME
```

虽然语法形式相似，但目标实际代表什么，取决于该路径当前是否为目录。

---

## 覆盖已有文件的风险

如果目标文件已经存在，例如：

```text
archive/
├── today.txt
└── commands.txt
```

执行：

```bash
mv archive/today.txt archive/commands.txt
```

在常见 Unix/Linux 环境中，原来的 `commands.txt` 可能会被覆盖。

重要文件操作前可以先检查：

```bash
ls archive
```

也可以使用交互模式：

```bash
mv -i archive/today.txt archive/commands.txt
```

如果目标已经存在，`mv` 会询问是否覆盖。

---

## 常见误区

### 误区 1：用 `cp` 实现重命名

错误思路：

```bash
cp old.txt new.txt
```

这不是重命名，而是创建第二份文件。

真正的重命名：

```bash
mv old.txt new.txt
```

---

### 误区 2：认为同名文件会一起变化

下面两个文件：

```text
inbox/today.txt
archive/today.txt
```

只是恰好拥有相同的基本文件名 `today.txt`。

执行：

```bash
mv archive/today.txt archive/commands.txt
```

不会自动影响 `inbox/today.txt`。

---

### 误区 3：认为 `mv` 只能移动，不能改名

实际上：

```bash
mv old.txt new.txt
```

就是标准的文件重命名方式。

Unix/Linux 通常没有单独的 `rename` 基础命令来完成普通单文件重命名；最常用的工具就是 `mv`。

---

## 核心记忆

```bash
cp SOURCE DESTINATION
```

表示：

> 复制一份，源文件继续存在。

```bash
mv SOURCE DESTINATION
```

表示：

> 让原来的文件出现在新的路径下，旧路径不再指向它。

因此：

* 要保留原文件并创建副本：使用 `cp`；
* 要移动文件：使用 `mv`；
* 要重命名文件：使用 `mv`；
* 不想保留旧文件名：不要用 `cp` 代替 `mv`。

---

# Bash `mv` 练习题解答：只重命名 archive 中的文件

## 问题

目录 `/home/learner/bash-lab` 中存在两个同名文件：

```text
/home/learner/bash-lab/
├── inbox/
│   └── today.txt
└── archive/
    └── today.txt
```

要求：

1. 将归档目录中的文件重命名为：

   ```text
   /home/learner/bash-lab/archive/commands.txt
   ```

2. 原来的：

   ```text
   /home/learner/bash-lab/archive/today.txt
   ```

   不再存在。

3. 保持：

   ```text
   /home/learner/bash-lab/inbox/today.txt
   ```

   完全不变。

---

## 正确命令

如果当前工作目录是：

```text
/home/learner/bash-lab
```

执行：

```bash
mv archive/today.txt archive/commands.txt
```

也可以使用绝对路径：

```bash
mv /home/learner/bash-lab/archive/today.txt /home/learner/bash-lab/archive/commands.txt
```

---

## 为什么这样做

命令：

```bash
mv archive/today.txt archive/commands.txt
```

表示：

```text
SOURCE
archive/today.txt

DESTINATION
archive/commands.txt
```

源路径和目标路径位于同一个目录，因此实际上执行的是重命名：

```text
archive/today.txt
        ↓
archive/commands.txt
```

文件内容不会因为这次重命名而改变。

---

## 执行后的目录结构

操作前：

```text
bash-lab/
├── inbox/
│   └── today.txt
└── archive/
    └── today.txt
```

操作后：

```text
bash-lab/
├── inbox/
│   └── today.txt
└── archive/
    └── commands.txt
```

因此同时满足：

```text
archive/commands.txt      ✓ 存在
archive/today.txt         ✓ 不再存在
inbox/today.txt           ✓ 保持不变
```

---

## 不应该使用 `cp`

下面的命令不符合要求：

```bash
cp archive/today.txt archive/commands.txt
```

因为它会留下两个文件：

```text
archive/
├── today.txt
└── commands.txt
```

题目要求旧的归档文件名消失，因此应该使用：

```bash
mv archive/today.txt archive/commands.txt
```
