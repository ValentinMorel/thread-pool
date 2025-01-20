# Thread Pool Project

## Overview

This project implements a thread pool using C++17, providing Golang like goroutines. (At least an attempt)

---

## Features

- **Thread Pool**: Efficiently manage a fixed number of threads to execute tasks concurrently.
- **Logging**: Track task execution with detailed logs.
- **Unit Testing**: Comprehensive tests using Google Test (GTest).

---

## Installation and Usage

### Prerequisites
- C++17 or later
- CMake (version 3.10 or later)
- Google Test

### Building the Project
1. Clone the repository:
   ```bash
   git clone https://github.com/ValentinMorel/thread-pool.git
   cd thread-pool
   ```

2. Build the project using the Makefile
  ```bash
  make 
  ```

3. Run the main executable after building
  ```bash
  make run
  ```

4. Run the tests using Google Test:
  ```bash
  make test 
  ```


### Google Test Output 

```bash
Test project /home/user/Documents/thread-pool/build
Constructing a list of tests
Done constructing a list of tests
Updating test list for fixtures
Added 0 tests to meet fixture requirements
Checking test dependency graph...
Checking test dependency graph end
test 1
    Start 1: RoutinesTests

1: Test command: /home/user/Documents/thread-pool/build/tests "--gtest_color=yes" "--gtest_output=console" "--gtest_catch_exceptions=0"
1: Test timeout computed to be: 10000000
1: 
1: [WARNING] /build/googletest-j5yxiC/googletest-1.10.0/googletest/src/gtest.cc:5080:: WARNING: unrecognized output format "console" ignored.
1: [==========] Running 3 tests from 1 test suite.
1: [----------] Global test environment set-up.
1: [----------] 3 tests from RoutinesTests
1: [ RUN      ] RoutinesTests.AddTasks
1: [       OK ] RoutinesTests.AddTasks (0 ms)
1: [ RUN      ] RoutinesTests.TaskExecution
1: [       OK ] RoutinesTests.TaskExecution (2001 ms)
1: [ RUN      ] RoutinesTests.LargeTaskLoad
1: [       OK ] RoutinesTests.LargeTaskLoad (5000 ms)
1: [----------] 3 tests from RoutinesTests (7001 ms total)
1: 
1: [----------] Global test environment tear-down
1: [==========] 3 tests from 1 test suite ran. (7001 ms total)
1: [  PASSED  ] 3 tests.
1/1 Test #1: RoutinesTests ....................   Passed    7.01 sec

100% tests passed, 0 tests failed out of 1

Total Test time (real) =   7.02 sec
```


## Future Work
- Add task prioritization.
- Implement dynamic thread pool resizing.
- Provide a GUI for monitoring task execution.
