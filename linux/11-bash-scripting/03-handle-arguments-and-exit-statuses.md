# Bash 脚本参数校验、错误输出与退出状态

## 1. 为什么脚本应该“先检查，再执行”

任何会修改文件系统的脚本，都应该先验证输入，再执行真正的操作。

例如，一个复制脚本期望：

```text
safe-copy SOURCE DESTINATION
```

调用者必须提供：

1. 一个源文件；
2. 一个目标路径。

脚本应先确认：

* 参数数量是否正确；
* 源文件是否真实存在且是普通文件；

只有验证通过后，才创建目录并复制文件。

> 注意：`cp` 本身不会在“只有一个参数”或“源文件不存在”时成功复制并返回成功状态。它会报错并返回非零状态。脚本主动校验的价值主要在于：更早发现错误、提供更清晰的信息，并让调用者能够区分不同失败原因。

---

## 2. Bash 脚本的位置参数

Bash 会把命令行参数放入特殊变量中：

| 变量   | 含义          |
| ---- | ----------- |
| `$0` | 脚本被调用时使用的名称 |
| `$1` | 第 1 个参数     |
| `$2` | 第 2 个参数     |
| `$#` | 参数总数        |
| `$?` | 上一个命令的退出状态  |

例如：

```bash
./safe-copy source.txt backup/report.txt
```

脚本内部：

```bash
$0 = ./safe-copy
$1 = source.txt
$2 = backup/report.txt
$# = 2
```

### `$0` 为什么适合放在 usage 信息中

可以写：

```bash
printf 'Usage: %s SOURCE DESTINATION\n' "$0" >&2
```

如果用户执行：

```bash
./safe-copy
```

就会看到：

```text
Usage: ./safe-copy SOURCE DESTINATION
```

脚本不需要把自己的名称写死。

---

## 3. 检查参数数量

脚本要求恰好两个参数：

```bash
if [ "$#" -ne 2 ]; then
    printf 'Usage: %s SOURCE DESTINATION\n' "$0" >&2
    exit 1
fi
```

其中：

```bash
-ne
```

表示数字比较中的：

```text
not equal
```

即“不等于”。

因此：

```bash
[ "$#" -ne 2 ]
```

就是：

> 参数数量不等于 2。

---

## 4. 检查源文件

可以使用：

```bash
if [ ! -f "$1" ]; then
    printf 'Error: source is not a regular file: %s\n' "$1" >&2
    exit 2
fi
```

### `-f`

```bash
-f "$1"
```

表示：

> `$1` 存在，并且是普通文件（regular file）。

例如：

```bash
[ -f "/home/learner/quarterly.txt" ]
```

如果该文件存在，则条件为真。

### `!`

`!` 对条件取反：

```bash
[ ! -f "$1" ]
```

意思是：

> `$1` 不是一个现有的普通文件。

可能包括：

* 文件不存在；
* 路径是目录；
* 路径是其他非普通文件类型。

---

## 5. 为什么变量展开通常必须加双引号

应该写：

```bash
[ ! -f "$1" ]
```

而不是：

```bash
[ ! -f $1 ]
```

原因是 Bash 会先展开变量，再执行 `[`。

假设文件名是：

```text
quarterly report.txt
```

正确写法：

```bash
[ -f "$1" ]
```

展开后仍然是一个参数：

```text
quarterly report.txt
```

未引用：

```bash
[ -f $1 ]
```

则可能被拆成：

```text
quarterly
report.txt
```

导致条件表达式被错误解析。

### 通用原则

在 Bash 中处理路径和用户提供的参数时，通常应该：

```bash
"$1"
"$2"
"$variable"
```

而不是：

```bash
$1
$2
$variable
```

---

## 6. 退出状态（exit status）

Linux/Unix 命令执行结束后都会留下一个整数状态：

```text
0      成功
非 0   失败
```

例如：

```bash
cp source.txt target.txt
echo "$?"
```

成功时通常得到：

```text
0
```

失败时得到非零值。

### `$?`

`$?` 只保存**刚刚执行完成的那个命令**的状态：

```bash
cp source.txt target.txt
status=$?
```

应该立即保存，因为下一条命令又会覆盖 `$?`。

---

## 7. `exit`：决定整个脚本如何结束

可以主动指定脚本的退出状态：

```bash
exit 1
```

例如：

```bash
if [ "$#" -ne 2 ]; then
    printf 'Usage: %s SOURCE DESTINATION\n' "$0" >&2
    exit 1
fi
```

这里不仅停止脚本，还告诉调用者：

> 脚本执行失败。

不同的错误可以使用不同状态：

```text
1  参数错误
2  源文件错误
3  创建目录失败
4  复制失败
```

这些数字具体如何定义由脚本决定，关键是：

```text
成功 = 0
失败 = 非 0
```

---

## 8. stdout 与 stderr

程序主要有两个常用输出通道：

| 通道     | 用途      |
| ------ | ------- |
| stdout | 正常结果    |
| stderr | 错误、诊断信息 |

普通输出：

```bash
printf 'done\n'
```

默认进入 stdout。

错误信息：

```bash
printf 'Error: invalid source\n' >&2
```

`>&2` 表示把输出重定向到 stderr。

这样调用者可以分别处理正常结果和错误：

```bash
./safe-copy a b >output.txt 2>error.txt
```

其中：

```text
>output.txt
```

收集 stdout，而：

```text
2>error.txt
```

收集 stderr。

因此错误信息通常不应该混入正常输出。

---

