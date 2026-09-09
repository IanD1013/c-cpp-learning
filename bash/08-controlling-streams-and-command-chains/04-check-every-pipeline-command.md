# Bash 管道退出状态：`$?` 与 `PIPESTATUS`

## 1. 问题核心：`$?` 只能看到管道最后一个命令

在 Bash 中，管道：

```bash
command1 | command2 | command3
```

实际上运行了三个命令，每个命令都有自己的退出状态（exit status）。

例如：

```bash
cat words.txt | grep -F z | wc -l
```

三个阶段分别是：

```text
cat words.txt
     │
     ▼
grep -F z
     │
     ▼
wc -l
```

但管道执行完成后：

```bash
echo "$?"
```

默认只得到**最后一个命令 `wc -l` 的退出状态**。

---

## 2. 为什么 `wc -l` 会掩盖前面的失败

`wc -l` 的任务只是：

> 读取标准输入，计算其中有多少个换行符，然后输出数量。

即使输入为空：

```bash
printf '' | wc -l
```

仍然可以正常完成：

```text
0
```

因此：

```bash
wc -l
```

通常返回：

```text
0
```

表示它自己执行成功。

### 示例：文件中没有 `z`

假设：

```text
words.txt
```

内容为：

```text
apple
banana
hello
```

执行：

```bash
cat words.txt | grep -F z | wc -l
```

结果：

```text
0
```

三个命令的退出状态实际是：

| 命令              |  状态 | 原因        |
| --------------- | --: | --------- |
| `cat words.txt` | `0` | 文件读取成功    |
| `grep -F z`     | `1` | 没有找到匹配内容  |
| `wc -l`         | `0` | 成功统计了 0 行 |

但是：

```bash
echo "$?"
```

得到的是：

```text
0
```

因为 `$?` 只保存最后一个命令 `wc -l` 的状态。

---

## 3. `grep` 的退出状态需要特别理解

`grep` 的退出状态并不是简单的“0 成功，非 0 失败”。

通常：

|   状态 | 含义     |
| ---: | ------ |
|  `0` | 找到了匹配  |
|  `1` | 没有找到匹配 |
| `>1` | 真正发生错误 |

例如：

```bash
printf 'apple\nbanana\n' | grep -F z
```

没有输出，而且：

```bash
echo "$?"
```

得到：

```text
1
```

这并不表示 `grep` 崩溃了，只表示：

> 没有任何输入行包含字面字符串 `z`。

其中：

```bash
-F
```

表示按**固定字符串（fixed string）**匹配，而不是把 `z` 当正则表达式解析。

---

## 4. 文件不存在时为什么 `$?` 仍可能是 0

例如：

```bash
cat does-not-exist.txt | grep -F z | wc -l
```

`cat` 会报错：

```text
cat: does-not-exist.txt: No such file or directory
```

但管道仍会继续：

1. `cat` 打开文件失败，返回 `1`
2. `grep` 收到空输入，没有找到 `z`，返回 `1`
3. `wc -l` 收到空输入，成功输出 `0`，返回 `0`

因此各阶段大致为：

```text
cat     grep    wc
 1       1      0
```

但：

```bash
echo "$?"
```

仍然只能看到：

```text
0
```

所以只检查 `$?` 会错误地认为整个处理过程没有问题。

---

## 5. `PIPESTATUS`：获取管道每个命令的状态

Bash 提供了特殊数组：

```bash
PIPESTATUS
```

它保存**最近一个前台管道中每个命令的退出状态**。

例如：

```bash
cat words.txt | grep -F z | wc -l
```

随后：

```bash
printf '%s\n' "${PIPESTATUS[@]}"
```

可能得到：

```text
0
1
0
```

元素顺序与管道从左到右完全对应：

```text
cat words.txt | grep -F z | wc -l
     0             1          0
```

因此：

```bash
PIPESTATUS[0]
```

对应：

```bash
cat words.txt
```

```bash
PIPESTATUS[1]
```

对应：

```bash
grep -F z
```

```bash
PIPESTATUS[2]
```

对应：

```bash
wc -l
```

---

## 6. 最重要的规则：必须立即复制 `PIPESTATUS`

`PIPESTATUS` 只描述**最近执行的管道**。

之后再运行其他命令，它就会被更新。

