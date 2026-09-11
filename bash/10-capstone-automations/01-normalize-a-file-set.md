# Bash 批量重命名：先预检冲突，再统一执行

## 目标

将当前目录中的所有：

```text
*.LOG
```

重命名为：

```text
*.log
```

例如：

```text
API.LOG     -> API.log
worker.LOG  -> worker.log
```

需要满足以下要求：

* 只有扩展名 `.LOG` 变成 `.log`，文件名前面的部分保持不变。
* 如果所有目标文件名都空闲，才开始重命名。
* 如果任何目标已经存在，整个批次立即失败，**一个文件都不能移动**。
* 目标即使是一个失效的符号链接（broken symlink），也必须视为冲突。
* 目录里没有 `.LOG` 文件时，正常退出，状态码为 `0`。
* 冲突时退出状态码为 `2`。

---

## 最终脚本

创建：

```text
/home/learner/bash-lab/normalize-names.sh
```

内容如下：

```bash
#!/usr/bin/env bash

shopt -s nullglob

# 第一遍：只检查目标是否冲突，不移动任何文件
for file in *.LOG
do
  target=${file%.LOG}.log

  if [[ -e "$target" || -L "$target" ]]; then
    exit 2
  fi
done

# 第二遍：确认整个批次安全后，再执行重命名
for file in *.LOG
do
  target=${file%.LOG}.log
  mv -- "$file" "$target"
done
```

运行：

```bash
cd /home/learner/bash-lab
bash normalize-names.sh
```

这就是通过该练习所需要的核心实现。

---

## 为什么必须分成两遍？

最直接的写法可能是：

```bash
for file in *.LOG
do
  target=${file%.LOG}.log
  mv -- "$file" "$target"
done
```

问题是：它会**边检查边修改目录**。

假设目录中有：

```text
API.LOG
worker.LOG
worker.log
```

循环可能先执行：

```text
API.LOG -> API.log
```

随后发现：

```text
worker.log
```

已经存在。

此时目录已经变成：

```text
API.log
worker.LOG
worker.log
```

批次只执行了一半，目录同时存在两种命名规则。

因此应采用：

```text
第一遍：检查全部目标
        ↓
全部安全？
        ↓ 是
第二遍：统一执行移动
```

核心思想是：

> **在改变状态之前，先验证整个批次是否满足执行条件。**

---

## 第一遍：冲突预检

```bash
for file in *.LOG
do
  target=${file%.LOG}.log

  if [[ -e "$target" || -L "$target" ]]; then
    exit 2
  fi
done
```

这一遍绝不能执行 `mv`。

它只负责回答：

> 所有 `.LOG` 文件对应的 `.log` 目标名称是否都可以使用？

只要发现一个冲突：

```bash
exit 2
```

脚本立即结束。

因为此前没有执行任何移动，所以原目录保持完整。

---

## `${file%.LOG}.log` 是什么意思？

例如：

```bash
file="API.LOG"
```

表达式：

```bash
${file%.LOG}
```

使用 Bash 的**参数展开（parameter expansion）中的后缀删除**。

语法：

```bash
${变量%模式}
```

`%` 的含义是：

> 从变量值的末尾删除最短的匹配模式。

因此：

```bash
${file%.LOG}
```

得到：

```text
API
```

再加：

```bash
.log
```

最终：

```text
API.log
```

完整代码：

```bash
target=${file%.LOG}.log
```

### 示例

| `file`           | `target`         |
| ---------------- | ---------------- |
| `API.LOG`        | `API.log`        |
| `worker.LOG`     | `worker.log`     |
| `My Service.LOG` | `My Service.log` |

注意：

```text
API.LOG -> API.log
```

而不是：

```text
API.LOG -> api.log
```

这里只修改扩展名大小写，不修改文件主体。

---

## 为什么需要 `shopt -s nullglob`？

默认情况下，如果目录里不存在任何 `.LOG` 文件：

```bash
for file in *.LOG
```

Bash 不会得到“零个文件”。

它反而可能把未匹配的字符串原样保留下来：

```text
*.LOG
```

