# Bash `for` 循环：批量处理固定列表并统一重定向输出

## 1. 为什么使用 `for` 循环

当多个对象需要执行完全相同的操作时，不应该重复写多份相同代码。

例如需要依次输出三个服务的状态：

```text
api ready
worker ready
web ready
```

可以重复写：

```bash
printf '%s %s\n' "api" "$1"
printf '%s %s\n' "worker" "$1"
printf '%s %s\n' "web" "$1"
```

但如果以后增加第四个服务，就必须再复制一行代码。

更合适的方法是使用 `for` 循环：

```bash
for service in api worker web
do
  printf '%s %s\n' "$service" "$1"
done
```

核心思想是：

> **把需要处理的数据写成列表，把重复执行的操作只写一次。**

---

## 2. `for` 循环的基本结构

Bash 中最常见的列表循环语法是：

```bash
for 变量 in 值1 值2 值3
do
  命令
done
```

例如：

```bash
for service in api worker web
do
  printf '%s\n' "$service"
done
```

执行过程相当于：

```text
service=api
执行循环体

service=worker
执行循环体

service=web
执行循环体
```

因此输出：

```text
api
worker
web
```

---

## 3. 迭代变量是什么

在：

```bash
for service in api worker web
```

中：

* `service` 是**迭代变量（iteration variable）**
* `api worker web` 是需要依次遍历的值

Bash 每执行一轮循环，就重新给 `service` 赋一个值。

### 第一轮

```bash
service=api
```

所以：

```bash
"$service"
```

得到：

```text
api
```

### 第二轮

```bash
service=worker
```

### 第三轮

```bash
service=web
```

因此：

```bash
"$service"
```

始终表示**当前这一轮正在处理的服务**，而不是整个列表。

---

## 4. `do` 和 `done`

完整结构：

```bash
for service in api worker web
do
  ...
done
```

其中：

* `do`：开始循环体
* `done`：结束整个循环

可以理解为：

```text
for ...
开始
    重复执行的代码
结束
```

`done` 是循环的**结束标记**，不是循环体的开始。

---

## 5. 在循环中使用命令行参数

假设脚本通过第一个位置参数接收服务状态：

```bash
bash services.sh ready
```

这里：

```bash
$1
```

的值就是：

```text
ready
```

如果执行：

```bash
bash services.sh draining
```

那么：

```bash
$1
```

就是：

```text
draining
```

因此可以把固定的服务名称和调用者提供的状态组合起来：

```bash
for service in api worker web
do
  printf '%s %s\n' "$service" "$1"
done
```

执行：

```bash
bash services.sh ready
```

得到：

```text
api ready
worker ready
web ready
```

执行：

```bash
bash services.sh draining
```

得到：

```text
api draining
worker draining
web draining
```

---

## 6. `printf` 如何生成每一行

循环中的命令：

```bash
printf '%s %s\n' "$service" "$1"
```

格式字符串是：

```bash
'%s %s\n'
```

含义：

| 部分       | 含义           |
| -------- | ------------ |
| 第一个 `%s` | 输出当前服务名      |
| 中间空格     | 在两个值之间输出一个空格 |
| 第二个 `%s` | 输出 `$1` 中的状态 |
| `\n`     | 换行           |

例如当前：

```bash
service=api
$1=ready
```

那么：

```bash
printf '%s %s\n' "$service" "$1"
```

输出：

```text
api ready
```

下一轮 `service=worker` 时，同一条命令自动变成：

```text
worker ready
```

这正是循环的价值：**代码不变，数据变化。**

---

## 7. 给整个循环统一添加输出重定向

完整脚本可以写成：

```bash
#!/usr/bin/env bash

for service in api worker web
do
  printf '%s %s\n' "$service" "$1"
done > services.txt
```

关键部分是：

```bash
done > services.txt
```

这里的：

```bash
> services.txt
```

作用于**整个 `for` 循环产生的标准输出**，而不仅仅是最后一轮。

因此三次 `printf` 的输出都会进入：

```text
services.txt
```

执行：

```bash
bash services.sh ready
```

文件内容为：

