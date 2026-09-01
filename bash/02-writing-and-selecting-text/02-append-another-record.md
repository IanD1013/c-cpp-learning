# Bash 追加重定向：用 `>>` 在文件末尾添加内容

## 目标场景

假设文件：

```text
/home/learner/bash-lab/inventory.txt
```

已经包含 10 行资产标签，现在需要在**不修改原有 10 行内容和顺序**的前提下，在文件末尾追加：

```text
probe-3364
```

并保证新记录以换行符结束。

正确命令：

```bash
cd /home/learner/bash-lab
printf 'probe-3364\n' >> inventory.txt
```

---

## `>` 和 `>>` 的核心区别

Bash 中，`>` 和 `>>` 都是**标准输出重定向（stdout redirection）**，但打开目标文件的方式不同。

| 操作符  | 行为   | 原文件内容      |
| ---- | ---- | ---------- |
| `>`  | 覆盖写入 | 会先被清空      |
| `>>` | 追加写入 | 保留，新内容写到末尾 |

### `>`：覆盖文件

例如：

```bash
printf 'probe-3364\n' > inventory.txt
```

执行时，Bash 会先：

1. 打开 `inventory.txt`
2. 将文件截断为 **0 字节**
3. 把命令的标准输出连接到该文件
4. 执行 `printf`

因此原来的 10 行会全部消失，最终只剩：

```text
probe-3364
```

关键点是：

> `>` 清空文件发生在命令真正输出内容之前。

所以如果文件里有需要保留的数据，不应该使用 `>`。

---

## `>>`：在文件末尾追加

正确做法是：

```bash
printf 'probe-3364\n' >> inventory.txt
```

`>>` 同样会把标准输出发送到文件，但它不会先清空文件，而是从文件末尾继续写。

假设原文件是：

```text
asset-1001
asset-1002
asset-1003
...
asset-1010
```

执行：

```bash
printf 'probe-3364\n' >> inventory.txt
```

结果变成：

```text
asset-1001
asset-1002
asset-1003
...
asset-1010
probe-3364
```

原来的内容：

* 不需要读出来；
* 不需要复制；
* 不需要重新写入；
* 顺序不会因为追加操作而改变。

这也是 `>>` 最重要的用途：

> **保留已有内容，只在末尾增加新的输出。**

---

## 重定向改变的是“输出去哪里”

考虑下面两个命令：

```bash
printf 'probe-3364\n'
```

和：

```bash
printf 'probe-3364\n' >> inventory.txt
```

`printf` 本身执行的事情没有改变：它仍然产生相同的输出。

区别只是输出目的地不同：

```text
printf
   │
   │ 标准输出 stdout
   ▼
终端
```

加入 `>>` 后：

```text
printf
   │
   │ 标准输出 stdout
   ▼
inventory.txt 文件末尾
```

因此：

```bash
>>
```

不是 `printf` 的参数，而是 **Bash Shell 的语法**。

Bash 会先处理重定向，再运行 `printf`。

---

## 为什么使用 `printf 'probe-3364\n'`

`printf` 不会自动添加换行符。

例如：

```bash
printf 'probe-3364'
```

只会输出：

```text
probe-3364
```

不会自动在后面加入 newline。

为了让它成为完整的一行记录，需要显式写：

```bash
printf 'probe-3364\n'
```

其中：

```text
\n
```

表示换行符。

因此写入文件的实际字节结构相当于：

```text
probe-3364<newline>
```

这很重要，因为文本文件通常约定：

> 每一行，包括最后一行，都应该以换行符结束。

---

## 为什么使用单引号

推荐：

```bash
printf 'probe-3364\n'
```

而不是随意依赖 Shell 对特殊字符的解释。

单引号：

```bash
'...'
```

会让 Bash 基本按字面意义保留其中的字符。

因此：

```bash
'probe-3364\n'
```

会把 `\n` 原样交给 `printf`，然后由 `printf` 将它解释为换行符。

可以把过程理解为：

```text
Bash
  ↓
保留 \n
  ↓
printf 收到 probe-3364\n
  ↓
printf 将 \n 解释成换行符
```

---

## 工作目录与相对路径

如果当前位于：

```text
/home/learner/bash-lab
```

那么：

```bash
inventory.txt
```

