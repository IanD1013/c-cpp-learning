# Bash 使用 `tar` 精确打包发布文件

## 目标

发布包中只允许包含以下 3 个文件：

```text
app.sh
app.conf
README.txt
```

目录中的其他内容，例如日志、CSV、缓存目录等，都属于开发过程中的工作状态，不能进入最终发布包。

因此，打包脚本应遵循两个原则：

1. **打包前验证所有必需文件都存在**
2. **显式指定要放入归档的文件，而不是直接打包整个目录**

最终生成：

```text
release.tgz
```

---

## 1. 不要直接打包整个目录

假设当前目录为：

```text
bash-lab/
├── app.sh
├── app.conf
├── README.txt
├── API.log
├── worker.log
├── records.csv
├── summary.txt
├── health.txt
└── build/
    └── cache
```

如果执行类似：

```bash
tar -czf release.tgz .
```

那么当前目录中的工作文件也会全部进入发布包。

更安全的方式是明确列出允许发布的文件：

```bash
tar -czf release.tgz app.sh app.conf README.txt
```

这样归档中只会包含这三个成员。

---

## 2. `tar -czf` 的含义

命令：

```bash
tar -czf release.tgz app.sh app.conf README.txt
```

可以拆成：

| 参数            | 含义            |
| ------------- | ------------- |
| `-c`          | create，创建新的归档 |
| `-z`          | 使用 gzip 压缩    |
| `-f`          | 指定归档文件名       |
| `release.tgz` | 要生成的归档文件      |
| 后面的文件名        | 要加入归档的成员      |

其中 `-f` 很重要：

```bash
-f release.tgz
```

表示 `release.tgz` 是输出归档的名字，因此它必须出现在待打包文件之前。

最终归档内部的成员应当正好是：

```text
app.sh
app.conf
README.txt
```

---

## 3. 打包前必须完整验证输入文件

发布包要求三个文件缺一不可，所以应逐个检查：

```bash
for file in app.sh app.conf README.txt
do
  [[ -f "$file" ]] || exit 2
done
```

### `for` 循环

```bash
for file in app.sh app.conf README.txt
```

循环三次，使 `$file` 依次等于：

```text
app.sh
app.conf
README.txt
```

### `[[ -f "$file" ]]`

```bash
[[ -f "$file" ]]
```

`-f` 判断：

> 指定路径是否存在，并且是普通文件。

例如：

```bash
[[ -f "app.conf" ]]
```

如果 `app.conf` 存在，则返回状态 `0`；否则返回非零状态。

---

## 4. `|| exit 2`：检查失败立即停止

这一行：

```bash
[[ -f "$file" ]] || exit 2
```

利用 Bash 的 OR 列表。

基本形式：

```bash
command1 || command2
```

含义是：

> 只有当 `command1` 失败时，才执行 `command2`。

因此：

```bash
[[ -f "$file" ]] || exit 2
```

表示：

> 如果当前文件不存在，立即让整个脚本以状态码 `2` 退出。

例如缺少：

```text
README.txt
```

执行过程会在验证阶段结束：

```text
app.sh      -> 存在
app.conf    -> 存在
README.txt  -> 不存在
                 ↓
              exit 2
```

程序不会继续执行后面的 `tar`。

---

## 5. 为什么验证必须放在 `tar` 前面

正确结构：

```bash
for file in app.sh app.conf README.txt
do
  [[ -f "$file" ]] || exit 2
done

tar -czf release.tgz app.sh app.conf README.txt
```

这里存在明确的顺序：

```text
验证全部文件
    ↓
所有文件都存在
    ↓
执行 tar
    ↓
创建 release.tgz
```

如果任何文件不存在：

```text
验证失败
    ↓
exit 2
    ↓
tar 永远不会运行
```

因此脚本自身不会创建一个缺文件的发布包。

---

## 6. 推荐的完整 `package-release.sh`

```bash
#!/usr/bin/env bash

for file in app.sh app.conf README.txt
do
  [[ -f "$file" ]] || exit 2
done

tar -czf release.tgz app.sh app.conf README.txt
```

保存位置：

```text
/home/learner/bash-lab/package-release.sh
```

如果需要赋予执行权限：

```bash
chmod +x package-release.sh
```

运行：

```bash
./package-release.sh
```

如果三个文件都存在，脚本正常结束，退出状态为：

```text
0
```

并生成：

```text
release.tgz
```

---

## 7. 检查脚本退出状态

运行后可以查看 `$?`：

```bash
./package-release.sh
echo $?
```

成功时：

```text
0
```

如果缺少必需文件：

```text
2
```

例如：

```bash
mv app.conf app.conf.bak

./package-release.sh
echo $?
```

应得到：

```text
2
```

---

## 8. 独立检查归档实际包含了什么

不能只看打包脚本“打算”放什么，还应该检查归档实际存储的成员。

使用：

```bash
tar -tzf release.tgz
```

