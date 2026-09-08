# Debian 中测试和安装 `/etc/cron.daily` 定时任务

## 1. 核心思想：把“任务命令”和“调度机制”分开测试

验证一个 cron 定时任务时，不应该为了测试而临时修改正式执行时间。

例如，一个任务计划每天凌晨 04:00 执行：

* 等到 04:00 才测试，效率太低；
* 临时改成“两分钟后执行”，测试的又不是最终配置。

更可靠的方法是把问题拆成两部分：

1. **任务本身能不能正确执行？**
2. **cron 调度系统会不会在正确时间调用它？**

这两部分可能独立失败。

例如：

```text
脚本正常 + cron 没启动
→ 手动执行成功，但永远不会自动执行

cron 正常 + 脚本有错误
→ cron 会调用脚本，但任务执行失败
```

因此，测试 cron 的基本原则是：

> **先直接测试任务，再单独确认调度服务正常。**

---

## 2. Debian 的周期任务目录

Debian 提供几个用于常见周期任务的目录：

```text
/etc/cron.hourly
/etc/cron.daily
/etc/cron.weekly
/etc/cron.monthly
```

它们分别表示：

| 目录                  | 含义    |
| ------------------- | ----- |
| `/etc/cron.hourly`  | 每小时执行 |
| `/etc/cron.daily`   | 每天执行  |
| `/etc/cron.weekly`  | 每周执行  |
| `/etc/cron.monthly` | 每月执行  |

这些目录中的文件**不是 crontab 配置行，而是可以直接执行的程序或脚本**。

例如：

```text
/etc/cron.daily/course-daily
```

文件本身不需要写：

```cron
0 4 * * *
```

因为：

> `/etc/cron.daily` 这个目录本身已经表示“每天执行”。

具体几点钟执行，由系统的 cron/anacron 配置决定。

---

## 3. `/etc/cron.daily` 和普通 crontab 的区别

普通 crontab：

```cron
minute hour day-of-month month day-of-week command
```

例如：

```cron
0 4 * * * /usr/local/bin/backup
```

表示每天 04:00 执行。

而 `/etc/cron.daily`：

```text
/etc/cron.daily/backup
```

没有任何时间字段。

可以理解为：

```text
普通 crontab：
时间 + 命令

cron.daily：
目录 = 时间周期
文件 = 命令
```

---

## 4. `run-parts`：执行目录中的周期任务

Debian 使用 `run-parts` 来执行这类目录中的脚本。

基本形式：

```bash
run-parts /etc/cron.daily
```

它会按照文件名顺序执行符合要求的文件。

### 只查看哪些文件会被执行

使用：

```bash
run-parts --test /etc/cron.daily
```

`--test` 不会真正执行脚本，只会打印将被执行的文件。

这是检查任务是否被系统识别的安全方法。

例如：

```bash
sudo run-parts --test /etc/cron.daily
```

如果输出包含：

```text
/etc/cron.daily/course-daily
```

说明文件名、权限等基本符合 `run-parts` 的要求。

---

## 5. 不要随便执行整个 `/etc/cron.daily`

下面的命令：

```bash
sudo run-parts /etc/cron.daily
```

并不是只运行刚刚创建的脚本，而是会执行目录中**所有符合条件的任务**。

真实 Debian 系统中可能包含：

```text
logrotate
man-db
apt 相关维护任务
其他系统维护脚本
```

因此普通机器上，更安全的测试方式通常是：

```bash
sudo /etc/cron.daily/course-daily
```

而检查整个目录使用：

```bash
sudo run-parts --test /etc/cron.daily
```

只有明确知道目录中的其他任务可以安全立即执行时，才运行：

```bash
sudo run-parts /etc/cron.daily
```

---

## 6. `run-parts` 对文件名有严格要求

一个非常常见的错误是：

```text
course-daily.sh
```

在 Debian 默认的 `run-parts` 规则下，文件名中的 `.` 会导致文件被忽略。

因此：

```text
course-daily.sh    ❌
course-daily       ✅
```

建议周期任务脚本直接使用：

```text
/etc/cron.daily/course-daily
```

而不是：

```text
/etc/cron.daily/course-daily.sh
```

### 为什么这个问题容易发现不了？

因为 cron 通常不会因为这个文件名主动报一个明显错误。

结果只是：

```text
文件存在
脚本内容正确
手动 ./course-daily.sh 可以运行
但是 run-parts 根本不执行它
```

可以通过以下命令发现问题：

```bash
run-parts --test /etc/cron.daily
```

---

## 7. 脚本必须具有执行权限

放入 `/etc/cron.daily` 的任务需要是可执行文件。

例如：

```bash
-rwxr-xr-x
```

对应：

```text
755
```

可以使用：

```bash
sudo chmod 755 /etc/cron.daily/course-daily
```

权限 `755` 表示：

```text
owner: rwx
group: r-x
other: r-x
```

即：

* root 可以读、写、执行；
* 其他用户只能读和执行。

检查：

```bash
ls -l /etc/cron.daily/course-daily
```

应该类似：

```text
-rwxr-xr-x 1 root root ... /etc/cron.daily/course-daily
```

---

## 8. 系统级任务通常应属于 root

系统的 cron 目录属于机器级配置，因此任务通常应该：

```text
owner = root
group = root
mode  = 755
```

可以使用：

```bash
sudo chown root:root /etc/cron.daily/course-daily
sudo chmod 755 /etc/cron.daily/course-daily
```

或者安装文件时一次完成：

```bash
sudo install -o root -g root -m 755 course-daily /etc/cron.daily/course-daily
```

`install` 在这种场景下比“复制后再 chmod/chown”更方便，因为一个命令就能同时指定：

* 目标路径；
* owner；
* group；
* mode。

---

