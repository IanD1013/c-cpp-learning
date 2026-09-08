# Bash 变量赋值、引用与命令替换

## 1. Bash 变量赋值的基本语法

Bash 中变量赋值必须写成：

```bash
name=value
```

`=` 两边不能有空格。

正确：

```bash
label=server
```

错误：

```bash
label = server
```

因为 Bash 会把空格当作命令和参数之间的分隔符，而不是赋值语法的一部分。

---

## 2. 为什么 `label=course server` 是错误的

下面这条命令：

```bash
label=course server
```

并不表示：

```text
把 "course server" 保存到 label
```

Bash 实际上会把它解析成：

```text
label=course    server
```

这里：

```bash
label=course
```

被解释为给紧随其后的命令临时设置环境变量，而：

```bash
server
```

被当成要执行的命令。

因此 Bash 会尝试寻找一个名为 `server` 的程序，最终可能得到：

```text
server: command not found
```

如果变量值中包含空格，必须使用引号：

```bash
label='course server'
```

或者：

```bash
label="course server"
```

---

## 3. 单引号与双引号

### 单引号 `'...'`

单引号中的内容基本按字面值处理。

```bash
label='course server'
```

结果：

```text
course server
```

单引号内部不会进行变量展开：

```bash
name='Linux'
text='$name'
```

此时：

```bash
printf '%s\n' "$text"
```

输出：

```text
$name
```

因此，当字符串完全是固定文本时，单引号通常最直接。

---

### 双引号 `"..."`

双引号同样可以保护空格，但仍然允许 `$变量`、命令替换等展开。

```bash
name='Linux'
text="kernel: $name"
```

结果：

```text
kernel: Linux
```

因此需要读取变量值时，通常使用双引号：

```bash
printf '%s\n' "$label"
```

---

## 4. 变量展开时为什么通常要加双引号

读取变量：

```bash
$label
```

例如：

```bash
label='course server'
```

下面两种写法的意义不同：

```bash
printf '%s' $label
```

和：

```bash
printf '%s' "$label"
```

未加引号时，变量展开后的内容可能发生 **word splitting（单词分割）**：

```text
course server
```

会被 Bash 拆成两个单词。

而：

```bash
"$label"
```

会把整个值作为一个参数：

```text
course server
```

### 推荐习惯

除非明确需要 Bash 对展开结果进行拆分，否则应默认写：

```bash
"$variable"
```

而不是：

```bash
$variable
```

这是 Bash 脚本中非常重要的习惯。

---

## 5. 命令替换：把命令输出保存到变量

语法：

```bash
variable=$(command)
```

例如：

```bash
kernel=$(uname -s)
```

执行过程：

1. Bash 执行：

   ```bash
   uname -s
   ```

2. 获取命令的标准输出。

3. 删除输出末尾的换行符。

4. 将剩余内容赋值给 `kernel`。

在 Linux 上通常得到：

```bash
kernel='Linux'
```

因此：

```bash
printf 'kernel: %s\n' "$kernel"
```

会输出：

```text
kernel: Linux
```

---

## 6. 为什么命令替换不会保留末尾换行

直接运行：

```bash
uname -s
```

实际上会输出类似：

```text
Linux\n
```

但：

```bash
kernel=$(uname -s)
```

会删除末尾的换行符，因此变量中的值实际上类似：

```text
Linux
```

这样可以由脚本自己决定输出格式：

```bash
printf 'kernel: %s\n' "$kernel"
```

而不会出现多余空行。

---

## 7. 使用 `printf` 输出变量

推荐：

```bash
printf 'label: %s\n' "$label"
printf 'kernel: %s\n' "$kernel"
```

格式字符串：

```text
label: %s\n
```

含义：

* `label: `：固定文本
* `%s`：插入一个字符串参数
* `\n`：换行

因此：

```bash
label='course server'
printf 'label: %s\n' "$label"
```

输出：

