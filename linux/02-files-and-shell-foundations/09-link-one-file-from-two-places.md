# 学习笔记

## Linux 符号链接与硬链接：`ln` / `ln -s`

Linux 中的“链接”可以让一个文件通过多个路径被访问，而不必创建一份独立副本。

常见的两种链接是：

* **硬链接（hard link）**
* **符号链接（symbolic link / symlink）**

创建链接使用 `ln` 命令。

---

## 1. 为什么使用链接而不是复制文件

假设服务的实时配置文件位于：

```text
/srv/demo/active.conf
```

内容为：

```text
MODE=active
```

现在希望在工作目录中通过：

```text
/home/learner/lab/demo.conf
```

访问同一个配置。

如果直接复制：

```bash
cp /srv/demo/active.conf /home/learner/lab/demo.conf
```

就会得到两个独立文件。

之后服务端配置发生变化时，工作目录中的副本不会自动变化，容易产生**配置漂移（configuration drift）**。

如果希望工作目录始终访问服务当前使用的文件，应考虑使用链接。

---

## 2. 硬链接

普通的 `ln` 创建硬链接：

```bash
ln TARGET LINK_NAME
```

例如：

```bash
ln /srv/demo/active.conf /home/learner/lab/demo.conf
```

### 硬链接的本质

硬链接相当于：

> 给同一份磁盘数据增加另一个文件名。

两个名字都直接指向同一个 inode / 文件数据。

因此：

```text
active.conf ─┐
             ├── 同一个文件数据
demo.conf   ─┘
```

修改文件内容时，通过任意一个名字读取，都可以看到修改后的内容。

### 硬链接的重要限制

硬链接通常：

* **不能跨文件系统创建**
* **不能链接目录**
* 某些特殊或挂载文件系统可能完全禁止创建硬链接

例如可能出现：

```text
Operation not permitted
```

---

## 3. 符号链接

使用 `-s` 创建符号链接：

```bash
ln -s TARGET LINK_NAME
```

例如：

```bash
ln -s /srv/demo/active.conf /home/learner/lab/demo.conf
```

符号链接本身是一个很小的特殊文件，其中保存的是：

```text
/srv/demo/active.conf
```

访问：

```text
/home/learner/lab/demo.conf
```

时，操作系统会根据符号链接保存的路径找到真正的目标文件。

可以把它理解为：

```text
demo.conf
    │
    └──> /srv/demo/active.conf
```

---

## 4. 硬链接与符号链接最关键的区别

两者最大的区别不是“能不能看到文件变化”，而是它们究竟链接到什么。

| 类型   | 实际绑定对象       |
| ---- | ------------ |
| 硬链接  | 文件数据 / inode |
| 符号链接 | 文件路径         |

这一点在目标文件被**替换**时尤其重要。

---

## 5. 为什么“替换文件”时符号链接更合适

很多服务更新配置时，不会直接修改原文件，而是采用类似下面的流程：

```bash
# 先生成一个新文件
new.conf

# 再通过 rename/mv 原子替换旧文件
mv new.conf active.conf
```

此时：

```text
/srv/demo/active.conf
```

这个**路径没有变化**，但这个路径背后对应的文件已经换成了新的 inode。

### 如果使用硬链接

硬链接绑定的是旧文件数据。

因此可能变成：

```text
/srv/demo/active.conf ───> 新文件

/home/learner/lab/demo.conf ───> 旧文件
```

也就是说，服务已经开始使用新配置，但硬链接仍可能看到旧数据。

### 如果使用符号链接

符号链接保存的是：

```text
/srv/demo/active.conf
```

程序每次打开符号链接时都会重新解析这个路径。

因此目标被替换以后：

```text
demo.conf
   │
   └──> /srv/demo/active.conf
                     │
                     └──> 新文件
```

仍然能够访问服务当前使用的配置。

### 结论

如果需求是：

> 始终跟随某个固定路径当前指向的文件

通常应该使用**符号链接**。

---

## 6. `ln` 的参数顺序

`ln` 最容易写反的地方就是参数顺序。

格式：

