# Bash `case` 语句：根据命令参数选择分支

## 1. 为什么使用 `case`

当 Bash 脚本需要根据一个字符串执行不同操作时，可以使用多个 `if / elif`：

```bash
if [[ "$1" == "start" ]]; then
  ...
elif [[ "$1" == "stop" ]]; then
  ...
elif [[ "$1" == "status" ]]; then
  ...
fi
```

但这种写法会不断重复：

```bash
"$1"
```

如果所有判断都围绕**同一个值**展开，`case` 通常更清晰：

```bash
case "$1" in
  start)
    ...
    ;;
  stop)
    ...
    ;;
  status)
    ...
    ;;
esac
```

核心思想是：

> `case` 读取一个值，然后从上到下检查多个 shell pattern（Shell 模式），执行第一个匹配的分支。

---

## 2. `case` 的基本语法

```bash
case "$1" in
  pattern1)
    commands
    ;;
  pattern2)
    commands
    ;;
  *)
    commands
    ;;
esac
```

各部分含义：

| 语法             | 含义             |
| -------------- | -------------- |
| `case "$1" in` | 获取要判断的值        |
| `pattern)`     | 定义一个匹配模式       |
| `commands`     | 匹配成功后执行的命令     |
| `;;`           | 结束当前分支         |
| `*)`           | 默认分支，匹配剩余所有情况  |
| `esac`         | 结束整个 `case` 结构 |

`esac` 就是 `case` 反过来写。

---

## 3. 一个完整示例

创建 `action.sh`：

```bash
#!/usr/bin/env bash

case "$1" in
  start)
    printf 'starting\n'
    ;;
  stop)
    printf 'stopping\n'
    ;;
  status)
    printf 'checking\n'
    ;;
  *)
    printf 'unknown\n'
    ;;
esac
```

运行：

```bash
bash action.sh start
```

输出：

```text
starting
```

运行：

```bash
bash action.sh stop
```

输出：

```text
stopping
```

运行：

```bash
bash action.sh status
```

输出：

```text
checking
```

如果传入没有定义的命令：

```bash
bash action.sh reload
```

输出：

```text
unknown
```

---

## 4. `$1` 表示什么

在 Bash 脚本中：

```bash
$1
```

表示脚本收到的**第一个位置参数**。

例如：

```bash
bash action.sh start
```

这里：

```text
$1 = start
```

因此：

```bash
case "$1" in
```

实际上相当于让 Bash 判断字符串：

```text
start
```

应该匹配哪个分支。

---

## 5. `case` 使用的是字符串模式匹配

`case` 分支左侧写的并不是算术表达式，而是 **shell pattern（Shell 模式）**。

例如：

```bash
case "$1" in
  status)
    printf 'checking\n'
    ;;
esac
```

这里：

```bash
status)
```

表示：

> 当 `$1` 正好匹配字符串 `status` 时，执行这个分支。

因此：

```bash
bash action.sh status
```

会匹配，而：

```bash
bash action.sh status2
```

不会匹配。

---

## 6. `*` 是通配符

Shell pattern 可以使用与文件名匹配类似的通配符。

常见模式：

| 模式       | 含义                      |
| -------- | ----------------------- |
| `start`  | 精确匹配 `start`            |
| `*`      | 匹配任意字符串                 |
| `start*` | 匹配所有以 `start` 开头的字符串    |
| `?`      | 匹配任意一个字符                |
| `[abc]`  | 匹配 `a`、`b` 或 `c` 中的一个字符 |

例如：

```bash
case "$1" in
  start*)
    printf 'start command\n'
    ;;
esac
```

这些都会匹配：

```text
start
starting
start-now
```

因此，如果只希望接受准确的命令：

```text
start
```

就应该写：

```bash
start)
```

而不是：

```bash
start*)
```

---

## 7. `*)` 是默认分支

下面的分支：

```bash
*)
  printf 'unknown\n'
  ;;
```

中的：

```bash
*
```

会匹配任意字符串。

因为 `case` 从上到下寻找**第一个匹配项**，所以通常把它放在最后：

```bash
case "$1" in
  start)
    printf 'starting\n'
    ;;
  stop)
    printf 'stopping\n'
    ;;
  status)
    printf 'checking\n'
    ;;
  *)
    printf 'unknown\n'
    ;;
esac
```

例如：

```bash
bash action.sh reload
```

前面的：

```text
start
stop
status
```

全部不匹配，最终进入：

```bash
*)
```

于是输出：

```text
unknown
```

### 为什么 `*` 应该放最后

如果写成：

```bash
case "$1" in
  *)
    printf 'unknown\n'
    ;;
  start)
    printf 'starting\n'
    ;;
esac
```

那么即使传入：

```bash
bash action.sh start
```

第一个 `*` 已经匹配成功，后面的 `start)` 永远不会执行。

因此规则是：

> `case` 从上到下匹配，最具体的模式放前面，最宽泛的 `*` 放最后。

---

## 8. `;;` 的作用

每个普通 `case` 分支通常以：

```bash
;;
```

结束：

```bash
start)
  printf 'starting\n'
  ;;
```

它的作用可以理解为：

> 当前分支执行结束后，退出整个 `case`。

因此：

```bash
case "$1" in
  start)
    printf 'starting\n'
    ;;
  stop)
    printf 'stopping\n'
    ;;
esac
```

当 `$1=start` 时，只会输出：

```text
starting
```

不会继续执行：

```text
stopping
```

这与某些语言的 `switch` 不同：普通 Bash `case` 使用 `;;` 时不会发生传统意义上的 fall-through（继续掉进下一个分支）。

---

## 9. 一次调用只执行一个分支

假设执行：

```bash
bash action.sh start
```

