# Bash 函数的标准输出与返回状态

## 第一部分：学习笔记

### 1. 一个 Bash 函数实际上有两种“输出”

调用 Bash 函数时，需要区分两个完全不同的通道：

| 信息类型               | 用途                      | 获取方式               |
| ------------------ | ----------------------- | ------------------ |
| 标准输出 `stdout`      | 返回真正的数据，例如 URL、文件名、查询结果 | 直接输出或用 `$(...)` 捕获 |
| 返回状态 `exit status` | 表示成功或失败                 | `$?`               |

例如，一个查询函数找到 endpoint 时：

```bash
lookup api
```

它可以：

* 在标准输出打印：

```text
https://api.local
```

* 同时返回状态：

```text
0
```

这里：

* URL 是**数据**
* `0` 是**成功状态**

这两者不能混在一起。

---

## 2. 为什么错误信息不应该打印到标准输出

假设一个函数失败时这样写：

```bash
lookup() {
    echo "Sorry, key not found"
    return 2
}
```

如果调用者通过命令替换捕获结果：

```bash
endpoint=$(lookup other)
```

那么：

```bash
endpoint="Sorry, key not found"
```

调用者可能误以为这就是查询得到的数据。

因此，适合返回给调用者的数据才应该放在标准输出中。

对于当前的 `lookup` 函数：

* 查询成功：标准输出打印 endpoint
* 查询失败：标准输出保持为空
* 是否成功：使用返回状态表示

这种设计可以让“数据”和“状态”保持独立。

---

## 3. `return` 返回的是状态码，不是文本

Bash 函数中的：

```bash
return 2
```

并不会打印：

```text
2
```

它只是：

1. 立即结束当前函数；
2. 将函数的返回状态设置为 `2`。

调用结束后，可以通过：

```bash
$?
```

读取这个状态。

例如：

```bash
lookup "other"
echo "$?"
```

输出：

```text
2
```

注意：这里的 `2` 是 `echo "$?"` 打印出来的，而不是 `return 2` 自己打印的。

---

## 4. Bash 返回状态的惯例

通常：

```text
0     表示成功
非 0  表示失败
```

具体使用哪个非零值，由程序自己定义。

例如：

```bash
return 0
```

表示成功。

```bash
return 2
```

可以定义为“未知 key”。

因此函数完全可以：

```bash
printf '%s\n' 'https://api.local'
return 0
```

也可以：

```bash
return 2
```

而完全不产生标准输出。

---

## 5. `$?` 必须立即保存

`$?` 保存的是：

> **最近完成的那一条命令的返回状态。**

例如：

```bash
lookup "$1"; first=$?
```

执行顺序是：

1. 执行 `lookup "$1"`
2. 函数结束并产生状态码
3. `$?` 立即展开为这个状态
4. 保存到变量 `first`

这样：

```bash
first
```

就保存了第一次查询的结果状态。

### 为什么不能中间执行其他命令

错误示例：

```bash
lookup "$1"
echo "finished"
first=$?
```

此时 `first` 保存的已经不是 `lookup` 的状态，而是：

```bash
echo "finished"
```

的状态。

因为 `$?` 永远只记住**最后一条命令**。

最佳实践是立即保存：

```bash
lookup "$1"; first=$?
```

---

## 6. 标准输出和返回状态互不影响

下面这个函数：

```bash
example() {
    printf '%s\n' "some data"
    return 2
}
```

既：

* 打印了 `"some data"`
* 又返回失败状态 `2`

因此：

> “有没有输出数据”和“命令是否成功”是两个独立的问题。

反过来也一样：

```bash
example() {
    return 0
}
```

这个函数：

* 什么也不打印
* 但仍然成功返回 `0`

---

# `[[ ... ]]` 中字符串比较与模式匹配

## 1. 精确比较

Bash 可以使用：

```bash
[[ "$1" == "api" ]]
```

判断函数收到的第一个参数是否恰好等于：

```text
api
```

完整形式：

