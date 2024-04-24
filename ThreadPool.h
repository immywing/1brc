#pragma once

#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <fstream>
#include <unordered_map>
#include "WStationData.h"
#include <tuple>
#include <memory>
#include <Windows.h>

class ThreadPool {
private:
    std::unordered_map<size_t, long long> x;
    std::vector<std::thread> workers;
    std::mutex mtx;
    std::condition_variable condition;
    std::unordered_map<size_t, std::unordered_map<std::string, WStationData>>& map;
    char* mappedFile;
    bool stop;
    const char semicolon = ';';
    const char newline = '\n';
    const char dot = '.';
    const char negative = '-';
    int floatParse(const char& v, int multiplier);
    //void processChunk(size_t& offset, size_t& readSize, size_t& threadId);
public:
    int tasksIn  = 0;
    int tasksOut = 0;
    std::queue<std::tuple<size_t, size_t>> tasks;
    ThreadPool(
        size_t& num_threads,
        HANDLE& hMapping,
        uint64_t& fileSize,
        std::unordered_map<size_t, std::unordered_map<std::string, WStationData>>& map
    );
    ~ThreadPool();
    void enqueue(size_t offset, size_t readSize);
};

#endif // !THREAD_POOL_H