Bash 的逻辑大致是：

```text
$1 = start

start 匹配吗？
→ 是

执行：
printf 'starting\n'

遇到 ;;
→ 离开 case

结束
```

因此：

```bash
start
```

即使后面的：

```bash
*
```

理论上也能匹配它，`*)` 仍然不会执行。

原因是 Bash 只执行**第一个匹配的普通分支**。

---

## 10. 不写默认分支会发生什么

下面的代码没有：

```bash
*)
```

分支：

```bash
case "$1" in
  start)
    printf 'starting\n'
    ;;
  stop)
    printf 'stopping\n'
    ;;
  status)
    printf 'checking\n'
    ;;
esac
```

现在执行：

```bash
bash action.sh reload
```

没有任何模式匹配。

结果是：

```text
没有输出
```

而 `case` 本身不会因为“没有匹配项”自动报错。

通常这种情况下脚本仍然可以以成功状态结束：

```text
exit status 0
```

这意味着：

> “什么都没执行”并不自动等于“发生错误”。

如果未知命令应该被明确处理，就应保留默认分支：

```bash
*)
  printf 'unknown\n'
  ;;
```

如果还希望未知命令表示失败，可以进一步写：

```bash
*)
  printf 'unknown\n'
  exit 1
  ;;
```

---

## 11. 将多次运行结果写入文件

如果希望依次运行：

```text
start
stop
status
reload
```

并把输出保存到 `action-results.txt`，可以执行：

```bash
bash action.sh start > action-results.txt
bash action.sh stop >> action-results.txt
bash action.sh status >> action-results.txt
bash action.sh reload >> action-results.txt
```

最终文件内容：

```text
starting
stopping
checking
unknown
```

这里：

```bash
>
```

表示创建或覆盖文件：

```bash
bash action.sh start > action-results.txt
```

而：

```bash
>>
```

表示追加内容：

```bash
bash action.sh stop >> action-results.txt
```

如果四次都使用：

```bash
>
```

那么每一次都会覆盖前一次结果，最终只会留下最后一行。

---

## 12. 为什么建议写 `case "$1" in`

推荐：

```bash
case "$1" in
```

而不是：

```bash
case $1 in
```

对于 `case` 来说，未加引号的变量展开不像普通命令参数那样容易产生 word splitting 和 pathname expansion 问题，因此很多情况下两种形式都能工作。

不过：

```bash
case "$1" in
```

仍然是更清晰、一致的写法：

* 明确表示 `$1` 被当作一个完整字符串；
* 与 Bash 脚本中“变量展开默认加引号”的习惯保持一致；
* 阅读代码时更容易理解这里比较的是一个整体值。

---

## 13. `case` 与 `if / elif` 的选择

### 更适合 `if`

当判断的是不同条件：

```bash
if (( age < 18 )); then
  ...
elif [[ -f "$file" ]]; then
  ...
elif [[ "$user" == "admin" ]]; then
  ...
fi
```

这些条件彼此不同，因此 `if` 更自然。

### 更适合 `case`

当反复判断**同一个字符串变量**：

```bash
case "$command" in
  start)
    ...
    ;;
  stop)
    ...
    ;;
  status)
    ...
    ;;
  *)
    ...
    ;;
esac
```

这种场景下 `case`：

* 重复更少；
* 分支结构更清晰；
* 很适合命令、状态、文件扩展名等字符串分类。

---

## 14. 常见错误

### 错误 1：忘记默认分支

```bash
case "$1" in
  start)
    printf 'starting\n'
    ;;
esac
```

执行：

```bash
bash action.sh reload
```

不会输出错误信息。

如果未知输入需要处理，应增加：

```bash
*)
  printf 'unknown\n'
  ;;
```

---

### 错误 2：把 `*` 放在最前面

```bash
case "$1" in
  *)
    printf 'unknown\n'
    ;;
  start)
    printf 'starting\n'
    ;;
esac
```

`*` 可以匹配所有输入，因此：

```bash
start)
```

永远没有机会执行。

---

### 错误 3：误以为 `start)` 是算术或条件表达式

```bash
start)
```

不是：

```bash
[[ "$1" == "start" ]]
```

也不是算术表达式。

它是一个 **Shell pattern**，由 `case` 自动拿来和指定值匹配。

---

### 错误 4：无意中写成宽泛匹配

如果要求只能接受：

```text
status
```

应写：

```bash
status)
```

不要写：

```bash
status*)
```

因为后者还会接受：

```text
status-old
status123
statusAnything
```

---

## 15. 推荐写法

一个简单的命令分派脚本可以写成：

```bash
#!/usr/bin/env bash

case "$1" in
  start)
    printf 'starting\n'
    ;;
  stop)
    printf 'stopping\n'
    ;;
  status)
    printf 'checking\n'
    ;;
  *)
    printf 'unknown\n'
    ;;
esac
```

对应关系：

| `$1`     | 输出         |
| -------- | ---------- |
| `start`  | `starting` |
| `stop`   | `stopping` |
| `status` | `checking` |
| 其他内容     | `unknown`  |

## 核心知识点

* `case` 适合根据**同一个字符串值**选择不同分支。
* 基本结构是 `case ... in ... esac`。
* 每个分支前面写的是 shell pattern，而不是算术表达式。
* `case` 按照代码中的顺序寻找第一个匹配模式。
* 普通分支使用 `;;` 结束，因此一个调用通常只执行一个分支。
* `*)` 可以作为默认分支，应通常放在最后。
* 没有默认分支时，无法匹配的输入可以什么都不做，并不一定产生错误。
* 对命令行子命令、状态名称等有限字符串集合，`case` 通常比一长串 `if / elif` 更清晰。
