# 学习笔记

## Bash 通配符（Glob）与文件批量匹配

在 Bash 中，如果需要一次处理一批具有相似文件名的文件，可以使用 **glob（文件名通配符）**，而不需要逐个写出文件名。

例如目录中有：

```text
/var/tmp/glob-lab/
├── app.log
├── api.log
└── app.txt
```

如果只想复制两个 `.log` 文件，可以使用：

```bash
cp /var/tmp/glob-lab/*.log /home/learner/stage/
```

这里：

```bash
*.log
```

会匹配：

```text
app.log
api.log
```

但不会匹配：

```text
app.txt
```

---

## 1. Glob 是 Bash 展开的，不是命令自己处理的

一个重要概念是：

> **通配符匹配由 Bash 在命令执行之前完成。**

例如：

```bash
cp /var/tmp/glob-lab/*.log /home/learner/stage/
```

Bash 会先把：

```bash
/var/tmp/glob-lab/*.log
```

展开成：

```bash
/var/tmp/glob-lab/app.log /var/tmp/glob-lab/api.log
```

因此真正交给 `cp` 的效果类似：

```bash
cp /var/tmp/glob-lab/app.log \
   /var/tmp/glob-lab/api.log \
   /home/learner/stage/
```

也就是说，**`cp` 本身通常根本看不到 `*.log` 这个模式。**

执行顺序可以理解为：

```text
输入命令
   ↓
Bash 展开 glob
   ↓
得到实际文件名列表
   ↓
执行 cp / echo / rm 等命令
```

---

## 2. 常用 Glob 通配符

### `*`：匹配任意长度字符

```bash
*.log
```

表示：

> 任意文件名 + `.log`

例如可以匹配：

```text
app.log
api.log
server.log
.log
```

`*` 可以匹配 **0 个或多个字符**。

---

### `?`：匹配恰好一个字符

例如：

```bash
file?.txt
```

可以匹配：

```text
file1.txt
fileA.txt
file_.txt
```

但不会匹配：

```text
file.txt
file12.txt
```

因为 `?` 必须对应 **正好一个字符**。

---

### `[...]`：匹配集合中的一个字符

例如：

```bash
[ap]pp.log
```

其中 `[ap]` 表示：

```text
a 或 p
```

它只匹配 **一个字符**。

常见形式：

```bash
[abc]
```

匹配：

```text
a
b
c
```

例如：

```bash
file[123].txt
```

可以匹配：

```text
file1.txt
file2.txt
file3.txt
```

---

## 3. 写 Glob 时要考虑“会不会匹配太多”

假设目录中有：

```text
app.log
api.log
app.txt
```

下面这个模式：

```bash
app.*
```

会匹配：

```text
app.log
app.txt
```

因此如果目标只是复制日志文件，它就太宽泛了。

更合适的是：

```bash
*.log
```

它根据扩展名筛选：

```text
app.log
api.log
```

核心原则：

> **执行批量操作之前，要能够明确预测 glob 会匹配哪些文件。**

尤其在搭配以下命令时更重要：

```bash
rm
mv
cp
chmod
```

因为匹配范围错误可能造成数据丢失或误操作。

---

## 4. 用 `echo` 安全预览 Glob

执行可能修改文件的命令之前，可以先使用：

```bash
echo PATTERN
```

查看 Bash 实际会展开成什么。

例如：

```bash
echo /var/tmp/glob-lab/*.log
```

可能输出：

```text
/var/tmp/glob-lab/api.log /var/tmp/glob-lab/app.log
```

这就是其他命令实际会收到的文件列表。

因此一个非常实用的习惯是：

```text
先 echo 预览
    ↓
确认匹配结果
    ↓
再执行 cp / mv / rm
```

例如：

```bash
echo /var/tmp/glob-lab/*.log
```

确认无误后：

```bash
cp /var/tmp/glob-lab/*.log /home/learner/stage/
```

特别是使用 `rm` 时：

```bash
echo *.log
```

确认后再执行：

```bash
rm *.log
```

---

## 5. 示例：只复制日志文件

目标：

```text
app.log  → 复制
api.log  → 复制
app.txt  → 保留
```

先检查匹配结果：

```bash
echo /var/tmp/glob-lab/*.log
```

创建目标目录：

```bash
mkdir -p /home/learner/stage
```

复制所有 `.log` 文件：

```bash
cp /var/tmp/glob-lab/*.log /home/learner/stage/
```

最终结果：

```text
/home/learner/stage/
├── app.log
└── api.log
```

而原目录中的：

```text
/var/tmp/glob-lab/app.txt
```

不会被复制。

---

## 6. 需要记住的结论

* Glob 用于根据文件名模式批量选择文件。
* **Glob 是 Bash 展开的，不是 `cp`、`rm` 等命令展开的。**
* `*`：匹配 0 个或多个字符。
* `?`：匹配恰好 1 个字符。
* `[...]`：匹配集合中的 1 个字符。
* 写模式时要尽量精确，避免匹配不需要的文件。
* 对批量操作不确定时，先执行：

```bash
echo PATTERN
```

查看实际匹配结果。