```bash
ln [选项] TARGET LINK_NAME
```

记忆方式：

```text
已有的东西在前
新名字在后
```

例如：

```bash
ln -s /srv/demo/active.conf /home/learner/lab/demo.conf
```

其中：

```text
/srv/demo/active.conf
```

是已经存在的目标文件。

```text
/home/learner/lab/demo.conf
```

是准备创建的新链接。

---

## 7. 创建本例中的符号链接

正确命令：

```bash
ln -s /srv/demo/active.conf /home/learner/lab/demo.conf
```

创建之后：

```text
/home/learner/lab/demo.conf
```

本身是符号链接，而真正的数据仍然保存在：

```text
/srv/demo/active.conf
```

目标文件没有被移动，也没有被复制。

---

## 8. 使用 `ls -l` 检查符号链接

可以运行：

```bash
ls -l /home/learner/lab/demo.conf
```

符号链接通常会显示类似：

```text
demo.conf -> /srv/demo/active.conf
```

其中：

```text
->
```

表示链接指向的目标路径。

这是检查符号链接是否创建正确的常用方法。

---

## 9. 通过符号链接读取文件

符号链接对大多数程序来说几乎和普通文件一样。

例如：

```bash
cat /home/learner/lab/demo.conf
```

实际上会读取：

```text
/srv/demo/active.conf
```

输出：

```text
MODE=active
```

因此可以通过链接读取内容并重定向：

```bash
cat /home/learner/lab/demo.conf > /home/learner/lab/demo-readback.txt
```

最终：

```text
/home/learner/lab/demo-readback.txt
```

内容为：

```text
MODE=active
```

并保留结尾换行。

这里的 `cat` 也起到了验证作用：

> 如果符号链接损坏或目标不存在，`cat` 就无法成功读取。

---

## 10. 什么是 broken symbolic link

符号链接允许目标暂时不存在。

例如：

```text
demo.conf -> /srv/demo/active.conf
```

如果：

```text
/srv/demo/active.conf
```

不存在，那么 `demo.conf` 仍然可能作为一个符号链接存在，但它无法解析到有效文件。

这种情况称为：

**broken symlink（损坏的符号链接 / 悬空链接）**

执行：

```bash
cat demo.conf
```

就会失败。

因此要区分：

```text
符号链接本身存在
```

和：

```text
符号链接能够正确解析到目标
```

这两件事并不完全相同。

---

## 11. 链接创建错误时如何修复

如果链接已经存在但指向了错误的位置，可以先删除链接：

```bash
rm /home/learner/lab/demo.conf
```

再重新创建：

```bash
ln -s /srv/demo/active.conf /home/learner/lab/demo.conf
```

这里：

```bash
rm demo.conf
```

删除的是**符号链接本身**，不会删除它指向的：

```text
/srv/demo/active.conf
```

这是符号链接非常重要的行为。

---

## 12. 本例完整操作

```bash
ln -s /srv/demo/active.conf /home/learner/lab/demo.conf

cat /home/learner/lab/demo.conf > /home/learner/lab/demo-readback.txt

ls -l /home/learner/lab/demo.conf
```

需要满足：

```text
/home/learner/lab/demo.conf
```

是指向：

```text
/srv/demo/active.conf
```

的符号链接。

并且：

```text
/home/learner/lab/demo-readback.txt
```

内容严格为：

```text
MODE=active
```

结尾带一个换行符。

---

## 13. 核心记忆

### `ln`

创建硬链接：

```bash
ln TARGET LINK_NAME
```

### `ln -s`

创建符号链接：

```bash
ln -s TARGET LINK_NAME
```

### 参数顺序

```text
目标在前，链接名在后
```

### 最重要的区别

```text
硬链接 → 跟随文件数据
符号链接 → 跟随文件路径
```

因此，当程序可能通过“创建新文件 + rename”替换某个路径对应的文件时：

> **如果需要始终跟随该路径当前的文件，应使用符号链接。**

### 检查符号链接

```bash
ls -l LINK_NAME
```

### 删除符号链接

```bash
rm LINK_NAME
```

只删除链接本身，不会删除目标文件。
