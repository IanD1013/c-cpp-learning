# 时间、帧率与帧率无关的移动

对应文件：`sdl/include/Application.hpp` 的 `Application::Loop()`

## 一、帧和帧率

主循环转一圈（input → update → render）叫做**一帧（frame）**。

FPS（frames per second）就是「一秒钟能转多少圈」。

```
1 秒
├─── 帧 ───┼─── 帧 ───┼─── 帧 ───┼─── 帧 ───┤  ...
   16.6ms     16.6ms     16.6ms     16.6ms
```

60 FPS 意味着：

```
1000 ms ÷ 60 帧 = 16.666... ms / 帧
```

这 16.6ms 就是**一帧的预算**。
输入、更新、渲染三件事加起来必须在 16.6ms 内做完，否则这一帧就「超支」了，画面会掉帧。

常见目标帧率对应的预算：

| 目标 FPS | 每帧预算 |
| --- | --- |
| 30 | 33.3 ms |
| 60 | 16.6 ms |
| 120 | 8.3 ms |
| 144 | 6.9 ms |

## 二、SDL 怎么读时间

```cpp
Uint64 SDL_GetTicks();     // 从 SDL_Init 到现在经过的毫秒数
Uint64 SDL_GetTicksNS();   // 同上，但单位是纳秒，精度更高
```

它返回的是一个**从程序启动开始一直增长的计数**，不是当前时刻。
所以它只有一种用法：**两次相减，得到中间过了多久**。

```cpp
Uint64 start   = SDL_GetTicks();
DoSomething();
Uint64 elapsed = SDL_GetTicks() - start;   // 这段代码花了多少毫秒
```

`SDL_GetTicks()` 的分辨率只有 1 毫秒。
一帧才 16ms，1ms 的误差已经是 6%，所以更严肃的计时要用 `SDL_GetTicksNS()`。

## 三、统计当前跑到了多少帧

思路：每转一圈计数加一，每过 1 秒把计数打印出来并清零。

```cpp
Uint64 lastTime = SDL_GetTicks();   // 上一次「报时」的时刻
Uint64 framesElapsed = 0;           // 这一秒内已经跑了几帧

while (mRunning)
{
    Input();
    Update();
    Render();
    framesElapsed++;

    Uint64 currentTime = SDL_GetTicks();
    if (currentTime > lastTime + 1000)   // 距离上次报时超过 1000ms
    {
        SDL_Log("FPS: %" SDL_PRIu64, framesElapsed);
        framesElapsed = 0;
        lastTime      = SDL_GetTicks();
    }
}
```

不加限制的话，这个数字会非常大（几千甚至上万 FPS）。
因为现在的 `Update()` 只是挪几个矩形，根本用不掉 16ms。

## 四、帧率上限（frame capping）

跑太快其实是浪费：显示器一秒只刷新 60 次，多画出来的帧根本没人看到，纯烧 CPU 和电池。

做法是：**这一帧提前干完了，就主动睡一会儿。**

```cpp
Uint64 startOfFrame = SDL_GetTicks();

Input();
Update();
Render();

Uint64 elapsedTime = SDL_GetTicks() - startOfFrame;   // 这一帧实际用了多久

if (elapsedTime < 1000 / 60)
{
    SDL_Delay(1000 / 60 - elapsedTime);   // 把剩下的预算睡掉
}
```

图示：

```
预算 16ms
├────────────────────────────────┤

实际干活 4ms      补睡 12ms
├──────┤├───────────────────────┤
 工作      SDL_Delay
└────────── 一帧刚好 16ms ────────┘
```

`SDL_Delay` 就是让线程停住，把 CPU 让给系统。

### 帧上限解决不了的问题

帧上限只管**上界**，管不了**下界**。

```
机器很快  →  帧上限把它压到 60 FPS   ✅ 有效
机器很慢  →  本来就只有 20 FPS       ❌ 帧上限完全帮不上忙
```

慢机器上一帧要花 50ms，你总不能让它睡负数毫秒。
所以还需要另一套机制，让游戏在**任何帧率下都跑出一样的速度**，这就是下一节。

## 五、帧率无关的移动（frame independent movement）

这一节是重点，慢慢来。

### 5.1 先看问题出在哪

现在 `MovingRectangle::Update()` 是这么写的：

```cpp
mRectangle.x += mSpeed;   // mSpeed = 1.0f
```

翻译成人话：**每跑一帧，矩形往右挪 1 个像素。**

注意这里绑定的是「帧」，不是「时间」。
于是同一份代码在不同机器上：

```
快电脑：一秒跑 200 帧  →  一秒挪 200 像素   （嗖一下飞出屏幕）
你的机器：一秒跑 60 帧  →  一秒挪 60 像素    （正常）
慢电脑：一秒跑 20 帧   →  一秒挪 20 像素    （慢动作）
```

