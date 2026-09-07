# 使用 `tar` 打包并在其他位置恢复目录树

## 1. `tar` 解决什么问题

复制一批文件时，仅仅移动文件内容并不够，还需要保留目录结构，例如：

```text
/etc/app/
├── config.ini
├── logging.conf
└── plugins/
    └── auth.conf
```

`tar` 的核心作用是：

> **把一个目录树打包成单个归档文件，同时保存其中的相对路径和层级结构。**

例如可以将：

```text
app/
├── config.ini
└── plugins/
    └── auth.conf
```

打包成：

```text
app-config.tar
```

以后解包时，原来的目录结构会重新建立。

### `tar` 默认不负责压缩

需要区分两个概念：

* **归档（archive）**：把多个文件和目录组织成一个文件。
* **压缩（compression）**：减少数据占用的空间。

普通 `.tar` 文件只是归档，并没有压缩。

```text
目录树
  ↓ tar
app-config.tar
```

而 `.tar.gz`、`.tar.xz` 等格式是在归档之后又进行了压缩。

---

## 2. 最重要的三个操作

`tar` 最常用的三个操作是：

| 选项   | 含义      | 用途     |
| ---- | ------- | ------ |
| `-c` | create  | 创建归档   |
| `-t` | list    | 查看归档内容 |
| `-x` | extract | 解包归档   |
| `-f` | file    | 指定归档文件 |

### 创建归档

```bash
tar -cf archive.tar directory
```

可以理解为：

```text
-c  create：创建
-f  file：接下来指定归档文件
```

因此：

```bash
tar -cf app.tar app
```

表示：

> 把 `app` 打包到 `app.tar` 中。

---

## 3. 查看归档内容：`tar -tf`

在解包之前，最好先检查归档中保存了哪些路径：

```bash
tar -tf app-config.tar
```

例如可能看到：

```text
app/
app/config.ini
app/plugins/
app/plugins/auth.conf
```

这里显示的不只是文件名，更重要的是：

> **这些路径就是以后解包时 `tar` 要重新创建的路径。**

因此，检查 `tar -tf` 的输出可以避免把文件解包到意料之外的位置。

---

## 4. `-C`：控制归档内部保存的路径

`tar` 中非常重要但容易误解的选项是：

```text
-C directory
```

它的意思是：

> **先切换到指定目录，再执行后续操作。**

可以把它理解成临时执行了一次：

```bash
cd directory
```

但不会真正改变当前 Shell 的工作目录。

---

## 5. 为什么打包时推荐使用 `-C`

假设需要打包：

```text
/etc/app
```

希望归档内部保存成：

```text
app/
app/config.ini
```

可以执行：

```bash
tar -cf ~/app-config.tar -C /etc app
```

执行逻辑相当于：

```bash
cd /etc
tar -cf ~/app-config.tar app
```

于是归档内部保存的是：

```text
app/
app/config.ini
```

而不是完整的：

```text
/etc/app/config.ini
```

### 核心原则

> `tar` 保存什么路径，解包时就会重新创建什么路径。

所以打包时应该主动设计归档内部的目录结构。

---

## 6. `-C` 的位置为什么重要

命令：

```bash
tar -cf ~/app-config.tar -C /etc app
```

应按顺序理解：

```text
tar
├── -c                  创建归档
├── -f ~/app-config.tar 归档文件
├── -C /etc             先进入 /etc
└── app                  打包 /etc/app
```

这里的：

```text
app
```

是相对于：

```text
/etc
```

而言的，因此实际打包的是：

```text
/etc/app
```

但归档内部只记录：

```text
app/...
```

---

## 7. 解包时的 `-C`

`-C` 不仅能用于创建归档，也可以用于解包。

例如：

```bash
tar -xf app-config.tar -C /home/learner/restore
```

这里的意思是：

> 将归档内容解包到 `/home/learner/restore`。

假设归档内部保存的是：

```text
app/
app/config.ini
```

解包后得到：

```text
/home/learner/restore/
└── app/
    └── config.ini
```

因此最终文件路径是：

```text
/home/learner/restore/app/config.ini
```

### 打包和解包时 `-C` 的区别

| 场景        | `-C` 的作用    |
| --------- | ----------- |
| 创建归档 `-c` | 从哪个目录开始读取文件 |
| 解包归档 `-x` | 把文件解包到哪个目录  |

---

## 8. 一个完整的工作流程

目标：

