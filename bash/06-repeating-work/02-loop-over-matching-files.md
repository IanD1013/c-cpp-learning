# Bash：使用 `for` 循环安全处理通配符匹配的文件

## 目标

编写一个 Bash 脚本，遍历 `inbox/` 中所有 `.log` 文件，并复制到 `logs/`：

```text
bash-lab/
├── collect.sh
├── inbox/
│   ├── api.log
│   ├── worker.log
│   └── notes.txt
└── logs/
```

要求：

* `api.log` → `logs/api.log`
* `worker.log` → `logs/worker.log`
* `notes.txt` 不复制
* 如果 `inbox/` 中没有任何 `.log` 文件，脚本仍然正常结束，退出状态为 `0`
* 不能把字面量 `inbox/*.log` 当成文件名处理

---

## 1. Bash 的通配符展开

下面的代码：

```bash
for file in inbox/*.log
do
  ...
done
```

其中：

```text
inbox/*.log
```

是一个 **glob（文件名通配模式）**。

`*` 表示匹配任意字符串，因此如果目录中存在：

```text
inbox/api.log
inbox/worker.log
inbox/notes.txt
```

Bash 会先把：

```bash
inbox/*.log
```

展开成：

```text
inbox/api.log inbox/worker.log
```

然后 `for` 循环实际相当于遍历：

```bash
for file in inbox/api.log inbox/worker.log
```

因此两次循环中：

```text
第一次：file=inbox/api.log
第二次：file=inbox/worker.log
```

`notes.txt` 不符合 `*.log`，所以不会进入循环。

---

## 2. 默认行为的问题：没有匹配文件时，模式不会消失

Bash 默认情况下，如果 glob 没有匹配任何文件：

```bash
inbox/*.log
```

不会变成空列表，而是保留成原始字符串：

```text
inbox/*.log
```

因此：

```bash
for file in inbox/*.log
do
  echo "$file"
done
```

如果目录中没有 `.log` 文件，循环仍然会运行一次：

```text
file="inbox/*.log"
```

这通常不是我们想要的行为。

例如：

```bash
cp -- "$file" logs/
```

最终实际上会尝试：

```bash
cp -- 'inbox/*.log' logs/
```

由于通常不存在一个真的叫做 `*.log` 的文件，因此复制会失败。

---

## 3. `nullglob`：没有匹配结果时变成空列表

可以开启 Bash 的 `nullglob` 选项：

```bash
shopt -s nullglob
```

开启后，如果：

```bash
inbox/*.log
```

没有匹配任何文件，它会展开成：

```text
空列表
```

于是：

```bash
for file in inbox/*.log
do
  ...
done
```

会执行 **0 次循环**。

因此：

```bash
shopt -s nullglob

for file in inbox/*.log
do
  echo "$file"
done
```

在没有 `.log` 文件时什么都不会做，而且脚本可以正常结束。

### 两种行为对比

| 情况         | 默认 Bash          | 开启 `nullglob` |
| ---------- | ---------------- | ------------- |
| 有匹配文件      | 展开成实际文件          | 展开成实际文件       |
| 没有匹配文件     | 保留 `inbox/*.log` | 展开为空列表        |
| `for` 循环次数 | 1 次              | 0 次           |

对于“处理所有匹配文件，如果没有就什么也不做”的脚本，`nullglob` 非常实用。

---

## 4. 为什么 glob 不能加引号

正确：

```bash
for file in inbox/*.log
```

错误：

```bash
for file in "inbox/*.log"
```

因为引号会禁止文件名展开。

写成：

```bash
"inbox/*.log"
```

之后，Bash 会把它当作普通字符串：

```text
inbox/*.log
```

所以即使目录中存在：

```text
inbox/api.log
inbox/worker.log
```

循环也只会执行一次。

### 原则

> **需要 Bash 展开通配符时，不要给 glob 加引号。**

因此：

```bash
inbox/*.log
```

保持不加引号。

---

## 5. 为什么变量 `"$file"` 反而必须加引号

glob 不加引号：

```bash
for file in inbox/*.log
```

但是使用循环变量时应该写：

```bash
"$file"
```

例如：

```bash
cp -- "$file" logs/
```

而不是：

```bash
cp -- $file logs/
```

原因是文件名可能包含空格。

假设有：

```text
inbox/server error.log
```

变量内容就是：

```text
inbox/server error.log
```

使用：

```bash
cp -- "$file" logs/
```

Bash 会把整个路径作为一个参数：

