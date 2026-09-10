# Bash 管道中的重定向与 `tee`：边保存边继续处理数据

## 1. 核心问题：`>` 会让数据流在这里结束

在 Shell 中，管道 `|` 会把前一个命令的标准输出（stdout）交给下一个命令：

```bash
command1 | command2 | command3
```

例如：

```bash
tr '[:lower:]' '[:upper:]' < names.txt | wc -l
```

数据流是：

```text
names.txt
   ↓
  tr
   ↓
 wc -l
```

但如果这样写：

```bash
tr '[:lower:]' '[:upper:]' > audit.log
```

`tr` 的标准输出已经被重定向到 `audit.log`：

```text
tr
 ↓
audit.log
```

后续命令无法再从这份标准输出中读取数据。

因此，单纯使用 `>` 很难同时完成：

1. 保存完整输出；
2. 继续把相同输出交给下一个命令处理。

这正是 `tee` 的用途。

---

## 2. `tee`：把一个数据流复制成两份

基本语法：

```bash
command1 | tee file | command2
```

数据流可以理解为：

```text
                → file
               /
command1 → tee
               \
                → command2
```

`tee` 会：

* 从标准输入读取数据；
* 将相同数据写入指定文件；
* 同时将数据继续写到自己的标准输出。

例如：

```bash
printf 'api\nworker\nweb\n' | tee audit.log | wc -l
```

结果：

`audit.log`：

```text
api
worker
web
```

而：

```bash
wc -l
```

同时收到完全相同的三行，因此输出：

```text
3
```

### `tee` 的关键特点

`tee` 保存的数据和继续传入管道的数据是同一份字节流：

```text
stdin → tee → stdout
          ↓
        file
```

因此非常适合：

* 保存日志，同时继续分析；
* 保存命令输出，同时统计；
* 调试长管道；
* 一次读取不可重复的数据流，同时完成多个操作。

---

# 3. `tr`：通过标准输入转换字符

假设 `names.txt` 内容为：

```text
api
worker
web
```

命令：

```bash
tr '[:lower:]' '[:upper:]'
```

会把所有小写字母转换为大写字母。

例如：

```bash
tr '[:lower:]' '[:upper:]' < names.txt
```

输出：

```text
API
WORKER
WEB
```

这里没有给 `tr` 指定文件名。

这是因为 `tr` 通常通过：

```text
stdin → tr → stdout
```

工作。

也就是说：

```bash
tr '[:lower:]' '[:upper:]' < names.txt
```

可以理解为：

```text
names.txt
   ↓
stdin
   ↓
  tr
   ↓
stdout
```

---

# 4. `$1`：使用脚本的第一个参数

假设脚本 `audit.sh` 被这样调用：

```bash
bash audit.sh audit.log
```

那么脚本内部：

```bash
$1
```

就是：

```text
audit.log
```

因此：

```bash
tee "$1"
```

等价于：

```bash
tee audit.log
```

推荐始终写成：

```bash
"$1"
```

而不是：

```bash
$1
```

因为双引号可以正确处理文件名中的空格和特殊字符。

例如：

```bash
bash audit.sh "my audit.log"
```

如果脚本中使用：

```bash
tee "$1"
```

就可以正确写入：

```text
my audit.log
```

---

# 5. 完整的 `audit.sh`

目标是一次完成：

1. 从标准输入读取名字；
2. 转换为大写；
3. 保存大写后的完整数据；
4. 同时继续处理相同的数据；
5. 统计行数并写入 `count.txt`。

脚本：

```bash
tr '[:lower:]' '[:upper:]' | tee "$1" | wc -l > count.txt
```

完整的 `audit.sh` 可以写成：

```bash
#!/usr/bin/env bash

tr '[:lower:]' '[:upper:]' | tee "$1" | wc -l > count.txt
```

核心管道只有三个阶段：

```text
tr
 ↓
tee
 ↓
wc -l
```

---

# 6. 如何运行

假设目录：

```text
/home/learner/bash-lab/
├── audit.sh
└── names.txt
```

`names.txt`：

```text
api
worker
web
```

进入目录：

```bash
cd /home/learner/bash-lab
```

运行：

```bash
bash audit.sh audit.log < names.txt
```

这里有两个不同的信息来源。

### `audit.log`

这是脚本的第一个参数：

```bash
audit.log
```

所以：

```bash
"$1"
```

的值是：

```text
audit.log
```

### `names.txt`

这是脚本的标准输入：

```bash
< names.txt
```

也就是说，整个脚本获得的 stdin 来自：

```text
names.txt
```

---

# 7. 完整的数据流

执行：

```bash
bash audit.sh audit.log < names.txt
```

脚本内部：

