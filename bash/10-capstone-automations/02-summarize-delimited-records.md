# Bash 使用关联数组汇总 CSV 数据

## 目标

处理 `records.csv`：

```text
api,2
worker,4
api,3
web,1
worker,2
```

要求：

1. 按服务名累计 `count`。
2. 最终按服务名排序。
3. 输出到 `summary.txt`。
4. 服务名不能写死，必须根据输入动态处理。
5. `count` 只能是：

   * `0`
   * `1` 到 `99999`
6. 非法 `count` 必须：

   * 立即以状态码 `2` 退出；
   * **不能创建或修改 `summary.txt`**。

正确结果：

```text
api,5
web,1
worker,6
```

---

## 完整脚本：`summarize.sh`

```bash
#!/usr/bin/env bash

declare -A totals=()

while IFS=, read -r service count; do
    [[ "$count" =~ ^(0|[1-9][0-9]{0,4})$ ]] || exit 2

    (( totals[$service] += count ))
done < records.csv

for service in "${!totals[@]}"; do
    printf '%s,%s\n' "$service" "${totals[$service]}"
done | sort > summary.txt
```

然后运行：

```bash
chmod +x summarize.sh
./summarize.sh
```

---

## 1. 为什么需要关联数组

普通 Bash 数组使用数字作为下标：

```bash
arr[0]="hello"
arr[1]="world"
```

但这里需要按照服务名保存累计值：

```text
api    -> 5
worker -> 6
web    -> 1
```

因此应该使用 **关联数组（associative array）**：

```bash
declare -A totals=()
```

之后可以直接：

```bash
totals[api]=5
totals[worker]=6
```

动态变量也可以作为 key：

```bash
service="api"

echo "${totals[$service]}"
```

等价于：

```bash
echo "${totals[api]}"
```

### 为什么 `declare -A` 不能省略

如果没有：

```bash
declare -A totals
```

Bash 默认把：

```bash
totals[$service]
```

当成普通索引数组的算术下标。

例如：

```bash
service="api"
```

`api` 会被当成算术表达式中的变量，而未定义变量通常被解释为 `0`。

结果不同服务可能全部错误地累计到：

```bash
totals[0]
```

所以这里必须明确声明：

```bash
declare -A totals=()
```

---

## 2. 如何按逗号读取 CSV

核心代码：

```bash
while IFS=, read -r service count; do
    ...
done < records.csv
```

假设一行是：

```text
api,3
```

执行：

```bash
IFS=, read -r service count
```

后得到：

```bash
service="api"
count="3"
```

### `IFS=,`

`IFS` 是 Bash 的字段分隔符。

这里：

```bash
IFS=,
```

告诉 `read`：

> 只按照逗号拆分字段。

### `-r`

```bash
read -r
```

表示不要把反斜杠 `\` 当成转义字符。

处理文件内容时通常应该使用：

```bash
read -r
```

### `< records.csv`

```bash
done < records.csv
```

把整个 CSV 文件作为 `while` 循环的标准输入。

---

## 3. 在累计之前验证数字

不能直接相信：

```bash
count
```

因为：

```bash
(( ... ))
```

会把内容作为 Bash 算术表达式解析，而不只是简单检查“是不是数字”。

因此先验证：

```bash
[[ "$count" =~ ^(0|[1-9][0-9]{0,4})$ ]] || exit 2
```

### 正则表达式含义

```text
^(0|[1-9][0-9]{0,4})$
```

拆开来看：

| 部分           | 含义           |   |
| ------------ | ------------ | - |
| `^`          | 字符串开头        |   |
| `0`          | 单独允许数字 `0`   |   |
| `            | `            | 或 |
| `[1-9]`      | 第一位必须是 1～9   |   |
| `[0-9]{0,4}` | 后面最多再有 4 位数字 |   |
| `$`          | 字符串结尾        |   |

因此合法范围是：

```text
0
1
9
10
123
99999
```

非法示例：

```text
abc
12x
-1
1.5
0001
100000
```

---

## 4. 为什么限制最多五位数字

Bash 的：

```bash
(( ... ))
```

使用固定宽度整数进行算术。

数字如果极大，可能发生整数溢出，而不是得到一个明显的“数字太大”错误。

例如，一个极大的输入可能经过整数回绕以后变成完全不同的值。

因此不能只检查：

```bash
^[0-9]+$
```

而应该主动限制输入范围：

```bash
^(0|[1-9][0-9]{0,4})$
```

这里最大允许：

```text
99999
```

---

## 5. 为什么验证必须发生在写 `summary.txt` 之前

要求是：

> 如果任何一行存在非法 count，必须退出 2，而且不能写 `summary.txt`。

因此脚本先完成整个读取和验证：

```bash
while ...; do
    [[ ... ]] || exit 2
    ...
done < records.csv
```

只有循环全部成功以后，才执行：

```bash
... > summary.txt
```

这非常重要。

不要在读取过程中就打开输出文件，例如：

```bash
> summary.txt

