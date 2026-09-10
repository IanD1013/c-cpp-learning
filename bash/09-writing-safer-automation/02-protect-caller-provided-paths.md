# Bash 中安全处理特殊文件名：引号与 `--`

## 核心问题

假设当前目录中有一个文件：

```text
-weekly report.txt
```

这个文件名同时包含两个容易导致 Bash 命令出错的特征：

1. **以 `-` 开头**
2. **包含空格**

例如，希望把它复制到：

```text
copied reports/final report.txt
```

正确的复制方式是：

```bash
cp -- "$source" "$destination"
```

这里的：

* `"$source"` / `"$destination"` 解决的是 **Shell 参数拆分问题**
* `--` 解决的是 **命令把文件名误认为选项的问题**

它们解决的是两个完全不同的问题，不能互相替代。

---

## 1. 为什么文件名以 `-` 开头会出错

很多 Unix 命令使用 `-` 表示命令行选项，例如：

```bash
cp -r source destination
```

这里：

```text
-r
```

会被 `cp` 理解为一个选项。

因此，如果直接执行：

```bash
cp "-weekly report.txt" destination
```

即使文件名加了引号，传给 `cp` 的实际参数仍然是：

```text
-weekly report.txt
```

`cp` 看到第一个字符是 `-`，仍然可能把它当作选项，而不是文件名。

### 关键点

> 引号只影响 Bash 如何构造参数，不会告诉 `cp`：“这是文件名，不是选项”。

---

## 2. `--`：停止解析命令选项

很多 Unix 命令支持：

```bash
--
```

它叫作 **option terminator（选项终止符）**。

意思是：

> 从这里开始，后面的所有参数都按普通参数处理，不再识别为选项。

例如：

```bash
cp -- "-weekly report.txt" output.txt
```

参数可以理解为：

```text
cp
--
-weekly report.txt
output.txt
```

`cp` 看到 `--` 后，就知道：

```text
-weekly report.txt
```

虽然以 `-` 开头，但它是一个文件路径。

因此，对于可能以 `-` 开头的文件名，推荐：

```bash
cp -- "$source" "$destination"
```

同样的技巧也经常用于：

```bash
rm -- "$file"
mv -- "$source" "$destination"
cat -- "$file"
```

前提是对应命令支持 `--`。

---

## 3. 为什么变量展开必须加双引号

假设：

```bash
source="-weekly report.txt"
```

如果执行：

```bash
cp -- $source destination
```

Bash 可能把：

```text
-weekly report.txt
```

拆成两个参数：

```text
-weekly
report.txt
```

于是 `cp` 实际收到的并不是一个完整文件名。

正确方式：

```bash
cp -- "$source" "$destination"
```

双引号保证：

```bash
"$source"
```

无论变量中有没有空格，都会作为 **一个参数** 传递。

例如：

```bash
destination="copied reports/final report.txt"
```

使用：

```bash
"$destination"
```

后，整个字符串仍然是一个参数。

---

## 4. `"$var"` 与 `--` 的职责完全不同

这是本课最重要的知识点。

| 防御方式     | 解决的问题              | 示例                    |
| -------- | ------------------ | --------------------- |
| `"$var"` | 防止空格等导致参数被拆分       | `"weekly report.txt"` |
| `--`     | 防止以 `-` 开头的值被识别成选项 | `cp -- ...`           |

### 只有引号，不够

```bash
cp "$source" "$destination"
```

如果：

```bash
source="-weekly report.txt"
```

虽然 Bash 会把它作为一个完整参数传入，但 `cp` 仍可能把它理解为选项。

### 只有 `--`，也不够

```bash
cp -- $source $destination
```

`--` 可以解决开头的 `-`，但是变量中的空格仍可能被 Bash 拆成多个参数。

### 正确写法

```bash
cp -- "$source" "$destination"
```

记忆：

> **引号负责保护参数边界，`--` 负责结束选项解析。**

