# Bash：使用 `sort`、`uniq` 和管道生成排序后的去重列表

## 目标

假设文件：

```text
/home/learner/bash-lab/tags.txt
```

包含 15 个标签，每行一个，其中可能有重复项，而且顺序是标签写入文件时的顺序。

现在需要生成：

```text
/home/learner/bash-lab/unique-tags.txt
```

要求：

* 每个不同的标签只出现一次；
* 按字母顺序排列；
* 每行一个标签；
* 原始 `tags.txt` 不被修改。

在 `/home/learner/bash-lab` 目录中可以执行：

```bash
sort tags.txt | uniq > unique-tags.txt
```

---

## 1. `sort`：按行排序

`sort` 会读取文本的每一行，并将排序后的内容写到标准输出（standard output）。

例如：

```text
beta
alpha
gamma
alpha
```

执行：

```bash
sort tags.txt
```

输出：

```text
alpha
alpha
beta
gamma
```

这里最重要的不只是“排序”，而是：

> 排序会让相同的行彼此相邻。

这正是后面的 `uniq` 能够去重的前提。

---

## 2. `uniq`：删除相邻的重复行

`uniq` 会逐行读取输入，并删除与**前一行完全相同**的重复行。

例如：

```text
alpha
alpha
beta
gamma
gamma
```

经过：

```bash
uniq
```

得到：

```text
alpha
beta
gamma
```

但需要特别注意：

> `uniq` 并不会搜索整个文件来寻找重复项，它只比较相邻的行。

因此下面这样的输入：

```text
alpha
beta
alpha
```

执行 `uniq` 后仍然是：

```text
alpha
beta
alpha
```

因为两个 `alpha` 并不相邻。

---

## 3. 为什么通常要先 `sort` 再 `uniq`

如果原始文件中的重复项散落在不同位置，例如：

```text
beta
alpha
gamma
alpha
beta
```

先排序：

```bash
sort tags.txt
```

得到：

```text
alpha
alpha
beta
beta
gamma
```

此时所有重复项已经相邻。

再交给 `uniq`：

```text
alpha
beta
gamma
```

所以经典组合是：

```bash
sort tags.txt | uniq
```

逻辑可以理解为：

```text
原始数据
   ↓
 sort
   ↓
相同内容排到一起
   ↓
 uniq
   ↓
删除相邻重复项
```

---

## 4. `|`：管道

`|` 称为 **pipe（管道）**。

基本形式：

```bash
command1 | command2
```

含义是：

> 把 `command1` 的标准输出直接连接到 `command2` 的标准输入。

因此：

```bash
sort tags.txt | uniq
```

等价于：

1. `sort` 读取 `tags.txt`；
2. `sort` 输出排序结果；
3. 排序结果不再打印到终端，而是通过管道进入 `uniq`；
4. `uniq` 对这些排序后的行进行去重；
5. `uniq` 的结果默认打印到终端。

数据流是：

```text
tags.txt
   ↓
 sort
   ↓ stdout
   |
   | 管道
   ↓ stdin
 uniq
   ↓ stdout
终端
```

管道中的中间结果不需要保存成临时文件。

---

## 5. `>`：把最终结果写入文件

如果需要把结果保存到：

```text
unique-tags.txt
```

可以使用输出重定向：

```bash
sort tags.txt | uniq > unique-tags.txt
```

数据流变成：

```text
tags.txt
   ↓
 sort
   ↓
 uniq
   ↓
unique-tags.txt
```

其中：

```bash
> unique-tags.txt
```

只作用于整个管道最后一个命令 `uniq` 的标准输出。

因此：

* `tags.txt` 只被读取；
* `sort` 的结果通过管道传给 `uniq`；
* `uniq` 的结果被写入 `unique-tags.txt`。

---

## 6. 为什么 `uniq` 不应该放在前面

下面的命令看起来也像是在“去重然后排序”：

```bash
uniq tags.txt | sort > unique-tags.txt
```

但如果重复项原本并不相邻，它就无法正确去重。

例如原始文件：

```text
beta
alpha
gamma
alpha
beta
```

先执行：

```bash
uniq tags.txt
```

因为没有两个相邻行相同，所以所有内容都会保留下来：

```text
beta
alpha
gamma
alpha
beta
```

再排序：

```text
alpha
alpha
beta
beta
gamma
```

结果虽然有序，但重复项仍然存在。

因此正确顺序是：

```bash
sort tags.txt | uniq
```

