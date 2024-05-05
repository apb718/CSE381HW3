// Copyright 2024 Alan Ferrenberg
#include "BufferToggle.h"
#include "homework03Helpers.h"
#include <unistd.h>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <string>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <fstream>
#include <sstream>

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

std::string cpuTime(const int totalCpuTime) {
    int64_t seconds = totalCpuTime / sysconf(_SC_CLK_TCK);
    int hours = seconds / 3600;
    int minutes = (seconds % 3600) / 60;
    seconds = seconds % 60;
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << hours << ":"
        << std::setfill('0') << std::setw(2) << minutes << ":"
        << std::setfill('0') << std::setw(2) << seconds;
    return oss.str();
}
int getTime(const procStat& file) {
    // return file.utime + file.stime + file.cutime + file.cstime;
    return file.utime + file.stime;
}

void sortVector(std::vector<procStat>& statList,
                std::unordered_map<int, int> prevTimes) {
    std::sort(statList.begin(), statList.end(),
            [prevTimes] (const auto& lhs, const auto& rhs) {
            // std::cout << prevTimes[lhs.pid] << std::endl;
            if (prevTimes.find(lhs.pid) == prevTimes.end()) {
                return false;
            }
            if (prevTimes.find(rhs.pid) == prevTimes.end()) {
                return true;
            }
            int lhsTimeDiff = getTime(lhs) - prevTimes.at(lhs.pid);
            int rhsTimeDiff = getTime(rhs) - prevTimes.at(rhs.pid);
            return lhsTimeDiff > rhsTimeDiff;
            // return true;
        });
}

void printProcesses(std::vector<procStat> statList) {
    // clear terminal and print
    std::system("clear");
    std::cout << "   PID      CPU     COM\n";
    for (auto& file : statList) {
        std::string fileCpuTime = cpuTime(getTime(file));

        std::cout << std::setw(9) << file.pid  << " "
                    << std::setw(9) << fileCpuTime << " "
                    << file.tcomm << std::endl;
    }
}
void fillStatList(std::vector<std::string>& procFiles,
                  std::vector<procStat>& statList) {
        // Get the Proc Values
        for (auto& line : procFiles) {
            auto item = parseStatFile(line);
            std::string path = "/proc/" +
                std::to_string(item.pid) + "/loginuid";
            std::ifstream uidFile(path);
            std::string uid;
            std::getline(uidFile, uid);

            if (stol(uid) != getuid()) continue;
            statList.push_back(parseStatFile(line));
        }
}
void printLines() {
    std::unordered_map<int, int> prevTimes;
    bool firstRun = true;
    while (true) {
        std::vector<std::string> procFiles;
        std::vector<procStat> statList;
        getProcFiles(procFiles);
        fillStatList(procFiles, statList);
        if (firstRun) {
            for (auto item : statList) {
                prevTimes.emplace(item.pid, getTime(item));
            }
            firstRun = false;
            sleep(2);
            continue;
        }
        sortVector(statList, prevTimes);
        printProcesses(statList);
        // empty the map then add new time data
        prevTimes.clear();
        for (auto item : statList) {prevTimes.emplace(item.pid, getTime(item));}
        sleep(2);
    }
}



int main(int argc, char* argv[]) {
    // Launch a thread that will just wait for keyboard input.  When it gets
    // a 'q' it will use notify_all on a condition_variable to stop the program
    // This thread can run in the background with detach
    std::thread waiter(waitForInput);
    waiter.detach();
    std::thread printer(printLines);
    printer.detach();
    // Wait here until notified
    std::unique_lock<std::mutex> lock(waiter::waitMut);
    waiter::waitCond.wait(lock);
    exit(0);
}


