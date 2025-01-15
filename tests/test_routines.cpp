#include "routines.h"
#include "channel.h"
#include "logger.h"
#include <gtest/gtest.h>
#include <memory>
#include <vector>
#include <string>

void producer(Channel<int>& channel, int count) {
    for (int i = 1; i <= count; ++i) {
        Logger::log("Sending: " + std::to_string(i));
        channel.send(i);
    }
    channel.close();
}

void consumer(Channel<int>& channel, std::vector<int>& received) {
    while (true) {
        auto value = channel.receive();
        if (!value.has_value()) {
            break; // Channel is closed
        }
        Logger::log("Received: " + std::to_string(value.value()));
        received.push_back(value.value());
    }
}

TEST(RoutinesTests, AddTasks) {
    Routines routines(2);
    Channel<int> channel;

    EXPECT_NO_THROW(routines.go(producer, std::ref(channel), 5));
    EXPECT_NO_THROW(routines.go([](Channel<int>& ch) {
        std::vector<int> received;
        consumer(ch, received);
    }, std::ref(channel)));
    routines.shutdown();
}

TEST(RoutinesTests, TaskExecution) {
    Routines routines(2);
    Channel<int> channel;

    routines.go(producer, std::ref(channel), 5);

    std::vector<int> received;
    routines.go(consumer, std::ref(channel), std::ref(received));

    std::this_thread::sleep_for(std::chrono::seconds(2));
    routines.shutdown();

    EXPECT_EQ(received.size(), 5);
    EXPECT_EQ(received, (std::vector<int>{1, 2, 3, 4, 5}));
}

TEST(RoutinesTests, LargeTaskLoad) {
    Routines routines(4);
    Channel<int> channel;
    const int numTasks = 50;

    routines.go(producer, std::ref(channel), numTasks);

    std::vector<int> received;
    routines.go(consumer, std::ref(channel), std::ref(received));

    std::this_thread::sleep_for(std::chrono::seconds(5));
    routines.shutdown();

    EXPECT_EQ(received.size(), numTasks);
    EXPECT_EQ(received.front(), 1);
    EXPECT_EQ(received.back(), numTasks);
}

/***
TODO: Fix it
TEST(RoutinesTests, TaskWithCallback) {
    Routines routines(2);

    bool callbackTriggered = false;

    routines.go(
        []() { return 42; },
        [&callbackTriggered](int result) {
            callbackTriggered = true;
            EXPECT_EQ(result, 42);
        });

    std::this_thread::sleep_for(std::chrono::seconds(1));
    routines.shutdown();

    EXPECT_TRUE(callbackTriggered);
}
 */

