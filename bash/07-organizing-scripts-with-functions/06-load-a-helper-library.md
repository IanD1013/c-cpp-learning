# Bash：用 `source` 从外部文件加载函数

## 1. 为什么把函数放进单独文件后会出现 `command not found`

假设有两个文件：

```text
/home/learner/bash-lab/
├── app.sh
└── lib.sh
```

`lib.sh` 中定义函数：

```bash
write_pair() {
    printf '%s=%s\n' "$1" "$2"
}
```

如果在 `app.sh` 中这样写：

```bash
bash lib.sh
write_pair "$1" "$2"
```

会出现类似错误：

```text
write_pair: command not found
```

原因是：

```bash
bash lib.sh
```

会启动一个**新的 Bash 子进程**。

执行过程相当于：

```text
当前 Bash
   │
   ├── 启动新的 Bash
   │      │
   │      ├── 读取 lib.sh
   │      ├── 定义 write_pair
   │      └── 退出
   │
   └── 尝试调用 write_pair
          ↓
      找不到函数
```

函数只存在于**读取并定义它的那个 Shell 进程中**。

因此：

> 用 `bash lib.sh` 执行库文件，并不能把函数“导入”当前脚本。

---

## 2. `source` 的作用

要让当前脚本使用另一个文件中的函数，应使用：

```bash
source ./lib.sh
```

也可以使用 Bash 的简写：

```bash
. ./lib.sh
```

`source` 不会创建新的 Bash 进程，而是：

> 在当前 Shell 中读取并执行指定文件的内容。

因此：

```bash
source ./lib.sh
write_pair "$1" "$2"
```

执行过程是：

```text
当前 Bash
   │
   ├── 读取 lib.sh
   │      └── 定义 write_pair
   │
   └── 调用 write_pair
          ↓
        成功
```

可以把它理解成 Bash 在运行到：

```bash
source ./lib.sh
```

时，把 `lib.sh` 中的代码加载到了当前 Shell 环境。

---

## 3. `bash file.sh` 和 `source file.sh` 的区别

| 写法              | 是否启动新 Shell | 函数是否保留在当前 Shell |
| --------------- | ----------: | --------------: |
| `bash lib.sh`   |           是 |               否 |
| `source lib.sh` |           否 |               是 |
| `. lib.sh`      |           否 |               是 |

例如：

```bash
# lib.sh
hello() {
    printf 'hello\n'
}
```

错误：

```bash
bash lib.sh
hello
```

因为 `hello` 定义在子 Shell 中，子 Shell 结束后定义也消失。

正确：

```bash
source ./lib.sh
hello
```

输出：

```text
hello
```

---

## 4. Bash 库文件应该只负责“定义”

一个作为函数库使用的文件通常只包含函数、变量等定义，而不应该在被加载时自动执行主要业务逻辑。

例如 `lib.sh`：

```bash
write_pair() {
    printf '%s=%s\n' "$1" "$2"
}
```

这里：

* `$1`：函数收到的第一个参数
* `$2`：函数收到的第二个参数
* `printf`：把两者连接成 `key=value`
* `\n`：最后输出换行

例如：

```bash
write_pair "region" "eu-west"
```

输出：

```text
region=eu-west
```

### 为什么不要在库文件中直接调用函数

不推荐：

```bash
write_pair() {
    printf '%s=%s\n' "$1" "$2"
}

write_pair "region" "eu-west"
```

因为：

```bash
source ./lib.sh
```

会直接执行整个文件，因此最后一行也会立即运行。

库文件更适合只提供可复用能力：

```bash
write_pair() {
    printf '%s=%s\n' "$1" "$2"
}
```

由调用者决定什么时候执行：

```bash
source ./lib.sh
write_pair "$1" "$2"
```

---

## 5. 完整示例

目录：

```text
/home/learner/bash-lab/
├── app.sh
└── lib.sh
```

### `lib.sh`

```bash
write_pair() {
    printf '%s=%s\n' "$1" "$2"
}
```

这个文件只定义函数，不执行其他操作。

### `app.sh`

```bash
#!/usr/bin/env bash

source ./lib.sh
write_pair "$1" "$2" > pair.txt
```

运行：

```bash
cd /home/learner/bash-lab
bash app.sh region eu-west
```

查看：

```bash
cat pair.txt
```

结果：

```text
region=eu-west
```

---

## 6. 函数参数和脚本参数之间的关系

执行：

```bash
bash app.sh region eu-west
```

对于 `app.sh`：

```text
$1 = region
$2 = eu-west
```

然后：

```bash
write_pair "$1" "$2"
```

等价于：

```bash
write_pair "region" "eu-west"
```

进入函数后，函数拥有自己的位置参数：

```bash
write_pair() {
    printf '%s=%s\n' "$1" "$2"
}
```

