# Bash：用 `while`、`case` 和函数处理逐行输入

## 1. 场景与目标

文件：

```text
/home/learner/actions
```

每行保存一个请求执行的服务操作，例如：

```text
start
status
restart
reload
stop
```

系统只接受以下三个操作：

```text
start
stop
restart
```

目标是编写脚本：

```text
/home/learner/bin/action-report
```

逐行读取 `/home/learner/actions`，只保留合法操作，并输出：

```text
start	accepted
restart	accepted
stop	accepted
```

其中操作名和 `accepted` 之间使用 **Tab（制表符）**。

最终运行：

```bash
/home/learner/bin/action-report > /home/learner/action-summary
```

生成：

```text
/home/learner/action-summary
```

---

## 2. 这个任务对应三个 Bash 构造

这个问题可以拆成三个独立职责：

| 职责       | Bash 构造         |
| -------- | --------------- |
| 逐行读取文件   | `while read -r` |
| 判断操作是否合法 | `case`          |
| 统一格式化输出  | 函数              |

核心结构：

```bash
while IFS= read -r action; do
    case "$action" in
        start|stop|restart)
            record "$action"
            ;;
        *)
            ;;
    esac
done < /home/learner/actions
```

---

# 3. `for` 和 `while` 的区别

## `for`：遍历已经拥有的一组值

当数据本来就是一个明确的列表时，`for` 很自然：

```bash
for name in api web worker; do
    printf '%s\n' "$name"
done
```

输出：

```text
api
web
worker
```

这里：

```bash
api web worker
```

已经是脚本中现成的一组值。

---

## `while read`：逐行处理输入流

文件内容不是一个预先写好的 Bash 单词列表，而是一行一行到来的输入流，因此更适合：

```bash
while IFS= read -r action; do
    printf '%s\n' "$action"
done < /home/learner/actions
```

工作过程类似：

```text
读取第一行
↓
执行循环体
↓
读取第二行
↓
执行循环体
↓
...
↓
文件结束，read 失败
↓
循环结束
```

`while` 的本质是：

> 只要后面的命令返回成功状态，就继续循环。

这里的命令是：

```bash
read -r action
```

每成功读取一行，`read` 返回成功，因此循环继续；到达文件末尾后，循环停止。

---

# 4. 为什么推荐 `IFS= read -r`

常见写法：

```bash
while IFS= read -r action; do
    ...
done
```

其中两个细节都很重要。

## `-r`：不要把反斜杠当转义字符

例如文件中有：

```text
foo\bar
```

使用：

```bash
read -r action
```

会原样得到：

```text
foo\bar
```

因此读取普通文本文件时，通常应该使用 `read -r`。

---

## `IFS=`：不要自动删除首尾空白

Bash 的 `read` 默认会受到 `IFS`（Internal Field Separator，内部字段分隔符）的影响。

使用：

```bash
IFS= read -r action
```

可以更忠实地按原始行读取内容。

因此，逐行读文本文件的常见安全模板是：

```bash
while IFS= read -r line; do
    ...
done < file
```

---

# 5. 为什么重定向写在 `done` 后面

正确写法：

```bash
while IFS= read -r action; do
    ...
done < /home/learner/actions
```

这里：

```bash
< /home/learner/actions
```

是给**整个循环**提供标准输入。

因此每一次：

```bash
read -r action
```

都会从同一个文件继续读取下一行。

可以把它理解为：

```text
/home/learner/actions
        │
        ▼
   while 循环
        │
        ├── read 第一行
        ├── read 第二行
        ├── read 第三行
        └── ...
```

---

# 6. 避免 `cat file | while read ...`

虽然下面这种写法看起来很自然：

```bash
cat /home/learner/actions |
while IFS= read -r action; do
    ...
done
```

但在 Bash 中，这可能带来一个重要问题：

> 管道中的 `while` 循环通常运行在子 Shell（subshell）中。

例如：

```bash
count=0

printf '%s\n' a b c |
while IFS= read -r line; do
    count=$((count + 1))
done

printf '%s\n' "$count"
```

你可能期望：

```text
3
```

但通常得到：

```text
0
```

原因是循环中的：

```bash
count=$((count + 1))
```

修改的是**子 Shell 中的变量**。

循环结束后子 Shell 消失，外层 Shell 的：

```bash
count
```

仍然是：

```text
0
```

---

## 更好的写法

使用输入重定向：

```bash
count=0

while IFS= read -r line; do
    count=$((count + 1))
done < file

printf '%s\n' "$count"
```

此时循环通常直接运行在当前 Shell 中，因此变量修改可以保留下来。

### 记忆

推荐：

```bash
while read ...; do
    ...
done < file
```

尽量避免仅为了读取文件而写：

```bash
cat file | while read ...; do
    ...
done
```

这不仅多启动了一个 `cat`，还可能引入子 Shell 变量作用域问题。

