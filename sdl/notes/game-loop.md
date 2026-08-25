# 游戏应用结构与主循环

## 一个游戏程序的骨架

任何游戏或图形应用都可以拆成三段。

```
初始化 (setup)
   │  创建窗口、加载资源、初始化子系统
   ▼
主循环 (game loop)
   │  只要程序还在运行，就一直转
   ▼
清理 (cleanup)
      释放资源、关闭子系统
```

真正决定「这是一个游戏」的是中间那段主循环。
前后两段每个程序只跑一次，主循环每秒要跑几十上百次。

## 主循环的三步

主循环内部固定是这三步，顺序不能乱。

```
┌──────────────────────────────┐
│                              │
│   input()   处理输入          │
│      ↓      键盘、鼠标、手柄   │
│                              │
│   update()  更新状态          │
│      ↓      物理、AI、逻辑     │
│                              │
│   render()  渲染画面          │
│      ↓      把当前状态画出来    │
│                              │
└──────────┐        ┌──────────┘
           └────────┘
             下一帧
```

为什么是这个顺序：

- 先 `input`，因为这一帧的逻辑要基于玩家最新的操作。
- 再 `update`，把输入和时间的影响作用到游戏状态上。
- 最后 `render`，画的必须是「已经更新完」的状态，否则画面会落后一帧。

## 用 struct 封装成 Application

把主循环包进一个对象里，让「循环怎么转」和「每一步具体做什么」分开。

```cpp
struct Application {
    Application() {}    // Constructor
    ~Application() {}   // Destructor

    void loop() {
        while (true) {
            pfn_input();
            pfn_update();
            pfn_render();
        }
    }

    void SetInputFunction(void_void func)  { pfn_input  = func; }
    void SetUpdateFunction(void_void func) { pfn_update = func; }
    void SetRenderFunction(void_void func) { pfn_render = func; }

    void_void pfn_input;
    void_void pfn_update;
    void_void pfn_render;
};
```

`Application` 只负责「一直转，并且按 input → update → render 的顺序调用」。
每一步的具体实现由外部塞进来。

```
Application            外部提供的函数
   loop()  ──调用──>   input()
           ──调用──>   update()
           ──调用──>   render()
```

这样换一个游戏，只要换掉这三个函数，`Application` 本身一行都不用改。

## 函数指针

```cpp
typedef void (*void_void)(void);
```

这行的读法，从里往外拆：

```
typedef void (*void_void)(void);
        ^^^^   ^^^^^^^^^  ^^^^
         │         │        └── 参数列表：不接受参数
         │         └─────────── 新类型的名字（* 表示这是指针）
         └───────────────────── 返回类型：void
```

意思是：给「指向一个返回 `void`、不接受参数的函数」的指针类型，起个名字叫 `void_void`。

有了这个名字之后：

```cpp
void_void pfn_input;          // 声明一个这样的函数指针
pfn_input = &input;           // 指向 input 函数（& 可省略）
pfn_input();                  // 通过指针调用，等价于 (*pfn_input)()
```

命名习惯上 `pfn_` 前缀表示 pointer to function。

不用 `typedef` 的话，成员声明会写成这样，可读性差很多：

```cpp
void (*pfn_input)(void);
```

## 为什么要用函数指针

核心是**解耦**。

```
写死的写法                         函数指针的写法

void loop() {                     void loop() {
    while (true) {                    while (true) {
        input();      <-- 绑死            pfn_input();   <-- 运行时可换
        update();                        pfn_update();
        render();                        pfn_render();
    }                                 }
}                                 }
```

好处：

- 同一个 `Application` 可以驱动不同的游戏或不同的场景。
- 可以在运行时切换行为，比如暂停时把 `update` 换成一个空函数。
- 测试时可以塞进假的实现。

## 这份示例代码的两个坑

**成员变量没有初始化。**
`pfn_input` / `pfn_update` / `pfn_render` 在构造函数里没有赋值，是未定义的野指针。
如果忘记调用某个 `SetXxxFunction` 就直接 `loop()`，程序会直接崩掉。
写成这样更安全：

```cpp
void_void pfn_input  = nullptr;
void_void pfn_update = nullptr;
void_void pfn_render = nullptr;
```

调用前再判一下空，或者干脆默认指向空实现。

**`while (true)` 没有退出条件。**
这个循环停不下来，只能靠 Ctrl+C 杀进程。
实际写法要有一个状态位，由 `input()` 在收到退出事件时置为 false。

```cpp
bool running = true;
while (running) { ... }
```

## 和 SDL 代码的对应关系

`main.cpp` 里那个循环就是这个模型的具体版本。

```
概念模型                      SDL3 里的实现

初始化          ──>          SDL_Init() + SDL_CreateWindow()

input()         ──>          while (SDL_PollEvent(&event)) { ... }
update()        ──>          （还没写，后面加游戏逻辑）
render()        ──>          （还没写，后面加绘制代码）

退出条件        ──>          SDL_EVENT_QUIT 把 running 置为 false

清理            ──>          SDL_DestroyWindow() + SDL_Quit()
```

## C++ 里更现代的替代方案

函数指针是 C 风格的写法，能用但有局限：它只能指向普通函数，没法捕获状态。

- `std::function<void()>`：可以接受 lambda、成员函数、带捕获的可调用对象。
- 虚函数：定义一个基类，让每个游戏继承并重写 `input` / `update` / `render`。

先用函数指针理解机制，再换成上面两种更实用的方式。

## 编译运行

```sh
g++ -g -Wall main.cpp -o prog
./prog
```

`-g` 生成调试信息，`-Wall` 打开常见警告。
这个例子没用到 SDL，所以不需要 `pkg-config`。