因此函数内部：

```text
$1 = region
$2 = eu-west
```

最终输出：

```text
region=eu-west
```

可以理解为：

```text
脚本参数
region eu-west
   │      │
   ▼      ▼
"$1"    "$2"
   │      │
   └──┬───┘
      ▼
write_pair "region" "eu-west"
      │
      ▼
函数自己的 $1 和 $2
      │
      ▼
region=eu-west
```

---

## 7. 为什么参数要写成 `"$1"` 和 `"$2"`

推荐：

```bash
write_pair "$1" "$2"
```

以及：

```bash
printf '%s=%s\n' "$1" "$2"
```

不要习惯性写成：

```bash
write_pair $1 $2
```

未加引号时，参数中的空格、通配符等可能被 Bash 再次拆分或展开。

例如：

```bash
bash app.sh "server region" "eu west"
```

使用正确的引用：

```bash
write_pair "$1" "$2"
```

函数收到的仍然是两个参数：

```text
$1 = server region
$2 = eu west
```

结果：

```text
server region=eu west
```

核心原则：

> Bash 中，只要希望变量展开后仍然保持为一个完整参数，通常就应该写成 `"$variable"`。

---

## 8. `source ./lib.sh` 中 `./` 的真正含义

这里最容易产生误解。

```bash
source ./lib.sh
```

其中：

```text
./
```

表示的是：

> 当前工作目录（current working directory）

它**不是**：

> `app.sh` 文件所在的目录

假设：

```text
/home/learner/bash-lab/app.sh
/home/learner/bash-lab/lib.sh
```

如果先进入：

```bash
cd /home/learner/bash-lab
```

然后运行：

```bash
bash app.sh region eu-west
```

此时：

```bash
source ./lib.sh
```

寻找的是：

```text
/home/learner/bash-lab/lib.sh
```

所以能够成功。

---

## 9. 从其他目录运行时为什么可能失败

假设当前位于：

```text
/home/learner
```

然后执行：

```bash
bash bash-lab/app.sh region eu-west
```

虽然执行的脚本位于：

```text
/home/learner/bash-lab/app.sh
```

但当前工作目录仍然是：

```text
/home/learner
```

因此：

```bash
source ./lib.sh
```

寻找的是：

```text
/home/learner/lib.sh
```

而不是：

```text
/home/learner/bash-lab/lib.sh
```

于是可能出现：

```text
./lib.sh: No such file or directory
```

之后：

```bash
write_pair "$1" "$2"
```

也会失败，因为函数根本没有被加载：

```text
write_pair: command not found
```

因此，这个简单版本的 `app.sh` 应当从项目目录运行：

```bash
cd /home/learner/bash-lab
bash app.sh region eu-west
```

---

## 10. 为什么推荐写 `source ./lib.sh`，而不是 `source lib.sh`

下面两种写法并不完全相同：

```bash
source lib.sh
```

```bash
source ./lib.sh
```

### `source lib.sh`

因为文件名中没有 `/`，Bash 会尝试根据 `PATH` 查找它。

因此，如果 `PATH` 中某个目录已经存在：

```text
lib.sh
```

就有可能加载到那个文件，而不是你当前目录中的文件。

在非 POSIX 模式下，如果 `PATH` 中没有找到，Bash 通常还会继续检查当前目录。

### `source ./lib.sh`

因为路径中已经包含 `/`：

```text
./lib.sh
```

Bash 不需要通过 `PATH` 搜索，而是直接读取当前目录中的：

```text
lib.sh
```

所以在明确想加载当前目录文件时：

```bash
source ./lib.sh
```

通常比：

```bash
source lib.sh
```

更加明确。

但仍然要记住：

> `./` 是当前工作目录，而不是脚本自身所在目录。

---

## 11. `$0` 与脚本自身路径

运行：

```bash
bash bash-lab/app.sh
```

时，`$0` 通常保存脚本被调用时使用的路径，例如：

```text
bash-lab/app.sh
```

因此，在更完善的 Bash 程序中，可以根据脚本自身的路径计算出它所在的目录，然后从那里加载库。

概念上是：

```text
$0
 ↓
找到 app.sh 所在目录
 ↓
加载那个目录中的 lib.sh
```

这样，无论用户当前站在哪个目录，都可以找到库文件。

不过：

```bash
source ./lib.sh
```

本身并不会自动完成这件事。

---

## 12. `>` 为什么会替换文件

调用：

```bash
write_pair "$1" "$2" > pair.txt
```

其中：

```bash
>
```

表示把标准输出写入文件，并覆盖原有内容。

第一次：

```bash
bash app.sh region eu-west
```

得到：

```text
region=eu-west
```

第二次：

```bash
bash app.sh zone west
```

`pair.txt` 会变成：

```text
zone=west
```

旧内容：

