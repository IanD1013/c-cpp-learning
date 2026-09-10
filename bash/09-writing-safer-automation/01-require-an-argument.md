# Bash 必需参数校验：`${parameter:?message}`

## 1. 问题场景

假设脚本 `require-env.sh` 必须接收一个环境名称：

```bash
./require-env.sh prod
```

这里：

```text
$1 = prod
```

脚本最终把环境名称写入：

```text
selected-env.txt
```

如果直接这样写：

```bash
environment=$1
printf '%s\n' "$environment" > selected-env.txt
```

那么调用者没有传参数时：

```bash
./require-env.sh
```

`$1` 会展开为空字符串，最终文件可能变成：

```text
```

也就是只有一个换行符。

问题在于：**缺少必需参数时，脚本不应该继续运行，更不应该创建一个内容为空的结果文件。**

---

## 2. `${1:?message}`：要求参数必须存在且非空

Bash 提供了参数展开语法：

```bash
${parameter:?message}
```

例如：

```bash
environment=${1:?environment is required}
```

它的逻辑可以理解为：

```text
如果 $1 有非空值：
    返回这个值
否则：
    向 stderr 输出错误
    终止脚本
```

因此：

```bash
environment=${1:?environment is required}
```

在执行：

```bash
./require-env.sh prod
```

时相当于：

```bash
environment=prod
```

但执行：

```bash
./require-env.sh
```

时，Bash 会直接报错并终止脚本。

---

## 3. 完整脚本

文件：

```text
/home/learner/bash-lab/require-env.sh
```

内容：

```bash
#!/usr/bin/env bash

environment=${1:?environment is required}
printf '%s\n' "$environment" > selected-env.txt
```

这里最重要的是执行顺序：

```text
environment=${1:?environment is required}
        │
        ├── 参数有效 → 继续
        │
        └── 参数缺失/为空 → Bash 报错并终止
                                  ↓
                   printf 根本不会执行
                                  ↓
                   selected-env.txt 不会被创建
```

因此，**参数校验发生在文件重定向之前**。

---

## 4. 三种输入情况

### 情况一：完全没有传参数

```bash
./require-env.sh
```

此时 `$1` 是 **unset（未设置）**。

`${1:?environment is required}` 会失败。

Bash 会向标准错误 `stderr` 输出类似：

```text
./require-env.sh: line 3: 1: environment is required
```

退出状态：

```bash
echo $?
```

通常得到：

```text
1
```

并且：

```text
selected-env.txt
```

不会被创建。

---

### 情况二：显式传入空字符串

```bash
./require-env.sh ""
```

这和完全没有参数有一个细微区别：

```text
./require-env.sh       → $1 未设置
./require-env.sh ""    → $1 已设置，但是值为空字符串
```

但是 `${1:?message}` 会同时拒绝：

```text
unset
```

和：

```text
empty string
```

所以同样会报错并终止：

```text
./require-env.sh: line 3: 1: environment is required
```

`selected-env.txt` 仍然不会创建。

---

### 情况三：传入有效值

```bash
./require-env.sh prod
```

此时：

```bash
$1 = prod
```

所以：

```bash
environment=${1:?environment is required}
```

得到：

```bash
environment=prod
```

接下来执行：

```bash
printf '%s\n' "$environment" > selected-env.txt
```

文件内容：

```text
prod
```

可以检查：

```bash
cat selected-env.txt
```

输出：

```text
prod
```

---

## 5. 为什么 `:?` 中的冒号很重要

Bash 有两种非常相似的写法：

```bash
${1:?message}
```

和：

```bash
${1?message}
```

但它们检查的条件不同。

| 写法              | 参数未设置 | 参数为空字符串 |
| --------------- | ----: | ------: |
| `${1:?message}` |  ❌ 报错 |    ❌ 报错 |
| `${1?message}`  |  ❌ 报错 |    ✅ 接受 |

关键区别就是：

```text
:
```

冒号表示：

> 把“未设置”和“值为空”都视为没有有效值。

因此：

```bash
./require-env.sh ""
```

对于：

```bash
${1:?message}
```

会失败。

但对于：

```bash
${1?message}
```

却会通过。

对于通常的“必填参数”，一般需要：

```bash
${1:?message}
```

因为空字符串通常也不能算有效输入。

---

## 6. `${1:?message}` 与 `${1:-default}` 的区别

这两个语法都处理“参数为空或不存在”，但是目的完全不同。

### 提供默认值

```bash
environment=${1:-dev}
```

意思是：

> 如果 `$1` 不存在或为空，就使用 `dev`。

例如：

```bash
./script.sh
```

最终：

```text
environment=dev
```

适合**可选参数**。

---

### 强制要求参数

```bash
environment=${1:?environment is required}
```

意思是：

> 如果 `$1` 不存在或为空，立即报错。

适合**必需参数**。

可以记成：

```text
:-    → 没有就给默认值
:?    → 没有就报错
```

---

