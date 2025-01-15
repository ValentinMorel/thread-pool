#include <iostream>
#include <sstream>
#include "logger.h"

std::queue<std::string> Logger::logQueue;
std::mutex Logger::queueMutex;
std::condition_variable Logger::condv;
std::atomic<bool> Logger::stop(false);
std::thread Logger::logThread;

const size_t Logger::MAX_QUEUE_SIZE = 1000; 

Logger::Logger() {
    logThread = std::thread(Logger::processLogs); 
}

Logger::~Logger() {
    stop.store(true);
    condv.notify_all();
    if (logThread.joinable()) {
        logThread.join();
    }
    flush(); 
}

void Logger::log(const std::string& message) {
    static thread_local std::ostringstream buffer; // Thread-local buffer

    buffer << message << std::endl; 

    {
        std::lock_guard<std::mutex> lock(queueMutex);
        if (logQueue.size() >= MAX_QUEUE_SIZE) {
            std::cerr << "[Logger ERROR] Log queue full. Dropping message: " << buffer.str();
            buffer.str("");
            buffer.clear();
            return;
        }
        logQueue.push(buffer.str()); 
        buffer.str("");
        buffer.clear();
    }

    condv.notify_one();
}

void Logger::flush() {
    std::lock_guard<std::mutex> lock(queueMutex);
    while (!logQueue.empty()) {
        std::cout << logQueue.front(); 
        logQueue.pop();
    }
}

void Logger::processLogs() {
    while (!stop.load() || !logQueue.empty()) {
        std::unique_lock<std::mutex> lock(queueMutex);
        condv.wait(lock, []() { return stop.load() || !logQueue.empty(); });

        while (!logQueue.empty()) {
            std::cout << logQueue.front();
            logQueue.pop();
        }
    }
}
