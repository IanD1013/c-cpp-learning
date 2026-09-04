# 使用 `find -exec` 隔离长期未修改的缓存文件

## 目标

将 `/srv/cache` 中**超过 7 个完整 24 小时周期未修改的普通文件**移动到 `/srv/quarantine`，同时满足：

* 只移动普通文件；
* 保留原文件名；
* 文件名包含空格也能安全处理；
* 搜索过程不使用 `sudo`；
* 只有真正执行 `mv` 时使用 `sudo`；
* 最近修改的文件保持不动。

---

## 一、先找出符合条件的文件

```bash
find /srv/cache -type f -mtime +7
```

各部分含义：

| 部分           | 含义                        |
| ------------ | ------------------------- |
| `find`       | 搜索文件                      |
| `/srv/cache` | 从该目录开始搜索                  |
| `-type f`    | 只匹配普通文件                   |
| `-mtime +7`  | 匹配修改时间超过 7 个完整 24 小时周期的文件 |

建议先运行这条命令，确认哪些文件会被移动。

---

## 二、理解 `-mtime +7`

`-mtime` 根据文件内容的最后修改时间（mtime）判断文件年龄。

```bash
-mtime +7
```

并不简单等于：

> 超过 7 × 24 小时就匹配。

`find` 会先把文件年龄按 24 小时计算，然后**舍弃小数部分**，再进行比较。

例如：

|    距离最后修改 | 计算结果 | `-mtime +7` |
| --------: | ---: | ----------- |
|       7 天 |    7 | ❌           |
|     7.5 天 |    7 | ❌           |
| 7 天 23 小时 |    7 | ❌           |
|       8 天 |    8 | ✅           |
|       9 天 |    9 | ✅           |

因此：

```bash
-mtime +7
```

实际匹配的是大约 **8 天及以上**没有修改的文件。

### `+7`、`7`、`-7` 的区别

```bash
-mtime +7
```

年龄计数 **大于 7**。

```bash
-mtime 7
```

年龄计数 **恰好为 7**。

```bash
-mtime -7
```

年龄计数 **小于 7**。

---

## 三、使用 `-exec` 对搜索结果执行命令

找到文件之后，可以让 `find` 直接执行 `mv`：

```bash
find /srv/cache -type f -mtime +7 \
  -exec sudo mv -t /srv/quarantine -- {} +
```

这就是完整的移动命令。

---

## 四、理解 `-exec ... {} +`

核心部分：

```bash
-exec sudo mv -t /srv/quarantine -- {} +
```

### `{}`：匹配到的文件

`{}` 是占位符，代表 `find` 找到的路径。

假设找到了：

```text
/srv/cache/old cache.tmp
/srv/cache/expired.tmp
```

那么这些路径会作为独立的命令参数交给 `mv`。

---

### `+`：批量执行

结尾：

```bash
{} +
```

表示尽量把多个匹配结果一次性交给同一个命令。

效果类似：

```bash
sudo mv -t /srv/quarantine -- \
  "/srv/cache/old cache.tmp" \
  "/srv/cache/expired.tmp"
```

而不是每找到一个文件就启动一次 `mv`。

对比：

```bash
-exec command {} \;
```

每个文件执行一次命令。

```bash
-exec command {} +
```

多个文件批量执行，通常效率更高。

---

## 五、为什么这里使用 `mv -t`

普通的 `mv` 通常写成：

```bash
mv SOURCE DESTINATION
```

多个文件则写成：

```bash
mv FILE1 FILE2 FILE3 DESTINATION_DIRECTORY
```

也就是说，目标目录通常必须位于最后。

但是：

```bash
-exec ... {} +
```

会把找到的文件路径追加到命令后面。

因此如果写：

```bash
-exec mv {} /srv/quarantine +
```

就无法正确表达“最后一个参数永远是目标目录”。

GNU `mv` 提供了：

```bash
-t DIRECTORY
```

用于提前指定目标目录：

