### 访问和修改字典值

字典用于存储键值对，但仅存储数据是不够的，你还需要能够检索和更新它。在实际应用程序中，你经常需要通过键来查找配置设置、用户个人资料和缓存结果。了解如何安全地访问值（尤其是当键可能不存在时）可以防止程序崩溃，并使代码更加健壮。

### 工作原理

你可以使用**方括号表示法**配合键来访问字典中的值：

```python
user = {"name": "Alice", "age": 30}
print(user["name"])  # Alice
```

如果你尝试访问一个不存在的键，Python 会引发 `KeyError`：

```python
print(user["email"])  # KeyError: 'email'
```

这就是 `.get()` 大显身手的地方。它尝试检索一个值，但**绝不会引发错误**——而是返回你指定的后备默认值：

```python
user.get("email", "not provided")  # returns "not provided"
```

如果你省略 `.get()` 的第二个参数，当键不存在时它会返回 `None`：

```python
user.get("email")  # returns None
```

### 修改和添加条目

要更新现有值或添加新的键值对，可以使用方括号进行赋值：

```python
user["age"] = 31          # updates existing key
user["email"] = "a@b.com" # adds new key
```

### 语法

```python
# Access by key (raises KeyError if missing)
value = my_dict[key]

# Safe access with fallback
value = my_dict.get(key, fallback_value)

# Add or update
my_dict[key] = new_value
```

### 示例

```python
scores = {"math": 95, "science": 88}

# Direct access - key exists
scores["math"]              # 95

# Safe access - key exists, returns actual value
scores.get("science", 0)    # 88

# Safe access - key missing, returns fallback
scores.get("history", 0)    # 0

# Check membership before accessing
if "art" in scores:
    print(scores["art"])
else:
    print("No art score")   # prints: No art score

# Modify and add
scores["math"] = 97         # updated
scores["art"] = 76          # added
```

### 常见模式

| **模式描述**              |                            |
| --------------------- | -------------------------- |
| `d[key]`              | 直接访问——速度快，但如果键可能不存在则有风险    |
| `d.get(key)`          | 如果键不存在则返回 `None`           |
| `d.get(key, default)` | 如果键不存在则返回 `default`        |
| `key in d`            | 检查键是否存在（返回 `True`/`False`） |
| `d[key] = value`      | 添加新键或覆盖现有值                 |