## 7. 为什么这里不需要写成 `"${1:?message}"`

下面的写法：

```bash
environment=${1:?environment is required}
```

是安全的。

虽然在普通命令中通常应该给变量加双引号：

```bash
printf '%s\n' "$environment"
```

但：

```bash
environment=...
```

属于 Bash 的**变量赋值上下文**。

赋值右侧不会进行普通的 word splitting（单词拆分）和 pathname expansion（文件名通配展开）。

例如：

```bash
./require-env.sh "production east"
```

即使代码是：

```bash
environment=${1:?environment is required}
```

变量仍然完整保存：

```text
production east
```

不会被拆成两个单词。

因此这里：

```bash
environment=${1:?environment is required}
```

已经足够。

不过后面真正使用变量时仍然应该加引号：

```bash
printf '%s\n' "$environment"
```

---

## 8. 为什么必须先校验，再进行重定向

推荐：

```bash
environment=${1:?environment is required}
printf '%s\n' "$environment" > selected-env.txt
```

执行顺序是：

```text
校验参数
↓
校验成功
↓
创建/覆盖文件
```

如果校验失败：

```text
校验参数
↓
Bash 终止
↓
printf 不运行
↓
> selected-env.txt 不发生
```

这体现了一个重要的 Shell 脚本原则：

> **先验证输入，再产生副作用。**

文件创建、文件覆盖、网络请求、部署、删除操作等都属于副作用。

应该尽量采用：

```text
验证
↓
计算
↓
执行副作用
```

而不是：

```text
先修改系统
↓
后来才发现参数有问题
```

---

## 9. `printf` 为什么优先于 `echo`

脚本使用：

```bash
printf '%s\n' "$environment"
```

而不是：

```bash
echo "$environment"
```

`printf` 的行为更加明确：

```bash
printf '%s\n' "$environment"
```

其中：

```text
%s   → 输出一个字符串
\n   → 输出换行符
```

因此：

```bash
environment=prod
```

产生：

```text
prod\n
```

也就是文件中正常的一行：

```text
prod
```

在 Shell 脚本中，需要可靠控制输出格式时通常更推荐 `printf`。

---

## 10. 如何完整测试脚本

测试前先删除旧文件：

```bash
rm -f selected-env.txt
```

### 测试缺失参数

```bash
./require-env.sh
echo $?
ls selected-env.txt
```

预期：

```text
出现 parameter 1 的错误信息
退出状态为 1
selected-env.txt 不存在
```

---

### 测试空参数

```bash
rm -f selected-env.txt

./require-env.sh ""
echo $?
ls selected-env.txt
```

预期：

```text
出现 parameter 1 的错误信息
退出状态为 1
selected-env.txt 不存在
```

---

### 测试正常参数

```bash
./require-env.sh prod
echo $?
cat selected-env.txt
```

预期：

```text
0
prod
```

---

## 11. 常见误区

### 误区一：认为 `$1` 不存在会自动报错

不会。

默认情况下：

```bash
environment=$1
```

如果没有 `$1`，它通常只是变成空字符串：

```text
environment=""
```

脚本仍然继续执行。

所以必需参数必须主动检查。

---

### 误区二：`${1?message}` 和 `${1:?message}` 一样

不一样。

```bash
${1?message}
```

只检查：

```text
unset
```

而：

```bash
${1:?message}
```

同时检查：

```text
unset
empty
```

对于必填参数，通常应该使用：

```bash
${1:?message}
```

---

### 误区三：报错后只停止当前赋值

在普通非交互 Bash 脚本中，`${parameter:?message}` 失败是参数展开错误，会导致脚本终止。

因此：

```bash
environment=${1:?environment is required}
printf '%s\n' "$environment" > selected-env.txt
```

如果第一行失败，第二行不会执行。

---

### 误区四：把默认值和必填校验混为一谈

```bash
${1:-prod}
```

不是校验。

它表示：

```text
没有值 → 自动使用 prod
```

而：

```bash
${1:?environment is required}
```

才表示：

```text
没有值 → 拒绝继续运行
```

---

## 12. 最终推荐写法

```bash
#!/usr/bin/env bash

environment=${1:?environment is required}
printf '%s\n' "$environment" > selected-env.txt
```

核心行为：

```text
./require-env.sh
→ 拒绝：$1 未设置

./require-env.sh ""
→ 拒绝：$1 为空

./require-env.sh prod
→ 接受
→ selected-env.txt 内容为 prod
```

最重要的记忆点：

```bash
${parameter:?message}
```

表示：

> **这个参数是必需的；如果它未设置或为空，就由 Bash 直接报错并终止脚本。**

而：

```text
${parameter:-default}   → 没有值就提供默认值
${parameter:?message}   → 没有值就拒绝继续
```

对于部署环境、文件路径、用户名、目标服务器等“没有就不能安全执行”的输入，`${parameter:?message}` 是非常简洁有效的 Bash 参数校验方式。
