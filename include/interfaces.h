#ifndef INTERFACES_H
#define INTERFACES_H

#include <string>



class ITask {
public:
    virtual std::string getName() const = 0;
    virtual int getDuration() const = 0;
    virtual void execute() = 0;
    virtual ~ITask() = default;
};

class ILogger {
public:
    virtual void log(const std::string& message) = 0;
    virtual ~ILogger() = default;
};

#endif // INTERFACES_H


