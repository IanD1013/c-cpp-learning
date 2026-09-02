# Bash 中使用 `basename`、命令替换与变量保存路径末尾名称

## 1. 场景：从路径中提取最后一部分

假设有一个路径：

```bash
/srv/releases/api
```

其中：

* `/srv/releases/` 是目录路径；
* `api` 是路径最后一部分，通常可以表示服务名、组件名或目录名。

如果路径以后可能变化，不应该手动把 `api` 写死，而应该让 Bash 自动提取。

可以使用 `basename`：

```bash
basename /srv/releases/api
```

输出：

```text
api
```

`basename` 的作用就是：

> 从路径中去掉前面的目录部分，只保留最后一个名称。

例如：

```bash
basename /home/user/project
```

输出：

```text
project
```

---

## 2. 为什么直接运行 `basename` 不够

下面的命令：

```bash
basename /srv/releases/api
```

只是把结果打印到标准输出：

```text
api
```

如果后面的命令还需要使用这个结果，就需要把输出保存起来。

Bash 中通常使用**命令替换（command substitution）**：

```bash
$(command)
```

Bash 会：

1. 先执行括号中的命令；
2. 捕获它的标准输出；
3. 用这个输出替换整个 `$(...)`。

例如：

```bash
$(basename /srv/releases/api)
```

可以理解为 Bash 先执行：

```bash
basename /srv/releases/api
```

得到：

```text
api
```

然后：

```bash
$(basename /srv/releases/api)
```

整体就相当于：

```text
api
```

---

## 3. 把命令输出保存到变量

命令替换最常见的用法是配合变量赋值：

```bash
component=$(basename /srv/releases/api)
```

执行顺序是：

```text
basename /srv/releases/api
        ↓
       api
        ↓
component=api
```

因此变量 `component` 保存的是：

```text
api
```

而不是：

```text
/srv/releases/api
```

也不是：

```text
basename /srv/releases/api
```

原因是 `basename` 已经在赋值过程中执行完毕，变量保存的是它产生的输出。

可以检查：

```bash
printf '%s\n' "$component"
```

输出：

```text
api
```

---

## 4. 命令替换不是“保存一条以后再执行的命令”

这一点很容易混淆。

```bash
component=$(basename /srv/releases/api)
```

并不是把下面这段文字保存进变量：

```text
basename /srv/releases/api
```

而是立即执行 `basename`，再保存它的结果。

也就是说：

```bash
variable=$(command)
```

表示：

> 执行 `command`，把它的标准输出保存到 `variable`。

例如：

```bash
current_user=$(whoami)
```

如果 `whoami` 输出：

```text
alice
```

那么最终等价于：

```bash
current_user=alice
```

---

## 5. 命令替换会删除末尾换行符

很多 Unix 命令输出文本时都会在最后附带换行符。

例如：

```bash
basename /srv/releases/api
```

实际输出可以理解为：

```text
api\n
```

其中 `\n` 表示换行。

但是使用命令替换：

```bash
component=$(basename /srv/releases/api)
```

时，Bash 会删除命令输出末尾的换行符。

因此 `component` 中保存的是：

```text
api
```

而不是：

```text
api\n
```

这也是为什么下面的命令需要通过 `printf` 主动添加换行：

```bash
printf '%s\n' "$component"
```

这里的：

```text
\n
```

负责在输出末尾增加换行。

---

## 6. 把变量内容写入文件

如果需要把提取出的服务名保存到文件：

```bash
component=$(basename /srv/releases/api)
printf '%s\n' "$component" > component.txt
```

最终：

```text
component.txt
```

中的内容是：

```text
api
```

并且文件末尾有一个换行符。

如果当前目录是：

```text
/home/learner/bash-lab
```

那么最终文件路径就是：

```text
/home/learner/bash-lab/component.txt
```

---

## 7. 两条命令分别做了什么

完整命令：

```bash
component=$(basename /srv/releases/api)
printf '%s\n' "$component" > component.txt
```

### 第一条：计算并保存结果

```bash
component=$(basename /srv/releases/api)
```

可以拆成：

```bash
basename /srv/releases/api
```

得到：

```text
api
```

然后保存：

```bash
component=api
```

### 第二条：格式化并重定向到文件

```bash
printf '%s\n' "$component" > component.txt
```

其中：

```bash
printf '%s\n' "$component"
```

输出：

```text
api
```

末尾带换行。

而：

```bash
> component.txt
```

把标准输出写入文件，而不是显示在终端。

最终：

```text
component.txt
```