```text
源目录：
/etc/app

归档：
/home/learner/app-config.tar

恢复目录：
/home/learner/restore

最终文件：
/home/learner/restore/app/config.ini
```

整个过程可以分成四步。

### 第一步：创建归档

```bash
tar -cf /home/learner/app-config.tar -C /etc app
```

归档内部会保存类似：

```text
app/
app/config.ini
```

---

### 第二步：查看归档内容

```bash
tar -tf /home/learner/app-config.tar
```

确认输出类似：

```text
app/
app/config.ini
```

这一步很重要，因为它可以确认解包后会创建什么目录结构。

---

### 第三步：创建目标目录

```bash
mkdir -p /home/learner/restore
```

`mkdir -p` 的作用是：

* 创建目录；
* 如果父目录不存在，也一起创建；
* 如果目录已经存在，通常不会报错。

---

### 第四步：解包

```bash
tar -xf /home/learner/app-config.tar -C /home/learner/restore
```

最终结构：

```text
/home/learner/restore/
└── app/
    └── config.ini
```

---

## 9. 常见误区

### 误区一：认为 `.tar` 就是压缩文件

不是。

```text
.tar
```

首先表示的是**归档**。

归档和压缩是两个不同操作。

---

### 误区二：忽略归档内部的路径

例如看到：

```bash
tar -cf backup.tar ...
```

只关心有没有成功生成 `backup.tar`，却没有查看：

```bash
tar -tf backup.tar
```

这是危险的，因为解包位置最终由：

```text
解包目录 + 归档内部路径
```

共同决定。

例如归档中存在：

```text
app/config.ini
```

执行：

```bash
tar -xf backup.tar -C /tmp/restore
```

最终得到：

```text
/tmp/restore/app/config.ini
```

---

### 误区三：把 `-C` 理解成“指定文件”

`-C` 不是选择某个文件，而是：

> **改变 `tar` 后续操作所使用的工作目录。**

例如：

```bash
tar -cf backup.tar -C /etc app
```

其中：

```text
/etc   ← 工作目录
app    ← 要打包的对象
```

---

## 10. 最实用的记忆方式

创建：

```bash
tar -cf ARCHIVE -C PARENT DIRECTORY
```

例如：

```bash
tar -cf app-config.tar -C /etc app
```

查看：

```bash
tar -tf ARCHIVE
```

例如：

```bash
tar -tf app-config.tar
```

解包：

```bash
tar -xf ARCHIVE -C DESTINATION
```

例如：

```bash
tar -xf app-config.tar -C /home/learner/restore
```

可以记成：

```text
-c = create   创建
-t = table    查看目录表
-x = extract  解包
-f = file     归档文件
-C = change directory
```

---

# 实践任务解答：打包 `/etc/app` 并恢复到其他目录

## 问题

将：

```text
/etc/app
```

归档到：

```text
/home/learner/app-config.tar
```

然后：

1. 在解包前查看归档内容；
2. 创建 `/home/learner/restore`；
3. 将归档解包到该目录；
4. 最终使原来的配置文件位于：

```text
/home/learner/restore/app/config.ini
```

## 完整命令

```bash
tar -cf /home/learner/app-config.tar -C /etc app

tar -tf /home/learner/app-config.tar

mkdir -p /home/learner/restore

tar -xf /home/learner/app-config.tar -C /home/learner/restore
```

## 为什么这样写

第一条：

```bash
tar -cf /home/learner/app-config.tar -C /etc app
```

先让 `tar` 进入：

```text
/etc
```

然后打包：

```text
app
```

因此归档内部保存：

```text
app/
app/config.ini
```

而不是让 `/etc` 成为归档路径的一部分。

第二条：

```bash
tar -tf /home/learner/app-config.tar
```

用于检查归档内部保存的路径。

第三条：

```bash
mkdir -p /home/learner/restore
```

创建解包目标目录。

第四条：

```bash
tar -xf /home/learner/app-config.tar -C /home/learner/restore
```

把归档中的：

```text
app/config.ini
```

放到：

```text
/home/learner/restore
```

下面，因此最终得到：

```text
/home/learner/restore/app/config.ini
```

整个路径变化可以概括为：

```text
/etc/app/config.ini
        │
        │ 打包时：-C /etc
        ▼
app/config.ini
        │
        │ 解包时：-C /home/learner/restore
        ▼
/home/learner/restore/app/config.ini
```
