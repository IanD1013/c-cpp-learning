# Bash 中单引号与双引号：变量展开和输出重定向

## 目标

假设 Bash 变量 `name` 的值是：

```bash
name=Ada
```

需要创建文件：

```text
/home/learner/bash-lab/quotes.txt
```

最终内容必须恰好是：

```text
Hello, Ada
Hello, $name
```

这两个结果的关键区别在于：**双引号允许变量展开，单引号会阻止变量展开。**

---

## 1. 双引号：变量会展开

在 Bash 中：

```bash
"$name"
```

会发生 **参数展开（parameter expansion）**。

例如：

```bash
name=Ada
printf '%s\n' "Hello, $name"
```

Bash 在执行 `printf` 之前，会先把：

```text
$name
```

替换成变量保存的值：

```text
Ada
```

因此 `printf` 实际收到的参数相当于：

```text
Hello, Ada
```

输出：

```text
Hello, Ada
```

### 为什么通常推荐给变量加双引号

写成：

```bash
"$name"
```

而不是：

```bash
$name
```

可以避免变量值中的空格被 Bash 拆成多个参数。

例如：

```bash
name="Ada Lovelace"
printf '%s\n' "$name"
```

能够完整输出：

```text
Ada Lovelace
```

因此，一个非常实用的 Bash 习惯是：

> **需要变量值时，默认写成 `"$variable"`。**

---

## 2. 单引号：内容完全按字面处理

单引号：

```bash
'Hello, $name'
```

会关闭变量展开。

因此：

```bash
name=Ada
printf '%s\n' 'Hello, $name'
```

输出仍然是：

```text
Hello, $name
```

这里：

```text
$name
```

不会被替换成 `Ada`。

在单引号内部，诸如 `$` 这样的字符通常都会被当作普通文本处理。

因此可以简单记忆：

| 写法               | `$name` 是否展开 | 结果示例           |
| ---------------- | ------------ | -------------- |
| `"$name"`        | 是            | `Ada`          |
| `'${name}'`      | 否            | `${name}`      |
| `'Hello, $name'` | 否            | `Hello, $name` |

---

## 3. 引号本身不会写入输出

例如：

```bash
printf '%s\n' "Hello, $name"
printf '%s\n' 'Hello, $name'
```

输出分别是：

```text
Hello, Ada
Hello, $name
```

文件中不会出现：

```text
"Hello, Ada"
'Hello, $name'
```

原因是单引号和双引号都是 **Shell 语法的一部分**，用于告诉 Bash 如何解释文本；它们本身通常不会作为参数内容传递给程序。

---

## 4. `>` 与 `>>` 的区别

Shell 可以通过重定向把命令输出写入文件。

### `>`：覆盖文件

```bash
printf '%s\n' "Hello, $name" > quotes.txt
```

`>` 会：

1. 创建 `quotes.txt`（如果不存在）；
2. 如果文件已经存在，则清空原内容；
3. 把当前命令的标准输出写进去。

执行后：

```text
Hello, Ada
```

---

### `>>`：追加到文件末尾

```bash
printf '%s\n' 'Hello, $name' >> quotes.txt
```

`>>` 不会删除原内容，而是把新输出追加到文件末尾。

因此先执行 `>`，再执行 `>>`：

```bash
printf '%s\n' "Hello, $name" > quotes.txt
printf '%s\n' 'Hello, $name' >> quotes.txt
```

最终得到：

```text
Hello, Ada
Hello, $name
```

可以记成：

```text
>   overwrite   覆盖
>>  append      追加
```

---

## 5. 完整操作

进入工作目录：

```bash
cd /home/learner/bash-lab
```

定义变量：

```bash
name=Ada
```

写入第一行：

```bash
printf '%s\n' "Hello, $name" > quotes.txt
```

这里使用双引号，因此：

```text
$name → Ada
```

再追加第二行：

```bash
printf '%s\n' 'Hello, $name' >> quotes.txt
```

这里使用单引号，因此 `$name` 保持原样。

完整命令：

```bash
cd /home/learner/bash-lab
name=Ada
printf '%s\n' "Hello, $name" > quotes.txt
printf '%s\n' 'Hello, $name' >> quotes.txt
```

最终：

```bash
cat quotes.txt
```

应该得到：

```text
Hello, Ada
Hello, $name
```

---

## 6. `printf '%s\n'` 是什么意思

命令：

```bash
printf '%s\n' "Hello, $name"
```

可以拆成：

```text
printf
│
├── '%s\n'          格式字符串
│    ├── %s         输出一个字符串
│    └── \n         输出换行
│
└── "Hello, $name"  要输出的字符串
```

因此：

```bash
printf '%s\n' "Hello, $name"
```

本质上是：

> 输出 `"Hello, $name"` 展开后的内容，然后换行。

使用固定格式字符串：

```bash
printf '%s\n' "$value"
```

通常也比：

```bash
printf "$value"
```

更安全、更容易预测。

---

## 7. 常见错误

### 错误 1：第二行使用双引号

```bash
printf '%s\n' "Hello, $name" >> quotes.txt
```

因为双引号允许变量展开，所以得到：

```text
Hello, Ada
```

而不是：

```text
Hello, $name
```

如果需要保留 `$name` 原样，应使用：

```bash
printf '%s\n' 'Hello, $name'
```

---

### 错误 2：第二次仍然使用 `>`

```bash
printf '%s\n' "Hello, $name" > quotes.txt
printf '%s\n' 'Hello, $name' > quotes.txt
```

第二个 `>` 会清空第一行，因此最终文件只剩：

```text
Hello, $name
```

需要保留第一行时，第二次应使用：

```bash
>>
```

---

### 错误 3：误以为引号会进入文件

命令：

```bash
printf '%s\n' 'Hello, $name'
```

不会输出：

```text
'Hello, $name'
```

而是：

```text
Hello, $name
```

单引号只是 Bash 用来控制解析方式的语法。

---

## 8. 核心记忆

```bash
name=Ada
```

### 想要变量的值

```bash
echo "$name"
```

得到：

```text
Ada
```

### 想要字面上的 `$name`

```bash
echo '$name'
```

得到：

```text
$name
```

### 写文件时

```bash
>   # 覆盖
>>  # 追加
```

最核心的规则可以记成：

> **双引号 `"..."`：保护字符串整体，但仍然允许 `$变量` 展开。**
> **单引号 `'...'`：把内部内容当作字面文本，不进行 `$变量` 展开。**
