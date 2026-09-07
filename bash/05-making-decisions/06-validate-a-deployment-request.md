# Bash 多条件输入校验：参数数量、字符串、正则表达式与文件检查

## 1. 核心思想：所有条件都通过，请求才有效

假设部署脚本接收三个参数：

```text
环境名 副本数量 manifest文件
```

例如：

```bash
./validate-deploy.sh prod 5 manifest.yml
```

一个合法请求必须同时满足：

1. 恰好有 3 个参数；
2. 环境只能是 `dev` 或 `prod`；
3. 副本数量只能是 `1`～`5`；
4. manifest 必须是一个实际存在的普通文件。

这是一组**逻辑与（AND）关系**：

```text
参数数量正确
AND 环境合法
AND 副本数量合法
AND manifest 是普通文件
```

因此：

> 某一个条件通过，并不能证明整个请求安全；所有校验都通过后才能继续执行。

最佳实践是采用 **fail fast（尽早失败）**：每检查一个条件，如果失败，立即输出 `invalid` 并以状态码 `2` 退出。

---

## 2. 第一关：检查参数数量 `$#`

Bash 中：

```bash
$#
```

表示当前脚本收到的**位置参数数量**。

例如：

```bash
./validate-deploy.sh dev 2 manifest.yml
```

此时：

```bash
$# == 3
```

而：

```bash
./validate-deploy.sh dev 2
```

得到：

```bash
$# == 2
```

如果多传一个参数：

```bash
./validate-deploy.sh dev 2 manifest.yml extra
```

则：

```bash
$# == 4
```

因此可以写：

```bash
[[ $# -eq 3 ]]
```

这里使用 `-eq`，因为比较的是整数。

完整 guard（守卫条件）：

```bash
[[ $# -eq 3 ]] || { printf 'invalid\n'; exit 2; }
```

意思是：

> 如果参数数量不是 3，则输出 `invalid` 并立即退出。

---

## 3. `-eq` 与 `==`：数字比较和字符串比较不能混淆

在 `[[ ... ]]` 中，两类比较用途不同。

### 数值比较

| 运算符   | 含义   |
| ----- | ---- |
| `-eq` | 等于   |
| `-ne` | 不等于  |
| `-lt` | 小于   |
| `-le` | 小于等于 |
| `-gt` | 大于   |
| `-ge` | 大于等于 |

例如：

```bash
[[ $# -eq 3 ]]
```

是在比较整数。

---

### 字符串比较

字符串通常使用：

```bash
==
```

例如：

```bash
[[ "$env_name" == "dev" ]]
```

表示：

> `env_name` 的文本内容必须等于 `dev`。

因此：

```bash
[[ 05 -eq 5 ]]
```

关注的是数值意义，而：

```bash
[[ "05" == "5" ]]
```

结果为假，因为两个字符串并不完全相同。

---

## 4. 通过参数数量检查后再读取 `$1`、`$2`、`$3`

确认确实存在三个参数后，再保存它们：

```bash
env_name=$1
replicas=$2
manifest=$3
```

对应关系：

```text
$1 → 环境名
$2 → 副本数量
$3 → manifest 路径
```

例如：

```bash
./validate-deploy.sh prod 5 manifest.yml
```

得到：

```bash
env_name=prod
replicas=5
manifest=manifest.yml
```

---

## 5. 校验环境名：在 `[[ ... ]]` 内使用 `||`

允许的环境只有：

```text
dev
prod
```

因此：

```bash
[[ "$env_name" == "dev" || "$env_name" == "prod" ]]
```

这里的：

```bash
||
```

位于 `[[ ... ]]` **内部**，表示逻辑 OR：

```text
环境是 dev
OR
环境是 prod
```

只要其中一个条件成立，整个测试就成立。

加入失败处理：

```bash
[[ "$env_name" == "dev" || "$env_name" == "prod" ]] ||
  { printf 'invalid\n'; exit 2; }
```

于是：

```text
dev   → 合法
prod  → 合法
qa    → 非法
test  → 非法
```

