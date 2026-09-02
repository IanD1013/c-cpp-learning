# Bash 索引数组与逐行输出

## 1. 为什么要使用数组

如果需要保存多个彼此独立的值，例如三个服务名：

```text
api
worker
scheduler
```

不应简单把它们塞进一个普通字符串变量：

```bash
servers="api worker scheduler"
```

因为这只是**一个字符串**，并没有真正保存“三个独立元素”的结构。

更适合的方式是使用 Bash 的**索引数组（indexed array）**：

```bash
servers=(api worker scheduler)
```

数组会把每个值单独保存：

|  索引 | 元素          |
| --: | ----------- |
| `0` | `api`       |
| `1` | `worker`    |
| `2` | `scheduler` |

Bash 索引数组默认从 `0` 开始编号。

---

## 2. 创建 Bash 索引数组

基本语法：

```bash
数组名=(元素1 元素2 元素3)
```

例如：

```bash
servers=(api worker scheduler)
```

这里：

* `servers` 是数组名；
* `()` 表示创建数组；
* `api`、`worker`、`scheduler` 是三个独立元素；
* 元素之间使用空格分隔。

如果某个元素本身包含空格，需要引用：

```bash
servers=(api "background worker" scheduler)
```

此时 `"background worker"` 仍然只是一个数组元素。

---

## 3. 访问数组元素

可以通过数字索引读取某个元素：

```bash
echo "${servers[0]}"
echo "${servers[1]}"
echo "${servers[2]}"
```

输出：

```text
api
worker
scheduler
```

推荐始终使用这种形式：

```bash
"${servers[索引]}"
```

而不是省略引号，因为双引号可以避免值中的空格等字符被 Bash 再次拆分。

---

## 4. `${servers[@]}`：展开所有数组元素

要一次取得数组中的全部元素，可以使用：

```bash
"${servers[@]}"
```

这是 Bash 数组中非常重要的写法。

例如：

```bash
printf '<%s>\n' "${servers[@]}"
```

等价于：

```bash
printf '<%s>\n' "api" "worker" "scheduler"
```

输出：

```text
<api>
<worker>
<scheduler>
```

关键点是：

> `[@]` 表示所有已有的数组元素，而 `"...[@]..."` 会保持每个元素都是独立参数。

---

## 5. 为什么 `"${servers[@]}"` 必须加双引号

考虑：

```bash
servers=(api "background worker" scheduler)
```

正确写法：

```bash
printf '%s\n' "${servers[@]}"
```

Bash 会把它展开成三个参数：

```text
api
background worker
scheduler
```

因此输出：

```text
api
background worker
scheduler
```

如果写成：

```bash
printf '%s\n' ${servers[@]}
```

没有双引号时，`background worker` 可能再次发生**单词分割（word splitting）**，变成两个参数：

```text
api
background
worker
scheduler
```

因此推荐形成固定习惯：

```bash
"${array[@]}"
```

而不是：

```bash
${array[@]}
```

即使当前数组元素中没有空格，也应优先使用前者。

---

## 6. `printf` 为什么可以自动逐行打印数组

命令：

```bash
printf '%s\n' "${servers[@]}"
```

这里的格式字符串是：

```bash
'%s\n'
```

含义：

* `%s`：打印一个字符串；
* `\n`：打印换行符。

数组展开后，命令实际上类似于：

```bash
printf '%s\n' "api" "worker" "scheduler"
```

当 `printf` 收到的参数比格式字符串中的占位符多时，它会**重复使用格式字符串**。

因此效果相当于：

```text
'%s\n' + api
'%s\n' + worker
'%s\n' + scheduler
```

最终得到：

```text
api
worker
scheduler
```

这正是 `printf` 与数组非常常见的组合：

```bash
printf '%s\n' "${array[@]}"
```

---

## 7. 将数组内容写入文件

如果当前目录是：

```text
/home/learner/bash-lab
```

可以执行：

```bash
servers=(api worker scheduler)
printf '%s\n' "${servers[@]}" > servers.txt
```

最终：

