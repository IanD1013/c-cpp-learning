# Bash：用 `grep` 计数并根据退出状态判断日志健康度

## 目标

编写 `/home/learner/bash-lab/check-health.sh`：

1. 从命令行参数 `"$1"` 接收日志文件路径。
2. 统计以 `ERROR:` 开头的日志行数量。
3. 根据数量写入 `health.txt`：

   * `0` 条错误 → `healthy`
   * `1～3` 条错误 → `degraded`
   * `4` 条及以上 → `failed`
4. 如果日志文件无法读取：

   * 脚本返回退出状态 `2`
   * **不能写入 `health.txt`**

---

## 可以直接通过练习的脚本

在 `/home/learner/bash-lab/check-health.sh` 中写入：

```bash
#!/usr/bin/env bash

count=$(grep -c '^ERROR:' "$1")
grep_status=$?

if (( grep_status > 1 )); then
    exit 2
fi

if (( count == 0 )); then
    printf '%s\n' 'healthy' > health.txt
elif (( count <= 3 )); then
    printf '%s\n' 'degraded' > health.txt
else
    printf '%s\n' 'failed' > health.txt
fi
```

然后运行：

```bash
cd /home/learner/bash-lab
chmod +x check-health.sh
./check-health.sh API.log
```

这样会根据 `API.log` 的内容创建：

```text
health.txt
```

---

## 1. `grep -c`：统计匹配的行数

核心命令：

```bash
grep -c '^ERROR:' "$1"
```

其中：

```text
grep
│
├── -c          只输出匹配的行数
├── '^ERROR:'   匹配以 ERROR: 开头的行
└── "$1"        要读取的日志文件
```

例如日志：

```text
INFO: server started
ERROR: database unavailable
WARNING: retrying
ERROR: request failed
```

执行：

```bash
grep -c '^ERROR:' API.log
```

输出：

```text
2
```

---

## 2. 为什么使用 `^ERROR:` 而不是 `ERROR:`

正则表达式中的：

```text
^
```

表示：

> 行的开头。

所以：

```bash
'^ERROR:'
```

只匹配：

```text
ERROR: database unavailable
```

而不会匹配：

```text
INFO: previous ERROR: database unavailable
```

这是在统计真正的 `ERROR` 日志记录，而不是任何碰巧包含 `ERROR:` 的行。

---

## 3. 为什么不能使用 `grep -F`

`-F` 表示把模式当作普通字符串，不解释正则表达式。

错误写法：

```bash
grep -cF '^ERROR:' "$1"
```

这里 `grep` 会真的寻找字符：

```text
^ERROR:
```

而不是把 `^` 理解成“行首”。

因此这里应该使用：

```bash
grep -c '^ERROR:' "$1"
```

---

## 4. Command Substitution：保存命令输出

```bash
count=$(grep -c '^ERROR:' "$1")
```

`$(...)` 称为 **命令替换（command substitution）**。

Bash 会：

1. 执行括号里的命令；
2. 获取命令的标准输出；
3. 把输出放进变量。

例如：

```bash
count=$(grep -c '^ERROR:' API.log)
```

如果 `grep` 输出：

```text
2
```

那么最终相当于：

```bash
count=2
```

---

## 5. `$?`：取得上一条命令的退出状态

紧接着必须写：

```bash
grep_status=$?
```

`$?` 表示：

> **刚刚执行完成的命令的退出状态。**

对于 `grep`：

| `grep` 状态 | 含义                |
| --------: | ----------------- |
|       `0` | 文件读取成功，并且找到了匹配    |
|       `1` | 文件读取成功，但没有找到匹配    |
|   `2` 或更高 | 发生错误，例如文件不存在或无法读取 |

所以：

```bash
count=$(grep -c '^ERROR:' "$1")
grep_status=$?
```

这两行应该紧挨着。

### 为什么必须立即保存 `$?`

因为执行任何下一条命令都会覆盖 `$?`。

例如：

```bash
count=$(grep -c '^ERROR:' "$1")
echo "checking"
grep_status=$?
```

此时：

```bash
grep_status
```

记录的是 `echo` 的状态，而不是 `grep` 的状态。

正确方式：

```bash
count=$(grep -c '^ERROR:' "$1")
grep_status=$?
```

---

## 6. 最重要的区别：0 个匹配 ≠ 文件读取失败

这是本练习的核心。

### 情况一：文件正常，但没有 ERROR

例如：

```text
INFO: started
INFO: request complete
```

执行：

```bash
grep -c '^ERROR:' API.log
```

输出：

```text
0
```

同时：

```bash
$? = 1
```

这不是错误。

它表示：

> 文件成功读取，只是没有找到任何匹配。

因此健康状态应该是：

```text
healthy
```

---

### 情况二：文件根本打不开

例如：

```bash
./check-health.sh missing.log
```

`grep` 可能报告：

```text
grep: missing.log: No such file or directory
```

此时退出状态通常是：

```text
2
```

这种情况下不能说系统：

```text
healthy
```

