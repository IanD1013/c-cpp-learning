# `/etc/cron.d`：配置系统级 Cron 定时任务

## 1. 用户 Crontab 与系统级 Cron 的区别

Cron 任务常见有两种放置方式：

* **用户级任务**：使用 `crontab -e`
* **系统级任务**：放在 `/etc/cron.d/`

如果一个任务属于某个用户自己的日常工作，通常放进该用户的 crontab。

如果一个任务属于**整台机器的系统配置**，例如：

* 定期清理临时文件
* 系统维护
* 日志归档
* 由管理员统一部署的后台任务

更适合放在：

```text
/etc/cron.d/
```

`/etc/cron.d/` 中每个文件都是系统 Cron 配置的一部分，cron 守护进程会读取这些文件。

---

## 2. `/etc/cron.d` 的格式

普通用户 crontab 的格式是：

```text
minute hour day-of-month month day-of-week command
```

而 `/etc/cron.d` 多了一个非常重要的字段：

```text
minute hour day-of-month month day-of-week user command
```

也就是：

```text
分 时 日 月 星期 用户 命令
```

例如：

```cron
0 4 * * 0 learner /usr/local/bin/course-cleanup
```

含义是：

| 字段           |                               值 | 含义                 |
| ------------ | ------------------------------: | ------------------ |
| minute       |                             `0` | 第 0 分钟             |
| hour         |                             `4` | 04:00              |
| day-of-month |                             `*` | 每个月的每一天            |
| month        |                             `*` | 每个月                |
| day-of-week  |                             `0` | 星期日                |
| user         |                       `learner` | 以 `learner` 用户身份执行 |
| command      | `/usr/local/bin/course-cleanup` | 要运行的程序             |

因此：

```cron
0 4 * * 0 learner /usr/local/bin/course-cleanup
```

表示：

> 每周日凌晨 04:00，以 `learner` 用户身份执行 `/usr/local/bin/course-cleanup`。

---

## 3. 为什么 `/etc/cron.d` 必须写用户字段

普通用户使用：

```bash
crontab -e
```

时，cron 已经知道这个 crontab 属于哪个用户，因此不需要再指定用户。

但是 `/etc/cron.d` 中的文件通常由 `root` 安装和管理。仅从文件本身无法判断任务应该以哪个用户运行，因此必须显式写出：

```text
user
```

例如：

```cron
0 4 * * 0 learner /usr/local/bin/course-cleanup
```

这里的：

```text
learner
```

不是命令的一部分，而是：

> 指定后面的命令以哪个用户身份运行。

### 常见错误：忘记 user 字段

错误：

```cron
0 4 * * 0 /usr/local/bin/course-cleanup
```

Cron 会按照 `/etc/cron.d` 的格式解析：

```text
0 4 * * 0 <user> <command>
```

于是它可能把：

```text
/usr/local/bin/course-cleanup
```

当成用户名，而真正的命令字段变为空或被错误解析。

因此不能把用户 crontab 的格式直接复制到 `/etc/cron.d`。

---

## 4. `/etc/cron.d` 文件权限非常重要

Cron 对 `/etc/cron.d` 下的文件有安全要求。

推荐配置：

```text
owner: root
mode: 644
```

即：

```bash
-rw-r--r-- root root
```

可以使用：

```bash
sudo chown root:root /etc/cron.d/course-cleanup
sudo chmod 644 /etc/cron.d/course-cleanup
```

`644` 的含义是：

```text
root:        rw-
group:       r--
others:      r--
```

也就是：

* root 可以读写
* 其他用户只能读取
* 其他用户不能修改

### 为什么不能允许其他用户写入

如果 `/etc/cron.d` 中的文件允许 group 或 other 写入，例如：

```text
666
664
```

攻击者可能修改系统定时任务。

因此 cron 会对这些文件进行安全检查，不符合要求的文件可能被忽略。

所以权限不是一个无关紧要的附加设置，而是系统级 Cron 配置的一部分。

---

## 5. Cron 的运行环境与登录 Shell 不同

一个命令在终端里能够正常运行，并不代表它在 cron 中也一定能运行。

最常见原因是：

> Cron 的环境变量比交互式 Shell 少得多。

例如登录 Bash 时，可能会读取：

```text
~/.bashrc
~/.profile
/etc/profile
```

这些文件可能设置：

```bash
PATH=...
```

但 cron 通常不会读取用户的 `.bashrc`。

因此终端里可以运行：

```bash
my-command
```

cron 中却可能出现：

```text
command not found
```

---

## 6. 显式设置 `SHELL`

可以在 `/etc/cron.d` 文件顶部写：

```cron
SHELL=/bin/bash
```

这表示 Cron 执行命令时使用：

```text
/bin/bash
```

