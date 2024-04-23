#ifndef BUCKET_H
#define BUCKET_H
#include <vector>
#include "WStationData.h"
#include <memory>
struct Node {
    std::string key;
    WStationData value;
    std::shared_ptr<Node> next;

    Node(const std::string& key, WStationData value) : key(key), value(value), next(nullptr) {}
};

class HashTable {
private:
    int M;  // Hash table size (prime number slightly larger than 10,000)
    std::vector<std::shared_ptr<Node>> table;

public:
    HashTable(int M = 10007) : M(M) {
        for (int i = 0; i < M; ++i) {
            table.push_back(nullptr);  // Initialize all buckets to empty
        }
    }

    void insert_at(size_t& hash_value, const std::string& key, WStationData value) {
        auto new_node = std::make_shared<Node>(key, value);
        auto head = table[hash_value % M];
        new_node->next = head;
        table[hash_value] = new_node;
    }

    WStationData lookup_at(size_t hash_value, const std::string& key) {
        auto head = table[hash_value % M];
        while (head != nullptr) {
            if (head->key == key) {
                return head->value;
            }
            head = head->next;
        }
        table[hash_value % M] = std::make_shared<Node>(key, WStationData());
        return table[hash_value % M]->value;
    }
};

#endif // !BUCKET_H