因为脚本根本不知道日志里有没有错误。

正确行为是：

```bash
exit 2
```

---

## 7. 为什么不能写 `|| true`

一种常见但危险的写法是：

```bash
count=$(grep -c '^ERROR:' "$1" || true)
```

它会把 `grep` 的失败状态强行变成成功。

例如文件不存在：

```bash
./check-health.sh missing.log
```

`grep` 没有输出有效数字，可能导致：

```bash
count=
```

而 Bash 的算术表达式可能把空值当成 `0`：

```bash
(( count == 0 ))
```

于是脚本可能错误地写：

```text
healthy
```

这相当于：

> “日志打不开，所以系统很健康。”

显然不合理。

因此必须保留 `grep` 自己的状态：

```bash
count=$(grep -c '^ERROR:' "$1")
grep_status=$?
```

再单独处理：

```bash
if (( grep_status > 1 )); then
    exit 2
fi
```

---

## 8. 为什么 `grep_status` 的判断是 `> 1`

正确判断：

```bash
if (( grep_status > 1 )); then
    exit 2
fi
```

因为：

```text
0 → 找到了匹配          → 正常
1 → 没找到匹配          → 正常
2+ → grep 真正发生错误   → 异常
```

因此：

```bash
0 和 1
```

都应该继续使用 `count`。

只有：

```bash
> 1
```

才退出。

---

## 9. 根据 ERROR 数量分类

### 0 条：healthy

```bash
if (( count == 0 )); then
    printf '%s\n' 'healthy' > health.txt
```

结果：

```text
healthy
```

---

### 1～3 条：degraded

```bash
elif (( count <= 3 )); then
    printf '%s\n' 'degraded' > health.txt
```

这里的边界是**包含 3**：

```text
1 → degraded
2 → degraded
3 → degraded
```

因此必须使用：

```bash
<= 3
```

而不是：

```bash
< 3
```

---

### 4 条及以上：failed

```bash
else
    printf '%s\n' 'failed' > health.txt
fi
```

因为前面的情况已经排除了：

```text
count == 0
count <= 3
```

所以进入 `else` 时一定满足：

```text
count > 3
```

即：

```text
4, 5, 6, ...
```

---

## 10. 完整判断流程

整个脚本的逻辑可以表示为：

```text
读取日志
   │
   ▼
grep -c '^ERROR:'
   │
   ├── grep 状态 > 1
   │       │
   │       └── exit 2
   │           不写 health.txt
   │
   └── grep 状态为 0 或 1
           │
           ▼
       检查 count
           │
           ├── count == 0
           │       └── healthy
           │
           ├── count <= 3
           │       └── degraded
           │
           └── count > 3
                   └── failed
```

---

## 11. 练习要求对应表

| ERROR 行数 | `grep` 状态 | `health.txt` | 脚本状态 |
| -------: | --------: | ------------ | ---- |
|        0 |         1 | `healthy`    | 正常   |
|        2 |         0 | `degraded`   | 正常   |
|        3 |         0 | `degraded`   | 正常   |
|        4 |         0 | `failed`     | 正常   |
|   文件无法读取 |        ≥2 | 不应写入         | `2`  |

---

## 12. 可以手动验证的测试

### 测试 0 条错误

```bash
printf '%s\n' \
    'INFO: started' \
    'INFO: complete' > test.log

./check-health.sh test.log
cat health.txt
```

应该得到：

```text
healthy
```

### 测试 3 条错误

```bash
printf '%s\n' \
    'ERROR: one' \
    'ERROR: two' \
    'ERROR: three' > test.log

./check-health.sh test.log
cat health.txt
```

应该得到：

```text
degraded
```

### 测试 4 条错误

```bash
printf '%s\n' \
    'ERROR: one' \
    'ERROR: two' \
    'ERROR: three' \
    'ERROR: four' > test.log

./check-health.sh test.log
cat health.txt
```

应该得到：

```text
failed
```

### 测试不存在的文件

为了避免之前的 `health.txt` 干扰判断，可以先删除：

```bash
rm -f health.txt
```

然后：

```bash
./check-health.sh does-not-exist.log
echo $?
```

应该看到退出状态：

```text
2
```

同时：

```bash
test ! -e health.txt
echo $?
```

应该输出：

```text
0
```

表示 `health.txt` 确实不存在。

---

## 最终答案

要通过本课，`check-health.sh` 的核心实现应为：

```bash
#!/usr/bin/env bash

count=$(grep -c '^ERROR:' "$1")
grep_status=$?

if (( grep_status > 1 )); then
    exit 2
fi

if (( count == 0 )); then
    printf '%s\n' 'healthy' > health.txt
elif (( count <= 3 )); then
    printf '%s\n' 'degraded' > health.txt
else
    printf '%s\n' 'failed' > health.txt
fi
```

关键知识点是：**不能只看 `grep -c` 得到的数字，还必须同时检查 `grep` 的退出状态，因为“0 个匹配”和“文件根本没读到”是完全不同的情况。**