```bash
if [[ "$1" == "api" ]]; then
    ...
fi
```

这里两边都加引号：

```bash
"$1"
"api"
```

可以明确表达：

> 按普通字符串进行比较。

---

## 2. 为什么 `==` 右边值得加引号

在 Bash 的：

```bash
[[ ... ]]
```

中，`==` 右侧如果没有加引号，可以被解释为 **pattern（模式）**。

例如：

```bash
[[ "$1" == api* ]]
```

这里：

```text
api*
```

不是普通字符串，而是模式：

```text
以 api 开头的任意字符串
```

因此以下字符串都会匹配：

```text
api
apifoo
api123
api-server
```

而：

```bash
[[ "$1" == "api*" ]]
```

则要求字符串真的等于：

```text
api*
```

其中 `*` 被当作普通字符。

---

## 3. 为什么 `"api"` 看起来加不加引号都一样

下面两种写法：

```bash
[[ "$1" == api ]]
```

和：

```bash
[[ "$1" == "api" ]]
```

对于 `api` 来说结果相同。

原因是：

```text
api
```

本身没有：

```text
*
?
[...]
```

等通配符。

但是养成：

```bash
[[ "$1" == "api" ]]
```

的写法更清晰，也避免以后把值改成带通配符的字符串时产生意外的模式匹配。

---

# 标准输出重定向

运行：

```bash
bash lookup.sh api other > known.txt
```

其中：

```bash
> known.txt
```

只会把脚本的**标准输出**重定向到：

```text
known.txt
```

假设：

```bash
lookup api
```

打印：

```text
https://api.local
```

而：

```bash
lookup other
```

什么也不打印，那么最终：

```text
known.txt
```

只包含：

```text
https://api.local
```

函数返回的：

```text
0
2
```

不会自动进入标准输出，因此也不会自动进入 `known.txt`。

这正体现了标准输出和返回状态之间的区别。

---

# 第二部分：练习解答

## 问题

编写 `lookup.sh`：

1. 定义 `lookup` 函数，检查自己的第一个参数。
2. 使用：

```bash
[[ "$1" == "api" ]]
```

3. 参数为 `api` 时：

   * 打印 `https://api.local`
   * `return 0`
4. 其他参数：

   * 不产生标准输出
   * `return 2`
5. 分别调用函数处理脚本的第一个和第二个参数，并立即保存两次 `$?`。
6. 将两个状态写入 `lookup-statuses.txt`。
7. 脚本本身最终始终以状态 `0` 结束。
8. 执行：

```bash
bash lookup.sh api other > known.txt
```

---

## `lookup.sh`

```bash
#!/usr/bin/env bash

lookup() {
    if [[ "$1" == "api" ]]; then
        printf '%s\n' 'https://api.local'
        return 0
    fi

    return 2
}

lookup "$1"; first=$?
lookup "$2"; second=$?

printf '%s\n%s\n' "$first" "$second" > lookup-statuses.txt

exit 0
```

---

## 执行

在 `bash-lab` 目录中运行：

```bash
bash lookup.sh api other > known.txt
```

第一次调用相当于：

```bash
lookup "api"
```

因此：

* 标准输出：

```text
https://api.local
```

* 返回状态：

```text
0
```

第二次调用：

```bash
lookup "other"
```

因此：

* 标准输出为空
* 返回状态：

```text
2
```

---

## 最终文件内容

### `known.txt`

```text
https://api.local
```

### `lookup-statuses.txt`

```text
0
2
```

脚本最后执行：

```bash
exit 0
```

所以即使其中某个 `lookup` 查询失败，整个脚本最终的退出状态仍然是：

```text
0
```

---

# 核心记忆

```bash
printf / echo
```

负责输出**数据**。

```bash
return
```

负责返回函数的**成功或失败状态**。

```bash
$?
```

读取**最近一条命令**的状态，因此需要立即保存：

```bash
lookup "$1"; first=$?
```

最重要的设计原则是：

> **数据走标准输出，成功或失败走返回状态。**
