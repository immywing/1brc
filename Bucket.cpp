//#include "Bucket.h"
//#include <iostream>
//Bucket::Bucket(): capacity(0){}
//
//Bucket::Bucket(size_t& maxKeys): container(std::vector<std::pair<std::string, WStationData>>(maxKeys)), capacity(container.size()){}
//
//bool Bucket::hasKey(size_t& indexHash, std::string& key)
//{
//	size_t end = indexHash % capacity -1;
//	size_t index = indexHash % capacity;
//	while (container[index].first != key && index != end)
//	{
//		index = (index + 1) % capacity;
//	}
//	//std::cout << "exit has key" << std::endl;
//	return index != end && container[index].first == key;
//}
//
//void Bucket::insertUpdate(size_t& indexHash, std::string& key, int& value)
//{
//	//std::cout << "enter insert tupdate" << std::endl;
//	size_t index = indexHash % capacity;
//	while (!container[index].first.empty())
//	{
//		index = (index + 1) % capacity;
//	}
//	//std::cout << "exit insert update" << std::endl;
//	container[index].first = key;
//	container[index].second.update(value);
//}
//
//void Bucket::update(size_t& indexHash, std::string& key, int& value)
//{
//	//std::cout << "enter update" << std::endl;
//	size_t index = indexHash % capacity;
//	while (!container[index].first.empty())
//	{
//		index = (index + 1) % capacity;
//	}
//	//std::cout << "exit update 1" << std::endl;
//	while (container[index].first != key)
//	{
//		index = (index + 1) % capacity;
//	}
//	//std::cout << "exit update 2" << std::endl;
//	container[index].second.update(value);
//}