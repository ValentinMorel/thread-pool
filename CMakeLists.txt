cmake_minimum_required(VERSION 3.10)

# Set the project name
project(ThreadPool)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Specify source files for the main application
file(GLOB APP_SOURCES "src/*.cpp")
list(REMOVE_ITEM APP_SOURCES "${CMAKE_SOURCE_DIR}/src/main.cpp") # Exclude main.cpp for the tests

# Add the main application executable
add_executable(thread_pool ${APP_SOURCES} src/main.cpp)
target_link_libraries(thread_pool PRIVATE pthread)

# Google Test Integration
find_package(GTest REQUIRED)
enable_testing()

# Specify source files for the tests
file(GLOB TEST_SOURCES "tests/*.cpp")
add_executable(tests ${TEST_SOURCES} ${APP_SOURCES} tests/test_main.cpp) # Use APP_SOURCES without main.cpp
target_include_directories(tests PRIVATE include)
target_link_libraries(tests PRIVATE GTest::GTest GTest::Main pthread)

# Enable tests and verbose output
add_test(NAME RoutinesTests COMMAND tests --gtest_color=yes --gtest_output=console --gtest_catch_exceptions=0)
