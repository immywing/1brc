#pragma once

#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <future>
#include <fstream>
#include <map>
#include <unordered_map>
#include "WStationData.h"
#include <sstream>
#include <iostream>
#include <functional>

class ThreadPool {
private:
    int x = 0;
    size_t nThreads;
    std::vector<std::thread> workers_;
    std::queue<std::vector<char>> tasks_;
    std::mutex mutex_;
    std::condition_variable condition_;
    std::unordered_map<size_t, std::unordered_map<std::string, WStationData>>& map;
    bool start;
    bool stop_;
    const char semicolon = ';';
    const char newline = '\n';
    const char dot = '.';
    const char negative = '-';
    int floatParse(const char& v, int multiplier);
    void processChunk(std::vector<char>&&, size_t& threadId);
public:
    ThreadPool(
        size_t num_threads,
        std::unordered_map<size_t, std::unordered_map<std::string, WStationData>>& map
    );
    ~ThreadPool();
    void enqueue(std::vector<char>&& task);
};

#endif // !THREAD_POOL_H