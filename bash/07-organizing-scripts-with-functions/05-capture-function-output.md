# Bash 函数输出、命令替换与文件重定向

## 核心思想

在 Bash 中，函数最好只负责“生成数据”，而由调用者决定这些数据最终去哪里。

例如，一个函数负责生成名称：

```bash
build_name() {
    printf '%s-%s\n' "$1" "$2"
}
```

调用：

```bash
build_name "worker" "staging"
```

输出：

```text
worker-staging
```

这里 `build_name` 只把结果写到 **标准输出（stdout）**，并没有自己打开文件。

这样，同一个结果可以被调用者：

* 打印到终端；
* 保存到变量；
* 写入文件；
* 拼接其他字符串；
* 传给另一个命令。

这比让函数内部固定写某个文件更灵活。

---

## 1. 使用命令替换捕获函数输出

Bash 的命令替换语法是：

```bash
result=$(command)
```

如果要捕获函数输出：

```bash
result=$(build_name "$1" "$2")
```

执行过程是：

1. 调用 `build_name "$1" "$2"`；
2. 收集它写到标准输出的内容；
3. 删除输出末尾的所有换行符；
4. 把剩余文本赋值给 `result`。

例如：

```bash
build_name() {
    printf '%s-%s\n' "$1" "$2"
}

result=$(build_name "api" "prod")

printf '%s\n' "$result"
```

输出：

```text
api-prod
```

---

## 2. 为什么捕获后的结果没有末尾换行符

函数：

```bash
printf '%s-%s\n' "$1" "$2"
```

实际上输出的是：

```text
api-prod\n
```

但命令替换：

```bash
result=$(build_name "api" "prod")
```

会删除输出末尾的换行符，因此变量实际保存的是：

```text
api-prod
```

而不是：

```text
api-prod\n
```

这使得后续拼接非常方便：

```bash
printf '%s.log\n' "$result"
```

得到：

```text
api-prod.log
```

如果命令替换没有删除末尾换行符，那么 `.log` 就可能跑到下一行。

### 注意

Bash 删除的是**所有末尾换行符**，而不仅仅是一个。

---

## 3. 函数参数 `$1`、`$2` 与脚本参数 `$1`、`$2`

Bash 中的 `$1`、`$2` 表示当前位置参数，但具体含义取决于所在上下文。

假设脚本这样运行：

```bash
bash name.sh worker staging
```

在脚本顶层：

```bash
"$1"
```

是：

```text
worker
```

而：

```bash
"$2"
```

是：

```text
staging
```

如果脚本调用：

```bash
build_name "$1" "$2"
```

那么函数收到：

```text
build_name worker staging
```

于是函数内部：

```bash
"$1"
```

表示：

```text
worker
```

函数内部：

```bash
"$2"
```

表示：

```text
staging
```

可以理解为：

```text
脚本参数
    │
    ├── $1 = worker
    └── $2 = staging
           │
           ▼
build_name "$1" "$2"
           │
           ▼
函数参数
    ├── $1 = worker
    └── $2 = staging
```

---

## 4. 为什么参数通常应该加双引号

推荐：

```bash
build_name "$1" "$2"
```

而不是：

```bash
build_name $1 $2
```

函数内部也应该写：

```bash
printf '%s-%s\n' "$1" "$2"
```

双引号可以避免：

* 空格导致参数被拆分；
* 通配符 `*` 被展开；
* 空字符串意外消失。

例如：

```bash
bash name.sh "api server" prod
```

使用：

```bash
build_name "$1" "$2"
```

函数正确收到两个参数：

```text
api server
prod
```

生成：

```text
api server-prod
```

---

## 5. 同一个变量可以通过不同路线输出

捕获结果：

```bash
result=$(build_name "$1" "$2")
```

之后可以多次使用：

```bash
printf '%s\n' "$result"
printf '%s.log\n' "$result" > name.txt
```

这里同一个 `result` 被用于两种目的。

### 标准输出

```bash
printf '%s\n' "$result"
```

例如输出：

```text
api-prod
```

### 写入文件

```bash
printf '%s.log\n' "$result" > name.txt
```

文件 `name.txt` 中得到：

```text
api-prod.log
```

这体现了一个重要设计原则：

> 函数生成数据，调用者决定数据的用途。

