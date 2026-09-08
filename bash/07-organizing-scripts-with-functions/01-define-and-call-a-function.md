# Bash 函数：封装重复操作并使用脚本参数

## 1. 为什么需要函数

当同一组命令会在脚本中执行多次时，直接复制命令容易产生两个问题：

* 重复代码越来越多；
* 某次修改时只改了一处，导致不同位置的行为逐渐不一致。

Bash 可以用 **函数（function）** 把一组命令放到一个名字下面，需要时通过这个名字执行。

例如：

```bash
greet() {
  printf 'hello\n' > greeting.txt
}
```

这里定义了一个名为 `greet` 的函数。

定义函数本身**不会执行函数体**。只有调用：

```bash
greet
```

函数体中的命令才会真正运行。

---

## 2. Bash 函数的基本语法

标准形式：

```bash
function_name() {
  command1
  command2
}
```

例如：

```bash
write_header() {
  printf '%s\n' "$heading" > report.txt
}
```

各部分含义：

```text
write_header() {
    ...
}
```

* `write_header`：函数名；
* `()`：Bash 函数定义语法，这里通常不写参数；
* `{ ... }`：函数体；
* 函数体中的命令在调用函数时执行。

### 多行写法

```bash
greet() {
  printf 'hello\n'
}
```

每个换行可以结束一条命令，因此最后一条命令后通常不需要 `;`。

### 单行写法

```bash
greet() { printf 'hello\n'; }
```

这里必须在 `}` 前加入 `;`：

```bash
printf 'hello\n';
```

原因是 Bash 需要一个命令结束符，将最后一条命令与 `}` 分开。

---

## 3. 定义函数和调用函数是两件事

下面的代码只是在定义：

```bash
greet() {
  printf 'hello\n'
}
```

执行到这里时：

* Bash 记住函数名 `greet`；
* Bash 记住对应的函数体；
* `printf` 不会立即执行。

真正执行函数需要：

```bash
greet
```

可以把函数理解为一种“自定义命令”。

完整示例：

```bash
#!/usr/bin/env bash

greet() {
  printf 'hello\n'
}

greet
```

执行：

```bash
bash script.sh
```

输出：

```text
hello
```

---

## 4. 脚本参数 `$1`

运行 Bash 脚本时，可以在脚本名后面传入参数：

```bash
bash report.sh Inventory
```

此时在脚本顶层：

```bash
$1
```

表示第一个参数：

```text
Inventory
```

例如：

```bash
#!/usr/bin/env bash

heading="$1"

printf '%s\n' "$heading"
```

运行：

```bash
bash report.sh Inventory
```

得到：

```text
Inventory
```

运行：

```bash
bash report.sh Backlog
```

得到：

```text
Backlog
```

同一个脚本因此可以处理不同输入，而不需要修改脚本内容。

---

## 5. 为什么先保存 `$1`，再让函数使用变量

一种清晰的写法是：

```bash
heading="$1"

write_header() {
  printf '%s\n' "$heading" > report.txt
}

write_header
```

这里：

```bash
heading="$1"
```

先把**脚本的第一个参数**保存到普通变量 `heading` 中。

之后函数读取：

```bash
"$heading"
```

这是安全且容易理解的。

### 为什么不直接在函数里写 `$1`

需要注意一个重要规则：

> Bash 函数拥有自己的一组位置参数。

例如：

```bash
#!/usr/bin/env bash

show() {
  printf '%s\n' "$1"
}

show Hello
```

函数内部的：

```bash
$1
```

表示传给 `show` 的第一个参数，也就是：

```text
Hello
```

它不是自动表示整个脚本的 `$1`。

因此，如果一个值本来属于脚本级配置，可以先保存：

```bash
heading="$1"
```

然后在函数中使用：

```bash
"$heading"
```

这样可以避免混淆“脚本参数”和“函数参数”。

---

## 6. 变量为什么能在函数里读取

下面的代码可以工作：

```bash
heading="$1"

write_header() {
  printf '%s\n' "$heading"
}
```

因为 Bash 函数默认可以读取调用环境中的 shell 变量。

也就是说，脚本先定义：

```bash
heading="Inventory"
```

函数中就可以访问：

```bash
"$heading"
```

这与许多编程语言中的“外层作用域变量”概念类似。

---

## 7. 变量展开为什么要加双引号

推荐：

```bash
printf '%s\n' "$heading"
```

而不是：

```bash
printf '%s\n' $heading
```

假设：

```bash
heading="Weekly Inventory"
```

带双引号：

```bash
printf '%s\n' "$heading"
```

传给 `printf` 的是一个完整参数：

```text
Weekly Inventory
```

输出：

```text
Weekly Inventory
```

不加双引号时：

```bash
printf '%s\n' $heading
```

Shell 可能进行单词拆分，相当于传入两个参数：

```text
Weekly
Inventory
```

而 `printf` 会重复使用格式字符串：

```text
Weekly
Inventory
```

于是原本应该在同一行的标题被拆成两行。

### Bash 中的重要习惯

除非明确需要单词拆分或通配符展开，否则变量展开通常应该写成：

