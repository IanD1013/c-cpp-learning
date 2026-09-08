# Cron 定时任务与 Crontab 基础

## 1. Cron 是什么

`cron` 是 Linux/Unix 中用于**按时间自动执行命令**的定时任务系统。

例如：

* 每天凌晨备份数据库
* 每周清理日志
* 每个工作日早上生成报表
* 每 10 分钟运行一次监控脚本

`cron` 会定期检查系统时间，并执行当前时间匹配的任务。

---

## 2. Crontab 是什么

每个用户都可以拥有自己的 **crontab**，即该用户的 cron 定时任务列表。

一条普通 crontab 记录的格式是：

```text
minute hour day-of-month month day-of-week command
```

即：

```text
分钟 小时 日 月 星期 命令
```

例如：

```cron
15 6 * * 1-5 /usr/local/bin/course-report
```

表示：

> 每周一到周五的 **06:15** 执行 `/usr/local/bin/course-report`。

---

## 3. 五个时间字段

| 字段           | 含义     | 常见范围   |
| ------------ | ------ | ------ |
| minute       | 分钟     | `0-59` |
| hour         | 小时     | `0-23` |
| day-of-month | 每月第几天  | `1-31` |
| month        | 月份     | `1-12` |
| day-of-week  | 星期     | `0-6`  |
| command      | 要执行的命令 | 任意命令   |

其中星期通常表示：

```text
0 = Sunday
1 = Monday
2 = Tuesday
3 = Wednesday
4 = Thursday
5 = Friday
6 = Saturday
```

因此：

```cron
1-5
```

就是：

```text
Monday - Friday
```

### 一个容易犯的错误：分钟在前，小时在后

例如：

```cron
15 6
```

不是：

```text
15:06
```

而是：

```text
06:15
```

因为字段顺序是：

```text
minute hour
```

---

## 4. Cron 时间字段中的常用写法

### `*`：所有值

```cron
* * * * *
```

表示每一分钟都匹配。

例如：

```cron
0 6 * * *
```

表示：

> 每天 06:00。

---

### 单个数字

```cron
15 6 * * *
```

表示：

> 每天 06:15。

---

### 列表 `,`

```cron
0 6 1,15 * *
```

表示：

> 每个月的 1 日和 15 日，06:00 执行。

---

### 范围 `-`

```cron
15 6 * * 1-5
```

表示：

> 周一到周五 06:15。

---

### 步长 `/`

```cron
*/10 * * * *
```

表示：

> 每 10 分钟执行一次。

例如：

```text
00
10
20
30
40
50
```

分钟都会匹配。

---

## 5. Cron 如何判断任务是否执行

对于一条：

```cron
15 6 * * 1-5 command
```

可以理解为：

```text
分钟      = 15
小时      = 6
日期      = 任意
月份      = 任意
星期      = 周一到周五
```

因此它会在工作日的：

```text
06:15
```

执行命令。

---

## 6. 管理 Crontab 的三个重要命令

### 查看当前 crontab

```bash
crontab -l
```

`-l` 是 **list**。

它会打印当前用户已经设置的所有 cron 任务。

例如：

```bash
crontab -l
```

可能看到：

```cron
0 2 * * * /usr/local/bin/backup
15 6 * * 1-5 /usr/local/bin/course-report
```

---

### 编辑 crontab

```bash
crontab -e
```

`-e` 是 **edit**。

它会打开编辑器，让用户直接修改自己的 crontab。

例如添加：

```cron
15 6 * * 1-5 /usr/local/bin/course-report
```

保存退出后，cron 会自动加载新的配置。

---

### 从标准输入安装整个 crontab

```bash
crontab -
```

这里的 `-` 表示：

> 从标准输入 `stdin` 读取 crontab 内容。

例如：

```bash
printf '%s\n' '15 6 * * 1-5 /usr/local/bin/course-report' | crontab -
```

这会把输入内容安装为当前用户的新 crontab。

### 重要：`crontab -` 是“替换”，不是“追加”

如果原来已经存在：

```cron
0 2 * * * /usr/local/bin/backup
```

然后执行：

```bash
printf '%s\n' '15 6 * * 1-5 /usr/local/bin/course-report' | crontab -
```

原来的 backup 任务可能会被删除，因为整个 crontab 会被替换成：