---

# 7. 用 `case` 判断多个固定值

需要判断：

```text
start
stop
restart
```

是否属于允许范围。

非常适合使用：

```bash
case "$action" in
    start|stop|restart)
        ...
        ;;
    *)
        ...
        ;;
esac
```

基本语法：

```bash
case "$variable" in
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

---

## `start|stop|restart` 的意思

```bash
start|stop|restart)
```

表示三个模式中的任意一个：

```text
start
或
stop
或
restart
```

因此：

```bash
case "$action" in
    start|stop|restart)
        printf '合法\n'
        ;;
esac
```

以下值都会匹配：

```text
start
stop
restart
```

---

# 8. `case` 使用的是 Shell 模式，不是正则表达式

`case` 的模式属于 Shell glob pattern（通配模式）。

常见符号：

| 模式      | 含义            |            |
| ------- | ------------- | ---------- |
| `*`     | 任意多个字符        |            |
| `?`     | 任意一个字符        |            |
| `[abc]` | `a`、`b` 或 `c` |            |
| `start  | stop`         | 多个模式中的任意一个 |

例如：

```bash
case "$name" in
    web*)
        printf 'Web service\n'
        ;;
esac
```

可以匹配：

```text
web
web01
web-server
```

---

# 9. 为什么 `*` 必须放最后

```bash
*)
```

表示：

> 匹配任何内容。

因此通常作为默认分支：

```bash
case "$action" in
    start|stop|restart)
        record "$action"
        ;;
    *)
        ;;
esac
```

如果把 `*` 放前面：

```bash
case "$action" in
    *)
        ...
        ;;
    start)
        ...
        ;;
esac
```

那么所有值都会先匹配：

```bash
*
```

后面的：

```bash
start)
```

永远不会执行。

因此：

> `case` 从上到下寻找第一个匹配项，`*` 通常必须放在最后。

---

# 10. `case` 与 `if` 的选择

同样的逻辑当然可以写成：

```bash
if [ "$action" = "start" ]; then
    ...
elif [ "$action" = "stop" ]; then
    ...
elif [ "$action" = "restart" ]; then
    ...
fi
```

但是使用 `case` 更清晰：

```bash
case "$action" in
    start|stop|restart)
        ...
        ;;
esac
```

因为需要判断的是：

> 一个变量属于几个固定字符串中的哪一种。

经验规则：

* 判断条件表达式，例如数字大小、文件是否存在 → `if`
* 一个值需要匹配多个固定值或模式 → `case`

例如：

```bash
if [ "$usage" -ge 90 ]; then
    ...
fi
```

适合 `if`。

而：

```bash
case "$command" in
    start|stop|restart)
        ...
        ;;
esac
```

适合 `case`。

---

# 11. Bash 函数：把重复操作集中到一个地方

输出格式始终相同：

```text
操作名<Tab>accepted
```

与其每个分支都重复：

```bash
printf '%s\taccepted\n' "$action"
```

可以定义函数：

```bash
record() {
    printf '%s\taccepted\n' "$1"
}
```

然后调用：

```bash
record "$action"
```

---

# 12. Bash 函数的基本语法

定义：

```bash
function_name() {
    commands
}
```

例如：

```bash
greet() {
    printf 'hello %s\n' "$1"
}
```

调用：

```bash
greet "Alice"
```

输出：

```text
hello Alice
```

---

# 13. 函数中的 `$1` 与脚本中的 `$1`

函数也有自己的位置参数。

例如：

```bash
record() {
    printf '%s\taccepted\n' "$1"
}

record "$action"
```

执行：

```bash
record "start"
```

时，函数内部：

```bash
$1
```

就是：

```text
start
```

重要的是：

> 函数中的 `$1` 是函数收到的第一个参数，而不是脚本本身的 `$1`。

例如脚本运行：

```bash
./script hello
```

脚本级别：

```bash
$1
```

是：

```text
hello
```

但：

```bash
record "restart"
```

进入函数以后，函数内部的 `$1` 是：

```text
restart
```

---

# 14. 使用 `printf` 输出 Tab

要输出：

```text
start<Tab>accepted
```

可以使用：

```bash
printf '%s\taccepted\n' "$1"
```

其中：

```text
%s
```

表示插入字符串。

```text
\t
```

表示 Tab。

```text
\n
```

表示换行。

因此：

```bash
printf '%s\taccepted\n' "start"
```

得到：

```text
start	accepted
```

相比：

```bash
echo -e
```

更推荐 `printf`，因为行为更加明确、可移植。

---

# 15. 完整脚本

创建：

```text
/home/learner/bin/action-report
```

内容：

```bash
#!/bin/bash

record() {
    printf '%s\taccepted\n' "$1"
}

while IFS= read -r action; do
    case "$action" in
        start|stop|restart)
            record "$action"
            ;;
        *)
            ;;
    esac
