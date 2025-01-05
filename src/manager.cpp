#include "manager.h"


TaskManager::TaskManager(size_t threadCount) : stop(false) {
  for (size_t i = 0; i < threadCount; ++i) {
      workers.emplace_back([this]() {
          while (!stop.load()) {
              auto task = taskQueue.pop();
              if (task.has_value()) {
                  task.value()->execute();
              }
          }
      });
  }
}

void TaskManager::addTask(std::shared_ptr<ITask> task) {
    log("Adding task to queue.");
    taskQueue.push(task);
}

void TaskManager::stopManager() {
    log("Stopping Task Manager...");
    stop.store(true);
    for (auto& worker : workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

TaskManager::~TaskManager() {
    if (!stop.load()) {
        stopManager();
    }
}

void TaskManager::log(const std::string& message) const {
    std::cout << "[TaskManager LOG] " << message << std::endl;
}
