#include "DataProcessor.h"
#include <chrono>
#include <fstream>

DataProcessor::DataProcessor(std::wstring fpath, size_t nThreads) :
    fpath(fpath), nThreads(nThreads)
{
    for (size_t i = 0; i < nThreads; ++i) {
        map[i].reserve(65536);
    }
}

void DataProcessor::process() 
{
    SYSTEM_INFO systemInfo;
    GetSystemInfo(&systemInfo);

    SIZE_T allocationGranularity =
        systemInfo.dwPageSize > systemInfo.dwAllocationGranularity ? 
        systemInfo.dwPageSize : systemInfo.dwAllocationGranularity;

    std::unique_ptr<ThreadPool> threadPool = std::make_unique<ThreadPool>(nThreads, map);

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

    // Get file size
    LARGE_INTEGER size;
    GetFileSizeEx(hFile, &size);
    uint64_t fileSize = size.QuadPart;
    if (fileSize == INVALID_FILE_SIZE) {
        std::cerr << "Error getting file size" << std::endl;
        CloseHandle(hFile);
    }

    // Create a file mapping object
    HANDLE hMapping = CreateFileMapping(hFile, 0, PAGE_READONLY, 0, 0, 0);
    if (hMapping == nullptr) {
        std::cerr << "Error creating file mapping object" << std::endl;
        CloseHandle(hFile);
    }

    const size_t chunkSize = (fileSize / nThreads) & ~(allocationGranularity - 1);
    std::vector<char> buffer(chunkSize);
    std::vector<char> overflow;

    auto start = std::chrono::high_resolution_clock::now();
    for (size_t offset = 0; offset < fileSize; offset += chunkSize) {
  
        size_t readSize = offset + chunkSize < fileSize  ? chunkSize : fileSize - offset;
        LPVOID chunkData = MapViewOfFile(hMapping, FILE_MAP_READ, 0, offset, readSize);
        //std::cout << chunkToRead << std::endl;
        if (chunkData == nullptr) {
            DWORD err = GetLastError();
            std::cerr << err << std::endl;
            break;
        }
        
        buffer.resize(readSize + overflow.size());
        std::copy(overflow.begin(), overflow.end(), buffer.data());
        std::memcpy(buffer.data() + overflow.size(), chunkData, readSize);
        size_t rf = findLastNewLine(buffer);
        overflow = std::vector<char>(buffer.begin() + rf + 1, buffer.end());
        buffer.erase(buffer.begin() + rf + 1, buffer.end());
        threadPool->enqueue(buffer);
        // Unmap the view when done with the chunk
        UnmapViewOfFile(chunkData);
    }
    while (!threadPool->tasks.empty()) {  }
    // Close the mapping object and file handle
    CloseHandle(hMapping);
    CloseHandle(hFile);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "generating chunks to queue in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " milliseconds" << std::endl;
}

size_t DataProcessor::findLastNewLine(std::vector<char>& vec)
{
    auto it = std::find(vec.rbegin(), vec.rend(), '\n');
    if (it == vec.rend()) {
        return std::string::npos;
    }
    // Calculate index directly from the iterator
    //return (vec.size() - 1) - std::distance(vec.rbegin(), it);
    return std::distance(it, vec.rend()) - 1;
}

void DataProcessor::aggregateAndOutput() 
{
    std::unordered_map<std::string, WStationData> aggregate;
    long long i = 0;
    for (auto& outerPair : map) {
        for (auto& innerPair : outerPair.second) {
            //i += innerPair.second.count;
            aggregate[innerPair.first].aggregate(innerPair.second);
        }
    }
    std::stringstream output;
    for (auto& pair : aggregate) {
        output
            << "{"
            << pair.first
            << "=" << static_cast<double>(pair.second.min) / 10
            << "/" << std::fixed << std::setprecision(1)
            << (static_cast<double>((pair.second.sum) / 10) / pair.second.count)
            << "/" << static_cast<double>(pair.second.max) / 10
            << ",";
            //<< std::endl;
    }
    output << "}" << std::endl;
    const std::string long_string = output.str(); // Your very long string
    const char* cstr = long_string.c_str(); // Get null-terminated character array
    //size_t bytes_written = _write(1, cstr, long_string.length());
    //if (bytes_written == -1) {
    //    perror("write");
    //}
    //std::cout << output.str() << std::endl;
    //std::cout << i << " counted ";
}


