# Bash 使用 `sed` 安全脱敏配置文件

## 1. 场景

假设 `~/app.env` 中保存了配置：

```text
MODE=course
TOKEN=abc123-secret-value
PORT=8080
```

需要生成一个可以安全交付的副本：

```text
MODE=course
TOKEN=[redacted]
PORT=8080
```

同时要求：

* 原始文件 `~/app.env` 完全不变；
* 只替换 `TOKEN=` 这一行；
* 其他配置原样保留；
* 使用可重复执行的命令，而不是手动编辑。

这种任务非常适合使用 `sed`。

---

## 2. `sed` 的基本工作方式

`sed` 是一个**流编辑器（stream editor）**。

它通常按照下面的方式工作：

```text
输入文件
   │
   ▼
 sed 按规则处理每一行
   │
   ▼
stdout
```

例如：

```bash
sed 's/foo/bar/' file
```

表示：

> 把匹配到的 `foo` 替换成 `bar`，然后把处理后的内容输出到标准输出。

默认情况下：

```bash
sed '...' file
```

**不会修改 `file` 本身**。

这正适合制作脱敏副本，因为可以：

```text
原文件 → sed 脱敏 → 新文件
```

而不是直接修改包含真实凭据的源文件。

---

## 3. `sed` 替换语法

最常见的替换命令是：

```bash
s/pattern/replacement/
```

其中：

* `s`：substitute，替换
* `pattern`：要匹配的模式
* `replacement`：替换后的内容

例如：

```bash
sed 's/cat/dog/' file
```

会把每行中第一个匹配到的：

```text
cat
```

替换成：

```text
dog
```

---

## 4. 匹配整个 `TOKEN=` 配置项

为了把：

```text
TOKEN=abc123-secret-value
```

整体替换成：

```text
TOKEN=[redacted]
```

可以使用模式：

```regex
^TOKEN=.*
```

完整替换规则：

```bash
sed 's/^TOKEN=.*/TOKEN=[redacted]/'
```

---

## 5. 正则表达式拆解

### `^`：行首锚点

```regex
^TOKEN=
```

表示：

> `TOKEN=` 必须从这一行的开头出现。

因此它会匹配：

```text
TOKEN=abc123
```

但不会匹配：

```text
BACKUP_TOKEN=abc123
```

也不会匹配：

```text
COMMENT=old TOKEN=abc123
```

这个限制非常重要，否则可能错误修改其他内容。

---

### `.*`：匹配后面的全部内容

在基本正则表达式中：

```text
.
```

表示任意单个字符。

```text
*
```

表示前一个模式重复零次或多次。

所以：

```regex
.*
```

通常可以理解为：

> 匹配这一行剩余的所有字符。

因此：

```regex
^TOKEN=.*
```

可以匹配整行：

```text
TOKEN=abc123-secret-value
```

而不仅仅是其中的凭据。

---

## 6. 为什么替换整行更安全

一种思路是只匹配等号后面的内容：

```text
TOKEN=<secret>
```

但这里直接匹配：

```regex
^TOKEN=.*
```

再整体替换成：

```text
TOKEN=[redacted]
```

更简单：

```bash
sed 's/^TOKEN=.*/TOKEN=[redacted]/'
```

处理过程相当于：

```text
TOKEN=abc123-secret-value
        │
        ▼
匹配整行
        │
        ▼
TOKEN=[redacted]
```

这样可以确保旧凭据不会残留在输出中。

---

## 7. 为什么 `^` 很重要

如果写成：

```bash
sed 's/TOKEN=.*/TOKEN=[redacted]/'
```

那么下面这种内容也可能被匹配：

```text
COMMENT=previous TOKEN=abc123
```

因为：

```regex
TOKEN=.*
```

可以从一行中间开始匹配。

结果可能变成：

```text
COMMENT=previous TOKEN=[redacted]
```

这说明模式的匹配范围比真正需要的范围更大。

使用：

```regex
^TOKEN=.*
```

则明确表达：

> 只处理以 `TOKEN=` 开头的配置行。

---

## 8. 用重定向创建脱敏副本

`sed` 默认把结果打印到 stdout，因此可以使用：

```bash
>
```

把结果保存到另一个文件：

```bash
sed 's/^TOKEN=.*/TOKEN=[redacted]/' ~/app.env > /home/learner/redacted.env
```

数据流为：

```text
~/app.env
   │
   ▼
sed 's/^TOKEN=.*/TOKEN=[redacted]/'
   │
   │ stdout
   ▼
> /home/learner/redacted.env
```

