# Bash 相对路径、`..` 与 `mkdir -p`

## 场景

当前工作目录：

```text
/home/learner/bash-lab/inbox
```

需要创建：

```text
/home/learner/bash-lab/archive/daily
```

目录关系可以理解为：

```text
/home/learner/bash-lab/
├── inbox/          ← 当前所在位置
└── archive/
    └── daily/      ← 需要创建
```

因为 `archive` 和 `inbox` 都位于 `bash-lab` 下，所以可以使用**相对路径**完成操作，而不必写完整的绝对路径。

---

## 1. 绝对路径与相对路径

### 绝对路径

以 `/` 开头，从文件系统根目录开始描述位置：

```bash
/home/learner/bash-lab/archive/daily
```

优点是位置明确，但通常较长，而且如果整个项目目录被移动，路径可能失效。

### 相对路径

不以 `/` 开头，Bash 会基于**当前工作目录（current working directory）**解释它。

例如当前位于：

```text
/home/learner/bash-lab/inbox
```

那么：

```text
notes.txt
```

实际表示：

```text
/home/learner/bash-lab/inbox/notes.txt
```

因此，相对路径的含义取决于当前所在目录。

可以使用：

```bash
pwd
```

查看当前工作目录。

---

## 2. `.` 和 `..` 的含义

Bash 路径中有两个特殊目录：

| 写法   | 含义       |
| ---- | -------- |
| `.`  | 当前目录     |
| `..` | 当前目录的父目录 |

假设当前目录是：

```text
/home/learner/bash-lab/inbox
```

那么：

```text
.
```

表示：

```text
/home/learner/bash-lab/inbox
```

而：

```text
..
```

表示：

```text
/home/learner/bash-lab
```

因此：

```text
../archive
```

表示先从 `inbox` 回到父目录 `bash-lab`，然后进入它下面的 `archive`：

```text
/home/learner/bash-lab/archive
```

进一步：

```text
../archive/daily
```

就是：

```text
/home/learner/bash-lab/archive/daily
```

> `.` 是当前目录，`..` 是父目录。两个点并不是“当前目录的另一种写法”。

---

## 3. 如何理解 `../archive/daily`

从：

```text
/home/learner/bash-lab/inbox
```

开始解析：

```text
../archive/daily
```

过程是：

```text
当前位置
/home/learner/bash-lab/inbox

..  → 回到父目录
/home/learner/bash-lab

archive → 进入 archive
/home/learner/bash-lab/archive

daily → 进入 daily
/home/learner/bash-lab/archive/daily
```

所以它与下面的绝对路径指向同一个位置：

```text
/home/learner/bash-lab/archive/daily
```

---

## 4. `mkdir` 为什么有时不能一次创建多层目录

`mkdir` 用来创建目录：

```bash
mkdir directory_name
```

例如：

```bash
mkdir archive
```

会创建一个 `archive` 目录。

但如果执行：

```bash
mkdir ../archive/daily
```

而 `../archive` 还不存在，命令会失败。

原因是普通 `mkdir` 默认只负责创建**最后一级目录**，不会自动补齐缺失的父目录。

也就是说，它无法直接做到：

```text
archive 不存在
↓
创建 archive
↓
再创建 daily
```

---

## 5. `mkdir -p`：自动创建缺失的父目录

`-p` 表示创建所需的父目录：

```bash
mkdir -p one/two/three
```

如果这些目录都不存在，它会依次创建：

```text
one/
└── two/
    └── three/
```

因此，本例可以直接运行：

```bash
mkdir -p ../archive/daily
```

这里：

* `..`：回到 `bash-lab`
* `archive`：创建 `archive`
* `daily`：在其中创建 `daily`
* `-p`：保证缺失的中间目录也会一起创建

最终得到：

```text
/home/learner/bash-lab/
├── inbox/
└── archive/
    └── daily/
```

---

## 6. `mkdir -p` 的另一个重要特性

如果父目录已经存在：

```bash
mkdir -p ../archive/daily
```

通常不会因为目录已经存在而报错。

因此它很适合脚本和自动化操作，例如：

```bash
mkdir -p logs/archive/2026
```

无论 `logs` 或 `archive` 是否已经存在，都可以安全执行。

---

## 7. 为什么优先使用相对路径

下面两条命令可以达到相同结果：

```bash
mkdir -p /home/learner/bash-lab/archive/daily
```

```bash
mkdir -p ../archive/daily
```

如果当前已经位于 `inbox`，第二种通常更合适。

原因包括：

* 更短；
* 清晰体现目录之间的关系；
* 不依赖项目存放在固定的绝对位置。

例如以后整个目录被移动到：

```text
/tmp/bash-lab/
```

只要仍然从：

```text
/tmp/bash-lab/inbox
```

执行：

```bash
mkdir -p ../archive/daily
```

命令仍然有效。

---

## 8. 本例应执行的命令

可先确认当前位置：

```bash
pwd
```

如果当前确实是：

```text
/home/learner/bash-lab/inbox
```

则直接执行：

```bash
mkdir -p ../archive/daily
```

不需要先：

```bash
cd ..
```

因为 `..` 已经可以直接在路径中表示父目录。

---

## 核心总结

```bash
mkdir -p ../archive/daily
```

结合了两个关键知识点：

1. `..` 表示父目录，因此可以从 `inbox` 定位到旁边的 `archive`；
2. `mkdir -p` 可以自动创建缺失的中间目录。

记住：

```text
相对路径 → 从当前工作目录开始解析
.        → 当前目录
..       → 父目录
mkdir    → 创建目录
mkdir -p → 连同缺失的父目录一起创建
```
