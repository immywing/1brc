#include "DataIn.h"

DataIn::DataIn(std::string& fpath) {
	std::string chunk;
	while (std::getline(file, chunk, '\n')) {
		// Check if the chunk is full or the end of file is reached
		if (chunk.size() >= chunk_size || file.eof()) {
			// Lock the mutex before pushing to the queue
			std::lock_guard<std::mutex> lock(chunkQueueMutex);

			// Start a new thread to process the chunk asynchronously
			chunkQueue.push_back(std::async(std::launch::async, processChunk, chunk));

			// Clear the chunk for the next iteration
			chunk.clear();
		}
	}
	file.close();

	for (auto& future : chunkQueue) {
		future.get();
	}

	std::cout << "All chunks processed!" << std::endl;
}