原始文件不会被修改。

---

## 9. 输入与输出示例

### 原始文件

```text
MODE=course
TOKEN=abc123-secret-value
PORT=8080
```

执行：

```bash
sed 's/^TOKEN=.*/TOKEN=[redacted]/' ~/app.env
```

stdout：

```text
MODE=course
TOKEN=[redacted]
PORT=8080
```

只有匹配的行发生变化，其他行直接通过。

如果再加重定向：

```bash
sed 's/^TOKEN=.*/TOKEN=[redacted]/' ~/app.env > /home/learner/redacted.env
```

则处理后的内容会进入新文件。

---

## 10. `sed` 默认不会修改源文件

下面的命令：

```bash
sed 's/^TOKEN=.*/TOKEN=[redacted]/' ~/app.env
```

只是：

1. 读取 `~/app.env`
2. 处理内容
3. 输出处理结果

不会把结果写回原文件。

这与带有 `-i` 的写法不同：

```bash
sed -i 's/^TOKEN=.*/TOKEN=[redacted]/' ~/app.env
```

`-i` 表示 **in-place editing（原地修改）**。

对于需要保留真实凭据的源文件，这通常不是本任务想要的行为。

因此这里应该使用：

```bash
sed ... ~/app.env > new-file
```

而不是：

```bash
sed -i ...
```

---

## 11. 一个重要的重定向陷阱

不要把输入和输出写成同一个文件：

```bash
sed 's/^TOKEN=.*/TOKEN=[redacted]/' ~/app.env > ~/app.env
```

这是危险的。

Shell 会在 `sed` 开始读取之前先处理：

```bash
> ~/app.env
```

这可能立即把原文件截断为空文件。

正确方式是输出到另一个文件：

```bash
sed 's/^TOKEN=.*/TOKEN=[redacted]/' ~/app.env > /home/learner/redacted.env
```

---

## 12. `sed`、`grep` 和 `tr` 的职责区别

| 工具     | 主要用途         |
| ------ | ------------ |
| `grep` | 找出匹配的行       |
| `sed`  | 匹配并修改文本      |
| `tr`   | 一个字符转换成另一个字符 |

例如：

```bash
grep '^TOKEN=' ~/app.env
```

可以找到 TOKEN 行，但不会修改它。

```bash
tr ':' '\t'
```

适合字符转换，但并不知道什么是 `TOKEN` 配置项。

而：

```bash
sed 's/^TOKEN=.*/TOKEN=[redacted]/'
```

同时完成：

```text
找到目标文本 → 替换目标文本
```

因此最适合这种配置文件脱敏任务。

---

## 13. 核心记忆

`sed` 最常见的替换结构：

```bash
sed 's/pattern/replacement/' file
```

对于 `.env` 中类似：

```text
TOKEN=secret
```

的配置，安全脱敏可以使用：

```bash
sed 's/^TOKEN=.*/TOKEN=[redacted]/' file
```

关键点：

```text
^       → 必须从行首开始
TOKEN=  → 精确定位配置项
.*      → 匹配后面的整个值
```

如果要保留源文件并创建新副本：

```bash
sed '...' source > destination
```

---

# Bash 实战：生成脱敏后的 `redacted.env`

## 任务

源文件：

```text
~/app.env
```

包含：

```text
MODE=course
TOKEN=<真实凭据>
PORT=8080
```

需要创建：

```text
/home/learner/redacted.env
```

内容必须为：

```text
MODE=course
TOKEN=[redacted]
PORT=8080
```

同时不能修改原始 `~/app.env`。

## 正确命令

```bash
sed 's/^TOKEN=.*/TOKEN=[redacted]/' ~/app.env > /home/learner/redacted.env
```

## 命令拆解

```bash
sed
```

使用流编辑器处理文本。

```bash
's/^TOKEN=.*/TOKEN=[redacted]/'
```

表示：

```text
s/                开始替换
^TOKEN=.*         匹配整条 TOKEN 配置
/                 分隔 pattern 和 replacement
TOKEN=[redacted]  新内容
/                 结束替换
```

```bash
~/app.env
```

读取原始配置文件。

```bash
> /home/learner/redacted.env
```

把处理后的 stdout 写入新的脱敏文件。

最终完整命令：

```bash
sed 's/^TOKEN=.*/TOKEN=[redacted]/' ~/app.env > /home/learner/redacted.env
```

原始文件保持不变，只有新生成的副本中的凭据被替换为 `[redacted]`。
