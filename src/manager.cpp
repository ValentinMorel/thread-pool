#include <algorithm>
#include <mutex>
#include <condition_variable>
#include "manager.h"

TaskManager* globalTaskManager = nullptr;

std::mutex cvMutex;
std::condition_variable cv;
std::mutex logMutex;

void TaskManager::log(const std::string& message) const {
    std::lock_guard<std::mutex> lock(logMutex);
    std::cout << "[TaskManager LOG] " << message << std::endl;
}

void TaskManager::worker(size_t threadIndex) {
    {
        std::lock_guard<std::mutex> lock(logMutex);
        std::cout << "Worker thread " << threadIndex << " started." << std::endl;
    }

    while (true) {
        auto task = taskQueue.pop();

        if (!task) {
            {
                std::lock_guard<std::mutex> lock(logMutex);
                std::cout << "Worker thread " << threadIndex << " received shutdown signal." << std::endl;
            }
            break; // Exit the loop if a nullptr is received
        }

        idleFlags[threadIndex]->store(false); // Mark thread as active

        auto taskWrapper = task.value();
        if (!taskWrapper->canceled.load()) {
            try {
                {
                    std::lock_guard<std::mutex> lock(logMutex);
                    std::cout << "[LOG] Executing task: " << taskWrapper->id << std::endl;
                }
                taskWrapper->task->execute();
                {
                    std::lock_guard<std::mutex> lock(logMutex);
                    std::cout << "Task " << taskWrapper->id << " executed successfully." << std::endl;
                }
            } catch (const std::exception& e) {
                std::lock_guard<std::mutex> lock(logMutex);
                std::cerr << "Task execution failed: " << e.what() << std::endl;
            }
        } else {
            std::lock_guard<std::mutex> lock(logMutex);
            std::cout << "Task " << taskWrapper->id << " was canceled." << std::endl;
        }

        idleFlags[threadIndex]->store(true); // Mark thread as idle

        if (stop.load()) { // Check stop flag after finishing task
            break;
        }
    }

    {
        std::lock_guard<std::mutex> lock(logMutex);
        std::cout << "Worker thread " << threadIndex << " exiting." << std::endl;
    }
}




TaskManager::TaskManager(size_t initialThreadCount, size_t maxThreadCount)
    : stop(false), activeThreads(0), maxThreads(maxThreadCount), nextTaskId(0) {
    globalTaskManager = this; // Set global pointer to this instance

    idleFlags.resize(maxThreadCount);
    for (size_t i = 0; i < maxThreadCount; ++i) {
        idleFlags[i] = std::make_shared<std::atomic<bool>>(false); // Initialize idle flags
    }

    // Create worker threads
    for (size_t i = 0; i < initialThreadCount; ++i) {
        workers.emplace_back([this, i]() { worker(i); });
    }

    // Monitor queue for scaling
    monitorThread = std::thread([this]() { monitorQueue(); });
    // Set up signal handler for graceful shutdown : Ctrl + C
    std::signal(SIGINT, TaskManager::signalHandler);
}

TaskManager::~TaskManager() {
    {
        std::lock_guard<std::mutex> lock(cvMutex); // Protect shared state
        stop.store(true);                          // Signal threads to stop
    }
    cv.notify_all(); // Notify the monitor thread to exit

    log("Waiting for monitor thread...");
    if (monitorThread.joinable()) {
        monitorThread.join(); // Wait for monitor thread to finish
    }

    log("Cleaning up worker threads...");
    for (auto& worker : workers) {
        if (worker.joinable()) {
            worker.join(); // Wait for worker threads to finish
        }
    }

    log("Task manager stopped.");
}





void TaskManager::addTask(const std::shared_ptr<ITask>& task) {
    auto taskWrapper = std::make_shared<TaskWrapper>(task, ++nextTaskId);
    taskQueue.push(taskWrapper);
}


void TaskManager::stopManager() {
    {
        std::lock_guard<std::mutex> lock(cvMutex); // Ensure thread-safe access
        stop.store(true);                          // Set the stop flag
    }
    cv.notify_all(); // Notify the monitor thread to exit

    // Push a nullptr for each worker to wake them up
    for (size_t i = 0; i < workers.size(); i++) {
        taskQueue.push(nullptr); // Signal worker threads to exit
    }

    log("Shutdown signal sent to all worker threads.");
}



void TaskManager::monitorQueue() {
    log("Monitor thread started.");
    std::unique_lock<std::mutex> lock(cvMutex);

    while (!cv.wait_for(lock, std::chrono::seconds(2), [this]() { return stop.load(); })) {
        size_t queueSize = 0;

        // Check the size of the task queue
        auto task = taskQueue.pop();
        while (task.has_value()) {
            queueSize++;
            taskQueue.push(task.value());
            task = taskQueue.pop();
        }

        size_t idleThreads = std::count_if(
            idleFlags.begin(), idleFlags.end(), [](const std::shared_ptr<std::atomic<bool>>& flag) { return flag->load(); });

        if (queueSize > idleThreads && workers.size() < maxThreads) {
            // Scale up: Add new threads
            size_t newThreads = std::min(maxThreads - workers.size(), queueSize - idleThreads);
            for (size_t i = 0; i < newThreads; ++i) {
                size_t threadIndex = workers.size();
                workers.emplace_back([this, threadIndex]() { worker(threadIndex); });
                idleFlags.emplace_back(std::make_shared<std::atomic<bool>>(false)); // Mark new thread as active
            }
            std::cout << "Scaled up: Added " << newThreads << " thread(s)." << std::endl;
        }
    }

    log("Monitor thread exiting.");
}



void TaskManager::signalHandler(int signal) {
    {
        std::lock_guard<std::mutex> lock(logMutex);
        std::cout << "\nSignal " << signal << " received. Initiating graceful shutdown..." << std::endl;
    }

    if (globalTaskManager) {
        globalTaskManager->stopManager(); // Gracefully stop the manager
    }

    
    //std::exit(0); // Registered atexit functions 
}


bool TaskManager::isStopped() const {
    return stop.load();
}
