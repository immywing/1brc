#include "HashBucket.h"

template <typename T>
HashBucket<T>::HashBucket(size_t& allocation_size) : container(std::vector<T>(10000)) {}

template <typename T>
void HashBucket<T>::put(size_t& index_hash, T& data)
{
	container[index_hash % container.size()] = data;
}

template <typename T>
T* HashBucket<T>::find(size_t& index_hash)
{
	return &container[index_hash % container.size()];
}