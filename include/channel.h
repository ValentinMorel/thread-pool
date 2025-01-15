#ifndef CHANNEL_H
#define CHANNEL_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>

#include "logger.h"

template <typename T>
class Channel {
public:
    void send(const T& value) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            queue_.push(value);
        }
        cv_.notify_one();
    }

    std::optional<T> receive() {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [this]() { return !queue_.empty() || closed_; });
        
        if (queue_.empty()) {
            return std::nullopt; // Channel is closed
        }

        T value = queue_.front();
        queue_.pop();
        return value;
    }

    void close() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            closed_ = true;
        }
        Logger::log("Channel closed");
        cv_.notify_all();
    }

    bool isClosed() const {
        return closed_;
    }

private:
    std::queue<T> queue_;
    mutable std::mutex mutex_;
    std::condition_variable cv_;
    bool closed_ = false;
};

#endif // CHANNEL_H