内容为：

```text
api
```

---

## 8. 为什么应该写 `"$component"`

推荐：

```bash
printf '%s\n' "$component"
```

而不是：

```bash
printf '%s\n' $component
```

双引号可以确保变量展开后的内容始终作为**一个参数**传给命令。

例如：

```bash
name="my service"
```

使用：

```bash
printf '%s\n' "$name"
```

`printf` 接收到一个参数：

```text
my service
```

而未加引号：

```bash
printf '%s\n' $name
```

Bash 可能会把展开结果按空格拆成：

```text
my
service
```

因此 Bash 中一个非常重要的习惯是：

```bash
"$variable"
```

> 使用变量值时，除非明确需要单词拆分，否则通常应该加双引号。

---

## 9. 命令替换外面通常不需要额外引号进行变量赋值

下面这种变量赋值是正常且常见的：

```bash
component=$(basename /srv/releases/api)
```

在普通 Bash 变量赋值中，命令替换的结果不会因为空格而被拆成多个参数，因此这里通常不需要写成：

```bash
component="$(basename /srv/releases/api)"
```

不过后一种写法也是合法的：

```bash
component="$(basename /srv/releases/api)"
```

在实际 Shell 脚本中，很多人仍然习惯给命令替换加双引号，以明确表达“整个结果作为一个字符串使用”。

相比之下，在变量真正作为命令参数使用时，双引号通常更加重要：

```bash
printf '%s\n' "$component"
```

---

## 10. `>` 重定向会覆盖文件

命令：

```bash
printf '%s\n' "$component" > component.txt
```

中的 `>` 表示：

> 将标准输出写入文件，并覆盖文件原有内容。

如果文件原来是：

```text
old data
```

执行：

```bash
printf '%s\n' "$component" > component.txt
```

后，旧内容会被替换为：

```text
api
```

如果想追加而不是覆盖，应使用：

```bash
>>
```

例如：

```bash
printf '%s\n' "$component" >> component.txt
```

---

## 11. 常见误区

### 误区一：认为变量保存的是完整路径

```bash
component=$(basename /srv/releases/api)
```

不会保存：

```text
/srv/releases/api
```

因为 `basename` 已经把目录部分删除了。

真正保存的是：

```text
api
```

---

### 误区二：认为变量保存的是命令本身

变量不会保存：

```text
basename /srv/releases/api
```

`$(...)` 的含义是**立即执行命令并获取输出**，不是延迟执行。

---

### 误区三：忘记 `$()`

下面的写法：

```bash
component=basename /srv/releases/api
```

并不表示“执行 basename 并保存输出”。

如果需要命令的结果，必须使用命令替换：

```bash
component=$(basename /srv/releases/api)
```

---

### 误区四：把反引号和现代语法混淆

旧式 Shell 脚本中可能看到：

```bash
component=`basename /srv/releases/api`
```

它和：

```bash
component=$(basename /srv/releases/api)
```

基本作用相同，但现代 Bash 更推荐：

```bash
$(...)
```

因为它：

* 更容易阅读；
* 更容易嵌套；
* 不容易和普通单引号混淆。

例如嵌套：

```bash
result=$(basename "$(pwd)")
```

明显比反引号形式更清晰。

---

## 12. 一个通用模式

保存命令输出的基本模板是：

```bash
variable=$(command argument)
```

之后使用：

```bash
printf '%s\n' "$variable"
```

如果要写入文件：

```bash
printf '%s\n' "$variable" > output.txt
```

例如：

```bash
filename=$(basename /tmp/archive/report.txt)
printf '%s\n' "$filename" > filename.txt
```

最终 `filename.txt` 中是：

```text
report.txt
```

---

## 13. 核心知识总结

```bash
component=$(basename /srv/releases/api)
printf '%s\n' "$component" > component.txt
```

包含三个核心 Bash 概念：

1. **`basename`**

   ```bash
   basename /srv/releases/api
   ```

   从路径中提取最后一部分：

   ```text
   api
   ```

2. **命令替换 `$()`**

   ```bash
   component=$(...)
   ```

   执行命令，并把标准输出保存到变量中；末尾换行符会被删除。

3. **输出重定向 `>`**

   ```bash
   > component.txt
   ```

   将命令的标准输出写入文件，并覆盖原有内容。

最终：

```bash
component=$(basename /srv/releases/api)
printf '%s\n' "$component" > component.txt
```

会创建或覆盖 `component.txt`，使其包含：

```text
api
```

并以一个换行符结束。
