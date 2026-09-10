# Bash `trap EXIT`：可靠清理临时目录

## 核心目标

脚本创建临时目录后，必须保证：

* 正常执行结束时删除临时目录；
* 脚本主动 `exit 2` 等非零退出时也删除；
* 删除前记录目录中的文件数量；
* 记录被删除的临时目录路径；
* 失败运行不能生成最终 `report.txt`。

关键工具是：

```bash
trap 'commands' EXIT
```

`EXIT` trap 会在 Bash 脚本退出时执行，无论脚本：

* 正常运行到结尾；
* 执行 `exit 0`；
* 执行 `exit 2` 等非零退出。

但 `kill -9`（`SIGKILL`）会立即杀死进程，Bash 没机会执行 trap，因此这种情况无法保证清理。

---

## 1. 为什么不能只在脚本最后写 `rm`

下面这种写法不可靠：

```bash
tmpdir=$(mktemp -d "$PWD/tmp.XXXXXX")

# ...执行任务...

rm -rf -- "$tmpdir"
```

如果中途发生：

```bash
exit 2
```

脚本直接退出，最后的 `rm` 根本不会执行。

因此，临时资源创建成功后，应立刻注册清理操作：

```bash
tmpdir=$(mktemp -d "$PWD/tmp.XXXXXX")
trap 'rm -rf -- "$tmpdir"' EXIT
```

### 为什么 `trap` 必须紧跟 `mktemp`

错误：

```bash
tmpdir=$(mktemp -d "$PWD/tmp.XXXXXX")

printf '%s\n' "$tmpdir" > temp-path.txt

trap 'rm -rf -- "$tmpdir"' EXIT
```

如果中间的 `printf` 因某种原因导致脚本退出，临时目录已经存在，但清理 trap 还没有注册。

更安全的原则是：

> **资源创建成功后，马上注册对应的清理操作。**

```bash
tmpdir=$(mktemp -d "$PWD/tmp.XXXXXX")
trap '...' EXIT
```

两行之间不要放其他命令。

---

## 2. `mktemp -d` 如何创建唯一临时目录

命令：

```bash
tmpdir=$(mktemp -d "$PWD/tmp.XXXXXX")
```

例如可能得到：

```text
/home/learner/bash-lab/tmp.a7KpQx
```

### 各部分含义

```bash
mktemp -d
```

`-d` 表示创建目录，而不是临时文件。

```bash
"$PWD/tmp.XXXXXX"
```

是目录名模板。

其中：

```text
XXXXXX
```

会被替换成随机字符，从而让每次运行获得不同的目录。

例如两次运行：

```text
/home/learner/bash-lab/tmp.Kjd83A
/home/learner/bash-lab/tmp.P0xLq2
```

### `$PWD` 是什么

`PWD` 是 Bash 自动维护的当前工作目录变量：

```bash
echo "$PWD"
```

通常与：

```bash
pwd
```

输出相同。

注意：

> `$PWD` 表示**运行脚本时所在的目录**，不是脚本文件本身所在的目录。

---

## 3. 为什么 trap 使用单引号

推荐：

```bash
trap 'rm -rf -- "$tmpdir"' EXIT
```

这里外层必须使用单引号。

单引号让：

```bash
$tmpdir
```

在注册 trap 时暂时不展开，而是在真正执行 trap 时再展开。

可以理解为 Bash 保存的是：

```bash
rm -rf -- "$tmpdir"
```

等退出时才执行它。

而不是提前把整个命令求值。

---

## 4. 一个 trap 可以执行多条命令

本练习退出时需要依次完成三件事：

1. 统计临时目录中的文件数量；
2. 记录即将删除的临时目录路径；
3. 删除临时目录。

因此可以写：

```bash
trap 'ls -1 "$tmpdir" | wc -l > staged-count.txt; printf "%s\n" "$tmpdir" >> removed.txt; rm -rf -- "$tmpdir"' EXIT
```

关键在于顺序。

必须：

```text
统计
↓
记录
↓
删除
```

而不是先删除：

```bash
rm -rf -- "$tmpdir"
ls -1 "$tmpdir" | wc -l
```

否则已经无法正确统计目录内容。

---

## 5. `>` 和 `>>` 的区别

### `staged-count.txt`

要求只保存最近一次运行的结果，所以使用：

```bash
> staged-count.txt
```

例如：

```bash
ls -1 "$tmpdir" | wc -l > staged-count.txt
```

`>` 会覆盖旧内容。

最终：

```text
1
```

### `removed.txt`

