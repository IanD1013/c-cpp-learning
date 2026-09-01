# 学习笔记

## Bash 环境变量与持久化

### 1. 什么是环境变量

Linux 中的每个进程都可以拥有一组**环境变量（environment variables）**。

环境变量本质上是：

```text
变量名=变量值
```

例如：

```bash
HOME=/home/learner
PATH=/usr/local/bin:/usr/bin:/bin
```

常见环境变量：

| 变量     | 作用                   |
| ------ | -------------------- |
| `HOME` | 当前用户的 home directory |
| `PATH` | Bash 查找可执行程序时搜索的目录列表 |
| `USER` | 当前用户名                |

查看当前 shell 中的环境变量：

```bash
env
```

---

## 2. Shell 变量与 `export`

可以在 Bash 中创建变量：

```bash
COURSE_ENV=training
```

但普通 shell 变量默认只属于当前 Bash。

如果希望**当前 Bash 启动的子进程也能读取这个变量**，需要使用：

```bash
export COURSE_ENV=training
```

`export` 的核心作用是：

> 将 shell 变量加入环境，使之后启动的子进程继承它。

例如：

```bash
export COURSE_ENV=training
```

此时 Bash 中：

```bash
echo "$COURSE_ENV"
```

输出：

```text
training
```

---

## 3. 环境变量的生命周期

直接执行：

```bash
export COURSE_ENV=training
```

只对**当前 shell 会话**有效。

关闭终端后，这个 Bash 进程结束，变量也会消失。

因此需要区分：

### 临时变量

适合一次测试：

```bash
export COURSE_ENV=training
```

生命周期：

```text
当前 shell → shell 结束 → 变量消失
```

### 持久变量

如果希望以后每次打开 Bash 都自动存在，需要把设置写入 Bash 的启动配置文件。

对于交互式 Bash，常见配置文件是：

```text
~/.bashrc
```

因此可以加入：

```bash
export COURSE_ENV=training
```

以后新的 Bash 会自动执行这条命令。

---

## 4. `~/.bashrc`

`~/.bashrc` 是用户 home directory 中的 Bash 配置文件：

```text
~/.bashrc
```

其中：

```text
~
```

表示当前用户的 home directory，例如：

```text
/home/learner
```

所以：

```text
~/.bashrc
```

等价于：

```text
/home/learner/.bashrc
```

文件名前面的 `.` 表示这是一个**隐藏文件（dotfile）**。

`.bashrc` 通常包含：

* prompt 配置
* alias
* 环境变量
* Bash 行为设置
* PATH 修改等

---

## 5. 修改配置文件时不要覆盖原内容

`.bashrc` 往往已经存在，并包含系统或用户配置。

因此不要使用：

```bash
printf 'export COURSE_ENV=training\n' > ~/.bashrc
```

因为：

```bash
>
```

会**覆盖整个文件**。

应该使用：

```bash
>>
```

表示追加内容：

```bash
printf '%s\n' 'export COURSE_ENV=training' >> ~/.bashrc
```

区别：

| 操作符  | 作用      |
| ---- | ------- |
| `>`  | 覆盖文件    |
| `>>` | 追加到文件末尾 |

修改配置文件时尤其要小心这一区别。

---

## 6. 为什么修改 `.bashrc` 后变量没有立即出现

编辑：

```text
~/.bashrc
```

只是修改磁盘上的文件。

当前正在运行的 Bash **不会自动重新读取它**。

`.bashrc` 通常只会在新的交互式 Bash 启动时读取。

因此修改后有两种方法：

### 方法一：重新打开 shell

新的 Bash 会读取：

```text
~/.bashrc
```

### 方法二：使用 `source`

```bash
source ~/.bashrc
```

`source` 会让**当前 Bash 直接执行这个文件中的命令**。

因此：

```bash
source ~/.bashrc
```

之后：

```bash
echo "$COURSE_ENV"
```

就会得到：

```text
training
```

也可以写成：

```bash
. ~/.bashrc
```

`.` 是 `source` 的简写。

---

