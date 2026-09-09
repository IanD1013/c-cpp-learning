# Bash Here Document：生成带变量展开的配置文件

## 1. 核心概念

Bash 的 **here-document（Here Doc）** 可以把多行文本直接作为命令的标准输入：

```bash
cat > app.conf <<CONFIG
name=api
port=8080
CONFIG
```

可以把它理解为：

> Bash 收集 `CONFIG` 与结束标记 `CONFIG` 之间的文本，然后把这些内容通过标准输入交给 `cat`，再由 `>` 写入文件。

最终 `app.conf`：

```text
name=api
port=8080
```

Here Doc 是否进行变量展开，关键取决于 **开始标记是否被引用**。

---

## 2. `<<'CONFIG'` 与 `<<CONFIG` 的区别

### 引用 delimiter：不展开变量

```bash
cat > script.sh <<'SCRIPT'
echo "$1"
echo "$HOME"
SCRIPT
```

这里的 `'SCRIPT'` 被单引号引用，因此 Here Doc 内容基本按字面值保留。

写入文件的是：

```bash
echo "$1"
echo "$HOME"
```

而不是当前 Shell 中 `$1` 或 `$HOME` 的值。

这种方式特别适合：

* 生成 Shell 脚本
* 生成模板
* 文件中需要保留 `$variable`
* 不希望当前 Bash 修改内容

---

### 不引用 delimiter：进行变量展开

```bash
port=8080

cat > app.conf <<CONFIG
port=$port
CONFIG
```

`CONFIG` 没有加引号，因此 Bash 会先展开 `$port`。

最终文件：

```text
port=8080
```

这种方式适合：

* 根据脚本参数生成配置文件
* 把环境变量写入文件
* 根据运行时计算结果渲染配置

可以记成：

| 写法        | Here Doc 内容是否展开 |
| --------- | --------------- |
| `<<'EOF'` | ❌ 不展开           |
| `<<"EOF"` | ❌ 不展开           |
| `<<\EOF`  | ❌ 不展开           |
| `<<EOF`   | ✅ 展开            |

---

## 3. 未引用 Here Doc 会展开什么

例如：

```bash
cat <<EOF
user=$USER
home=$HOME
result=$((2 + 3))
date=$(date)
EOF
```

未引用 delimiter 时，Bash 会执行主要的：

* 参数展开：`$USER`
* 命令替换：`$(date)`
* 算术展开：`$((2 + 3))`

因此它不是简单地“复制文本”。

这也是为什么：

```bash
cat <<EOF
path=$RUNTIME_DIR/app.log
EOF
```

会尝试立即展开 `$RUNTIME_DIR`。

---

## 4. 如何让某个 `$变量` 保留到生成后的文件中

假设希望生成：

```text
log=$RUNTIME_DIR/app.log
```

而不是现在就展开 `$RUNTIME_DIR`。

在未引用的 Here Doc 中，可以转义 `$`：

```bash
cat > app.conf <<CONFIG
log=\$RUNTIME_DIR/app.log
CONFIG
```

这里：

```text
\$
```

告诉当前 Bash：

> 不要把这个 `$` 当成变量展开符。

最终文件得到：

```text
log=$RUNTIME_DIR/app.log
```

注意：反斜杠本身不会写进最终文件。

---

## 5. 一个典型场景：根据脚本参数生成配置文件

目标是编写：

```text
make-config.sh
```

脚本接受：

```text
$1 = 服务名称
$2 = 端口号
```

例如运行：

```bash
bash make-config.sh api 8080
```

希望生成：

```text
name=api
port=8080
mode=prod
log=$RUNTIME_DIR/app.log
```

其中：

* `api` 来自 `$1`
* `8080` 来自 `$2`
* `prod` 是固定值
* `$RUNTIME_DIR` 必须保持原样，留给未来启动服务时再展开

---

## 6. 完整脚本

```bash
#!/usr/bin/env bash

service=$1
port=$2

cat > app.conf <<CONFIG
name=$service
port=$port
mode=prod
log=\$RUNTIME_DIR/app.log
CONFIG
```

逐行理解。

### 读取位置参数

```bash
service=$1
port=$2
```

如果运行：

```bash
bash make-config.sh api 8080
```

那么：

```text
service=api
port=8080
```

---

### 开始 Here Doc

```bash
cat > app.conf <<CONFIG
```

包含两个独立动作：

```bash
> app.conf
```

表示把 `cat` 的标准输出写入 `app.conf`。

```bash
<<CONFIG
```

表示下面直到 `CONFIG` 为止的文本作为 `cat` 的标准输入。

---

### 展开脚本变量

```bash
name=$service
port=$port
```

因为 delimiter `CONFIG` 没有加引号，所以 Bash 会展开变量。

因此：

```bash
name=$service
```

变成：

```text
name=api
```

而：

```bash
port=$port
```

变成：

```text
port=8080
```

---

### 固定文本

```bash
mode=prod
```

没有任何需要展开的内容，因此直接写入：

```text
mode=prod
```

---

### 保留未来需要展开的变量

```bash
log=\$RUNTIME_DIR/app.log
```

当前 Bash 不展开 `$RUNTIME_DIR`。

生成文件中得到：

```text
log=$RUNTIME_DIR/app.log
```

这样以后真正读取这个配置的程序或 Shell 可以决定如何处理 `$RUNTIME_DIR`。

---

## 7. 运行结果

假设当前目录是：

```text
/home/learner/bash-lab
```

进入目录：

```bash
cd /home/learner/bash-lab
```

运行：

```bash
bash make-config.sh api 8080
```

查看：

```bash
cat app.conf
```

得到：

```text
name=api
port=8080
mode=prod
log=$RUNTIME_DIR/app.log
```

---

