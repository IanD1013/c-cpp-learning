### 抽象类作为接口

在许多语言中，**接口**（interface）是一种结构，它定义了一个类必须实现的一组方法——而自身不提供任何具体行为。Python 没有专门的 `interface` 关键字，但你可以通过使用**每个方法都是抽象方法**的抽象类来实现完全相同的模式。当一个抽象类完全不包含具体方法时，它就变成了一个纯粹的契约（contract）：它声明了*哪些*操作必须存在，但完全不涉及它们*如何*工作。

当你有完全不相关的类，但它们仍需要支持相同的一组操作时，这种方式非常强大。`Shape` 和 `DatabaseConnection` 毫无共同之处，但它们可能都需要满足带有一个 `close()` 方法的 `Closable` 接口。

### 工作原理

一个仅包含 `@abstractmethod` 方法的抽象类不能被直接实例化。任何子类**必须**实现每一个抽象方法，否则它也将是抽象的且无法实例化。这种强制机制在实例化时生效——如果你尝试创建带有未实现抽象方法的类的实例，Python 会抛出 `TypeError`。

由于没有提供共享行为的具体方法，每个实现类完全负责其自身的逻辑。抽象类仅仅保证了具有特定签名的某些方法存在。

### 语法

```python
from abc import ABC, abstractmethod

class MyInterface(ABC):
    @abstractmethod
    def operation_one(self):
        pass

    @abstractmethod
    def operation_two(self):
        pass
```

任何继承自 `MyInterface` 的类都必须实现 `operation_one` 和 `operation_two` 才能被实例化。

### 示例

```python
from abc import ABC, abstractmethod

# An interface for anything that can be exported
class Exportable(ABC):
    @abstractmethod
    def export(self):
        pass

    @abstractmethod
    def file_extension(self):
        pass

# A CSV exporter — completely implements the interface
class CsvExport(Exportable):
    def __init__(self, rows):
        self.rows = rows

    def export(self):
        return "\n".join(",".join(row) for row in self.rows)

    def file_extension(self):
        return ".csv"

# A JSON exporter — unrelated to CSV but satisfies the same contract
class JsonExport(Exportable):
    def __init__(self, data):
        self.data = data

    def export(self):
        import json
        return json.dumps(self.data)

    def file_extension(self):
        return ".json"

# Both work polymorphically through the Exportable interface
csv = CsvExport([["name", "age"], ["Alice", "30"]])
json_exp = JsonExport({"name": "Bob"})

for item in [csv, json_exp]:
    print(f"{item.file_extension()}: {item.export()}")
```

请注意 `CsvExport` 和 `JsonExport` 是如何不共享任何实现细节的——它们只是都遵守了 `Exportable` 契约。

### 常见模式

* **工厂风格的选择**：根据参数创建适当的实现类，并通过接口来使用它。
* **多态处理**：遍历接口类型对象的列表，在不知道具体类型的情况下对每个对象调用相同的方法。
* **isinstance 检查**：你可以使用 `isinstance(obj, MyInterface)` 来验证一个对象是否满足该接口。