## 9. 示例：创建 `course-daily`

目标：

每天执行一个脚本，把：

```text
daily complete
```

写入：

```text
/var/lib/course/daily-ran
```

### 脚本内容

`course-daily`：

```sh
#!/bin/sh

mkdir -p /var/lib/course
printf '%s\n' 'daily complete' > /var/lib/course/daily-ran
```

### 每一行的作用

```sh
#!/bin/sh
```

指定使用 `/bin/sh` 执行脚本。

```sh
mkdir -p /var/lib/course
```

确保目标目录存在。

`-p` 的作用是：

* 目录不存在时创建；
* 已经存在时不报错。

```sh
printf '%s\n' 'daily complete' > /var/lib/course/daily-ran
```

将：

```text
daily complete
```

加一个换行符后写入文件。

这里使用：

```bash
>
```

因此每次执行都会**覆盖**之前的内容。

如果希望每次追加一行，则应该使用：

```bash
>>
```

---

## 10. 安装脚本

假设当前目录存在：

```text
course-daily
```

执行：

```bash
sudo install -o root -g root -m 755 \
  course-daily \
  /etc/cron.daily/course-daily
```

然后检查：

```bash
ls -l /etc/cron.daily/course-daily
```

应该看到类似：

```text
-rwxr-xr-x 1 root root ... /etc/cron.daily/course-daily
```

---

## 11. 先检查 `run-parts` 是否识别任务

运行：

```bash
sudo run-parts --test /etc/cron.daily
```

应该包含：

```text
/etc/cron.daily/course-daily
```

如果没有出现，首先检查：

### 文件名

错误：

```text
course-daily.sh
```

正确：

```text
course-daily
```

### 是否可执行

检查：

```bash
ls -l /etc/cron.daily/course-daily
```

必须包含执行位：

```text
-rwxr-xr-x
```

---

## 12. 启动 cron 服务

脚本配置正确并不代表 cron 一定会自动执行它。

如果 cron 服务没有运行：

```text
/etc/cron.daily/course-daily
```

只是一个没人调度的文件。

在使用 systemd 的 Debian 系统中，可以启动：

```bash
sudo systemctl start cron
```

检查状态：

```bash
systemctl status cron
```

或者：

```bash
systemctl is-active cron
```

如果正常运行，会看到：

```text
active
```

如果希望系统启动后自动启动 cron，可以使用：

```bash
sudo systemctl enable cron
```

或者一次完成：

```bash
sudo systemctl enable --now cron
```

其中：

```text
--now
```

表示除了启用开机启动，还立即启动服务。

---

## 13. 手动运行任务，验证实际效果

### 更安全的方法：直接执行单个脚本

```bash
sudo /etc/cron.daily/course-daily
```

然后检查：

```bash
cat /var/lib/course/daily-ran
```

应该得到：

```text
daily complete
```

### 如果实验环境允许执行整个 daily 目录

可以运行：

```bash
sudo run-parts /etc/cron.daily
```

然后：

```bash
cat /var/lib/course/daily-ran
```

仍然应该得到：

```text
daily complete
```

这证明：

```text
run-parts
    ↓
/etc/cron.daily/course-daily
    ↓
写入 /var/lib/course/daily-ran
```

整条任务执行链已经能够正常工作。

---

## 14. 一套完整的验证流程

推荐按照以下顺序检查：

```bash
# 1. 检查文件权限和所有者
ls -l /etc/cron.daily/course-daily

# 2. 检查 run-parts 是否会识别它
sudo run-parts --test /etc/cron.daily

# 3. 检查 cron 服务
systemctl is-active cron

# 4. 手动执行任务
sudo /etc/cron.daily/course-daily

# 5. 检查任务结果
cat /var/lib/course/daily-ran
```

如果实验环境要求测试整个 daily 目录，则第 4 步改成：

```bash
sudo run-parts /etc/cron.daily
```

---

## 15. 常见错误

### 错误 1：文件名带 `.sh`

```text
/etc/cron.daily/course-daily.sh
```

可能被默认的 `run-parts` 忽略。

改成：

```text
/etc/cron.daily/course-daily
```

---

### 错误 2：脚本没有执行权限

例如：

```text
-rw-r--r--
```

修复：

```bash
sudo chmod 755 /etc/cron.daily/course-daily
```

---

### 错误 3：脚本能手动执行，就认为 cron 一定正常

错误推理：

```text
脚本执行成功
→ cron 配置一定成功
```

实际上还需要确认：

```text
cron 服务是否运行
run-parts 是否识别脚本
系统的周期调度是否正常
```

---

### 错误 4：为了测试修改正式执行时间

不要通过把凌晨任务临时改成几分钟后来证明脚本正常。

应该分别测试：

```text
任务本身
+
调度机制
```

这样不会为了测试破坏最终配置。

---

### 错误 5：直接运行整个 `/etc/cron.daily`

```bash
sudo run-parts /etc/cron.daily
```

可能同时触发系统中其他维护任务。

一般优先：

```bash
sudo run-parts --test /etc/cron.daily
sudo /etc/cron.daily/course-daily
```

---

## 16. 关键知识总结

Debian 周期任务可以使用：

```text
/etc/cron.hourly
/etc/cron.daily
/etc/cron.weekly
/etc/cron.monthly
```

其中：

```text
目录名称决定执行周期
文件本身负责具体工作
```

安装 `/etc/cron.daily` 任务时重点检查四件事：

```text
1. 文件名符合 run-parts 规则，不要随意使用 .sh
2. 文件具有执行权限
3. 系统级任务通常由 root 拥有
4. cron 调度服务正在运行
```

最重要的测试思路是：

```text
先证明“命令能运行”
再证明“调度系统会调用它”
```

不要把“脚本正确”和“定时调度正确”当成同一个问题。
