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
    std::shared_ptr<Node> head = table[index];
    while (head != nullptr) {
        //size_t h_key_length = head->key.size();
        //size_t a_key_length = key.size();
        //size_t str_length = h_key_length < a_key_length ? h_key_length : a_key_length;
        //
        //if (memcmp(head->key.c_str(), key.c_str(), str_length) == 0 && h_key_length == a_key_length) {
        //    return head->value;
        //}
        if (head->key == key) {
            return head->value;
        }
        head = head->next;
    }
    table[hash_value % M] = std::make_shared<Node>(key, WStationData());
    return table[hash_value % M]->value;
}