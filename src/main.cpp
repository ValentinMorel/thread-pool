#include "routines.h"
#include "channel.h"
#include "logger.h"


void consumer(Channel<int>& channel) {
    while (true) {
        auto value = channel.receive();
        if (!value.has_value()) {
            break; 
        }
        Logger::log("Received: " + std::to_string(value.value()));
    }
}

int main() {
    Logger logger;
    Routines routines(8); // 4 worker threads

    Channel<int> channel;

    Logger::log("Starting consumer routine...");
    routines.go(consumer, std::ref(channel));

    Logger::log("Sending values to the channel...");

    channel.send(1);
    channel.send(2);
    channel.send(3);
    channel.send(4);
    channel.send(5);


    std::this_thread::sleep_for(std::chrono::seconds(2));
    Logger::log("Closing the channel...");
    channel.close();


    routines.shutdown();
    return 0;
}