---

## 6. `>` 与 `>>` 的区别

### `>`：覆盖文件

```bash
printf '%s.log\n' "$result" > name.txt
```

如果 `name.txt` 已经存在，其原内容会被替换。

第一次：

```text
worker-staging.log
```

下一次运行：

```bash
bash name.sh api prod
```

文件变成：

```text
api-prod.log
```

原来的内容不会保留。

---

### `>>`：追加文件

```bash
printf '%s.log\n' "$result" >> name.txt
```

则会在原文件末尾继续添加。

例如可能变成：

```text
worker-staging.log
api-prod.log
```

如果要求“每次运行只保留最新结果”，应该使用：

```bash
>
```

而不是：

```bash
>>
```

---

## 7. 为什么推荐使用 `printf`

可以写：

```bash
echo "$result"
```

但脚本中通常更推荐：

```bash
printf '%s\n' "$result"
```

原因是 `printf` 的行为更加明确和可预测。

格式：

```bash
printf '%s\n' "$result"
```

含义：

* `%s`：输出一个字符串；
* `\n`：输出换行符；
* `"$result"`：填入 `%s` 的内容。

例如：

```bash
result="api-prod"

printf '%s\n' "$result"
```

得到：

```text
api-prod
```

---

## 8. 命令替换运行在子 Shell 中

命令替换：

```bash
result=$(build_name "$1" "$2")
```

其中 `$(...)` 里的命令通常运行在一个 **subshell（子 Shell）** 环境中。

子 Shell 可以理解为当前 Shell 的一个临时副本。

因此，在命令替换内部修改变量：

```bash
value="before"

change_value() {
    value="after"
    printf '%s\n' "hello"
}

result=$(change_value)

printf '%s\n' "$value"
```

通常仍然输出：

```text
before
```

而不是：

```text
after
```

原因是：

```text
父 Shell
value=before
    │
    ▼
创建子 Shell
value=before
    │
    ├── value=after
    └── printf "hello"
          │
          ▼
       stdout 被捕获
          │
          ▼
子 Shell 结束
变量修改消失
    │
    ▼
父 Shell
value 仍然是 before
```

---

## 9. 从命令替换中带出数据：使用标准输出

考虑：

```bash
build_name() {
    result="$1-$2"
}
```

然后：

```bash
name=$(build_name "api" "prod")
```

这不会得到：

```text
api-prod
```

因为函数没有向标准输出打印任何内容。

函数内部的：

```bash
result="$1-$2"
```

只是修改了子 Shell 中的变量。

子 Shell 结束后，这个修改不会作为命令替换结果传回来。

正确做法是：

```bash
build_name() {
    printf '%s-%s\n' "$1" "$2"
}
```

然后：

```bash
name=$(build_name "api" "prod")
```

此时函数通过标准输出传递：

```text
api-prod
```

所以：

```bash
name="api-prod"
```

### 关键原则

对于：

```bash
value=$(some_command)
```

`value` 收集的是：

> `some_command` 写到标准输出的文本。

不是：

* 函数内部变量；
* 函数的 `return` 数字；
* 函数内部其他 Shell 状态。

---

## 10. 函数应该避免向 stdout 打印额外信息

如果函数用于命令替换：

```bash
result=$(build_name "$1" "$2")
```

那么函数写到 stdout 的**所有内容**都会被捕获。

错误示例：

```bash
build_name() {
    printf 'Building name...\n'
    printf '%s-%s\n' "$1" "$2"
}
```

执行：

```bash
result=$(build_name api prod)
```

`result` 会包含：

```text
Building name...
api-prod
```

而不是只有：

```text
api-prod
```

因此，用于返回数据的函数应该保证 stdout 尽量只包含真正的数据。

如果确实需要打印调试或错误信息，可以发送到标准错误：

```bash
printf 'Building name...\n' >&2
```

这样：

```bash
result=$(build_name api prod)
```

仍然只捕获 stdout。

---

## 11. 完整示例：`name.sh`

```bash
#!/usr/bin/env bash

build_name() {
    printf '%s-%s\n' "$1" "$2"
}

result=$(build_name "$1" "$2")

printf '%s\n' "$result"
printf '%s.log\n' "$result" > name.txt
```

### 运行

