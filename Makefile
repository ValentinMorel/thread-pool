BUILD_DIR = build
EXECUTABLE = thread_pool
TEST_EXECUTABLE = ctest --verbose

all: build run

build:
	mkdir -p $(BUILD_DIR) && cd ${BUILD_DIR} && cmake .. && make

run: 
	./$(BUILD_DIR)/$(EXECUTABLE)

test: build
	cd build && $(TEST_EXECUTABLE)

clean:
	rm -rf $(BUILD_DIR)
