# systemd 服务的当前运行状态与开机自启动

## 核心概念：`running` 和 `enabled` 是两回事

在 Linux 中安装一个服务软件，例如 Nginx，并不意味着它会立刻运行。

安装软件通常只是完成两件事：

1. 把程序文件安装到系统中；
2. 把对应的 `systemd` unit 文件安装好，例如：

```text
nginx.service
```

至于：

* **现在要不要运行**
* **以后开机时要不要自动运行**

是两个独立的问题，由 `systemd` 分别管理。

---

## systemd 与 service unit

`systemd` 是现代 Linux 系统常见的初始化和服务管理系统。系统启动时，它通常作为 PID 1 运行，并负责启动、停止和监督其他服务。

每个被管理的对象使用一个 **unit（单元）** 描述。

常见的服务 unit：

```text
nginx.service
ssh.service
docker.service
```

一个 `.service` 文件通常会描述：

* 应该启动哪个程序；
* 启动前依赖什么；
* 服务如何停止；
* 服务退出后是否重启；
* 服务与其他 unit 的启动关系。

---

## 查看服务状态：`systemctl status`

检查服务时，通常首先运行：

```bash
systemctl status nginx
```

输出中最容易混淆的是两种状态：

```text
Loaded: loaded (...; enabled; ...)
Active: inactive (dead)
```

它们回答的是完全不同的问题。

### `Active:`：服务现在是否正在运行

例如：

```text
Active: active (running)
```

表示：

> Nginx 当前正在运行。

而：

```text
Active: inactive (dead)
```

表示：

> Nginx 当前没有运行。

因此，`Active:` 关注的是**现在这一刻的运行状态**。

---

### `Loaded:` 中的 `enabled/disabled`：是否开机自启动

例如：

```text
Loaded: loaded (...; enabled; ...)
```

其中：

```text
enabled
```

表示：

> 系统以后启动时，systemd 会按照配置自动启动这个服务。

而：

```text
disabled
```

表示：

> 该服务默认不会因为系统启动而自动启动。

这里讨论的是**未来开机时的行为**，不是服务当前是否运行。

---

## 四种常见状态组合

因为“当前运行”和“开机自启动”互相独立，所以存在四种组合：

| 当前状态       | 开机状态       | 含义               |
| ---------- | ---------- | ---------------- |
| `active`   | `enabled`  | 现在运行，开机也会自动启动    |
| `active`   | `disabled` | 现在运行，但下次开机不会自动启动 |
| `inactive` | `enabled`  | 现在没运行，但下次开机会自动启动 |
| `inactive` | `disabled` | 现在没运行，开机也不会自动启动  |

其中最容易误解的是：

```text
inactive + enabled
```

这完全正常。

它表示：

> 服务现在没有运行，但系统重启后会自动运行。

---

## `start`：只改变当前状态

启动服务：

```bash
sudo systemctl start nginx
```

作用是：

> 立即启动 Nginx。

它不会决定 Nginx 下次开机时是否自动启动。

例如原来的状态：

```text
Active: inactive
Loaded: ... disabled
```

执行：

```bash
sudo systemctl start nginx
```

之后可能变成：

```text
Active: active
Loaded: ... disabled
```

也就是：

> 现在运行，但重启之后不会自动启动。

---

## `enable`：只改变开机行为

启用开机自启动：

```bash
sudo systemctl enable nginx
```

作用是：

> 配置 systemd，使 Nginx 在以后合适的系统启动阶段自动启动。

它通常不会立即启动当前服务。

因此，如果原来：

```text
Active: inactive
Loaded: ... disabled
```

执行：

```bash
sudo systemctl enable nginx
```

之后可能变成：

```text
Active: inactive
Loaded: ... enabled
```

服务仍然没有运行，只是以后开机会自动启动。

---

## `enable --now`：同时完成两件事

如果希望：

1. 现在立即启动；
2. 以后开机自动启动；

