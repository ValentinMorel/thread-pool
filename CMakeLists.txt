cmake_minimum_required(VERSION 3.10)

# Set the project name
project(ThreadPool)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Qt MOC processing
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Find Qt5
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)

# Include directories
include_directories(include)

# Add source files
file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "include/*.h") # Explicitly find headers

# Explicitly mark headers for MOC
set_property(SOURCE ${HEADERS} PROPERTY SKIP_AUTOMOC OFF)

# Add the main application executable
add_executable(thread_pool ${SOURCES} ${HEADERS})

# Link Qt5 libraries and pthread
target_link_libraries(thread_pool PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets pthread)

# Google Test Integration
find_package(GTest REQUIRED)
enable_testing()

# Add the test executable
file(GLOB TEST_SOURCES "tests/*.cpp")
add_executable(tests ${TEST_SOURCES} ${SOURCES} ${HEADERS})
target_include_directories(tests PRIVATE include)
target_link_libraries(tests PRIVATE GTest::GTest GTest::Main Qt5::Core Qt5::Gui Qt5::Widgets pthread)

# Enable tests
add_test(NAME TaskManagerTests COMMAND tests)
