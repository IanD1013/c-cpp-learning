# 在 `match/case` 中使用 `|` 的 OR 模式

在使用结构化模式匹配时，你经常会发现多个不同的值应该触发完全相同的行为。

与其重复编写 `case` 子句或退回到冗长的 `if/elif` 链，Python 的 OR 模式允许你使用 `|` 运算符将多个备选项合并到一个 `case` 分支中。

这既能保持代码简洁，又能清晰地表达意图：

> “这些值中的任何一个都应该以相同的方式处理。”

## 工作原理

`case` 子句中的 `|` 运算符表示：

> “匹配此模式 **或** 彼模式。”

Python 会从左到右针对每个备选项评估目标对象，一旦有任何一个匹配成功，就会进入该分支。

用 `|` 分隔的每个备选项都必须是一个独立的模式——你可以组合字面量、通配符 `_` 等等。

一个重要规则：

> 如果任何一个备选项使用了捕获变量，那么该 OR 模式中的**每个**备选项都必须绑定**完全相同的一组变量名**（或者都不绑定任何变量）。

对于简单的字面量匹配，因为没有捕获任何变量，所以很自然地满足这一规则。

## 语法

```python
match subject:
    case pattern_a | pattern_b | pattern_c:
        # runs if subject matches any of pattern_a, pattern_b, or pattern_c
    case _:
        # wildcard fallback
```

## 示例

考虑一个对 HTTP 状态码进行分类的函数：

```python
def status_category(code):
    match code:
        case 200 | 201 | 204:
            return "success"
        case 301 | 302 | 307:
            return "redirect"
        case 400 | 401 | 403 | 404:
            return "client error"
        case 500 | 502 | 503:
            return "server error"
        case _:
            return "unknown"
```

在这里：

```python
case 200 | 201 | 204:
```

会匹配这三个整数中的任意一个。

如果没有 OR 模式，你将需要三个独立的 `case` 子句或一个：

```python
if code in (200, 201, 204)
```

检查。

另一个示例——分类用户输入的命令：

```python
def parse_command(cmd):
    match cmd.strip().lower():
        case "quit" | "exit" | "q":
            return "shutdown"
        case "help" | "h" | "?":
            return "show_help"
        case "save" | "s":
            return "save_file"
        case _:
            return "unknown_command"
```

请注意输入是如何在进入 `match` 语句**之前**被规范化（去除首尾空格并转换为小写）的。

这是一个常见且有效的模式：

> 规范化一次，然后针对清晰规整的值进行匹配。

## 常见模式

- **先规范化输入**：在进入 `match` 块之前将字符串转换为小写（或去除首尾空格），这样你的模式就只需要处理一种大小写形式。
- **使用 `_` 作为全捕获（catch-all）**：始终包含一个通配符 case，以优雅地处理非预期的输入。
- **返回结构化数据**：将 `match/case` 与字典或元组返回值相结合，通过单个函数提供丰富的结果。
- **避免在 OR 模式中捕获变量**，除非每个备选项捕获相同的变量名——对于直接的 OR 匹配，请尽量使用字面量。