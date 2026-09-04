# Linux `find`：按文件类型、名称和权限筛选文件

## 1. `find` 的核心思路

`ls -R` 会递归列出目录中的所有内容，但真正的筛选工作需要人工完成。

`find` 的思路相反：

> 指定“从哪里找”以及“要找什么”，让系统递归遍历目录并只返回符合条件的文件。

基本语法：

```bash
find <起始目录> <条件1> <条件2> ...
```

例如：

```bash
find /srv/course -type f -name '*.tmp'
```

含义是：

1. 从 `/srv/course` 开始递归搜索；
2. 只保留普通文件；
3. 文件名必须以 `.tmp` 结尾。

---

## 2. 搜索范围：首先指定起始目录

```bash
find /srv/course
```

`/srv/course` 是搜索根目录，`find` 会递归检查它下面的所有内容。

应尽量缩小搜索范围。例如：

```bash
find /srv/course ...
```

通常比：

```bash
find / ...
```

更合适，因为从 `/` 开始会遍历整个文件系统，不但更慢，还可能遇到大量无关文件和权限错误。

---

## 3. `-type f`：只查普通文件

```bash
-type f
```

其中：

* `f` = regular file，普通文件
* `d` = directory，目录
* `l` = symbolic link，符号链接

例如：

```bash
find /srv/course -type f
```

只返回普通文件。

这一条件很重要，因为目录本身也可能叫：

```text
cache.tmp/
```

如果任务要求寻找 `.tmp` **文件**，仅使用：

```bash
-name '*.tmp'
```

可能把这个目录也选出来。

加入：

```bash
-type f
```

即可排除目录。

---

## 4. `-name '*.tmp'`：按文件名匹配

```bash
-name '*.tmp'
```

表示文件名必须符合 shell glob 模式：

```text
*.tmp
```

其中 `*` 表示任意数量的字符，因此可以匹配：

```text
a.tmp
cache.tmp
report.old.tmp
```

但不会匹配：

```text
file.txt
tmp.log
```

### `-name` 匹配的是文件名

例如文件：

```text
/srv/course/logs/cache.tmp
```

`-name '*.tmp'` 检查的是：

```text
cache.tmp
```

而不是整个：

```text
/srv/course/logs/cache.tmp
```

如果需要匹配完整路径，可以使用 `-path`。

---

## 5. 为什么 `*.tmp` 必须加引号

正确：

```bash
find /srv/course -name '*.tmp'
```

不推荐：

```bash
find /srv/course -name *.tmp
```

原因是：

> `find` 运行之前，Bash 会先处理命令行中的通配符。

假设当前目录正好有：

```text
hello.tmp
```

那么：

```bash
find /srv/course -name *.tmp
```

可能会先被 Bash 展开成：

```bash
find /srv/course -name hello.tmp
```

这时 `find` 根本没有收到 `*.tmp`。

使用：

```bash
'*.tmp'
```

可以阻止 Bash 提前展开，让这个模式完整地交给 `find`。

因此使用 `find -name` 时，通常应写：

```bash
-name '*.tmp'
```

---

# 6. Unix 文件权限中的 `0002`

一个典型权限：

```text
-rw-r--rw-
```

可以分成：

```text
owner   group   others
rw-     r--     rw-
```

每组权限由三个 bit 表示：

| 权限     | 数值 |
| ------ | -: |
| `r` 读取 |  4 |
| `w` 写入 |  2 |
| `x` 执行 |  1 |

因此：

```text
0002
```

重点表示：

```text
others 的 write 位
```

也就是：

> 其他用户是否拥有写权限。

例如：

```text
-rw-r--rw-
```

最后三位：

```text
rw-
```

包含 `w`，因此属于 **world-writable** 文件。

---

# 7. `-perm -0002`：检查某个权限位是否存在

要寻找“其他用户可写”的文件，可以使用：

```bash
-perm -0002
```

这里最重要的是 `0002` 前面的 `-`。

它表示：

> 指定的这些权限位必须全部存在，其他权限位是什么都无所谓。

因此：

```bash
-perm -0002
```

检查的是：

> others 的 write bit 是否开启。

例如以下文件都会匹配：

```text
-rw-rw-rw-
-rwxr-xrwx
-rw-r--rw-
---------w-
```

因为它们都包含：

```text
others write
```

---

## 8. `-perm 0002` 和 `-perm -0002` 的区别

### 精确匹配

```bash
-perm 0002
```

要求权限模式精确等于：

```text
---------w-
```

也就是说除了 others-write 之外，其他权限位都不能存在。

这通常不是实际任务想表达的意思。

例如：

```text
-rw-r--rw-
```

虽然明显允许其他用户写入，但不会匹配：

```bash
-perm 0002
```

因为它还有其他权限。

---

### 检查指定权限是否存在

```bash
-perm -0002
```

则表示：

```text
至少必须包含 0002 这些权限位
```

