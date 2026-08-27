# CMake 与 Makefile 构建系统笔记

对应文件：`sdl/CMakeLists.txt`、`sdl/Makefile`

## 为什么会有两层

一开始每次编译都要手敲这一长串。

```sh
g++ -std=c++20 ./src/main.cpp -I./include -o prog $(pkg-config --cflags --libs sdl3)
```

于是加了两层封装，各自解决不同的问题。

```
你
↓  make run
Makefile          ← 把长命令缩短成好记的动词
↓  cmake --build build
CMakeLists.txt    ← 真正描述「项目由什么组成」
↓
CMake 生成的 build system
↓
g++ / clang++
↓
prog
```

关键区别。

`CMakeLists.txt` 是**项目定义**，说清楚有哪些源文件、依赖哪些库。
`Makefile` 只是**快捷方式**，本身不懂 C++，只负责替你调用 CMake。

所以 Makefile 被称为「对 CMake 的一层薄封装」。
「薄」是因为它没做任何复杂的事，只是把几条长命令换成 `make` / `make run`。

## 第一部分：CMakeLists.txt

### 完整内容

```cmake
cmake_minimum_required(VERSION 3.20)
project(sdl)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL3 REQUIRED)

add_executable(prog src/main.cpp)

target_include_directories(prog PRIVATE include)
target_link_libraries(prog PRIVATE SDL3::SDL3)

set_target_properties(prog PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
```

### 逐行说明

`cmake_minimum_required(VERSION 3.20)` 声明这份脚本至少需要 CMake 3.20。
版本太老的 CMake 会直接报错，而不是跑到一半才出奇怪问题。

`project(sdl)` 给项目起名，同时触发 CMake 检测编译器。

`set(CMAKE_CXX_STANDARD 20)` 等价于命令行的 `-std=c++20`。
`CMAKE_CXX_STANDARD_REQUIRED ON` 表示「必须是 C++20」，编译器不支持就报错，而不是悄悄降级到 C++17。

`set(CMAKE_EXPORT_COMPILE_COMMANDS ON)` 会生成 `build/compile_commands.json`。
这个文件记录了每个源文件的完整编译参数，clangd 和 VS Code 的 C/C++ 插件读了它之后，SDL3 的头文件跳转、自动补全、实时报错才会准确。
不生成的话，编辑器不知道 `-I/usr/local/include` 这些参数，会满屏红波浪线。

`add_executable(prog src/main.cpp)` 声明要产出一个叫 `prog` 的可执行文件，由 `src/main.cpp` 编译而来。

`target_include_directories(prog PRIVATE include)` 等价于 `-I./include`，让 `#include "Application.hpp"` 能找到头文件。

`target_link_libraries(prog PRIVATE SDL3::SDL3)` 把 SDL3 链接进来，替代了原来的 `$(pkg-config --libs sdl3)`。

`set_target_properties(...RUNTIME_OUTPUT_DIRECTORY...)` 把可执行文件放回源码目录，下面单独讲。

### PRIVATE 是什么意思

`PRIVATE` 表示这个依赖只有 `prog` 自己用得到，不会传染给别人。

因为 `prog` 是最终的可执行文件，没有任何东西会再依赖它，所以这里写 `PRIVATE` 永远是对的。
只有当你写的是一个**库**，而且库的头文件里也 `#include <SDL3/SDL.h>` 时，才需要 `PUBLIC`。
现阶段一律写 `PRIVATE` 即可。

### 为什么要把可执行文件放回源码目录

CMake 默认把产物放在 `build/` 里。
但代码里写的是相对路径。

```cpp
SDL_LoadBMP("./assets/space_invader.bmp");
```

相对路径是相对于**当前工作目录（cwd）**，不是相对于可执行文件的位置。
这一点非常容易搞混。

```
人在 sdl/ 执行 ./prog
   cwd = /home/ian/.../sdl
   找   /home/ian/.../sdl/assets/space_invader.bmp   ✓

人在 sdl/ 执行 ./build/prog
   cwd 仍然是 /home/ian/.../sdl
   找   /home/ian/.../sdl/assets/space_invader.bmp   ✓

人在 sdl/build/ 执行 ./prog
   cwd = /home/ian/.../sdl/build
   找   /home/ian/.../sdl/build/assets/...           ✗ 找不到
```

`RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}` 把 `prog` 生成在 `sdl/` 下，保持 `./prog` 的老习惯。
`${CMAKE_SOURCE_DIR}` 是内置变量，指向最外层 `CMakeLists.txt` 所在的目录。

以后想彻底解决路径问题，正确做法是用 `SDL_GetBasePath()` 拿到可执行文件所在目录再拼资源路径。
那时这行就可以删掉。

## 第二部分：新增源文件时该做什么

### CMake 不会自动发现新文件

这是新手最容易踩的坑。

假设现在多了一个 `src/game.cpp`。

```
sdl/
├── CMakeLists.txt
└── src/
    ├── main.cpp
    └── game.cpp   ← 新文件
```

如果不改 `CMakeLists.txt`，`game.cpp` **不会被编译**。

原因是这一行本身就是一份明确的清单。

```cmake
add_executable(prog src/main.cpp)
```

它在告诉 CMake：`prog` 只由 `main.cpp` 组成。
目录里多出一个 `.cpp`，CMake 不会自作主张认为它属于 `prog`。

必须手动加进去。

```cmake
add_executable(prog
    src/main.cpp
    src/game.cpp
)
```

改完之后 CMake 才知道。

```
             prog
              │
       ┌──────┴──────┐
       ↓             ↓
  main.cpp       game.cpp
       │             │
       └──────┬──────┘
              ↓
           linker
              ↓
             prog
```

### 什么时候需要重新 configure

严格的理论流程是两步。

```
CMakeLists.txt 改了
        ↓
cmake -S . -B build     ← 重新 configure，生成新的 build rules
        ↓
cmake --build build     ← 编译
```

但实际使用中，通常只需要第二条。

因为 CMake 生成的 build system 自己会监控 `CMakeLists.txt`。
一旦发现它变新了，会自动先跑一次 configure 再继续编译。

所以你会看到这样的输出。

```
-- Configuring done
-- Generating done
-- Build files have been written to: ...
[ 33%] Building CXX object ...
```

含义是：你只执行了 `cmake --build build`，但 CMake 发现 `CMakeLists.txt` 变了，于是自动重新配置，然后才编译。

### 速查表

| 你做了什么 | 要不要改 CMakeLists.txt | 日常执行 |
| --- | --- | --- |
| 修改 `main.cpp` 内容 | 不需要 | `cmake --build build` |
| 修改 `game.cpp` 内容 | 不需要 | `cmake --build build` |
| 新增 `game.cpp` | 需要 | 改 CMakeLists，然后 build |
| 删除一个 `.cpp` | 需要 | 改 CMakeLists，然后 build |
| 新增一个 library | 需要 | 改 CMakeLists，然后 build |
| 改 SDL / link 设置 | 需要 | 改 CMakeLists，然后 build |
| 删掉了整个 `build/` | 不需要 | 必须重新 `cmake -S . -B build` |

一句话记忆。

改 `.cpp` 内容，直接 build。
新增或删除 `.cpp`，改 CMakeLists 再 build，configure 会自动触发。
第一次或 `build/` 没了，跑 `cmake -S . -B build`。

有了本项目的 `Makefile` 之后，上面这些一律简化成 `make`，它会自己判断。

### 关于 file(GLOB)

以后可能会见到这种「自动找 cpp」的写法。

```cmake
file(GLOB SOURCES "src/*.cpp")
```

不推荐现在用。
GLOB 只在 configure 阶段展开一次，新增文件后如果不重新跑 cmake 就不会被发现，出问题时很难排查。
手动把每个 `.cpp` 写进 `add_executable()` 最清楚，也最不容易出错。

## 第三部分：理解 SDL3::SDL3

### 它不是一种固定语法

`SDL3::SDL3` 并不是「库叫 X 就写 X::X」的规则。
它是 SDL3 这个 CMake package 提供的一个 **target 名字**。

可以把 target 想成一个「已经配置好的库对象」，里面打包了这些东西。

```
SDL3::SDL3
   ├── SDL3 的 include 路径
   ├── SDL3 的 .so / .a 文件
   ├── SDL3 需要的编译选项
   └── SDL3 需要的 linker 参数
```

所以 `target_link_libraries(prog PRIVATE SDL3::SDL3)` 一行就够了，不用自己拼 `-I` 和 `-l`。
这也是它比 `pkg-config` 可靠的地方，参数顺序之类的细节由 CMake 处理。

