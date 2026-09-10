# Bash `set -u` 与环境变量：区分“必需变量”和“可选变量”

## 目标

编写一个 `settings.sh`，从环境变量读取：

* `APP_NAME`：必需。如果没有提供，脚本立即失败。
* `LOG_LEVEL`：可选。如果没有提供或为空，则默认使用 `info`。

最终把两个值写入 `settings.txt`：

```text
<APP_NAME>
<LOG_LEVEL>
```

例如：

```text
api
info
```

---

## 1. Bash 默认不会报错：未设置变量会变成空字符串

默认情况下：

```bash
app_name=$APP_NAME
```

如果 `APP_NAME` 根本没有定义，Bash 通常会把它展开为空字符串：

```bash
app_name=
```

这很危险，因为脚本可能继续执行，并且看起来“运行成功”，但实际上已经使用了错误的数据。

例如：

```bash
printf '%s\n' "$app_name" > settings.txt
```

可能会悄悄写入一个空行。

---

## 2. `set -u`：使用未定义变量时立即失败

```bash
set -u
```

也叫 **nounset**。

启用之后，如果代码直接使用一个从未定义过的变量：

```bash
app_name=$APP_NAME
```

而 `APP_NAME` 不存在，Bash 会报类似：

```text
APP_NAME: unbound variable
```

并停止非交互式脚本。

因此：

```bash
set -u
app_name=$APP_NAME
```

可以表达：

> `APP_NAME` 是必需变量，没有它脚本就不能继续。

### 为什么 `set -u` 必须放在前面？

错误：

```bash
app_name=$APP_NAME
set -u
```

读取 `APP_NAME` 时 nounset 还没有启用，因此检查没有意义。

正确：

```bash
set -u
app_name=$APP_NAME
```

---

## 3. `set -u` 只检查“未定义”，不检查“空字符串”

下面两种情况不同。

### 完全没有定义变量

```bash
unset APP_NAME
bash settings.sh
```

使用：

```bash
$APP_NAME
```

会触发 `set -u`。

### 变量存在，但值为空

```bash
APP_NAME='' bash settings.sh
```

这里 `APP_NAME` 已经存在，只不过值是：

```text
""
```

因此 `set -u` 不会报错。

可以记成：

| 状态        | `set -u` 是否报错 |
| --------- | ------------- |
| 变量未设置     | 是             |
| 变量设置为空字符串 | 否             |
| 变量有正常值    | 否             |

所以 `set -u` 的含义不是：

> 禁止空值

而是：

> 禁止直接读取不存在的变量。

---

## 4. 可选变量应该显式提供默认值

`LOG_LEVEL` 是可选配置，所以不应该直接写：

```bash
log_level=$LOG_LEVEL
```

否则启用 `set -u` 后，用户没有设置它就会导致脚本失败。

应该使用：

```bash
log_level=${LOG_LEVEL:-info}
```

含义是：

> 如果 `LOG_LEVEL` 已设置并且非空，使用它；否则使用 `info`。

例如：

```bash
LOG_LEVEL=debug
```

结果：

```bash
log_level=debug
```

如果：

```bash
unset LOG_LEVEL
```

结果：

```bash
log_level=info
```

如果：

```bash
LOG_LEVEL=''
```

仍然得到：

```bash
log_level=info
```

---

## 5. 为什么 `${LOG_LEVEL:-info}` 在 `set -u` 下不会报错？

因为 Bash 的参数展开：

```bash
${LOG_LEVEL:-info}
```

已经明确告诉 Bash：

> 如果这个变量不存在，就使用备用值。

因此即使有：

```bash
set -u
```

下面的代码仍然安全：

```bash
log_level=${LOG_LEVEL:-info}
```

这体现了一个重要设计思想：

```bash
$APP_NAME
```

表示：

> 我要求这个变量必须存在。

而：

```bash
${LOG_LEVEL:-info}
```

表示：

> 这个变量可以不存在，因为我已经规定了默认值。

---

## 6. `${VAR:-default}` 中冒号的作用

形式：

```bash
${VAR:-default}
```

会在两种情况下使用默认值：

1. `VAR` 未设置；
2. `VAR` 被设置为空字符串。

例如：

```bash
VAR=''
echo "${VAR:-hello}"
```

输出：

```text
hello
```

如果使用：

```bash
${VAR-default}
```

则只有变量**完全未设置**时才使用默认值。

如果：

```bash
VAR=''
```

它仍然会保留空字符串。

因此：

```bash
${LOG_LEVEL:-info}
```

通常更适合配置文件中的默认值。

---

## 7. 完整的 `settings.sh`

```bash
set -u
app_name=$APP_NAME
log_level=${LOG_LEVEL:-info}

printf '%s\n' "$app_name" "$log_level" > settings.txt
```

前三行必须保持：