---

## 6. `||` 的两种不同作用

Bash 中的 `||` 很容易因为出现位置不同而混淆。

### 用法一：位于 `[[ ... ]]` 内部

```bash
[[ "$env_name" == "dev" || "$env_name" == "prod" ]]
```

这是**逻辑 OR**：

> 左右两个条件至少一个成立即可。

---

### 用法二：连接两个命令

```bash
[[ $# -eq 3 ]] || printf 'invalid\n'
```

这里的 `||` 是 Shell 的**命令控制运算符**：

> 只有左边命令失败时，才执行右边命令。

Shell 中通常：

```text
退出状态 0   → 成功
非 0         → 失败
```

因此：

```bash
test_command || error_command
```

可以理解为：

```text
如果 test_command 失败，就执行 error_command
```

---

## 7. 为什么要用 `{ ...; }` 把失败处理组合起来

失败时需要做两件事：

```bash
printf 'invalid\n'
exit 2
```

如果希望 `||` 控制这两个命令，应把它们组成一个 command group：

```bash
{ printf 'invalid\n'; exit 2; }
```

于是：

```bash
[[ -f "$manifest" ]] || { printf 'invalid\n'; exit 2; }
```

逻辑是：

```text
如果文件测试失败
    输出 invalid
    退出状态码 2
```

### 花括号语法注意事项

正确：

```bash
{ printf 'invalid\n'; exit 2; }
```

要注意：

* `{` 后必须有空格；
* `}` 前的最后一个命令必须用 `;` 或换行结束。

错误：

```bash
{printf 'invalid\n'; exit 2;}
```

---

## 8. 副本数量必须先进行文本校验

目标是只允许：

```text
1
2
3
4
5
```

可以使用正则表达式：

```bash
[[ "$replicas" =~ ^[1-5]$ ]]
```

正则：

```text
^[1-5]$
```

可以拆成：

| 部分      | 含义             |
| ------- | -------------- |
| `^`     | 字符串开头          |
| `[1-5]` | 一个 `1`～`5` 的数字 |
| `$`     | 字符串结尾          |

因此它要求：

> 整个字符串必须恰好由一个 `1`～`5` 的数字组成。

结果：

```text
1     → 合法
5     → 合法
0     → 非法
6     → 非法
15    → 非法
2+2   → 非法
abc   → 非法
```

完整检查：

```bash
[[ "$replicas" =~ ^[1-5]$ ]] ||
  { printf 'invalid\n'; exit 2; }
```

---

## 9. 为什么不能直接使用 `(( ))` 验证用户输入

下面这种写法看起来合理：

```bash
(( replicas >= 1 && replicas <= 5 ))
```

但单独使用它校验外部输入并不安全。

原因是：

> `(( ... ))` 会把内容当作 Bash 算术表达式，而不是单纯的数字文本。

例如用户传入：

```text
2+2
```

那么：

```bash
replicas='2+2'
```

执行：

```bash
(( replicas >= 1 && replicas <= 5 ))
```

时，Bash 会计算：

```text
2 + 2 = 4
```

于是恶意或错误输入：

```text
2+2
```

竟然会通过 `1`～`5` 的范围检查。

这不是我们想要的行为。

---

## 10. 正确顺序：先验证格式，再做算术

如果确实还想进行算术范围判断，应采用：

```bash
[[ "$replicas" =~ ^[1-5]$ ]] ||
  { printf 'invalid\n'; exit 2; }

(( replicas >= 1 && replicas <= 5 )) ||
  { printf 'invalid\n'; exit 2; }
```

关键原则：

```text
外部文本
↓
先验证格式
↓
确认确实是允许的数字文本
↓
再进行算术运算
```

这里第二个范围判断其实已经是冗余的，因为：

```bash
^[1-5]$
```

本身已经严格限制了范围。

因此本题直接使用：

```bash
[[ "$replicas" =~ ^[1-5]$ ]]
```

即可。

---

## 11. `&&` 也有不同的使用场景

