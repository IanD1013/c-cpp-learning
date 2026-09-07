# Bash：从动态状态文件生成排序后的报告

## 目标

假设目录结构如下：

```text
bash-lab/
├── status-report.sh
├── status-report.txt
└── states/
    ├── api.state
    ├── db.state
    └── web.state
```

每个 `.state` 文件只保存一个状态，例如：

```text
up
```

或：

```text
down
```

需要编写 `status-report.sh`，每次运行时都根据 `states/` 当前存在的文件重新生成 `status-report.txt`：

```text
api:up
db:down
web:up
```

要求：

* 服务名必须从文件名动态提取，不能写死；
* 只接受 `up` 和 `down`；
* 非法状态不写入报告；
* 最终按生成的 `name:status` 记录排序，而不是依赖文件路径顺序；
* 每次运行都覆盖旧报告，不能保留旧内容。

---

## 完整脚本

```bash
#!/usr/bin/env bash

for file in states/*.state; do
  name="${file##*/}"
  name="${name%.state}"

  IFS= read -r status < "$file"

  if [[ "$status" == "up" || "$status" == "down" ]]; then
    printf '%s:%s\n' "$name" "$status"
  fi
done | sort > status-report.txt
```

在 `bash-lab` 中运行：

```bash
bash status-report.sh
```

---

## 1. 使用通配符遍历状态文件

```bash
for file in states/*.state; do
```

`states/*.state` 会经过 Bash 的**文件名展开（filename expansion / globbing）**。

例如目录中有：

```text
states/api.state
states/db.state
states/web.state
```

循环依次得到这些路径：

```text
states/api.state
states/db.state
states/web.state
```

因此每轮循环中，`file` 保存一个完整路径。

脚本没有写死 `api`、`db`、`web`，所以服务增加或删除后不需要修改脚本。

---

## 2. 使用参数展开提取文件名

假设：

```bash
file="states/api.state"
```

先执行：

```bash
name="${file##*/}"
```

得到：

```text
api.state
```

### `${变量##模式}`

语法：

```bash
${variable##pattern}
```

表示：

> 从变量**开头**删除匹配 `pattern` 的最长部分。

这里：

```bash
${file##*/}
```

模式是：

```text
*/
```

因此会删除直到最后一个 `/` 为止的所有内容。

例如：

```text
states/api.state
       ↓
api.state
```

这是一种纯 Bash 的路径处理方式，不需要额外运行 `basename`。

---

## 3. 删除 `.state` 扩展名

接着执行：

```bash
name="${name%.state}"
```

如果：

```text
name="api.state"
```

结果就是：

```text
api
```

### `${变量%模式}`

语法：

```bash
${variable%pattern}
```

表示：

> 从变量**末尾**删除匹配 `pattern` 的最短部分。

所以：

```bash
"${name%.state}"
```

会删除末尾的：

```text
.state
```

完整过程：

```text
states/api.state
    │
    ├── ${file##*/}  → api.state
    │
    └── ${name%.state} → api
```

### `#`、`##`、`%`、`%%` 的方向

| 写法              | 删除位置 | 匹配范围 |
| --------------- | ---- | ---- |
| `${x#pattern}`  | 开头   | 最短匹配 |
| `${x##pattern}` | 开头   | 最长匹配 |
| `${x%pattern}`  | 末尾   | 最短匹配 |
| `${x%%pattern}` | 末尾   | 最长匹配 |

可以简单记忆：

```text
#  从前面删
%  从后面删
双符号表示删最长匹配
```

---

## 4. 安全读取状态文件

```bash
IFS= read -r status < "$file"
```

这条命令从当前状态文件读取一行，并保存到变量 `status`。

例如：

```text
states/api.state
```

内容为：

```text
up
```

执行后：

```bash
status="up"
```

### 为什么使用 `IFS=`？

普通 `read` 会受到 `IFS`（Internal Field Separator，内部字段分隔符）的影响。

写成：

```bash
IFS= read ...
```

表示本次 `read` 不使用字段分隔字符，因此可以更忠实地保留输入内容。

### 为什么使用 `-r`？

```bash
read -r
```

