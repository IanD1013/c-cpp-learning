### 使用 `match`/`case` 进行结构化模式匹配

Python 3.10 引入了**结构化模式匹配**（structural pattern matching）——一种强大的控制流机制，它计算表达式并将其与一系列模式进行比对。与测试任意条件的链式 `if`/`elif` 代码块不同，`match`/`case` 以声明式的方式描述你预期的结构和值，使意图更加清晰，代码更具可读性。

### 工作原理

`match` 语句对**目标表达式**（subject expression）计算一次，然后**自上而下**检查每个 `case` 子句。与目标匹配的第一个模式会触发其对应的代码块。在该代码块执行之后，控制流将跳过整个 `match` 语句——这里**没有像 C 风格 `switch` 语句那样的贯穿（fall-through）**。如果没有匹配的模式且没有通配符，执行将直接继续运行 `match` 代码块之后的代码。

### 语法

```python
match subject_expression:
    case pattern_1:
        # runs if subject matches pattern_1
    case pattern_2:
        # runs if subject matches pattern_2
    case _:
        # wildcard — matches anything (default/fallback)
```

关键要点：

- 每个 `case` 测试的是一个**模式**（pattern），而不是布尔条件
- `case _:` 是**通配符模式**（wildcard pattern）——它匹配任何值，并充当默认分支（类似于 `else`）
- 每次 `match` 求值仅会执行一个 case 分支

### 示例

```python
# Literal pattern matching with integers
def describe_day(day_number):
    match day_number:
        case 1:
            return "Monday"
        case 2:
            return "Tuesday"
        case 3:
            return "Wednesday"
        case _:
            return "Other day"

describe_day(1)   # "Monday"
describe_day(99)  # "Other day"
```

```python
# Literal patterns with strings
def translate_color(color):
    match color:
        case "red":
            return "rojo"
        case "blue":
            return "azul"
        case "green":
            return "verde"
        case _:
            return "desconocido"

translate_color("blue")    # "azul"
translate_color("purple")  # "desconocido"
```

```python
# Combining match/case with additional logic
def evaluate_score(score):
    matched = True
    match score:
        case 100:
            label = "Perfect"
        case 0:
            label = "Zero"
        case _:
            label = "Unranked"
            matched = False
    return {"label": label, "was_ranked": matched}

evaluate_score(100)  # {"label": "Perfect", "was_ranked": True}
evaluate_score(42)   # {"label": "Unranked", "was_ranked": False}
```

### 常见模式

- **字面量模式**（Literal patterns）匹配精确值：整数（`case 42`）、字符串（`case "hello"`）、布尔值（`case True`）、`None`（`case None`）
- **通配符** `case _:` 应始终作为**最后一个 case**，因为它可以匹配任何内容
- 你可以在 `match` 代码块之前或内部设置变量，并在其完成后使用它们
- 由于没有 fall-through，每个 case 都是独立的——不需要 `break`