# Bash 函数中的变量作用域：`local` 与动态作用域

## 1. Bash 函数默认不会隔离变量

在 Bash 中，函数并不会自动拥有独立的变量作用域。

例如：

```bash
#!/usr/bin/env bash

label=outer

helper() {
  label=inner
}

helper
printf '%s\n' "$label"
```

输出：

```text
inner
```

虽然 `label=inner` 写在函数内部，但它是普通赋值，因此修改了函数外部的 `label`。

也就是说：

```bash
label=outer

helper() {
  label=inner
}
```

执行 `helper` 后，外层变量已经变成：

```bash
label=inner
```

这种修改不会产生警告，因此很容易造成隐蔽的副作用。

---

## 2. 使用 `local` 创建函数局部变量

如果某个变量只应该在函数调用期间存在，应使用：

```bash
local 变量名=值
```

例如：

```bash
label=outer

helper() {
  local label=inner
  printf '%s\n' "$label"
}

helper
printf '%s\n' "$label"
```

输出：

```text
inner
outer
```

这里发生了两件事：

1. 进入 `helper` 后，创建了一个局部的 `label`
2. 函数返回后，外层原来的 `label=outer` 重新可见

因此：

```bash
local label=inner
```

不会覆盖外层变量。

常见写法是直接使用函数参数初始化：

```bash
helper() {
  local label="$1"
}
```

其中：

* `$1` 是当前函数收到的第一个参数
* `local label="$1"` 创建局部变量 `label`
* 双引号可以防止参数中的空格发生单词拆分

---

## 3. `local` 只能在函数内部使用

下面是正确用法：

```bash
helper() {
  local label="$1"
}
```

不能在脚本顶层写：

```bash
local label=outer
```

这会报错，因为 `local` 只适用于函数调用环境。

---

## 4. Bash 的 `local` 是动态作用域

Bash 一个容易误解的重要特性是：

> 函数中的 `local` 变量不仅当前函数能看到，该函数调用的其他函数也能看到。

例如：

```bash
peek() {
  printf '%s\n' "$label"
}

show() {
  local label=inner
  peek
}

label=outer
show
```

输出：

```text
inner
```

虽然 `peek`：

* 没有接收参数
* 没有声明 `label`
* 自己也没有赋值

但它仍然能读取 `show` 中的局部变量：

```bash
local label=inner
```

因为 `peek` 是在 `show` 的调用期间执行的。

这种规则称为 **动态作用域（dynamic scope）**。

可以把调用过程理解为：

```text
脚本
label=outer

└── show
    local label=inner

    └── peek
        读取 label → inner

show 返回

脚本继续
读取 label → outer
```

局部变量的有效范围并不仅仅取决于代码写在哪里，还取决于当前的函数调用链。

---

## 5. `local` 与普通赋值的区别

### 使用 `local`

```bash
show_sealed() {
  local label="$1"
  printf '%s\n' "$label"
}
```

调用：

```bash
label=outer
show_sealed inner
```

函数执行期间：

```text
label=inner
```

函数返回之后：

```text
label=outer
```

外部变量没有被修改。

---

### 不使用 `local`

```bash
show_leaky() {
  label="$1"
  printf '%s\n' "$label"
}
```

调用：

```bash
label=outer
show_leaky leaked
```

函数返回之后：

```text
label=leaked
```

因为：

```bash
label="$1"
```

只是普通的 Bash 变量赋值，没有建立新的局部变量。

---

## 6. 完整实验：比较局部变量和泄漏变量

创建：

```text
/home/learner/bash-lab/scope.sh
```

内容如下：

```bash
#!/usr/bin/env bash

peek() {
  printf '%s\n' "$label"
}

show_sealed() {
  local label="$1"
  printf '%s\n' "$label"
  peek
}

show_leaky() {
  label="$1"
  printf '%s\n' "$label"
}

label="$1"

show_sealed "$2" > /home/learner/bash-lab/scope.txt
printf '%s\n' "$label" >> /home/learner/bash-lab/scope.txt

show_leaky "$3" >> /home/learner/bash-lab/scope.txt
printf '%s\n' "$label" >> /home/learner/bash-lab/scope.txt

exit 0
```

运行：

```bash
cd /home/learner/bash-lab
bash scope.sh outer inner leaked
```

生成的：

```text
/home/learner/bash-lab/scope.txt
```

内容应为：

```text
inner
inner
outer
leaked
leaked
```

---

## 7. 五行输出分别说明什么

脚本参数为：

```bash
bash scope.sh outer inner leaked
```

因此：

```text
$1 = outer
$2 = inner
$3 = leaked
```

脚本首先执行：

```bash
label="$1"
```

因此全局变量为：

```text
label=outer
```

### 第 1 行

```text
inner
```

来自：

```bash
show_sealed "$2"
```

进入函数后：

```bash
local label="$1"
```

这里函数自己的 `$1` 是：

```text
inner
```

所以：

