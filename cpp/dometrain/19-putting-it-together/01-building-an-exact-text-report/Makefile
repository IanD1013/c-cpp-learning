CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -g

BUILD  := build
TARGET := $(BUILD)/report_test
OBJS   := $(BUILD)/report.o $(BUILD)/report_test.o

.PHONY: all test clean

all: $(TARGET)

# Build if needed, then run the tests. Exit code is non-zero when a test fails.
test: $(TARGET)
	./$(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Every .cpp here includes report.h, so rebuild an object when the header changes.
$(BUILD)/%.o: %.cpp report.h | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD):
	mkdir -p $(BUILD)

clean:
	rm -rf $(BUILD)
