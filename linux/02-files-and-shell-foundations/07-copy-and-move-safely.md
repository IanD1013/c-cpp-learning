# 学习笔记

## Linux 文件复制、移动与删除：`cp`、`mv`、`rm`

在 Linux 中，文件管理最常用的三个命令是：

| 命令   | 作用       | 原文件是否保留 |
| ---- | -------- | ------- |
| `cp` | 复制文件     | ✅ 保留    |
| `mv` | 移动或重命名文件 | ❌ 原路径消失 |
| `rm` | 删除文件     | ❌ 删除    |

---

## 1. `cp`：复制文件

基本语法：

```bash
cp SOURCE DESTINATION
```

例如：

```bash
cp draft.conf config/current.conf
```

执行后：

```text
draft.conf
config/current.conf
```

两个文件都会存在。

### 为什么配置文件迁移时常先用 `cp`？

因为复制完成后，可以先检查新文件是否正确，再决定是否删除原文件。

例如：

```bash
cp draft.conf config/current.conf
cat config/current.conf
```

确认内容正确后：

```bash
rm draft.conf
```

这种流程比直接 `mv` 更安全。

---

## 2. `mv`：移动或重命名

基本语法：

```bash
mv SOURCE DESTINATION
```

例如：

```bash
mv draft.conf config/current.conf
```

执行后：

```text
draft.conf               # 不再存在
config/current.conf      # 存在
```

`mv` 可以同时完成：

* 移动文件
* 修改文件名

因此，如果只关心最终状态，一个 `mv` 就足够：

```bash
mv draft.conf config/current.conf
```

但它的问题是：**执行之后源文件立即消失**。

如果目标文件有问题，就失去了先检查副本的机会。

---

## 3. `rm`：删除文件

基本语法：

```bash
rm FILE
```

例如：

```bash
rm draft.conf
```

`rm` 删除普通文件时通常**不会进入回收站**。

因此：

> **如果某个文件还可能需要验证、备份或恢复，不要过早执行 `rm`。**

一个安全习惯是：

```text
复制 → 验证 → 删除
```

也就是：

```bash
cp ...
cat ...
rm ...
```

---

## 4. 创建目标目录：`mkdir -p`

如果目标目录还不存在，可以先创建：

```bash
mkdir -p ~/lab/config
```

其中：

* `mkdir`：创建目录
* `-p`：如果上层目录已经存在，不报错；如果中间目录缺失，也一并创建

例如：

```bash
mkdir -p ~/lab/config
```

可以确保：

```text
~/lab/config/
```

存在。

---

## 5. 为什么应该复制文件，而不是重新输入内容？

假设原文件内容是：

```text
PORT=8080
```

并且结尾包含一个 newline（换行符）。

执行：

```bash
cp draft.conf config/current.conf
```

会直接复制原文件的字节内容，因此：

* 字符不会改变
* 换行符不会改变
* 不容易引入拼写错误

如果重新手动创建：

```bash
printf ...
```

则可能因为：

* 漏掉换行
* 多输入空格
* 拼错字符
* 多出额外换行

导致文件与原文件并不完全一致。

### 重要原则

当已有文件内容正确，只需要改变其位置或名称时：

> **优先复制或移动原文件，不要重新输入文件内容。**

---

## 6. 一个安全的文件“晋升”流程

假设：

```text
~/lab/draft.conf
```

是草稿配置，现在需要正式成为：

```text
~/lab/config/current.conf
```

推荐流程：

```bash
mkdir -p ~/lab/config
cp ~/lab/draft.conf ~/lab/config/current.conf
cat ~/lab/config/current.conf
rm ~/lab/draft.conf
```

逻辑是：

```text
创建目标目录
      ↓
复制草稿
      ↓
检查副本
      ↓
确认正确
      ↓
删除草稿
```

核心思想：

> **不可逆操作最后做。**

这里最不可逆的操作就是：

```bash
rm
```

---

## 7. 相对路径可以让命令更简洁

如果当前已经位于：

```bash
~/lab
```

那么可以写成：

```bash
mkdir -p config
cp draft.conf config/current.conf
cat config/current.conf
rm draft.conf
```

这和使用完整路径：

```bash
mkdir -p /home/learner/lab/config
cp /home/learner/lab/draft.conf /home/learner/lab/config/current.conf
```

效果相同。

---

## 核心结论

记住下面几个区别即可：

```text
cp = 复制，原文件还在
mv = 移动/重命名，原路径消失
rm = 删除文件
```

对于重要文件，更安全的操作流程是：

```text
cp → 验证 → rm
```

而不是：

```text
mv
```

因为前者允许你在删除源文件之前确认目标文件正确。

---

# 问题解答

## 目标

最终需要满足：

```text
/home/learner/lab/config/current.conf
```

存在，并且内容准确为：

```text
PORT=8080
```

同时：

```text
/home/learner/lab/draft.conf
```

不再存在。

## 推荐命令

如果当前位于任意目录：

```bash
mkdir -p ~/lab/config
cp ~/lab/draft.conf ~/lab/config/current.conf
cat ~/lab/config/current.conf
rm ~/lab/draft.conf
```

其中：

```bash
mkdir -p ~/lab/config
```

创建目标目录；

```bash
cp ~/lab/draft.conf ~/lab/config/current.conf
```

复制并同时将文件命名为 `current.conf`；

```bash
cat ~/lab/config/current.conf
```

检查复制后的内容，应看到：

```text
PORT=8080
```

确认正确后：

```bash
rm ~/lab/draft.conf
```

删除原来的草稿。

这样既满足最终状态，也遵循了更安全的：

```text
复制 → 验证 → 删除
```

操作顺序。