```text
"inbox/server error.log"
```

如果不加引号：

```bash
cp -- $file logs/
```

则可能被拆成两个参数：

```text
inbox/server
error.log
```

导致命令出错。

### 记忆规则

```text
glob 用于展开 → 不加引号
展开后的变量 → 通常加双引号
```

也就是：

```bash
for file in inbox/*.log
do
  cp -- "$file" logs/
done
```

---

## 6. `cp -- "$file" logs/` 的含义

```bash
cp -- "$file" logs/
```

可以拆成：

### `cp`

复制文件：

```bash
cp source destination
```

### `--`

```bash
--
```

表示：

> 后面的内容全部作为普通参数处理，不再识别为命令选项。

这是很多 Unix 命令的防御性写法。

### `"$file"`

当前循环处理的文件，例如：

```text
inbox/api.log
```

双引号保证路径作为一个完整参数传给 `cp`。

### `logs/`

目标目录。

因此：

```bash
cp -- "$file" logs/
```

可能实际执行为：

```bash
cp -- "inbox/api.log" logs/
```

最终得到：

```text
logs/api.log
```

---

## 7. 完整的 `collect.sh`

```bash
#!/usr/bin/env bash

shopt -s nullglob

for file in inbox/*.log
do
  cp -- "$file" logs/
done
```

### 每一部分的作用

```bash
#!/usr/bin/env bash
```

使用环境中的 Bash 来执行脚本。

```bash
shopt -s nullglob
```

让没有匹配结果的 glob 展开为空列表。

```bash
for file in inbox/*.log
```

寻找 `inbox/` 下所有 `.log` 文件，并逐个赋值给 `file`。

```bash
do
```

开始循环体。

```bash
cp -- "$file" logs/
```

把当前 `.log` 文件复制到 `logs/`。

```bash
done
```

结束循环。

---

## 8. 运行脚本

应当从 `bash-lab` 目录运行，因为脚本使用的是相对路径：

```bash
cd /home/learner/bash-lab
bash collect.sh
```

如果：

```text
inbox/api.log
inbox/worker.log
inbox/notes.txt
```

存在，那么运行后：

```text
logs/api.log
logs/worker.log
```

会被创建或覆盖，而：

```text
notes.txt
```

不会被复制，因为它不匹配：

```text
*.log
```

---

## 9. 没有 `.log` 文件时会发生什么

假设：

```text
inbox/
└── notes.txt
```

因为已经启用：

```bash
shopt -s nullglob
```

所以：

```bash
inbox/*.log
```

没有匹配时会变成空列表。

因此：

```bash
for file in inbox/*.log
```

没有任何值可遍历，循环体执行：

```text
0 次
```

也就是说：

```bash
cp -- "$file" logs/
```

根本不会执行。

脚本正常结束，不会错误地尝试复制：

```text
inbox/*.log
```

---

## 10. 常见错误

### 错误一：没有启用 `nullglob`

```bash
for file in inbox/*.log
do
  cp -- "$file" logs/
done
```

如果没有 `.log` 文件，`file` 可能变成：

```text
inbox/*.log
```

导致复制失败。

正确：

```bash
shopt -s nullglob
```

---

### 错误二：给 glob 加引号

```bash
for file in "inbox/*.log"
```

这样会禁止 glob 展开。

应该写：

```bash
for file in inbox/*.log
```

---

### 错误三：不给变量加引号

```bash
cp -- $file logs/
```

对于包含空格等特殊字符的文件名可能出错。

应该写：

```bash
cp -- "$file" logs/
```

---

### 错误四：直接复制整个目录

```bash
cp inbox/* logs/
```

这样会把：

```text
notes.txt
```

也复制过去。

题目只要求 `.log` 文件，因此应该使用：

```bash
inbox/*.log
```

---

## 11. 核心规律

安全地遍历一组可能不存在的匹配文件，可以使用：

```bash
shopt -s nullglob

for file in directory/*.ext
do
  command "$file"
done
```

关键点只有三个：

1. **`nullglob`**：没有匹配文件时，让 glob 变成空列表，而不是保留通配符文本。
2. **glob 不加引号**：例如 `inbox/*.log`，这样 Bash 才会进行文件名展开。
3. **变量要加引号**：例如 `"$file"`，保证展开后的每个文件路径作为一个完整参数传递。

最终脚本：

```bash
#!/usr/bin/env bash

shopt -s nullglob

for file in inbox/*.log
do
  cp -- "$file" logs/
done
```