因此：

```text
-rw-r--rw-
```

可以匹配。

寻找 **world-writable files** 时，通常使用：

```bash
-perm -0002
```

---

# 9. 多个 `find` 条件默认是 AND

例如：

```bash
find /srv/course \
  -type f \
  -name '*.tmp' \
  -perm -0002
```

等价于：

```text
是普通文件
AND
文件名以 .tmp 结尾
AND
其他用户拥有写权限
```

只有同时满足三个条件的路径才会输出。

因此可以把 `find` 理解成逐层过滤：

```text
所有对象
   ↓
普通文件
   ↓
*.tmp 文件
   ↓
others 可写
   ↓
最终结果
```

---

# 10. 使用 `sort` 获得稳定顺序

`find` 返回文件的顺序通常依赖文件系统遍历顺序，不应假设结果天然是按字母排序的。

因此可以：

```bash
find ... | sort
```

例如：

```bash
find /srv/course -type f -name '*.tmp' -perm -0002 | sort
```

管道：

```bash
|
```

表示：

> 把左边命令的标准输出作为右边命令的标准输入。

于是：

1. `find` 找出符合条件的路径；
2. `sort` 按顺序排列这些路径。

这样结果更加稳定，尤其适合自动测试、脚本输出和保存结果。

---

# 11. 使用 `>` 将结果保存到文件

```bash
> /home/learner/world-writable
```

表示把标准输出重定向到：

```text
/home/learner/world-writable
```

如果文件不存在，会创建它。

如果已经存在，会覆盖原内容。

因此：

```bash
find ... | sort > /home/learner/world-writable
```

数据流可以理解为：

```text
find
 ↓
符合条件的路径
 ↓
sort
 ↓
排序后的路径
 ↓
/home/learner/world-writable
```

---

# 12. 完整命令

寻找 `/srv/course` 中：

* 普通文件；
* 文件名以 `.tmp` 结尾；
* 其他用户拥有写权限；

然后按路径排序，并保存到 `/home/learner/world-writable`：

```bash
find /srv/course -type f -name '*.tmp' -perm -0002 | sort > /home/learner/world-writable
```

命令逐部分解释：

```bash
find /srv/course
```

从 `/srv/course` 递归搜索。

```bash
-type f
```

只保留普通文件。

```bash
-name '*.tmp'
```

只保留名称以 `.tmp` 结尾的文件。

```bash
-perm -0002
```

只保留 others 拥有写权限的文件。

```bash
| sort
```

将结果排序。

```bash
> /home/learner/world-writable
```

把最终结果写入指定文件。

---

# 13. 如何检查结果

执行后可以查看：

```bash
cat /home/learner/world-writable
```

例如可能得到：

```text
/srv/course/cache/debug.tmp
/srv/course/logs/test.tmp
```

每个路径占一行。

也可以手动检查权限：

```bash
ls -l /srv/course/cache/debug.tmp
```

如果看到类似：

```text
-rw-r--rw-
```

最后三位：

```text
rw-
```

说明 `others` 确实拥有写权限。

---

# 14. 常见错误

## 错误 1：忘记 `-type f`

```bash
find /srv/course -name '*.tmp'
```

可能同时找到：

* `.tmp` 文件；
* 名字以 `.tmp` 结尾的目录。

如果题目明确要求文件，应加入：

```bash
-type f
```

---

## 错误 2：通配符没有加引号

```bash
-name *.tmp
```

`*.tmp` 可能被 Bash 提前展开。

应写：

```bash
-name '*.tmp'
```

---

## 错误 3：使用 `-perm 0002`

```bash
-perm 0002
```

这是精确权限匹配，并不等价于“others 可以写”。

寻找任何包含 others-write 权限的文件，应写：

```bash
-perm -0002
```

---

## 错误 4：从 `/` 开始搜索

```bash
find / ...
```

如果任务范围只是：

```text
/srv/course
```

就应该：

```bash
find /srv/course ...
```

更快、更准确，也能减少无关权限错误。

---

## 错误 5：忘记排序

```bash
find /srv/course -type f -name '*.tmp' -perm -0002 > /home/learner/world-writable
```

筛选本身是正确的，但输出顺序可能因环境而变化。

如果要求结果稳定，应：

```bash
find /srv/course -type f -name '*.tmp' -perm -0002 | sort > /home/learner/world-writable
```

---

# 核心记忆

```bash
find 起始目录 条件...
```

典型组合：

```bash
find /srv/course -type f -name '*.tmp' -perm -0002
```

记住三个关键点：

```text
-type f
```

只找普通文件。

```text
-name '*.tmp'
```

按文件名匹配，glob 通常要加引号。

```text
-perm -0002
```

检查指定权限位是否至少存在；这里表示“其他用户可写”。

最终完整命令：

```bash
find /srv/course -type f -name '*.tmp' -perm -0002 | sort > /home/learner/world-writable
```