```text
/home/learner/bash-lab/servers.txt
```

内容为：

```text
api
worker
scheduler
```

其中：

```bash
> servers.txt
```

表示把整个 `printf` 命令的标准输出重定向到文件。

如果文件已经存在，`>` 会覆盖原内容。

---

## 8. 完整示例

```bash
cd /home/learner/bash-lab

servers=(api worker scheduler)
printf '%s\n' "${servers[@]}" > servers.txt
```

可以检查结果：

```bash
cat servers.txt
```

应输出：

```text
api
worker
scheduler
```

---

## 9. 数组和普通字符串的区别

### 普通字符串

```bash
servers="api worker scheduler"
```

逻辑上只有一个值：

```text
"api worker scheduler"
```

### 数组

```bash
servers=(api worker scheduler)
```

逻辑上有三个值：

```text
servers[0] = api
servers[1] = worker
servers[2] = scheduler
```

如果程序后续需要：

* 遍历多个名称；
* 单独读取某一项；
* 保留包含空格的元素；
* 把多个值作为独立命令参数传递；

就应该优先使用数组，而不是把所有内容拼成一个字符串。

---

## 10. 数组索引不一定连续

虽然下面的数组自然使用索引 `0`、`1`、`2`：

```bash
servers=(api worker scheduler)
```

但 Bash 数组允许索引之间存在空缺：

```bash
servers[0]=api
servers[5]=worker
servers[10]=scheduler
```

这仍然是合法数组。

执行：

```bash
printf '%s\n' "${servers[@]}"
```

仍然会展开所有**实际存在的元素**：

```text
api
worker
scheduler
```

因此：

> `"${servers[@]}"` 的含义是“展开所有现有元素”，而不是机械地从索引 `0` 一直数到最大索引。

---

## 11. `"${array[@]}"` 与 `"$@"`

Bash 脚本中经常还会看到：

```bash
"$@"
```

它表示脚本收到的所有位置参数，并且同样保持：

> 一个原始参数对应一个展开后的参数。

例如运行：

```bash
./script.sh api "background worker" scheduler
```

脚本中的：

```bash
printf '%s\n' "$@"
```

会输出：

```text
api
background worker
scheduler
```

因此可以把二者类比记忆：

```bash
"${servers[@]}"
```

表示：

> 数组中的每个元素各自作为一个参数。

而：

```bash
"$@"
```

表示：

> 脚本收到的每个位置参数各自作为一个参数。

---

## 12. 常见误区

### 误区一：把多个值存成一个字符串

```bash
servers="api worker scheduler"
```

这并不是真正的三元素数组。

需要多个独立值时，应写：

```bash
servers=(api worker scheduler)
```

### 误区二：省略数组展开外的双引号

不推荐：

```bash
printf '%s\n' ${servers[@]}
```

推荐：

```bash
printf '%s\n' "${servers[@]}"
```

前者在数组元素包含空格、通配符等特殊内容时可能产生错误拆分。

### 误区三：手工写三次输出

虽然可以写：

```bash
printf '%s\n' api
printf '%s\n' worker
printf '%s\n' scheduler
```

但这样数据没有真正存储在一个可复用的数据结构中。

更好的方式是：

```bash
servers=(api worker scheduler)
printf '%s\n' "${servers[@]}"
```

数据与输出逻辑因此被分开：

* 数组负责保存数据；
* `printf` 负责输出数据。

---

## 13. 核心记忆

创建索引数组：

```bash
servers=(api worker scheduler)
```

访问单个元素：

```bash
"${servers[0]}"
```

展开全部元素，并保持元素边界：

```bash
"${servers[@]}"
```

逐行打印全部元素：

```bash
printf '%s\n' "${servers[@]}"
```

写入文件：

```bash
printf '%s\n' "${servers[@]}" > servers.txt
```

完整命令：

```bash
cd /home/learner/bash-lab
servers=(api worker scheduler)
printf '%s\n' "${servers[@]}" > servers.txt
```

最终 `servers.txt` 应严格包含三行：

```text
api
worker
scheduler
```
