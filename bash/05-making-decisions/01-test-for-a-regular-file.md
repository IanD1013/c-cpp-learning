# Bash 文件类型判断：`[[ -f PATH ]]`、`if` 与命令分组

## 1. 为什么“路径存在”还不够

在 Bash 脚本中，一个路径可能代表不同类型的对象，例如：

* 普通文件（regular file）
* 目录（directory）
* 符号链接
* 设备文件
* 不存在的路径

例如：

```text
item.txt   # 普通文件
folder     # 目录
missing    # 不存在
```

仅根据名字无法判断类型：

* `folder` 没有扩展名，并不意味着它一定是目录；
* `item.txt` 有 `.txt`，也不意味着它一定是普通文件。

Linux 中文件名只是名字，**真正的类型必须通过文件系统检查**。

---

## 2. Bash 的 `[[ ... ]]` 条件判断

Bash 提供：

```bash
[[ 条件 ]]
```

用于执行条件测试。

例如：

```bash
[[ -f "$1" ]]
```

含义是：

> 判断 `$1` 指向的路径是否为普通文件。

### `[[ ... ]]` 返回的是退出状态

`[[ ... ]]` 不是返回 `"true"` 或 `"false"` 字符串，而是像普通命令一样产生 **exit status（退出状态）**：

| 结果    | Exit Status |
| ----- | ----------: |
| 条件成立  |         `0` |
| 条件不成立 |       非 `0` |

在 Shell 中：

```text
0      → 成功 / true
非 0   → 失败 / false
```

因此可以直接与 `if` 配合。

---

## 3. `-f`：判断普通文件

语法：

```bash
[[ -f PATH ]]
```

`-f` 只有在 `PATH` 存在并且是**普通文件**时才成立。

例如：

```bash
[[ -f "item.txt" ]]
```

如果 `item.txt` 是普通文件：

```text
true
```

而：

```bash
[[ -f "folder" ]]
```

即使 `folder` 确实存在，只要它是目录，结果仍然是假。

同样：

```bash
[[ -f "missing" ]]
```

如果路径不存在，也是假。

因此：

| 路径         | 实际情况 | `[[ -f PATH ]]` |
| ---------- | ---- | --------------- |
| `item.txt` | 普通文件 | true            |
| `folder`   | 目录   | false           |
| `missing`  | 不存在  | false           |

这正适合“普通文件”和“其他所有情况”之间的分类。

---

## 4. 使用 `if` 根据测试结果执行不同分支

基本结构：

```bash
if [[ -f "$1" ]]; then
  ...
else
  ...
fi
```

执行过程：

```text
[[ -f "$1" ]]
      │
      ├── exit status = 0 ──→ then
      │
      └── exit status != 0 ─→ else
```

例如：

```bash
if [[ -f "$1" ]]; then
  echo "file"
else
  echo "not-file"
fi
```

如果运行：

```bash
bash inspect.sh item.txt
```

并且 `item.txt` 是普通文件：

```text
file
```

如果运行：

```bash
bash inspect.sh folder
```

而 `folder` 是目录：

```text
not-file
```

---

## 5. `$1`：脚本的第一个参数

Bash 脚本中：

```bash
$1
```

表示执行脚本时传入的**第一个位置参数**。

例如：

```bash
bash inspect.sh item.txt
```

那么脚本中：

```bash
$1
```

就是：

```text
item.txt
```

运行：

```bash
bash inspect.sh folder
```

则 `$1` 是：

```text
folder
```

因此：

```bash
[[ -f "$1" ]]
```

表示：

> 判断调用者传给脚本的第一个路径是不是普通文件。

---

## 6. 为什么通常写成 `"$1"`

推荐：

```bash
"$1"
```

而不是：

```bash
$1
```

例如路径可能包含空格：

```text
my file.txt
```

在很多 Shell 场景下，未加引号的变量可能发生：

* word splitting（单词拆分）
* glob expansion（通配符展开）

所以通常应养成习惯：

```bash
"$1"
```

在 Bash 的 `[[ ... ]]` 内部，变量不会像普通命令参数那样发生传统的单词拆分，因此：

```bash
[[ -f $1 ]]
```

通常也能工作。

但是：

```bash
[[ -f "$1" ]]
```

仍然更加清晰、安全，也更容易与其他 Shell 代码保持一致。

---

## 7. `[[` 和 `]]` 需要空格

正确：

```bash
[[ -f "$1" ]]
```

错误：

```bash
[[-f "$1"]]
```

`[[`、`-f`、路径和 `]]` 都是 Bash 语法中的独立部分，因此需要正确的空格。

同样：

```bash
if [[ -f "$1" ]]; then
```

其中的：

```bash
;
```

用于结束条件命令，然后进入 `then`。

也可以写成多行：

```bash
if [[ -f "$1" ]]
then
  echo "file"
fi
```

---

## 8. 常见文件测试操作符

除了 `-f`，Bash 还有许多文件测试。

| 测试        | 含义      |
| --------- | ------- |
| `-f PATH` | 是普通文件   |
| `-d PATH` | 是目录     |
| `-e PATH` | 路径存在    |
| `-r PATH` | 当前用户可读  |
| `-w PATH` | 当前用户可写  |
| `-x PATH` | 当前用户可执行 |
| `-L PATH` | 是符号链接   |

例如：

```bash
if [[ -d "$1" ]]; then
  echo "directory"
fi
```

