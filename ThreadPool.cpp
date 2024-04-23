#include "ThreadPool.h"
#include <iostream>
size_t findLastNewLine(std::vector<char>& vec)
{
    for (size_t i = vec.size() - 1; i > 0; --i)
    {
        if (vec[i] == '\n')
        {
            return i;
        }
    }
    return std::string::npos;
}

size_t findFirstNewLine(std::vector<char>& vec)
{
    for (size_t i = 0; i < vec.size(); ++i)
    {
        if (vec[i] == '\n')
        {
            return i;
        }
    }
    return std::string::npos;
}

ThreadPool::ThreadPool(
    size_t& num_threads,
    //HashTable& map
    std::unordered_map<size_t, std::unordered_map<std::string, WStationData>>& map
) : stop(false), map(map)
{
    // Create worker threads
    for (size_t i = 0; i < num_threads; ++i) {
        workers.emplace_back(
            [this, i] {
                size_t threadId = i;
                while (true) {
                    std::tuple<LPVOID, size_t, size_t> task;
                    {
                        std::unique_lock<std::mutex> lock(mtx);
                        while (tasks.empty() && !stop) {
                            condition.wait(lock);
                        }
                        if (stop && tasks.empty()) {
                            return; // Exit the thread when stopping and no tasks left
                        }
                        task = tasks.front();
                        tasks.pop();
                    }
                    
                    processChunk(std::get<0>(task), std::get<1>(task), std::get<2>(task), threadId);
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
}

void ThreadPool::enqueue(LPVOID& chunkData, size_t& readSize, size_t& chunkNumber)
{
    int x = 10;
    tasks.push({ chunkData , readSize, chunkNumber });
    condition.notify_one();
}

void ThreadPool::processChunk(LPVOID& chunkData, size_t& readSize, size_t& chunkNumber, size_t& threadId)
{
    std::vector<char> overflow;
    //if (chunkNumber > 0)
    //{
    //    while (overflowMap.count(chunkNumber - 1) == 0) {}
    //    overflow = overflowMap.at(chunkNumber - 1);
    //}

    std::vector<char> buffer(readSize + overflow.size());

    //std::copy(overflow.begin(), overflow.end(), buffer.data());
    std::memcpy(buffer.data() + overflow.size(), chunkData, readSize);
    UnmapViewOfFile(chunkData);

    size_t firstNewLine = findFirstNewLine(buffer);
    size_t lastNewLine  = findLastNewLine(buffer);
    //if (lastNewLine != std::string::npos) {
        overflow = std::vector<char>(buffer.begin() + lastNewLine + 1, buffer.end());
    //}
    overflowMap[chunkNumber] = overflow;
    //buffer.erase(buffer.begin() + lastNewLine + 1, buffer.end());
    //buffer.erase(buffer.begin(), buffer.begin() + firstNewLine + 1);
    //std::cout << "thread " << threadId << " got chunk to buffer!" << std::endl;
    int value = 0;
    bool negativeValue = false;
    std::string station;
    auto inserter = std::back_inserter<std::string>(station);
    char c;
    int multiplier = 100;
    size_t len = firstNewLine + 1;
    while (len < lastNewLine +1) {
        c = buffer.at(len);
        while (c!= semicolon) {
            *inserter = c;
            ++len;
            c = buffer.at(len);
        }
        len++;
        c = buffer.at(len);
        if (c == negative) {
            negativeValue = true; 
            len++;
            c = buffer.at(len);
        }
        while (c != newline) {
            if (c != dot) {
                value += floatParse(c, multiplier);
                multiplier /= 10;
            }
            len++;
            c = buffer.at(len);
        }
        if (negativeValue) {
            value *= -1;
        }
        map[threadId][station].update(value);
        negativeValue = false;
        value = 0;
        multiplier = 100;
        station.clear();
        len++;
    }
    //std::cout << "thread " << threadId << " finished!" << std::endl;
}

int ThreadPool::floatParse(const char& v, int multiplier) 
{
    return (v - 48) * multiplier;
}


