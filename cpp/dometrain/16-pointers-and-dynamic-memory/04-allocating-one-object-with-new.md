# C++ 动态内存基础：`new`、`delete` 与对象生命周期

## 1. `new` 做了什么？

在 C++ 中，`new` 会：

1. 为对象申请一块内存；
2. 在这块内存中构造对象；
3. 返回这个对象的地址。

例如：

```cpp
int *p = new int(7);
```

可以理解为：

* 创建一个值为 `7` 的 `int`
* 这个对象不是普通的局部变量
* `new` 返回它的地址
* 因此需要使用 `int *` 保存这个地址

对于类也是一样：

```cpp
Item *item = new Item(3);
```

这里会调用：

```cpp
Item(3)
```

也就是使用参数 `3` 调用 `Item` 的构造函数。

---

## 2. `new` 返回的是指针

这一点非常重要：

```cpp
Item *item = new Item(3);
```

`new Item(3)` 返回的不是 `Item` 对象本身，而是：

```cpp
Item *
```

也就是指向该对象的指针。

因此访问成员时要使用：

```cpp
item->method();
```

例如：

```cpp
tracker->offer(label);
```

它等价于：

```cpp
(*tracker).offer(label);
```

通常使用 `->` 更方便。

---

## 3. 局部对象与 `new` 创建的对象：生命周期不同

普通局部对象：

```cpp
void f() {
    Item item(3);
}
```

执行到 `}` 时，`item` 会自动销毁，并自动调用析构函数。

而：

```cpp
void f() {
    Item *item = new Item(3);
}
```

即使函数执行到了：

```cpp
}
```

这个 `Item` 对象也不会自动销毁。

它会一直存在，直到执行：

```cpp
delete item;
```

因此：

```cpp
new
```

创建的对象不受当前代码块作用域控制。

---

## 4. `delete` 做什么？

例如：

```cpp
int *p = new int(7);

delete p;
```

`delete p` 会：

1. 调用对象的析构函数；
2. 释放之前由 `new` 获得的内存。

对于类：

```cpp
Item *item = new Item(3);

delete item;
```

执行 `delete` 时会调用：

```cpp
~Item()
```

然后释放对应的内存。

基本规则：

```text
一次 new
   ↓
最终应该对应一次 delete
```

例如：

```cpp
Item *item = new Item(3);

// 使用 item

delete item;
```

---

## 5. 内存泄漏

如果使用了：

```cpp
new
```

却没有执行：

```cpp
delete
```

那么这块内存就可能永远无法再次使用，这称为：

> 内存泄漏（memory leak）

例如：

```cpp
void f() {
    Item *item = new Item(3);

    return;
}
```

这里虽然 `item` 这个指针变量本身消失了，但它指向的对象仍然存在。

问题在于：

```text
对象还在
但是地址丢失了
```

因此已经没有办法再执行：

```cpp
delete item;
```

---

## 6. 特别注意提前 `return`

错误示例：

```cpp
Item *item = new Item(3);

if (something) {
    return;
}

delete item;
```

如果进入：

```cpp
if (something)
```

函数会直接返回，于是：

```cpp
delete item;
```

永远不会执行。

这会导致内存泄漏。

所以手动管理 `new/delete` 时，必须保证：

> 每一条可能离开函数的路径，在离开之前都已经释放对象。

例如：

```cpp
Item *item = new Item(3);

if (something) {
    delete item;
    return;
}

delete item;
```

---

## 7. `delete nullptr` 是安全的

C++ 规定：

```cpp
delete nullptr;
```

是合法的，什么也不会发生。

例如：

```cpp
Item *item = nullptr;

delete item;
```

这是安全的。

因此很多代码在不知道一个指针是否为空时，也可以直接：

```cpp
delete p;
```

---

## 8. `delete` 之后，指针不会自动消失

这是一个非常重要的陷阱。

```cpp
Item *item = new Item(3);

delete item;
```

执行 `delete` 后：

* `Item` 对象已经不存在；
* 内存已经被释放；
* 但变量 `item` 里面仍然保存着原来的地址。

也就是说：

```cpp
item
```

仍然看起来像一个地址。

这种指针叫：

> 悬空指针（dangling pointer）

例如：

```cpp
delete item;

item->length();   // 错误
```

代码可能能够通过编译，但运行结果属于：

```text
未定义行为（undefined behavior）
```

因为你正在访问一块已经不再属于这个对象的内存。

---

## 9. `delete` 前先保存需要的数据

如果对象销毁后仍然需要某个结果，应当先复制出来：

```cpp
int result = item->length();

delete item;

return result;
```

不能这样：

```cpp
delete item;

return item->length();   // 错误
```

因为对象已经不存在了。

正确的思维顺序是：

```text
读取需要的数据
      ↓
保存到普通变量
      ↓
delete
      ↓
以后只使用保存下来的数据
```

---

# 本题中的核心模式

假设有：

```cpp
LongestSoFar *tracker =
    new LongestSoFar(maxLength);
```

然后遍历输入：

```cpp
for (const std::string &label : labels) {
    tracker->offer(label);
}
```

读取结果：

```cpp
int length = tracker->length();
```

然后销毁对象：

```cpp
delete tracker;
```

最后再返回：

```cpp
return {length, destroyedCount};
```

这里必须先：

```cpp
int length = tracker->length();
```

再：

```cpp
delete tracker;
```

因为 `delete` 以后不能再使用：

```cpp
tracker->length()
```

---

# 为什么题目使用 `destroyedCount`？

题目中的 `LongestSoFar` 析构函数会执行类似：

```cpp
~LongestSoFar() {
    destroyedCount++;
}
```

因此：

```cpp
delete tracker;
```

之后：

```cpp
destroyedCount == 1
```

说明对象确实执行了析构函数。

所以返回：

```cpp
{length, destroyedCount}
```

实际上是在同时检查：

```text
1. 你的算法结果是否正确
2. 你是否真的调用了 delete
```

---

# 本节最重要的知识点

```cpp
Type *p = new Type(...);
```

表示：

```text
动态创建一个对象
↓
new 返回对象地址
↓
用指针保存
```

访问成员：

```cpp
p->method();
```

释放对象：

```cpp
delete p;
```

需要牢记：

```text
new 出来的对象不会因为函数结束自动销毁
```

因此必须保证：

```text
每个 new 最终对应一次 delete
```

而且：

```text
delete 之后不能再解引用这个指针
```

如果还需要对象中的数据：

```cpp
先复制数据
↓
再 delete
```

---

## 现代 C++ 补充

在真实的现代 C++ 项目里，一般不推荐手动写大量：

```cpp
new
delete
```

因为很容易在异常、提前 `return` 等情况下遗漏释放。

通常会使用：

```cpp
std::unique_ptr
```

例如：

```cpp
auto tracker =
    std::make_unique<LongestSoFar>(maxLength);
```

对象离开作用域时会自动销毁。

不过本题明确禁止 `unique_ptr`，目的就是练习理解最基础的：

```text
new
→ pointer
→ use
→ delete
```

的完整生命周期。
