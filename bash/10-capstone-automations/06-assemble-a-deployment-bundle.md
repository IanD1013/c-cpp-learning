# Bash：构建并验证 `bundle.tgz`

## 目标

编写：

```text
/home/learner/bash-lab/build-bundle.sh
```

脚本需要：

1. 从第一个参数读取应用名；
2. 验证 `app.sh` 和 `app.conf` 都存在且是普通文件；
3. 创建临时 staging 目录；
4. 把两个文件和生成的 `metadata.txt` 放入 staging；
5. 只将这三个文件打包进 `bundle.tgz`；
6. 重新读取 archive，独立验证其中的成员和 metadata；
7. 无论成功还是失败，都删除 staging 目录；
8. 使用指定的退出状态区分不同失败情况。

---

## 最终脚本

进入目录：

```bash
cd /home/learner/bash-lab
```

创建 `build-bundle.sh`：

```bash
#!/usr/bin/env bash

app_name=${1:?application name is required}

validate_inputs() {
    local item

    for item in "$@"; do
        if [[ ! -f "$item" ]]; then
            return 1
        fi
    done
}

validate_inputs app.sh app.conf || exit 2

stage=$(mktemp -d "$PWD/stage.XXXXXX")
trap 'rm -rf -- "$stage"' EXIT

printf '%s\n' "$stage" > last-staging-path.txt

cp -- app.sh app.conf "$stage/"
printf 'name=%s\n' "$app_name" > "$stage/metadata.txt"

tar -czf bundle.tgz -C "$stage" app.sh app.conf metadata.txt || exit 3

tar -tzf bundle.tgz | sort > bundle-contents.txt
tar -xOf bundle.tgz metadata.txt > bundle-metadata.txt
```

赋予执行权限：

```bash
chmod +x build-bundle.sh
```

运行：

```bash
bash build-bundle.sh worker
```

---

## 1. 强制要求应用名

```bash
app_name=${1:?application name is required}
```

脚本这样运行：

```bash
bash build-bundle.sh worker
```

此时：

```bash
$1
```

就是：

```text
worker
```

所以：

```bash
app_name=worker
```

如果没有提供参数：

```bash
bash build-bundle.sh
```

`${1:?...}` 会立即终止脚本，并向标准错误输出类似：

```text
application name is required
```

关键点是：**这一检查发生在创建 `bundle.tgz` 和 staging 目录之前。**

因此缺少应用名时不会产生 archive。

---

## 2. 用函数验证输入文件

```bash
validate_inputs() {
    local item

    for item in "$@"; do
        if [[ ! -f "$item" ]]; then
            return 1
        fi
    done
}
```

调用：

```bash
validate_inputs app.sh app.conf || exit 2
```

### `"$@"` 是什么

`"$@"` 表示传给函数的全部参数，并且每个参数保持独立。

因此：

```bash
validate_inputs app.sh app.conf
```

相当于函数内部依次检查：

```text
app.sh
app.conf
```

### `[[ -f "$item" ]]`

`-f` 判断：

> 路径是否存在，并且是否为普通文件（regular file）。

因此：

```bash
[[ ! -f "$item" ]]
```

表示：

> 如果这个参数不是普通文件。

发现第一个无效输入后：

```bash
return 1
```

函数返回非零。

然后：

```bash
validate_inputs app.sh app.conf || exit 2
```

使整个脚本退出状态为：

```text
2
```

### 为什么要在创建 staging 之前验证

顺序必须是：

```text
检查应用名
    ↓
检查 app.sh 和 app.conf
    ↓
创建 staging
    ↓
创建 archive
```

这样输入错误时不会留下不应该存在的 `bundle.tgz`。

---

## 3. 创建唯一的临时 staging 目录

```bash
stage=$(mktemp -d "$PWD/stage.XXXXXX")
```

例如可能生成：

```text
/home/learner/bash-lab/stage.aB91xQ
```

`mktemp -d`：

* `-d`：创建目录；
* `XXXXXX`：由 `mktemp` 替换成随机字符；
* 每次运行都获得独立目录，避免并发运行互相冲突。

---

## 4. 创建后立即注册清理

```bash
trap 'rm -rf -- "$stage"' EXIT
```

`trap ... EXIT` 表示：

> Bash 脚本退出时执行指定命令。

所以以下情况都会触发清理：

```text
正常执行到脚本结尾
exit 2
exit 3
其他普通 shell 退出路径
```

这里执行：

```bash
rm -rf -- "$stage"
```

删除临时目录。

### 为什么 `trap` 要紧跟 `mktemp`

应该写成：

```bash
stage=$(mktemp -d "$PWD/stage.XXXXXX")
trap 'rm -rf -- "$stage"' EXIT
```

因为从临时目录创建出来的那一刻开始，就应该保证后续退出路径能够清理它。

---

## 5. 保存 staging 路径用于验证

```bash
printf '%s\n' "$stage" > last-staging-path.txt
```

例如：