可以运行：

```bash
sudo systemctl enable --now nginx
```

这基本等价于：

```bash
sudo systemctl enable nginx
sudo systemctl start nginx
```

`--now` 的存在也说明了一个重要事实：

> `enable` 和 `start` 本来就是两个独立操作。

---

## 对应的停止与禁用命令

同样，停止服务和取消开机自启动也是两个不同操作。

### 只停止当前服务

```bash
sudo systemctl stop nginx
```

服务现在停止，但如果它仍然是 `enabled`，下次开机仍可能自动启动。

---

### 取消开机自启动

```bash
sudo systemctl disable nginx
```

这不会自动停止当前正在运行的服务。

例如可能出现：

```text
Active: active
Loaded: ... disabled
```

表示：

> 服务现在仍在运行，但下次开机不会自动启动。

---

### 同时禁用并立即停止

常见写法：

```bash
sudo systemctl disable --now nginx
```

效果是：

* 现在停止服务；
* 以后开机也不自动启动。

---

## 常用命令对照

| 命令                              | 当前运行状态 | 开机自启动状态 |
| ------------------------------- | ------ | ------- |
| `systemctl start nginx`         | 启动     | 不改变     |
| `systemctl stop nginx`          | 停止     | 不改变     |
| `systemctl enable nginx`        | 不改变    | 启用      |
| `systemctl disable nginx`       | 不改变    | 禁用      |
| `systemctl enable --now nginx`  | 启动     | 启用      |
| `systemctl disable --now nginx` | 停止     | 禁用      |
| `systemctl status nginx`        | 查看     | 查看      |

---

## 推荐的排查流程

当一个已安装的服务没有工作时，不要直接假设安装失败。

先查看：

```bash
systemctl status nginx
```

重点检查：

```text
Loaded:
Active:
```

假设看到：

```text
Loaded: loaded (...; enabled; ...)
Active: inactive (dead)
```

正确理解是：

> Nginx 已配置为开机自动启动，但当前没有运行。

此时只需要启动当前服务：

```bash
sudo systemctl start nginx
```

然后再次检查：

```bash
systemctl status nginx
```

应该能看到类似：

```text
Active: active (running)
```

因为它本来已经是 `enabled`，所以没有必要重复执行：

```bash
sudo systemctl enable nginx
```

---

## 为什么应该查看操作前后的状态

推荐操作方式：

```bash
systemctl status nginx

sudo systemctl start nginx

systemctl status nginx
```

这样可以直接观察：

```text
Active: inactive
```

变为：

```text
Active: active (running)
```

相比“执行命令后假设它成功了”，这种方式可以确认实际系统状态。

如果启动失败，`systemctl status` 还经常会显示错误信息或相关日志线索。

---

## 常见误区

### 误区 1：安装了服务，就应该已经在运行

不一定。

```bash
sudo apt install nginx
```

和：

```bash
sudo systemctl start nginx
```

解决的是不同问题。

前者安装软件，后者控制运行状态。

---

### 误区 2：`enabled` 表示服务正在运行

错误。

```text
enabled
```

主要表示：

> 以后系统启动时应该自动启动。

判断当前是否运行应该看：

```text
Active: active (running)
```

---

### 误区 3：`systemctl start` 会自动配置开机启动

不会。

```bash
sudo systemctl start nginx
```

只控制当前运行状态。

---

### 误区 4：`systemctl enable` 会立即启动服务

通常不会。

如果需要立即启动并开启自启动，应使用：

```bash
sudo systemctl enable --now nginx
```

---

## 记忆方法

把 systemd 服务管理拆成两个问题：

```text
现在运行吗？     → start / stop
以后开机运行吗？ → enable / disable
```

对应状态：

```text
Active:     → 现在
enabled:    → 下次开机
```

最重要的原则是：

> **`start/stop` 管“现在”，`enable/disable` 管“开机以后”。**
