BUILD_DIR = build
EXECUTABLE = thread_pool
TEST_EXECUTABLE = tests

all: build run

build:
	cmake -B $(BUILD_DIR) -S .
	cmake --build $(BUILD_DIR)

run: build
	./$(BUILD_DIR)/$(EXECUTABLE)

test: build
	./$(BUILD_DIR)/$(TEST_EXECUTABLE)

clean:
	rm -rf $(BUILD_DIR)
