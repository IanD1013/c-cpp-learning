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