```text
/home/learner/bash-lab/stage.aB91xQ
```

脚本结束后，可以读取这个文件，检查 staging 是否真的被删除。

例如：

```bash
cat last-staging-path.txt
```

然后：

```bash
test ! -e "$(cat last-staging-path.txt)"
```

如果退出状态为 `0`，说明该路径已经不存在。

---

## 6. 只把需要发布的文件放进 staging

复制输入：

```bash
cp -- app.sh app.conf "$stage/"
```

生成 metadata：

```bash
printf 'name=%s\n' "$app_name" > "$stage/metadata.txt"
```

运行：

```bash
bash build-bundle.sh worker
```

时，metadata 内容为：

```text
name=worker
```

此时 staging 中应该只有：

```text
app.sh
app.conf
metadata.txt
```

---

## 7. 明确指定 archive 成员

```bash
tar -czf bundle.tgz -C "$stage" app.sh app.conf metadata.txt || exit 3
```

各参数含义：

| 参数                             | 含义              |
| ------------------------------ | --------------- |
| `-c`                           | 创建 archive      |
| `-z`                           | 使用 gzip 压缩      |
| `-f bundle.tgz`                | 指定输出文件          |
| `-C "$stage"`                  | 先切换到 staging 目录 |
| `app.sh app.conf metadata.txt` | 明确指定需要打包的成员     |

### 为什么不能直接打包整个目录

不要写：

```bash
tar -czf bundle.tgz "$stage"
```

否则 archive 内可能出现：

```text
stage.aB91xQ/app.sh
stage.aB91xQ/app.conf
stage.aB91xQ/metadata.txt
```

下游期待的是：

```text
app.sh
app.conf
metadata.txt
```

所以使用：

```bash
-C "$stage"
```

再明确列出三个文件。

---

## 8. 为什么 `tar` 成功还不够

下面命令退出状态为 `0`：

```bash
tar -czf bundle.tgz ...
```

只能说明：

> `tar` 成功创建了 archive。

它不能证明 archive 内容正确。

例如它不会替你确认：

* 是否漏掉 `metadata.txt`；
* 是否错误加入额外文件；
* 成员路径是否带了 staging 前缀；
* metadata 内容是否正确。

因此创建后要**重新读取 archive 本身**。

---

## 9. 独立检查 archive 的成员

```bash
tar -tzf bundle.tgz | sort > bundle-contents.txt
```

这里：

```bash
tar -tzf bundle.tgz
```

表示列出 archive 内容。

`-t` 即 list。

经过：

```bash
sort
```

后，`bundle-contents.txt` 应该精确为：

```text
app.conf
app.sh
metadata.txt
```

验证：

```bash
cat bundle-contents.txt
```

---

## 10. 从 archive 本身读取 metadata

```bash
tar -xOf bundle.tgz metadata.txt > bundle-metadata.txt
```

参数：

| 参数              | 含义          |
| --------------- | ----------- |
| `-x`            | extract，提取  |
| `-O`            | 将提取内容写到标准输出 |
| `-f bundle.tgz` | 指定 archive  |
| `metadata.txt`  | 只提取这个成员     |

注意这里是大写：

```text
-O
```

而不是数字零。

运行：

```bash
bash build-bundle.sh worker
```

之后：

```bash
cat bundle-metadata.txt
```

应该得到：

```text
name=worker
```

这验证的是：

> **真正被打进 archive 的 metadata 内容。**

而不是 staging 中曾经存在的文件。

---

## 11. archive 写入失败时退出 3

核心命令：

```bash
tar -czf bundle.tgz -C "$stage" app.sh app.conf metadata.txt || exit 3
```

如果 `tar` 无法写入：

```text
bundle.tgz
```

则：

```bash
exit 3
```

但由于之前已经注册：

```bash
trap 'rm -rf -- "$stage"' EXIT
```

所以即使发生：

```bash
exit 3
```

staging 目录依然会被删除。

这正是 `trap` 的价值：**清理逻辑不需要散落在每一个失败分支中。**

---

# 通过 Lesson 的操作步骤

## 第一步：确认输入存在

```bash
cd /home/learner/bash-lab

ls -l app.sh app.conf
```

两个文件都应该存在。

---

## 第二步：创建脚本

`build-bundle.sh`：

```bash
#!/usr/bin/env bash

app_name=${1:?application name is required}

validate_inputs() {
    local item

    for item in "$@"; do
        if [[ ! -f "$item" ]]; then
            return 1
        fi
    done
}

validate_inputs app.sh app.conf || exit 2

stage=$(mktemp -d "$PWD/stage.XXXXXX")
trap 'rm -rf -- "$stage"' EXIT

printf '%s\n' "$stage" > last-staging-path.txt

cp -- app.sh app.conf "$stage/"
printf 'name=%s\n' "$app_name" > "$stage/metadata.txt"

tar -czf bundle.tgz -C "$stage" app.sh app.conf metadata.txt || exit 3

tar -tzf bundle.tgz | sort > bundle-contents.txt
tar -xOf bundle.tgz metadata.txt > bundle-metadata.txt
```

