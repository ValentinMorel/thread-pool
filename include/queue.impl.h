#ifndef QUEUE_IMPL_H
#define QUEUE_IMPL_H

#include "queue.h"

// Constructor
template <typename T>
LockFreeQueue<T>::LockFreeQueue() {
    Node<T>* dummy = new Node<T>(T{});
    head.store(dummy);
    tail.store(dummy);
}

// Destructor
template <typename T>
LockFreeQueue<T>::~LockFreeQueue() {
    while (head.load() != nullptr) {
        Node<T>* node = head.load();
        head.store(node->next);
        delete node;
    }
}

// Push method
template <typename T>
void LockFreeQueue<T>::push(T value) {
    Node<T>* newNode = new Node<T>(std::move(value));
    Node<T>* oldTail = nullptr;

    while (true) {
        oldTail = tail.load();
        Node<T>* next = oldTail->next;
        if (next == nullptr) {
            if (std::atomic_compare_exchange_weak(&(oldTail->next), &next, newNode)) {
                break;
            }
        } else {
            std::atomic_compare_exchange_weak(&tail, &oldTail, next);
        }
    }

    std::atomic_compare_exchange_weak(&tail, &oldTail, newNode);
}

// Pop method
template <typename T>
std::optional<T> LockFreeQueue<T>::pop() {
    while (true) {
        Node<T>* oldHead = head.load();
        Node<T>* next = oldHead->next;

        if (next == nullptr) {
            return std::nullopt;
        }

        if (std::atomic_compare_exchange_weak(&head, &oldHead, next)) {
            T result = std::move(next->data);
            delete oldHead;
            return result;
        }
    }
}

#endif // QUEUE_IMPL_H
