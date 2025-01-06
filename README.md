# Thread Pool Project

## Overview

This project implements a thread pool using C++17, providing task management and logging functionality. It includes a `TaskManager` to manage multiple tasks and a `Task` class to represent individual units of work. The project is designed to demonstrate multithreading concepts and can be extended for real-world applications.

---

## Features

- **Thread Pool**: Efficiently manage a fixed number of threads to execute tasks concurrently.
- **Task Management**: Add tasks dynamically, each with a unique name and duration.
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
    Start 1: TaskManagerTests
1/1 Test #1: TaskManagerTests .................   Passed    4.01 sec

100% tests passed, 0 tests failed out of 1

Total Test time (real) =   4.02 sec
```

## High level design

```mermaid
classDiagram
    %% Task class details
    class Task {
        +name: string
        +duration: int
        +execute(): void
        +log(message: string): void
    }

    %% TaskManager class details
    class TaskManager {
        +numThreads: int
        +tasks: LockFreeQueue
        +addTask(task: Task): void
        +start(): void
        +log(message: string): void
        +~TaskManager()
    }

    %% LockFreeQueue class details
    class LockFreeQueue {
        +head: atomic<Node>
        +tail: atomic<Node>
        +push(value: Task): void
        +pop(): Task
    }

    %% Node class details
    class Node {
        +data: Task
        +next: atomic<Node>
        +Node(value: Task)
    }

    %% Interfaces
    class ILogger {
        <<interface>>
        +log(message: string): void
    }

    class ITask {
        <<interface>>
        +execute(): void
    }

    %% Relationships
    TaskManager "1" --> "1" LockFreeQueue : uses
    LockFreeQueue "1" --> "n" Node : consists of
    Node --> "1" Task : stores
    TaskManager --> ILogger : logs
    Task --> ILogger : logs
    TaskManager --> ITask : executes
    Task --> ITask : executes
```


## Future Work
- Add task prioritization.
- Implement dynamic thread pool resizing.
- Provide a GUI for monitoring task execution.
