#include "HashTable.h"

Node::Node(const std::string& key, WStationData value) : key(key), value(value), next(nullptr) {}

HashTable::HashTable() 
{
    for (size_t i = 0; i < M; ++i) {
        table.push_back(nullptr);
    }
}

WStationData HashTable::lookup_at(size_t& hash_value, const std::string& key) 
{
    size_t index = hash_value % M;
    Node* head = table[index];
    while (head != nullptr) {
        //if (strcmp(head->key.c_str(), key.c_str()) == 0) {
        //    return head->value;
        //}
        if (head->key == key) {
            return head->value;
        }
        head = head->next;
    }
    table[index] = new Node(key, WStationData());
    return table[hash_value % M]->value;
}