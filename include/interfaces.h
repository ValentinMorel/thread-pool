#pragma once
#include <string>

class ILogger {
public:
    virtual void log(const std::string& message) const = 0; 
};


class ITask {
public:
    virtual void execute() = 0; 
    virtual ~ITask() = default; 
};