---

# `copy-report.sh` 的正确实现

在 `bash-lab` 中创建：

```text
copy-report.sh
```

内容：

```bash
#!/usr/bin/env bash

source="$1"
destination="$2"

cp -- "$source" "$destination"
```

这里：

```bash
source="$1"
```

表示把第一个位置参数保存到变量 `source`。

```bash
destination="$2"
```

表示把第二个位置参数保存到变量 `destination`。

真正执行复制的是：

```bash
cp -- "$source" "$destination"
```

---

## 运行脚本

确保脚本可以执行：

```bash
chmod +x copy-report.sh
```

然后执行：

```bash
./copy-report.sh "-weekly report.txt" "copied reports/final report.txt"
```

也可以显式通过 Bash 执行：

```bash
bash copy-report.sh "-weekly report.txt" "copied reports/final report.txt"
```

### 为什么调用脚本时也要加引号

这一部分：

```bash
"-weekly report.txt"
```

需要保持为一个参数。

这一部分：

```bash
"copied reports/final report.txt"
```

同样包含空格，也必须保持为一个参数。

于是脚本内部得到：

```bash
$1 = -weekly report.txt
$2 = copied reports/final report.txt
```

---

# 如何通过这道练习

## 第一次复制

运行：

```bash
cd bash-lab
```

创建脚本：

```bash
cat > copy-report.sh <<'EOF'
#!/usr/bin/env bash

source="$1"
destination="$2"

cp -- "$source" "$destination"
EOF
```

执行：

```bash
bash copy-report.sh "-weekly report.txt" "copied reports/final report.txt"
```

检查退出状态：

```bash
echo $?
```

应该得到：

```text
0
```

检查文件：

```bash
ls -l "copied reports/final report.txt"
```

---

## 第二次复制

练习要求还要把同一个以 `-` 开头的源文件复制到**另一个包含空格的目标路径**。

例如：

```bash
bash copy-report.sh "-weekly report.txt" "copied reports/second report.txt"
```

再次检查：

```bash
echo $?
```

应该是：

```text
0
```

可以查看目录：

```bash
ls -l "copied reports"
```

应该至少能看到：

```text
final report.txt
second report.txt
```

---

# 常见错误

## 错误 1：变量没有引号

```bash
cp -- $source $destination
```

问题：

```text
-weekly report.txt
```

和：

```text
copied reports/final report.txt
```

都可能因为空格被拆成多个参数。

应该写：

```bash
cp -- "$source" "$destination"
```

---

## 错误 2：只有引号，没有 `--`

```bash
cp "$source" "$destination"
```

引号确实保护了空格，但如果：

```bash
source="-weekly report.txt"
```

`cp` 仍可能把它理解为命令选项。

应该写：

```bash
cp -- "$source" "$destination"
```

---

## 错误 3：在脚本里写死路径

例如：

```bash
cp -- "-weekly report.txt" "copied reports/final report.txt"
```

虽然这条命令本身可以工作，但不符合练习要求。

脚本应该使用调用者传入的两个参数：

```bash
source="$1"
destination="$2"

cp -- "$source" "$destination"
```

这样才能执行：

```bash
bash copy-report.sh "-weekly report.txt" "copied reports/final report.txt"
```

也能执行：

```bash
bash copy-report.sh "-weekly report.txt" "copied reports/another destination.txt"
```

---

# 最终应记住的模式

处理来自变量、用户输入或脚本参数的文件路径时，推荐习惯性写成：

```bash
command -- "$path"
```

涉及两个路径：

```bash
command -- "$source" "$destination"
```

例如：

```bash
cp -- "$source" "$destination"
```

它同时防御两类常见问题：

```text
"$变量" → 防止空格等破坏参数边界
--       → 防止以 - 开头的文件名被当成选项
```

对于本课，最关键的一行就是：

```bash
cp -- "$source" "$destination"
```
