### 遍历深层嵌套的 JSON 结构

实际应用中的 API 和配置文件很少返回扁平的 JSON。相反，你通常会遇到深层嵌套的结构：字典中嵌套字典、对象列表以及各种任意组合。要可靠地提取埋藏在多层深处的值，需要一步一步仔细遍历该结构，并在进入下一层之前验证每一层的类型。

### 工作原理

可以将嵌套的 JSON 想象成一棵树。要到达叶子节点，需要沿着一条**路径（path）**前进——即一系列键（针对字典）和索引（针对列表）。在每一步中，你必须验证当前节点的类型是否与下一步所需的操作相匹配：

- 如果下一步是一个**字符串键（string key）**，则当前节点必须是包含该键的字典。
- 如果下一步是一个**整数索引（integer index）**，则当前节点必须是列表，且该索引在有效范围内。

如果任何一步失败，则路径无效，遍历应平稳终止而不是抛出异常。

### 语法

```python
import json

# Parse a JSON string into Python objects
data = json.loads(json_string)

# Access nested dictionary keys
value = data["level1"]["level2"]["level3"]

# Access list elements within nested structures
item = data["results"][0]["name"]

# Safe checking before access
if isinstance(node, dict) and "key" in node:
    node = node["key"]

if isinstance(node, list) and 0 <= index < len(node):
    node = node[index]
```

### 示例

以天气 API 的响应为例：

```python
import json

weather_json = '{"forecast": {"daily": [{"temp": 72}, {"temp": 68}]}}'
weather = json.loads(weather_json)

# Direct chained access (risky — raises KeyError/IndexError if missing)
temp_day1 = weather["forecast"]["daily"][0]["temp"]  # 72

# Safe traversal pattern
def safe_get(data, path):
    current = data
    for step in path:
        if isinstance(step, str) and isinstance(current, dict) and step in current:
            current = current[step]
        elif isinstance(step, int) and isinstance(current, list) and 0 <= step < len(current):
            current = current[step]
        else:
            return None  # Path is invalid
    return current

# Works even when keys are missing
safe_get(weather, ["forecast", "daily", 1, "temp"])     # 68
safe_get(weather, ["forecast", "hourly", 0, "temp"])    # None ("hourly" doesn't exist)
safe_get(weather, ["forecast", "daily", 5, "temp"])     # None (index out of range)
```

另一个用户资料的示例：

```python
profile_json = '{"user": {"addresses": [{"city": "Portland"}, {"city": "Seattle"}], "age": 30}}'
profile = json.loads(profile_json)

safe_get(profile, ["user", "addresses", 1, "city"])  # "Seattle"
safe_get(profile, ["user", "age"])                    # 30
safe_get(profile, ["user", "phone"])                  # None
safe_get(profile, ["user", "age", "value"])           # None (30 is int, not dict)
```

### 常见陷阱

| **陷阱** | **产生的结果** | **预防措施** |
| :--- | :--- | :--- |
| 字典中缺少键 | 抛出 `KeyError` | 先检查 `key in dict` |
| 索引越界 | 抛出 `IndexError` | 先检查 `0 <= idx < len(list)` |
| 某一层的类型错误 | 抛出 `TypeError` | 使用 `isinstance()` 验证类型 |
| 无效的 JSON 字符串 | 抛出 `json.JSONDecodeError` | 将 `json.loads()` 包裹在 try/except 中 |
| 对字典使用整数索引 | 抛出 `TypeError` | 验证容器类型是否与键的类型匹配 |