while ...; do
    ...
done
```

因为即使随后发现错误，`summary.txt` 已经被创建或清空了。

正确思路是：

```text
读取
 ↓
验证
 ↓
累计
 ↓
全部成功
 ↓
最后才写 summary.txt
```

---

## 6. 如何累计相同服务

核心代码：

```bash
(( totals[$service] += count ))
```

假设第一行：

```text
api,2
```

相当于：

```bash
totals[api] = 0 + 2
```

于是：

```text
api -> 2
```

之后再次遇到：

```text
api,3
```

变成：

```bash
totals[api] = 2 + 3
```

最终：

```text
api -> 5
```

不存在的关联数组元素在这里可以作为 `0` 开始累计，因此不需要先写：

```bash
totals[$service]=0
```

---

## 7. 如何取得关联数组的所有 key

假设：

```bash
declare -A totals=(
    [api]=5
    [worker]=6
    [web]=1
)
```

### 获取所有 key

```bash
"${!totals[@]}"
```

得到的是：

```text
api
worker
web
```

因此可以：

```bash
for service in "${!totals[@]}"; do
    ...
done
```

### 获取某个 key 对应的值

```bash
"${totals[$service]}"
```

例如：

```bash
service="api"

echo "${totals[$service]}"
```

输出：

```text
5
```

### 容易混淆的区别

```bash
"${totals[@]}"
```

得到的是 **values**。

```bash
"${!totals[@]}"
```

得到的是 **keys**。

这里需要服务名，所以必须使用：

```bash
"${!totals[@]}"
```

---

## 8. 关联数组本身没有需要依赖的排序

不能假设：

```bash
for service in "${!totals[@]}"
```

会按照：

```text
api
web
worker
```

输出。

关联数组 key 的遍历顺序不能作为字母顺序依赖。

因此先输出：

```bash
for service in "${!totals[@]}"; do
    printf '%s,%s\n' "$service" "${totals[$service]}"
done
```

然后交给：

```bash
sort
```

完整形式：

```bash
for service in "${!totals[@]}"; do
    printf '%s,%s\n' "$service" "${totals[$service]}"
done | sort > summary.txt
```

数据流为：

```text
for 循环
    │
    │ api,5
    │ worker,6
    │ web,1
    ▼
  sort
    │
    │ api,5
    │ web,1
    │ worker,6
    ▼
summary.txt
```

---

## 9. 为什么不要写死服务名

错误做法：

```bash
echo "api,${totals[api]}"
echo "web,${totals[web]}"
echo "worker,${totals[worker]}"
```

虽然当前数据可能通过，但换成：

```text
db,3
cache,8
db,4
```

脚本就失效了。

正确做法是从关联数组动态获取所有服务：

```bash
for service in "${!totals[@]}"; do
    printf '%s,%s\n' "$service" "${totals[$service]}"
done
```

因此同一个脚本可以处理任意服务名。

---

## 通过 Lesson 所需的最终实现

`/home/learner/bash-lab/summarize.sh`：

```bash
#!/usr/bin/env bash

declare -A totals=()

while IFS=, read -r service count; do
    [[ "$count" =~ ^(0|[1-9][0-9]{0,4})$ ]] || exit 2
    (( totals[$service] += count ))
done < records.csv

for service in "${!totals[@]}"; do
    printf '%s,%s\n' "$service" "${totals[$service]}"
done | sort > summary.txt
```

运行：

```bash
cd /home/learner/bash-lab
chmod +x summarize.sh
./summarize.sh
```

检查：

```bash
cat summary.txt
```

当前数据应得到：

```text
api,5
web,1
worker,6
```

检查退出状态：

```bash
echo $?
```

正常执行应为：

```text
0
```

---

## 失败测试

### 非数字 count

例如：

```text
api,2
worker,abc
```

运行：

```bash
./summarize.sh
echo $?
```

应得到状态码：

```text
2
```

并且脚本不能因为这次失败而创建新的 `summary.txt`。

### 超过五位数字

例如：

```text
api,100000
```

同样应该：

```text
exit status = 2
```

因为：

```text
100000
```

有六位数字。

---

## 核心知识点

```bash
declare -A totals=()
```

创建字符串 key 的关联数组。

```bash
while IFS=, read -r service count; do
```

按照逗号读取两个字段。

```bash
[[ "$count" =~ ^(0|[1-9][0-9]{0,4})$ ]] || exit 2
```

在算术运算前验证输入，并限制到 `0～99999`。

```bash
(( totals[$service] += count ))
```

按照服务名动态累计。

```bash
"${!totals[@]}"
```

获取关联数组所有 key。

```bash
printf '%s,%s\n' "$service" "${totals[$service]}"
```

生成 `service,total`。

```bash
... | sort > summary.txt
```

排序后才写最终文件，并确保非法输入发生时还没有打开 `summary.txt`。