## 9. 创建目标文件所在目录

如果目标是：

```text
/home/learner/copied/report.txt
```

真正需要创建的目录是：

```text
/home/learner/copied
```

可以用：

```bash
dirname -- "$2"
```

提取目录部分：

```bash
dirname -- "/home/learner/copied/report.txt"
```

得到：

```text
/home/learner/copied
```

然后：

```bash
mkdir -p -- "$(dirname -- "$2")"
```

### `mkdir -p`

`-p` 表示：

* 父目录不存在时一起创建；
* 目录已经存在时不会因为这一点而报错。

### `--`

例如：

```bash
cp -- "$1" "$2"
```

`--` 表示：

> 从这里开始，后面的内容全部作为普通参数处理，不再解释成命令选项。

这样即使文件名类似：

```text
--help
```

也不会被误认为 `cp` 的选项。

这是处理用户提供文件名时的好习惯。

---

## 10. 一个安全复制脚本的基本流程

典型流程是：

```text
检查参数数量
      ↓
检查源文件
      ↓
获得目标目录
      ↓
创建目标目录
      ↓
复制文件
      ↓
成功退出
```

核心原则是：

> Validation before action：先验证，再产生副作用。

这样脚本的行为更可预测，也更容易被其他脚本或自动化系统调用。

---

## 11. 常见误区

### 误区 1：打印错误信息就代表失败

错误：

```bash
printf 'Error\n'
```

如果随后正常结束，脚本最后仍可能返回：

```text
0
```

应该配合：

```bash
exit 1
```

---

### 误区 2：错误信息输出到 stdout

错误：

```bash
printf 'Error: file missing\n'
```

更合适：

```bash
printf 'Error: file missing\n' >&2
```

错误和正常数据应该分开。

---

### 误区 3：文件路径不需要加引号

不推荐：

```bash
cp $1 $2
```

推荐：

```bash
cp -- "$1" "$2"
```

这样才能可靠处理包含空格等特殊字符的路径。

---

### 误区 4：只检查 `cp`，不需要提前验证

单纯执行：

```bash
cp "$1" "$2"
```

在许多错误情况下确实会失败。

但脚本主动验证可以提供：

* 更明确的错误原因；
* 可控的退出状态；
* 更容易理解的使用方式；
* 在产生其他副作用之前终止执行。

---

# `safe-copy` 脚本实现

## 任务

创建：

```text
/home/learner/bin/safe-copy
```

要求：

* 接收两个参数：源文件和目标文件；
* 参数数量错误时拒绝执行；
* 源文件不是现有普通文件时拒绝执行；
* 错误信息写入 stderr；
* 不同错误使用非零退出状态；
* 自动创建目标文件所在目录；
* 正常情况下复制文件；
* 文件权限为 `755`；
* 最后使用它把：

```text
/home/learner/quarterly.txt
```

复制到：

```text
/home/learner/copied/report.txt
```

---

## 1. 创建脚本

```bash
mkdir -p /home/learner/bin

cat > /home/learner/bin/safe-copy <<'EOF'
#!/bin/bash

if [ "$#" -ne 2 ]; then
    printf 'Usage: %s SOURCE DESTINATION\n' "$0" >&2
    exit 1
fi

if [ ! -f "$1" ]; then
    printf 'Error: source is not a regular file: %s\n' "$1" >&2
    exit 2
fi

destination_dir=$(dirname -- "$2")

if ! mkdir -p -- "$destination_dir"; then
    printf 'Error: could not create destination directory: %s\n' "$destination_dir" >&2
    exit 3
fi

if ! cp -- "$1" "$2"; then
    printf 'Error: could not copy %s to %s\n' "$1" "$2" >&2
    exit 4
fi

exit 0
EOF
```

---

## 2. 设置权限

```bash
chmod 755 /home/learner/bin/safe-copy
```

如果脚本是以 `learner` 用户创建的，它通常已经由该用户拥有。

可以检查：

```bash
ls -l /home/learner/bin/safe-copy
```

权限应类似：

```text
-rwxr-xr-x
```

即：

```text
755
```

---

## 3. 执行复制

```bash
/home/learner/bin/safe-copy \
    /home/learner/quarterly.txt \
    /home/learner/copied/report.txt
```

脚本会先创建：

```text
/home/learner/copied
```

然后复制：

```text
/home/learner/quarterly.txt
```

到：

```text
/home/learner/copied/report.txt
```

---

## 4. 验证结果

检查目标文件：

```bash
ls -l /home/learner/copied/report.txt
```

也可以比较两个文件：

```bash
cmp /home/learner/quarterly.txt /home/learner/copied/report.txt
```

如果内容完全相同，`cmp` 不会输出任何内容，并返回：

```text
0
```

可以确认：

```bash
echo "$?"
```

---

## 5. 测试错误处理

### 参数不足

```bash
/home/learner/bin/safe-copy
echo "$?"
```

应显示 usage，并返回：

```text
1
```

### 源文件不存在

```bash
/home/learner/bin/safe-copy /does/not/exist /tmp/result.txt
echo "$?"
```

应在 stderr 输出错误，并返回：

```text
2
```

### 正常复制

```bash
/home/learner/bin/safe-copy \
    /home/learner/quarterly.txt \
    /home/learner/copied/report.txt

echo "$?"
```

成功时：

```text
0
```

核心模式可以概括为：

```bash
验证输入
→ 失败时 stderr + 非零 exit
→ 验证通过后再执行真正操作
→ 成功返回 0
```
