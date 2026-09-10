# Bash：用 `if ! command` 正确处理命令失败

## 核心问题

Bash 默认不会因为某一条命令失败就自动停止整个脚本。

例如：

```bash
cp -- "$1" "$2"
echo "copied" > "$3"
```

如果 `cp` 失败，例如源文件不存在：

```text
cp: cannot stat 'missing.txt': No such file or directory
```

Bash 仍然会继续执行下一行：

```bash
echo "copied" > "$3"
```

结果就会产生一个严重的逻辑错误：

* 文件实际上没有复制成功；
* marker 文件却写入了 `copied`；
* 后续程序可能因此误以为复制成功。

因此，关键原则是：

> **命令失败并不等于脚本停止。必须显式检查退出状态，并在失败时主动 `exit`。**

---

## 1. 命令的退出状态

Linux/Unix 命令执行结束后都会返回一个 **exit status（退出状态码）**。

通常：

```text
0       成功
非 0    失败
```

例如：

```bash
cp source.txt copied.txt
echo "$?"
```

复制成功时：

```text
0
```

如果源文件不存在：

```bash
cp missing.txt copied.txt
echo "$?"
```

通常会得到非零状态，例如：

```text
1
```

`$?` 表示：

> **上一条命令的退出状态。**

注意必须马上读取，因为执行下一条命令后，`$?` 就会被覆盖。

---

## 2. `if` 可以直接检查命令是否成功

Bash 的 `if` 并不要求条件必须是类似：

```bash
x == 10
```

它本质上判断的是：

> **条件位置中的命令退出状态是否为 0。**

例如：

```bash
if cp -- "$1" "$2"; then
    echo "copy succeeded"
fi
```

含义是：

```text
运行 cp
   ↓
检查 cp 的退出状态
   ↓
0       → 执行 then
非 0    → 不执行 then
```

因此在 Shell 中：

```bash
if command; then
```

可以理解为：

```text
如果 command 执行成功
```

---

## 3. `!`：反转命令的成功与失败

如果希望 `then` 专门处理失败情况，可以使用：

```bash
if ! cp -- "$1" "$2"; then
    ...
fi
```

这里的：

```bash
!
```

表示对命令的退出状态进行逻辑取反。

因此：

```bash
if ! cp -- "$1" "$2"; then
```

可以直接读成：

> **如果复制没有成功，那么执行下面的失败处理。**

逻辑过程：

```text
cp 成功
exit status = 0
        ↓
! 反转
        ↓
条件为假
        ↓
跳过 then
```

而：

```text
cp 失败
exit status != 0
        ↓
! 反转
        ↓
条件为真
        ↓
进入 then
```

`!` 并不会改变 `cp` 实际执行了什么，只是改变 `if` 如何解释它的成功或失败。

---

## 4. 失败时输出错误并终止脚本

典型的保护模式是：

```bash
if ! cp -- "$1" "$2"; then
    echo "copy failed" >&2
    exit 2
fi
```

这里有两个重要动作。

### `>&2`：把错误信息发送到标准错误

```bash
echo "copy failed" >&2
```

Shell 中常见的三个标准流是：

| FD  | 名称     | 用途   |
| --- | ------ | ---- |
| `0` | stdin  | 标准输入 |
| `1` | stdout | 正常输出 |
| `2` | stderr | 错误输出 |

普通：

```bash
echo "copy failed"
```

默认写入 stdout。

而：

```bash
echo "copy failed" >&2
```

表示：

> 将这条信息写到 stderr。

错误信息使用 stderr 是良好实践，因为正常输出和错误输出可以分别处理。

例如：

```bash
./guarded-copy.sh ... >output.log 2>error.log
```

---

## 5. `exit 2` 才是真正阻止脚本继续执行的关键

失败分支中：

```bash
exit 2
```

有两个作用：

1. 立即终止整个脚本；
2. 将脚本的退出状态设置为 `2`。

例如：

```bash
if ! cp -- "$1" "$2"; then
    echo "copy failed" >&2
    exit 2
fi

echo "copied" > "$3"
```

如果 `cp` 失败：

```text
cp 失败
 ↓
进入 then
 ↓
打印 copy failed
 ↓
exit 2
 ↓
整个脚本结束
```

因此：

```bash
echo "copied" > "$3"
```

根本不会执行。

这正是 marker 文件不会被错误创建的原因。

---

## 6. 完整的 `guarded-copy.sh`

文件：

```text
/home/learner/bash-lab/guarded-copy.sh
```

内容：

```bash
#!/usr/bin/env bash

if ! cp -- "$1" "$2"; then
    echo "copy failed" >&2
    exit 2
fi

echo "copied" > "$3"
```

三个参数分别表示：

```text
$1    源文件
$2    目标文件
$3    成功 marker 文件
```

例如：

```bash
./guarded-copy.sh source.txt copied.txt success.txt
```

等价于：

```text
source.txt
    ↓ cp
copied.txt

复制成功后：

success.txt
└── copied
```

---

## 7. 为什么 `cp` 后面使用 `--`

代码中：

```bash
cp -- "$1" "$2"
```

而不是简单写：

```bash
cp "$1" "$2"
```

`--` 表示：

> **从这里开始，不再解析命令行选项。**

例如文件可能叫：

```text
-weekly.txt
```

如果直接执行：

```bash
cp -weekly.txt destination.txt
```

`cp` 可能把：

```text
-weekly.txt
```

误认为命令选项。

使用：

```bash
cp -- "-weekly.txt" destination.txt
```

