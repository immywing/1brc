#ifndef BUCKET_H
#define BUCKET_H
#include <vector>
#include "WStationData.h"
class Bucket
{
private:
	std::vector<std::pair<std::string,WStationData>> container;
public:
	const size_t capacity;
	Bucket();
	Bucket(size_t& maxKeys);
	bool hasKey(size_t& indexHash, std::string& key);
	void insertUpdate(size_t& indexHash, std::string& key, int& value);
	void update(size_t& indexHash, std::string& key, int& value);
};
#endif // !BUCKET_H

