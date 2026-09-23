### 修改内部状态的方法

在面向对象编程中，对象同时包含**数据**（属性）和**行为**（方法）。方法最重要的用途之一就是以受控的方式修改对象自身的属性。与其从类外部直接更改属性，不如编写方法在状态发生变化时强制执行规则和验证。这种模式——将状态变化封装在方法内部——是面向对象编程（OOP）的核心原则，能够确保你的对象具有可预测性且安全。

### 工作原理

当一个方法修改 `self.some_attribute` 时，它会改变对象的内部状态。该方法不需要返回任何内容——其目的是产生更新属性的**副作用**（side effect）。稍后，另一个方法可以通过返回该属性的值来公开该状态。这种分离为你提供了完全的控制权：你可以决定状态*如何*改变、*何时*允许改变以及外部人员可以查看*哪些*内容。

### 语法

```python
class MyClass:
    def __init__(self):
        self.attribute = initial_value

    def modify_state(self):
        self.attribute = new_value  # or self.attribute += something

    def read_state(self):
        return self.attribute
```

请注意，`modify_state` 没有返回任何内容——它只是更改了 `self.attribute`。而 `read_state` 方法提供了对属性当前值的受控访问。

### 示例

```python
# A simple toggle switch
class LightSwitch:
    def __init__(self):
        self.is_on = False

    def flip(self):
        self.is_on = not self.is_on

    def status(self):
        return "on" if self.is_on else "off"

switch = LightSwitch()
print(switch.status())  # "off"
switch.flip()
print(switch.status())  # "on"
switch.flip()
print(switch.status())  # "off"
```

```python
# A temperature tracker that records the highest temperature seen
class TemperatureTracker:
    def __init__(self):
        self.highest = None

    def record(self, temp):
        if self.highest is None or temp > self.highest:
            self.highest = temp

    def get_highest(self):
        return self.highest

tracker = TemperatureTracker()
tracker.record(72)
tracker.record(85)
tracker.record(68)
print(tracker.get_highest())  # 85
```

### 常见模式

* **累加器（Accumulator）**：方法对存储在属性中的累计值进行增加或减少。
* **切换器（Toggle）**：方法在布尔属性的 `True` 和 `False` 之间进行翻转切换。
* **有界变异（Bounded mutation）**：方法更新属性但强制执行约束条件（例如，最小值或最大值）。
* **只读访问（Read-only access）**：getter 方法返回属性的值而不修改它，使属性始终处于类的控制之下。
