#include "HashStore.h"
#include <iostream>

HashStore::HashStore(size_t& allocation_size) : 
	container(std::vector<WStationData>(allocation_size)),
	container_size(allocation_size) 
{}

void HashStore::put(size_t& index_hash, int& value, std::string& key)
{
	size_t index = index_hash % container_size;
	while (container[index].count != 0) {
		index = (index + 1) % container_size;
	}
	container[index].update(value, key);
	std::cout << " loop exited " << std::endl;
}

std::vector<WStationData> HashStore::items()
{
	return container;
}