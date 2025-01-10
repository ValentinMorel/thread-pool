#ifndef MANAGER_H
#define MANAGER_H

#include "queue.h"
#include "task.h"
#include <vector>
#include <thread>
#include <atomic>
#include <memory>
#include <iostream>
#include <optional>
#include <csignal>


class TaskManager {
public:
    TaskManager(size_t initialThreadCount, size_t maxThreadCount);
    virtual ~TaskManager();

    void addTask(const std::shared_ptr<ITask>& task);
    void cancelTask(int taskId);
    void stopManager();
    static void signalHandler(int signal);
    bool isStopped() const; 


private:
    struct TaskWrapper {
        std::shared_ptr<ITask> task;
        int id;
        std::atomic<bool> canceled;

        TaskWrapper(const std::shared_ptr<ITask>& t, int taskId)
            : task(t), id(taskId), canceled(false) {}
    };

    void worker(size_t threadIndex);
    void monitorQueue();
    void log(const std::string& message) const;

    LockFreeQueue<std::shared_ptr<TaskWrapper>> taskQueue;
    std::vector<std::thread> workers;
    std::vector<std::shared_ptr<std::atomic<bool>>> idleFlags;

    std::atomic<bool> stop;
    std::atomic<size_t> activeThreads;
    size_t maxThreads;
    int nextTaskId;

    std::thread monitorThread;
};

#endif // MANAGER_H
