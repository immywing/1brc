#include "ThreadPool.h"
#include <iostream>

ThreadPool::ThreadPool(
    size_t& num_threads,
    HANDLE& hMapping,
    uint64_t& fileSize,
    std::unordered_map<size_t, std::unordered_map<std::string, WStationData>>& map
) : stop(false), map(map)
{
    mappedFile = reinterpret_cast<char*>(MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, fileSize));
    if (mappedFile == nullptr) {
        DWORD err = GetLastError();
        std::cerr << err << std::endl;
    }
    for (size_t i = 0; i < num_threads; ++i) {
        workers.emplace_back(
            [this, i] {
                size_t threadId = i;
                while (true) {
                    std::tuple<size_t, size_t> task;
                    {
                        std::unique_lock<std::mutex> lock(mtx);
                        while (tasks.empty() && !stop) {
                            condition.wait(lock);
                        }
                        if (stop && tasks.empty()) {
                            return;
                        }
                        task = tasks.front();
                        tasks.pop();
                    }
                    //processChunk(std::get<0>(task), std::get<1>(task), threadId);
                }
            }
        );
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(mtx);
        stop = true;
    }
    condition.notify_all();
    for (auto& thread : workers) {
        thread.join();
    }
    size_t total = 0;
    for (const auto& y : x) {
        total += y.second;
    }
    std::cout << total << std::endl;
}

void ThreadPool::enqueue(size_t offset, size_t readSize)
{
    tasksIn++;
    tasks.push(std::tuple(offset, readSize));
    condition.notify_one();
}

//void ThreadPool::processChunk(size_t& offset, size_t& readSize, size_t& threadId)
//{
//    int value = 0;
//    bool negativeValue = false;
//    std::string station;
//    auto inserter = std::back_inserter<std::string>(station);
//    char c;
//    int multiplier = 100;
//    size_t len = offset;
//    while (len < readSize) {
//        c = mappedFile[len];
//        while (c != semicolon) {
//            *inserter = c;
//            ++len;
//            c = mappedFile[len];
//        }
//        len++;
//        c = mappedFile[len];
//        if (c == negative) {
//            negativeValue = true;
//            len++;
//            c = mappedFile[len];
//        }
//        while (c != newline) {
//            if (c != dot) {
//                value += floatParse(c, multiplier);
//                multiplier /= 10;
//            }
//            len++;
//            c = mappedFile[len];
//        }
//        if (negativeValue) {
//            value *= -1;
//        }
//        map[threadId][station].update(value);
//        negativeValue = false;
//        value = 0;
//        multiplier = 100;
//        station.clear();
//        len++;
//        x[threadId]++;
//    }
//    tasksOut++;
//    //std::cout << "thread " << threadId << " finished" << std::endl;
//}

int ThreadPool::floatParse(const char& v, int multiplier)
{
    return (v - 48) * multiplier;
}


