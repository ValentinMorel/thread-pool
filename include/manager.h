#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include <atomic>
#include <thread>
#include <vector>
#include <iostream>
#include "interfaces.h"
#include "queue.h"

class TaskManager : public QObject, public ILogger {
    Q_OBJECT

public:
    TaskManager(size_t threadCount);

    void addTask(std::shared_ptr<ITask> task);
    void stopManager();
    ~TaskManager();

    void log(const std::string& message) override;
    std::vector<std::tuple<std::string, int, std::string>> getTaskStatuses();
    void startExecution();
    void cancelExecution();

signals:
    void logMessage(const QString& message);

private:
    LockFreeQueue<std::shared_ptr<ITask>> taskQueue;
    std::vector<std::pair<int, std::thread>> workers;

    std::atomic<bool> stop;
};

#endif // MANAGER_H
