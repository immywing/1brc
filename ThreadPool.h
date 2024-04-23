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
#include "HashTable.h"
#include "SimpleHash.h"

class ThreadPool {
private:
    std::vector<std::thread> workers;
    std::mutex mtx;
    std::condition_variable condition;
    std::unordered_map<size_t, std::unordered_map<std::string, WStationData>>& map;
    //HashTable& map;
    
    bool stop;
    const char semicolon = ';';
    const char newline = '\n';
    const char dot = '.';
    const char negative = '-';
    int floatParse(const char& v, int multiplier);
    void processChunk(std::vector<char>&, size_t& threadId);
public:
    std::queue<std::vector<char>> tasks;
    ThreadPool(
        size_t& num_threads,
        //HashTable& map
        std::unordered_map<size_t, std::unordered_map<std::string, WStationData>>& map
    );
    ~ThreadPool();
    void enqueue(std::vector<char>& task);
};

#endif // !THREAD_POOL_H