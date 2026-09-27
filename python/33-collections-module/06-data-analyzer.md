# Collections 模块综合实践

Python 的 `collections` 模块提供了专用的容器类型，其功能远超基础的 `list` 和 `dict`。在真实世界的数据处理中，你经常需要在同一个流水线中统计出现次数、对项目进行分组、保持插入顺序、限制缓冲区大小以及清晰地结构化数据。本综合练习融合了五个关键的 `collections` 工具。

## 各工具的工作原理

**`namedtuple`** 用于创建带有具名字段的轻量级不可变类。每个实例的行为都类似于 `tuple`，但允许你通过名称访问字段。`_asdict()` 方法可以将 `namedtuple` 转换为字典。

**`defaultdict`** 的工作方式类似于普通 `dict`，但会使用工厂函数自动创建缺失的键——在追加元素前无需再检查 `if key in dict`。

**`Counter`** 是用于统计可哈希对象的 `dict` 子类。它的 `most_common(n)` 方法返回按出现次数降序排列的前 `n` 个最高频元素，表示为 `(element, count)` 元组。

**`deque`** 是双端队列。当你设置了 `maxlen` 时，队列满后它会自动从另一端丢弃元素——非常适合用作“前 N 项”缓冲区。

## Syntax

```python
from collections import namedtuple, defaultdict, Counter, deque

# namedtuple
Point = namedtuple('Point', ['x', 'y'])
p = Point(3, 4)
print(p.x)          # 3
print(p._asdict())  # {'x': 3, 'y': 4}

# defaultdict
groups = defaultdict(list)
groups['a'].append(1)  # No KeyError — auto-creates []

# Counter
c = Counter(['red', 'blue', 'red', 'green'])
c.most_common(2)  # [('red', 2), ('blue', 1)]

# deque with maxlen
buf = deque(maxlen=2)
buf.append('x')
buf.append('y')
buf.append('z')  # 'x' is dropped
print(buf)  # deque(['y', 'z'], maxlen=2)
```

## Examples

```python
# Grouping scores by student
scores = defaultdict(list)
for student, score in [('Ana', 88), ('Ben', 92), ('Ana', 95)]:
    scores[student].append(score)
# scores -> {'Ana': [88, 95], 'Ben': [92]}

# Counting colors
colors = Counter(['red', 'blue', 'red', 'red', 'green'])
colors.most_common()  # [('red', 3), ('blue', 1), ('green', 1)]

# Converting namedtuples to dicts
City = namedtuple('City', ['name', 'pop'])
c = City('Oslo', 700000)
dict(c._asdict())  # {'name': 'Oslo', 'pop': 700000}

# Keeping only the last 2 log entries
logs = deque(maxlen=2)
for msg in ['boot', 'login', 'query', 'logout']:
    logs.append(msg)
# logs -> deque(['query', 'logout'], maxlen=2)
```

## 常用模式

- 使用 `Record(*tuple_data)` 将原始元组转换为 `namedtuple`，以便更清晰地访问字段
- 使用 `defaultdict(list)` 累积值，然后计算每个分组的聚合数据
- 可以通过 `counter[key] += 1` 手动递增 `Counter`，也可以直接从可迭代对象构建
- 在填充 `deque(maxlen=N)` 之前对记录进行排序，以获取前 N 项
- 在较早的 Python 版本中 `_asdict()` 会返回 `OrderedDict`；使用 `dict()` 进行包装即可获得普通字典