类似 `||`，`&&` 也会根据位置表达不同层面的逻辑。

### 在算术表达式内部

```bash
(( replicas >= 1 && replicas <= 5 ))
```

这里表示：

```text
replicas >= 1
AND
replicas <= 5
```

两个算术条件都必须成立。

---

### 在命令之间

例如：

```bash
command1 && command2
```

表示：

> 只有 `command1` 成功，才执行 `command2`。

因此：

```bash
mkdir output && printf 'created\n'
```

只有目录创建成功时才打印：

```text
created
```

---

## 12. 使用 `-f` 检查普通文件

manifest 必须是一个已经存在的**普通文件**。

使用：

```bash
[[ -f "$manifest" ]]
```

例如：

```bash
[[ -f "manifest.yml" ]]
```

如果 `manifest.yml` 存在，并且是普通文件，则测试成功。

完整 guard：

```bash
[[ -f "$manifest" ]] ||
  { printf 'invalid\n'; exit 2; }
```

---

## 13. 为什么 `-f` 比“路径存在”更严格

Shell 提供多种文件测试。

| 测试   | 含义    |
| ---- | ----- |
| `-e` | 路径存在  |
| `-f` | 是普通文件 |
| `-d` | 是目录   |

假设：

```text
manifest-dir/
```

是一个目录。

那么：

```bash
[[ -e manifest-dir ]]
```

会成功，因为路径确实存在。

但：

```bash
[[ -f manifest-dir ]]
```

会失败，因为它不是普通文件。

如果业务要求：

> 这里必须提供 manifest 文件，目录不能代替。

就应该使用：

```bash
-f
```

而不是只使用：

```bash
-e
```

---

## 14. 完整脚本

`validate-deploy.sh`：

```bash
#!/usr/bin/env bash

[[ $# -eq 3 ]] || { printf 'invalid\n'; exit 2; }

env_name=$1
replicas=$2
manifest=$3

[[ "$env_name" == "dev" || "$env_name" == "prod" ]] ||
  { printf 'invalid\n'; exit 2; }

[[ "$replicas" =~ ^[1-5]$ ]] ||
  { printf 'invalid\n'; exit 2; }

[[ -f "$manifest" ]] ||
  { printf 'invalid\n'; exit 2; }

printf 'valid\n'
exit 0
```

它采用了典型的 guard clause 结构：

```text
检查参数数量
失败 → 立即退出

检查环境
失败 → 立即退出

检查副本数量
失败 → 立即退出

检查文件
失败 → 立即退出

全部成功 → valid
```

相比把所有条件塞进一个巨大的表达式，这种结构通常更容易阅读、调试和扩展。

---

## 15. 测试边界值与非法输入

假设当前目录存在：

```text
manifest.yml
```

并且：

```text
manifest-dir/
```

是一个目录。

需要测试以下情况。

### 合法请求

最小边界：

```bash
./validate-deploy.sh dev 1 manifest.yml
```

输出：

```text
valid
```

最大边界：

```bash
./validate-deploy.sh prod 5 manifest.yml
```

输出：

```text
valid
```

---

### 非法环境

```bash
./validate-deploy.sh qa 3 manifest.yml
```

输出：

```text
invalid
```

---

### 副本数量低于下限

```bash
./validate-deploy.sh dev 0 manifest.yml
```

输出：

```text
invalid
```

---

### 副本数量超过上限

```bash
./validate-deploy.sh dev 6 manifest.yml
```

输出：

```text
invalid
```

---

### 拒绝算术表达式形式的输入

```bash
./validate-deploy.sh dev '2+2' manifest.yml
```

输出：

```text
invalid
```

这验证了脚本是在检查**用户输入的文本格式**，而不是先把它当算术表达式计算。

---

### 拒绝目录

```bash
./validate-deploy.sh dev 2 manifest-dir
```

输出：

```text
invalid
```

---

### 拒绝不存在的文件

```bash
./validate-deploy.sh dev 2 missing.yml
```

输出：

```text
invalid
```

---

### 拒绝参数过少

