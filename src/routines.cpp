#include <iostream>
#include "routines.h"

Routines::Routines(size_t maxThreads) : stop(false) {
    for (size_t i = 0; i < maxThreads; ++i) {
        threads.emplace_back([this]() { worker(); });
    }
}

Routines::~Routines() {
    shutdown();
}

void Routines::shutdown() {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        stop.store(true);
    }
    cv.notify_all();

    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void Routines::worker() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            cv.wait(lock, [this]() { return stop.load() || !taskQueue.empty(); });

            if (stop.load() && taskQueue.empty()) {
                return;
            }

            task = std::move(taskQueue.front());
            taskQueue.pop();
        }

        try {
            task(); // Execute the task
        } catch (const std::exception& e) {
            std::cerr << "Exception in Routines: " << e.what() << std::endl;
        }
    }
}
