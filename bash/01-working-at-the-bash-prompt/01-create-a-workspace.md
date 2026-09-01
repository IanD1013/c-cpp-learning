# Bash 基础：命令、参数、路径与 `mkdir`

## 1. Shell 命令的基本结构

在 Bash 中，一条命令通常可以理解为：

```bash
command argument1 argument2 ...
```

例如：

```bash
mkdir reports
```

这里：

* `mkdir`：**命令（command）**
* `reports`：传给命令的**参数（argument）**

Shell 会先根据空格把输入拆成多个独立的词，然后再执行命令。

因此：

```bash
mkdir reports
```

表示执行 `mkdir`，并把 `reports` 作为一个参数交给它。

> **空格非常重要：命令名和参数必须分开。**

---

## 2. `mkdir`：创建目录

`mkdir` 是 **make directory** 的缩写，用于创建目录。

基本语法：

```bash
mkdir 路径
```

例如：

```bash
mkdir reports
```

如果当前目录是：

```text
/home/learner
```

执行后会创建：

```text
/home/learner/reports
```

---

## 3. 相对路径与绝对路径

### 相对路径

不以 `/` 开头的路径通常是**相对路径**，会根据当前所在目录进行解析。

例如当前目录是：

```text
/home/learner
```

执行：

```bash
mkdir bash-lab
```

实际创建：

```text
/home/learner/bash-lab
```

---

### 绝对路径

以 `/` 开头的路径是**绝对路径**，从文件系统根目录 `/` 开始定位。

例如：

```bash
mkdir /home/learner/bash-lab
```

无论当前 Shell 位于哪里，都会尝试创建：

```text
/home/learner/bash-lab
```

### 对比

| 命令                             | 含义                                 |
| ------------------------------ | ---------------------------------- |
| `mkdir bash-lab`               | 在当前目录下创建 `bash-lab`                |
| `mkdir /home/learner/bash-lab` | 明确在 `/home/learner` 下创建 `bash-lab` |

---

## 4. 创建目录不会自动进入目录

执行：

```bash
mkdir bash-lab
```

只负责**创建目录**。

它不会把当前工作目录切换到：

```text
/home/learner/bash-lab
```

如果执行前位于：

```text
/home/learner
```

执行后仍然位于：

```text
/home/learner
```

进入目录需要另外使用：

```bash
cd bash-lab
```

---

## 5. Linux 命令成功时可能什么都不输出

执行：

```bash
mkdir bash-lab
```

成功时，`mkdir` 通常不会打印任何内容。

例如：

```text
learner@host:~$ mkdir bash-lab
learner@host:~$
```

这种“没有输出”的情况通常代表：

> **命令成功完成，没有需要报告的错误。**

不要因为没有看到 `Success` 之类的信息，就认为命令没有执行。

---

## 6. 当前练习需要执行的命令

当前 Shell 已经位于：

```text
/home/learner
```

因此可以直接执行：

```bash
mkdir bash-lab
```

它会创建：

```text
/home/learner/bash-lab
```

也可以使用完整的绝对路径：

```bash
mkdir /home/learner/bash-lab
```

两种写法在当前情况下效果相同。

---

## 7. 本节需要记住

* Shell 使用空格区分**命令**和**参数**。
* `mkdir` 用于创建目录。
* 不以 `/` 开头的是相对路径，依赖当前目录。
* 以 `/` 开头的是绝对路径，不依赖当前目录。
* `mkdir` 创建目录后**不会自动进入该目录**。
* Linux 命令成功时经常**没有任何输出**。
* 当前练习的目标命令是：

```bash
mkdir bash-lab
```
