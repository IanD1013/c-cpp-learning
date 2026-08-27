cmake_minimum_required(VERSION 3.20)
project(sdl)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 让 clangd / VS Code 能读到编译参数
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# SDL3 由 `sudo make install` 装到了 /usr/local，自带 CMake 配置文件
find_package(SDL3 REQUIRED)

add_executable(prog src/main.cpp)

target_include_directories(prog PRIVATE include)
target_link_libraries(prog PRIVATE SDL3::SDL3)

# 程序用 "./assets/..." 这类相对路径读资源，
# 把可执行文件放回 sdl/ 目录，直接 ./prog 就能跑
set_target_properties(prog PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
