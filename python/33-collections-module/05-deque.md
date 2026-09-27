### Deque —— 双端队列

Python 的 `collections.deque` 是一个高性能容器，专门针对**两端**的快速添加（append）和弹出（pop）操作进行了优化。虽然普通的 `list` 在右端进行 `append()` 和 `pop()` 能达到 O(1) 的时间复杂度，但从左端插入或删除（`list.insert(0, x)` 或 `list.pop(0)`）的开销为 O(n)，因为每个元素都必须移动。而 `deque` 在两端进行添加/弹出这四种操作时均提供 O(1) 的时间复杂度。

### 工作原理

在底层实现上，deque 是由固定大小块组成的双向链表。这意味着访问任意一端都是常数时间，不过按索引进行随机访问（`dq[3]`）的时间复杂度为 O(n)。可以把它想象成一副扑克牌，你可以立即从任意一侧发牌或向任意一侧加牌。

### 语法

```python
from collections import deque

# Create a deque
dq = deque()              # empty
dq = deque([1, 2, 3])     # from iterable
dq = deque(maxlen=5)      # bounded deque — auto-discards from opposite end when full
```

### 核心操作

| **方法** | **描述** | **时间复杂度** |
| :--- | :--- | :--- |
| `append(x)` | 添加到右端 | O(1) |
| `appendleft(x)` | 添加到左端 | O(1) |
| `pop()` | 从右端移除并返回 | O(1) |
| `popleft()` | 从左端移除并返回 | O(1) |
| `rotate(n)` | 向右旋转 n 步（负数表示向左） | O(k) |
| `maxlen` | 只读属性，表示最大容量 | — |

### 示例

```python
from collections import deque

# Basic operations
tasks = deque(["email", "report"])
tasks.appendleft("urgent_fix")    # deque(['urgent_fix', 'email', 'report'])
next_task = tasks.popleft()       # 'urgent_fix', deque(['email', 'report'])

# Rotation — like shifting a circular buffer
positions = deque([1, 2, 3, 4, 5])
positions.rotate(2)               # deque([4, 5, 1, 2, 3])
positions.rotate(-1)              # deque([5, 1, 2, 3, 4])

# Bounded deque — keeps only the last N items
recent_logs = deque(maxlen=3)
recent_logs.append("log_A")
recent_logs.append("log_B")
recent_logs.append("log_C")
recent_logs.append("log_D")       # deque(['log_B', 'log_C', 'log_D']) — 'log_A' auto-discarded
```

### 使用 Deque 作为索引追踪器

一种强大的模式是在 deque 中存储**索引**而非具体值，从而维护某种有序的不变量（invariant）。例如，为了追踪滚动 7 天周期内的最低温度，你可以维护一个日期索引的 deque，其中温度按升序排列。随着每个新日期的到来：

1. 如果队首的旧索引超出了窗口范围，则将其从**前端过期移除**
2. 从**后端驱逐**破坏有序不变量的索引
3. 将新索引**添加**到后端
4. 从前端**读取**答案

```python
# Conceptual example: rolling minimum over window of size 3
temps = [30, 25, 28, 22, 26]
# Window [30,25,28] -> min 25
# Window [25,28,22] -> min 22
# Window [28,22,26] -> min 22
# Result: [25, 22, 22]
```

这种模式对每个元素恰好处理一次（每个索引最多被添加和移除一次），无论窗口大小是多少，总时间复杂度都为 O(n) —— 远优于扫描每个窗口的 O(n\*k) 暴力方法。