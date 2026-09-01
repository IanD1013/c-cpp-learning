# Bash `cp`：复制文件而不删除原文件

## 1. `cp` 的基本作用

`cp`（copy）用于**复制文件或目录**。

基本语法：

```bash
cp SOURCE DESTINATION
```

其中：

* `SOURCE`：源文件，即要复制的文件。
* `DESTINATION`：目标位置，即副本要保存的位置。

例如：

```bash
cp notes.txt notes.bak
```

执行后会得到两个独立文件：

```text
notes.txt
notes.bak
```

`notes.txt` 不会被删除或修改。

## 2. `cp` 与 `mv` 的核心区别

```bash
cp source.txt target.txt
```

效果：

```text
source.txt   # 原文件仍然存在
target.txt   # 新的副本
```

而：

```bash
mv source.txt target.txt
```

通常表示**移动或重命名**，原路径下不会继续保留源文件。

因此：

| 命令   | 原文件保留 | 创建另一份文件 |
| ---- | ----- | ------- |
| `cp` | ✅     | ✅       |
| `mv` | ❌     | ✅       |

需要“两个地方同时存在同样内容”时，应使用 `cp`。

## 3. 目标路径的两种含义

`cp` 如何处理第二个参数，取决于目标路径是什么。

### 情况一：目标是一个文件路径

```bash
cp notes.txt notes.bak
```

如果 `notes.bak` 不存在，`cp` 会创建它，并复制 `notes.txt` 的内容。

也可以复制到另一个目录并同时改名：

```bash
cp inbox/today.txt archive/today.txt
```

结果：

```text
inbox/today.txt
archive/today.txt
```

两份文件都会存在。

### 情况二：目标是一个已经存在的目录

假设 `backup/` 已经存在：

```bash
cp notes.txt backup/
```

`cp` 会自动使用源文件的文件名，因此相当于：

```bash
cp notes.txt backup/notes.txt
```

结果：

```text
notes.txt
backup/notes.txt
```

## 4. 复制后两个文件彼此独立

文件被复制之后，源文件和副本是两个独立的文件。

例如：

```bash
cp notes.txt notes.bak
```

之后修改：

```text
notes.txt
```

不会自动修改：

```text
notes.bak
```

可以把 `cp` 理解为：

> 读取源文件的内容，在另一个路径创建一份副本，而不是让两个路径永久指向同一份内容。

## 5. 目标文件已经存在时要注意

如果目标文件已经存在，例如：

```bash
cp notes.txt notes.bak
```

而 `notes.bak` 已经存在，那么普通的 `cp` 通常会**直接覆盖目标文件**。

因此操作重要文件时，应先确认目标路径是否正确。

如果希望覆盖前进行确认，可以使用：

```bash
cp -i notes.txt notes.bak
```

`-i` 表示 interactive（交互式），覆盖已有文件前会询问确认。

## 6. 典型目录示例

假设目录结构为：

```text
/home/learner/bash-lab/
├── inbox/
│   └── today.txt
└── archive/
```

希望：

1. `inbox/today.txt` 保持不变；
2. 在 `archive/` 中创建内容完全相同的 `today.txt`。

可以执行：

```bash
cp /home/learner/bash-lab/inbox/today.txt /home/learner/bash-lab/archive/today.txt
```

执行后：

```text
/home/learner/bash-lab/
├── inbox/
│   └── today.txt
└── archive/
    └── today.txt
```

两个文件的内容相同，但之后彼此独立。

## 核心总结

```bash
cp SOURCE DESTINATION
```

记住三点：

1. `cp` 是**复制**，不会删除源文件。
2. 如果目标是文件路径，可以在复制的同时修改文件名。
3. 如果目标是已有目录，副本默认使用源文件原来的文件名。

---

# 问题解答：把 `today.txt` 复制到 archive 并保留原文件

## 问题

现有文件：

```text
/home/learner/bash-lab/inbox/today.txt
```

要求：

* 将它复制到：

```text
/home/learner/bash-lab/archive/today.txt
```

* 内容必须完全相同。
* 原来的 `inbox/today.txt` 必须继续存在且内容不变。

## 答案

执行：

```bash
cp /home/learner/bash-lab/inbox/today.txt /home/learner/bash-lab/archive/today.txt
```

## 执行结果

目录中会同时存在：

```text
/home/learner/bash-lab/inbox/today.txt
/home/learner/bash-lab/archive/today.txt
```

`cp` 只读取源文件并创建副本，因此：

```text
inbox/today.txt
```

不会被删除或移动。

这里不能使用 `mv`：

```bash
mv /home/learner/bash-lab/inbox/today.txt /home/learner/bash-lab/archive/today.txt
```

因为 `mv` 会把文件移动到 `archive`，导致原来的 inbox 文件不再存在。