### target 名字由库自己决定

用一个新库 X 时，`find_package(X REQUIRED)` 之后，link 的 target **不一定**叫 `X::X`。

它可能叫 `X::Core`、`X::Library`、`X::Client`，都有可能。

看几个真实例子。

```cmake
find_package(SDL3 REQUIRED)
target_link_libraries(prog PRIVATE SDL3::SDL3)      # 刚好是 X::X

find_package(fmt REQUIRED)
target_link_libraries(prog PRIVATE fmt::fmt)        # 也刚好是 X::X

find_package(spdlog REQUIRED)
target_link_libraries(prog PRIVATE spdlog::spdlog)  # 还是 X::X
```

但 Boost 就明显不是。

```cmake
find_package(Boost REQUIRED COMPONENTS filesystem)
target_link_libraries(prog PRIVATE Boost::filesystem)
```

这里 package 叫 `Boost`，但 target 叫 `Boost::filesystem`。
需要多个组件时分别列出。

```cmake
target_link_libraries(prog PRIVATE
    Boost::filesystem
    Boost::system
)
```

### :: 是命名空间

可以类比 C++ 的 namespace 来理解。

```
C++:     std::vector        std 是 namespace
CMake:   SDL3::SDL3         SDL3 是 package 命名空间
         Boost::filesystem
```

结构上是这样。

```
SDL3 package
   └── SDL3 target

Boost package
   ├── filesystem target
   └── system target
```

这样命名可以避免冲突。
不同的库都可能有个叫 `Core` 的东西，但 `Foo::Core` 和 `Bar::Core` 不会混。

### 正确的思路

```
我要用 X
   ↓
find_package(X ...)
   ↓
X 提供了哪些 CMake target？
   ↓
查 X 的官方文档
   ↓
target_link_libraries(... X::Something)
```

不要自己猜 `X::X`，去文档里确认。

## 第四部分：Makefile

### 完整内容

```make
# 对 CMake 的一层薄封装，日常只需要 `make run`
MAKEFLAGS += --no-print-directory
BUILD := build

.PHONY: all run clean rebuild

# 默认目标：增量编译
all: $(BUILD)/CMakeCache.txt
	cmake --build $(BUILD) -j

# build/ 还没配置过时自动跑一次 cmake
$(BUILD)/CMakeCache.txt: CMakeLists.txt
	cmake -S . -B $(BUILD)

# 编译并运行（cwd 是 sdl/，assets 相对路径才找得到）
run: all
	./prog

clean:
	rm -rf $(BUILD) prog

rebuild: clean all
```

注意命令前面必须是 **Tab**，不能是空格。
这是 Makefile 最古老也最坑的一条规则，用空格会报 `missing separator`。

### MAKEFLAGS += --no-print-directory

让输出干净一点。

make 递归调用自己时会打印这类信息。

```
make[1]: Entering directory '/home/ian/.../sdl/build'
make[1]: Leaving directory '/home/ian/.../sdl/build'
```

因为 CMake 生成的其实也是 Makefile，`cmake --build` 会递归调 make，默认会刷一堆目录切换日志。
`--no-print-directory` 关掉它，而且这个设置会传递给子 make。

`+=` 表示在已有的 `MAKEFLAGS` 后面追加一个选项，而不是覆盖。

### BUILD := build

定义变量。
之后写 `$(BUILD)` 就等于 `build`。

```make
cmake --build $(BUILD)     →     cmake --build build
```

好处是以后想把目录名从 `build` 改成 `out`，只改一行，其他地方自动跟着变。

关于 `=` 和 `:=` 的区别，现阶段可以先理解成 `:=` 是「立刻把右边的值算出来存下来」。
这里右边就是个字符串常量，两者没有差别，写 `:=` 是习惯。

### .PHONY

```make
.PHONY: all run clean rebuild
```

告诉 make：这四个是**命令名**，不是文件名。

Make 原本是围绕文件设计的。
看到 `clean:` 它会默认理解成「我要生成一个叫 clean 的文件」。

问题就在这里。
如果目录里刚好存在一个叫 `clean` 的文件，`make clean` 会认为「文件已经在了，不用干活」，然后什么都不做。

声明成 `.PHONY` 之后，make 就知道这是个虚拟目标，每次都要执行。
所有「动作型」的目标都应该写进 `.PHONY`。

### Makefile 的基本语法