**同一个游戏，在不同电脑上速度完全不一样。**
这就是没有做帧率无关时的经典 bug。
（老 DOS 游戏在新电脑上快到没法玩，就是这个原因。）

### 5.2 像五岁小孩一样理解

两个小朋友要从家走到学校。

- 小快：腿快，一秒能迈 **10 步**。
- 小慢：腿慢，一秒只能迈 **5 步**。

**错误的规定：「每迈一步，前进 1 米。」**

```
小快：10 步/秒 × 1 米/步 = 10 米/秒   → 一下就到学校
小慢： 5 步/秒 × 1 米/步 =  5 米/秒   → 慢吞吞
```

两人速度不一样。这就是 `x += mSpeed` 的写法。

**正确的规定：「不管你迈几步，每秒都必须前进 5 米。」**

那每一步该迈多大？看这一步花了多少时间：

```
小快：一步花 0.1 秒  →  这一步走 5 米/秒 × 0.1 秒 = 0.5 米
小慢：一步花 0.2 秒  →  这一步走 5 米/秒 × 0.2 秒 = 1.0 米

一秒之后：
小快：0.5 米 × 10 步 = 5 米  ✅
小慢：1.0 米 ×  5 步 = 5 米  ✅
```

**两人同时到达学校。**

- 「一步」= 一帧。
- 「这一步花了多少时间」= **delta time**。
- 「每秒前进 5 米」= 速度，单位是**像素/秒**，不是像素/帧。

一句话总结：

> 腿快的人步子迈小一点，腿慢的人步子迈大一点，最后走得一样远。

### 5.3 公式

小学物理那条：

```
距离 = 速度 × 时间
```

写成代码：

```cpp
x += speed * deltaTime;
```

| 名字 | 含义 | 单位 |
| --- | --- | --- |
| `speed` | 你希望它跑多快 | 像素 / 秒 |
| `deltaTime` | 上一帧到这一帧过了多久 | 秒 |
| `speed * deltaTime` | 这一帧应该挪多少 | 像素 |

`deltaTime` 就是 delta（变化量）+ time，中文常叫「帧间隔」，代码里一般写 `dt`。

### 5.4 算一遍验证

设 `speed = 30`（每秒 30 像素）。

**情况一：跑在 10 FPS**

```
deltaTime = 1/10 = 0.1 秒
每帧移动  = 30 × 0.1 = 3 像素
一秒总计  = 3 像素 × 10 帧 = 30 像素   ✅
```

**情况二：跑在 20 FPS**

```
deltaTime = 1/20 = 0.05 秒
每帧移动  = 30 × 0.05 = 1.5 像素
一秒总计  = 1.5 像素 × 20 帧 = 30 像素  ✅
```

**情况三：跑在 200 FPS**

```
deltaTime = 1/200 = 0.005 秒
每帧移动  = 30 × 0.005 = 0.15 像素
一秒总计  = 0.15 像素 × 200 帧 = 30 像素  ✅
```

帧率翻倍，每帧的位移就减半，**一秒走过的总距离恒定**。

```
10 FPS   ├──3──┼──3──┼──3──┼──3──┤ ...  10 段
20 FPS   ├─1.5─┼─1.5─┼─1.5─┼─1.5─┤ ...  20 段
                                        两条线终点相同
```

这也解释了为什么位置要用 `float` 而不是 `int`：
0.15 像素这种小数一旦被截断成 0，物体就再也不动了。

### 5.5 deltaTime 怎么求

**写法 A：直接量上一帧用了多久（推荐）**

```cpp
Uint64 lastFrame = SDL_GetTicks();

while (mRunning)
{
    Uint64 now       = SDL_GetTicks();
    float  deltaTime = (now - lastFrame) / 1000.0f;   // 毫秒 → 秒
    lastFrame        = now;

    Input();
    Update(deltaTime);
    Render();
}
```

每一帧都拿到自己真实的时长，最准确，也最常见。

**写法 B：用上一秒的平均帧率反推**

```cpp
deltaTime = 1.0f / framesElapsed;   // 上一秒跑了 framesElapsed 帧
```

这是课程视频里那种写法。
优点是简单，缺点是：

- 它是**上一秒的平均值**，帧率突然变化时会滞后一整秒。
- 第一秒还没统计出来，`framesElapsed` 可能是 0，直接除零。

理解概念够用，实际项目用写法 A。

### 5.6 完整的循环