### `-e` 和 `-f` 的区别

假设：

```text
item.txt   普通文件
folder     目录
missing    不存在
```

则：

| 路径         | `-e`  | `-f`  |
| ---------- | ----- | ----- |
| `item.txt` | true  | true  |
| `folder`   | true  | false |
| `missing`  | false | false |

因此，如果问题是：

> “这个路径存在吗？”

使用：

```bash
-e
```

如果问题是：

> “这个路径是不是普通文件？”

使用：

```bash
-f
```

---

## 9. `echo` 与脚本退出状态

脚本可以输出：

```bash
echo "file"
```

或：

```bash
echo "not-file"
```

这里需要区分两个概念：

```text
标准输出 ≠ 退出状态
```

例如：

```bash
echo "not-file"
```

会：

1. 打印：

   ```text
   not-file
   ```

2. 正常执行成功，通常返回退出状态：

   ```text
   0
   ```

因此，“不是文件”完全可以作为一种**正常的分类结果**，而不是脚本错误。

例如脚本：

```bash
if [[ -f "$1" ]]; then
  echo "file"
else
  echo "not-file"
fi
```

无论进入哪个分支，最后执行的 `echo` 通常都成功，因此整个脚本最终返回：

```text
0
```

---

## 10. 使用 `{ ...; }` 对多个命令进行分组

有时需要让多个命令共享同一个重定向。

例如：

```bash
{
  bash inspect.sh item.txt
  bash inspect.sh folder
  bash inspect.sh missing
} > inspect-results.txt
```

也可以写成一行：

```bash
{ bash inspect.sh item.txt; bash inspect.sh folder; bash inspect.sh missing; } > inspect-results.txt
```

这里：

```bash
{ ...; }
```

把多个命令组合成一个 command group（命令组）。

然后：

```bash
> inspect-results.txt
```

会把整个命令组的标准输出写入同一个文件。

最终效果等价于依次收集三个脚本的输出。

---

## 11. `{ ...; }` 的语法注意事项

### `{` 后面需要空格

正确：

```bash
{ echo one; echo two; }
```

不要写成：

```bash
{echo one; echo two;}
```

因为 `{` 在 Bash 中是一个保留字，需要与后面的命令分开。

### 最后一个命令后需要 `;` 或换行

一行写法：

```bash
{ echo one; echo two; }
```

注意：

```text
echo two;
```

后面必须有 `;`。

多行写法则可以：

```bash
{
  echo one
  echo two
}
```

这里换行本身就负责结束命令。

---

## 12. 重定向整个命令组

以下写法：

```bash
{ command1; command2; command3; } > result.txt
```

表示：

```text
command1 stdout ─┐
command2 stdout ─┼──→ result.txt
command3 stdout ─┘
```

因此不需要分别写：

```bash
command1 > result.txt
command2 >> result.txt
command3 >> result.txt
```

命令分组在需要多个命令共享以下操作时非常实用：

* 输出重定向
* 错误重定向
* 管道
* 条件控制

---

## 13. 核心知识总结

```bash
[[ -f "$1" ]]
```

用于判断脚本第一个参数是否指向普通文件。

关键点：

* `[[ ... ]]` 通过退出状态表达真假；
* `0` 表示条件成立；
* `-f` 只匹配普通文件；
* 目录即使存在，`-f` 仍然是假；
* 不存在的路径，`-f` 也是假；
* `$1` 是脚本的第一个参数；
* 路径变量建议写成 `"$1"`；
* `if ... then ... else ... fi` 根据条件退出状态选择分支；
* `{ ...; }` 可以把多个命令组成一个整体；
* `> file` 可以一次重定向整个命令组的输出。

---

# 练习解答：编写 `inspect.sh` 判断普通文件

## 问题

在目录：

```text
/home/learner/bash-lab
```

中编写：

```text
inspect.sh
```

要求：

* 第一个参数是需要检查的路径；

* 如果路径是普通文件，输出：

  ```text
  file
  ```

* 如果是目录或不存在，输出：

  ```text
  not-file
  ```

* 两种结果都属于正常执行，脚本退出状态应为 `0`；

* 分别检查：

  ```text
  item.txt
  folder
  missing
  ```

* 将三个结果依次保存到：

  ```text
  inspect-results.txt
  ```

---

## `inspect.sh`

```bash
#!/bin/bash

if [[ -f "$1" ]]; then
  echo "file"
else
  echo "not-file"
fi
```

保存到：

```text
/home/learner/bash-lab/inspect.sh
```

---

## 运行三个测试

进入目录：

```bash
cd /home/learner/bash-lab
```

执行：

```bash
{ bash inspect.sh item.txt; bash inspect.sh folder; bash inspect.sh missing; } > inspect-results.txt
```

---

## `inspect-results.txt` 的正确内容

```text
file
not-file
not-file
```

对应关系：

| 输入         | 类型   | 输出         |
| ---------- | ---- | ---------- |
| `item.txt` | 普通文件 | `file`     |
| `folder`   | 目录   | `not-file` |
| `missing`  | 不存在  | `not-file` |

---

## 验证结果

查看输出文件：

```bash
cat inspect-results.txt
```

应得到：

```text
file
not-file
not-file
```

还可以验证脚本退出状态：

```bash
bash inspect.sh folder
echo $?
```

应得到：

```text
not-file
0
```

这里的 `not-file` 是分类结果，而 `0` 表示脚本本身执行成功。