```bash
"$variable"
```

而不是：

```bash
$variable
```

---

## 8. 使用 `printf` 写入文件

要把标题写进 `report.txt`：

```bash
printf '%s\n' "$heading" > report.txt
```

这里：

```bash
'%s\n'
```

表示：

* `%s`：输出一个字符串；
* `\n`：最后换行。

例如：

```bash
heading="Inventory"
```

执行后，文件内容为：

```text
Inventory
```

文件末尾还包含一个换行符。

---

## 9. `>` 会覆盖旧文件

命令：

```bash
printf '%s\n' "$heading" > report.txt
```

中的：

```bash
>
```

表示：

> 将标准输出写入文件，并覆盖文件原来的内容。

假设 `report.txt` 原来是：

```text
Old Report
Some old data
```

再次运行：

```bash
printf '%s\n' "Inventory" > report.txt
```

文件会变成：

```text
Inventory
```

旧内容被清除。

这非常适合“每次重新生成文件”的场景。

### 与 `>>` 的区别

追加：

```bash
printf '%s\n' "$heading" >> report.txt
```

不会删除旧内容，而是在末尾继续写。

例如运行两次：

```bash
printf '%s\n' "Inventory" >> report.txt
printf '%s\n' "Backlog" >> report.txt
```

结果：

```text
Inventory
Backlog
```

因此：

| 操作符  | 行为      |
| ---- | ------- |
| `>`  | 覆盖文件    |
| `>>` | 追加到文件末尾 |

如果报告每次都应该重新构建，应使用：

```bash
>
```

---

## 10. 完整示例：`report.sh`

目录：

```text
/home/learner/bash-lab/
├── report.sh
└── report.txt
```

`report.sh`：

```bash
#!/usr/bin/env bash

heading="$1"

write_header() {
  printf '%s\n' "$heading" > report.txt
}

write_header
```

运行：

```bash
cd /home/learner/bash-lab
bash report.sh Inventory
```

此时：

```text
/home/learner/bash-lab/report.txt
```

内容为：

```text
Inventory
```

再次运行：

```bash
bash report.sh Backlog
```

文件会被重新写成：

```text
Backlog
```

而不是：

```text
Inventory
Backlog
```

因为脚本使用的是：

```bash
>
```

而不是：

```bash
>>
```

---

## 11. Shebang 的作用

脚本第一行常写：

```bash
#!/usr/bin/env bash
```

这叫 **shebang**。

它表示：

> 如果这个文件被直接作为可执行程序运行，就使用环境中的 `bash` 来解释它。

例如先添加执行权限：

```bash
chmod +x report.sh
```

然后可以：

```bash
./report.sh Inventory
```

如果使用：

```bash
bash report.sh Inventory
```

则是显式调用 Bash，此时即使没有执行权限也通常可以运行。

---

## 12. 常见误区

### 误区一：定义函数后以为它会自动运行

错误理解：

```bash
write_header() {
  printf '%s\n' "$heading"
}
```

函数定义完成后不会自动执行。

还需要：

```bash
write_header
```

---

### 误区二：把 `()` 当作函数参数列表

Bash 的：

```bash
write_header() {
  ...
}
```

中的 `()` 不是像 Python 或 JavaScript 那样写参数名称的地方。

不能写：

```bash
write_header(heading) {
  ...
}
```

Bash 函数参数是在**调用时**传入：

```bash
write_header Inventory
```

然后函数内部通过：

```bash
$1
```

读取。

---

### 误区三：混淆脚本的 `$1` 和函数的 `$1`

脚本：

```bash
bash report.sh Inventory
```

在脚本顶层：

```bash
$1
```

是：

```text
Inventory
```

但如果：

```bash
write_header Backlog
```

那么函数内部：

```bash
$1
```

是：

```text
Backlog
```

函数执行期间有自己的一组位置参数。

---

### 误区四：变量展开不加引号

不推荐：

```bash
printf '%s\n' $heading
```

推荐：

```bash
printf '%s\n' "$heading"
```

特别是变量可能包含：

* 空格；
* `*`；
* `?`；
* 其他可能触发 Shell 展开的字符。

---

### 误区五：本来想覆盖文件，却用了 `>>`

如果每次运行都应该重新生成报告：

```bash
> report.txt
```

相关写入应使用：

```bash
printf ... > report.txt
```

如果使用：

```bash
>>
```

旧运行产生的内容会一直累积。

---

## 13. 核心总结

Bash 函数适合把重复命令封装成一个可复用的名字：

```bash
write_header() {
  ...
}
```

定义函数不会执行它，必须显式调用：

```bash
write_header
```

脚本参数可以通过：

```bash
$1
```

读取，并保存到普通变量：

```bash
heading="$1"
```

函数可以读取这个变量：

```bash
printf '%s\n' "$heading" > report.txt
```

其中最重要的几个 Bash 习惯是：

```bash
"$variable"
```

用于安全展开变量；

```bash
>
```

用于覆盖文件；

```bash
>>
```

用于追加文件；

而函数可以把这些重复操作集中定义一次，避免脚本不同位置的实现逐渐产生差异。
