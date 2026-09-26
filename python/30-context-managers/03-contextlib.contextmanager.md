### `@contextmanager` 装饰器

你已经知道如何通过编写带有 `__enter__` 和 `__exit__` 的类来构建上下文管理器。这种方法虽然可行，但在许多使用场景下，样板代码比实际需要的要多。`contextlib` 模块提供了 `@contextmanager`，这是一个装饰器，可让你将一个简单的生成器函数转换为功能齐全的上下文管理器。当基于类的方法显得过于繁琐时，这是有经验的 Python 开发者常用的惯用捷径。

### 工作原理

使用 `@contextmanager` 装饰的生成器必须且只能 `yield` 一次。该装饰器在 `yield` 处将你的生成器拆分为两部分：

1. **`yield` 之前的所有内容**在进入 `with` 块时执行（“setup”阶段）。
2. **生成出的值**（如果有的话）会绑定到 `as` 后面的变量。
3. **`yield` 之后的所有内容**在退出 `with` 块时执行（“teardown”阶段），即使块内部发生了异常也是如此。

为了确保无论是否发生异常 teardown 都能执行，请将 `yield` 包裹在 `try`/`finally` 块中。

### 语法

```python
from contextlib import contextmanager

@contextmanager
def managed_resource(args):
    # setup code
    resource = acquire(args)
    try:
        yield resource  # or yield with no value
    finally:
        # teardown code
        release(resource)
```

用法与任何其他上下文管理器完全相同：

```python
with managed_resource(some_args) as r:
    # r is whatever was yielded
    do_something(r)
# teardown has already run here
```

### 示例

**示例 1 — 统计代码块的运行时间：**

```python
import time
from contextlib import contextmanager

@contextmanager
def timer(label):
    start = time.perf_counter()
    try:
        yield
    finally:
        elapsed = time.perf_counter() - start
        print(f"{label}: {elapsed:.4f}s")

with timer("sort"):
    sorted(range(1_000_000, 0, -1))
# prints something like "sort: 0.0732s"
```

注意这里的 `yield` 没有返回值——调用方并不需要。`finally` 块确保即使代码主体抛出异常，耗费的时间也会被打印出来。

**示例 2 — 临时修改全局设置：**

```python
from contextlib import contextmanager

config = {"debug": False, "log_level": "INFO"}

@contextmanager
def override_config(key, new_value):
    old_value = config[key]
    config[key] = new_value
    try:
        yield old_value  # yield the previous value for inspection
    finally:
        config[key] = old_value

with override_config("debug", True) as was:
    print(config["debug"])   # True
    print(was)               # False
print(config["debug"])       # False — restored
```

在这里，yield 返回的值让调用方可以查看被修改前的设置。

**示例 3 — 确保即使发生错误也能清理：**

```python
from contextlib import contextmanager

@contextmanager
def sandbox_directory(path):
    import os, tempfile, shutil
    tmp = tempfile.mkdtemp(dir=path)
    try:
        yield tmp
    finally:
        shutil.rmtree(tmp)

# The temporary directory is always cleaned up
```

### 常见模式

| 模式 | yield 之前 | yield 之后 |
| --- | --- | --- |
| 资源管理 | 打开/获取 | 关闭/释放 |
| 状态切换 | 保存并替换 | 恢复 |
| 日志 / 计时 | 记录开始 | 记录结束 |
| 测试中的 Mock | 打补丁 | 取消补丁 |

“保存、替换、恢复”模式在测试和配置管理中尤为常见。