## 7. 变量展开：`$VARIABLE`

Bash 中：

```bash
$COURSE_ENV
```

表示：

> 将变量名替换成它当前的值。

例如：

```bash
COURSE_ENV=training
```

那么：

```bash
echo "$COURSE_ENV"
```

实际上相当于：

```bash
echo "training"
```

这个过程叫：

> **variable expansion（变量展开）**

---

## 8. 为什么变量通常要写成 `"$VARIABLE"`

推荐：

```bash
"$COURSE_ENV"
```

而不是：

```bash
$COURSE_ENV
```

原因是双引号可以保证变量展开后的内容保持为**一个完整参数**。

例如：

```bash
NAME="Linux Course"
```

推荐：

```bash
printf '%s\n' "$NAME"
```

这样 `Linux Course` 会作为一个参数传给 `printf`。

因此 Bash 中一个非常重要的习惯是：

```bash
"$VARIABLE"
```

> **变量展开时默认加双引号，除非你明确知道自己需要 word splitting。**

---

## 9. 使用 `printf` 输出变量

`printf` 可以使用格式字符串：

```bash
printf '%s\n' "$COURSE_ENV"
```

其中：

```text
%s
```

表示：

> 在这里插入一个字符串参数。

而：

```text
\n
```

表示 newline。

所以：

```bash
printf '%s\n' "$COURSE_ENV"
```

如果：

```bash
COURSE_ENV=training
```

输出就是：

```text
training
```

并在末尾带一个 newline。

---

## 10. 将环境变量写入文件

可以结合：

* 变量展开
* `printf`
* 输出重定向

例如：

```bash
printf '%s\n' "$COURSE_ENV" > ~/lab/course-env.txt
```

执行过程可以理解为：

```text
$COURSE_ENV
    ↓
training
    ↓
printf
    ↓
training\n
    ↓
>
    ↓
course-env.txt
```

最终文件内容是：

```text
training\n
```

即：

* `training`
* 后面正好一个 newline

---

## 11. 为什么要“从变量本身”创建文件

以下命令虽然可能产生相同文件：

```bash
printf '%s\n' 'training' > ~/lab/course-env.txt
```

但它并不能证明：

```bash
COURSE_ENV
```

已经正确设置。

更好的做法是：

```bash
printf '%s\n' "$COURSE_ENV" > ~/lab/course-env.txt
```

这样同时验证了：

1. `.bashrc` 中配置正确
2. 当前 shell 已经加载配置
3. `COURSE_ENV` 的值确实是 `training`
4. 变量展开正常工作
5. 输出重定向正常工作

---

# 完整操作流程

目标：

```text
/home/learner/.bashrc
```

包含：

```bash
export COURSE_ENV=training
```

并创建：

```text
/home/learner/lab/course-env.txt
```

文件内容必须严格为：

```text
training\n
```

可以执行：

```bash
printf '%s\n' 'export COURSE_ENV=training' >> ~/.bashrc
source ~/.bashrc
printf '%s\n' "$COURSE_ENV" > ~/lab/course-env.txt
```

检查变量：

```bash
printf '%s\n' "$COURSE_ENV"
```

预期：

```text
training
```

检查文件：

```bash
cat ~/lab/course-env.txt
```

预期：

```text
training
```

---

# 核心知识总结

记住这条完整链路：

```text
export
    ↓
让子进程继承变量

~/.bashrc
    ↓
让变量在新的 Bash session 中自动设置

source ~/.bashrc
    ↓
让当前 Bash 立即重新加载配置

"$VARIABLE"
    ↓
安全地展开变量

printf '%s\n'
    ↓
精确输出字符串和 newline

>
    ↓
把输出写入文件
```

最重要的几个命令：

```bash
export COURSE_ENV=training
```

```bash
source ~/.bashrc
```

```bash
printf '%s\n' "$COURSE_ENV"
```

```bash
printf '%s\n' "$COURSE_ENV" > file.txt
```

以及文件操作中的关键区别：

```bash
>   # 覆盖
>>  # 追加
```
