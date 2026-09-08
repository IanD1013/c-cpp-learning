# Linux 综合故障排查：systemd、日志统计、备份与 Cron

## 场景目标

一台 Linux 主机上的 `course-reporter` 部署没有完成，需要修复四个彼此独立的问题：

1. 修复并启动 `course-reporter.service`。
2. 从输入日志生成错误统计文件。
3. 备份生成报表所使用的输入目录。
4. 安装每周自动清理的 Cron 任务。

最终状态应满足：

* `course-reporter.service` 为 `active`
* `/var/lib/course-reporter/summary.tsv` 内容为：

```text
api	3
worker	2
```

* `/var/backups/course-reporter-inputs.tar.gz` 是整个 `inputs` 目录的压缩备份
* `/etc/cron.d/course-reporter-cleanup`

  * owner 为 `root`
  * mode 为 `644`
  * 每周日 `03:30`
  * 以 `root` 身份执行 `/usr/local/bin/course-cleanup`

---

# 1. 先修复 systemd 服务

## 核心原则：先读取错误，再修改配置

服务启动失败时，不要直接修改程序文件。首先应该查看 systemd 告诉你的实际错误：

```bash
sudo systemctl status course-reporter.service
```

如果信息不够，可以继续查看日志：

```bash
sudo journalctl -u course-reporter.service
```

重点寻找类似：

```text
No such file or directory
Failed at step EXEC
status=203/EXEC
```

这通常说明 systemd unit 中 `ExecStart=` 指定的程序路径不存在。

---

## 对比 unit 配置和实际文件

查看 unit：

```bash
sudo systemctl cat course-reporter.service
```

重点检查：

```ini
ExecStart=/some/path/program
```

然后查看实际安装的程序：

```bash
ls -l /usr/local/bin/
```

这里的问题是：

> unit 文件要求执行的程序名，与磁盘上真正存在的程序名只有几个字符的差异。

因此应该修复 **unit 文件中的路径**，而不是修改一个本身正常的程序。

找到实际 unit 文件：

```bash
systemctl show -p FragmentPath course-reporter.service
```

然后编辑对应文件，例如：

```bash
sudo nano /etc/systemd/system/course-reporter.service
```

把：

```ini
ExecStart=错误的程序路径
```

改成：

```ini
ExecStart=实际存在的程序路径
```

---

## 修改 unit 后必须 daemon-reload

systemd 会缓存 unit 配置，因此修改文件以后需要：

```bash
sudo systemctl daemon-reload
```

然后启动服务：

```bash
sudo systemctl start course-reporter.service
```

验证：

```bash
sudo systemctl status course-reporter.service
```

或者：

```bash
systemctl is-active course-reporter.service
```

预期：

```text
active
```

### 关键流程

```text
status / journalctl
        ↓
理解失败原因
        ↓
检查 ExecStart
        ↓
寻找实际程序
        ↓
修改 unit
        ↓
daemon-reload
        ↓
start
        ↓
验证 active
```

---

# 2. 从日志生成 `summary.tsv`

## 输入格式

输入文件位于：

```text
/var/lib/course-reporter/inputs
```

每行至少包含：

```text
component severity ...
```

例如：

```text
api ERROR request failed
worker INFO started
api ERROR timeout
worker ERROR job failed
```

其中：

* 第一列：component
* 第二列：severity

目标是：

1. 只选择 `ERROR`
2. 提取 component
3. 按 component 统计数量
4. 按 component 名字排序
5. 写入 TSV 文件

---

## 使用 `awk`

因为字段位置固定，`awk` 很适合完成“判断第二列，再输出第一列”：

```bash
awk '$2 == "ERROR" { print $1 }'
```

例如输入：

```text
api ERROR failure
worker INFO started
api ERROR timeout
```

输出：

```text
api
api
```

---

## `sort` + `uniq -c` 统计

`uniq` 只能统计**相邻的重复行**，所以必须先排序：

```bash
sort | uniq -c
```

例如：

```text
api
worker
api
api
worker
```

经过：

```bash
sort
```

变成：

```text
api
api
api
worker
worker
```

然后：

```bash
uniq -c
```

得到：

```text
      3 api
      2 worker
```

---

## 输出成 `component<TAB>count`

`uniq -c` 的顺序是：

```text
count component
```

但目标格式是：

```text
component<TAB>count
```

因此可以再次使用 `awk`：

```bash
awk '{ print $2 "\t" $1 }'
```

完整管道可以写成：

```bash
awk '$2 == "ERROR" { print $1 }' /var/lib/course-reporter/inputs/* \
  | sort \
  | uniq -c \
  | awk '{ print $2 "\t" $1 }' \
  > /var/lib/course-reporter/summary.tsv
```

因为第一次 `sort` 已经按照 component 排序，所以最终结果仍然按照 component 名字排列。

---

## 验证结果

```bash
cat /var/lib/course-reporter/summary.tsv
```

