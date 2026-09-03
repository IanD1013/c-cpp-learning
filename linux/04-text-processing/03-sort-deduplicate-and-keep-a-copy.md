# Bash：使用 `sort`、`uniq` 和 `tee` 去重并同时保存与显示结果

## 目标

假设文件 `~/host-events` 按主机到达顺序记录主机名，其中可能有重复，例如：

```text
web01
db01
web01
cache01
db01
web01
```

目标是：

* 每个主机名只保留一次；
* 按字母顺序排列；
* 写入 `/home/learner/hosts.txt`；
* 同时在终端显示结果。

可以使用：

```bash
sort ~/host-events | uniq | tee /home/learner/hosts.txt
```

如果输入中有 3 个不同的主机名，最终文件和终端都会得到类似：

```text
cache01
db01
web01
```

---

## 1. 为什么不能直接使用 `uniq`

`uniq` 的作用是合并**相邻的重复行**。

例如：

```text
web01
web01
db01
```

执行：

```bash
uniq
```

可以得到：

```text
web01
db01
```

但如果重复内容没有挨在一起：

```text
web01
db01
web01
```

那么：

```bash
uniq
```

仍然会输出：

```text
web01
db01
web01
```

因为两个 `web01` 中间隔着 `db01`。

因此要记住：

> `uniq` 不是“全局去重”，它只能消除连续出现的重复行。

---

## 2. 为什么必须先 `sort` 再 `uniq`

`sort` 会对文本行排序，因此相同的行会自动聚集到一起。

原始数据：

```text
web01
db01
web01
cache01
db01
web01
```

执行：

```bash
sort ~/host-events
```

得到：

```text
cache01
db01
db01
web01
web01
web01
```

此时再交给：

```bash
uniq
```

就会得到：

```text
cache01
db01
web01
```

所以经典的文本去重管道是：

```bash
sort file | uniq
```

执行顺序非常重要：

```text
原始数据
   │
   ▼
 sort
   │
   │ 相同的行被放到一起
   ▼
 uniq
   │
   │ 删除相邻重复行
   ▼
唯一且排序后的结果
```

---

## 3. 管道 `|` 如何连接命令

下面的命令：

```bash
sort ~/host-events | uniq
```

表示：

1. `sort` 读取 `~/host-events`；
2. `sort` 将排序后的结果写到标准输出 `stdout`；
3. `|` 把这些数据直接传给 `uniq` 的标准输入 `stdin`；
4. `uniq` 对数据进行去重。

不需要创建临时文件：

```bash
sort ~/host-events > sorted.txt
uniq sorted.txt
```

可以直接写成：

```bash
sort ~/host-events | uniq
```

这也是 Unix/Linux 命令行的重要思想：

> 让多个小工具通过管道组成一个数据处理流程。

---

## 4. 为什么普通重定向 `>` 不够

如果执行：

```bash
sort ~/host-events | uniq > /home/learner/hosts.txt
```

结果会写入文件：

```text
/home/learner/hosts.txt
```

但是终端不会显示这些内容。

原因是：

```bash
>
```

把命令的标准输出整体重定向到了文件。

数据流变成：

```text
sort → uniq → hosts.txt
```

而不是终端。

---

## 5. `tee`：同时写文件和显示到屏幕

如果希望同一份输出：

* 保存到文件；
* 同时继续输出到终端；

可以使用：

```bash
tee
```

完整命令：

```bash
sort ~/host-events | uniq | tee /home/learner/hosts.txt
```

数据流可以理解为：

```text
~/host-events
     │
     ▼
   sort
     │
     ▼
   uniq
     │
     ▼
    tee
   ┌─┴──────────────┐
   ▼                ▼
hosts.txt         stdout
                    │
                    ▼
                  终端
```

`tee` 会：

1. 从 `stdin` 读取数据；
2. 把数据写入指定文件；
3. 同时把完全相同的数据写到自己的 `stdout`。

因此它非常适合“一份数据给两个消费者”的情况。

---

## 6. `tee` 与 `>` 的区别

| 写法                    | 写入文件 | 显示在终端 |
| --------------------- | ---: | ----: |
| `command > file`      |    ✅ |     ❌ |
| `command \| tee file` |    ✅ |     ✅ |

例如：

```bash
printf 'hello\n' > output.txt
```

只保存：

```text
output.txt
```

而：

```bash
printf 'hello\n' | tee output.txt
```

既保存到文件，也显示：

```text
hello
```

---

## 7. 最终命令逐段理解

```bash
sort ~/host-events | uniq | tee /home/learner/hosts.txt
```

### `sort ~/host-events`

读取：

```text
~/host-events
```

并按行排序，使相同主机名彼此相邻。

### `| uniq`

将排序结果交给 `uniq`，删除相邻重复行。

### `| tee /home/learner/hosts.txt`

将最终结果：

* 写入 `/home/learner/hosts.txt`；
* 同时输出到终端。

最终文件满足：

* 一个主机名一行；
* 没有重复；
* 已排序。

---

## 8. 更简洁的写法：`sort -u`

`sort` 本身也支持去重：

```bash
sort -u ~/host-events
```

其中：

```text
-u = unique
```

因此实际工作中可以把：

```bash
sort ~/host-events | uniq
```

简化为：

```bash
sort -u ~/host-events
```

完整命令可以写成：

```bash
sort -u ~/host-events | tee /home/learner/hosts.txt
```

不过学习 `sort | uniq` 仍然很重要，因为它能清楚体现：

> `uniq` 只能处理相邻重复项，所以通常需要先排序。

---

## 核心记忆

```bash
sort file | uniq
```

用于：

> 排序后去重。

```bash
command | tee file
```

用于：

> 一边保存输出，一边继续显示输出。

因此本例的完整处理流程是：

```bash
sort ~/host-events | uniq | tee /home/learner/hosts.txt
```

也可以简写为：

```bash
sort -u ~/host-events | tee /home/learner/hosts.txt
```