则明确告诉 `cp`：

```text
-- 后面的东西全部当作路径
```

与此同时：

```bash
"$1"
"$2"
```

中的双引号负责保护包含空格等特殊字符的路径。

因此这两个保护解决的是不同问题：

```bash
cp -- "$1" "$2"
```

* `"$1"`：防止 Bash 对路径进行单词拆分；
* `--`：防止命令把以 `-` 开头的路径误认为选项。

---

## 8. 成功测试

假设：

```bash
cd /home/learner/bash-lab
```

并且 `source.txt` 已经存在。

执行：

```bash
./guarded-copy.sh \
  source.txt \
  /home/learner/bash-lab/copied.txt \
  success.txt
```

马上检查退出状态：

```bash
echo "$?"
```

应该得到：

```text
0
```

### 为什么是 `0`

脚本中没有显式执行：

```bash
exit 0
```

但最后一条命令：

```bash
echo "copied" > "$3"
```

执行成功。

脚本正常结束时，会使用最后执行命令的退出状态，因此最终状态为：

```text
0
```

此时：

```text
copied.txt
```

包含复制过来的内容，而：

```text
success.txt
```

包含：

```text
copied
```

---

## 9. 失败测试

先删除第一次成功运行产生的 marker，否则无法判断第二次运行是否创建了它：

```bash
rm -f success.txt
```

然后使用不存在的源文件：

```bash
./guarded-copy.sh \
  missing.txt \
  /home/learner/bash-lab/copied.txt \
  success.txt
```

可能看到：

```text
cp: cannot stat 'missing.txt': No such file or directory
copy failed
```

马上检查：

```bash
echo "$?"
```

应该得到：

```text
2
```

此时不会创建：

```text
success.txt
```

因为执行流程在：

```bash
exit 2
```

处已经结束。

---

## 10. 将两次退出状态写入 `guard-results.txt`

要求最终文件：

```text
/home/learner/bash-lab/guard-results.txt
```

内容为：

```text
0
2
```

一种清晰的完整操作方式：

```bash
cd /home/learner/bash-lab

./guarded-copy.sh \
  source.txt \
  /home/learner/bash-lab/copied.txt \
  success.txt
success_status=$?

rm -f success.txt

./guarded-copy.sh \
  missing.txt \
  /home/learner/bash-lab/copied.txt \
  success.txt
failure_status=$?

printf '%s\n%s\n' "$success_status" "$failure_status" > guard-results.txt
```

最终：

```bash
cat guard-results.txt
```

应该得到：

```text
0
2
```

---

## 11. 为什么要立刻保存 `$?`

错误写法：

```bash
./guarded-copy.sh source.txt copied.txt success.txt

echo "finished"

echo "$?"
```

这里的 `$?` 已经不是脚本的退出状态，而是：

```bash
echo "finished"
```

的退出状态。

正确方式：

```bash
./guarded-copy.sh source.txt copied.txt success.txt
status=$?
```

先保存：

```bash
status=$?
```

之后再执行其他命令：

```bash
echo "finished"
echo "$status"
```

仍然能够得到原脚本的退出状态。

---

## 12. 常见错误

### 错误 1：复制后无条件创建成功标记

```bash
cp -- "$1" "$2"
echo "copied" > "$3"
```

如果复制失败，marker 仍然会创建。

正确：

```bash
if ! cp -- "$1" "$2"; then
    echo "copy failed" >&2
    exit 2
fi

echo "copied" > "$3"
```

---

### 错误 2：只打印错误但不退出

```bash
if ! cp -- "$1" "$2"; then
    echo "copy failed" >&2
fi

echo "copied" > "$3"
```

虽然检测到了错误，但 Bash 离开 `if` 后仍继续运行，因此 marker 还是会创建。

必须：

```bash
exit 2
```

---

### 错误 3：把错误信息写到 stdout

```bash
echo "copy failed"
```

更规范的是：

```bash
echo "copy failed" >&2
```

这样正常输出和错误诊断可以分开。

---

### 错误 4：测试第二次运行前忘记删除旧 marker

如果第一次成功运行已经创建：

```text
success.txt
```

第二次失败即使没有创建 marker，旧文件仍然存在。

这可能让人误认为失败运行创建了 marker。

因此测试失败路径前应删除旧 marker：

```bash
rm -f success.txt
```

---

## 13. 可复用的 Bash 错误处理模式

这种结构非常常见：

```bash
if ! some_command; then
    echo "operation failed" >&2
    exit 1
fi

# 只有 some_command 成功才会执行这里
```

例如：

```bash
if ! mkdir -- "$directory"; then
    echo "failed to create directory" >&2
    exit 1
fi
```

或者：

```bash
if ! curl -o output.txt "$url"; then
    echo "download failed" >&2
    exit 1
fi
```

这种模式的核心思想是：

```text
执行关键操作
    ↓
失败？
 ┌──┴──┐
是     否
↓       ↓
报错    继续
exit
```

## 核心总结

```bash
if ! cp -- "$1" "$2"; then
    echo "copy failed" >&2
    exit 2
fi

echo "copied" > "$3"
```

需要记住四点：

1. **Bash 默认不会因为普通命令失败就停止脚本。**
2. `if` 可以直接根据命令的退出状态判断成功或失败。
3. `! command` 将判断反转，因此非常适合编写失败处理分支。
4. 真正阻止失败之后的代码继续运行的是：

```bash
exit 2
```

因此，“成功 marker”这类操作应该只放在已经确认前面的关键操作成功之后。
