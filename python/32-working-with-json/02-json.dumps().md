# 使用 `json.dumps()` 将 Python 对象序列化为 JSON

虽然 `json.loads()` 可以将 JSON 字符串转换为 Python 对象，但 `json.dumps()` 的作用恰恰相反 —— 它将 Python 对象**序列化（serialize）**为 JSON 格式的字符串。

当你需要通过网络发送数据、编写配置文件、记录结构化数据日志或与需要 JSON 的 API 进行通信时，这都是必不可少的。

## 工作原理

`json.dumps()` 接收一个 Python 对象并将其转换为对应的 JSON 字符串表示形式。

其映射关系非常直观：

| Python Type | JSON Type |
| :--- | :--- |
| `dict` | object |
| `list` / `tuple` | array |
| `str` | string |
| `int` / `float` | number |
| `True` / `False` | true / false |
| `None` | null |

超出此集合的类型（如 `datetime`、`set` 或自定义类）将引发 `TypeError`，除非你提供了自定义编码器 —— 但这超出了本练习的范围。

## 语法

```python
import json

json_string = json.dumps(obj)
json_string = json.dumps(obj, indent=N)
json_string = json.dumps(obj, sort_keys=True)
json_string = json.dumps(obj, indent=N, sort_keys=True)
```

- **`obj`**：要序列化的 Python 对象。
- **`indent`**：当设置为整数 `N` 时，输出将进行美化打印（pretty-print），每层缩进 `N` 个空格。如果不设置该参数，输出将是紧凑的（单行）。
- **`sort_keys`**：当为 `True` 时，输出中的字典键将按字母顺序排序。默认情况下，键按插入顺序显示。

## 示例

```python
import json

# Compact output (default)
user = {"name": "Mia", "age": 28}
print(json.dumps(user))
# {"name": "Mia", "age": 28}

# Pretty-printed with 4-space indent
config = {"debug": True, "version": 3}
print(json.dumps(config, indent=4))
# {
#     "debug": true,
#     "version": 3
# }

# Sorted keys for deterministic output
settings = {"zoom": 1.5, "theme": "dark", "auto_save": False}
print(json.dumps(settings, sort_keys=True, indent=3))
# {
#    "auto_save": false,
#    "theme": "dark",
#    "zoom": 1.5
# }
```

注意 Python 的 `True` / `False` / `None` 在输出中是如何变为 JSON 的 `true` / `false` / `null` 的。

还要注意，每个嵌套层级都会添加指定数量的空格。

## 常见模式

当结合使用 `indent` 和 `sort_keys` 时，你可以获得既易于人类阅读又具有确定性的输出 —— 这两个特性对于配置文件、测试快照以及调试来说非常宝贵：

```python
# Nested structure
project = {
    "name": "Atlas",
    "tags": ["python", "api"],
    "meta": {"stars": 42}
}

print(json.dumps(project, indent=2, sort_keys=True))

# {
#   "meta": {
#     "stars": 42
#   },
#   "name": "Atlas",
#   "tags": [
#     "python",
#     "api"
#   ]
# }
```

键排序会递归应用于所有嵌套字典，且缩进适用于每一层嵌套。