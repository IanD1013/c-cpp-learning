# Bash 退出状态（Exit Status）与 `$?`

## 1. 输出信息和退出状态是两回事

Bash 命令结束时，通常会产生两类信息：

1. **文本输出**：给人看的，例如：

```bash
echo "permission denied"
```

2. **退出状态（exit status）**：给调用者看的，例如：

```bash
exit 2
```

因此：

```bash
echo "permission denied"
```

只是打印了一段文字，并不能告诉另一个脚本“这个程序失败了”。

如果希望调用者能够判断执行结果，需要返回一个退出状态。

---

## 2. 什么是退出状态

每个 Bash 命令执行结束后都会返回一个整数，称为 **退出状态（exit status / exit code）**。

基本约定：

|   状态码 | 含义        |
| ----: | --------- |
|   `0` | 成功        |
| 非 `0` | 失败或其他特殊状态 |

例如一个脚本可以自行规定：

```text
0 = 成功
2 = 输入文件不存在
7 = 无法连接服务器
```

这样调用脚本就不仅知道“失败了”，还可以通过不同数字判断具体失败原因。

---

## 3. 使用 `exit` 返回状态码

Bash 中可以使用：

```bash
exit NUMBER
```

立即结束脚本，并把 `NUMBER` 返回给调用者。

例如：

```bash
#!/usr/bin/env bash

echo "Something went wrong"
exit 2
```

运行：

```bash
bash example.sh
```

脚本打印：

```text
Something went wrong
```

同时返回退出状态：

```text
2
```

---

## 4. `$?`：读取上一条命令的退出状态

Bash 会把**刚刚执行完成的命令**的退出状态保存在：

```bash
$?
```

例如：

```bash
bash example.sh
echo "$?"
```

如果 `example.sh` 执行：

```bash
exit 2
```

那么：

```bash
echo "$?"
```

会输出：

```text
2
```

---

## 5. `$?` 只能保存「上一条命令」的状态

这是最重要的规则之一：

> `$?` 永远表示最近执行完成的那一条命令的退出状态。

例如：

```bash
bash example.sh
echo "hello"
echo "$?"
```

这里最后的：

```bash
echo "$?"
```

得到的已经不是 `example.sh` 的退出状态。

原因是：

```bash
echo "hello"
```

本身也是一条命令，它执行之后会覆盖 `$?`。

因此应该立即保存：

```bash
bash example.sh
status=$?
```

或者写在同一行：

```bash
bash example.sh; status=$?
```

换行和分号没有本质区别。真正重要的是：

> 在读取 `$?` 之前不能插入其他命令。

---

## 6. 使用位置参数让调用者决定退出状态

如果希望运行脚本时指定退出状态：

```bash
bash status-demo.sh 2
```

那么：

```bash
$1
```

就是：

```text
2
```

可以先保存到变量：

```bash
status=$1
```

然后：

```bash
exit "$status"
```

于是完整脚本：

```bash
#!/usr/bin/env bash

status=$1
exit "$status"
```

这里最好保留双引号：

```bash
exit "$status"
```

这样脚本不会把某个状态码写死，而是返回调用者提供的值。

例如：

```bash
bash status-demo.sh 0
```

返回：

```text
0
```

而：

```bash
bash status-demo.sh 2
```

返回：

```text
2
```

甚至：

```bash
bash status-demo.sh 7
```

也会尝试返回：

```text
7
```

---

## 7. 完成 `status-demo.sh`

进入实验目录：

```bash
cd bash-lab
```

创建：

```bash
status-demo.sh
```

内容：

```bash
#!/usr/bin/env bash

status=$1
exit "$status"
```

可以用：

```bash
cat > status-demo.sh <<'SCRIPT'
#!/usr/bin/env bash

status=$1
exit "$status"
SCRIPT
```

---

## 8. 运行脚本并立即保存 `$?`

第一次传入 `0`：

```bash
bash status-demo.sh 0
first=$?
```

此时：

```bash
echo "$first"
```

应该得到：

```text
0
```

第二次传入 `2`：

```bash
bash status-demo.sh 2
second=$?
```

检查：

```bash
echo "$second"
```

得到：

```text
2
```

也可以使用分号：

```bash
bash status-demo.sh 0; first=$?
bash status-demo.sh 2; second=$?
```

效果相同。

---

## 9. 把两个状态写入 `statuses.txt`

要求文件内容为：

```text
0
2
```

可以使用：

```bash
printf '%s\n' "$first" "$second" > statuses.txt
```

其中：

```bash
printf '%s\n'
```

表示每个值输出一行。

所以：

```bash
printf '%s\n' "$first" "$second" > statuses.txt
```

会生成：

```text
0
2
```

检查：

```bash
cat statuses.txt
```

应该看到：

```text
0
2
```

---

## 10. 完整操作流程

```bash
cd bash-lab

cat > status-demo.sh <<'SCRIPT'
#!/usr/bin/env bash

status=$1
exit "$status"
SCRIPT

bash status-demo.sh 0; first=$?
bash status-demo.sh 2; second=$?

printf '%s\n' "$first" "$second" > statuses.txt
```

最终目录至少包含：

```text
bash-lab/
├── status-demo.sh
└── statuses.txt
```

`status-demo.sh`：

```bash
#!/usr/bin/env bash

status=$1
exit "$status"
```

`statuses.txt`：

```text
0
2
```

---

## 11. 常见错误

### 错误一：只打印错误信息，不设置退出状态

```bash
echo "permission denied"
```

这条命令本身通常成功，因此脚本最后可能返回：

```text
0
```

调用者就无法知道真正发生了错误。

如果需要表示失败，应使用：

```bash
echo "permission denied"
exit 2
```

---

### 错误二：没有立即保存 `$?`

错误：

```bash
bash status-demo.sh 2
echo "finished"
status=$?
```

这里：

```bash
status=$?
```

保存的是：

```bash
echo "finished"
```

的状态，而不是 `status-demo.sh` 的状态。

正确：

```bash
bash status-demo.sh 2
status=$?
```

---

### 错误三：把退出状态写死

例如：

```bash
status=$1
exit 2
```

虽然读取了 `$1`，但完全没有使用它。

这样：

```bash
bash status-demo.sh 0
bash status-demo.sh 7
```

都会返回 `2`。

正确：

```bash
status=$1
exit "$status"
```

---

## 12. 核心记忆

```bash
exit 2
```

表示：

> 当前脚本立即结束，并返回状态码 `2`。

```bash
$?
```

表示：

> 最近一条已经执行完成的命令的退出状态。

典型模式：

```bash
some_command
status=$?
```

或者：

```bash
some_command; status=$?
```

如果需要把状态继续返回给上层调用者：

```bash
exit "$status"
```

最关键的原则是：

> **文本输出是给人看的；退出状态是给程序判断成功或失败的。**
