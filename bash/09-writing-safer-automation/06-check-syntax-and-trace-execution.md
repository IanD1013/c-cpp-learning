# Bash 语法检查、退出状态与可选调试跟踪

## 1. 为什么脚本一个命令都没有执行

Bash 在执行脚本之前，会先解析（parse）脚本的语法结构。

例如：

```bash
if [[ -n "$1" ]]; then
    printf '%s\n' "$1"
```

这里打开了一个 `if`，却缺少对应的：

```bash
fi
```

因此 Bash 读取到文件末尾时发现语法结构没有结束，会直接报语法错误并退出。

关键点：

> **语法解析失败时，脚本中的命令不会开始执行。**

所以即使错误发生在文件最后，看起来前面的 `printf` 是正确的，它也不会运行。

---

## 2. 使用 `bash -n` 只检查语法

检查 Bash 脚本语法：

```bash
bash -n debug-me.sh
```

`-n` 表示：

> 读取并解析脚本，但不执行其中的命令。

### 检查成功

```bash
bash -n debug-me.sh
```

如果语法正确：

* 不输出任何内容
* 退出状态为 `0`

可以查看：

```bash
echo $?
```

结果：

```text
0
```

### 检查失败

如果脚本缺少 `fi`：

```bash
bash -n debug-me.sh
```

可能看到：

```text
debug-me.sh: line 6: syntax error: unexpected end of file
```

退出状态非零。

### 为什么 `bash -n` 很有用

它可以把两个问题分开：

```text
脚本能不能被 Bash 正确理解？
        ↓
脚本实际执行是否正确？
```

先解决语法，再调试运行逻辑，比直接不断执行脚本更容易定位问题。

---

## 3. 修复 `debug-me.sh`

这个练习还要求：

* 保留打印第一个参数的逻辑
* 支持 `DEBUG=1` 时开启执行跟踪
* `DEBUG=0` 或没有设置时正常运行

可以写成：

```bash
#!/usr/bin/env bash
[[ "${DEBUG:-0}" == "1" ]] && set -x

if [[ -n "${1:-}" ]]; then
    printf '%s\n' "$1"
fi
```

保存为：

```text
/home/learner/bash-lab/debug-me.sh
```

### 关键修复

最重要的是最后的：

```bash
fi
```

Bash 的结构必须完整：

```bash
if condition; then
    commands
fi
```

---

# `DEBUG` 环境变量与 `set -x`

## 4. `set -x` 是什么

执行：

```bash
set -x
```

之后，Bash 会在执行每条命令之前，把展开后的命令打印到 **standard error（stderr，标准错误）**。

例如脚本：

```bash
printf '%s\n' "$1"
```

执行：

```bash
DEBUG=1 bash debug-me.sh api
```

标准输出仍然是：

```text
api
```

但 stderr 还会出现类似：

```text
+ [[ -n api ]]
+ printf '%s\n' api
```

这叫做 **execution trace（执行跟踪）**。

它非常适合调试：

* 变量最终展开成什么值
* 哪个 `if` 分支被执行
* 实际执行了哪些命令
* 命令收到哪些参数

---

## 5. 不应该永久开启 `set -x`

如果直接写：

```bash
set -x
```

那么每次运行都会产生大量调试信息。

更好的方式是：

```bash
[[ "${DEBUG:-0}" == "1" ]] && set -x
```

意思是：

```text
DEBUG 的值是否等于 1？
        │
        ├── 是 → set -x
        │
        └── 否 → 不开启 trace
```

---

## 6. `${DEBUG:-0}` 的意义

语法：

```bash
${DEBUG:-0}
```

表示：

> 如果 `DEBUG` 已设置且非空，就使用它；否则使用默认值 `0`。

因此：

```bash
DEBUG=1
```

得到：

```text
1
```

而：

```bash
DEBUG=0
```

得到：

```text
0
```

如果完全没有设置 `DEBUG`，也得到：

```text
0
```

这样下面的比较始终安全：

```bash
[[ "${DEBUG:-0}" == "1" ]]
```

---

# Wrapper Script：根据语法检查结果决定是否继续

## 7. `check-debug.sh` 的目标

wrapper 脚本需要完成：

```text
bash -n debug-me.sh
        │
        ├── 成功
        │    ├── syntax-status.txt ← syntax-ok
        │    └── 运行 debug-me.sh api
        │             ↓
        │       debug-output.txt ← api
        │
        └── 失败
             ├── 不写 syntax-ok
             └── wrapper exit 2
```

---

## 8. 推荐的 `check-debug.sh`

创建：

```text
/home/learner/bash-lab/check-debug.sh
```

内容：

```bash
#!/usr/bin/env bash

if ! bash -n debug-me.sh; then
    rm -f syntax-status.txt debug-output.txt
    exit 2
fi

printf '%s\n' 'syntax-ok' > syntax-status.txt
bash debug-me.sh api > debug-output.txt
```

其中最核心的检查命令严格是：

```bash
bash -n debug-me.sh
```

---

## 9. `if ! command; then` 的含义

这里：

```bash
if ! bash -n debug-me.sh; then
```

首先执行：

```bash
bash -n debug-me.sh
```

然后读取它的退出状态。

正常情况下：

```text
0       → 成功
非 0    → 失败
```

`!` 会反转这个判断：

```text
bash -n 成功 → ! 后为 false → 不进入 then
bash -n 失败 → ! 后为 true  → 进入 then
```

因此：

```bash
if ! bash -n debug-me.sh; then
    exit 2
fi
```

可以理解为：

