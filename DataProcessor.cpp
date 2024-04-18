#include "DataProcessor.h"
#include <chrono>

DataProcessor::DataProcessor(std::wstring fpath, size_t nThreads) :
    fpath(fpath), nThreads(nThreads)
{}

void DataProcessor::process() 
{
    std::unique_ptr<ThreadPool> _threadPool = std::make_unique<ThreadPool>(nThreads, map);

    HANDLE hFile = CreateFile(
        fpath.c_str(),
        GENERIC_READ,
        FILE_SHARE_READ,
        nullptr, 
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        std::cerr << "Error opening file: " << std::endl;
    }

    // Get file size
    DWORD fileSize = GetFileSize(hFile, nullptr);
    if (fileSize == INVALID_FILE_SIZE) {
        std::cerr << "Error getting file size" << std::endl;
        CloseHandle(hFile);
    }

    // Create a file mapping object
    HANDLE hMapping = CreateFileMapping(hFile, nullptr, PAGE_READONLY, 0, 0, nullptr);
    if (hMapping == nullptr) {
        std::cerr << "Error creating file mapping object" << std::endl;
        CloseHandle(hFile);
    }
    
    // Process data in chunks
    size_t chunkSize = 1024 * 1024 * 10;  // Adjust chunk size as needed
    std::vector<char> buffer;
    std::vector<char> overflow;
    for (size_t offset = 0; offset < fileSize; offset += chunkSize) {
        size_t chunkToRead = offset + chunkSize < fileSize  ? chunkSize : fileSize - offset;
        LPVOID chunkData = MapViewOfFile(hMapping, FILE_MAP_READ, 0, offset, chunkToRead);
        if (chunkData == nullptr) {
            std::cerr << "Error mapping view of file" << std::endl;
            break;
        }
        buffer = std::vector<char>(chunkToRead + overflow.size());
        memcpy(buffer.data(), chunkData, chunkToRead);
        buffer.insert(buffer.begin(), overflow.begin(), overflow.end());
        size_t rf = findLastNewLine(buffer);
        overflow = std::vector<char>(buffer.begin() + rf + 1, buffer.end());
        buffer.erase(buffer.begin() + rf + 1, buffer.end());
        _threadPool->enqueue(std::move(buffer));
        // Unmap the view when done with the chunk
        UnmapViewOfFile(chunkData);
    }

    // Close the mapping object and file handle
    CloseHandle(hMapping);
    CloseHandle(hFile);


    //std::ifstream file(fpath);
    //if (!file.is_open()) {
    //    std::cerr << "Error opening file" << std::endl;
    //}
    //
    //file.seekg(0, std::ios::end);
    //size_t fileSize = file.tellg();
    //file.seekg(0);
    //std::cout << fileSize << std::endl;
    //size_t chunkSize = fileSize / nThreads;//1024 * 100000; //fileSize / nThreads; //
    ////std::cout << chunkSize << std::endl;
    //std::vector<char> overflow;
    //size_t bytes_read = 1;
    //while (bytes_read > 0) {
    //    std::vector<char> buffer(chunkSize);
    //    file.read(buffer.data(), chunkSize);
    //    bytes_read = file.gcount();
    //    if (bytes_read == 0) { break; }
    //    buffer.resize(bytes_read);
    //    buffer.insert(buffer.begin(), overflow.begin(), overflow.end());
    //    size_t rf = findLastNewLine(buffer);
    //    overflow = std::vector<char>(buffer.begin() + rf + 1, buffer.end());
    //    buffer.erase(buffer.begin() + rf + 1, buffer.end());
    //    _threadPool->enqueue(std::move(buffer));
    //}
    //auto end = std::chrono::high_resolution_clock::now();
    //auto duration_seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    //std::cout << "\tdone reading chunks in " << duration_seconds.count() << " seconds" << std::endl;
    //file.close();
}

size_t DataProcessor::findLastNewLine(std::vector<char>& vec)
{
    for (size_t i = vec.size() -1; i > 0; --i) {
        if (vec[i] == '\n') {
            return i;
        }
    }
    return std::string::npos;
}

void DataProcessor::aggregateAndOutput() 
{
    std::unordered_map<std::string, WStationData> aggregate;
    size_t i = 0;
    for (auto& outerPair : map) {
        for (auto& innerPair : outerPair.second) {
            aggregate[innerPair.first].update(innerPair.second);
        }
    }
    std::stringstream output;
    for (auto& pair : aggregate) {
        output
            << "{ "
            << pair.first
            << "=" << static_cast<double>(pair.second.min) / 10
            << "/" << std::fixed << std::setprecision(1) 
            << (static_cast<double>((pair.second.sum) / 10) / pair.second.count)
            << "/" << static_cast<double>(pair.second.max) / 10
            << " }"
            << std::endl;
    }
    std::cout << output.str() << std::endl;
}


