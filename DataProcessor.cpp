#include "DataProcessor.h"
#include <chrono>
#include <fstream>

DataProcessor::DataProcessor(std::wstring fpath, size_t nThreads) :
    fpath(fpath), nThreads(nThreads)
{
    for (size_t i = 0; i < nThreads; ++i) {
        map.push_back(std::unordered_map<std::string, WStationData>());
        map[i].reserve(65536);
        table.push_back(HashTable());
    }
}

void DataProcessor::process() 
{
    //SYSTEM_INFO systemInfo;
    //GetSystemInfo(&systemInfo);

    //SIZE_T allocationGranularity =
    //    systemInfo.dwPageSize > systemInfo.dwAllocationGranularity ? 
    //    systemInfo.dwPageSize : systemInfo.dwAllocationGranularity;

    HANDLE hFile = CreateFile(
        fpath.c_str(),
        GENERIC_READ,
        FILE_SHARE_READ,
        0, 
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        0
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        std::cerr << "Error opening file: " << std::endl;
    }

    LARGE_INTEGER size;
    GetFileSizeEx(hFile, &size);
    uint64_t fileSize = size.QuadPart;
    if (fileSize == INVALID_FILE_SIZE) {
        std::cerr << "Error getting file size" << std::endl;
        CloseHandle(hFile);
    }

    HANDLE hMapping = CreateFileMapping(hFile, 0, PAGE_READONLY, 0, 0, 0);
    if (hMapping == nullptr) {
        std::cerr << "Error creating file mapping object" << std::endl;
        CloseHandle(hFile);
    }

    unsigned char* mappedPtr = reinterpret_cast<unsigned char*>(MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, fileSize));
    if (mappedPtr == nullptr) {
        DWORD err = GetLastError();
        std::cerr << err << std::endl;
    }

    const size_t chunkSize = (fileSize / nThreads);

    size_t threadId = 0;
    for (size_t offset = 0; offset < fileSize; offset += chunkSize) 
    {        
        size_t readSize = offset + chunkSize < fileSize ? chunkSize : fileSize - offset;
        size_t overflow = offset + readSize;

        while (mappedPtr[overflow] != '\n' && overflow < fileSize)
        {
            overflow++;
        }
        if (overflow < fileSize)
        {
            overflow++;
        }
        overflow = overflow - (offset + readSize);
        
        workers.emplace_back(
            [this, mappedPtr, offset, readSize, overflow, threadId]
            {
                size_t offset_   = offset;
                size_t readSize_ = readSize + offset;
                size_t threadId_ = threadId;
                processChunk(mappedPtr, offset_, readSize_, threadId_);
            }
        );
        offset += overflow;
        threadId++;
    }

    for (auto& thread : workers)
    {
        thread.join();
    }
    UnmapViewOfFile(mappedPtr);
    CloseHandle(hMapping);
    CloseHandle(hFile);
}

int DataProcessor::floatParse(const unsigned char& v, int multiplier)
{
    return (v - 48) * multiplier;
}

void DataProcessor::processChunk(unsigned char* mappedFile, size_t& offset, size_t& readSize, size_t& threadId)
{
    int value = 0;
    std::string station;
    auto inserter = std::back_inserter<std::string>(station);
    int multiplier = 100;
    int mod = 1;
    size_t len = offset;
    size_t hash = 0;
    while (len < readSize) {

        while (mappedFile[len] != ';') {
            *inserter = mappedFile[len];
            hash = (hash * 31) + mappedFile[len];
            len++;
        }
        len++;

        if (mappedFile[len] == '-') {
            mod = -1;
            len++;
        }
        else { mod = 1; }
        if (mappedFile[len + 1] == '.') {
            value += floatParse(mappedFile[len], 10);
            value += (mappedFile[len + 2] - 48);
            value *= mod;
            len += 5;
        }
        else {
            value += floatParse(mappedFile[len], 100);
            value += floatParse(mappedFile[len + 1], 10);
            value += (mappedFile[len + 3] - 48);
            value *= mod;
            len += 6;
        }

        map[threadId][station].update(value);
        value = 0;
        multiplier = 100;
        station.clear();
        hash = 0;
    }
}

void DataProcessor::aggregateAndOutput() 
{
    std::unordered_map<std::string, WStationData> aggregate;
    long long i = 0;
    for (auto& outerPair : map) {
        for (auto& innerPair : outerPair) {
            i += innerPair.second.count;
            aggregate[innerPair.first].aggregate(innerPair.second);
        }
    }

    std::stringstream output;
    output << "{";
    for (auto& pair : aggregate) {
        output
            << pair.first
            << "=" << static_cast<double>(pair.second.min) / 10
            << "/" << std::fixed << std::setprecision(1)
            << (static_cast<double>((pair.second.sum) / 10) / pair.second.count)
            << "/" << static_cast<double>(pair.second.max) / 10
            << ",";
            //<< std::endl;
    }
    output << "}" << std::endl;
    const std::string long_string = output.str();
    const char* cstr = long_string.c_str(); 
    size_t bytes_written = _write(1, cstr, long_string.length());
    if (bytes_written == -1) {
        perror("write");
    }
    //std::cout << output.str() << std::endl;
    std::cout << i << " counted ";
}


