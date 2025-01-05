#ifndef QUEUE_H
#define QUEUE_H

#include <atomic>
#include <optional>


template <typename T>
struct Node {
    T data;
    std::atomic<Node<T>*> next;

    Node(T value) : data(std::move(value)), next(nullptr) {}
};


template <typename T>
class LockFreeQueue {
public:
    LockFreeQueue();
    ~LockFreeQueue();

    void push(T value);
    std::optional<T> pop();

private:
    std::atomic<Node<T>*> head;
    std::atomic<Node<T>*> tail;
};

// Include implementation
#include "queue.impl.h"

#endif 