而不是：

```bash
uniq tags.txt | sort
```

核心原因是：

> `uniq` 只能删除相邻的重复行，因此必须先让 `sort` 把重复内容排列到一起。

---

## 7. 管道中为什么不给 `uniq` 文件名

正确写法：

```bash
sort tags.txt | uniq > unique-tags.txt
```

这里：

```bash
uniq
```

没有文件参数，因此它会从**标准输入**读取数据，也就是读取管道中 `sort` 发送过来的结果。

不要写成：

```bash
sort tags.txt | uniq tags.txt > unique-tags.txt
```

因为一旦明确给 `uniq` 一个文件：

```bash
uniq tags.txt
```

它就会直接读取该文件，而不是使用前面管道送来的排序结果。

这样就失去了：

```text
sort → uniq
```

这个处理链的意义。

使用管道时，一个常见原则是：

> 后面的命令如果需要处理前一个命令的结果，就应让它从标准输入读取。

---

## 8. 原始文件为什么不会被修改

执行：

```bash
sort tags.txt | uniq > unique-tags.txt
```

时：

```bash
tags.txt
```

只作为 `sort` 的输入文件：

```bash
sort tags.txt
```

没有任何命令向它写入数据。

真正被 `>` 打开并写入的是：

```text
unique-tags.txt
```

因此原始文件：

```text
tags.txt
```

会保持原来的内容和顺序。

---

## 9. 完整示例

假设 `tags.txt` 内容为：

```text
release
beta
stable
release
alpha
beta
```

执行：

```bash
sort tags.txt | uniq > unique-tags.txt
```

### 第一步：`sort`

产生：

```text
alpha
beta
beta
release
release
stable
```

### 第二步：`uniq`

产生：

```text
alpha
beta
release
stable
```

### 第三步：`>`

把结果保存到：

```text
unique-tags.txt
```

最终该文件包含：

```text
alpha
beta
release
stable
```

而原始的 `tags.txt` 仍然保持：

```text
release
beta
stable
release
alpha
beta
```

---

## 10. 更简洁的写法：`sort -u`

对于“排序并去重”这个常见任务，`sort` 本身提供了：

```bash
-u
```

选项，即 `unique`。

因此：

```bash
sort tags.txt | uniq > unique-tags.txt
```

通常也可以写成：

```bash
sort -u tags.txt > unique-tags.txt
```

两者在这种简单场景下效果相同。

不过理解：

```bash
sort tags.txt | uniq
```

仍然非常重要，因为它同时展示了三个 Bash 核心概念：

* 标准输出；
* 标准输入；
* 管道组合多个命令。

---

## 11. 常见误区

### 误区一：认为 `uniq` 会自动寻找整个文件中的重复项

错误理解：

```bash
uniq tags.txt
```

可以删除所有重复行。

实际上：

> `uniq` 只能删除连续出现的重复行。

因此一般需要：

```bash
sort tags.txt | uniq
```

---

### 误区二：把 `uniq` 放在 `sort` 前面

错误：

```bash
uniq tags.txt | sort
```

如果原始重复项不相邻，`uniq` 根本不会删除它们。

正确：

```bash
sort tags.txt | uniq
```

---

### 误区三：给管道后的 `uniq` 再指定原始文件

不应写：

```bash
sort tags.txt | uniq tags.txt
```

应该让 `uniq` 从管道读取：

```bash
sort tags.txt | uniq
```

---

### 误区四：把 `>` 和 `>>` 混淆

```bash
> unique-tags.txt
```

表示重新生成目标文件，旧内容会被清空。

```bash
>> unique-tags.txt
```

表示追加，会把新的结果添加到旧内容后面。

如果目标是每次重新生成完整的唯一标签列表，应使用：

```bash
>
```

而不是：

```bash
>>
```

---

## 12. 核心命令

在：

```text
/home/learner/bash-lab
```

目录中：

```bash
sort tags.txt | uniq > unique-tags.txt
```

含义可以逐段阅读：

```bash
sort tags.txt
```

先对原始标签排序；

```bash
|
```

把排序后的结果交给下一个命令；

```bash
uniq
```

删除已经相邻的重复标签；

```bash
> unique-tags.txt
```

把最终结果保存到新文件。

核心规律：

> `sort` 负责让重复项相邻，`uniq` 负责删除相邻重复项，`|` 负责把两个命令连接成处理流水线，`>` 负责把最终结果写入文件。
