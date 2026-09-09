# Bash `pipefail`：让管道中的失败影响整个管道状态

## 1. Bash 管道默认只看最后一个命令

Bash 管道：

```bash
command1 | command2 | command3
```

默认情况下，整个管道的退出状态（exit status）等于**最后一个命令 `command3` 的退出状态**。

例如：

```bash
printf '%s\n' "hello" | grep -F "xyz" | wc -l
```

执行过程：

1. `printf` 输出：

   ```text
   hello
   ```

2. `grep -F "xyz"` 没找到匹配，因此：

   * 不输出任何内容；
   * 退出状态为 `1`。

3. `wc -l` 收到空输入：

   * 输出 `0`；
   * 正常完成，因此退出状态为 `0`。

于是默认情况下：

```bash
printf '%s\n' "hello" | grep -F "xyz" | wc -l
echo "$?"
```

结果类似：

```text
0
0
```

第一个 `0` 是 `wc -l` 统计出的行数。

第二个 `0` 是**整个管道的退出状态**。

虽然中间的 `grep` 返回了 `1`，但 Bash 默认只采用最后一个命令 `wc` 的状态，因此调用者看不到中间阶段的非零状态。

---

## 2. `grep` 的退出状态

理解这个例子时，要特别注意 `grep` 的状态码：

| 状态码  | 含义               |
| ---- | ---------------- |
| `0`  | 找到了至少一个匹配        |
| `1`  | 没找到匹配            |
| `>1` | 真正的执行错误，例如文件无法读取 |

因此：

```bash
grep -F "hello"
```

“没有匹配”虽然不一定意味着程序发生故障，但它仍然是一个**非零退出状态**。

在 shell 条件判断中，非零就会被视为 false。

---

## 3. `set -o pipefail`

开启：

```bash
set -o pipefail
```

后，Bash 不再简单地采用最后一个命令的状态。

规则变为：

> 如果管道中的命令全部成功，管道返回 `0`；如果存在失败，则返回**最右边那个非零退出状态**。

例如：

```bash
set -o pipefail

printf '%s\n' "hello" | grep -F "xyz" | wc -l

echo "$?"
```

此时：

* `printf` → `0`
* `grep` → `1`
* `wc` → `0`

所以整个管道返回：

```text
1
```

虽然 `wc` 仍然打印：

```text
0
```

但管道的退出状态变成了：

```text
1
```

### 关键区别

`pipefail` **不会改变管道中的数据流**。

也就是说：

```bash
printf ... | grep ... | wc -l
```

开启 `pipefail` 前后：

* `grep` 仍然输出同样的数据；
* `wc` 仍然收到同样的数据；
* `wc` 仍然计算相同的数字。

改变的只有：

> **整个 pipeline 最终向 Bash 报告什么退出状态。**

---

# 4. `pipefail` 与 `if` 配合

Shell 中可以直接把一个管道作为 `if` 的条件：

```bash
if first | second | third; then
    ...
else
    ...
fi
```

Bash 判断的是：

```text
pipeline exit status == 0 ?
```

也就是：

* 返回 `0` → 执行 `then`
* 返回非零 → 执行 `else`

例如：

```bash
set -o pipefail

if printf '%s\n' "hello" |
   grep -F "xyz" |
   wc -l
then
    echo "passed"
else
    echo "failed"
fi
```

因为：

```text
printf → 0
grep   → 1
wc     → 0
```

在 `pipefail` 下，整个管道返回 `1`，所以执行：

```text
failed
```

---

# 5. 为什么 `wc -l` 仍然会写出 `0`

假设：

```bash
printf '%s\n%s\n' "$1" "$2" |
grep -F "$3" |
wc -l > count.txt
```

如果 `$3` 在两行中都不存在：

```text
printf
  │
  ▼
line1
line2
  │
  ▼
grep
没有匹配
退出状态 = 1
输出 = 空
  │
  ▼
wc -l
统计空输入
输出 = 0
退出状态 = 0
```

因此：

```text
count.txt
```

仍然会包含：

```text
0
```

与此同时，由于启用了：

```bash
set -o pipefail
```

整个管道的退出状态是：

```text
1
```

这两个结果并不矛盾：

* `count.txt` 表示**匹配了多少行**
* pipeline status 表示**管道在 shell 条件判断中是否成功**

---

# 6. 完整示例：`verify-stream.sh`

文件：

```text
/home/learner/bash-lab/verify-stream.sh
```

内容：

```bash
#!/usr/bin/env bash

set -o pipefail

if printf '%s\n%s\n' "$1" "$2" |
   grep -F "$3" |
   wc -l > /home/learner/bash-lab/count.txt
then
    printf '%s\n' "passed" > /home/learner/bash-lab/pipeline-verdict.txt
else
    printf '%s\n' "failed" > /home/learner/bash-lab/pipeline-verdict.txt
fi
```

## 参数含义

脚本接受三个位置参数：

```text
$1 → 第一行
$2 → 第二行
$3 → 要搜索的字符串
```

例如：

```bash
./verify-stream.sh "apple" "banana" "orange"
```

相当于让管道处理：

```text
apple
banana
```

并查找：

```text
orange
```

---

# 7. 为什么参数必须写成 `"$1"`、`"$2"`、`"$3"`

应该写：

```bash
"$1"
"$2"
"$3"
```

而不是：

```bash
$1
$2
$3
```

双引号可以避免 shell 对参数再次执行：

* word splitting（单词拆分）
* pathname expansion / globbing（通配符展开）