应该看到：

```text
api	3
worker	2
```

也可以用下面的命令检查 Tab 是否真的存在：

```bash
cat -A /var/lib/course-reporter/summary.tsv
```

Tab 通常显示为：

```text
api^I3$
worker^I2$
```

其中：

* `^I` = Tab
* `$` = 行尾

### 常见错误

#### 错误 1：直接使用 `grep ERROR`

例如：

```bash
grep ERROR *
```

虽然有时可以工作，但如果 `ERROR` 出现在消息正文而不是 severity 字段中，也可能误匹配。

如果日志具有明确字段结构，最好比较字段：

```bash
awk '$2 == "ERROR"'
```

#### 错误 2：直接 `uniq -c`

```bash
... | uniq -c
```

如果相同 component 没有相邻出现，统计会被分成多个组。

正确顺序：

```bash
sort | uniq -c
```

#### 错误 3：输出的是空格而不是 Tab

目标是 TSV，因此应明确使用：

```awk
"\t"
```

而不是：

```awk
" "
```

---

# 3. 备份生成报表所使用的输入

## 为什么应该先生成 summary，再备份

合理顺序是：

```text
读取 inputs
    ↓
生成 summary
    ↓
备份 inputs
```

这样保存下来的备份就是：

> 实际用于生成这份报表的那一组输入数据。

如果先备份、随后输入文件又发生变化，那么：

```text
summary 使用的数据 ≠ backup 保存的数据
```

会降低备份对于审计和故障分析的价值。

---

## 使用 `tar` 创建 gzip 压缩包

目标：

```text
/var/backups/course-reporter-inputs.tar.gz
```

需要保存整个：

```text
inputs
```

目录。

可以使用：

```bash
sudo tar -czf /var/backups/course-reporter-inputs.tar.gz \
  -C /var/lib/course-reporter inputs
```

参数含义：

| 参数       | 含义                |
| -------- | ----------------- |
| `-c`     | create，创建 archive |
| `-z`     | 使用 gzip 压缩        |
| `-f`     | 后面指定 archive 文件名  |
| `-C DIR` | 先进入指定目录再处理文件      |

因此：

```bash
-C /var/lib/course-reporter inputs
```

表示：

> 从 `/var/lib/course-reporter` 中打包 `inputs` 目录。

压缩包内部会类似：

```text
inputs/
inputs/file1
inputs/file2
```

而不会包含整条绝对路径。

---

## 创建备份后立即验证

不要因为 `tar` 没报错就假设 archive 一定可用。

列出内容：

```bash
tar -tzf /var/backups/course-reporter-inputs.tar.gz
```

参数：

| 参数   | 含义              |
| ---- | --------------- |
| `-t` | 列出 archive 内容   |
| `-z` | archive 使用 gzip |
| `-f` | 指定文件            |

应该至少看到：

```text
inputs/
```

以及里面的输入文件。

### 重要原则

备份操作应包含两步：

```text
创建备份
+
确认能够读取备份
```

只创建而不检查，并不能证明备份真正可用。

---

# 4. 创建每周 Cron 清理任务

## `/etc/cron.d` 格式

目标：

* 每周日
* `03:30`
* 以 `root` 身份运行
* 命令：

```text
/usr/local/bin/course-cleanup
```

`/etc/cron.d` 中的格式为：

```text
minute hour day-of-month month day-of-week user command
```

因此：

```cron
30 3 * * 0 root /usr/local/bin/course-cleanup
```

各字段含义：

| 字段           |                               值 | 含义          |
| ------------ | ------------------------------: | ----------- |
| minute       |                            `30` | 第 30 分钟     |
| hour         |                             `3` | 凌晨 3 点      |
| day of month |                             `*` | 每个日期        |
| month        |                             `*` | 每个月         |
| day of week  |                             `0` | 星期日         |
| user         |                          `root` | 以 root 身份运行 |
| command      | `/usr/local/bin/course-cleanup` | 执行的程序       |

---

## 创建文件

可以编辑：

```bash
sudo nano /etc/cron.d/course-reporter-cleanup
```

内容：

```cron
30 3 * * 0 root /usr/local/bin/course-cleanup
```

也可以直接写入：

```bash
printf '%s\n' '30 3 * * 0 root /usr/local/bin/course-cleanup' \
  | sudo tee /etc/cron.d/course-reporter-cleanup
```

---

## 设置 owner 和权限

目标要求：

```text
owner = root
mode = 644
```

执行：

```bash
sudo chown root:root /etc/cron.d/course-reporter-cleanup
sudo chmod 644 /etc/cron.d/course-reporter-cleanup
```

`644` 表示：

```text
owner:  rw-
group:  r--
others: r--
```

即：

* root 可以读写
* 其他用户只能读
* 非 root 用户不能修改 Cron 配置

验证：

```bash
ls -l /etc/cron.d/course-reporter-cleanup
```

应该类似：

