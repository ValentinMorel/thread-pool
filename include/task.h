#ifndef TASK_H
#define TASK_H

#include <string>
#include "interfaces.h"

class Task : public ITask, public ILogger {
public:
    Task(const std::string& taskName, int taskDuration);

    void execute() override;

    void log(const std::string& message) override;
    std::string getName() const override;
    int getDuration() const override;


private:
    std::string name;
    int duration;
};

#endif 