需要每次运行都追加一条路径，所以使用：

```bash
>> removed.txt
```

例如：

```bash
printf "%s\n" "$tmpdir" >> removed.txt
```

两次运行后可能得到：

```text
/home/learner/bash-lab/tmp.k2Js8A
/home/learner/bash-lab/tmp.z91Qwe
```

每个临时目录路径都不同。

---

## 6. 本练习需要创建的 `temp-work.sh`

进入实验目录：

```bash
cd /home/learner/bash-lab
```

创建：

```text
/home/learner/bash-lab/temp-work.sh
```

内容如下：

```bash
#!/usr/bin/env bash

tmpdir=$(mktemp -d "$PWD/tmp.XXXXXX")
trap 'ls -1 "$tmpdir" | wc -l > staged-count.txt; printf "%s\n" "$tmpdir" >> removed.txt; rm -rf -- "$tmpdir"' EXIT

printf '%s\n' "$tmpdir" > temp-path.txt

printf '%s\n' "$1" > "$tmpdir/report.txt"

if [[ "$1" == "fail" ]]; then
    exit 2
fi

cp -- "$tmpdir/report.txt" report.txt
```

给脚本执行权限：

```bash
chmod +x temp-work.sh
```

---

## 7. 脚本执行流程

假设执行：

```bash
./temp-work.sh success
```

流程是：

```text
创建临时目录
    ↓
注册 EXIT trap
    ↓
把临时目录路径写入 temp-path.txt
    ↓
在临时目录创建 report.txt
    ↓
参数不是 fail
    ↓
复制临时 report.txt 到最终 report.txt
    ↓
脚本正常结束
    ↓
EXIT trap 执行
    ↓
统计临时目录文件数
    ↓
把路径追加到 removed.txt
    ↓
删除临时目录
```

最终：

```text
report.txt
```

存在，而且内容是：

```text
success
```

---

## 8. `fail` 为什么不会留下最终报告

执行：

```bash
./temp-work.sh fail
```

脚本先创建：

```text
$tmpdir/report.txt
```

然后遇到：

```bash
if [[ "$1" == "fail" ]]; then
    exit 2
fi
```

因此下面这条：

```bash
cp -- "$tmpdir/report.txt" report.txt
```

不会执行。

但是 `exit 2` 仍然会触发：

```bash
trap '...' EXIT
```

因此临时目录依然会被删除。

这是一种非常有用的设计：

> **先在临时位置构建完整结果，只有成功时才把结果复制到正式位置。**

失败时，半成品只存在于临时目录中，随后由 trap 删除。

---

## 9. 如何运行才能通过练习

最好先清理旧结果，避免之前运行留下的 `report.txt` 或 `removed.txt` 干扰判断：

```bash
cd /home/learner/bash-lab

rm -f report.txt \
      removed.txt \
      staged-count.txt \
      temp-path.txt \
      cleanup-status.txt
```

### 第一次：正常运行

```bash
./temp-work.sh success
```

此时应该：

```bash
cat report.txt
```

得到：

```text
success
```

并且：

```bash
cat staged-count.txt
```

得到：

```text
1
```

---

### 第二次：失败运行

先删除上一轮成功生成的最终报告：

```bash
rm -f report.txt
```

然后执行：

```bash
./temp-work.sh fail
```

因为脚本故意返回状态码 `2`，Shell 中看到失败是正常现象。

检查：

```bash
test ! -e report.txt
```

应该成功，说明失败运行没有生成最终报告。

检查：

```bash
cat staged-count.txt
```

应该仍然是：

```text
1
```

因为 trap 删除目录之前，临时目录内存在一个：

```text
report.txt
```

---

## 10. 检查 `removed.txt`

执行：

```bash
cat removed.txt
```

应该有两行，例如：

```text
/home/learner/bash-lab/tmp.Qf82xA
/home/learner/bash-lab/tmp.Lm19Kd
```

要求：

* 一次运行对应一行；
* 两条路径不同；
* 都是完整路径；
* 对应目录现在都已经被删除。

原因是 `mktemp` 每次都会生成不同名称。

---

## 11. 最后生成 `cleanup-status.txt`

`temp-path.txt` 保存的是最近一次运行创建的临时目录：

```bash
cat temp-path.txt
```

将它读入变量：

```bash
tmpdir=$(cat temp-path.txt)
```

确认目录已经不存在：

```bash
test ! -e "$tmpdir"
```

如果没有输出且返回成功，就说明 trap 已清理目录。

然后创建要求的状态文件：

