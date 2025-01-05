cmake_minimum_required(VERSION 3.10)

# Set the project name
project(ThreadPool)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Add non-template source files
file(GLOB SOURCES "src/*.cpp")
# Add the executable
add_executable(thread_pool ${SOURCES})
# Link pthread library
target_link_libraries(thread_pool PRIVATE pthread)



# Google Test Integration
find_package(GTest REQUIRED)
enable_testing()

# Add the test executable
file(GLOB TEST_SOURCES "tests/*.cpp")
add_executable(tests ${TEST_SOURCES} ${SOURCES})
target_include_directories(tests PRIVATE include)
target_link_libraries(tests PRIVATE GTest::GTest GTest::Main pthread)

# Enable tests
add_test(NAME TaskManagerTests COMMAND tests)
