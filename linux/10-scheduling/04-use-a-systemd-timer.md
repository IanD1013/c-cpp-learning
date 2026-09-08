# systemd Timer：适合系统级定时任务的现代调度方式

## 1. systemd Timer 与 cron 的核心区别

`cron` 和 `systemd timer` 都可以定时执行任务，但 systemd timer 的优势不只是“定时”。

一个 systemd 定时任务通常由两个 unit（单元）组成：

```text
course-cleanup.timer
course-cleanup.service
```

它们职责分离：

| Unit       | 作用           |
| ---------- | ------------ |
| `.timer`   | 决定**什么时候执行** |
| `.service` | 决定**执行什么**   |

两个文件使用相同的基础名称：

```text
course-cleanup.timer
course-cleanup.service
```

因此 systemd 会自动把它们关联起来：

```text
course-cleanup.timer
        ↓ 到达触发时间
course-cleanup.service
        ↓
执行实际命令
```

这是一种很重要的设计思想：

> **调度逻辑和任务逻辑分离。**

这样可以独立测试任务，而不需要等待定时时间到来。

---

## 2. `.service`：定义“做什么”

例如：

```ini
[Unit]
Description=Course cleanup task

[Service]
Type=oneshot
ExecStart=/usr/local/bin/course-cleanup
```

这里最关键的是：

```ini
ExecStart=/usr/local/bin/course-cleanup
```

表示启动这个 service 时执行该程序。

`Type=oneshot` 表示：

> 这个服务执行一次命令，命令结束后任务就完成。

这很适合清理、备份、生成报告等一次性任务。

### 手动测试 service

不需要等 timer 到达 02:30，可以直接运行：

```bash
sudo systemctl start course-cleanup.service
```

这样只会执行任务本身，不会修改 timer 的计划。

检查执行状态：

```bash
systemctl status course-cleanup.service
```

查看日志：

```bash
journalctl -u course-cleanup.service
```

因此 systemd timer 的调试通常分成两部分：

```text
先测试 service 是否能正确执行
        ↓
再测试 timer 是否正确调度
```

---

## 3. `.timer`：定义“什么时候执行”

每天凌晨 02:30 可以写成：

```ini
[Unit]
Description=Run course cleanup daily at 02:30

[Timer]
OnCalendar=*-*-* 02:30:00
Persistent=true

[Install]
WantedBy=timers.target
```

其中最重要的是：

```ini
OnCalendar=*-*-* 02:30:00
```

systemd 的日历时间可以理解成：

```text
年-月-日 时:分:秒
```

因此：

```text
*-*-* 02:30:00
```

表示：

```text
任意年-任意月-任意日 02:30:00
```

也就是：

> **每天凌晨 02:30。**

---

## 4. `OnCalendar` 与 cron 的写法差异

cron 中通常先写时间：

```text
minute hour day-of-month month day-of-week
```

例如每天 02:30：

```cron
30 2 * * *
```

systemd timer 则更接近普通日期写法：

```ini
OnCalendar=*-*-* 02:30:00
```

对比：

| 含义       | cron         | systemd timer    |
| -------- | ------------ | ---------------- |
| 每天 02:30 | `30 2 * * *` | `*-*-* 02:30:00` |

systemd 的格式更像：

```text
日期 + 时间
```

因此阅读复杂日期时通常更直观。

---

## 5. `Persistent=true`：补执行错过的任务

timer 中常见：

```ini
Persistent=true
```

它解决的是：

> 如果原本应该执行任务时机器处于关机状态怎么办？

假设任务计划：

```text
每天 02:30
```

但是服务器：

```text
02:00 关机
08:00 开机
```

如果设置：

```ini
Persistent=true
```

systemd 会发现：

```text
02:30 的执行机会被错过了
```

然后在系统重新运行后补执行任务。

这和普通 cron 的行为不同。传统 cron 通常不会自动补执行错过的任务。

---

## 6. 为什么修改 unit 后必须运行 `daemon-reload`

systemd 不会因为磁盘上的 unit 文件发生变化，就立刻重新读取全部配置。

创建或修改：

```text
/etc/systemd/system/course-cleanup.service
/etc/systemd/system/course-cleanup.timer
```

之后需要执行：

```bash
sudo systemctl daemon-reload
```

它的作用是：

> 让 systemd 重新读取 unit 文件。

常见错误是修改了文件后直接运行：

```bash
systemctl start ...
```

结果发现行为没有变化。

原因往往不是配置没写对，而是：

```text
systemd 仍然使用之前加载的配置
```

因此可以形成习惯：

```text
修改 unit 文件
        ↓
systemctl daemon-reload
        ↓
启动或重启 unit
```

---

## 7. `enable --now` 的作用

创建 timer 后可以执行：

```bash
sudo systemctl enable --now course-cleanup.timer
```

这相当于同时完成两件事。

### `enable`

```bash
systemctl enable course-cleanup.timer
```

表示：

> 配置 timer，使其在以后系统启动时自动启用。

### `--now`

```bash
--now
```

表示：

> 不等待下次开机，现在立即启动 timer。

所以：

```bash
sudo systemctl enable --now course-cleanup.timer
```

可以理解为：

```text
以后开机自动启动
+
现在立刻启动
```

