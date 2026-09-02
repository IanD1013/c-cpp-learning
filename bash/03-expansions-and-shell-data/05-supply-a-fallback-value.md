# Bash 默认值参数展开：`${VAR:-default}`

## 核心概念

在 Bash 中，读取一个**未设置（unset）**的变量时，默认不会报错，而是展开为空字符串。

例如：

```bash
printf '%s\n' "$REGION"
```

如果 `REGION` 没有设置，等价于：

```bash
printf '%s\n' ""
```

程序因此可能继续运行，只是拿到了一个空值。这种“静默失败”有时比直接报错更难发现。

当变量没有值时需要自动使用备用值，可以使用 Bash 的**默认值参数展开（default-value parameter expansion）**：

```bash
${VAR:-default}
```

例如：

```bash
printf '%s\n' "${REGION:-eu-west}"
```

其含义是：

* `REGION` 有非空值 → 使用 `REGION` 的值；
* `REGION` 未设置 → 使用 `eu-west`；
* `REGION=""` → 也使用 `eu-west`。

---

## `${VAR:-default}` 的结构

```bash
${REGION:-eu-west}
```

可以拆成：

| 部分        | 含义              |
| --------- | --------------- |
| `${`      | 开始参数展开          |
| `REGION`  | 要读取的变量          |
| `:-`      | 当变量未设置或为空时使用默认值 |
| `eu-west` | 默认值             |
| `}`       | 结束参数展开          |

例如：

```bash
REGION=us-east
printf '%s\n' "${REGION:-eu-west}"
```

输出：

```text
us-east
```

因为 `REGION` 已经有值。

如果：

```bash
unset REGION
printf '%s\n' "${REGION:-eu-west}"
```

输出：

```text
eu-west
```

---

## `:-` 中冒号的作用

`${VAR:-default}` 中的冒号非常重要。

### 使用 `:-`

```bash
${VAR:-default}
```

以下两种情况都会使用默认值：

1. 变量未设置；
2. 变量已经设置，但值为空字符串。

例如：

```bash
unset VAR
printf '%s\n' "${VAR:-hello}"
```

输出：

```text
hello
```

以及：

```bash
VAR=""
printf '%s\n' "${VAR:-hello}"
```

仍然输出：

```text
hello
```

---

### 不使用冒号：`${VAR-default}`

Bash 还支持：

```bash
${VAR-default}
```

它只在变量**未设置**时使用默认值。

如果变量已经存在，即使它是空字符串，也不会使用默认值：

```bash
VAR=""
printf '%s\n' "${VAR-hello}"
```

输出的是一个空行，而不是：

```text
hello
```

因此可以记住：

| 写法                | 未设置 | 空字符串 | 有值 |
| ----------------- | --- | ---- | -- |
| `${VAR:-default}` | 默认值 | 默认值  | 原值 |
| `${VAR-default}`  | 默认值 | 空字符串 | 原值 |

实际 Shell 脚本中，`${VAR:-default}` 通常更加常用，因为“没有设置”和“设置成空字符串”往往都可以视为“没有有效值”。

---

## 默认值不会修改变量

这是 `${VAR:-default}` 最容易误解的地方：

```bash
unset REGION
printf '%s\n' "${REGION:-eu-west}"
```

虽然输出：

```text
eu-west
```

但 Bash **并没有执行**：

```bash
REGION=eu-west
```

执行完成后：

```bash
printf '<%s>\n' "$REGION"
```

仍然得到：

```text
<>
```

也就是说：

```bash
${REGION:-eu-west}
```

只是表示：

> 这一次展开时，如果 `REGION` 没有有效值，就临时使用 `eu-west`。

默认值只存在于这一次参数展开中。

---

## 为什么通常要加双引号

推荐写：

```bash
"${REGION:-eu-west}"
```

而不是：

```bash
${REGION:-eu-west}
```

原因是变量内容可能包含空格或其他特殊字符。

例如：

```bash
REGION="west europe"
printf '<%s>\n' "${REGION:-eu-west}"
```

会把：

```text
west europe
```

作为**一个完整参数**传递给 `printf`。

因此 Bash 中通常应养成习惯：

```bash
"$VAR"
"${VAR:-default}"
```

除非明确知道自己需要 Bash 进行单词分割或通配符展开。

---

## `unset` 与空字符串不是一回事

### 未设置变量

```bash
unset REGION
```

此时变量不存在。

### 设置为空字符串

```bash
REGION=""
```

此时变量存在，只是它的值长度为 0。

`${VAR:-default}` 会把这两种情况都视为需要默认值：

```bash
unset REGION
printf '%s\n' "${REGION:-eu-west}"

REGION=""
printf '%s\n' "${REGION:-eu-west}"
```

两次都会输出：

```text
eu-west
```

---

## 为什么测试前可以先执行 `unset`

环境变量可能从父 Shell 继承而来。

例如当前环境中可能已经存在：

```bash
export REGION=ap-southeast
```

这时：

```bash
printf '%s\n' "${REGION:-eu-west}"
```

会输出：

```text
ap-southeast
```

而不是默认值。

如果目的是明确测试“变量不存在时使用默认值”，可以先执行：

```bash
unset REGION
```

这样可以避免当前 Shell 中已有的值影响结果。

---

## 示例：把默认区域写入文件

假设当前工作目录是：

```text
/home/learner/bash-lab
```

可以执行：

```bash
unset REGION
printf '%s\n' "${REGION:-eu-west}" > region.txt
```

执行过程：

1. `unset REGION`

   * 删除当前 Shell 中的 `REGION` 变量。

2. `${REGION:-eu-west}`

   * `REGION` 未设置；
   * 因此展开成 `eu-west`。

3. `printf '%s\n'`

   * 输出字符串；
   * 并在末尾添加换行符。

4. `> region.txt`

   * 将标准输出写入 `region.txt`；
   * 如果文件已经存在，会覆盖原内容。

最终：

```text
region.txt
```

内容为：

```text
eu-west
```

并且文件末尾存在换行符。

完整命令：

```bash
cd /home/learner/bash-lab

unset REGION
printf '%s\n' "${REGION:-eu-west}" > region.txt
```

---

## 常见误区

### 误区 1：认为默认值会保存到变量中

```bash
unset REGION
printf '%s\n' "${REGION:-eu-west}"
```

不会让：

```bash
REGION=eu-west
```

永久成立。

`${VAR:-default}` 只提供临时默认值。

---

### 误区 2：认为未设置变量一定会报错

普通 Bash 默认情况下：

```bash
echo "$NOT_DEFINED"
```

通常只会输出空行。

只有在启用了：

```bash
set -u
```

或：

```bash
set -o nounset
```

等设置后，访问未设置变量才可能直接报错。

---

### 误区 3：忽略空字符串的情况

下面两个状态不同：

```bash
unset REGION
```

和：

```bash
REGION=""
```

但：

```bash
${REGION:-eu-west}
```

对二者的处理结果相同：都使用 `eu-west`。

---

## 记忆口诀

```bash
${VAR:-default}
```

可以理解为：

> `VAR` 如果“没有有效值”，这一次就使用 `default`。

其中“没有有效值”包括：

* 没有设置；
* 设置了，但为空字符串。

而且：

> `:-` 只决定这次展开使用什么值，不会给变量赋值。
