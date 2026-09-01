# Bash 标准输入输出与重定向

## 1. 每个命令都有三个标准流

Linux/Bash 中，每个进程默认都有三个标准数据流：

| 流        | 文件描述符 | 用途   | 默认位置 |
| -------- | ----: | ---- | ---- |
| `stdin`  |   `0` | 标准输入 | 键盘   |
| `stdout` |   `1` | 正常输出 | 终端   |
| `stderr` |   `2` | 错误输出 | 终端   |

例如：

```bash
printf 'hello\n'
```

产生的是 **stdout**。

而：

```bash
ls /missing
```

由于目录不存在，产生的是 **stderr**。

虽然默认情况下两者都会显示在终端里，但它们实际上是两个独立的数据流，因此可以分别重定向。

---

## 2. stdout 重定向：`>` 与 `>>`

### `>`：覆盖写入

```bash
printf 'hello\n' > file.txt
```

含义：

> 将命令的 stdout 写入 `file.txt`。

如果文件已经存在，**原内容会被清空并覆盖**。

例如：

```bash
printf 'start\n' > audit.log
```

执行后：

```text
start
```

### `>>`：追加写入

```bash
printf 'finish\n' >> audit.log
```

不会删除已有内容，而是在文件末尾追加：

```text
start
finish
```

核心区别：

```text
>   覆盖
>>  追加
```

---

## 3. stderr 重定向：`2>`

因为 stderr 的文件描述符是 `2`，所以：

```bash
command 2> error.log
```

表示：

> 将 stderr 写入 `error.log`。

例如：

```bash
ls /missing 2> audit.err
```

错误不会显示在终端，而会保存到：

```text
audit.err
```

其中会包含类似：

```text
ls: cannot access '/missing': No such file or directory
```

注意：

```bash
ls /missing > audit.err
```

通常不能捕获这个错误，因为 `>` 默认只重定向 **stdout**。

---

## 4. 为什么数字 `2` 很重要

Shell 的重定向本质上操作的是文件描述符：

```text
0 = stdin
1 = stdout
2 = stderr
```

因此：

```bash
command > file
```

实际上可以理解为：

```bash
command 1> file
```

而：

```bash
command 2> file
```

明确表示重定向 stderr。

所以：

```bash
> file
```

和：

```bash
2> file
```

重定向的并不是同一个数据流。

---

## 5. 常见重定向形式

| 写法         | 含义          |
| ---------- | ----------- |
| `> file`   | stdout 覆盖写入 |
| `>> file`  | stdout 追加写入 |
| `2> file`  | stderr 覆盖写入 |
| `2>> file` | stderr 追加写入 |

例如：

```bash
command >> output.log 2>> error.log
```

表示：

* 正常输出追加到 `output.log`
* 错误输出追加到 `error.log`

---

## 6. 如何选择 `>`、`>>` 和 `2>`

可以先问自己两个问题：

### 这个输出是正常输出还是错误输出？

正常输出：

```bash
>
>>
```

错误输出：

```bash
2>
2>>
```

### 文件原有内容应该保留吗？

不保留：

```bash
>
2>
```

保留并继续添加：

```bash
>>
2>>
```

---

## 7. 一个典型日志场景

假设希望：

1. 新建日志并写入 `start`
2. 单独记录一个错误
3. 在日志末尾追加 `finish`

可以使用：

```bash
printf 'start\n' > audit.log
ls /missing 2> audit.err
printf 'finish\n' >> audit.log
```

最终：

```text
audit.log
├── start
└── finish
```

而错误信息单独保存在：

```text
audit.err
```

这样就体现了 Unix/Linux 的一个重要思想：

> **正常输出和错误输出是两个独立的数据流，可以分别处理。**

---

## 8. 一个很实用的性质：命令可以安全重跑

如果第一条命令使用：

```bash
> audit.log
```

那么每次重新执行整个流程时，`audit.log` 都会先被清空。

例如：

```bash
printf 'start\n' > audit.log
printf 'finish\n' >> audit.log
```

无论之前文件里有什么，最终都会重新得到：

```text
start
finish
```

因此在需要得到**确定文件内容**的脚本中，经常先使用 `>` 初始化文件，再使用 `>>` 继续追加。

---

## 快速记忆

```text
0 = stdin
1 = stdout
2 = stderr
```

```bash
>     stdout 覆盖
>>    stdout 追加
2>    stderr 覆盖
2>>   stderr 追加
```

最重要的区别：

> **stdout 是正常结果，stderr 是错误信息；它们虽然默认都显示在终端，但实际上是独立的数据流。**
