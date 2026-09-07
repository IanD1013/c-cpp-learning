# Bash 中使用 `[[ ... ]]` 精确匹配字符串

## 核心目标

编写一个 Bash 脚本，根据第一个命令行参数判断环境名称：

* 参数**恰好等于** `prod` → 输出 `protected`
* 其他所有值 → 输出 `open`
* `production`、`prod-eu` 等虽然以 `prod` 开头，但都**不能**视为 `prod`

例如：

```text
prod        → protected
production  → open
prod-eu     → open
```

关键在于：这里需要的是**精确字符串匹配**，而不是前缀匹配。

---

## 1. `[[ ... ]]` 中的字符串比较

Bash 的 `[[ ... ]]` 是条件判断语法。

基本形式：

```bash
if [[ "$1" == "prod" ]]; then
  echo "protected"
else
  echo "open"
fi
```

其中：

```bash
"$1"
```

表示脚本收到的第一个位置参数。

例如：

```bash
./environment.sh prod
```

此时：

```bash
$1
```

的值就是：

```text
prod
```

---

## 2. `==` 默认是字符串比较

在：

```bash
[[ "$1" == "prod" ]]
```

中，`==` 比较的是字符串内容。

也就是说，字符必须完全一致：

```bash
[[ "prod" == "prod" ]]       # true
[[ "production" == "prod" ]] # false
[[ "prod-eu" == "prod" ]]    # false
```

字符串比较不会自动转换成数字。

例如：

```bash
[[ "08" == "8" ]]
```

结果为：

```text
false
```

因为 `"08"` 和 `"8"` 是两个不同的字符串。

---

## 3. `[[ ... ]]` 中 `==` 右侧是否加引号很重要

Bash 的 `[[ ... ]]` 有一个容易忽略的特性：

如果 `==` 右侧**没有加引号**，它可能被解释为模式（pattern）。

例如：

```bash
[[ "$1" == prod* ]]
```

这里：

```text
prod*
```

不是普通字符串，而是一个模式。

`*` 表示：

> 匹配任意数量的任意字符。

因此下面这些都会匹配成功：

```text
prod
production
prod-eu
prod123
product
```

例如：

```bash
[[ "production" == prod* ]]
```

结果为：

```text
true
```

如果目的是只保护生产环境名称 `prod`，这样的写法就过于宽泛。

---

## 4. 给右侧加引号可以强制精确匹配

写成：

```bash
[[ "$1" == "prod" ]]
```

右侧 `"prod"` 会被解释为普通字符串，而不是模式。

因此：

```bash
[[ "prod" == "prod" ]]        # true
[[ "production" == "prod" ]]  # false
[[ "prod-eu" == "prod" ]]     # false
```

这正是“环境名称必须**恰好是 `prod`**”所需要的行为。

### 对比

| 条件                     | 含义                  |
| ---------------------- | ------------------- |
| `[[ "$1" == "prod" ]]` | 精确匹配 `prod`         |
| `[[ "$1" == prod* ]]`  | 匹配所有以 `prod` 开头的字符串 |

对于安全保护逻辑，通常应优先使用明确的精确匹配，避免无意中扩大匹配范围。

---

## 5. `*`、`?` 和 `[...]` 是模式通配符

当 `[[ ... ]]` 中 `==` 的右侧没有被引用时，可以使用 Bash 模式匹配。

### `*`

匹配任意数量字符：

```bash
[[ "production" == prod* ]]
```

匹配成功。

---

### `?`

匹配任意**一个**字符：

```bash
[[ "prod1" == prod? ]]
```

匹配成功。

但：

```bash
[[ "prod12" == prod? ]]
```

匹配失败，因为 `?` 只能代表一个字符。

---

### `[...]`

匹配指定集合中的一个字符：

```bash
[[ "prod1" == prod[123] ]]
```

匹配成功。

```bash
[[ "prod4" == prod[123] ]]
```

匹配失败。

---

## 6. 为什么左侧也建议写成 `"$1"`

推荐写法：

```bash
[[ "$1" == "prod" ]]
```

而不是：

```bash
[[ $1 == "prod" ]]
```

但这里有一个重要细节：

在 `[[ ... ]]` 内部，左侧的 `$1` 即使没有引号，通常也不会发生传统 shell 中的：

* word splitting（单词分割）
* pathname expansion / glob expansion（路径名通配展开）

例如：

```bash
./environment.sh "a b c"
```

即使写：

```bash
[[ $1 == "prod" ]]
```

`$1` 仍然会作为完整字符串：

```text
a b c
```