```bash
printf '%s\n' clean > cleanup-status.txt
```

检查：

```bash
cat cleanup-status.txt
```

必须得到：

```text
clean
```

---

## 12. 一套完整的通过流程

```bash
cd /home/learner/bash-lab

cat > temp-work.sh <<'EOF'
#!/usr/bin/env bash

tmpdir=$(mktemp -d "$PWD/tmp.XXXXXX")
trap 'ls -1 "$tmpdir" | wc -l > staged-count.txt; printf "%s\n" "$tmpdir" >> removed.txt; rm -rf -- "$tmpdir"' EXIT

printf '%s\n' "$tmpdir" > temp-path.txt

printf '%s\n' "$1" > "$tmpdir/report.txt"

if [[ "$1" == "fail" ]]; then
    exit 2
fi

cp -- "$tmpdir/report.txt" report.txt
EOF

chmod +x temp-work.sh

rm -f report.txt removed.txt staged-count.txt temp-path.txt cleanup-status.txt

./temp-work.sh success

rm -f report.txt

./temp-work.sh fail || true

tmpdir=$(cat temp-path.txt)

if [[ ! -e "$tmpdir" ]]; then
    printf '%s\n' clean > cleanup-status.txt
fi
```

然后检查：

```bash
cat removed.txt
cat staged-count.txt
cat cleanup-status.txt
test ! -e report.txt
```

预期：

```text
removed.txt
    两条不同的临时目录路径

staged-count.txt
    1

cleanup-status.txt
    clean

report.txt
    不存在
```

最后一次运行是 `fail`，因此最终 `report.txt` 不应该存在。

---

## 13. 常见错误

### 错误 1：trap 注册得太晚

```bash
tmpdir=$(mktemp -d "$PWD/tmp.XXXXXX")
printf '%s\n' "$tmpdir" > temp-path.txt
trap '...' EXIT
```

不符合要求。

应该紧挨着：

```bash
tmpdir=$(mktemp -d "$PWD/tmp.XXXXXX")
trap '...' EXIT
```

---

### 错误 2：先删除再统计

错误：

```bash
trap 'rm -rf -- "$tmpdir"; ls -1 "$tmpdir" | wc -l > staged-count.txt' EXIT
```

目录已经被删除，无法得到正确的 staged count。

正确顺序：

```bash
trap 'ls -1 "$tmpdir" | wc -l > staged-count.txt; printf "%s\n" "$tmpdir" >> removed.txt; rm -rf -- "$tmpdir"' EXIT
```

---

### 错误 3：`removed.txt` 使用 `>`

错误：

```bash
printf "%s\n" "$tmpdir" > removed.txt
```

第二次运行会覆盖第一次记录。

应该：

```bash
printf "%s\n" "$tmpdir" >> removed.txt
```

---

### 错误 4：失败之前没有创建 staged 文件

如果写成：

```bash
if [[ "$1" == "fail" ]]; then
    exit 2
fi

printf '%s\n' "$1" > "$tmpdir/report.txt"
```

那么失败运行退出时临时目录是空的：

```text
staged-count.txt = 0
```

不符合练习要求。

因此必须先：

```bash
printf '%s\n' "$1" > "$tmpdir/report.txt"
```

再判断：

```bash
if [[ "$1" == "fail" ]]; then
    exit 2
fi
```

这样无论成功还是失败，trap 执行时目录里都有一个 staged 文件，因此：

```text
staged-count.txt = 1
```

---

### 错误 5：认为 `trap EXIT` 什么退出都能处理

它可以处理正常退出和 Bash 自己执行的：

```bash
exit 2
```

但无法处理：

```bash
kill -9 PID
```

因为 `SIGKILL` 不能被捕获或处理，进程会被操作系统立即终止。

---

## 知识点总结

最重要的模式是：

```bash
resource=$(create_resource)
trap 'cleanup "$resource"' EXIT
```

在临时目录场景中：

```bash
tmpdir=$(mktemp -d "$PWD/tmp.XXXXXX")
trap 'rm -rf -- "$tmpdir"' EXIT
```

这解决了一个非常常见的问题：

> **无论脚本从正常路径还是错误路径退出，都尽可能执行统一的资源清理逻辑。**

本练习进一步展示了 trap 中多条命令的顺序：

```text
检查/统计资源
→ 记录资源
→ 删除资源
```

而不是：

```text
删除资源
→ 再尝试检查资源
```

这是 Bash 脚本中实现可靠临时资源生命周期管理的基础模式。