```bash
tr '[:lower:]' '[:upper:]' | tee "$1" | wc -l > count.txt
```

整个过程可以画成：

```text
names.txt
   │
   │ stdin
   ▼
┌──────┐
│  tr  │
└──────┘
   │
   │ API
   │ WORKER
   │ WEB
   ▼
┌──────────┐
│   tee    │──────→ audit.log
└──────────┘
   │
   │ 相同的数据
   ▼
┌──────────┐
│  wc -l   │
└──────────┘
   │
   │ 3
   ▼
count.txt
```

最终：

```text
/home/learner/bash-lab/audit.log
```

内容：

```text
API
WORKER
WEB
```

而：

```text
/home/learner/bash-lab/count.txt
```

内容：

```text
3
```

---

# 8. 为什么不能先保存，再从文件读取？

当然可以写成类似：

```bash
tr '[:lower:]' '[:upper:]' > audit.log
wc -l < audit.log > count.txt
```

但这种方式存在一个重要区别：

```text
第一次：读取输入 → 保存文件
第二次：重新读取文件 → 统计
```

而使用：

```bash
tr '[:lower:]' '[:upper:]' | tee audit.log | wc -l
```

则是：

```text
读取输入一次 → 保存 + 统计
```

这对于流式数据非常重要，因为有些输入：

* 不能重新读取；
* 没有对应的普通文件；
* 可能来自网络；
* 可能来自另一个程序；
* 可能持续实时产生。

因此更通用的设计是：

> **让统计直接从正在经过的流中完成，而不是依赖之后重新读取保存下来的文件。**

---

# 9. 为什么说“流只能经过一次”？

假设：

```bash
some_command | tr ... | tee log.txt | wc -l
```

Shell 管道中的数据是持续向前流动的。

某个字节被一个程序读走之后，它不会自动回到前面的管道中供程序再次读取。

可以把管道想象成传送带：

```text
A → B → C
```

数据从 A 到 B，再到 C。

如果需要同时：

```text
        → 文件
       /
数据 →
       \
        → 后续处理
```

就必须主动复制数据流。

`tee` 正是负责这个复制操作的程序。

---

# 10. `/dev/null`：丢弃所有写入的数据

Unix/Linux 中：

```text
/dev/null
```

是一个特殊设备文件。

写入它的数据都会被直接丢弃。

例如：

```bash
echo hello > /dev/null
```

不会产生普通文件，也不会显示：

```text
hello
```

可以把 `/dev/null` 理解成：

```text
数据黑洞
```

---

## `tee /dev/null` 仍然可以继续传递数据

例如：

```bash
printf 'api\nworker\nweb\n' | tee /dev/null | wc -l
```

虽然：

```bash
tee /dev/null
```

把自己的文件副本丢掉了，但它仍然会把数据写到 stdout。

所以：

```bash
wc -l
```

依然收到：

```text
api
worker
web
```

结果仍然是：

```text
3
```

这说明：

> `tee` 写入哪个目标文件，不影响后面的管道是否能够继续收到数据。

因此脚本：

```bash
tr '[:lower:]' '[:upper:]' | tee "$1" | wc -l > count.txt
```

即使这样运行：

```bash
bash audit.sh /dev/null < names.txt
```

`count.txt` 仍然应该是：

```text
3
```

只是大写后的名字没有被真正保存下来。

---

# 11. `tee` 默认会覆盖旧文件

默认情况下：

```bash
tee audit.log
```

会像：

```bash
> audit.log
```

一样，在写入前截断（truncate）已有文件。

假设原来的 `audit.log`：

```text
OLD DATA
MORE OLD DATA
```

执行：

```bash
printf 'API\nWORKER\nWEB\n' | tee audit.log
```

之后文件只剩：

```text
API
WORKER
WEB
```

旧内容不会保留。

这正适合“每次运行生成新的完整日志”的场景。

---

# 12. `tee -a` 是追加模式

如果使用：

```bash
tee -a audit.log
```

其中 `-a` 表示：

```text
append
```

即追加。

例如第一次：

```bash
printf 'API\n' | tee audit.log
```

文件：

```text
API
```

第二次：

```bash
printf 'WEB\n' | tee -a audit.log
```

文件变成：

```text
API
WEB
```

因此：

| 命令            | 行为     |
| ------------- | ------ |
| `tee file`    | 覆盖旧文件  |
| `tee -a file` | 追加到旧文件 |

如果要求每次运行后日志只包含当前输入，就应该使用：

```bash
tee "$1"
```

而不是：

```bash
tee -a "$1"
```

---

# 13. `wc -l` 实际统计什么？

命令：

```bash
wc -l
```

统计的是输入中的**换行符数量**。

通常文本文件每一行都以：

```text
\n
```

