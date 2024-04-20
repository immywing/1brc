
#ifndef DATA_PROCESSOR_H
#define DATA_PROCESSOR_H

#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include "WStationData.h"
#include "ThreadPool.h"
#include <sstream>
#include <windows.h>
#include <iomanip>
#include <cstring>
#include <io.h>
#include "SimpleHash.h"
class DataProcessor
{
private:
	std::wstring fpath;
	size_t nThreads;
	std::unordered_map<size_t, std::unordered_map<std::string, WStationData>> map;
	size_t findLastNewLine(std::vector<char>& vec);
public:
	std::vector<std::thread> threads;
	DataProcessor(std::wstring fpath, size_t nThreads);
	void process();
	void aggregateAndOutput();
};
#endif