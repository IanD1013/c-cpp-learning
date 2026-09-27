### 使用 `json.loads()` 解析 JSON 字符串

JSON（JavaScript Object Notation）是网络上最主要的数据交换格式。每当你的 Python 代码与 REST API 通信、读取配置文件或处理来自消息队列的数据时，你几乎肯定都在处理 JSON。Python 内置的 `json` 模块为你提供了在 JSON 字符串与原生 Python 对象之间无缝转换的工具。

### 工作原理

`json` 模块是 Python 标准库的一部分——只需 `import json` 即可使用。用于解析的核心函数是 `json.loads()`（“load string”，加载字符串），它接收一个 JSON 格式的字符串并返回等效的 Python 数据结构。

JSON 和 Python 类型会自然地相互映射：

| **JSON 类型** | **Python 类型** | **示例 JSON** | **Python 结果** |
| :--- | :--- | :--- | :--- |
| object | `dict` | `{"a": 1}` | `{'a': 1}` |
| array | `list` | `[1, 2, 3]` | `[1, 2, 3]` |
| string | `str` | `"hello"` | `'hello'` |
| number (int) | `int` | `42` | `42` |
| number (float) | `float` | `3.14` | `3.14` |
| true / false | `True` / `False` | `true` | `True` |
| null | `None` | `null` | `None` |

### 语法

```python
import json

result = json.loads(json_string)
```

`json_string` 必须是格式有效的 JSON 字符串。如果不是，则会引发 `json.JSONDecodeError`。

### 示例

```python
import json

# Parsing a JSON object into a dict
server_config = json.loads('{"host": "localhost", "port": 8080}')
print(server_config["host"])   # "localhost"
print(server_config["port"])   # 8080
print(type(server_config))     # <class 'dict'>

# Parsing a JSON array into a list
colors = json.loads('["red", "green", "blue"]')
print(colors[0])               # "red"
print(len(colors))             # 3

# Parsing nested structures
response = json.loads('{"user": {"id": 7, "active": true}, "tags": ["admin"]}')
print(response["user"]["active"])  # True (Python bool, not JSON "true")
print(response["tags"])            # ['admin']

# JSON null becomes Python None
profile = json.loads('{"bio": null, "score": 0}')
print(profile["bio"])          # None
print(profile["bio"] is None)  # True
```

### 常用模式

一旦 `json.loads()` 返回了一个 dict，你就可以像处理任何其他 Python 字典一样使用它。两种特别有用的技巧：

- **`dict[key]`**：如果键不存在，会引发 `KeyError`。
- **`dict.get(key, default)`**：如果键不存在，则返回 `default`（默认为 `None`）——不会引发异常。

```python
data = json.loads('{"city": "Berlin"}')
print(data.get("city"))       # "Berlin"
print(data.get("country"))    # None — key missing, no error
print(data.get("country", "Unknown"))  # "Unknown" — custom default
```