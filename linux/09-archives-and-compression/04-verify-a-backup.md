# 备份验证：校验和与恢复测试

## 核心原则：没有恢复过的归档，还不能算经过验证的备份

备份的真正目标不是“生成一个压缩文件”，而是：

> **能够把保存下来的字节重新恢复成原来的目录和文件。**

例如，`course-backup.tar.gz` 看起来大小正常，并不能证明它真的可用。很多问题都可能产生一个“看起来合理”的归档：

* 文件传输中途被截断；
* 写入时磁盘空间耗尽；
* 原目录在打包前就缺少文件；
* 归档本身损坏；
* 打包时选错了目录或路径。

因此，一份可靠的备份至少需要两类验证：

1. **Checksum（校验和）**：验证归档文件之后有没有发生变化或损坏。
2. **Restore test（恢复测试）**：验证归档是否真的能恢复出需要的数据。

这两种验证解决的是不同问题，不能互相替代。

---

## 1. 创建归档备份

假设要备份：

```text
/srv/course-data
```

目标归档为：

```text
/home/learner/course-backup.tar.gz
```

可以执行：

```bash
tar -czf /home/learner/course-backup.tar.gz -C /srv course-data
```

参数含义：

* `-c`：创建归档；
* `-z`：使用 gzip 压缩；
* `-f`：指定归档文件名；
* `-C /srv`：先进入 `/srv`；
* `course-data`：归档这个目录。

这样归档内部保存的路径类似：

```text
course-data/
course-data/README
course-data/...
```

而不是保存完整的：

```text
/srv/course-data/...
```

使用相对路径通常更适合恢复，因为归档不会绑定到原机器上的绝对路径。

---

## 2. 用 SHA-256 校验归档文件

生成校验和：

```bash
sha256sum /home/learner/course-backup.tar.gz
```

输出格式类似：

```text
8a3f...e912  /home/learner/course-backup.tar.gz
```

SHA-256 摘要是一个 **64 个十六进制字符**的值。只要归档中的任意一个字节发生变化，计算出来的摘要通常就会完全不同。

为了以后能够验证，应把输出保存下来：

```bash
sha256sum /home/learner/course-backup.tar.gz \
  > /home/learner/course-backup.sha256
```

最终：

```text
/home/learner/course-backup.sha256
```

会保存类似：

```text
8a3f...e912  /home/learner/course-backup.tar.gz
```

### 为什么要校验归档，而不是只校验源文件？

因为真正需要长期保存和传输的是：

```text
course-backup.tar.gz
```

校验和要证明的是：

> **现在手里的这个归档文件，是否仍然和当初创建出来的那个归档完全一致。**

以后可以执行：

```bash
sha256sum -c /home/learner/course-backup.sha256
```

如果文件没有变化，会得到类似：

```text
/home/learner/course-backup.tar.gz: OK
```

---

## 3. 校验和能证明什么？

SHA-256 可以发现：

* 网络传输过程中发生的损坏；
* 磁盘上的 bit corruption；
* 文件被截断；
* 文件内容被修改。

它回答的问题是：

> **“现在这个归档文件，还是不是之前那个归档文件？”**

---

## 4. 校验和不能证明什么？

Checksum 并不能证明归档内容本来就是正确的。

例如，假设创建备份时误操作：

```bash
tar -czf backup.tar.gz wrong-directory
```

之后执行：

```bash
sha256sum backup.tar.gz
```

完全可以得到一个合法的 SHA-256。

即使十年后再次验证仍然显示：

```text
backup.tar.gz: OK
```

也只能说明：

> 错误的归档从创建以来没有变化。

它不能证明：

> 当初归档进去的数据就是正确的。

因此：

```text
checksum 正确
```

不等于：

```text
backup 正确
```

---

## 5. 恢复测试才验证“能不能还原”

验证备份时，不要直接覆盖原目录，而应该恢复到一个临时目录，例如：

```bash
mkdir -p /home/learner/verify
```

然后解压：

```bash
tar -xzf /home/learner/course-backup.tar.gz \
  -C /home/learner/verify \
  --strip-components=1
```

参数含义：

* `-x`：解包；
* `-z`：处理 gzip 压缩；
* `-f`：指定归档文件；
* `-C /home/learner/verify`：恢复到验证目录；
* `--strip-components=1`：去掉归档路径最前面的一级目录。

---

## 6. `--strip-components=1` 的作用

假设归档内部结构是：

```text
course-data/
course-data/README
course-data/config.ini
```

如果普通解压：

```bash
tar -xzf course-backup.tar.gz -C /home/learner/verify
```

结果会是：

```text
/home/learner/verify/
└── course-data/
    ├── README
    └── config.ini
```