让反斜杠 `\` 保持原样，而不是被当作转义字符处理。

这是读取文件行时非常常见的安全写法：

```bash
IFS= read -r variable
```

### 为什么使用 `< "$file"`？

```bash
< "$file"
```

把文件作为 `read` 的标准输入。

因此：

```bash
IFS= read -r status < "$file"
```

可以理解为：

> 从这个文件读取一行，原样放进 `status`。

路径变量需要加引号：

```bash
"$file"
```

避免文件名包含空格等字符时被 Bash 拆分。

---

## 5. 只接受合法状态

状态只有两个合法值：

```text
up
down
```

因此使用：

```bash
if [[ "$status" == "up" || "$status" == "down" ]]; then
```

也可以写成单行条件：

```bash
[[ "$status" == "up" || "$status" == "down" ]]
```

含义是：

```text
status 等于 up
或者
status 等于 down
```

只有条件成立时才生成记录。

例如：

| 文件内容      | 是否输出 |
| --------- | ---- |
| `up`      | 是    |
| `down`    | 是    |
| `unknown` | 否    |
| `failed`  | 否    |
| `UP`      | 否    |
| 空字符串      | 否    |

这里是**精确、区分大小写的字符串比较**。

### 为什么右边写成 `"up"` 和 `"down"`？

在 `[[ ... ]]` 中，`==` 右侧未加引号时可以被解释为 shell pattern。

例如：

```bash
[[ "$status" == up* ]]
```

可以匹配：

```text
up
updating
upper
```

但这里要求精确匹配固定文本，因此写成：

```bash
[[ "$status" == "up" ]]
```

更加明确。

---

## 6. 使用 `printf` 构造记录

合法状态通过验证后：

```bash
printf '%s:%s\n' "$name" "$status"
```

例如：

```bash
name="api"
status="up"
```

产生：

```text
api:up
```

格式串：

```text
%s:%s\n
```

含义：

* 第一个 `%s`：服务名；
* `:`：普通冒号；
* 第二个 `%s`：状态；
* `\n`：换行。

因此：

```bash
printf '%s:%s\n' "$name" "$status"
```

比：

```bash
echo "$name:$status"
```

通常更加明确和可预测。

---

## 7. 为什么要在整个循环之后执行 `sort`

脚本的关键部分是：

```bash
done | sort > status-report.txt
```

这意味着整个循环的输出会统一进入：

```bash
sort
```

处理流程是：

```text
状态文件
   ↓
for 循环
   ↓
生成 name:status
   ↓
sort
   ↓
status-report.txt
```

重点是：

> 排序的是最终生成的记录，而不是原始文件路径。

---

## 8. 文件路径顺序不一定等于记录顺序

假设目录中有：

```text
states/cache.state
states/cache2.state
```

文件名展开时，路径可能按如下顺序出现：

```text
states/cache.state
states/cache2.state
```

因为比较路径时：

```text
cache.state
cache2.state
```

在 `cache` 后面比较的是：

```text
.
```

和：

```text
2
```

`.` 排在 `2` 前面。

但是假设它们生成：

```text
cache:up
cache2:down
```

重新按**记录本身**排序时，需要比较：

```text
cache:up
cache2:down
```

在共同的 `cache` 后面，现在比较的是：

```text
:
```

和：

```text
2
```

此时 `2` 排在 `:` 前面，所以结果可能变成：

```text
cache2:down
cache:up
```

因此不能简单依赖：

```bash
states/*.state
```

本身提供的文件路径顺序。

真正的需求如果是：

> 按最终报告记录排序

就必须：

```bash
... | sort
```

排序生成后的记录。

---

## 9. 为什么使用 `>` 而不是 `>>`

最终输出：

```bash
sort > status-report.txt
```

这里的：

```bash
>
```

表示**覆盖输出**。

假设旧报告原来是：

```text
stale
```

运行脚本后，旧内容会被清除，并重新生成：

```text
api:up
db:down
web:up
```

### `>` 与 `>>`

| 操作符  | 行为           |
| ---- | ------------ |
| `>`  | 清空旧文件，然后重新写入 |
| `>>` | 保留旧文件，在末尾追加  |

如果错误地写成：

```bash
sort >> status-report.txt
```

第一次运行可能得到：

```text
stale
api:up
db:down
web:up
```

第二次运行又变成：

```text
stale
api:up
db:down
web:up
api:up
db:down
web:up
```

这显然不符合“重新生成报告”的要求。

因此应该使用：

```bash
>
```

---

## 10. 为什么脚本可以重复运行

正确脚本具有**幂等性（idempotence）**：

> 如果输入状态没有变化，多次运行产生的最终报告也不会变化。

例如第一次运行：

```text
api:up
db:down
web:up
```

第二次运行仍然是：

```text
api:up
db:down
web:up
```

不会重复追加，因为使用了：

```bash
>
```

而不是：

```bash
>>
```

这种特性对于生成报告、配置文件和构建产物非常重要。

---

## 11. 服务变化时无需修改脚本

假设之后目录变成：

```text
states/
├── cache.state
├── cache2.state
├── db.state
└── worker.state
```

其中：

```text
cache.state   → up
cache2.state  → down
db.state      → unknown
worker.state  → up
```

脚本仍然完全不需要修改。

它会：

1. 遍历当前存在的所有 `.state` 文件；
2. 从文件名推导服务名；
3. 读取文件中的状态；
4. 跳过 `unknown`；
5. 生成合法记录；
6. 对最终记录排序；
7. 覆盖旧报告。

最终报告只包含合法状态，例如：

```text
cache2:down
cache:up
worker:up
```

核心原则是：

> 脚本描述处理规则，而不是写死当前数据。

---

## 12. 数据处理流水线

这类 Bash 脚本可以理解成一个小型数据处理流水线：

```text
states/*.state
      │
      ▼
遍历文件
      │
      ▼
提取服务名
      │
      ▼
读取状态
      │
      ▼
过滤非法状态
      │
      ▼
生成 name:status
      │
      ▼
sort
      │
      ▼
覆盖 status-report.txt
```

对应代码：

```bash
for file in states/*.state; do
  name="${file##*/}"
  name="${name%.state}"

  IFS= read -r status < "$file"

  if [[ "$status" == "up" || "$status" == "down" ]]; then
    printf '%s:%s\n' "$name" "$status"
  fi
