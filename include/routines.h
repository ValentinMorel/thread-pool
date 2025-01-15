#ifndef ROUTINES_H
#define ROUTINES_H

#include <functional>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <iostream>
#include <tuple>
#include <utility>

class Routines {
public:
    Routines(size_t maxThreads);
    ~Routines();

    // Generic task with arguments
    template <typename F, typename... Args>
    void go(F&& task, Args&&... args) {
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            taskQueue.push([task = std::forward<F>(task), 
                            args = std::make_tuple(std::forward<Args>(args)...)]() mutable {
                std::apply(task, std::move(args)); // Use std::apply to call the task
            });
        }
        cv.notify_one();
    }

    void shutdown();

private:
    void worker();

    std::vector<std::thread> threads;
    std::queue<std::function<void()>> taskQueue;
    std::mutex queueMutex;
    std::condition_variable cv;
    std::atomic<bool> stop;
};

#endif // ROUTINES_H