因此这种写法有问题：

```bash
cat words.txt | grep -F z | wc -l

echo "pipeline finished"

codes=("${PIPESTATUS[@]}")
```

这里的：

```bash
echo "pipeline finished"
```

本身也是一个新的命令，会修改 `PIPESTATUS`。

所以真正安全的写法是：

```bash
cat words.txt | grep -F z | wc -l
codes=("${PIPESTATUS[@]}")
```

也就是说：

> 管道执行后的第一条命令必须是保存 `PIPESTATUS`。

---

## 7. 为什么这样复制数组

正确写法：

```bash
codes=("${PIPESTATUS[@]}")
```

可以分成三部分理解。

### `PIPESTATUS[@]`

表示数组中的所有元素：

```bash
"${PIPESTATUS[@]}"
```

会展开成类似：

```text
"0" "1" "0"
```

每个数组元素保持为独立参数。

### `(...)`

在 Bash 中：

```bash
codes=(...)
```

表示创建数组。

因此：

```bash
codes=("${PIPESTATUS[@]}")
```

相当于把：

```text
PIPESTATUS = [0, 1, 0]
```

复制成：

```text
codes = [0, 1, 0]
```

之后即使 `PIPESTATUS` 被其他命令更新：

```bash
echo hello
```

`codes` 中保存的数据仍然不会改变。

---

## 8. 为什么推荐使用 `"${array[@]}"`

处理 Bash 数组时，通常应该使用：

```bash
"${array[@]}"
```

例如：

```bash
printf '%s\n' "${codes[@]}"
```

如果：

```bash
codes=(0 1 0)
```

它相当于：

```bash
printf '%s\n' "0" "1" "0"
```

输出：

```text
0
1
0
```

这是“一元素一个参数”的展开方式，通常是处理 Bash 数组最安全的形式。

---

## 9. 管道中的输出重定向

如果希望：

```bash
wc -l
```

产生的数字写入文件：

```bash
count.txt
```

可以写：

```bash
cat "$1" | grep -F z | wc -l > count.txt
```

这里的：

```bash
> count.txt
```

只重定向最后一个命令 `wc -l` 的标准输出。

数据流为：

```text
文件
 │
 ▼
cat
 │
 ▼
grep -F z
 │
 ▼
wc -l
 │
 ▼
count.txt
```

例如没有找到 `z`：

```text
count.txt
```

中就是：

```text
0
```

需要注意：

```bash
> count.txt
```

不是一个额外的管道命令，因此：

```bash
PIPESTATUS
```

仍然有三个元素。

---

## 10. `$1`：脚本的第一个位置参数

如果运行：

```bash
bash pipeline-status.sh words.txt
```

那么脚本内部：

```bash
$1
```

就是：

```text
words.txt
```

所以：

```bash
cat "$1"
```

实际等价于：

```bash
cat words.txt
```

通常应写成：

```bash
"$1"
```

而不是：

```bash
$1
```

因为引号可以正确处理包含空格的文件名，例如：

```bash
bash pipeline-status.sh "my words.txt"
```

---

## 11. 三种典型情况

### 情况一：文件存在，但没有 `z`

输入：

```text
apple
banana
hello
```

管道：

```bash
cat "$1" | grep -F z | wc -l
```

结果：

```text
count.txt
0
```

状态：

```text
pipeline-statuses.txt
0
1
0
```

含义：

```text
cat   = 成功
grep  = 没找到
wc    = 成功
```

---

### 情况二：文件存在，并且含有 `z`

例如：

```text
apple
zebra
banana
```

结果：

```text
count.txt
1
```

状态：

```text
pipeline-statuses.txt
0
0
0
```

因为三个阶段都成功，并且 `grep` 找到了匹配。

---

### 情况三：文件不存在

例如：

```bash
bash pipeline-status.sh missing.txt
```

状态：

```text
pipeline-statuses.txt
1
1
0
```

含义：

```text
cat   = 无法读取文件
grep  = 收到空输入，没有匹配
wc    = 成功统计 0 行
```

而：

```text
count.txt
```

仍然会包含：

```text
0
```

这正说明：

> “最终输出是 0”与“整个管道成功”是两个不同的问题。

---

## 12. `$?`、`PIPESTATUS` 和 `pipefail` 的区别

### `$?`

