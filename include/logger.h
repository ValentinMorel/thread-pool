#ifndef LOGGER_H
#define LOGGER_H


#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <string>

class Logger {
public:
    Logger();
    ~Logger();

    static void log(const std::string& message);

    static void flush();

private:
    static void processLogs();

    static std::queue<std::string> logQueue;           
    static std::mutex queueMutex;                     
    static std::condition_variable condv;              
    static std::atomic<bool> stop;                     
    static std::thread logThread;

    static const size_t MAX_QUEUE_SIZE;                
};

#endif // LOGGER_H