而不是依赖系统默认 Shell。

这在命令使用 Bash 特有语法时尤其重要。

---

## 7. 显式设置 `PATH`

推荐在系统级 cron 文件中显式声明：

```cron
PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
```

这样 cron 执行任务时，搜索程序的位置就是确定的。

完整示例：

```cron
SHELL=/bin/bash
PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
0 4 * * 0 learner /usr/local/bin/course-cleanup
```

这样配置的优点是：

* 不依赖用户 `.bashrc`
* 不依赖管理员当前的 `PATH`
* 不依赖安装任务时使用的 Shell 环境
* 配置更容易复现和排查

---

## 8. 为什么命令最好使用绝对路径

推荐：

```cron
/usr/local/bin/course-cleanup
```

而不是：

```cron
course-cleanup
```

虽然设置了 `PATH` 后后者可能也能运行，但绝对路径更加明确。

Cron 任务最好尽量减少对隐式环境的依赖，例如：

```cron
0 4 * * 0 learner /usr/local/bin/course-cleanup
```

比：

```cron
0 4 * * 0 learner course-cleanup
```

更可靠。

同样，如果脚本内部调用程序，也可以考虑使用完整路径，或者确保这些程序所在目录已经包含在 `PATH` 中。

---

## 9. 完整配置示例

目标：

> 创建 `/etc/cron.d/course-cleanup`，每周日 04:00，以 `learner` 用户运行 `/usr/local/bin/course-cleanup`。

文件内容应为**恰好三行**：

```cron
SHELL=/bin/bash
PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
0 4 * * 0 learner /usr/local/bin/course-cleanup
```

然后设置所有权和权限：

```bash
sudo chown root:root /etc/cron.d/course-cleanup
sudo chmod 644 /etc/cron.d/course-cleanup
```

检查：

```bash
ls -l /etc/cron.d/course-cleanup
```

应类似：

```text
-rw-r--r-- 1 root root ... /etc/cron.d/course-cleanup
```

---

## 10. 与用户 Crontab 对比

| 特性       | 用户 `crontab -e`   | `/etc/cron.d`            |
| -------- | ----------------- | ------------------------ |
| 主要用途     | 用户自己的任务           | 系统级任务                    |
| 是否指定执行用户 | 不需要               | 必须                       |
| 格式       | 5 个时间字段 + command | 5 个时间字段 + user + command |
| 常见管理者    | 普通用户              | root / 系统管理员             |
| 文件权限要求   | 由 crontab 工具管理    | 需要正确的 owner 和 mode       |
| 是否适合机器配置 | 一般                | 是                        |

例如，同一个任务在用户 crontab 中可能写：

```cron
0 4 * * 0 /usr/local/bin/course-cleanup
```

而在 `/etc/cron.d` 中必须写：

```cron
0 4 * * 0 learner /usr/local/bin/course-cleanup
```

关键区别就是：

```text
learner
```

这个执行用户字段。

---

## 11. 常见误区

### 误区 1：把用户 crontab 格式直接复制到 `/etc/cron.d`

错误：

```cron
0 4 * * 0 /usr/local/bin/course-cleanup
```

正确：

```cron
0 4 * * 0 learner /usr/local/bin/course-cleanup
```

---

### 误区 2：认为 Cron 会加载 `.bashrc`

通常不会。

不要依赖 `.bashrc` 中的：

```bash
PATH
aliases
functions
environment variables
```

系统级任务应该显式定义需要的环境。

---

### 误区 3：脚本属于 `learner`，所以 cron 文件也应该属于 `learner`

不是。

Cron 配置文件与执行命令的用户是两个概念。

推荐：

```text
/etc/cron.d/course-cleanup
owner = root
```

但任务本身可以写：

```text
user = learner
```

即：

```cron
0 4 * * 0 learner /usr/local/bin/course-cleanup
```

含义是：

> root 管理这个 Cron 配置，但命令以 learner 身份运行。

---

### 误区 4：`0 4 * * 0` 表示每天 04:00

不是。

```text
0 4 * * 0
```

最后一个字段是：

```text
day-of-week = 0
```

因此表示星期日。

每天 04:00 应该是：

```cron
0 4 * * *
```

---

## 12. 核心记忆

系统级 `/etc/cron.d` 配置最重要的三点是：

```text
正确格式
+ 明确环境
+ 安全权限
```

典型模板：

```cron
SHELL=/bin/bash
PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
分 时 日 月 星期 用户 命令
```

例如：

```cron
SHELL=/bin/bash
PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
0 4 * * 0 learner /usr/local/bin/course-cleanup
```

文件则应通常保持：

```text
root:root
644
```

这就是一个可靠、明确且适合系统管理的 `/etc/cron.d` 定时任务配置。
