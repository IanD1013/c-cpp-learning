# ZIP：面向跨平台文件交换的打包与压缩格式

## 1. ZIP 适合什么场景

ZIP 的主要优势是**跨平台兼容性好**。如果压缩包需要交给 Windows、macOS 或 Linux 用户，尤其希望对方直接使用系统自带工具打开，`.zip` 通常是稳妥的选择。

常见经验：

* **与普通用户或不同操作系统交换文件**：优先考虑 ZIP。
* **Linux/Unix 机器之间传输、备份目录**：通常更常见 `tar.gz`、`tar.xz` 等格式。
* 现代 Windows 和 macOS 对多种压缩格式的支持已经比过去更好，因此不能简单认为它们“无法打开 `.tar.gz`”；ZIP 的优势主要是**普遍、直观、工具支持广泛**。

---

## 2. ZIP 与 `tar.gz` 的结构差异

ZIP 和 `tar.gz` 最大的概念差异是：**ZIP 自己同时负责归档和压缩，而 `tar.gz` 是两个工具组合起来完成的。**

### ZIP

```text
多个文件
   ↓
  ZIP
   ↓
reports.zip
```

ZIP 可以：

1. 把多个文件放进一个归档；
2. 同时压缩其中的数据。

因此只有一个扩展名：

```text
reports.zip
```

### `tar.gz`

```text
多个文件
   ↓
  tar
   ↓
reports.tar
   ↓
 gzip
   ↓
reports.tar.gz
```

其中：

* `tar`：负责把多个文件和目录组织成一个归档；
* `gzip`：负责压缩这个单独的归档文件。

因此：

```text
.tar.gz
```

实际上表示两个阶段：

```text
tar → gzip
```

---

## 3. 使用 `zip` 创建压缩包

基本语法：

```bash
zip ARCHIVE.zip FILE1 FILE2 ...
```

例如：

```bash
zip reports.zip summary.txt detail.txt
```

执行后：

```text
summary.txt
detail.txt
    ↓
reports.zip
```

与 `gzip` 不同，`zip` 默认**不会删除原文件**。

---

## 4. ZIP 会保存文件路径

ZIP 不只是保存文件内容，还会保存文件名以及路径信息。

这是使用 ZIP 时非常重要的一点。

假设存在：

```text
/srv/reports/
├── summary.txt
└── detail.txt
```

### 推荐方式：先进入目录再压缩

```bash
cd /srv/reports
zip /home/learner/reports.zip summary.txt detail.txt
```

此时 ZIP 中保存的是：

```text
summary.txt
detail.txt
```

可以查看：

```bash
unzip -l /home/learner/reports.zip
```

这通常是文件交换时最理想的结构。

---

## 5. 为什么不建议直接写很长的路径

例如：

```bash
zip reports.zip /srv/reports/summary.txt
```

常见的 Info-ZIP `zip` 实现会移除开头的 `/`，但仍然保存其余目录结构，因此归档中可能出现：

```text
srv/reports/summary.txt
```

而不是简单的：

```text
summary.txt
```

解压后就可能得到：

```text
srv/
└── reports/
    └── summary.txt
```

因此，如果目标是让 ZIP 中只出现文件本身，最简单可靠的方法是：

```bash
cd /srv/reports
zip /home/learner/reports.zip summary.txt detail.txt
```

### 核心原则

> ZIP 会保存你交给它的路径结构，因此创建归档前要先考虑“解压后希望看到什么目录结构”。

---

## 6. 使用 `unzip` 解压

基本语法：

```bash
unzip ARCHIVE.zip
```

例如：

```bash
unzip reports.zip
```

这会直接解压到当前目录。

更推荐使用：

```bash
unzip reports.zip -d TARGET_DIRECTORY
```

其中：

```text
-d = directory
```

表示指定解压目录。

例如：

```bash
unzip /home/learner/reports.zip -d /home/learner/unpacked
```

