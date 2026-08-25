# pkg-config 与 SDL3 编译笔记


## 用 CMake 构建并安装 SDL3

```
SDL/
源码 + CMakeLists.txt
        │
        │ cmake ../SDL/
        ▼
sdl_build/
Makefile + 构建配置
        │
        │ make -j 8
        ▼
真正调用 gcc/clang 编译
        │
        ▼
生成 SDL3 library
        │
        │ sudo make install
        ▼
把刚刚编译好的 SDL3 文件"安装"到系统目录里，
让其他 C/C++ 项目能够找到和使用 SDL3。
```

CMake 的作用更像是：

```
你告诉 CMake：
"我的项目有哪些文件、要生成什么、依赖什么"
        ↓
CMake 帮你生成正确的构建配置
        ↓
再调用 GCC / Clang / MSVC 编译
```

可以想成：

```
C++ source code
      ↓
CMake
      ↓
build system
      ↓
compiler
      ↓
executable
```


## 编译命令

```sh
g++ -std=c++20 main.cpp -o prog $(pkg-config --cflags --libs sdl3)
```

## pkg-config 的作用

`pkg-config --cflags sdl3`

问 pkg-config：如果我要编译一个使用 SDL3 的 C/C++ 程序，编译器需要加哪些 header 搜索参数？

结果类似：

```
-I/usr/local/include
```

## 三个常用命令

```
pkg-config --cflags sdl3
        ↓
"SDL3 的头文件在哪里？"

pkg-config --libs sdl3
        ↓
"SDL3 的 library 在哪里、要链接什么？"

pkg-config --cflags --libs sdl3
        ↓
"把编译 SDL3 程序需要的参数全给我"
```

## 整体编译流程

```
main.cpp
   │
   │ g++
   ▼
寻找 SDL header
   │
   │ pkg-config --cflags sdl3
   ▼
编译成 object file
   │
   │ pkg-config --libs sdl3
   ▼
链接 libSDL3.so
   │
   ▼
app
```