```text
region=eu-west
```

不会保留。

### `>` 与 `>>`

| 操作符  | 行为      |
| ---- | ------- |
| `>`  | 覆盖文件    |
| `>>` | 追加到文件末尾 |

这里要求每次运行只留下最新数据，所以应使用：

```bash
>
```

而不是：

```bash
>>
```

---

## 13. 重定向作用于函数的标准输出

这一行：

```bash
write_pair "$1" "$2" > pair.txt
```

可以理解为：

```text
write_pair
   │
   │ 标准输出
   ▼
region=eu-west
   │
   │ >
   ▼
pair.txt
```

函数本身只负责：

```bash
printf '%s=%s\n' "$1" "$2"
```

它并不知道数据最终是：

* 显示在终端；
* 写进文件；
* 被另一个命令读取；
* 还是被命令替换捕获。

例如，同一个函数可以这样使用：

```bash
write_pair "region" "eu-west"
```

输出到终端。

也可以：

```bash
write_pair "region" "eu-west" > pair.txt
```

写进文件。

这体现了一个重要的 Shell 设计原则：

> 函数负责产生数据，调用者决定数据去哪里。

---

## 14. 为什么不应该在 `app.sh` 中再次定义 `write_pair`

错误设计：

```bash
#!/usr/bin/env bash

source ./lib.sh

write_pair() {
    printf '%s=%s\n' "$1" "$2"
}

write_pair "$1" "$2" > pair.txt
```

虽然程序可能看起来能够工作，但这里实际上重新定义了：

```bash
write_pair
```

后面的定义会覆盖库中加载进来的函数。

结果是：

```text
lib.sh
  │
  └── 定义 write_pair A

app.sh
  │
  └── 又定义 write_pair B
          │
          ▼
       实际调用 B
```

此时即使修改：

```text
lib.sh
```

程序行为也可能完全不变。

这样就失去了库文件的意义。

正确结构应该是：

```bash
# lib.sh
write_pair() {
    printf '%s=%s\n' "$1" "$2"
}
```

```bash
# app.sh
#!/usr/bin/env bash

source ./lib.sh
write_pair "$1" "$2" > pair.txt
```

形成：

```text
lib.sh
  │
  └── 提供 write_pair
          │
          ▼
       source
          │
          ▼
       app.sh
          │
          ▼
       调用函数
```

---

## 15. Exit Status：为什么函数调用最好放在最后

Shell 中，每个命令都有一个退出状态：

```text
0     成功
非 0  失败
```

如果脚本没有显式执行：

```bash
exit ...
```

脚本最终通常会继承最后一个命令的退出状态。

因此：

```bash
#!/usr/bin/env bash

source ./lib.sh
write_pair "$1" "$2" > pair.txt
```

最后执行的是：

```bash
write_pair "$1" "$2" > pair.txt
```

正常情况下它返回：

```text
0
```

脚本最终也会以成功状态结束。

如果后面又添加一个会失败的命令：

```bash
write_pair "$1" "$2" > pair.txt
false
```

那么脚本最终的退出状态会变成：

```text
1
```

所以如果希望脚本自然以函数调用的成功状态结束，不要在后面随意添加其他命令。

---

## 16. 推荐的最终实现

### `/home/learner/bash-lab/lib.sh`

```bash
write_pair() {
    printf '%s=%s\n' "$1" "$2"
}
```

### `/home/learner/bash-lab/app.sh`

```bash
#!/usr/bin/env bash

source ./lib.sh
write_pair "$1" "$2" > pair.txt
```

运行：

```bash
cd /home/learner/bash-lab
bash app.sh region eu-west
```

结果：

```bash
cat pair.txt
```

```text
region=eu-west
```

再次运行：

```bash
bash app.sh environment production
```

此时：

```bash
cat pair.txt
```

结果只剩：

```text
environment=production
```

---

## 17. 核心知识点

* `bash lib.sh` 会创建新的 Bash 进程，函数定义不会自动回到调用者。
* `source ./lib.sh` 在**当前 Shell** 中读取文件，因此可以加载其中的函数。
* Bash 函数只属于定义它的 Shell 进程。
* 库文件应主要包含函数等定义，不应自动执行主要业务逻辑。
* `source ./lib.sh` 中的 `./` 指**当前工作目录**，不是脚本所在目录。
* `source lib.sh` 可能根据 `PATH` 查找文件；`source ./lib.sh` 明确指定当前目录。
* 调用函数时应写：

  ```bash
  write_pair "$1" "$2"
  ```

  以保持参数边界。
* `>` 会覆盖输出文件，`>>` 会追加。
* 函数负责产生标准输出，调用者通过重定向决定输出去哪里。
* 如果函数来自库文件，就不应在调用脚本中重复定义同名函数，否则会破坏真正的代码复用。
