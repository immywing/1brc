#include "ThreadPool.h"

ThreadPool::ThreadPool(
    size_t& num_threads,
    std::unordered_map<size_t, std::unordered_map<std::string, WStationData>>& map
) : stop(false), map(map)
{
    // Create worker threads
    for (size_t i = 0; i < num_threads; ++i) {
        map[i];
        workers.emplace_back(
            [this, i] {
                size_t threadId = i;
                while (true) {
                    std::vector<char> task;
                    {
                        std::unique_lock<std::mutex> lock(mtx);
                        while (tasks.empty() && !stop) {
                            condition.wait(lock);
                        }
                        if (stop && tasks.empty()) {
                            return; // Exit the thread when stopping and no tasks left
                        }
                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    processChunk(std::move(task), threadId);
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

void ThreadPool::enqueue(std::vector<char>&& task)
{
    std::unique_lock<std::mutex> lock(mtx);
    tasks.push(std::move(task));
    condition.notify_one();
}

void ThreadPool::processChunk(std::vector<char>&& chunk, size_t& threadId) 
{
    int value = 0;
    bool negativeValue = false;
    std::string station;
    auto inserter = std::back_inserter<std::string>(station);
    char c;
    int multiplier = 100;
    size_t len = 0;
    while (len < chunk.size()) {
        c = chunk[len];
        while (c!= semicolon) {
            *inserter = c;
            ++len;
            c = chunk[len];
        }
        len++;
        c = chunk[len];
        if (c == negative) {
            negativeValue = true; 
            len++;
            c = chunk[len];
        }
        while (c != newline) {
            if (c != dot) {
                value += floatParse(c, multiplier);
                multiplier /= 10;
            }
            len++;
            c = chunk[len];
        }
        if (negativeValue) {
            value = value * -1;
        }
        map[threadId][station].update(value);
        negativeValue = false;
        value = 0;
        multiplier = 100;
        station.clear();
        len++;
    }
}

int ThreadPool::floatParse(const char& v, int multiplier) 
{
    return (v - 48) * multiplier;
}


