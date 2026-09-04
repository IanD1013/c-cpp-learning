# Bash：使用位置参数让调用者决定复制源文件和目标文件

## 目标

编写一个通用 Bash 脚本，让运行脚本的人决定：

* 要复制哪个文件；
* 要复制到哪里。

例如：

```bash
./backup-file.sh source.txt backup/copied.txt
```

把：

```text
source.txt
```

复制成：

```text
backup/copied.txt
```

同一个脚本还应该支持：

```bash
./backup-file.sh source.txt backup/alternate.txt
```

这次则复制到：

```text
backup/alternate.txt
```

关键点是：**源路径和目标路径不能写死在脚本里，而应该从命令行参数读取。**

---

## 1. Bash 的位置参数

运行：

```bash
./backup-file.sh source.txt backup/copied.txt
```

Bash 会把脚本后面的参数保存为位置参数：

| 变量   | 内容                  |
| ---- | ------------------- |
| `$0` | `./backup-file.sh`  |
| `$1` | `source.txt`        |
| `$2` | `backup/copied.txt` |

因此脚本可以写成：

```bash
cp "$1" "$2"
```

意思就是：

```text
复制 第一个参数 到 第二个参数
```

---

## 2. 为什么不能把文件名写死

下面这种写法：

```bash
cp source.txt backup/copied.txt
```

虽然第一次运行可能正确，但目标路径已经固定。

即使运行：

```bash
./backup-file.sh source.txt backup/alternate.txt
```

脚本仍然只会执行：

```bash
cp source.txt backup/copied.txt
```

因为传入脚本的参数只有在使用 `$1`、`$2` 等变量时才会参与执行。

正确方式是：

```bash
cp -- "$1" "$2"
```

这样调用者可以自由选择路径。

---

## 3. 为什么 `$1` 和 `$2` 要加双引号

应该写：

```bash
cp -- "$1" "$2"
```

不要写：

```bash
cp -- $1 $2
```

原因是文件路径可能包含空格。

例如：

```bash
./backup-file.sh "release notes.txt" "backup/release notes.txt"
```

使用：

```bash
"$1"
```

会把：

```text
release notes.txt
```

作为**一个完整参数**传递给 `cp`。

如果使用：

```bash
$1
```

Bash 可能把它拆成：

```text
release
notes.txt
```

于是 `cp` 接收到的参数数量和含义都会发生变化。

### 通用原则

在 Bash 中，路径变量和位置参数通常应该使用：

```bash
"$variable"
"$1"
"$2"
"$@"
```

而不是裸写变量。

---

## 4. `cp -- "$1" "$2"` 中的 `--` 是什么

完整命令：

```bash
cp -- "$1" "$2"
```

其中：

```text
--
```

表示：

> 从这里开始，后面的内容全部作为普通参数处理，不再解释成命令选项。

例如有一个文件叫：

```text
-weird-file
```

如果执行：

```bash
cp -weird-file backup/file
```

`cp` 可能把：

```text
-weird-file
```

当成命令选项。

使用：

```bash
cp -- -weird-file backup/file
```

就明确告诉 `cp`：

```text
-weird-file 是文件名，不是选项
```

因此：

```bash
cp -- "$1" "$2"
```

是一种更稳健的写法。

---

## 5. 完整脚本

进入实验目录：

```bash
cd /home/learner/bash-lab
```

创建：

```text
backup-file.sh
```

内容为：

```bash
#!/usr/bin/env bash
cp -- "$1" "$2"
```

其中：

```bash
#!/usr/bin/env bash
```

是 shebang，告诉系统使用 Bash 执行这个脚本。

第二行：

```bash
cp -- "$1" "$2"
```

表示：

```text
把调用者传入的第一个路径复制到调用者传入的第二个路径
```

---

## 6. 可以直接这样创建脚本

```bash
cd /home/learner/bash-lab

cat > backup-file.sh <<'SCRIPT'
#!/usr/bin/env bash
cp -- "$1" "$2"
SCRIPT
```

然后赋予执行权限：

```bash
chmod +x backup-file.sh
```

---

## 7. 第一次运行：复制到 `copied.txt`

执行：

```bash
./backup-file.sh source.txt backup/copied.txt
```

此时：

```text
$1 = source.txt
$2 = backup/copied.txt
```

脚本实际执行的效果相当于：

```bash
cp -- source.txt backup/copied.txt
```

可以检查：

```bash
cat backup/copied.txt
```

它应该与：

```bash
cat source.txt
```

显示相同内容。

---

## 8. 第二次运行：复制到另一个目标

同一个脚本执行：

```bash
./backup-file.sh source.txt backup/alternate.txt
```

此时：

```text
$1 = source.txt
$2 = backup/alternate.txt
```

脚本不需要任何修改，就会创建：

```text
backup/alternate.txt
```

可以检查：

```bash
cat backup/alternate.txt
```

---

## 9. 最终目录效果

第一次运行后：

```text
bash-lab/
├── source.txt
├── backup-file.sh
└── backup/
    └── copied.txt
```

第二次运行后：

```text
bash-lab/
├── source.txt
├── backup-file.sh
└── backup/
    ├── copied.txt
    └── alternate.txt
```

---

## 10. 常见错误

### 错误一：把路径写死

```bash
cp source.txt backup/copied.txt
```

问题：调用者传入的 `$1` 和 `$2` 完全没有被使用。

正确：

```bash
cp -- "$1" "$2"
```

### 错误二：没有加双引号

```bash
cp -- $1 $2
```

路径包含空格时可能出错。

正确：

```bash
cp -- "$1" "$2"
```

### 错误三：把 `$0` 当成第一个参数

```bash
cp "$0" "$1"
```

这是错误的，因为 `$0` 是脚本自己的名称。

例如：

```bash
./backup-file.sh source.txt backup/copied.txt
```

对应关系是：

```text
$0 → ./backup-file.sh
$1 → source.txt
$2 → backup/copied.txt
```

### 错误四：脚本正确但无法直接运行

如果出现：

```text
Permission denied
```

可能是没有执行权限。

运行：

```bash
chmod +x backup-file.sh
```

然后：

```bash
./backup-file.sh source.txt backup/copied.txt
```

---

## 11. 最终答案

`/home/learner/bash-lab/backup-file.sh`：

```bash
#!/usr/bin/env bash
cp -- "$1" "$2"
```

创建并测试：

```bash
cd /home/learner/bash-lab

cat > backup-file.sh <<'SCRIPT'
#!/usr/bin/env bash
cp -- "$1" "$2"
SCRIPT

chmod +x backup-file.sh

./backup-file.sh source.txt backup/copied.txt
./backup-file.sh source.txt backup/alternate.txt
```

核心模式是：

```bash
command -- "$1" "$2"
```

其中 `$1`、`$2` 让**调用者决定参数值**，双引号负责**保留每个参数的完整边界**，而 `--` 可以防止以 `-` 开头的路径被误认为命令选项。
