# Python 中的类型别名

随着类型注解变得越来越复杂，它们可能会变得难以阅读和维护。类型别名允许你为复杂的类型赋予一个有意义的名称，从而使函数签名更清晰，代码库更一致。在嵌套类型很常见的数据处理、配置管理和 API 设计等领域中，它们尤为有用。

## 工作原理

类型别名本质上就是一个变量赋值，其中右侧是一个类型表达式。Python 将该变量与其引用的类型视为可互换的。任何你可以编写完整类型的地方，都可以改为编写别名。这不会改变运行时行为 —— 它纯粹是为了提高可读性和文档性。

从 Python 3.12 开始，还引入了专门的 `type` 语句（`type Vector = list[float]`），但赋值形式适用于所有现代 Python 版本（3.9+）。

## 语法

```python
# Simple assignment form (works in Python 3.9+)
AliasName = some_complex_type

# Python 3.12+ explicit type statement
type AliasName = some_complex_type
```

## 示例

```python
# Without aliases — hard to read
def analyze(data: list[dict[str, list[tuple[float, float]]]]) -> dict[str, float]:
    ...

# With aliases — much clearer
Coordinate = tuple[float, float]
Path = list[Coordinate]
RegionData = dict[str, Path]

def analyze(data: list[RegionData]) -> dict[str, float]:
    ...
```

```python
# Building aliases step by step for a game board
Cell = str
BoardRow = list[Cell]
GameBoard = list[BoardRow]
Score = int

def evaluate_board(board: GameBoard) -> Score:
    total = 0
    for row in board:
        for cell in row:
            if cell == "X":
                total += 1
    return total
```

```python
# Aliases for an address book
PhoneNumber = str
ContactInfo = dict[str, PhoneNumber]
AddressBook = list[ContactInfo]

def find_contact(book: AddressBook, name: str) -> PhoneNumber | None:
    for contact in book:
        if name in contact:
            return contact[name]
    return None
```

## 常见模式

- **逐步分层构建别名**：先定义简单的别名，然后将它们组合成更复杂的别名（`Cell` → `Row` → `Grid`）
- **在模块级别使用别名**：将它们放置在文件顶部附近，以便所有函数都能引用它们
- **按领域含义为别名命名**：`Matrix` 优于 `NestedIntList`，因为前者传达了业务意图
- **扁平化嵌套结构**：带有多个 `for` 子句的列表推导式是扁平化处理的 Pythonic 方式：`[x for sublist in nested for x in sublist]`