实际上解析为：

```text
/home/learner/bash-lab/inventory.txt
```

因此可以执行：

```bash
cd /home/learner/bash-lab
printf 'probe-3364\n' >> inventory.txt
```

这里的 `inventory.txt` 是**相对路径**。

对应的绝对路径是：

```bash
/home/learner/bash-lab/inventory.txt
```

可以用：

```bash
pwd
```

确认当前目录：

```bash
pwd
```

预期输出：

```text
/home/learner/bash-lab
```

---

## 写入前后如何检查

修改重要文件之前，最好先检查当前内容。

### 查看文件

```bash
cat inventory.txt
```

`cat` 会把文件内容输出到终端，但不会修改文件。

推荐流程：

```bash
cat inventory.txt

printf 'probe-3364\n' >> inventory.txt

cat inventory.txt
```

这样可以直观看到：

```text
修改前：10 行
修改后：11 行
```

这种“修改前检查 + 修改后验证”的习惯很重要，因为：

```bash
>
```

一旦误用，文件可能已经在命令执行之前被清空。

---

## 使用 `wc -l` 检查行数

如果不关心具体内容，只想检查文件有多少行，可以使用：

```bash
wc -l inventory.txt
```

例如修改前：

```text
10 inventory.txt
```

追加后：

```text
11 inventory.txt
```

完整流程：

```bash
wc -l inventory.txt
printf 'probe-3364\n' >> inventory.txt
wc -l inventory.txt
```

---

## 推荐的安全操作流程

```bash
cd /home/learner/bash-lab

cat inventory.txt
wc -l inventory.txt

printf 'probe-3364\n' >> inventory.txt

cat inventory.txt
wc -l inventory.txt
```

最终应满足：

* 文件共有 11 行；
* 前 10 行完全保持原样；
* 第 11 行是 `probe-3364`；
* `probe-3364` 后面存在换行符。

---

## 常见错误

### 错误 1：使用 `>` 导致原文件被清空

错误：

```bash
printf 'probe-3364\n' > inventory.txt
```

结果：

```text
原来的 10 行全部丢失
```

正确：

```bash
printf 'probe-3364\n' >> inventory.txt
```

---

### 错误 2：忘记最后的换行符

不推荐：

```bash
printf 'probe-3364' >> inventory.txt
```

这样写入的最后一行没有 newline。

应该使用：

```bash
printf 'probe-3364\n' >> inventory.txt
```

---

### 错误 3：在错误目录中使用相对路径

如果当前目录不是：

```text
/home/learner/bash-lab
```

那么：

```bash
>> inventory.txt
```

可能会修改另一个同名文件，甚至创建一个新的文件。

可以先检查：

```bash
pwd
```

或者直接使用绝对路径：

```bash
printf 'probe-3364\n' >> /home/learner/bash-lab/inventory.txt
```

---

### 错误 4：认为必须先读取原文件才能追加

追加并不需要：

```bash
cat inventory.txt
```

`cat` 只是用于人工检查。

真正追加数据时：

```bash
printf 'probe-3364\n' >> inventory.txt
```

Bash 会直接从文件末尾写入，不需要知道原文件里面具体是什么。

---

## `>` 与 `>>` 的记忆方法

可以简单记成：

```text
>   = replace / overwrite
>>  = append
```

或者：

```text
>   清空后写
>>  保留后追加
```

典型用途：

```bash
# 创建或覆盖文件
printf 'first\n' > file.txt

# 在已有文件后继续追加
printf 'second\n' >> file.txt
```

最终：

```text
first
second
```

如果第二条命令也使用 `>`：

```bash
printf 'second\n' > file.txt
```

最终只会剩：

```text
second
```

---

## 核心知识总结

```bash
printf 'probe-3364\n' >> inventory.txt
```

可以拆成三个关键知识点：

```text
printf 'probe-3364\n'
│
├─ printf：产生输出
├─ \n：在记录末尾加入换行符
│
>> inventory.txt
│
└─ >>：把标准输出追加到文件末尾，不删除已有内容
```

最重要的区别：

```bash
>   # 覆盖：原内容会丢失
>>  # 追加：原内容保留
```

当目标是**在已有文件末尾增加内容，同时保留原数据**时，应使用：

```bash
>>
```