但如果加上：

```bash
--strip-components=1
```

最前面的 `course-data/` 会被去掉：

```text
/home/learner/verify/
├── README
└── config.ini
```

因此：

```text
course-data/README
```

会直接恢复成：

```text
/home/learner/verify/README
```

### 可以把它理解为

```text
归档路径：
course-data/README

去掉 1 层：
README
```

如果是：

```text
a/b/file.txt
```

那么：

```bash
--strip-components=1
```

得到：

```text
b/file.txt
```

而：

```bash
--strip-components=2
```

得到：

```text
file.txt
```

---

## 7. 恢复之后必须实际读取数据

仅仅“`tar` 命令没有报错”仍然不是最强的验证。

应该从恢复后的目录读取一个已知文件，例如：

```bash
cat /home/learner/verify/README
```

这样验证的是：

```text
归档文件
    ↓
成功解压
    ↓
恢复出目录和文件
    ↓
文件内容可以实际读取
```

这才真正接近备份的最终目的。

特别重要的是：验证目录中的文件必须来自**归档恢复**，而不能直接从源目录复制。例如：

```bash
cp /srv/course-data/README /home/learner/verify/README
```

不能算恢复测试，因为它绕过了备份文件本身。

---

## 8. 一套完整的备份验证流程

```bash
# 1. 创建压缩归档
tar -czf /home/learner/course-backup.tar.gz \
  -C /srv course-data

# 2. 保存归档的 SHA-256
sha256sum /home/learner/course-backup.tar.gz \
  > /home/learner/course-backup.sha256

# 3. 创建独立的恢复测试目录
mkdir -p /home/learner/verify

# 4. 从归档恢复数据
tar -xzf /home/learner/course-backup.tar.gz \
  -C /home/learner/verify \
  --strip-components=1

# 5. 实际读取恢复出的文件
cat /home/learner/verify/README
```

最终应满足：

```text
/home/learner/course-backup.tar.gz
```

存在，并包含 `/srv/course-data` 的内容。

```text
/home/learner/course-backup.sha256
```

保存 `sha256sum` 对归档文件产生的原始输出。

```text
/home/learner/verify/README
```

来自备份归档的实际恢复，而不是从源目录直接复制。

---

## 9. Checksum 与 Restore Test 的区别

| 验证方式             | 回答的问题      | 能发现的问题              | 不能证明        |
| ---------------- | ---------- | ------------------- | ----------- |
| SHA-256 checksum | 归档后来有没有变化？ | 截断、损坏、传输错误、内容修改     | 当初备份的数据是否正确 |
| Restore test     | 归档能不能真正恢复？ | 无法解压、路径错误、缺文件、内容不可读 | 归档未来是否会被损坏  |

因此完整思路是：

```text
创建备份
   ↓
计算 checksum
   ↓
恢复到独立目录
   ↓
读取并检查恢复后的数据
```

---

## 常见误区

### 误区 1：文件存在就说明备份成功

错误：

```bash
ls -lh backup.tar.gz
```

看到文件存在只能说明：

> 某个文件被创建出来了。

不能证明它包含正确数据，也不能证明它可以恢复。

---

### 误区 2：`sha256sum` 显示 OK 就证明备份正确

不正确。

```bash
sha256sum -c backup.sha256
```

显示：

```text
backup.tar.gz: OK
```

只证明归档字节与计算 checksum 时一致。

如果当初归档内容就是错的，checksum 仍然可能完全正确。

---

### 误区 3：查看归档列表等于恢复测试

可以用：

```bash
tar -tzf backup.tar.gz
```

查看文件列表，这很有用，但仍然弱于真正恢复。

它只能证明 tar 能读取归档目录结构，不能完整证明：

```text
文件能够成功解压并实际使用
```

因此重要备份应真正恢复到临时目录进行检查。

---

### 误区 4：验证时直接解压回原目录

例如：

```bash
tar -xzf backup.tar.gz -C /srv
```

风险较大，因为可能：

* 覆盖现有文件；
* 修改正在使用的数据；
* 混合“原数据”和“恢复数据”，导致无法判断哪些文件真正来自备份。

更好的方式是使用独立目录：

```text
/home/learner/verify
```

---

## 最佳实践

一个可靠的备份流程应同时考虑三个问题：

1. **有没有备份？**

   * 确认归档文件存在。

2. **备份文件有没有被破坏？**

   * 使用 SHA-256 等 checksum。

3. **备份能不能真的恢复？**

   * 定期恢复到独立目录并检查文件内容。

最值得记住的一句话是：

> **校验和验证“这些字节有没有变化”，恢复测试验证“这些字节是不是一个真正可用的备份”。**
