#include "manager.h"
#include "task.h"
#include <gtest/gtest.h>
#include <memory>
#include <vector>


TEST(TaskManagerTests, AddTasks) {
    TaskManager manager(2); 

    EXPECT_NO_THROW(manager.addTask(std::make_shared<Task>("Task1", 2)));
    EXPECT_NO_THROW(manager.addTask(std::make_shared<Task>("Task2", 3)));
    EXPECT_NO_THROW(manager.addTask(std::make_shared<Task>("Task3", 1)));
}

TEST(TaskManagerTests, TaskExecution) {
    TaskManager manager(2); 

    manager.addTask(std::make_shared<Task>("Task1", 1));
    manager.addTask(std::make_shared<Task>("Task2", 2));

    std::this_thread::sleep_for(std::chrono::seconds(3));

    SUCCEED();
}

TEST(TaskManagerTests, LargeTaskLoad) {
    TaskManager manager(4);
    const int numTasks = 20;

    for (int i = 0; i < numTasks; ++i) {
        EXPECT_NO_THROW(manager.addTask(std::make_shared<Task>("Task" + std::to_string(i), 1)));
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));

    SUCCEED();
  }

TEST(TaskManagerTests, NullTaskHandling) {
    TaskManager manager(2);

    EXPECT_THROW(manager.addTask(nullptr), std::invalid_argument);
}