```cron
15 6 * * 1-5 /usr/local/bin/course-report
```

所以实际使用时必须特别小心。

---

## 7. 安全地追加一个 Cron 任务

如果希望**保留已有任务，同时增加一条新任务**，常见写法是：

```bash
(crontab -l 2>/dev/null; printf '%s\n' '15 6 * * 1-5 /usr/local/bin/course-report') | crontab -
```

工作过程是：

```text
crontab -l
     │
     │ 读取已有任务
     ▼
已有 crontab
+
新的 cron 记录
     │
     ▼
crontab -
     │
     ▼
安装完整的新 crontab
```

其中：

```bash
2>/dev/null
```

用于隐藏“当前还没有 crontab”之类的错误信息。

不过这种方法重复执行时可能加入重复记录，因此手工管理任务时：

```bash
crontab -e
```

通常更直观。

---

## 8. 为什么不要直接修改 Cron 保存的系统文件

应该使用：

```bash
crontab -e
crontab -l
crontab -
```

而不是寻找 cron 内部保存 crontab 的文件后直接修改。

原因包括：

1. `crontab` 命令会正确处理文件权限；
2. 它会保证任务属于正确的用户；
3. 它会通知 cron 配置发生了变化；
4. 不同系统内部保存 crontab 的位置可能不同。

因此应把：

```bash
crontab
```

看成管理用户 cron 配置的正式接口。

---

## 9. Cron 运行环境与交互式 Shell 不完全相同

即使一个命令在终端里能够运行：

```bash
course-report
```

放进 cron 后也不一定成功。

因为 cron 通常拥有更有限的环境变量，尤其是：

```text
PATH
```

所以最好使用完整路径：

```cron
15 6 * * 1-5 /usr/local/bin/course-report
```

而不是：

```cron
15 6 * * 1-5 course-report
```

脚本本身也应该具有执行权限，例如：

```bash
chmod +x /usr/local/bin/course-report
```

如果脚本依赖其他程序，也最好考虑使用明确路径或在脚本中正确配置环境。

---

## 10. 核心知识总结

```text
crontab 格式：

minute hour day-of-month month day-of-week command
```

工作日上午 06:15：

```cron
15 6 * * 1-5 command
```

常用命令：

```bash
crontab -l   # 查看
crontab -e   # 编辑
crontab -    # 从 stdin 替换整个 crontab
```

需要特别记住：

> `crontab -` 会安装标准输入中的**整个 crontab**，不是简单追加一条任务。

---

# 问题解答：工作日 06:15 自动运行 Course Report

## 问题

将：

```text
/usr/local/bin/course-report
```

设置为当前用户每周一到周五 **06:15** 自动执行，并在修改前后查看 crontab。

## Cron 表达式

正确的记录是：

```cron
15 6 * * 1-5 /usr/local/bin/course-report
```

含义：

| 字段           |     值 | 含义      |
| ------------ | ----: | ------- |
| minute       |  `15` | 第 15 分钟 |
| hour         |   `6` | 上午 6 点  |
| day-of-month |   `*` | 每一天     |
| month        |   `*` | 每个月     |
| day-of-week  | `1-5` | 周一到周五   |

因此执行时间为：

```text
Monday-Friday, 06:15
```

## 第一步：查看当前 Crontab

```bash
crontab -l
```

这样可以先确认当前有哪些任务。

## 第二步：在保留已有任务的情况下添加新任务

```bash
(crontab -l 2>/dev/null; printf '%s\n' '15 6 * * 1-5 /usr/local/bin/course-report') | crontab -
```

这里不能简单地执行：

```bash
printf '%s\n' '15 6 * * 1-5 /usr/local/bin/course-report' | crontab -
```

除非确定当前没有其他任务，因为这样会用这一行**替换整个现有 crontab**。

## 第三步：再次检查

```bash
crontab -l
```

现在应该能看到：

```cron
15 6 * * 1-5 /usr/local/bin/course-report
```

如果之前还有其他 cron 任务，它们也应该仍然存在。

## 最终命令

```bash
crontab -l

(crontab -l 2>/dev/null; printf '%s\n' '15 6 * * 1-5 /usr/local/bin/course-report') | crontab -

crontab -l
```

这三个步骤分别对应：

```text
查看修改前配置
→ 添加工作日 06:15 的任务
→ 查看修改后的配置
```