于是循环会执行一次，相当于：

```bash
file='*.LOG'
```

这通常不是我们想要的。

启用：

```bash
shopt -s nullglob
```

之后，如果：

```text
*.LOG
```

一个文件都匹配不到，它会展开成**零个参数**。

因此：

```bash
for file in *.LOG
do
  ...
done
```

直接执行零次。

脚本随后正常结束：

```text
exit status = 0
```

所以空目录不需要额外写特殊判断。

---

## 为什么同时检查 `-e` 和 `-L`？

冲突判断使用：

```bash
[[ -e "$target" || -L "$target" ]]
```

不能只写：

```bash
[[ -e "$target" ]]
```

### `-e`

```bash
-e "$target"
```

表示目标路径是否存在。

它可以发现：

* 普通文件
* 目录
* 有效符号链接指向的对象
* 其他存在的文件类型

例如：

```text
API.log
```

已经是普通文件时：

```bash
[[ -e "API.log" ]]
```

为真。

---

## `-e` 为什么不足以检测失效符号链接？

假设：

```text
API.log -> missing-file
```

这是一个符号链接，但它指向的：

```text
missing-file
```

已经不存在。

这种符号链接称为：

```text
broken symlink
```

或者：

```text
dangling symlink
```

此时：

```bash
[[ -e "API.log" ]]
```

可能为假，因为 `-e` 会跟随符号链接检查其目标，而目标不存在。

但目录中实际上仍然存在名为：

```text
API.log
```

的符号链接目录项。

如果继续执行：

```bash
mv -- "API.LOG" "API.log"
```

就可能覆盖这个名称。

因此还必须检查：

```bash
-L "$target"
```

---

## `-L` 的作用

```bash
[[ -L "$target" ]]
```

检查：

> 这个路径本身是不是符号链接。

它不会要求符号链接指向的目标真实存在。

因此：

```bash
[[ -e "$target" || -L "$target" ]]
```

能够同时发现：

```text
普通文件
目录
正常符号链接
失效符号链接
```

这正是本练习要求的冲突检测方式。

---

## 第二遍：真正执行重命名

只有第一遍完全结束后，才运行：

```bash
for file in *.LOG
do
  target=${file%.LOG}.log
  mv -- "$file" "$target"
done
```

此时脚本已经确认：

```text
所有目标名称在预检时都不存在
```

所以开始逐个移动。

---

## 为什么 `mv` 要写成这样？

```bash
mv -- "$file" "$target"
```

这里有两个重要的安全措施。

### 1. 双引号保护文件名

```bash
"$file"
"$target"
```

可以正确处理：

```text
My Service.LOG
```

这种带空格的文件名。

如果不加引号：

```bash
mv $file $target
```

Bash 可能把：

```text
My Service.LOG
```

拆成：

```text
My
Service.LOG
```

两个参数。

---

### 2. `--` 终止选项解析

假设文件名是：

```text
-report.LOG
```

如果直接：

```bash
mv "$file" "$target"
```

程序可能把：

```text
-report.LOG
```

误认为命令选项。

使用：

```bash
mv -- "$file" "$target"
```

表示：

> 从 `--` 后面开始，所有参数都当作文件名，不再解析为选项。

这是处理任意文件名时非常有用的习惯。

---

## 四种必须通过的情况

### 情况 1：正常批量重命名

开始：

```text
API.LOG
worker.LOG
notes.txt
```

执行：

```bash
bash normalize-names.sh
```

结果：

```text
API.log
worker.log
notes.txt
```

必须保证：

* `API.log` 内容与原 `API.LOG` 相同。
* `worker.log` 内容与原 `worker.LOG` 相同。
* `API.LOG` 不再存在。
* `worker.LOG` 不再存在。
* `notes.txt` 完全不受影响。
* 脚本退出状态为 `0`。

---

### 情况 2：单个新 `.LOG` 文件

开始：

```text
server.LOG
```

且不存在：

```text
server.log
```

执行后：

```text
server.log
```

退出状态：

```text
0
```

---

