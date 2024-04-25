#ifndef BUCKET_H
#define BUCKET_H
#include <vector>
#include "WStationData.h"
#include <memory>
struct Node 
{
    const std::string key;
    WStationData value;
    Node* next;
    Node(const std::string& key, WStationData value);
};

class HashTable 
{
private:
    const size_t M = 65536;
    std::vector<Node*> table;
public:
    HashTable();
    WStationData lookup_at(size_t& hash_value, const std::string& key);
};
#endif

