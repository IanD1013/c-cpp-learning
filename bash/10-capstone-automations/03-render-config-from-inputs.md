# Bash：安全验证端口并生成配置文件

## 目标

编写 `/home/learner/bash-lab/render-config.sh`，读取两个环境变量：

* `APP_NAME`：必填，不能缺失或为空。
* `PORT`：可选，缺失或为空时默认使用 `8080`。

只有当端口是 **1～65535 之间的规范十进制整数** 时，才生成：

```text
app.conf
```

内容格式：

```text
name=api
port=8080
```

关键原则是：

> **先验证输入是合法的十进制文本，再让 Bash 把它当数字计算。**

---

## 为什么不能直接用 Bash 算术判断？

下面的写法不够安全：

```bash
(( port <= 65535 ))
```

因为 Bash 的 `(( ... ))` 会把变量内容当作**算术表达式**解析，而不是单纯的十进制数字字符串。

例如：

```bash
PORT=1+1
```

如果直接执行：

```bash
(( port <= 65535 ))
```

Bash 会计算：

```text
1+1 → 2
```

于是非法输入竟然通过了。

还会出现其他问题：

| 输入      | 问题                    |
| ------- | --------------------- |
| `1+1`   | 被当作表达式计算              |
| `080`   | 可能按八进制解析，`8` 是非法八进制数字 |
| `12x`   | 产生算术解析错误              |
| 超大整数    | 可能发生整数溢出或回绕           |
| `00080` | 虽然看起来像数字，但不是要求的规范形式   |

因此正确顺序应该是：

```text
字符串格式验证
    ↓
确认只包含规范十进制数字
    ↓
数值范围验证
    ↓
写配置文件
```

---

## 完整脚本

创建：

```text
/home/learner/bash-lab/render-config.sh
```

内容如下：

```bash
#!/usr/bin/env bash

app_name=${APP_NAME:?APP_NAME is required}
port=${PORT:-8080}

[[ "$port" =~ ^[1-9][0-9]{0,4}$ ]] || exit 2
(( port <= 65535 )) || exit 2

cat > app.conf <<CONFIG
name=$app_name
port=$port
CONFIG
```

如果需要：

```bash
chmod +x render-config.sh
```

---

## 1. 必填环境变量：`${VAR:?message}`

```bash
app_name=${APP_NAME:?APP_NAME is required}
```

含义：

* `APP_NAME` 已设置且非空：把值赋给 `app_name`
* `APP_NAME` 未设置：退出
* `APP_NAME=""`：同样退出

例如：

```bash
APP_NAME=api ./render-config.sh
```

可以继续执行。

而：

```bash
./render-config.sh
```

会产生类似错误：

```text
APP_NAME: APP_NAME is required
```

并以状态码 `1` 退出。

### 为什么不是只使用 `set -u`？

```bash
set -u
```

主要拒绝**未设置变量**。

但这里要求：

```text
未设置 → 拒绝
空字符串 → 也拒绝
```

`${APP_NAME:?APP_NAME is required}` 更准确地表达了“这是一个必填值”。

---

## 2. 默认值：`${VAR:-default}`

```bash
port=${PORT:-8080}
```

表示：

```text
PORT 有非空值
    ↓
使用 PORT

PORT 未设置或为空
    ↓
使用 8080
```

例如：

```bash
APP_NAME=api ./render-config.sh
```

得到：

```text
port=8080
```

下面也会使用默认值：

```bash
APP_NAME=api PORT="" ./render-config.sh
```

结果仍然是：

```text
port=8080
```

注意：

```bash
port=${PORT:-8080}
```

只是给局部变量 `port` 一个值，并不会修改调用者的 `PORT`。

---

## 3. 第一层验证：确认是规范十进制文本

```bash
[[ "$port" =~ ^[1-9][0-9]{0,4}$ ]] || exit 2
```

正则：

```regex
^[1-9][0-9]{0,4}$
```

可以拆成：

| 部分           | 含义           |
| ------------ | ------------ |
| `^`          | 字符串开头        |
| `[1-9]`      | 第一位必须是 1～9   |
| `[0-9]{0,4}` | 后面最多再有 4 位数字 |
| `$`          | 字符串结尾        |

因此允许：

```text
1
80
8080
65535
99999
```

拒绝：

```text
0
01
080
+80
-1
1+1
12x
100000
```

### 为什么第一位必须 `[1-9]`？

因为端口不能是 `0`，同时还需要拒绝：

```text
01
080
00080
```

这样的前导零形式。

---

## 4. 第二层验证：检查真正的端口范围

第一层允许的最大值实际上是：

```text
99999
```

因此还需要：

```bash
(( port <= 65535 )) || exit 2
```

最终有效范围为：

```text
1 <= port <= 65535
```

例如：

```text
65535 → 通过
65536 → 拒绝
99999 → 拒绝
```

### 为什么现在可以安全使用 `(( ... ))`？

因为在执行算术判断之前，已经通过正则证明：

```text
port 只能由 1～5 位十进制数字组成
```

所以此时不可能再出现：

```text
1+1
12x
18446744073709551617
```

这样的算术表达式或超大整数。

这体现了一个重要安全模式：

> **不要先解析再判断是否合法，而应该先验证文本格式，再解析。**

---

## 5. `|| exit 2` 的含义

例如：

```bash
[[ "$port" =~ ^[1-9][0-9]{0,4}$ ]] || exit 2
```