done < /home/learner/actions
```

---

# 16. 脚本执行流程

假设 `/home/learner/actions`：

```text
start
status
restart
reload
stop
```

处理过程：

```text
start
  ↓
匹配 start|stop|restart
  ↓
record "start"
  ↓
输出 start<Tab>accepted
```

然后：

```text
status
  ↓
不匹配允许操作
  ↓
进入 *
  ↓
什么也不输出
```

最终结果：

```text
start	accepted
restart	accepted
stop	accepted
```

注意：

> 输出顺序与输入文件中的顺序保持一致。

脚本不会主动排序。

---

# 17. 设置脚本权限

要求脚本权限为：

```text
755
```

执行：

```bash
chmod 755 /home/learner/bin/action-report
```

`755` 表示：

| 对象     | 权限    |
| ------ | ----- |
| owner  | `rwx` |
| group  | `r-x` |
| others | `r-x` |

也就是：

```text
-rwxr-xr-x
```

检查：

```bash
ls -l /home/learner/bin/action-report
```

---

# 18. 确认脚本由当前用户拥有

查看：

```bash
ls -l /home/learner/bin/action-report
```

例如：

```text
-rwxr-xr-x 1 learner learner ... action-report
```

第一个：

```text
learner
```

通常表示 owner。

如果文件本来就是当前用户创建的，一般不需要额外修改 owner。

必要时管理员可以使用：

```bash
chown learner:learner /home/learner/bin/action-report
```

---

# 19. 将脚本输出保存到文件

执行：

```bash
/home/learner/bin/action-report > /home/learner/action-summary
```

这里：

```bash
>
```

把脚本的标准输出重定向到：

```text
/home/learner/action-summary
```

如果文件不存在：

```text
创建文件
```

如果已经存在：

```text
清空旧内容，再写入新内容
```

查看结果：

```bash
cat /home/learner/action-summary
```

---

# 20. 最终命令示例

```bash
mkdir -p /home/learner/bin

cat > /home/learner/bin/action-report <<'EOF'
#!/bin/bash

record() {
    printf '%s\taccepted\n' "$1"
}

while IFS= read -r action; do
    case "$action" in
        start|stop|restart)
            record "$action"
            ;;
        *)
            ;;
    esac
done < /home/learner/actions
EOF

chmod 755 /home/learner/bin/action-report

/home/learner/bin/action-report > /home/learner/action-summary
```

检查：

```bash
ls -l /home/learner/bin/action-report
cat /home/learner/action-summary
```

---

# 21. 常见错误

## 错误 1：用 `for action in $(cat file)` 读取文件

例如：

```bash
for action in $(cat /home/learner/actions); do
    ...
done
```

这种写法会进行单词分割，并不能可靠地保留“每一行”的原始内容。

逐行读取文本更应该使用：

```bash
while IFS= read -r action; do
    ...
done < file
```

---

## 错误 2：通过管道读取后还想保留循环变量

```bash
cat file |
while read -r line; do
    count=$((count + 1))
done
```

循环可能运行在子 Shell 中，导致：

```bash
count
```

在循环结束后没有改变。

优先改成：

```bash
while IFS= read -r line; do
    count=$((count + 1))
done < file
```

---

## 错误 3：把 `*` 放在 `case` 最前面

错误：

```bash
case "$action" in
    *)
        ;;
    start)
        ...
        ;;
esac
```

`*` 会首先匹配所有输入，所以：

```bash
start)
```

永远无法执行。

---

## 错误 4：忘记 `;;`

每个普通 `case` 分支通常需要：

```bash
;;
```

例如：

```bash
start)
    record "$action"
    ;;
```

它表示当前分支执行结束，不再继续处理其他模式。

---

## 错误 5：重复写相同的格式化逻辑

不推荐：

```bash
case "$action" in
    start)
        printf '%s\taccepted\n' "$action"
        ;;
    stop)
        printf '%s\taccepted\n' "$action"
        ;;
    restart)
        printf '%s\taccepted\n' "$action"
        ;;
esac
```

更简洁：

```bash
case "$action" in
    start|stop|restart)
        record "$action"
        ;;
esac
```

把“判断是否合法”和“如何格式化输出”分开，脚本更容易维护。

---

# 22. 核心模板

处理“逐行读取 → 分类 → 执行统一动作”时，可以记住这个模板：

```bash
process() {
    printf '%s\n' "$1"
}

while IFS= read -r value; do
    case "$value" in
        pattern1|pattern2)
            process "$value"
            ;;
        *)
            ;;
    esac
done < input-file
```

核心思想是：

```text
while read
    ↓
逐行读取

case
    ↓
分类和过滤

function
    ↓
复用统一处理逻辑

done < file
    ↓
让文件成为整个循环的输入，并避免不必要的管道子 Shell
```

这是一种非常常见的 Bash 文本处理结构。
