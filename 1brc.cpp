// 1brc.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include "DataProcessor.h"
#include <chrono>
#include <vector>

static void run() {
    std::unique_ptr<DataProcessor> dataProcessor = std::make_unique<DataProcessor>(L"c-measurements.txt", std::thread::hardware_concurrency());
    dataProcessor->process();
    dataProcessor->aggregateAndOutput();
}

int main()
{
    size_t iterations = 1;
    std::cout << std::thread::hardware_concurrency() << " Threads available" << std::endl;
    std::vector<std::chrono::milliseconds> benchmarksMilliseconds;
    std::vector<std::chrono::seconds> benchmarksSeconds;
    for (size_t i = 0; i < iterations; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        run();
        auto end = std::chrono::high_resolution_clock::now();
        benchmarksMilliseconds.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - start));
        benchmarksSeconds.push_back(std::chrono::duration_cast<std::chrono::seconds>(end - start));
    }
    for (size_t i = 0; i < iterations; ++i) {
        std::cout << "milliseconds: " << benchmarksMilliseconds[i].count() << ", seconds: " << benchmarksSeconds[i].count() << std::endl;
    }
    return 0;
}


