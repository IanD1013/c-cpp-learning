# Bash 变量赋值与参数展开

## 1. Bash 变量赋值：`=` 两边不能有空格

在 Bash 中，变量赋值的基本语法是：

```bash
name=value
```

例如：

```bash
project=atlas
```

这里：

* 变量名：`project`
* 变量值：`atlas`

### 错误写法

```bash
project = atlas
```

这并不会被 Bash 识别为变量赋值。

Bash 会先按照空白字符把命令行拆成多个单词：

```text
project
=
atlas
```

然后把第一个单词 `project` 当成要执行的命令，把 `=` 和 `atlas` 当作参数，因此通常会得到：

```text
bash: project: command not found
```

### 核心规则

变量赋值时：

```bash
变量名=值
```

`变量名`、`=` 和 `值` 必须连在一起。

正确：

```bash
project=atlas
```

错误：

```bash
project =atlas
project= atlas
project = atlas
```

---

## 2. 使用 `$变量名` 读取变量

变量赋值之后，可以通过参数展开（parameter expansion）读取变量的值：

```bash
project=atlas
printf '%s\n' "$project"
```

输出：

```text
atlas
```

其中：

```bash
"$project"
```

并不是把字符串 `$project` 直接交给 `printf`。

Bash 会在执行 `printf` **之前**进行参数展开：

```text
"$project"
    ↓
"atlas"
```

因此最终相当于执行：

```bash
printf '%s\n' "atlas"
```

`printf` 本身并不知道这个值来自 Bash 变量。

---

## 3. 为什么通常应该写成 `"$project"`

虽然下面两种写法对于简单值 `atlas` 都可能正常工作：

```bash
printf '%s\n' $project
```

```bash
printf '%s\n' "$project"
```

但推荐始终使用：

```bash
"$project"
```

原因是双引号可以保证展开后的内容仍然作为**一个参数**传递。

例如：

```bash
project="new atlas"
```

推荐：

```bash
printf '%s\n' "$project"
```

Bash 会把：

```text
new atlas
```

作为一个完整参数传给 `printf`。

而没有引号时：

```bash
printf '%s\n' $project
```

展开结果可能继续发生单词分割，相当于传入两个参数：

```text
new
atlas
```

因此，一个非常实用的 Bash 习惯是：

> 普通变量展开默认写成 `"$variable"`，除非明确需要 Bash 进行单词分割等特殊行为。

---

## 4. Shell 变量属于当前 Shell 进程

执行：

```bash
project=atlas
```

之后，变量 `project` 存储在**当前 Bash 进程**中。

因此，在同一个 Shell 会话中：

```bash
project=atlas
printf '%s\n' "$project"
```

第二条命令可以读取第一条命令设置的变量。

但是，新启动的脚本通常运行在另一个 Bash 进程中。例如：

```bash
project=atlas
bash script.sh
```

默认情况下，`script.sh` 中看不到普通 Shell 变量 `project`。

这是因为：

```text
当前 Bash
├── project=atlas
└── 启动新的 Bash 进程
    └── 默认不会继承普通 Shell 变量
```

如果希望子进程继承变量，需要把它导出为环境变量：

```bash
export project=atlas
```

或者：

```bash
project=atlas
export project
```

也可以只给某一条命令临时提供变量：

```bash
project=atlas bash script.sh
```

---

## 5. 使用 `printf` 输出变量

`printf` 是 Bash 中常用的格式化输出命令。

例如：

```bash
project=atlas
printf 'Project: %s\n' "$project"
```

输出：

```text
Project: atlas
```

格式字符串中的：

```text
%s
```

表示插入一个字符串。

而：

```text
\n
```

表示换行。

因此：

```bash
printf 'Project: %s\n' "$project"
```

可以理解为：

```text
Project: + project 的值 + 换行
```

---

## 6. 使用 `>` 把输出写入文件

Shell 的输出重定向语法：