```text
-rw-r--r-- 1 root root ... /etc/cron.d/course-reporter-cleanup
```

---

## `/etc/cron.d` 与 `crontab -e` 的重要区别

用户自己的：

```bash
crontab -e
```

通常只有五个时间字段：

```cron
30 3 * * 0 /path/to/command
```

但是 `/etc/cron.d` 属于系统级 Cron，需要额外指定运行用户：

```cron
30 3 * * 0 root /usr/local/bin/course-cleanup
             ^^^^
```

忘记 `root` 会导致后面的字段被错误解析。

---

# 5. 推荐的完整操作顺序

四项任务大部分彼此独立，但按照下面的顺序最清晰：

## 第一步：修复 service

```bash
sudo systemctl status course-reporter.service
sudo journalctl -u course-reporter.service
sudo systemctl cat course-reporter.service
```

找到 `ExecStart` 与实际程序之间的差异，修改 unit 后：

```bash
sudo systemctl daemon-reload
sudo systemctl start course-reporter.service
systemctl is-active course-reporter.service
```

---

## 第二步：生成 summary

```bash
awk '$2 == "ERROR" { print $1 }' /var/lib/course-reporter/inputs/* \
  | sort \
  | uniq -c \
  | awk '{ print $2 "\t" $1 }' \
  > /var/lib/course-reporter/summary.tsv
```

验证：

```bash
cat /var/lib/course-reporter/summary.tsv
```

目标：

```text
api	3
worker	2
```

---

## 第三步：备份 inputs

```bash
sudo tar -czf /var/backups/course-reporter-inputs.tar.gz \
  -C /var/lib/course-reporter inputs
```

立即检查：

```bash
tar -tzf /var/backups/course-reporter-inputs.tar.gz
```

---

## 第四步：安装 Cron

```bash
printf '%s\n' '30 3 * * 0 root /usr/local/bin/course-cleanup' \
  | sudo tee /etc/cron.d/course-reporter-cleanup
```

设置权限：

```bash
sudo chown root:root /etc/cron.d/course-reporter-cleanup
sudo chmod 644 /etc/cron.d/course-reporter-cleanup
```

验证：

```bash
cat /etc/cron.d/course-reporter-cleanup
ls -l /etc/cron.d/course-reporter-cleanup
```

---

# 6. 这类综合故障题真正考察的思路

这类任务并不是在考新的 Linux 命令，而是在考如何把已经学过的工具组合起来解决真实问题。

## 先观察，后修改

尤其对于服务故障：

```text
不要看到失败就猜
↓
先读取 status / journal
↓
根据证据定位配置
↓
只修改真正有问题的地方
```

例如程序本身已经正确安装，而 `ExecStart` 写错时，应修改 unit，而不是重命名或改写程序。

---

## 固定字段数据优先使用字段处理工具

如果已经知道：

```text
第一列 = component
第二列 = severity
```

那么：

```bash
awk '$2 == "ERROR"'
```

比搜索整行：

```bash
grep ERROR
```

更精确。

---

## 管道中的顺序代表数据处理逻辑

统计流程：

```text
原始记录
  ↓
筛选 ERROR
  ↓
提取 component
  ↓
sort：让相同 component 相邻
  ↓
uniq -c：统计数量
  ↓
重新格式化
  ↓
summary.tsv
```

每条命令只负责一个简单任务，组合起来完成复杂处理。

---

## 备份不仅要创建，还要验证

```bash
tar -czf ...
tar -tzf ...
```

分别回答两个不同问题：

```text
备份创建了吗？
备份还能被正确读取吗？
```

第二个问题同样重要。

---

## 定时任务不需要等待执行才能验证

安装 Cron 时，可以立即验证：

```bash
cat /etc/cron.d/course-reporter-cleanup
ls -l /etc/cron.d/course-reporter-cleanup
```

需要确认的是：

* 时间表达式正确
* 用户是 `root`
* 命令路径正确
* owner 正确
* mode 正确

题目并不要求真的等到星期日凌晨 03:30。

---

# 核心总结

这套任务可以归纳成四种常见 Linux 运维模式：

| 问题     | 核心工具                     | 核心思路                |
| ------ | ------------------------ | ------------------- |
| 服务无法启动 | `systemctl`、`journalctl` | 先读取错误，再修配置          |
| 日志统计   | `awk`、`sort`、`uniq`      | 筛选 → 提取 → 排序 → 统计   |
| 数据备份   | `tar`                    | 创建后立即验证             |
| 周期任务   | `/etc/cron.d`            | 时间 + 用户 + 命令 + 正确权限 |

最值得记住的不是某一条完整命令，而是处理顺序：

```text
观察证据
→ 精确定位
→ 做最小修改
→ 验证结果
```

对于存在数据依赖的步骤，还应保持：

```text
使用输入生成结果
→ 再备份这些输入
```

这样备份保存的就是实际产生该结果的数据版本。
