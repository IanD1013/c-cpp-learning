# Bash 位置参数：让脚本接收命令行参数

## 1. 为什么脚本需要参数

如果把服务名直接写死在脚本里：

```bash
printf 'Deploying api\n' > deploy.log
```

这个脚本永远只能输出：

```text
Deploying api
```

更好的做法是让**运行脚本的人在执行时传入服务名**：

```bash
./deploy.sh api
```

此时脚本可以读取 `api`，从而根据输入决定部署哪个服务。

例如：

```bash
./deploy.sh api
./deploy.sh worker
./deploy.sh frontend
```

都可以复用同一个脚本。

---

## 2. Bash 的位置参数（Positional Parameters）

运行脚本时，脚本名称后面的参数会依次保存到 Bash 的**位置参数**中。

例如：

```bash
./deploy.sh api production
```

对应关系是：

| 参数   | 值             |
| ---- | ------------- |
| `$0` | `./deploy.sh` |
| `$1` | `api`         |
| `$2` | `production`  |

其中：

* `$0`：脚本自身的名称或调用路径
* `$1`：第一个命令行参数
* `$2`：第二个命令行参数
* `$3`：第三个命令行参数
* 依此类推

例如：

```bash
#!/usr/bin/env bash

printf 'Service: %s\n' "$1"
printf 'Environment: %s\n' "$2"
```

执行：

```bash
./deploy.sh api production
```

输出：

```text
Service: api
Environment: production
```

---

## 3. `$0` 不是第一个用户参数

容易误解的一点是：

```bash
$0
```

不是第一个传入参数。

它表示**脚本本身**。

例如：

```bash
./deploy.sh api
```

通常：

```text
$0 = ./deploy.sh
$1 = api
```

因此，如果需要读取用户输入的第一个参数，应使用：

```bash
"$1"
```

而不是：

```bash
"$0"
```

---

## 4. 位置参数不是环境变量

位置参数：

```bash
$1
$2
$3
```

和环境变量：

```bash
$HOME
$PATH
$USER
```

是不同的机制。

例如：

```bash
./deploy.sh api
```

这里的 `api` 只属于这一次脚本执行：

```bash
$1 = api
```

脚本结束之后，这个位置参数也就不存在了。

而环境变量通常来自当前 Shell 环境，例如：

```bash
echo "$HOME"
```

可能输出：

```text
/home/learner
```

即使父 Shell 中存在：

```bash
export SERVICE=api
```

它也不会自动变成：

```bash
$1
```

只有真正写在脚本名称后面的参数才会进入位置参数：

```bash
./deploy.sh api
```

---

## 5. 为什么参数展开通常要加双引号

推荐：

```bash
"$1"
```

而不是：

```bash
$1
```

原因是参数可能包含空格。

例如：

```bash
./deploy.sh "payment api"
```

脚本中：

```bash
printf '%s\n' "$1"
```

会把：

```text
payment api
```

作为**一个完整参数**处理。

双引号是 Bash 中非常重要的习惯：

```bash
"$1"
"$2"
"$variable"
```

一般情况下，只要不明确需要 Bash 进行单词分割或通配符展开，就应该给变量和位置参数加双引号。

---

## 6. `$1x`、`${1}` 和 `${12}`

### `$1x`

例如：

```bash
echo "$1x"
```

Bash 会理解为：

```text
$1 + 字符 x
```

如果：

```bash
$1 = api
```

那么：

```bash
echo "$1x"
```

输出：

```text
apix
```

因为位置参数的简写形式 `$1` 这里只读取一个数字。

---

### `${1}`

也可以使用花括号：

```bash
"${1}"
```

对于 1～9 号位置参数：

```bash
"$1"
"${1}"
```

效果相同。

花括号通常在需要明确变量边界时更清楚。

例如：

```bash
echo "${1}x"
```

明确表示：

```text
第一个参数 + x
```

---

### `${12}`

如果需要读取第 10 个及之后的位置参数，必须使用花括号。

例如：

```bash
"${12}"
```

表示第十二个参数。

而：

```bash
$12
```

并不表示第十二个参数，它实际上相当于：

```text
$1 后面跟一个普通字符 2
```

所以：

```bash
${10}
${11}
${12}
```

才是正确写法。

---

## 7. `printf` 如何使用位置参数

例如：

```bash
printf 'Ready: %s\n' "$2"
```

其中：

```text
'Ready: %s\n'
```

是格式字符串。

含义：

| 内容       | 含义          |
| -------- | ----------- |
| `Ready:` | 普通文本        |
| `%s`     | 放入一个字符串     |
| `\n`     | 换行          |
| `"$2"`   | 提供给 `%s` 的值 |