> 如果语法检查失败，立即以状态码 `2` 结束 wrapper。

---

# 输出重定向

## 10. 创建 `syntax-status.txt`

语法检查成功后：

```bash
printf '%s\n' 'syntax-ok' > syntax-status.txt
```

最终文件：

```text
syntax-ok
```

`>` 表示把 stdout 写入文件。

---

## 11. 创建 `debug-output.txt`

执行：

```bash
bash debug-me.sh api > debug-output.txt
```

这里：

```text
bash
└── debug-me.sh
      └── $1 = api
```

脚本执行：

```bash
printf '%s\n' "$1"
```

因此 stdout 是：

```text
api
```

由于：

```bash
> debug-output.txt
```

最终：

```text
debug-output.txt
```

内容就是：

```text
api
```

---

# 标准输出与标准错误

## 12. 为什么 `set -x` 不会污染 `debug-output.txt`

下面运行：

```bash
DEBUG=1 bash debug-me.sh api > debug-output.txt
```

实际上存在两条不同的输出流：

```text
printf
   ↓
stdout ───────→ debug-output.txt

set -x trace
   ↓
stderr ───────→ 终端
```

因此 `debug-output.txt` 仍然只有：

```text
api
```

trace 不会自动写进去，因为 `set -x` 使用的是 stderr。

如果想分别捕获：

```bash
DEBUG=1 bash debug-me.sh api >output.txt 2>trace.txt
```

那么：

```text
output.txt
```

包含：

```text
api
```

而：

```text
trace.txt
```

包含类似：

```text
+ [[ -n api ]]
+ printf '%s\n' api
```

---

# 完成练习的操作步骤

进入实验目录：

```bash
cd /home/learner/bash-lab
```

## 第一步：修复 `debug-me.sh`

确保内容类似：

```bash
#!/usr/bin/env bash
[[ "${DEBUG:-0}" == "1" ]] && set -x

if [[ -n "${1:-}" ]]; then
    printf '%s\n' "$1"
fi
```

---

## 第二步：确认语法正确

```bash
bash -n debug-me.sh
```

正常情况下：

```text
没有任何输出
```

检查状态：

```bash
echo $?
```

应该是：

```text
0
```

---

## 第三步：创建 wrapper

`check-debug.sh`：

```bash
#!/usr/bin/env bash

if ! bash -n debug-me.sh; then
    rm -f syntax-status.txt debug-output.txt
    exit 2
fi

printf '%s\n' 'syntax-ok' > syntax-status.txt
bash debug-me.sh api > debug-output.txt
```

---

## 第四步：运行 wrapper

```bash
bash check-debug.sh
```

检查状态：

```bash
echo $?
```

应该得到：

```text
0
```

---

## 第五步：检查两个输出文件

```bash
cat syntax-status.txt
```

应该是：

```text
syntax-ok
```

然后：

```bash
cat debug-output.txt
```

应该是：

```text
api
```

---

## 第六步：确认正常运行没有 trace

```bash
DEBUG=0 bash debug-me.sh api
```

stdout：

```text
api
```

stderr 不应该有 `+ ...` trace。

没有设置 `DEBUG` 也一样：

```bash
bash debug-me.sh api
```

结果：

```text
api
```

---

## 第七步：确认 `DEBUG=1` 会产生 trace

```bash
DEBUG=1 bash debug-me.sh api
```

正常输出仍然包含：

```text
api
```

同时 stderr 会出现类似：

```text
+ [[ -n api ]]
+ printf '%s\n' api
```

这证明：

```bash
[[ "${DEBUG:-0}" == "1" ]] && set -x
```

工作正常。

---

# 最终应有的两个脚本

## `debug-me.sh`

```bash
#!/usr/bin/env bash
[[ "${DEBUG:-0}" == "1" ]] && set -x

if [[ -n "${1:-}" ]]; then
    printf '%s\n' "$1"
fi
```

## `check-debug.sh`

```bash
#!/usr/bin/env bash

if ! bash -n debug-me.sh; then
    rm -f syntax-status.txt debug-output.txt
    exit 2
fi

printf '%s\n' 'syntax-ok' > syntax-status.txt
bash debug-me.sh api > debug-output.txt
```

成功运行：

```bash
bash check-debug.sh
```

之后目录中应该有：

```text
syntax-status.txt
debug-output.txt
```

内容分别为：

```text
syntax-ok
```

和：

```text
api
```

# 核心知识总结

| 语法                   | 作用                  |
| -------------------- | ------------------- |
| `bash -n script.sh`  | 只检查 Bash 语法，不执行脚本   |
| `$?`                 | 上一条命令的退出状态          |
| `0`                  | 命令成功                |
| 非 `0`                | 命令失败                |
| `if ! command; then` | 当命令失败时进入分支          |
| `exit 2`             | 让当前脚本立即结束，并返回状态码 2  |
| `set -x`             | 输出 Bash 执行跟踪        |
| `${DEBUG:-0}`        | DEBUG 未设置或为空时使用 `0` |
| `&&`                 | 左边成功才执行右边           |
| `>`                  | 将 stdout 写入文件       |
| `2>`                 | 将 stderr 写入文件       |
| `fi`                 | 结束 Bash 的 `if` 结构   |

最重要的调试思路是：

```text
先用 bash -n 确认脚本能够被解析
              ↓
再运行脚本检查程序逻辑
              ↓
需要更详细信息时用 DEBUG=1 + set -x
```

这样可以把 **语法错误、运行时逻辑错误和调试信息** 清楚地分开。
