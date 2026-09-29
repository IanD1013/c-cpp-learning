### 结构化模式匹配综合实践

Python 的 `match`/`case` 语句（在 3.10 中引入）是一个强大的工具，用于根据数据的结构进行分解和路由。在实际应用中，您经常会在单个 match 代码块中结合字面量值、捕获变量、OR 备选模式、守卫条件以及序列解包，以构建富有表现力且易读的解析器和分发器。

这个综合实践项目将你学到的所有模式匹配特性结合到一个完整的命令解析器中。

### 工作原理

当你编写 `match tokens:` 时，Python 会按顺序评估每个 `case`，测试主体是否与模式匹配。第一个匹配的 case 会被执行。模式可以：

- **匹配字面量**：如 `"start"` 或 `42` 这样的精确值
- **捕获变量**：绑定到部分数据的变量名
- **使用 OR 模式**：`pattern1 | pattern2` 匹配其中任意一个
- **应用守卫**：`case [...] if condition:` 添加运行时检查
- **解包序列**：`[first, *rest]` 捕获可变长度列表
- **通配符**：`_` 匹配任意内容且不进行绑定

### 语法

```python
match subject:
    case pattern1:
        # runs if subject matches pattern1
    case pattern2 if guard_condition:
        # runs if pattern2 matches AND guard is true
    case _:
        # default fallback
```

### 示例

```python
# OR pattern: match multiple literals
match direction:
    case "north" | "south" | "east" | "west":
        print("Cardinal direction")

# Sequence unpacking with capture
match command_tokens:
    case ["send", recipient, *message_parts]:
        full_message = " ".join(message_parts)
        print(f"Sending to {recipient}: {full_message}")

# Guard clause for validation
match values:
    case ["divide", x, y] if int(y) != 0:
        print(int(x) / int(y))
    case ["divide", _, _]:
        print("Cannot divide by zero")

# Combining patterns in a router
def handle_event(event):
    match event.split():
        case ["login" | "signin", username]:
            return f"Welcome, {username}"
        case ["search", *keywords] if len(keywords) > 0:
            return f"Searching for: {' '.join(keywords)}"
        case _:
            return "Unknown event"
```

### 实用参考

| **模式类型** | **语法**                 | **用途**   |
| :------- | :--------------------- | :------- |
| 字面量      | `case "value":`        | 匹配精确值    |
| 捕获       | `case [cmd, name]:`    | 绑定变量     |
| OR       | `case "a" \| "b":`     | 匹配备选模式   |
| 守卫       | `case [...] if cond:`  | 添加运行时检查  |
| 序列       | `case [first, *rest]:` | 解包可变长度序列 |
| 通配符      | `case _:`              | 默认/兜底匹配  |