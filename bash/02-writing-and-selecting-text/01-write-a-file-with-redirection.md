# Bash 输出重定向与 `printf`：把文本写入文件

## 目标

在目录 `/home/learner/bash-lab` 中创建 `inventory.txt`，文件内容必须严格为两行：

```text
api
worker
```

并且**两行都以换行符结尾**。

正确命令：

```bash
cd /home/learner/bash-lab
printf 'api\nworker\n' > inventory.txt
```

---

## 1. 标准输出：命令默认把结果写到终端

大多数 Bash 命令会把正常输出写到 **standard output（标准输出，stdout）**。

例如：

```bash
printf 'api\nworker\n'
```

默认情况下，stdout 连接到终端，因此屏幕上会显示：

```text
api
worker
```

但这些内容并没有保存到文件中。

---

## 2. `>`：把标准输出重定向到文件

```bash
printf 'api\nworker\n' > inventory.txt
```

这里：

* `printf`：命令
* `'api\nworker\n'`：传给 `printf` 的格式字符串
* `>`：Bash 的输出重定向操作符
* `inventory.txt`：接收 stdout 的文件

执行过程可以理解为：

```text
printf
  │
  │ stdout
  ▼
inventory.txt
```

而不是：

```text
printf
  │
  ▼
终端
```

重要的是，`> inventory.txt` 属于 **Bash 语法**，不是 `printf` 的参数。

Bash 会先处理重定向，再运行命令。因此 `printf` 实际收到的只是：

```text
api\nworker\n
```

---

## 3. `>` 会覆盖原文件

`>` 的含义不是“继续写到文件末尾”，而是：

1. 打开文件；
2. 如果文件不存在，则创建；
3. 如果文件已经存在，则先清空；
4. 把命令的 stdout 写进去。

所以连续执行两次：

```bash
printf 'api\nworker\n' > inventory.txt
printf 'api\nworker\n' > inventory.txt
```

最终仍然只有：

```text
api
worker
```

不会变成四行。

### `>` 与 `>>` 的区别

| 操作符  | 行为      |
| ---- | ------- |
| `>`  | 覆盖文件    |
| `>>` | 追加到文件末尾 |

例如：

```bash
printf 'api\n' > inventory.txt
printf 'worker\n' >> inventory.txt
```

也会得到：

```text
api
worker
```

---

## 4. `printf` 不会自动添加换行

与某些命令不同，`printf` 只输出格式字符串明确要求的字符。

例如：

```bash
printf 'api'
```

输出的是：

```text
api
```

但结尾没有换行。

因此要创建两行内容，需要显式写出：

```bash
printf 'api\nworker\n'
```

其中：

```text
api\nworker\n
   ^^      ^^
```

每个 `\n` 表示一个换行。

逻辑上相当于：

```text
api + 换行
worker + 换行
```

因此文件中的字节结构可以理解为：

```text
a p i \n w o r k e r \n
```

最后一个 `\n` 也很重要，因为“第二行以换行符结束”和“文件刚好在 `worker` 后结束”在 Unix 文本文件中并不完全相同。

---

## 5. 为什么要使用单引号

正确写法：

```bash
printf 'api\nworker\n'
```

单引号 `'...'` 告诉 Bash：

> 单引号内部的字符基本保持原样，不要把 `\` 当成 Bash 自己的转义字符处理。

因此 Bash 会把：

```text
api\nworker\n
```

完整地交给 `printf`。

然后由 `printf` 解释：

```text
\n
```

为换行。

### 解析过程

```text
用户输入
printf 'api\nworker\n'

        ↓ Bash 解析

printf 收到
api\nworker\n

        ↓ printf 解释 \n

实际输出
api
worker
```

单引号本身不会被写入文件。

---

## 6. 为什么不应该省略引号

下面的写法：

```bash
printf api\nworker\n
```

看起来似乎也包含 `\n`，但结果完全不同。

原因是：**未加引号的反斜杠 `\` 会先被 Bash 处理。**

例如：

```text
\n
```

在这里不是先交给 `printf`，而是 Bash 会移除反斜杠，把后面的 `n` 保留下来。

因此：

```bash
api\nworker\n
```

最终可能变成传给 `printf` 的：

```text
apinworkern
```

于是：

```bash
printf api\nworker\n
```

会产生类似：

```text
apinworkern
```

而不是：

```text
api
worker
```

### 核心原则

```text
单引号保护 \n
       ↓
Bash 不处理它
       ↓
printf 收到 \n
       ↓
printf 将它解释为换行
```

---

## 7. 相对路径与当前工作目录

执行：

```bash
cd /home/learner/bash-lab
```

之后，当前工作目录就是：

```text
/home/learner/bash-lab
```

所以：

```bash
> inventory.txt
```

中的：

```text
inventory.txt
```

是一个**相对路径**，实际对应：

```text
/home/learner/bash-lab/inventory.txt
```

因此：

```bash
cd /home/learner/bash-lab
printf 'api\nworker\n' > inventory.txt
```

等价于直接使用绝对路径：

```bash
printf 'api\nworker\n' > /home/learner/bash-lab/inventory.txt
```

---

## 8. 验证文件内容

普通查看：

```bash
cat inventory.txt
```

应该显示：

```text
api
worker
```

如果需要确认换行等不可见字符，可以使用：

```bash
cat -A inventory.txt
```

典型输出：

```text
api$
worker$
```

其中 `$` 表示该行后面存在换行符，并不是文件中的真实字符。

---

## 9. 常见错误

### 错误一：忘记重定向

```bash
printf 'api\nworker\n'
```

结果只显示在终端，不会写入 `inventory.txt`。

---

### 错误二：忘记单引号

```bash
printf api\nworker\n > inventory.txt
```

Bash 可能先处理反斜杠，导致文件内容变成：

```text
apinworkern
```

---

### 错误三：漏掉最后一个 `\n`

```bash
printf 'api\nworker' > inventory.txt
```

虽然视觉上看起来仍然是两行文字，但最后一行没有换行符。

要求严格时，应写成：

```bash
printf 'api\nworker\n' > inventory.txt
```

---

### 错误四：误以为 `>` 会追加

```bash
printf 'api\nworker\n' > inventory.txt
```

每次执行都会先清空旧内容。

需要追加时才使用：

```bash
>>
```

---

## 10. 最终命令

```bash
cd /home/learner/bash-lab
printf 'api\nworker\n' > inventory.txt
```

最终文件：

```text
/home/learner/bash-lab/inventory.txt
```

内容严格为：

```text
api
worker
```

核心记忆：

```bash
printf 'FORMAT' > FILE
```

其中：

* `printf` 按格式字符串输出内容；
* `\n` 表示换行；
* 单引号保护反斜杠，使其能够交给 `printf`；
* `>` 把标准输出写入文件，并覆盖文件原有内容；
* 相对文件名会基于当前工作目录解析。
