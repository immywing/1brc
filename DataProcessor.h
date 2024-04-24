
#ifndef DATA_PROCESSOR_H
#define DATA_PROCESSOR_H

#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include "WStationData.h"
#include <sstream>
#include <windows.h>
#include <iomanip>
#include <cstring>
#include <io.h>
#include "HashTable.h"
#include <thread>
class DataProcessor
{
private:
	std::vector<std::thread> workers;
	std::wstring fpath;
	size_t nThreads;
	std::unordered_map<size_t, std::unordered_map<std::string, WStationData>> map;
	//std::unordered_map<size_t, HashTable> table;
	void processChunk(unsigned char* mappedFile, size_t& offset, size_t& readSize, size_t& threadId);
	int floatParse(const char& v, int multiplier);
public:
	std::vector<std::thread> threads;
	DataProcessor(std::wstring fpath, size_t nThreads);
	void process();
	void aggregateAndOutput();
};
#endif