## 8. 为什么运行第二次不会变成 8 行

脚本使用的是：

```bash
cat > app.conf
```

其中：

```bash
>
```

会在打开文件时 **截断（truncate）原文件**。

也就是先把原文件清空，再写入新的内容。

所以连续运行两次：

```bash
bash make-config.sh api 8080
bash make-config.sh api 8080
```

文件仍然只有：

```text
name=api
port=8080
mode=prod
log=$RUNTIME_DIR/app.log
```

不会出现两份内容。

### 与 `>>` 对比

如果错误地写成：

```bash
cat >> app.conf <<CONFIG
...
CONFIG
```

那么：

```bash
>>
```

表示追加。

连续执行两次后可能得到：

```text
name=api
port=8080
mode=prod
log=$RUNTIME_DIR/app.log
name=api
port=8080
mode=prod
log=$RUNTIME_DIR/app.log
```

因此，重新生成完整配置文件通常应该使用：

```bash
>
```

而不是：

```bash
>>
```

---

## 9. 再次运行可以完全替换旧配置

第一次：

```bash
bash make-config.sh api 8080
```

生成：

```text
name=api
port=8080
mode=prod
log=$RUNTIME_DIR/app.log
```

之后运行：

```bash
bash make-config.sh worker 9000
```

旧文件会先被截断，然后重新生成：

```text
name=worker
port=9000
mode=prod
log=$RUNTIME_DIR/app.log
```

这种模式非常适合：

> 根据当前参数重新渲染整个配置文件。

---

## 10. Here Doc 结束标记必须严格匹配

正确：

```bash
cat <<CONFIG
hello
CONFIG
```

结束标记通常必须：

* 与开始时的 delimiter 完全相同；
* 单独占一行；
* 前后不能随意增加普通空格；
* 大小写必须完全一致。

错误：

```bash
cat <<CONFIG
hello
 config
```

或者：

```bash
cat <<CONFIG
hello
CONFIG 
```

或者：

```bash
cat <<CONFIG
hello
config
```

Bash 找不到正确结束标记时，会继续读取后面的内容，直到遇到真正的 delimiter 或文件结束。

在脚本结束前始终没有找到时，通常会出现类似警告：

```text
warning: here-document ... delimited by end-of-file
```

而原本以为属于后续 Shell 代码的内容，也可能被当成 Here Doc 数据。

---

## 11. 一个容易忽略的问题：`app.conf` 写在哪里

脚本中使用的是：

```bash
cat > app.conf
```

`app.conf` 是一个 **相对路径**。

它是相对于运行脚本时的 **当前工作目录（current working directory）**，而不是自动相对于脚本文件的位置。

例如：

```bash
cd /home/learner/bash-lab
bash make-config.sh api 8080
```

会产生：

```text
/home/learner/bash-lab/app.conf
```

但如果在别处运行：

```bash
cd /tmp
bash /home/learner/bash-lab/make-config.sh api 8080
```

则会产生：

```text
/tmp/app.conf
```

并不会自动产生在：

```text
/home/learner/bash-lab/app.conf
```

如果要求无论从哪里运行，都把配置写到脚本所在目录，则需要显式计算脚本目录。

---

## 12. 常见误区

### 误区 1：以为所有 Here Doc 都会展开变量

不是。

```bash
<<EOF
```

会展开，而：

```bash
<<'EOF'
```

不会展开。

---

### 误区 2：需要保留 `$VAR`，却忘记转义

错误：

```bash
cat <<EOF
log=$RUNTIME_DIR/app.log
EOF
```

如果 `$RUNTIME_DIR` 当前为空，可能生成：

```text
log=/app.log
```

如果目标是保留变量名，应写：

```bash
log=\$RUNTIME_DIR/app.log
```

---

### 误区 3：认为 `>` 是追加

```bash
>
```

是覆盖/截断。

```bash
>>
```

才是追加。

因此：

```bash
cat > app.conf
```

特别适合“每次重新生成配置文件”。

---

### 误区 4：认为配置文件中的 `$RUNTIME_DIR` 一定会自动展开

生成：

```text
log=$RUNTIME_DIR/app.log
```

只意味着文件中保存了这些字符。

它之后是否会被展开，取决于 **读取这个配置文件的程序**。

例如 Shell 执行：

```bash
source app.conf
```

时会按 Shell 语法解释 `$RUNTIME_DIR`。

但许多普通配置文件解析器并不会自动进行 Shell 环境变量展开。

因此必须确认目标程序是否支持这种语法。

---

## 13. 最重要的判断原则

生成文件时，先问自己：

> 文件中的 `$variable` 应该“现在展开”，还是“以后再展开”？

如果希望现在展开：

```bash
cat <<EOF
port=$port
EOF
```

如果希望原样保存：

```bash
cat <<EOF
path=\$RUNTIME_DIR/app.log
EOF
```

如果整个 Here Doc 基本都应该保持原样，则更适合直接引用 delimiter：

```bash
cat <<'EOF'
$HOME
$USER
$1
EOF
```

---

## 14. 核心速记

```bash
# 展开变量
cat <<EOF
value=$var
EOF
```

```bash
# 整个 Here Doc 不展开
cat <<'EOF'
value=$var
EOF
```

```bash
# Here Doc 整体允许展开，但单独保留某个 $
cat <<EOF
current=$var
future=\$OTHER_VAR
EOF
```

```bash
# 覆盖文件
cat > file <<EOF
...
EOF
```

```bash
# 追加文件
cat >> file <<EOF
...
EOF
```

最终可以把 Here Doc 的选择原则概括为：

> **引用 delimiter 用于保存模板；不引用 delimiter 用于渲染模板；在可展开的 Here Doc 中用 `\$` 保留需要以后处理的变量。**