```bash
mv -t /srv/quarantine FILE1 FILE2 FILE3
```

因此它非常适合和：

```bash
{} +
```

组合：

```bash
-exec sudo mv -t /srv/quarantine -- {} +
```

---

## 六、为什么需要 `--`

完整的 `mv` 部分是：

```bash
sudo mv -t /srv/quarantine -- {}
```

其中：

```bash
--
```

表示：

> 命令选项到这里结束，后面的内容全部当作普通参数处理。

例如某个文件名以 `-` 开头：

```text
-weird-file.tmp
```

没有 `--` 时，一些命令可能把它误认为选项。

使用：

```bash
mv -- -weird-file.tmp destination/
```

可以明确告诉 `mv`：

> `-weird-file.tmp` 是文件名，不是命令选项。

这是处理未知文件名时的良好习惯。

---

## 七、为什么不用管道传递文件名

假设文件名是：

```text
old cache.tmp
```

错误思路可能是：

```bash
find /srv/cache -type f -mtime +7 | xargs mv ...
```

如果文件名处理不正确，空格可能导致它被拆成：

```text
old
cache.tmp
```

于是 `mv` 会认为这是两个不同文件。

而：

```bash
find ... -exec ... {} +
```

不会通过普通文本让 shell 重新拆分文件名。

`find` 会把每个匹配结果作为**独立参数**直接传递给命令。

所以：

```text
/srv/cache/old cache.tmp
```

始终是一个参数。

这也是 `-exec` 特别适合处理未知文件名的重要原因。

> 如果必须使用 `xargs`，通常应该配合 `find -print0` 和 `xargs -0`，而不能依赖换行或空格分隔文件名。

---

## 八、为什么只给 `mv` 使用 `sudo`

搜索文件：

```bash
find /srv/cache -type f -mtime +7
```

本身不需要修改任何数据，因此如果当前用户拥有读取和遍历目录的权限，没有必要使用：

```bash
sudo find ...
```

真正需要管理员权限的是移动操作。

因此推荐：

```bash
find /srv/cache -type f -mtime +7 \
  -exec sudo mv -t /srv/quarantine -- {} +
```

权限提升只发生在：

```bash
sudo mv ...
```

这种原则可以概括为：

> **只对真正需要更高权限的操作使用 `sudo`。**

---

## 九、推荐操作流程

### 1. 先预览

```bash
find /srv/cache -type f -mtime +7
```

检查结果是否正确。

例如：

```text
/srv/cache/old cache.tmp
```

而较新的：

```text
/srv/cache/recent.tmp
```

不应该出现。

### 2. 确认后移动

```bash
find /srv/cache -type f -mtime +7 \
  -exec sudo mv -t /srv/quarantine -- {} +
```

### 3. 验证结果

检查原目录：

```bash
ls -l /srv/cache
```

检查隔离目录：

```bash
ls -l /srv/quarantine
```

预期：

* 旧文件已经从 `/srv/cache` 消失；
* 它以相同文件名出现在 `/srv/quarantine`；
* 最近修改的文件仍然留在 `/srv/cache`。

---

## 最终命令

先查看哪些文件符合条件：

```bash
find /srv/cache -type f -mtime +7
```

确认后移动：

```bash
find /srv/cache -type f -mtime +7 \
  -exec sudo mv -t /srv/quarantine -- {} +
```

## 核心知识点

```text
find
 ├── /srv/cache       搜索范围
 ├── -type f          只选择普通文件
 ├── -mtime +7        选择年龄计数大于 7 的文件
 └── -exec
      └── sudo mv
           ├── -t /srv/quarantine   提前指定目标目录
           ├── --                    结束选项解析
           ├── {}                    find 找到的路径
           └── +                     批量传递多个路径
```

最重要的组合是：

```bash
find ... -exec command {} +
```

它能够让 `find` **直接、安全地把匹配到的路径作为独立参数传递给其他命令**，避免普通文本管道在处理空格等特殊文件名时产生参数拆分问题。