```cpp
void Loop()
{
    Uint64 lastFrame    = SDL_GetTicks();   // 上一帧的时刻
    Uint64 lastReport   = SDL_GetTicks();   // 上一次打印 FPS 的时刻
    Uint64 framesElapsed = 0;

    while (mRunning)
    {
        Uint64 startOfFrame = SDL_GetTicks();

        // 1. 算出上一帧到现在过了多少秒
        float deltaTime = (startOfFrame - lastFrame) / 1000.0f;
        lastFrame       = startOfFrame;

        // 2. 把 deltaTime 交给游戏逻辑
        Input();
        Update(deltaTime);
        Render();
        framesElapsed++;

        // 3. 帧上限：干完得早就补睡（可选，但省电）
        Uint64 elapsedTime = SDL_GetTicks() - startOfFrame;
        if (elapsedTime < 1000 / 60)
        {
            SDL_Delay(1000 / 60 - elapsedTime);
        }

        // 4. 每秒报一次帧数
        Uint64 currentTime = SDL_GetTicks();
        if (currentTime > lastReport + 1000)
        {
            SDL_Log("FPS: %" SDL_PRIu64, framesElapsed);
            framesElapsed = 0;
            lastReport    = currentTime;
        }
    }
}
```

注意 `lastFrame` 和 `lastReport` 是两个独立的变量，用途不同：
一个用来算 delta time（每帧更新），一个用来控制打印节奏（每秒更新）。
原始代码里只有一个 `lastTime`，两件事混用会算错。

### 5.7 改造 MovingRectangle

`Update()` 要接受 `deltaTime`，`mSpeed` 的含义从「像素/帧」变成「像素/秒」。

```cpp
void Update(float deltaTime)
{
    // 边界反弹判断不变 ...

    if (xPositiveDirection) { mRectangle.x += mSpeed * deltaTime; }
    else                    { mRectangle.x -= mSpeed * deltaTime; }

    if (yPositiveDirection) { mRectangle.y += mSpeed * deltaTime; }
    else                    { mRectangle.y -= mSpeed * deltaTime; }
}
```

同时 `mSpeed` 的数值要放大。
原来 `1.0f` 是「每帧 1 像素」，在 60 FPS 下等于「每秒 60 像素」，所以：

```cpp
float mSpeed { 60.0f };   // 60 像素 / 秒，视觉效果和改造前一致
```

上层跟着改：

```cpp
void Update(float deltaTime)
{
    for (int i = 0; i < 30; i++)
    {
        mRectangles[i].Update(deltaTime);
    }
}
```

## 六、三种做法对比

| 做法 | 快机器 | 慢机器 | 说明 |
| --- | --- | --- | --- |
| 什么都不做 | 快到飞起 | 慢动作 | `x += speed` |
| 只加帧上限 | 正常 | 仍然是慢动作 | 只管上界 |
| delta time | 正常 | 画面卡但速度正常 | 真正的解法 |
| 帧上限 + delta time | 正常且省电 | 速度正常 | 实际项目的组合 |

两者不是二选一：

- **delta time** 保证「逻辑速度正确」。
- **帧上限** 保证「不浪费 CPU / 不烧显卡」。

## 七、几个容易踩的坑

**整数除法。**

```cpp
1000 / 60        // = 16，小数被丢掉了
1000.0f / 60.0f  // = 16.6667，对
```

`1000 / 60` 每帧少算 0.66ms，一秒累积下来大约多跑 2 帧。
要精确的话用 `SDL_GetTicksNS()` 配合纳秒预算。

**单位没统一。**
`SDL_GetTicks()` 给的是**毫秒**，而 `speed` 通常按**秒**定义。
忘记除以 1000，物体会瞬间飞出屏幕（快 1000 倍）。

**deltaTime 尖峰。**
拖动窗口、切换到别的程序、加载资源时，某一帧可能长达好几百毫秒。
这时 `speed * deltaTime` 会算出一个巨大的位移，物体直接**穿墙**。
标准做法是给它封顶：

```cpp
if (deltaTime > 0.05f) { deltaTime = 0.05f; }   // 最多按 20 FPS 算
```

**第一帧。**
第一帧的 `lastFrame` 如果没初始化好，`deltaTime` 会是个垃圾值。
在进入循环前先 `lastFrame = SDL_GetTicks()`。

**API 拼写。**
原笔记里写的是 `SDL_GetTickets()`，正确的是 `SDL_GetTicks()`。

**加速度也要乘 deltaTime。**
重力这类是「速度的变化」，同样按时间来：

```cpp
velocity += gravity * deltaTime;   // gravity 单位：像素/秒²
position += velocity * deltaTime;
```

## 八、术语表

| 词 | 意思 |
| --- | --- |
| frame | 主循环转一圈 |
| FPS | 一秒转多少圈 |
| frame budget | 一帧允许花的时间，60 FPS 下是 16.6ms |
| frame capping | 提前干完就 `SDL_Delay`，压住上限帧率 |
| delta time | 上一帧到这一帧过去了多少**秒** |
| frame independent | 逻辑按时间推进，不按帧数推进 |