注意：

> 启动 timer 不等于立即执行 service。

如果当前时间不是 02:30：

```bash
systemctl enable --now course-cleanup.timer
```

只是让 timer 开始等待下一个匹配时间。

如果想立即测试实际任务，应运行：

```bash
sudo systemctl start course-cleanup.service
```

---

## 8. systemd Timer 的日志优势

cron 任务的输出经常需要额外配置邮件、日志文件或重定向。

systemd service 的标准输出和错误默认进入 systemd journal，因此可以直接查询：

```bash
journalctl -u course-cleanup.service
```

例如只查看最近日志：

```bash
journalctl -u course-cleanup.service -n 50
```

持续查看：

```bash
journalctl -u course-cleanup.service -f
```

这意味着 systemd timer 可以直接使用整个 systemd 服务管理体系：

```text
systemctl status
journalctl
依赖关系
启动顺序
失败状态
权限控制
资源限制
```

这正是复杂系统任务更适合 systemd timer 的主要原因。

---

## 9. 查看 Timer 是否正确运行

列出 timer：

```bash
systemctl list-timers
```

也可以只查看指定 timer：

```bash
systemctl status course-cleanup.timer
```

`systemctl list-timers` 通常会显示：

```text
NEXT
LEFT
LAST
PASSED
UNIT
ACTIVATES
```

重点可以检查：

```text
UNIT
course-cleanup.timer
```

以及：

```text
ACTIVATES
course-cleanup.service
```

这样就能确认 timer 是否正确绑定 service，以及下一次执行时间。

---

## 10. 推荐的 systemd Timer 调试流程

不要一开始就等待计划时间。

更可靠的方法是分别测试任务和调度。

### 第一步：测试 service

```bash
sudo systemctl start course-cleanup.service
```

然后：

```bash
systemctl status course-cleanup.service
```

以及：

```bash
journalctl -u course-cleanup.service
```

确认实际命令可以正确执行。

### 第二步：检查 timer

```bash
systemctl status course-cleanup.timer
```

或者：

```bash
systemctl list-timers
```

确认下一次触发时间正确。

这种方法可以快速区分：

```text
任务本身坏了
```

还是：

```text
调度配置错了
```

---

## 11. 完整配置示例

### `/etc/systemd/system/course-cleanup.service`

```ini
[Unit]
Description=Course cleanup task

[Service]
Type=oneshot
ExecStart=/usr/local/bin/course-cleanup
```

其中：

```text
/usr/local/bin/course-cleanup
```

代表实际需要运行的清理程序。部署时应替换成真实脚本或程序路径。

### `/etc/systemd/system/course-cleanup.timer`

```ini
[Unit]
Description=Run course cleanup daily at 02:30

[Timer]
OnCalendar=*-*-* 02:30:00
Persistent=true

[Install]
WantedBy=timers.target
```

配置完成后：

```bash
sudo systemctl daemon-reload
sudo systemctl enable --now course-cleanup.timer
```

检查：

```bash
systemctl status course-cleanup.timer
systemctl list-timers
```

手动测试实际清理任务：

```bash
sudo systemctl start course-cleanup.service
journalctl -u course-cleanup.service
```

---

## 12. 常见误区

### 误区 1：`enable` 就是立即启动

不是。

```bash
systemctl enable course-cleanup.timer
```

主要配置的是：

```text
以后开机时自动启动
```

若希望现在也启动，应使用：

```bash
systemctl enable --now course-cleanup.timer
```

---

### 误区 2：启动 timer 会立即执行 service

不一定。

```bash
systemctl start course-cleanup.timer
```

只是启动计时器。

如果配置：

```ini
OnCalendar=*-*-* 02:30:00
```

它通常会等待到下一个 02:30。

立即测试实际任务应该：

```bash
systemctl start course-cleanup.service
```

---

### 误区 3：修改 unit 后不需要 reload

修改：

```text
/etc/systemd/system/*.service
/etc/systemd/system/*.timer
```

后通常都应该执行：

```bash
sudo systemctl daemon-reload
```

否则 systemd 可能继续使用之前加载的定义。

---

### 误区 4：timer 自己执行命令

通常不是。

典型关系是：

```text
.timer
  │
  │ 到达计划时间
  ▼
.service
  │
  ▼
ExecStart
```

timer 管“什么时候”，service 管“做什么”。

---

## 13. systemd Timer 与 cron 的选择

简单、独立的定时命令使用 cron 往往已经足够。

如果任务需要更多系统级能力，例如：

* 统一使用 `journalctl` 查看日志；
* 明确检查成功或失败状态；
* 和其他 systemd 服务建立依赖关系；
* 控制启动顺序；
* 补执行关机期间错过的任务；
* 使用 systemd 的安全与资源限制；
* 希望任务和调度可以独立测试；

则 systemd timer 通常更合适。

核心模型可以记成：

```text
.timer = when
.service = what
```

以及标准部署流程：

```text
写 .service
    ↓
写 .timer
    ↓
systemctl daemon-reload
    ↓
systemctl enable --now xxx.timer
    ↓
systemctl start xxx.service   # 手动测试任务
    ↓
journalctl -u xxx.service     # 查看日志
```