可以理解为：

```text
如果验证成功
    → 继续

如果验证失败
    → exit 2
```

同理：

```bash
(( port <= 65535 )) || exit 2
```

用于拒绝超出最大端口的值。

这里使用状态码：

```text
1 → APP_NAME 缺失或为空
2 → PORT 非法
```

---

## 6. 为什么验证必须放在写文件之前？

正确顺序：

```bash
[[ ... ]] || exit 2
(( ... )) || exit 2

cat > app.conf <<CONFIG
...
CONFIG
```

这样非法输入会在执行 `cat > app.conf` 之前结束。

否则可能生成一个错误配置文件，例如：

```text
name=api
port=65536
```

本题要求：

> 非法端口必须在写 `/home/learner/bash-lab/app.conf` 之前被拒绝。

---

## 7. 使用 here-document 生成配置文件

```bash
cat > app.conf <<CONFIG
name=$app_name
port=$port
CONFIG
```

这里的：

```bash
<<CONFIG
```

叫做 **here-document（Here-doc）**。

它会把：

```text
name=$app_name
port=$port
```

作为 `cat` 的标准输入。

因为结束标记没有加引号：

```bash
<<CONFIG
```

所以变量会发生展开。

例如：

```bash
app_name=api
port=8080
```

最终生成：

```text
name=api
port=8080
```

### 结束标记必须单独一行

正确：

```bash
CONFIG
```

不要写成：

```bash
  CONFIG
```

普通 `<<CONFIG` 要求结束标记位于行首，并且该行不能有其他内容。

---

## 8. 完成本课要求

进入目录：

```bash
cd /home/learner/bash-lab
```

创建脚本：

```bash
cat > render-config.sh <<'EOF'
#!/usr/bin/env bash

app_name=${APP_NAME:?APP_NAME is required}
port=${PORT:-8080}

[[ "$port" =~ ^[1-9][0-9]{0,4}$ ]] || exit 2
(( port <= 65535 )) || exit 2

cat > app.conf <<CONFIG
name=$app_name
port=$port
CONFIG
EOF
```

然后执行要求中的初始运行：

```bash
APP_NAME=api ./render-config.sh
```

检查：

```bash
cat app.conf
```

应该得到：

```text
name=api
port=8080
```

---

## 测试所有关键情况

### 默认端口

```bash
APP_NAME=api ./render-config.sh
cat app.conf
```

应该得到：

```text
name=api
port=8080
```

### PORT 为空时使用默认值

```bash
APP_NAME=api PORT="" ./render-config.sh
cat app.conf
```

应该仍然得到：

```text
name=api
port=8080
```

### 最小合法端口

```bash
APP_NAME=api PORT=1 ./render-config.sh
cat app.conf
```

结果：

```text
name=api
port=1
```

### 最大合法端口

```bash
APP_NAME=api PORT=65535 ./render-config.sh
cat app.conf
```

结果：

```text
name=api
port=65535
```

### 拒绝 0

```bash
APP_NAME=api PORT=0 ./render-config.sh
echo $?
```

应该返回：

```text
2
```

### 拒绝超过最大值

```bash
APP_NAME=api PORT=65536 ./render-config.sh
echo $?
```

应该返回：

```text
2
```

### 拒绝前导零

```bash
APP_NAME=api PORT=080 ./render-config.sh
echo $?
```

应该返回：

```text
2
```

### 拒绝表达式

```bash
APP_NAME=api PORT='1+1' ./render-config.sh
echo $?
```

应该返回：

```text
2
```

而不是把它计算成 `2`。

### 拒绝超大整数

```bash
APP_NAME=api PORT=18446744073709551617 ./render-config.sh
echo $?
```

应该返回：

```text
2
```

关键在于它会被**正则检查先拒绝**，因此不会进入 Bash 算术运算，也就不会发生整数回绕。

---

## 一个容易忽略的测试问题：旧的 `app.conf`

脚本保证的是：

> 本次非法运行不会执行写文件操作。

如果目录里已经存在上一次成功运行生成的：

```text
app.conf
```

那么非法运行并不会自动删除这个旧文件。

因此手动测试“非法输入不能生成文件”时，可以先：

```bash
rm -f app.conf
```

然后：

```bash
APP_NAME=api PORT='1+1' ./render-config.sh
```

最后检查：

```bash
test ! -e app.conf && echo "没有生成 app.conf"
```

---

## 核心知识总结

安全处理“数字形式的外部输入”时，应区分两个问题：

```text
这个字符串长得像合法数字吗？
              ↓
[[ ... =~ regex ... ]]

这个合法数字是否在允许范围内？
              ↓
(( ... ))
```

本例的核心代码是：

```bash
app_name=${APP_NAME:?APP_NAME is required}
port=${PORT:-8080}

[[ "$port" =~ ^[1-9][0-9]{0,4}$ ]] || exit 2
(( port <= 65535 )) || exit 2
```

其中：

* `${VAR:?message}`：必填值，缺失或为空就失败。
* `${VAR:-default}`：缺失或为空时使用默认值。
* `[[ ... =~ ... ]]`：先验证输入的文本形状。
* `(( ... ))`：只在文本已经安全后进行数值判断。
* `|| exit 2`：验证失败立即停止。
* Here-doc：方便生成包含变量值的多行配置文件。

最重要的原则是：

> **外部输入不能因为“看起来应该是数字”就直接交给算术解析器。先验证语法，再验证范围。**