```text
label=inner
```

---

### 第 2 行

```text
inner
```

来自：

```bash
peek
```

`peek` 自己没有声明 `label`：

```bash
peek() {
  printf '%s\n' "$label"
}
```

但因为它是在 `show_sealed` 调用期间运行的，所以看到了：

```bash
show_sealed
└── local label=inner
    └── peek → inner
```

这证明 Bash 的函数局部变量具有动态作用域。

---

### 第 3 行

```text
outer
```

`show_sealed` 返回之后：

```bash
local label=inner
```

结束生命周期。

因此外层原来的：

```bash
label=outer
```

重新可见。

这证明 `local` 没有修改外层变量。

---

### 第 4 行

```text
leaked
```

执行：

```bash
show_leaky "$3"
```

其中：

```bash
show_leaky() {
  label="$1"
  printf '%s\n' "$label"
}
```

没有使用 `local`。

因此：

```bash
label=leaked
```

直接修改了外层变量。

---

### 第 5 行

```text
leaked
```

`show_leaky` 返回以后，脚本再次读取：

```bash
"$label"
```

得到的仍然是：

```text
leaked
```

说明普通函数赋值已经泄漏到了外层环境。

---

## 8. 为什么第一次使用 `>`，后面使用 `>>`

第一条重定向：

```bash
show_sealed "$2" > /home/learner/bash-lab/scope.txt
```

使用：

```text
>
```

含义是：

> 重新创建文件，并覆盖之前的内容。

因此每次执行脚本都从空文件开始。

后续使用：

```bash
>>
```

例如：

```bash
printf '%s\n' "$label" >> /home/learner/bash-lab/scope.txt
```

含义是：

> 将新内容追加到文件末尾。

如果所有地方都使用：

```bash
>>
```

重复运行脚本就会不断累积旧结果。

如果所有地方都使用：

```bash
>
```

后面的输出又会覆盖前面的内容。

---

## 9. `$1` 的含义取决于当前调用层级

脚本启动时：

```bash
bash scope.sh outer inner leaked
```

脚本级位置参数是：

```text
$1 = outer
$2 = inner
$3 = leaked
```

但执行：

```bash
show_sealed "$2"
```

之后，函数自己的参数变成：

```text
$1 = inner
```

所以：

```bash
show_sealed() {
  local label="$1"
}
```

这里的 `$1` 是函数参数 `inner`，不是脚本原来的 `outer`。

因此应记住：

> Bash 的位置参数属于当前调用环境。函数被调用时，函数自己的 `$1`、`$2` 等来自调用该函数时提供的参数。

---

## 10. 常见误区

### 误区一：函数里的变量天然是局部变量

错误：

```bash
helper() {
  label="$1"
}
```

这里的 `label` 并不天然属于 `helper`。

如果不希望影响外部变量，应写：

```bash
helper() {
  local label="$1"
}
```

---

### 误区二：被调用的函数看不到调用者的 `local` 变量

在许多语言中可能如此，但 Bash 不一样。

```bash
outer() {
  local name=Alice
  inner
}

inner() {
  printf '%s\n' "$name"
}
```

执行：

```bash
outer
```

会输出：

```text
Alice
```

因为 Bash 使用动态作用域。

---

### 误区三：`local` 变量只保护当前函数的一小段代码

它实际上在整个函数调用期间有效，并且能被该函数继续调用的函数读取。

其生命周期大致是：

```text
进入函数
↓
创建 local 变量
↓
当前函数使用
↓
被调用函数也可以访问
↓
函数返回
↓
局部变量消失
↓
恢复外层同名变量
```

---

## 11. 最佳实践

函数内部临时使用的变量，通常应该主动声明为：

```bash
local
```

例如：

```bash
process_file() {
  local filename="$1"
  local result
  local status
}
```

这样可以：

* 避免意外覆盖脚本级变量
* 降低函数之间互相影响的风险
* 让变量生命周期更清楚
* 让函数更容易独立理解和复用

尤其要警惕这种写法：

```bash
helper() {
  result=...
  name=...
  status=...
}
```

如果这些只是函数内部临时变量，更安全的形式是：

```bash
helper() {
  local result
  local name
  local status
}
```

---

## 12. 核心总结

Bash 函数变量作用域最重要的规则是：

```text
普通赋值
    ↓
可能修改外层变量

local 变量
    ↓
只在当前函数调用期间存在
    ↓
被该函数调用的其他函数也能看到
    ↓
函数返回后恢复外层同名变量
```

典型写法：

```bash
helper() {
  local label="$1"
}
```

最关键的对比：

```bash
label="$1"
```

表示普通赋值，可能影响外部状态；

而：

```bash
local label="$1"
```

表示创建函数局部变量，避免修改外层同名变量。

同时必须记住 Bash 的特殊之处：

> `local` 是动态作用域：当前函数创建的局部变量，在这个函数继续调用的其他函数中同样可见，直到当前函数返回。
