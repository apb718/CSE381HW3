// Copyright 2024 Alan Ferrenberg
#include "BufferToggle.h"
#include <unistd.h>
#include <iostream>
#include <thread>
#include <atomic>
#include <condition_variable>

using std::atomic;

namespace waiter {
    std::condition_variable waitCond;
    std::mutex waitMut;
}

void waitForInput() {
    BufferToggle bt;
    bt.off();
    while (true) {
        char ch{};
        std::cin.get(ch);
        if (ch == 'q') {
            waiter::waitCond.notify_all();
            break;
        }
    }
}

int main(int argc, char* argv[]) {
    // Launch a thread that will just wait for keyboard input.  When it gets
    // a 'q' it will use notify_all on a condition_variable to stop the program
    // This thread can run in the background with detach
    std::thread waiter(waitForInput);
    waiter.detach();

    // Wait here until notified
    std::unique_lock<std::mutex> lock(waiter::waitMut);
    waiter::waitCond.wait(lock);
    exit(0);
}
