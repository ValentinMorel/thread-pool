#include "manager.h"
#include <memory>
#include <thread>
#include <chrono>


std::unique_ptr<TaskManager> taskManager;


int main() {
    // Initialize the TaskManager
    taskManager = std::make_unique<TaskManager>(4, 8); // 4 initial threads, max 8 threads
    // Add tasks
    taskManager->addTask(std::make_shared<Task>("Task1", 2));
    taskManager->addTask(std::make_shared<Task>("Task2", 3));
    taskManager->addTask(std::make_shared<Task>("Task3", 1));
    taskManager->addTask(std::make_shared<Task>("Task4", 4));
    taskManager->addTask(std::make_shared<Task>("Task5", 4));


    // Keep the program alive until the TaskManager is stopped
    while (!taskManager->isStopped()) {
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    taskManager.reset();

    return 0;
}