结束，因此可以近似理解成“统计行数”。

例如：

```bash
printf 'API\nWORKER\nWEB\n' | wc -l
```

得到：

```text
3
```

---

## 最后的空行也可能被统计

例如：

```bash
printf 'API\nWORKER\nWEB\n\n' | wc -l
```

这里的数据实际包含四个换行符，因此：

```bash
wc -l
```

输出：

```text
4
```

所以不能：

1. 保存数据；
2. 之后手工重建数据；
3. 再猜测原流有多少行。

正确方式是：

```bash
... | tee "$1" | wc -l
```

因为 `wc` 直接统计真实经过管道的数据。

---

# 14. `tee` 保存的数据与 `wc` 看到的数据完全相同

对于：

```bash
tr ... | tee "$1" | wc -l
```

`tee`：

1. 收到一串字节；
2. 将这些字节写入 `$1`；
3. 将相同字节写到 stdout；
4. stdout 被 `wc -l` 读取。

因此：

```text
保存到文件的数据
        =
传给 wc 的数据
```

这是这个设计最重要的性质。

如果数据中包含：

```text
API\n
WORKER\n
WEB\n
\n
```

那么：

* `tee` 会保留最后那个额外换行；
* `wc -l` 也会看到它；
* 因此会统计为 `4`。

---

# 15. `>` 与 `|` 的作用完全不同

容易混淆的两个符号是：

```bash
|
```

和：

```bash
>
```

## `|`：把 stdout 连接到下一个程序

```bash
command1 | command2
```

相当于：

```text
command1 stdout
       ↓
command2 stdin
```

---

## `>`：把 stdout 写入文件

```bash
command > file
```

相当于：

```text
command stdout
       ↓
      file
```

因此：

```bash
wc -l > count.txt
```

意味着：

```text
wc -l
  ↓
产生数字 3
  ↓
count.txt
```

而不是把 `count.txt` 作为 `wc` 的输入。

---

# 16. `<`：把文件连接到 stdin

命令：

```bash
bash audit.sh audit.log < names.txt
```

中的：

```bash
< names.txt
```

表示：

```text
names.txt → audit.sh 的 stdin
```

因此脚本内部第一个程序：

```bash
tr '[:lower:]' '[:upper:]'
```

不需要知道 `names.txt` 的名字。

它只需要读取：

```text
stdin
```

这使脚本更加通用。

同一个脚本既可以处理：

```bash
bash audit.sh audit.log < names.txt
```

也可以处理：

```bash
printf 'api\nworker\nweb\n' | bash audit.sh audit.log
```

甚至：

```bash
some_other_command | bash audit.sh audit.log
```

脚本只关心：

```text
stdin
```

而不关心 stdin 来自哪里。

---

# 17. Shell 中最重要的设计思想：程序只处理流

一个很常见的 Unix 风格设计是：

```text
stdin → 程序 → stdout
```

每个工具只完成一件事情。

这个例子中：

```text
tr
```

负责：

```text
字符转换
```

`tee` 负责：

```text
复制数据流
```

`wc -l` 负责：

```text
统计行数
```

`>` 负责：

```text
把 stdout 写入文件
```

最终通过：

```bash
tr ... | tee ... | wc ...
```

把几个简单工具组合成完整功能。

这就是 Shell 管道强大的核心原因：

> **复杂任务不是一定由一个复杂程序完成，而可以由多个简单程序通过标准输入和标准输出组合完成。**

---

# 18. 最终脚本与结果

`audit.sh`：

```bash
#!/usr/bin/env bash

tr '[:lower:]' '[:upper:]' | tee "$1" | wc -l > count.txt
```

输入文件 `names.txt`：

```text
api
worker
web
```

运行：

```bash
cd /home/learner/bash-lab
bash audit.sh audit.log < names.txt
```

最终：

`audit.log`：

```text
API
WORKER
WEB
```

`count.txt`：

```text
3
```

---

# 19. 关键知识点速记

```bash
# 文件作为标准输入
command < input.txt

# 标准输出写入文件，覆盖旧内容
command > output.txt

# 将前一个程序的 stdout 传给下一个程序
command1 | command2

# 保存数据，同时继续向管道输出
command1 | tee output.txt | command2

# tee 追加而不是覆盖
command1 | tee -a output.txt

# 丢弃保存的副本，但数据仍继续进入管道
command1 | tee /dev/null | command2
```

本例最核心的数据流：

```text
stdin
  ↓
 tr
  ↓
 tee ─────→ audit.log
  ↓
wc -l
  ↓
count.txt
```

核心原则：

> `>` 把标准输出引向文件；`|` 把标准输出引向下一个程序；`tee` 则可以同时做到“写入文件”和“继续沿管道传递数据”。
