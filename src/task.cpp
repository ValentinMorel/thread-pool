#include <iostream>
#include <thread>

#include "task.h"

Task::Task(const std::string& taskName, int taskDuration)
    : name(taskName), duration(taskDuration) {}

void Task::execute() {
    log("Executing task: " + name);
    std::this_thread::sleep_for(std::chrono::seconds(duration));
    log("Completed task: " + name);
}

void Task::log(const std::string& message) {
    std::cout << "[LOG] " << message << std::endl;
}

std::string Task::getName() const {
    return name;
}

int Task::getDuration() const {
    return duration;
}