---

## 第三步：运行成功案例

```bash
bash build-bundle.sh worker
```

检查退出状态：

```bash
echo $?
```

应该是：

```text
0
```

---

## 第四步：检查 archive 成员

```bash
cat bundle-contents.txt
```

必须是：

```text
app.conf
app.sh
metadata.txt
```

也可以直接检查：

```bash
tar -tzf bundle.tgz
```

里面不应该出现：

```text
stage.xxxxxx/
README.txt
logs
build/
```

或者其他文件。

---

## 第五步：检查 metadata

```bash
cat bundle-metadata.txt
```

必须是：

```text
name=worker
```

也可以直接从 archive 读取：

```bash
tar -xOf bundle.tgz metadata.txt
```

应该得到：

```text
name=worker
```

---

## 第六步：确认 staging 已删除

```bash
cat last-staging-path.txt
```

然后：

```bash
test ! -e "$(cat last-staging-path.txt)"
echo $?
```

应该得到：

```text
0
```

表示路径已经不存在。

Lesson 还要求：

> 确认 staging 被删除之后，在 `bundle-cleanup.txt` 中写入 `clean`。

因此执行：

```bash
if [[ ! -e "$(cat last-staging-path.txt)" ]]; then
    printf 'clean\n' > bundle-cleanup.txt
fi
```

检查：

```bash
cat bundle-cleanup.txt
```

应该是：

```text
clean
```

---

# 最终应存在的结果

成功运行后：

```bash
ls
```

应包含这些关键文件：

```text
build-bundle.sh
bundle.tgz
bundle-contents.txt
bundle-metadata.txt
last-staging-path.txt
bundle-cleanup.txt
```

其中：

### `bundle-contents.txt`

```text
app.conf
app.sh
metadata.txt
```

### `bundle-metadata.txt`

```text
name=worker
```

### `bundle-cleanup.txt`

```text
clean
```

而：

```bash
cat last-staging-path.txt
```

记录的 staging 路径应该已经不存在。

---

# 必须理解的三个退出状态

| 情况                       | 退出状态 | 是否创建 `bundle.tgz` |
| ------------------------ | ---: | ----------------- |
| 没有提供应用名                  |  `1` | 否                 |
| `app.sh` / `app.conf` 缺失 |  `2` | 否                 |
| `tar` 无法写入 archive       |  `3` | 失败，且 staging 必须清理 |
| 正常成功                     |  `0` | 是                 |

---

# 最容易出错的地方

## 错误 1：先创建 staging，再验证输入

错误顺序：

```bash
stage=$(mktemp -d ...)
validate_inputs ...
```

正确：

```bash
app_name=${1:?application name is required}
validate_inputs app.sh app.conf || exit 2

stage=$(mktemp -d "$PWD/stage.XXXXXX")
```

输入验证必须先完成。

---

## 错误 2：忘记 `trap`

如果只在最后写：

```bash
rm -rf "$stage"
```

那么：

```bash
exit 3
```

发生时，最后的删除命令永远不会执行。

应该使用：

```bash
trap 'rm -rf -- "$stage"' EXIT
```

---

## 错误 3：把整个 staging 目录交给 `tar`

不要：

```bash
tar -czf bundle.tgz "$stage"
```

应该：

```bash
tar -czf bundle.tgz -C "$stage" app.sh app.conf metadata.txt
```

这样 archive 才只有三个裸文件名。

---

## 错误 4：只相信 `tar` 的退出状态

创建成功：

```bash
tar -czf bundle.tgz ...
```

不代表 archive 内容符合要求。

因此必须重新检查：

```bash
tar -tzf bundle.tgz | sort > bundle-contents.txt
tar -xOf bundle.tgz metadata.txt > bundle-metadata.txt
```

---

## 错误 5：忘记给 `tar` 写入失败设置状态 3

Lesson 明确要求：

```bash
tar -czf bundle.tgz -C "$stage" app.sh app.conf metadata.txt || exit 3
```

不能只写：

```bash
tar -czf bundle.tgz ...
```

---

# 最终执行命令

完成脚本后，最重要的一组命令是：

```bash
cd /home/learner/bash-lab

bash build-bundle.sh worker

cat bundle-contents.txt
cat bundle-metadata.txt

test ! -e "$(cat last-staging-path.txt)"
echo $?

if [[ ! -e "$(cat last-staging-path.txt)" ]]; then
    printf 'clean\n' > bundle-cleanup.txt
fi

cat bundle-cleanup.txt
```

预期核心结果：

```text
app.conf
app.sh
metadata.txt
name=worker
0
clean
```

核心设计思想可以概括为：

```text
先验证
→ 再创建临时工作区
→ 立即注册清理
→ 明确 staging 内容
→ 明确 archive 成员
→ archive 创建失败使用 exit 3
→ 从 archive 本身重新验证结果
→ EXIT trap 自动清理 staging
```