如果执行：

```bash
./script.sh api production
```

那么：

```bash
$2 = production
```

输出：

```text
Ready: production
```

---

## 8. 输出重定向 `>`

命令：

```bash
printf 'Deploying %s\n' "$1" > deploy.log
```

中的：

```bash
> deploy.log
```

表示：

> 不把 `printf` 的输出显示在终端，而是写入 `deploy.log` 文件。

例如：

```bash
printf 'Deploying %s\n' "api" > deploy.log
```

执行后：

```bash
cat deploy.log
```

得到：

```text
Deploying api
```

---

## 9. `>` 会覆盖原文件

单个：

```bash
>
```

会在写入之前**清空原文件**。

例如第一次执行：

```bash
./deploy.sh api
```

文件内容：

```text
Deploying api
```

然后执行：

```bash
./deploy.sh worker
```

文件内容会变成：

```text
Deploying worker
```

不会变成：

```text
Deploying api
Deploying worker
```

因为 `>` 是覆盖写入。

如果需要追加内容，则使用：

```bash
>>
```

例如：

```bash
printf 'Deploying %s\n' "$1" >> deploy.log
```

---

# 完整的 `deploy.sh`

创建文件：

```bash
#!/usr/bin/env bash
printf 'Deploying %s\n' "$1" > deploy.log
```

其中：

```bash
#!/usr/bin/env bash
```

是 shebang，表示使用 Bash 解释这个脚本。

第二行：

```bash
printf 'Deploying %s\n' "$1" > deploy.log
```

完成三件事：

1. 从 `$1` 读取第一个命令行参数；
2. 生成类似 `Deploying api` 的文本；
3. 把结果覆盖写入 `deploy.log`。

---

## 10. 运行脚本

如果脚本还没有执行权限：

```bash
chmod +x deploy.sh
```

然后运行：

```bash
./deploy.sh api
```

查看结果：

```bash
cat deploy.log
```

应该得到：

```text
Deploying api
```

---

## 11. 验证脚本确实没有写死服务名

运行：

```bash
./deploy.sh worker
```

再查看：

```bash
cat deploy.log
```

应该得到：

```text
Deploying worker
```

这说明服务名来自：

```bash
"$1"
```

而不是硬编码在脚本中。

如果最终要求文件保存 `api`，再次运行：

```bash
./deploy.sh api
```

此时：

```bash
cat deploy.log
```

应为：

```text
Deploying api
```

---

## 12. 完整操作示例

```bash
cd /home/learner/bash-lab

cat > deploy.sh <<'SCRIPT'
#!/usr/bin/env bash
printf 'Deploying %s\n' "$1" > deploy.log
SCRIPT

chmod +x deploy.sh

./deploy.sh api

cat deploy.log
```

最终输出：

```text
Deploying api
```

也可以验证动态参数：

```bash
./deploy.sh worker
cat deploy.log
```

得到：

```text
Deploying worker
```

最后恢复为：

```bash
./deploy.sh api
```

---

## 13. 常见错误

### 错误 1：把服务名写死

```bash
printf 'Deploying api\n' > deploy.log
```

这样：

```bash
./deploy.sh worker
```

仍然会得到：

```text
Deploying api
```

正确做法：

```bash
printf 'Deploying %s\n' "$1" > deploy.log
```

---

### 错误 2：使用 `$0`

```bash
printf 'Deploying %s\n' "$0" > deploy.log
```

可能得到：

```text
Deploying ./deploy.sh
```

因为 `$0` 是脚本自身。

应该使用：

```bash
"$1"
```

---

### 错误 3：忘记给参数加引号

不推荐：

```bash
printf '%s\n' $1
```

推荐：

```bash
printf '%s\n' "$1"
```

双引号可以防止包含空格的参数被错误拆分。

---

### 错误 4：把 `>` 和 `>>` 混淆

```bash
> deploy.log
```

覆盖旧内容。

```bash
>> deploy.log
```

追加到旧内容后面。

如果要求 `deploy.log` 始终只有最新一次部署记录，应使用：

```bash
>
```

---

# 核心总结

运行：

```bash
./deploy.sh api
```

时：

```text
$0 = ./deploy.sh
$1 = api
```

因此可以写：

```bash
#!/usr/bin/env bash
printf 'Deploying %s\n' "$1" > deploy.log
```

实现：

```bash
./deploy.sh api
```

写入：

```text
Deploying api
```

而：

```bash
./deploy.sh worker
```

写入：

```text
Deploying worker
```

核心原则是：

> Bash 脚本不需要把运行时数据写死在代码中，可以通过 `$1`、`$2` 等位置参数读取调用者传入的命令行参数。
