# Bash `while` 循环与计数器：生成指定次数的重试计划

## 目标

编写一个 Bash 脚本 `retry-plan.sh`，由第一个位置参数决定需要生成多少次尝试记录。

例如：

```bash
bash retry-plan.sh 3
```

执行后，当前工作目录中的 `attempts.txt` 应只包含：

```text
attempt 1
attempt 2
attempt 3
```

如果执行：

```bash
bash retry-plan.sh 5
```

则文件应包含 5 行：

```text
attempt 1
attempt 2
attempt 3
attempt 4
attempt 5
```

核心知识点是：

> `while` 循环可以反复检查一个不断变化的变量，并在条件仍然成立时继续执行。

---

## 1. `while` 循环的基本结构

Bash 的 `while` 循环结构是：

```bash
while condition; do
  commands
done
```

执行顺序：

1. 检查 `condition`
2. 条件成功时执行循环体
3. 执行到 `done`
4. 回到顶部再次检查条件
5. 条件失败时退出循环

因此，`while` 属于**先判断、后执行**的循环。

---

## 2. 使用算术条件控制循环次数

计数循环通常使用 `(( ... ))`：

```bash
while (( count < limit )); do
  ...
done
```

这里：

* `count`：当前已经进行到第几次
* `limit`：最多需要进行多少次
* `<`：当 `count` 小于 `limit` 时继续循环

在 `(( ... ))` 内部，变量被当作数字处理，所以通常不需要写 `$`：

```bash
(( count < limit ))
```

而不是必须写成：

```bash
(( $count < $limit ))
```

推荐前一种形式。

---

## 3. 从命令行参数取得循环上限

执行：

```bash
bash retry-plan.sh 3
```

时：

```bash
$1
```

就是：

```text
3
```

可以保存到变量：

```bash
limit="$1"
```

因此：

```bash
while (( count < limit )); do
```

实际上会不断比较当前计数和用户指定的上限。

---

## 4. 为什么计数器从 0 开始

初始化：

```bash
count=0
```

循环内部先执行：

```bash
((count++))
```

再写入：

```bash
printf 'attempt %d\n' "$count"
```

第一次循环时：

```text
count = 0
```

因为：

```text
0 < 3
```

成立，所以进入循环。

随后：

```bash
((count++))
```

把 `count` 变成：

```text
1
```

因此第一行写出：

```text
attempt 1
```

对于 `limit=3`，整个过程是：

| 条件检查    | 是否进入循环 | 加 1 后 | 写入内容        |
| ------- | ------ | ----- | ----------- |
| `0 < 3` | 是      | 1     | `attempt 1` |
| `1 < 3` | 是      | 2     | `attempt 2` |
| `2 < 3` | 是      | 3     | `attempt 3` |
| `3 < 3` | 否      | —     | —           |

最终正好产生三行。

---

## 5. 每次运行前清空旧文件

如果直接使用：

```bash
>> attempts.txt
```

追加内容，那么第二次执行脚本时，第一次运行留下的数据仍然存在。

因此应在进入循环之前清空文件：

```bash
: > attempts.txt
```

这里的：

```bash
:
```

是 Bash 的空命令（null command），本身什么也不做。

配合重定向：

```bash
> attempts.txt
```

会：

* 文件不存在：创建空文件
* 文件已经存在：把内容截断为 0 字节

因此：

```bash
: > attempts.txt
```

可以理解为：

> 创建或清空 `attempts.txt`。

---

## 6. 在循环中追加内容

循环每生成一次记录，就使用：

```bash
printf 'attempt %d\n' "$count" >> attempts.txt
```

其中：

```bash
>>
```

表示**追加重定向**（append）。

它不会覆盖之前已经写入的行。

例如第一次：

```text
attempt 1
```

第二次追加后：

```text
attempt 1
attempt 2
```

第三次：

```text
attempt 1
attempt 2
attempt 3
```

因此这里需要组合使用：

```bash
: > attempts.txt
```

先清空一次，然后：

```bash
>> attempts.txt
```

在循环中不断追加。

---

## 7. 完整脚本

`retry-plan.sh`：

```bash
#!/usr/bin/env bash

limit="$1"
count=0

: > attempts.txt

while (( count < limit )); do
  ((count++))
  printf 'attempt %d\n' "$count" >> attempts.txt
done
```

运行：

```bash
cd /home/learner/bash-lab
bash retry-plan.sh 3
```

检查：

```bash
cat attempts.txt
```

结果：

```text
attempt 1
attempt 2
attempt 3
```

---

## 8. 为什么使用 `attempts.txt` 而不是脚本所在目录的完整路径

脚本写的是：

```bash
attempts.txt
```

这是一个**相对路径**。

它相对于运行脚本时的当前工作目录（current working directory），而不是相对于脚本文件所在的位置。

例如：

```bash
cd /home/learner/bash-lab
bash retry-plan.sh 3
```

文件会生成在：

```text
/home/learner/bash-lab/attempts.txt
```

但如果从其他目录执行：

```bash
cd /tmp
bash /home/learner/bash-lab/retry-plan.sh 3
```

那么文件会生成在：

```text
/tmp/attempts.txt
```

这是 Bash 脚本处理中相对路径时非常重要的区别。

---

## 9. `((count++))` 的一个细节

```bash
((count++))
```

不仅会修改变量，还会产生一个命令退出状态。

后置自增使用的是**增加之前的值**判断算术表达式真假。因此第一次执行时：

```bash
count=0
((count++))
```

虽然 `count` 成功变成了 `1`，但表达式原来的值是 `0`，所以该命令的退出状态是 `1`。

普通脚本中这通常没有影响。

但如果脚本启用了：

```bash
set -e
```

这种写法可能导致意外退出。

更稳妥的写法是：

```bash
((count += 1))
```

对于这里从 0 开始递增的场景，它第一次计算结果就是 `1`，退出状态为 0。

因此生产脚本中可以写成：

```bash
while (( count < limit )); do
  ((count += 1))
  printf 'attempt %d\n' "$count" >> attempts.txt
done
```

---

## 10. 核心总结

一个由变量控制次数的 `while` 循环通常包含三个部分：

```bash
count=0

while (( count < limit )); do
  ((count += 1))
  ...
done
```

关键逻辑是：

```text
初始化计数器
    ↓
检查条件
    ↓
执行循环体
    ↓
修改计数器
    ↓
重新检查条件
```

生成文件时，则常见模式是：

```bash
: > output.txt

while (( ... )); do
  printf '...\n' >> output.txt
done
```

其中：

* `>`：先清空或创建文件
* `>>`：之后逐次追加
* `while`：在每次循环前检查条件
* `(( ... ))`：进行整数算术判断
* `done`：标记循环体结束