done | sort > status-report.txt
```

这种写法的一个重要思想是：

> 循环只负责“产生记录”，`sort` 负责“排序记录”，重定向负责“保存结果”。

每个部分只负责一件事情，因此逻辑非常清晰。

---

## 常见错误

### 错误 1：使用追加重定向

```bash
done | sort >> status-report.txt
```

问题：

* 旧内容不会删除；
* 每运行一次就多追加一份报告。

正确：

```bash
done | sort > status-report.txt
```

---

### 错误 2：依赖文件路径本身的顺序

```bash
for file in states/*.state; do
  ...
  printf ...
done > status-report.txt
```

这种代码可能看起来已经“有序”，因为 glob 通常会产生排序后的路径。

但需求是按：

```text
name:status
```

排序，而不是按：

```text
states/name.state
```

排序。

正确：

```bash
done | sort > status-report.txt
```

---

### 错误 3：把服务名写死

例如：

```bash
printf 'api:up\n'
printf 'db:down\n'
printf 'web:up\n'
```

这样目录一变化脚本就失效。

应该让文件系统提供输入：

```bash
for file in states/*.state
```

并从路径中动态提取名字。

---

### 错误 4：输出未知状态

如果没有验证：

```bash
printf '%s:%s\n' "$name" "$status"
```

那么：

```text
db.state → unknown
```

会生成：

```text
db:unknown
```

正确做法是先过滤：

```bash
if [[ "$status" == "up" || "$status" == "down" ]]; then
  printf '%s:%s\n' "$name" "$status"
fi
```

---

### 错误 5：混淆脚本和报告文件

两个文件名称很接近：

```text
status-report.sh
status-report.txt
```

职责完全不同：

| 文件                  | 作用        |
| ------------------- | --------- |
| `status-report.sh`  | 保存程序逻辑    |
| `status-report.txt` | 保存程序生成的数据 |

运行的是：

```bash
bash status-report.sh
```

脚本生成的是：

```text
status-report.txt
```

---

## 最终应掌握的知识

```bash
name="${file##*/}"
```

从路径中删除最后一个 `/` 之前的内容：

```text
states/api.state → api.state
```

```bash
name="${name%.state}"
```

删除文件扩展名：

```text
api.state → api
```

```bash
IFS= read -r status < "$file"
```

安全地读取文件中的一行。

```bash
[[ "$status" == "up" || "$status" == "down" ]]
```

只允许两个合法状态。

```bash
printf '%s:%s\n' "$name" "$status"
```

把数据转换为最终记录：

```text
api:up
```

```bash
done | sort
```

对**循环生成的最终记录**排序。

```bash
> status-report.txt
```

清空旧报告并重新生成，保证不会留下 `stale` 或重复记录。

最终核心模式可以概括为：

```bash
for 输入 in 动态文件集合; do
  提取信息
  读取数据
  验证数据
  输出记录
done | 排序 > 重新生成的文件
```

这是 Bash 中处理一组文件、过滤内容、排序结果并生成报告的典型模式。