```make
目标: 依赖
	命令
```

读作：要完成「目标」，先确保「依赖」准备好，然后执行「命令」。

Make 判断依赖是否需要重做，靠的是**文件修改时间**。

```
CMakeLists.txt           15:00 修改
build/CMakeCache.txt     14:00 修改
        ↓
依赖比目标新
        ↓
目标需要重新生成
```

### all 和 CMakeCache.txt

```make
all: $(BUILD)/CMakeCache.txt
	cmake --build $(BUILD) -j
```

展开后是这样。

```make
all: build/CMakeCache.txt
	cmake --build build -j
```

意思是：要完成 `all`，先确保 `build/CMakeCache.txt` 存在且是最新的，然后编译。

`CMakeCache.txt` 是执行 `cmake -S . -B build` 之后生成的文件。
这里拿它当「build/ 已经配置过」的哨兵。

用这个真实文件当哨兵，而不是用 `build/` 目录本身，是有讲究的。
目录的修改时间会随内容变动，用它判断不可靠。

### 自动 configure 的那条规则

```make
$(BUILD)/CMakeCache.txt: CMakeLists.txt
	cmake -S . -B $(BUILD)
```

意思是：`build/CMakeCache.txt` 依赖 `CMakeLists.txt`。

触发条件有两个。

一是 `build/CMakeCache.txt` 不存在，也就是从没配置过。
二是 `CMakeLists.txt` 比它新，也就是你改过配置。

任一满足就执行 `cmake -S . -B build`。

第二个条件正好呼应前面讲的「新增 `.cpp` 要改 CMakeLists」。
改完直接 `make` 就行，重新 configure 会自动发生。

### -j 是并行编译

```make
cmake --build $(BUILD) -j
```

不带数字的 `-j` 表示用满所有 CPU 核心。

```
不并行                    并行 (-j)
main.cpp                  main.cpp  ─┐
   ↓                      game.cpp  ─┤
game.cpp                  player.cpp─┤ 同时编译
   ↓                      enemy.cpp ─┘
player.cpp
   ↓
enemy.cpp
```

现在只有一个源文件看不出差别，文件多了之后差距明显。

### run

```make
run: all
	./prog
```

`run` 依赖 `all`，所以 `make run` 一定跑的是最新代码，不会出现「改完忘了编译」的情况。

完整链路。

```
make run
   ↓
run 依赖 all
   ↓
all 依赖 build/CMakeCache.txt
   ↓
不存在或 CMakeLists 变了？ → cmake -S . -B build
   ↓
cmake --build build -j
   ↓
./prog
```

等价于以前手打的 `cmake --build build && ./prog`，但多处理了「从没配置过」的情况。

注意 `./prog` 是在 `sdl/` 目录下执行的，cwd 正确，assets 相对路径才找得到。

### clean

```make
clean:
	rm -rf $(BUILD) prog
```

删掉 `build/` 目录和 `prog` 可执行文件。
`src/`、`include/`、`assets/`、`CMakeLists.txt`、`Makefile` 都不动，只清编译产物。

`rm -rf` 里的 `-r` 是递归删除目录，`-f` 是不询问强制执行。
这个组合平时要小心，这里删的是明确的 `build/`，没有风险。

### rebuild

```make
rebuild: clean all
```

依赖 `clean` 和 `all`，按顺序执行。

```
rm -rf build prog
        ↓
build/CMakeCache.txt 没了
        ↓
cmake -S . -B build
        ↓
cmake --build build -j
```

用于配置出了奇怪问题、想彻底从头来一遍的时候。

### 为什么 make 默认执行 all

Make 默认执行 Makefile 里**第一个正常 target**。
`.PHONY` 不算正常 target，所以第一个是 `all`。

因此 `make` 等价于 `make all`。

## 第五部分：日常速查

| 命令 | 作用 |
| --- | --- |
| `make` | 增量编译 |
| `make run` | 编译 + 运行（最常用） |
| `make clean` | 删除 `build/` 和 `prog` |
| `make rebuild` | 清空后从头编译 |

`make run` 可以想成一个总按钮。

```
make run
   │
   ├─ CMake 没配置过？
   │      ↓ 是
   │   cmake -S . -B build
   │
   ├─ 编译
   │   cmake --build build -j
   │
   └─ 运行
       ./prog
```

真正每天要记的就这四条，其中 `make run` 占九成。