```bash
set -u
app_name=$APP_NAME
log_level=${LOG_LEVEL:-info}
```

### 最后一行做了什么？

```bash
printf '%s\n' "$app_name" "$log_level" > settings.txt
```

`printf` 会依次打印两个值：

```bash
"$app_name"
"$log_level"
```

格式：

```bash
'%s\n'
```

表示每个值后面添加一个换行。

因此：

```bash
APP_NAME=api
LOG_LEVEL=debug
```

会得到：

```text
api
debug
```

---

## 8. 如何运行这个练习

进入目录：

```bash
cd /home/learner/bash-lab
```

创建：

```bash
settings.sh
```

内容：

```bash
set -u
app_name=$APP_NAME
log_level=${LOG_LEVEL:-info}

printf '%s\n' "$app_name" "$log_level" > settings.txt
```

然后按照要求运行：

```bash
APP_NAME=api bash settings.sh
```

这里没有提供 `LOG_LEVEL`，因此：

```bash
LOG_LEVEL
```

会使用默认值：

```text
info
```

检查结果：

```bash
cat settings.txt
```

应该看到：

```text
api
info
```

---

## 9. `NAME=value command`：只给一个命令传环境变量

这一点非常重要。

下面这样：

```bash
APP_NAME=api
bash settings.sh
```

**不一定会把 `APP_NAME` 传给子进程。**

第一行只是创建当前 shell 的普通 shell 变量：

```bash
APP_NAME=api
```

而：

```bash
bash settings.sh
```

启动了一个新的 Bash 子进程。

普通 shell 变量默认不会自动进入子进程的环境。

因此脚本可能仍然发现：

```bash
APP_NAME
```

不存在。

---

## 10. 正确方式：把赋值直接写在命令前

```bash
APP_NAME=api bash settings.sh
```

语法：

```bash
NAME=value command
```

表示：

> 为这个 command 临时提供一个环境变量。

因此 `settings.sh` 可以读取：

```bash
$APP_NAME
```

但命令执行结束后，并不会永久修改当前 shell。

例如：

```bash
APP_NAME=api bash settings.sh
echo "$APP_NAME"
```

如果当前 shell 原本没有 `APP_NAME`，第二条命令不会因为前面的临时赋值而获得 `api`。

---

## 11. 如果想让后续子进程都获得变量：`export`

另一种方式是：

```bash
export APP_NAME=api
bash settings.sh
```

`export` 会把变量标记为环境变量，使之后启动的子进程能够继承它。

区别：

```bash
APP_NAME=api bash settings.sh
```

只影响这一次命令。

而：

```bash
export APP_NAME=api
```

会影响当前 shell 之后启动的子进程，直到变量被修改、取消或当前 shell 结束。

对于一次性运行测试，通常：

```bash
APP_NAME=api bash settings.sh
```

更加干净。

---

## 12. 关键测试场景

### 场景一：提供必需变量，不提供可选变量

```bash
APP_NAME=api bash settings.sh
```

结果：

```text
api
info
```

因为：

```bash
LOG_LEVEL
```

使用默认值 `info`。

---

### 场景二：两个变量都提供

```bash
APP_NAME=api LOG_LEVEL=debug bash settings.sh
```

结果：

```text
api
debug
```

此时不会使用默认值。

---

### 场景三：没有提供 `APP_NAME`

```bash
bash settings.sh
```

脚本会在：

```bash
app_name=$APP_NAME
```

这里失败，出现类似：

```text
APP_NAME: unbound variable
```

并且后面的：

```bash
printf ...
```

不会执行。

因此 `settings.txt` 不应该因为这次运行而被创建或重写。

---

## 13. 常见误区

### 误区一：`set -u` 会拒绝空字符串

不会。

```bash
APP_NAME='' bash settings.sh
```

`APP_NAME` 已经存在，因此不会触发 nounset。

---

### 误区二：启用 `set -u` 后不能使用默认值

错误。

下面是完全合法的：

```bash
set -u
log_level=${LOG_LEVEL:-info}
```

默认值展开本来就是安全处理“变量不存在”的机制。

---

### 误区三：赋值后运行脚本，脚本一定能看到变量

```bash
APP_NAME=api
bash settings.sh
```

这里 `APP_NAME` 只是普通 shell 变量，不一定进入子进程环境。

应使用：

```bash
APP_NAME=api bash settings.sh
```

或者：

```bash
export APP_NAME=api
bash settings.sh
```

---

## 14. 核心模式

对于 Bash 配置脚本，可以采用非常清晰的设计：

### 必需配置

```bash
set -u
required=$REQUIRED_VAR
```

没有配置就失败。

### 可选配置

```bash
optional=${OPTIONAL_VAR:-default_value}
```

没有配置就使用明确的默认值。

这种写法可以让脚本清楚地区分：

> **调用者必须提供什么，以及脚本自己能够决定什么。**