默认情况下：

```bash
a | b | c
echo "$?"
```

只得到：

```text
c
```

的状态。

适合：

> 只关心整个管道最后一个命令是否成功。

---

### `PIPESTATUS`

```bash
a | b | c
codes=("${PIPESTATUS[@]}")
```

可以分别得到：

```text
a b c
```

三个命令的状态。

适合：

> 需要知道具体哪一个阶段成功或失败。

---

### `set -o pipefail`

Bash 还支持：

```bash
set -o pipefail
```

开启后，如果管道中某个命令失败，整个管道不会再简单采用最后一个命令的成功状态。

例如：

```bash
set -o pipefail

cat missing.txt | grep -F z | wc -l
echo "$?"
```

此时 `$?` 通常会成为非零值。

但 `pipefail` 仍然不能告诉你：

> 到底是哪几个阶段分别返回了什么状态。

如果需要：

```text
1
1
0
```

这种完整信息，仍然应该使用：

```bash
PIPESTATUS
```

---

# 练习解答：创建 `pipeline-status.sh`

## 问题

创建 `/home/learner/bash-lab/pipeline-status.sh`，要求：

* 第一个命令使用 `cat` 读取脚本的第一个参数；
* 第二个命令使用 `grep -F z`；
* 第三个命令使用 `wc -l`；
* `wc -l` 的输出写入 `/home/learner/bash-lab/count.txt`；
* 管道结束后立即复制 `PIPESTATUS`；
* 三个退出状态逐行写入 `/home/learner/bash-lab/pipeline-statuses.txt`；
* 脚本最终退出状态为 `0`；
* 对不同输入应得到：

  * 没有 `z`：`0, 1, 0`
  * 有 `z`：`0, 0, 0`
  * 文件不存在：`1, 1, 0`

## 完整脚本

创建文件：

```bash
cd /home/learner/bash-lab

cat > pipeline-status.sh <<'SCRIPT'
#!/usr/bin/env bash

cd /home/learner/bash-lab || exit 1

cat "$1" | grep -F z | wc -l > count.txt
codes=("${PIPESTATUS[@]}")

printf '%s\n' "${codes[@]}" > pipeline-statuses.txt

exit 0
SCRIPT
```

然后运行：

```bash
bash pipeline-status.sh words.txt
```

## 最关键的两行

```bash
cat "$1" | grep -F z | wc -l > count.txt
codes=("${PIPESTATUS[@]}")
```

这两行必须紧挨着。

不能写成：

```bash
cat "$1" | grep -F z | wc -l > count.txt

echo "finished"
codes=("${PIPESTATUS[@]}")
```

因为 `echo` 会更新 `PIPESTATUS`，导致原来的三个管道状态丢失。

---

## 检查结果

查看计数：

```bash
cat count.txt
```

查看三个管道状态：

```bash
cat pipeline-statuses.txt
```

### `words.txt` 中没有 `z`

应得到：

```text
0
1
0
```

### 文件中存在 `z`

应得到：

```text
0
0
0
```

### 文件不存在

例如：

```bash
bash pipeline-status.sh missing.txt
```

会看到 `cat` 的错误信息，同时：

```bash
cat pipeline-statuses.txt
```

应得到：

```text
1
1
0
```

而：

```bash
cat count.txt
```

仍然是：

```text
0
```

---

## 为什么脚本最后是 `exit 0`

题目要求：

> 无论管道内部状态如何，脚本本身最终返回 `0`。

因此最后明确写：

```bash
exit 0
```

例如：

```bash
bash pipeline-status.sh missing.txt
echo "$?"
```

虽然管道内部记录为：

```text
1
1
0
```

但脚本整体返回：

```text
0
```

这两个概念必须区分：

```text
PIPESTATUS → 管道中各个命令发生了什么
脚本 exit → 整个脚本最终向调用者报告什么
```

## 核心记忆

```bash
first | second | third
codes=("${PIPESTATUS[@]}")
```

是获取管道各阶段状态的核心模式。

牢记三点：

1. `$?` 默认只表示管道最后一个命令的状态。
2. `${PIPESTATUS[@]}` 保存最近一个管道中所有命令的状态。
3. 必须在管道结束后**立即**复制 `PIPESTATUS`，否则下一条命令会覆盖它。