例如：

```bash
./verify-stream.sh "hello world" "second line" "hello world"
```

使用：

```bash
"$1"
```

才能把：

```text
hello world
```

作为一个完整参数处理。

---

# 8. 为什么使用 `grep -F`

命令：

```bash
grep -F "$3"
```

中的 `-F` 表示：

> Fixed String，按普通字符串匹配，而不是把搜索内容解释为正则表达式。

例如：

```bash
grep -F "a.b"
```

搜索的是字面内容：

```text
a.b
```

而普通：

```bash
grep "a.b"
```

中的 `.` 是正则表达式元字符，可以匹配任意单个字符。

如果脚本的目标只是查找用户提供的“一个字符串”，通常：

```bash
grep -F
```

更合适。

---

# 9. 测试没有匹配的情况

先赋予执行权限：

```bash
chmod +x /home/learner/bash-lab/verify-stream.sh
```

运行：

```bash
/home/learner/bash-lab/verify-stream.sh \
    "apple" \
    "banana" \
    "orange"
```

因为 `orange` 不存在于：

```text
apple
banana
```

所以：

```bash
cat /home/learner/bash-lab/pipeline-verdict.txt
```

应得到：

```text
failed
```

而：

```bash
cat /home/learner/bash-lab/count.txt
```

应得到：

```text
0
```

---

# 10. 最容易混淆的地方：脚本本身最后仍然返回 `0`

运行：

```bash
/home/learner/bash-lab/verify-stream.sh \
    "apple" \
    "banana" \
    "orange"

echo "$?"
```

可能得到：

```text
0
```

这并不意味着管道成功。

实际过程是：

```text
grep 没匹配
       ↓
grep 返回 1
       ↓
pipefail 让整个 pipeline 返回 1
       ↓
if 判断为 false
       ↓
进入 else
       ↓
printf "failed" 成功执行
       ↓
printf 返回 0
       ↓
if 这个复合命令最终返回 0
       ↓
脚本最后返回 0
```

因此这里同时存在三个不同概念：

| 内容          |  结果 |
| ----------- | --: |
| 匹配数量        | `0` |
| pipeline 状态 | `1` |
| 整个脚本最终状态    | `0` |

---

# 11. `if` 为什么会“吃掉”条件命令的失败状态

考虑：

```bash
if some-command; then
    printf '%s\n' "passed"
else
    printf '%s\n' "failed"
fi
```

如果：

```bash
some-command
```

返回：

```text
1
```

这个 `1` 被 `if` 用来决定进入哪个分支。

进入：

```bash
else
```

以后又执行：

```bash
printf '%s\n' "failed"
```

而 `printf` 成功返回：

```text
0
```

所以整个：

```bash
if ...; then
    ...
else
    ...
fi
```

最终可以返回 `0`。

因此：

> `pipefail` 决定的是 **`if` 看见什么状态**，并不自动决定整个脚本最终返回什么状态。

---

# 12. 如果希望脚本本身也返回失败

当前脚本的设计目标是：

* 用文件记录 `passed` / `failed`
* 脚本本身最终仍然成功结束

所以 `else` 中没有：

```bash
exit 1
```

如果希望调用这个脚本的其他程序也能检测失败，则可以明确返回非零状态：

```bash
if printf '%s\n%s\n' "$1" "$2" |
   grep -F "$3" |
   wc -l > count.txt
then
    printf '%s\n' "passed" > pipeline-verdict.txt
else
    printf '%s\n' "failed" > pipeline-verdict.txt
    exit 1
fi
```

此时：

```bash
./verify-stream.sh "apple" "banana" "orange"
echo "$?"
```

会得到：

```text
1
```

但这与前面的练习要求不同。练习要求的是：

```text
pipeline-verdict.txt → failed
count.txt            → 0
脚本最终状态          → 0
```

---

# 13. `pipefail`、`if` 和脚本退出状态的关系

可以把整个过程理解成三个层级：

```text
每个命令自己的状态
        ↓
pipeline 根据 pipefail 计算状态
        ↓
if 根据 pipeline 状态选择分支
        ↓
分支中的命令继续执行
        ↓
最后执行的命令影响脚本最终状态
```

例如：

```bash
set -o pipefail

if A | B | C; then
    D
else
    E
fi
```

假设：

```text
A → 0
B → 1
C → 0
E → 0
```

那么：

```text
B 返回 1
   ↓
pipefail
   ↓
A | B | C 返回 1
   ↓
if 进入 else
   ↓
E 返回 0
   ↓
if 返回 0
   ↓
如果没有后续命令，脚本返回 0
```

---

# 14. 核心总结

```bash
command1 | command2 | command3
```

默认情况下：

```text
pipeline status = command3 的状态
```

启用：

```bash
set -o pipefail
```

以后：

```text
全部为 0
→ pipeline 返回 0

存在非零状态
→ pipeline 返回最右侧的非零状态
```

`pipefail`：

* 不改变管道传递的数据；
* 不改变 `wc` 的统计结果；
* 只改变整个管道报告给 shell 的退出状态；
* 因而会影响 `if`、`&&`、`||` 等基于退出状态的控制逻辑。

而：

```bash
if pipeline; then
    ...
else
    ...
fi
```

会使用 pipeline 的状态选择分支，但之后执行的分支命令可以产生新的退出状态。

因此必须区分：

```text
命令的退出状态
≠ pipeline 的退出状态
≠ 整个脚本最终的退出状态
```

这是理解 Bash 错误处理时非常重要的基础。
