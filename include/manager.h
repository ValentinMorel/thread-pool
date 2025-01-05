#include <atomic>
#include <thread>
#include <vector>
#include <iostream>

#include "interfaces.h"
#include "queue.h"

class TaskManager : public ILogger {
public:
    TaskManager(size_t threadCount);

    void addTask(std::shared_ptr<ITask> task);

    void stopManager();

    ~TaskManager();

    void log(const std::string& message) const override;

private:
    LockFreeQueue<std::shared_ptr<ITask>> taskQueue;
    std::vector<std::thread> workers;
    std::atomic<bool> stop;
};
