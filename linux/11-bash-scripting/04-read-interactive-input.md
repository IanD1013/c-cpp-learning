# Bash 交互式输入：`read -r`、空值校验与安全引用

## 1. 使用 `read -r` 读取用户输入

Bash 中可以使用 `read` 从标准输入（stdin）读取一行内容：

```bash
read -r name
```

含义：

* `read`：读取一行输入；
* `-r`：不要把反斜杠 `\` 当作转义字符；
* `name`：把读取到的内容保存到变量 `name` 中。

例如用户输入：

```text
staging
```

之后：

```bash
$name
```

的值就是：

```text
staging
```

### 为什么通常应该使用 `-r`

如果省略 `-r`：

```bash
read name
```

`read` 会把反斜杠 `\` 解释为转义符。

例如用户输入的内容包含：

```text
foo\bar
```

脚本最终保存的内容可能不再与用户实际输入完全一致。

因此，读取普通文本时推荐：

```bash
read -r name
```

可以把它理解为：

> 尽可能原样读取用户输入。

---

## 2. 使用 `printf` 显示提示信息

常见的交互式输入写法是：

```bash
printf 'environment name: '
read -r name
```

运行后：

```text
environment name: staging
```

这里使用 `printf` 而不是：

```bash
echo 'environment name: '
```

一个重要原因是 `printf` 可以精确控制是否输出换行。

例如：

```bash
printf 'environment name: '
```

格式字符串中没有 `\n`，所以不会换行，光标会停在提示信息后面等待输入。

而：

```bash
printf 'environment name:\n'
```

则会输出换行。

---

## 3. 用户输入必须进行校验

交互式脚本不能假设用户一定输入有效内容。

例如用户看到：

```text
environment name:
```

直接按 Enter。

此时：

```bash
read -r name
```

通常仍然成功读取了一行，但：

```bash
$name
```

是空字符串。

如果直接写配置文件：

```bash
printf 'ENVIRONMENT=%s\n' "$name" > /home/learner/environment.conf
```

就会得到：

```text
ENVIRONMENT=
```

这通常不是想要的结果。

因此应该在真正执行操作之前进行校验。

---

## 4. 使用 `-z` 判断字符串是否为空

Bash 的 `[` 测试命令可以使用 `-z` 判断字符串长度是否为 0：

```bash
if [ -z "$name" ]; then
    ...
fi
```

含义是：

> 如果 `$name` 是空字符串，就执行 `then` 中的代码。

例如：

```bash
if [ -z "$name" ]; then
    printf 'environment name cannot be empty\n' >&2
    exit 1
fi
```

如果用户直接按 Enter：

1. `$name` 为空；
2. `[ -z "$name" ]` 成立；
3. 输出错误信息；
4. `exit 1` 结束脚本。

---

## 5. 为什么变量应该写成 `"$name"`

推荐：

```bash
[ -z "$name" ]
```

而不是：

```bash
[ -z $name ]
```

变量展开后，Shell 还可能进行单词拆分和其他处理。

假设：

```bash
name=""
```

没有引号时：

```bash
[ -z $name ]
```

展开后可能实际上变成：

```bash
[ -z ]
```

这不再是原本清晰的“对一个空字符串执行 `-z` 测试”。

某些情况下它恰好仍返回预期结果，但这是依赖 Shell 解析规则的偶然行为，而不是可靠代码。

因此基本原则是：

> Bash 中，只要变量需要作为一个完整字符串使用，通常都应该写成 `"$variable"`。

例如：

```bash
"$name"
"$file"
"$1"
"$HOME"
```

---

## 6. 错误信息应该写入 stderr

正常输出默认写到标准输出：

```bash
printf 'hello\n'
```

错误信息通常应该写到标准错误 stderr：

```bash
printf 'environment name cannot be empty\n' >&2
```

其中：

```bash
>&2
```

表示把本来要写入 stdout 的内容重定向到文件描述符 `2`，即 stderr。

常见文件描述符：

| 文件描述符 | 含义          |
| ----: | ----------- |
|   `0` | stdin，标准输入  |
|   `1` | stdout，标准输出 |
|   `2` | stderr，标准错误 |

这样可以让正常结果和错误信息分别处理。

---

## 7. 校验失败应该返回非零退出码

如果输入无效：

```bash
exit 1
```

表示脚本失败。

Unix/Linux 中通常约定：

```text
0       成功
非 0    失败
```

因此：

```bash
if [ -z "$name" ]; then
    printf 'environment name cannot be empty\n' >&2
    exit 1
fi
```

比只显示错误信息更完整。

调用脚本的其他程序也能通过退出状态判断它是否成功。

例如：

```bash
./set-environment

echo "$?"
```

如果脚本因为输入为空而执行：

```bash
exit 1
```

那么 `$?` 就是：

```text
1
```

---

## 8. 使用 `printf` 安全写入配置文件

假设环境名已经保存在：

```bash
name
```

可以写：

```bash
printf 'ENVIRONMENT=%s\n' "$name" > /home/learner/environment.conf
```

假如：

```bash
name="staging"
```

文件内容就是：

```text
ENVIRONMENT=staging
```

并且末尾有一个换行符。

### 为什么 `%s` 比直接拼接变量更清晰

推荐：

```bash
printf 'ENVIRONMENT=%s\n' "$name"
```

而不是：

```bash
printf "ENVIRONMENT=$name\n"
```

前一种方式明确区分：

* 格式字符串：`ENVIRONMENT=%s\n`
* 数据：`"$name"`

用户输入不会被当成 `printf` 格式字符串的一部分，因此更稳健。

---

## 9. 一个完整的交互式输入模式

一个典型 Bash 模式是：

```bash
#!/bin/bash

printf 'environment name: '
read -r name

if [ -z "$name" ]; then
    printf 'environment name cannot be empty\n' >&2
    exit 1
fi

printf 'ENVIRONMENT=%s\n' "$name" > /path/to/environment.conf
```

执行逻辑：

```text
显示提示
   ↓
读取一行输入
   ↓
检查是否为空
   ↓
为空 → 报错并退出
   ↓
非空 → 执行真正的文件写入
```

关键原则是：

> **先验证输入，再产生副作用。**

这样可以避免无效输入污染配置文件或触发错误操作。

---

## 10. 核心知识点

* `read -r variable`：读取一行输入，并尽量保持输入原样。
* `printf 'prompt: '`：可以显示不换行的输入提示。
* `[ -z "$variable" ]`：判断变量是否为空。
* Bash 变量展开通常应该加双引号：

  ```bash
  "$variable"
  ```
* 错误信息应写入 stderr：

  ```bash
  >&2
  ```
* 校验失败应使用非零退出码：

  ```bash
  exit 1
  ```
* 写配置文件时可以使用：

  ```bash
  printf 'KEY=%s\n' "$value" > file
  ```
* 用户输入属于不可信输入，应该在真正执行操作前检查。

---

# 实践：创建 `set-environment` Bash 脚本

## 任务

创建：

```text
/home/learner/bin/set-environment
```

要求：

1. 使用 Bash；
2. 文件权限为 `755`；
3. 文件由当前用户拥有；
4. 提示用户输入 environment name；
5. 使用 `read -r` 读取一行；
6. 拒绝空输入；
7. 空输入时：

   * 错误信息写入 stderr；
   * 返回非零退出码；
8. 有效输入时，把以下格式写入：

   ```text
   /home/learner/environment.conf
   ```
9. 输入：

   ```text
   staging
   ```

   后文件必须恰好包含：

   ```text
   ENVIRONMENT=staging
   ```

   并以换行符结束。

## 完整脚本

```bash
#!/bin/bash

printf 'environment name: '
read -r name

if [ -z "$name" ]; then
    printf 'environment name cannot be empty\n' >&2
    exit 1
fi

printf 'ENVIRONMENT=%s\n' "$name" > /home/learner/environment.conf
```

## 创建文件

```bash
mkdir -p /home/learner/bin
```

编辑：

```bash
nano /home/learner/bin/set-environment
```

写入：

```bash
#!/bin/bash

printf 'environment name: '
read -r name

if [ -z "$name" ]; then
    printf 'environment name cannot be empty\n' >&2
    exit 1
fi

printf 'ENVIRONMENT=%s\n' "$name" > /home/learner/environment.conf
```

保存后设置权限：

```bash
chmod 755 /home/learner/bin/set-environment
```

如果文件是当前用户创建的，通常已经由当前用户拥有。可以检查：

```bash
ls -l /home/learner/bin/set-environment
```

权限应类似：

```text
-rwxr-xr-x
```

其中：

```text
755 = rwxr-xr-x
```

即：

| 身份     | 权限     |
| ------ | ------ |
| owner  | 读、写、执行 |
| group  | 读、执行   |
| others | 读、执行   |

## 运行

```bash
/home/learner/bin/set-environment
```

看到：

```text
environment name:
```

输入：

```text
staging
```

然后检查文件：

```bash
cat /home/learner/environment.conf
```

应该得到：

```text
ENVIRONMENT=staging
```

也可以使用：

```bash
cat -A /home/learner/environment.conf
```

如果输出类似：

```text
ENVIRONMENT=staging$
```

这里的 `$` 表示这一行末尾存在换行符，并不是文件中真正保存了 `$` 字符。

## 空输入测试

再次运行：

```bash
/home/learner/bin/set-environment
```

直接按 Enter，应看到错误：

```text
environment name cannot be empty
```

脚本随后以失败状态结束。

可以检查：

```bash
echo "$?"
```

应得到：

```text
1
```

这说明脚本正确地拒绝了空输入，而不是继续写入无效配置。