其中：

| 参数   | 含义           |
| ---- | ------------ |
| `-t` | list，列出归档成员  |
| `-z` | 处理 gzip 压缩归档 |
| `-f` | 后面跟归档文件名     |

正确输出应为：

```text
app.sh
app.conf
README.txt
```

不能出现：

```text
API.log
worker.log
records.csv
summary.txt
health.txt
build/
```

---

## 9. 一个容易忽略的问题：旧的 `release.tgz`

假设之前已经成功创建过：

```text
release.tgz
```

之后删除了 `app.conf`，再次执行：

```bash
./package-release.sh
```

验证会正确返回 `2`，而且这一次不会执行 `tar`。

但是，**之前留下的旧 `release.tgz` 并不会因为 `exit 2` 自动消失**。

也就是说：

```text
旧 release.tgz 已存在
        ↓
app.conf 被删除
        ↓
重新运行脚本
        ↓
验证失败，exit 2
        ↓
旧 release.tgz 仍可能存在
```

如果测试要求：

> 必需文件缺失时，`release.tgz` 必须不存在

那么测试前应先删除旧归档：

```bash
rm -f release.tgz
```

或者为了让脚本自身满足更强的保证，可以在验证前清理旧包：

```bash
#!/usr/bin/env bash

rm -f release.tgz

for file in app.sh app.conf README.txt
do
  [[ -f "$file" ]] || exit 2
done

tar -czf release.tgz app.sh app.conf README.txt
```

这样即使以前存在旧发布包，本次验证失败后目录中也不会残留 `release.tgz`。

---

## 10. 如何验证所有要求

### 正常情况

确认：

```text
app.sh
app.conf
README.txt
```

都存在，然后：

```bash
rm -f release.tgz
./package-release.sh
echo $?
tar -tzf release.tgz
```

应得到退出状态：

```text
0
```

归档成员：

```text
app.sh
app.conf
README.txt
```

---

### 缺少 `app.sh`

```bash
rm -f release.tgz
mv app.sh app.sh.bak

./package-release.sh
echo $?
```

应满足：

```text
退出状态：2
release.tgz：不存在
```

测试完成后恢复：

```bash
mv app.sh.bak app.sh
```

---

### 缺少 `app.conf`

```bash
rm -f release.tgz
mv app.conf app.conf.bak

./package-release.sh
echo $?
```

应满足：

```text
退出状态：2
release.tgz：不存在
```

然后恢复：

```bash
mv app.conf.bak app.conf
```

---

### 缺少 `README.txt`

```bash
rm -f release.tgz
mv README.txt README.txt.bak

./package-release.sh
echo $?
```

应满足：

```text
退出状态：2
release.tgz：不存在
```

然后恢复：

```bash
mv README.txt.bak README.txt
```

---

## 11. 常见错误

### 错误一：打包整个目录

```bash
tar -czf release.tgz .
```

问题：

> 会把日志、CSV、缓存目录等不应该发布的内容一起打包。

发布脚本更安全的策略是使用**白名单**：

```bash
tar -czf release.tgz app.sh app.conf README.txt
```

---

### 错误二：只验证一个文件

```bash
[[ -f app.sh ]] || exit 2

tar -czf release.tgz app.sh app.conf README.txt
```

这不能保证：

```text
app.conf
README.txt
```

一定存在。

应该统一验证所有必需文件：

```bash
for file in app.sh app.conf README.txt
do
  [[ -f "$file" ]] || exit 2
done
```

---

### 错误三：先打包，再验证

错误顺序：

```bash
tar -czf release.tgz app.sh app.conf README.txt

[[ -f app.conf ]] || exit 2
```

验证已经失去意义，因为发布步骤已经发生。

正确原则是：

> **先验证（validate），再执行有副作用的操作。**

---

### 错误四：认为脚本失败会自动删除旧归档

```bash
exit 2
```

只会终止当前脚本，不会删除以前存在的文件。

如果要求失败时归档必须不存在，需要考虑：

```bash
rm -f release.tgz
```

---

## 12. 核心知识总结

这个发布脚本体现了几个很重要的 Shell 编程原则：

### 显式打包

只把允许发布的文件写到 `tar` 命令中：

```bash
tar -czf release.tgz app.sh app.conf README.txt
```

相比“打包所有东西再排除”，显式白名单通常更容易保证发布内容正确。

### 先验证，再执行

```bash
for file in ...
do
  [[ -f "$file" ]] || exit 2
done
```

必须放在真正创建归档之前。

### 失败立即退出

```bash
condition || exit 2
```

适合表达：

> 某个前置条件不成立时，后续操作绝不能继续。

### 验证实际结果

创建归档后使用：

```bash
tar -tzf release.tgz
```

检查的是 **tar 实际存储了什么**，而不是脚本作者原本想存储什么。

最终发布包应严格满足：

```text
release.tgz
├── app.sh
├── app.conf
└── README.txt
```
