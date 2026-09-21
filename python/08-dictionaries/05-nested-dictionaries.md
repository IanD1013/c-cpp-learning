### 嵌套字典

Python 中的字典值可以是任何类型 —— 整数、字符串、列表，甚至是其他字典。当一个字典包含另一个字典作为其值时，就构成了**嵌套字典**。在处理来自 Web API 的 JSON、配置文件或层级记录等结构化数据时，这种模式非常常见。

### 工作原理

嵌套字典本质上就是另一个字典内部的字典。要访问结构深处的值，可以逐层**链式调用**方括号表示法或 `.get()`。每一步都会带你深入结构的一层。

风险在于，如果调用链中的任何一个键不存在，Python 就会引发 `KeyError`。更糟糕的是，如果中间值根本不是字典，在其上链式调用方括号会引发 `TypeError`。

### 语法

```python
# Creating a nested dictionary
profile = {
    "contact": {
        "email": "bob@example.com",
        "phone": "555-1234"
    },
    "preferences": {
        "theme": "dark",
        "language": "en"
    }
}

# Direct access (raises KeyError if missing)
value = profile["contact"]["email"]

# Safe access with .get() — returns None if missing
value = profile.get("contact", {}).get("email")
```

### 示例

```python
# Example 1: Accessing a nested value
server = {"db": {"host": "localhost", "port": 5432}}
print(server["db"]["host"])       # "localhost"
print(server["db"]["port"])       # 5432

# Example 2: Safe access when outer key is missing
print(server.get("cache"))        # None ("cache" key doesn't exist)
print(server.get("cache", {}).get("host"))  # None (safe chaining)

# Example 3: Safe access when inner key is missing
print(server.get("db", {}).get("password"))  # None ("password" not in inner dict)

# Example 4: Beware — the outer value might not be a dict!
mixed = {"name": "Alice", "scores": [90, 85]}
# mixed["name"]["first"] would raise TypeError because "Alice" is a string, not a dict
# mixed["scores"]["math"] would raise TypeError because [90, 85] is a list
```

### 常见模式

* **使用** **`.get()`** **进行安全链式调用**：如果任一键缺失，`d.get(key1, {}).get(key2)` 会返回 `None`，但它假设中间值始终是一个字典或完全缺失。
* **显式检查**：当需要在访问前确保结构正确时，使用 `if key1 in d and key2 in d[key1]`。
* **`isinstance()`** **检查**：当不确定中间值是否真的是字典时，`isinstance(value, dict)` 允许你在深入访问之前进行验证。