```bash
command > file
```

表示把命令原本写到终端的标准输出（stdout）写入文件。

例如：

```bash
printf 'Project: %s\n' "$project" > project.txt
```

不会在终端显示：

```text
Project: atlas
```

而是把它写进：

```text
project.txt
```

文件内容为：

```text
Project: atlas
```

并且末尾包含一个换行符，因为格式字符串使用了：

```bash
\n
```

### `>` 会覆盖已有内容

如果 `project.txt` 已经存在：

```bash
printf 'Project: %s\n' "$project" > project.txt
```

会清空原内容，然后写入新的内容。

如果希望追加内容，应使用：

```bash
>>
```

例如：

```bash
printf 'Project: %s\n' "$project" >> project.txt
```

---

## 7. 完整示例

假设当前目录为：

```text
/home/learner/bash-lab
```

依次在**同一个 Bash 提示符**中执行：

```bash
project=atlas
printf 'Project: %s\n' "$project" > project.txt
```

执行流程：

```text
project=atlas
      │
      ▼
当前 Bash 保存变量
project → atlas
      │
      ▼
"$project"
      │ 参数展开
      ▼
"atlas"
      │
      ▼
printf 生成
Project: atlas\n
      │
      ▼
> project.txt
      │
      ▼
写入文件
/home/learner/bash-lab/project.txt
```

最终：

```text
/home/learner/bash-lab/project.txt
```

包含且只包含一行：

```text
Project: atlas
```

该行末尾还有一个换行符。

---

## 8. 为什么必须通过变量输出 `atlas`

下面的写法虽然最终文件看起来相同：

```bash
printf 'Project: atlas\n' > project.txt
```

但它没有真正练习变量。

练习要求的数据流应当是：

```text
atlas
  ↓
存入 project
  ↓
"$project" 参数展开
  ↓
printf
  ↓
project.txt
```

因此正确方式是：

```bash
project=atlas
printf 'Project: %s\n' "$project" > project.txt
```

而不是把 `atlas` 直接写死在格式字符串中。

---

## 9. 常见错误

| 写法                                                | 问题                                 |
| ------------------------------------------------- | ---------------------------------- |
| `project = atlas`                                 | `=` 两边有空格，Bash 会尝试执行 `project` 命令  |
| `project= atlas`                                  | `project=` 被当成临时变量赋值，`atlas` 被当成命令 |
| `project =atlas`                                  | `project` 被当成命令                    |
| `printf 'Project: atlas\n'`                       | 没有通过变量取得 `atlas`                   |
| `printf 'Project: %s\n' "$project"`               | 只输出到终端，没有写入文件                      |
| `printf 'Project: %s\n' '$project'`               | 单引号阻止变量展开，得到字面量 `$project`         |
| `printf 'Project: %s\n' "$project" > project.txt` | 正确                                 |

尤其要区分：

```bash
"$project"
```

和：

```bash
'$project'
```

双引号允许变量展开：

```bash
"$project"
# → atlas
```

单引号会保留字符本身：

```bash
'$project'
# → $project
```

---

## 10. 最终练习命令

```bash
cd /home/learner/bash-lab
project=atlas
printf 'Project: %s\n' "$project" > project.txt
```

可以检查结果：

```bash
cat project.txt
```

应输出：

```text
Project: atlas
```

## 核心记忆

```bash
# 赋值：等号两边没有空格
project=atlas

# 读取：通常给变量展开加双引号
"$project"

# 格式化输出
printf 'Project: %s\n' "$project"

# 写入文件
printf 'Project: %s\n' "$project" > project.txt
```

最重要的三个概念是：

1. Bash 变量赋值使用 `name=value`，`=` 两边不能有空格。
2. `$name` 会在命令执行前由 Bash 展开成变量值，通常应写成 `"$name"`。
3. 普通 Shell 变量属于当前 Shell；如果希望子进程继承，需要使用 `export`。