参与比较，而不会拆成三个词。

因此在 `[[ ... ]]` 内：

```bash
[[ $1 == "prod" ]]
```

通常也是安全的。

---

## 7. 为什么仍然建议养成给变量加引号的习惯

虽然 `[[ ... ]]` 对未加引号的变量展开比较安全，但这种特殊规则主要属于 Bash 的 `[[ ... ]]`。

在其他命令中，未加引号的变量可能发生单词分割。

例如：

```bash
value="a b c"

printf '%s\n' $value
```

shell 可能把它拆成：

```text
a
b
c
```

而：

```bash
printf '%s\n' "$value"
```

会保留为完整的一个参数：

```text
a b c
```

因此更推荐统一写成：

```bash
[[ "$1" == "prod" ]]
```

这样既清晰，也减少把相同习惯带到其他 shell 场景时产生错误的可能。

---

## 8. 完整脚本

文件：

```text
bash-lab/environment.sh
```

内容：

```bash
#!/usr/bin/env bash

if [[ "$1" == "prod" ]]; then
  echo "protected"
else
  echo "open"
fi
```

逻辑是：

```text
读取 $1
   │
   ▼
是否等于 "prod"？
   │
   ├── 是 → protected
   │
   └── 否 → open
```

`else` 会覆盖所有非 `prod` 的情况，包括：

```text
production
prod-eu
dev
test
staging
空字符串
```

---

## 9. 运行脚本

如果需要先添加执行权限：

```bash
chmod +x bash-lab/environment.sh
```

运行：

```bash
./bash-lab/environment.sh prod
```

输出：

```text
protected
```

再运行：

```bash
./bash-lab/environment.sh production
```

输出：

```text
open
```

---

## 10. 将两次运行结果保存到文件

目标文件：

```text
bash-lab/environment-results.txt
```

可以执行：

```bash
./bash-lab/environment.sh prod > bash-lab/environment-results.txt
./bash-lab/environment.sh production >> bash-lab/environment-results.txt
```

这里：

```bash
>
```

表示创建或覆盖文件。

而：

```bash
>>
```

表示追加到文件末尾。

最终：

```text
bash-lab/environment-results.txt
```

内容应为：

```text
protected
open
```

也可以在 `bash-lab` 目录内执行：

```bash
./environment.sh prod > environment-results.txt
./environment.sh production >> environment-results.txt
```

---

## 11. 为什么脚本的退出状态是 `0`

Bash 脚本默认以最后执行命令的退出状态作为脚本退出状态。

两个分支最终执行的都是：

```bash
echo ...
```

正常情况下 `echo` 成功后退出状态为：

```text
0
```

因此无论执行：

```bash
./environment.sh prod
```

还是：

```bash
./environment.sh production
```

脚本都会正常结束，并返回：

```text
0
```

可以检查：

```bash
./environment.sh prod
echo "$?"
```

输出：

```text
protected
0
```

---

## 12. 常见错误

### 错误：使用前缀模式

```bash
if [[ "$1" == prod* ]]; then
  echo "protected"
else
  echo "open"
fi
```

这会把：

```text
production
prod-eu
product
```

等值全部识别为受保护环境。

如果要求仅保护 `prod`，这是错误的。

---

### 错误：把“包含 prod”理解为“等于 prod”

下面三个字符串完全不同：

```text
prod
production
prod-eu
```

要求：

```text
exactly prod
```

意味着只有：

```text
prod
```

能够匹配。

---

## 13. 推荐写法

需要精确比较 Bash 字符串时，可以采用：

```bash
if [[ "$variable" == "expected-value" ]]; then
  ...
else
  ...
fi
```

例如：

```bash
if [[ "$1" == "prod" ]]; then
  echo "protected"
else
  echo "open"
fi
```

这样能够明确表达：

> 只有字符串完全等于 `prod` 时，条件才成立。

---

## 知识点总结

* `[[ ... ]]` 是 Bash 的条件表达式语法。
* `==` 可以进行字符串比较。
* 字符串比较要求内容一致，`08` 与 `8` 不相等。
* `[[ "$value" == pattern ]]` 中，未引用的右侧可以作为模式。
* `*`、`?`、`[...]` 都可以参与模式匹配。
* `prod*` 会匹配 `prod`、`production`、`prod-eu` 等多个值。
* `"prod"` 表示字面字符串，因此适合精确匹配。
* 推荐写成：

```bash
[[ "$1" == "prod" ]]
```

* 正确脚本行为应为：

```text
prod        → protected
production  → open
```

* 两次运行结果记录文件应为：

```text
protected
open
```