```bash
./validate-deploy.sh dev 2
```

输出：

```text
invalid
```

---

### 拒绝参数过多

```bash
./validate-deploy.sh dev 2 manifest.yml extra
```

输出：

```text
invalid
```

---

## 16. 将十次测试统一写入文件

### 方法一：第一次覆盖，之后追加

第一次：

```bash
./validate-deploy.sh dev 1 manifest.yml > validation-results.txt
```

后续：

```bash
./validate-deploy.sh prod 5 manifest.yml >> validation-results.txt
./validate-deploy.sh qa 3 manifest.yml >> validation-results.txt
./validate-deploy.sh dev 0 manifest.yml >> validation-results.txt
./validate-deploy.sh dev 6 manifest.yml >> validation-results.txt
./validate-deploy.sh dev '2+2' manifest.yml >> validation-results.txt
./validate-deploy.sh dev 2 manifest-dir >> validation-results.txt
./validate-deploy.sh dev 2 missing.yml >> validation-results.txt
./validate-deploy.sh dev 2 >> validation-results.txt
./validate-deploy.sh dev 2 manifest.yml extra >> validation-results.txt
```

其中：

```bash
>
```

表示覆盖文件，而：

```bash
>>
```

表示追加到文件末尾。

---

## 17. 更简洁的方法：命令组统一重定向

可以把十个命令放在一个 command group 中：

```bash
{
  ./validate-deploy.sh dev 1 manifest.yml
  ./validate-deploy.sh prod 5 manifest.yml
  ./validate-deploy.sh qa 3 manifest.yml
  ./validate-deploy.sh dev 0 manifest.yml
  ./validate-deploy.sh dev 6 manifest.yml
  ./validate-deploy.sh dev '2+2' manifest.yml
  ./validate-deploy.sh dev 2 manifest-dir
  ./validate-deploy.sh dev 2 missing.yml
  ./validate-deploy.sh dev 2
  ./validate-deploy.sh dev 2 manifest.yml extra
} > validation-results.txt
```

这里：

```bash
{
  ...
}
```

把十次运行组合成一个整体，然后：

```bash
> validation-results.txt
```

一次性重定向整个命令组的标准输出。

结果应为：

```text
valid
valid
invalid
invalid
invalid
invalid
invalid
invalid
invalid
invalid
```

---

## 18. 这类输入校验的推荐模式

对于来自用户、命令行、环境变量等外部来源的数据，推荐按照下面的顺序处理：

```text
1. 检查参数数量
2. 保存参数
3. 验证字符串允许值
4. 验证数字的文本格式
5. 必要时再进行算术运算
6. 验证文件系统对象的实际类型
7. 所有检查通过后才执行真正操作
```

例如：

```bash
[[ $# -eq 3 ]] || fail

[[ "$env_name" == "dev" || "$env_name" == "prod" ]] || fail

[[ "$replicas" =~ ^[1-5]$ ]] || fail

[[ -f "$manifest" ]] || fail
```

核心原则是：

> **先证明输入符合预期，再使用输入。不要依赖后续运算顺便帮你验证数据。**

---

## 19. 知识点总结

* `$#` 表示位置参数数量，可用 `[[ $# -eq 3 ]]` 检查参数个数。
* `-eq`、`-lt`、`-gt` 等用于数值比较；`==` 用于字符串比较。
* `[[ A || B ]]` 中的 `||` 表示逻辑 OR。
* `command1 || command2` 中的 `||` 表示左边失败后执行右边。
* `{ command1; command2; }` 可以把多个命令组合成一个整体。
* `=~` 可用于在进行算术之前验证用户输入的文本格式。
* `(( ... ))` 会解析算术表达式，因此不能单独依赖它验证“不受信任的数字文本”。
* `[[ -f "$path" ]]` 要求路径存在且是普通文件，可以拒绝目录。
* 多个独立校验应采用 fail-fast：任何一个规则失败都立即停止。
* 测试输入时应同时覆盖**合法边界值、非法边界值、错误类型、缺失参数和多余参数**。
