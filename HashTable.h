#ifndef BUCKET_H
#define BUCKET_H
#include <vector>
#include "WStationData.h"
#include <memory>

struct Node 
{
    const std::string key;
    WStationData value;
    std::shared_ptr<Node> next;
    Node(const std::string& key, WStationData value);
};

class HashTable 
{
private:
    const int M;
    std::vector<std::shared_ptr<Node>> table;
public:
    HashTable(int M = 10007);
    WStationData lookup_at(size_t hash_value, const std::string& key);
};
#endif