```text
api ready
worker ready
web ready
```

---

## 8. 为什么推荐把重定向写在 `done` 后面

当然，也可以每轮都单独重定向，例如：

```bash
for service in api worker web
do
  printf '%s %s\n' "$service" "$1" >> services.txt
done
```

但这里必须使用 `>>`，否则每一轮都会覆盖上一轮。

相比之下：

```bash
for service in api worker web
do
  printf '%s %s\n' "$service" "$1"
done > services.txt
```

更清晰：

1. 循环只负责产生输出；
2. 重定向只写一次；
3. 整个循环被视为一个整体；
4. 文件只在循环开始前被截断一次。

这是更好的写法。

---

## 9. `>` 与 `>>` 的重要区别

### `>`：覆盖文件

```bash
> services.txt
```

会先清空已有文件内容，然后写入新的输出。

因此连续执行两次：

```bash
bash services.sh ready
bash services.sh ready
```

最终仍然只有：

```text
api ready
worker ready
web ready
```

不会变成六行。

---

### `>>`：追加文件

如果改成：

```bash
done >> services.txt
```

第二次执行会把结果追加到第一次后面：

```text
api ready
worker ready
web ready
api ready
worker ready
web ready
```

因此：

* 希望**每次重新生成文件**：使用 `>`
* 希望**保留旧内容并继续添加**：使用 `>>`

---

## 10. 为什么 `>` 会让旧内容消失

这段代码：

```bash
for service in api worker web
do
  printf '%s %s\n' "$service" "$1"
done > services.txt
```

执行循环之前，Bash 会先处理重定向：

```bash
> services.txt
```

如果文件已经存在，它会被截断为长度 0。

然后循环产生的三行内容再写进去。

因此之前运行遗留下来的内容不会保留。

---

## 11. 完整示例

创建：

```text
/home/learner/bash-lab/services.sh
```

内容：

```bash
#!/usr/bin/env bash

for service in api worker web
do
  printf '%s %s\n' "$service" "$1"
done > services.txt
```

运行：

```bash
cd /home/learner/bash-lab
bash services.sh ready
```

查看结果：

```bash
cat services.txt
```

得到：

```text
api ready
worker ready
web ready
```

再次运行：

```bash
bash services.sh ready
```

文件仍然是：

```text
api ready
worker ready
web ready
```

而不是六行，因为 `>` 每次都会重新清空文件。

---

## 12. 常见误区

### 误区 1：认为 `service` 一次包含整个列表

错误理解：

```text
service = "api worker web"
```

实际情况是 Bash 每轮只赋一个值：

```text
第一轮：service=api
第二轮：service=worker
第三轮：service=web
```

---

### 误区 2：把 `>` 放进循环里

例如：

```bash
for service in api worker web
do
  printf '%s %s\n' "$service" "$1" > services.txt
done
```

这是错误的，因为每一轮都会重新清空文件。

最终只剩最后一轮：

```text
web ready
```

如果希望把整个循环的结果写入一个新文件，更适合：

```bash
for service in api worker web
do
  printf '%s %s\n' "$service" "$1"
done > services.txt
```

---

### 误区 3：为了保留每轮输出而随意使用 `>>`

下面虽然可以保留三轮输出：

```bash
for service in api worker web
do
  printf '%s %s\n' "$service" "$1" >> services.txt
done
```

但脚本再次运行时，会继续追加旧结果。

如果目标是“每次运行重新生成完整文件”，应该使用：

```bash
done > services.txt
```

---

## 13. 核心记忆

```bash
for service in api worker web
do
  printf '%s %s\n' "$service" "$1"
done > services.txt
```

可以拆成三个核心知识点：

1. **`for` 遍历列表**

   ```bash
   for service in api worker web
   ```

2. **迭代变量保存当前值**

   ```bash
   "$service"
   ```

3. **在 `done` 后重定向可以统一处理整个循环的输出**

   ```bash
   done > services.txt
   ```

最重要的思维方式是：

> 当一组不同的数据需要执行同一种操作时，用循环遍历数据；当整个循环的输出需要进入同一个文件时，可以给整个循环统一添加重定向。
