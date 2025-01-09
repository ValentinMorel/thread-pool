#include <QString>
#include "manager.h"
#include <future>

TaskManager::TaskManager(size_t threadCount) : stop(false) {
  workers.reserve(threadCount);
  for (size_t i = 0; i < threadCount; ++i) {
      workers.emplace_back(i, std::thread([this,i]() {
          while (!stop.load()) {
              auto task = taskQueue.pop();
              if (task.has_value()) {
                  task.value()->execute();
              }
          }
          log("Worker " + std::to_string(i) + " stopping...");
      }));
  }
}

void TaskManager::addTask(std::shared_ptr<ITask> task) {
    log("Adding task to queue.");
    taskQueue.push(task);
}

void TaskManager::stopManager() {
    log("Stopping Task Manager...");
    stop.store(true);
    for (auto& [id, worker] : workers) {
        if (worker.joinable()) {
            log("Joining worker thread " + std::to_string(id));
            worker.join();
        }
    }
}

TaskManager::~TaskManager() {
    if (!stop.load()) {
        stopManager();
    }
}


void TaskManager::log(const std::string& message) {
    emit logMessage(QString::fromStdString("[TaskManager LOG] " + message));
}

std::vector<std::tuple<std::string, int, std::string>> TaskManager::getTaskStatuses() {
    std::vector<std::tuple<std::string, int, std::string>> statuses;

    LockFreeQueue<std::shared_ptr<ITask>> tempQueue;
    while (auto task = taskQueue.pop()) {
        if (task) {
            statuses.emplace_back(task->get()->getName(), task->get()->getDuration(), "Pending");
            tempQueue.push(task.value()); // Push back to a temporary queue
        }
    }

    // Restore original queue
    while (auto task = tempQueue.pop()) {
        taskQueue.push(task.value());
    }

    return statuses;
}

void TaskManager::startExecution() {

}


void TaskManager::cancelExecution() {
    stop.store(true); // Signal threads to stop
    log("Task execution canceled.");

    for (auto& [id, worker] : workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