### 情况 3：没有 `.LOG` 文件

例如：

```text
notes.txt
README.md
```

由于：

```bash
shopt -s nullglob
```

两个循环都执行零次。

结果：

* 什么都不修改。
* 不创建奇怪的 `*.log` 文件。
* 退出状态为 `0`。

---

### 情况 4：目标已经存在

开始：

```text
API.LOG
API.log
worker.LOG
```

第一遍发现：

```bash
[[ -e "API.log" ]]
```

为真，于是：

```bash
exit 2
```

最终目录仍然是：

```text
API.LOG
API.log
worker.LOG
```

不能出现：

```text
API.log
worker.log
```

这种部分完成状态。

---

## 失效符号链接冲突

例如：

```bash
ln -s missing-file API.log
```

此时：

```text
API.log -> missing-file
```

即使：

```text
missing-file
```

不存在：

```bash
[[ -L "API.log" ]]
```

仍然为真。

所以脚本：

```bash
exit 2
```

并且不能移动任何 `.LOG` 文件。

---

## 如何检查退出状态？

运行：

```bash
bash normalize-names.sh
echo $?
```

正常情况应该看到：

```text
0
```

存在冲突时应该看到：

```text
2
```

---

## 如何自己快速测试

### 正常情况

```bash
cd /home/learner/bash-lab

rm -f -- API.LOG API.log worker.LOG worker.log
printf 'api\n' > API.LOG
printf 'worker\n' > worker.LOG

bash normalize-names.sh
echo $?
ls
```

应该存在：

```text
API.log
worker.log
```

---

### 普通文件冲突

```bash
rm -f -- API.LOG API.log worker.LOG worker.log
printf 'source\n' > API.LOG
printf 'existing\n' > API.log
printf 'worker\n' > worker.LOG

bash normalize-names.sh
echo $?
```

应该输出：

```text
2
```

并且：

```text
API.LOG
API.log
worker.LOG
```

都仍然存在。

---

### 失效符号链接冲突

```bash
rm -f -- API.LOG API.log missing-file
printf 'source\n' > API.LOG
ln -s missing-file API.log

bash normalize-names.sh
echo $?
```

应该得到：

```text
2
```

并且 `API.LOG` 没有被移动。

---

## 这种方案并不是原子的

“两遍检查”解决的是：

> 因为事先已经存在目标文件，而造成批次执行一半的问题。

但它并不能保证真正的**原子性（atomicity）**。

第一遍检查完成以后，到第二遍执行 `mv` 之间，目录状态仍然可能变化。

例如：

```text
第一遍确认 API.log 不存在
             ↓
其他进程突然创建 API.log
             ↓
第二遍开始 mv
```

另外，第二遍中的某个 `mv` 自身也可能失败，例如：

* 权限变化；
* 文件被删除；
* 文件系统错误；
* 目录状态被其他进程改变。

由于脚本没有实现事务或回滚，前面的文件可能已经成功移动。

因此这里的保证是：

> **预检发现冲突时，保证一个文件都不移动。**

而不是：

> **整个批量操作在任何情况下都具有事务级原子性。**

---

## 最终应记住的模式

```bash
#!/usr/bin/env bash

shopt -s nullglob

# Pass 1: validate
for file in *.LOG
do
  target=${file%.LOG}.log

  if [[ -e "$target" || -L "$target" ]]; then
    exit 2
  fi
done

# Pass 2: modify
for file in *.LOG
do
  target=${file%.LOG}.log
  mv -- "$file" "$target"
done
```

核心知识点：

```text
nullglob
    ↓
无匹配时循环执行 0 次

${file%.LOG}.log
    ↓
只替换末尾扩展名

-e || -L
    ↓
同时检测普通路径和失效符号链接

第一遍检查
    ↓
不修改任何东西

第二遍执行
    ↓
所有目标通过预检后才开始 mv

mv -- "$file" "$target"
    ↓
安全处理空格和以 - 开头的文件名
```

这是一种通用的批处理设计原则：

> **先验证整个批次，再执行修改；不要在验证尚未完成时逐个改变状态。**