---

## 7. 为什么最好解压到单独目录

不要随意执行：

```bash
unzip unknown.zip
```

因为一个 ZIP 中可能包含：

```text
file1
file2
file3
...
几十个文件
```

如果直接解压到当前工作目录，大量文件可能散落其中。

更安全、更整洁的习惯是：

```bash
mkdir -p unpacked
unzip archive.zip -d unpacked
```

这样所有内容都会集中在：

```text
unpacked/
```

中。

对于来源不明或别人发来的归档，还可以先查看内容：

```bash
unzip -l archive.zip
```

再决定是否解压。

---

## 8. 完整操作示例

目标：

1. 将 `/srv/reports` 中的：

   * `summary.txt`
   * `detail.txt`
2. 压缩成：

```text
/home/learner/reports.zip
```

3. ZIP 中只保存裸文件名，不保存 `srv/reports/`。
4. 解压到：

```text
/home/learner/unpacked
```

5. 确认 `summary.txt` 解压后内容没有变化。

### 第一步：进入源目录

```bash
cd /srv/reports
```

### 第二步：创建 ZIP

```bash
zip /home/learner/reports.zip summary.txt detail.txt
```

ZIP 内部结构：

```text
reports.zip
├── summary.txt
└── detail.txt
```

可以确认：

```bash
unzip -l /home/learner/reports.zip
```

### 第三步：创建解压目录

```bash
mkdir -p /home/learner/unpacked
```

### 第四步：解压

```bash
unzip /home/learner/reports.zip -d /home/learner/unpacked
```

得到：

```text
/home/learner/unpacked/
├── summary.txt
└── detail.txt
```

### 第五步：确认文件内容完全一致

```bash
cmp /srv/reports/summary.txt /home/learner/unpacked/summary.txt
```

如果两个文件完全相同，`cmp` 默认不会输出任何内容，并且退出状态为 `0`。

也可以写成：

```bash
cmp /srv/reports/summary.txt /home/learner/unpacked/summary.txt && echo "summary.txt intact"
```

成功时输出：

```text
summary.txt intact
```

---

## 9. 常用 ZIP 命令速查

| 操作         | 命令                               |
| ---------- | -------------------------------- |
| 创建 ZIP     | `zip archive.zip file1 file2`    |
| 查看 ZIP 内容  | `unzip -l archive.zip`           |
| 解压到当前目录    | `unzip archive.zip`              |
| 解压到指定目录    | `unzip archive.zip -d directory` |
| 递归压缩整个目录   | `zip -r archive.zip directory/`  |
| 比较两个文件是否一致 | `cmp file1 file2`                |

例如压缩整个目录：

```bash
zip -r reports.zip reports/
```

其中：

```text
-r = recursive
```

表示递归处理目录中的子目录和文件。

---

## 10. ZIP、tar 与单文件压缩工具的职责对比

| 工具       | 多文件归档 | 压缩 | 常见用途            |
| -------- | ----: | -: | --------------- |
| `zip`    |     ✅ |  ✅ | 跨平台文件交换         |
| `tar`    |     ✅ |  ❌ | Unix/Linux 目录归档 |
| `gzip`   |     ❌ |  ✅ | 压缩单个文件          |
| `bzip2`  |     ❌ |  ✅ | 压缩单个文件          |
| `xz`     |     ❌ |  ✅ | 压缩单个文件          |
| `tar.gz` |     ✅ |  ✅ | `tar` + `gzip`  |
| `tar.xz` |     ✅ |  ✅ | `tar` + `xz`    |

最重要的区别可以记成：

```text
ZIP      = 归档 + 压缩，一个格式完成

tar.gz   = tar 负责归档
           gzip 负责压缩
```

而选择格式时，首先考虑的通常不是“哪个更高级”，而是：

> **最终接收方使用什么工具，以及归档需要在哪些操作系统之间流转。**