```bash
bash name.sh worker staging
```

标准输出：

```text
worker-staging
```

`name.txt`：

```text
worker-staging.log
```

再次运行：

```bash
bash name.sh api prod
```

标准输出：

```text
api-prod
```

由于使用的是：

```bash
> name.txt
```

文件被覆盖，最终内容为：

```text
api-prod.log
```

如果最后一个 `printf` 成功，脚本通常以状态 `0` 结束。

---

## 12. 数据流

整个脚本的数据流可以表示为：

```text
脚本参数
$1 = api
$2 = prod
    │
    ▼
build_name "$1" "$2"
    │
    ▼
printf "%s-%s\n"
    │
    ▼
stdout: api-prod\n
    │
    ▼
$(...)
删除末尾换行符
    │
    ▼
result="api-prod"
    │
    ├──────────────► stdout
    │                api-prod\n
    │
    └──────────────► name.txt
                     api-prod.log\n
```

---

## 13. 常见错误

### 错误一：在函数内部直接写文件

```bash
build_name() {
    printf '%s-%s.log\n' "$1" "$2" > name.txt
}
```

这会把函数绑定到：

```text
name.txt
```

之后如果还想把同一个名称打印到屏幕、写到其他文件或继续加工，就不够灵活。

更好的方式是：

```bash
build_name() {
    printf '%s-%s\n' "$1" "$2"
}
```

由调用者处理结果。

---

### 错误二：把结果存进函数内部变量，却不打印

```bash
build_name() {
    result="$1-$2"
}

name=$(build_name api prod)
```

`name` 不会自动得到函数内部的 `result`。

命令替换捕获的是 stdout，所以应该：

```bash
build_name() {
    printf '%s-%s\n' "$1" "$2"
}
```

---

### 错误三：函数打印调试信息

```bash
build_name() {
    printf 'starting...\n'
    printf '%s-%s\n' "$1" "$2"
}
```

会污染：

```bash
result=$(build_name api prod)
```

应该让 stdout 只输出数据，必要的诊断信息发送到 stderr：

```bash
printf 'starting...\n' >&2
```

---

### 错误四：使用 `>>` 导致旧数据累积

```bash
printf '%s.log\n' "$result" >> name.txt
```

如果要求每次只保留当前结果，应改为：

```bash
printf '%s.log\n' "$result" > name.txt
```

---

### 错误五：把示例值写死

错误：

```bash
build_name() {
    printf '%s\n' 'api-prod'
}
```

这样：

```bash
bash name.sh worker staging
```

仍然会得到：

```text
api-prod
```

正确做法是使用参数：

```bash
build_name() {
    printf '%s-%s\n' "$1" "$2"
}
```

---

## 14. 最佳实践

对于需要“返回文本”的 Bash 函数，可以采用：

```bash
generate_value() {
    printf '%s\n' "some value"
}

value=$(generate_value)
```

推荐遵循以下原则：

1. **函数通过 stdout 输出数据。**
2. **调用者通过 `$(...)` 捕获数据。**
3. **函数不要把输出目的地写死。**
4. **stdout 中不要混入日志和提示文字。**
5. **诊断信息需要时写到 stderr。**
6. **参数引用通常写成 `"$1"`、`"$2"`。**
7. **使用 `printf` 获得更可预测的输出行为。**
8. **需要覆盖文件时使用 `>`，需要追加时才使用 `>>`。**

## 核心总结

```bash
result=$(build_name "$1" "$2")
```

可以拆成三个概念：

```text
build_name "$1" "$2"
        ↓
函数把数据写到 stdout
        ↓
$(...)
捕获 stdout，并删除末尾换行符
        ↓
result
保存捕获后的字符串
```

最重要的区别是：

```text
函数内部修改变量
≠
从命令替换中返回数据
```

真正能够被：

```bash
result=$(...)
```

捕获的是命令的**标准输出**。

因此，一个灵活的 Bash 函数通常只负责生成数据：

```bash
build_name() {
    printf '%s-%s\n' "$1" "$2"
}
```

然后由调用者决定：

```bash
result=$(build_name "$1" "$2")

printf '%s\n' "$result"
printf '%s.log\n' "$result" > name.txt
```

这种“生成数据”和“处理数据”分离的方式，更容易复用、测试和组合。
