
#ifndef HASH_STORE_H
#define HASH_STORE_H
#include <vector>
#include "WStationData.h"
class HashStore
{
public:
	HashStore(size_t& allocation_size);
	void put(size_t& index_hash, int& value, std::string& key);
	std::vector<WStationData> items();
private:
	std::vector<WStationData> container;
	size_t container_size;
};
#endif


//template <class T> 
//class HashStore
//{
//public:
//	HashStore(size_t& allocation_size) : container(std::vector<T>(allocation_size)), container_size(allocation_size) {}
//	void put(size_t& index_hash, T& data)
//	{
//		container[index_hash % container_size] = data;
//	}
//	T* get(size_t& index_hash)
//	{
//		return &container[index_hash % container_size];
//	}
//	std::vector<T> items()
//	{
//		return container;
//	}
//private:
//	std::vector<T> container;
//	size_t container_size;
//};

