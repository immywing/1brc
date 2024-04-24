#include "HashTable.h"

Node::Node(const std::string& key, WStationData value) : key(key), value(value), next(nullptr) {}

HashTable::HashTable(int M = 10007) : M(M) {
    for (int i = 0; i < M; ++i) {
        table.push_back(nullptr);
    }
}

WStationData HashTable::lookup_at(size_t hash_value, const std::string& key) {
    std::shared_ptr<Node> head = table[hash_value % M];
    while (head != nullptr) {
        if (head->key == key) {
            return head->value;
        }
        head = head->next;
    }
    table[hash_value % M] = std::make_shared<Node>(key, WStationData());
    return table[hash_value % M]->value;
}