```text
label: course server
```

相比依赖 `echo`，`printf` 的格式行为通常更加明确和可预测。

---

## 8. Bash 脚本中的典型组合

```bash
#!/bin/bash

label='course server'
kernel=$(uname -s)

printf 'label: %s\n' "$label"
printf 'kernel: %s\n' "$kernel"
```

这里同时使用了三个核心 Bash 技巧：

1. 带空格的固定字符串使用引号：

   ```bash
   label='course server'
   ```

2. 使用命令替换获取命令输出：

   ```bash
   kernel=$(uname -s)
   ```

3. 展开变量时使用双引号：

   ```bash
   "$label"
   "$kernel"
   ```

---

## 9. 核心规则总结

| 需求         | 推荐写法                      |
| ---------- | ------------------------- |
| 普通变量赋值     | `name=value`              |
| 值中包含空格     | `name='two words'`        |
| 读取变量       | `"$name"`                 |
| 固定字符串      | `'...'`                   |
| 字符串中需要变量展开 | `"..."`                   |
| 保存命令输出     | `result=$(command)`       |
| 格式化输出      | `printf '%s\n' "$result"` |

最值得形成习惯的两条规则是：

```bash
name=value
```

`=` 两边不要有空格。

以及：

```bash
"$variable"
```

变量展开默认加双引号。

---

# Bash 实践：创建 `course-host-report`

## 问题

创建 Bash 脚本：

```text
/home/learner/bin/course-host-report
```

要求：

* 使用 Bash；

* 文件权限为 `755`；

* 文件由当前用户拥有；

* 设置：

  ```text
  label = course server
  ```

* 使用 `uname -s` 获取内核名称；

* 输出两行；

* 将脚本输出保存到：

  ```text
  /home/learner/host-report
  ```

最终文件内容应为：

```text
label: course server
kernel: Linux
```

## 脚本内容

创建文件：

```bash
mkdir -p /home/learner/bin

cat > /home/learner/bin/course-host-report <<'EOF'
#!/bin/bash

label='course server'
kernel=$(uname -s)

printf 'label: %s\n' "$label"
printf 'kernel: %s\n' "$kernel"
EOF
```

脚本中的关键部分：

```bash
label='course server'
```

引号保证空格属于变量值，而不是 Bash 的参数分隔符。

```bash
kernel=$(uname -s)
```

执行 `uname -s`，并把输出保存到变量 `kernel`。

```bash
printf 'label: %s\n' "$label"
printf 'kernel: %s\n' "$kernel"
```

分别输出两行。

## 设置执行权限

```bash
chmod 755 /home/learner/bin/course-host-report
```

`755` 表示：

| 对象    | 权限           |
| ----- | ------------ |
| 文件所有者 | `rwx`：读、写、执行 |
| 所属组   | `r-x`：读、执行   |
| 其他用户  | `r-x`：读、执行   |

如果文件由当前用户创建，它通常已经属于当前用户。

可检查：

```bash
ls -l /home/learner/bin/course-host-report
```

权限应类似：

```text
-rwxr-xr-x
```

## 运行脚本并重定向输出

```bash
/home/learner/bin/course-host-report > /home/learner/host-report
```

这里的：

```bash
>
```

表示把脚本的标准输出写入文件。

如果目标文件已经存在，它会被覆盖。

## 验证结果

```bash
cat /home/learner/host-report
```

在 Linux 系统上应得到：

```text
label: course server
kernel: Linux
```

完整执行流程可以写成：

```bash
mkdir -p /home/learner/bin

cat > /home/learner/bin/course-host-report <<'EOF'
#!/bin/bash

label='course server'
kernel=$(uname -s)

printf 'label: %s\n' "$label"
printf 'kernel: %s\n' "$kernel"
EOF

chmod 755 /home/learner/bin/course-host-report

/home/learner/bin/course-host-report > /home/learner/host-report

cat /home/learner/host-report
```
