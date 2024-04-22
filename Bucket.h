//#ifndef BUCKET_H
//#define BUCKET_H
//#include <vector>
//struct Node {
//    std::string key;
//    int value;
//    std::shared_ptr<Node> next;
//
//    Node(const std::string& key, int value) : key(key), value(value), next(nullptr) {}
//};
//
//class HashTable {
//private:
//    int M;  // Hash table size (prime number slightly larger than 10,000)
//    std::vector<std::shared_ptr<Node>> table;
//
//    void insert_at(int hash_value, const std::string& key, int value) {
//        Node* new_node = new Node(key, value);
//        auto head = table[hash_value];
//        new_node->next = head;
//        table[hash_value] = new_node;
//    }
//
//    int lookup_at(int hash_value, const std::string& key) {
//        auto head = table[hash_value];
//        while (head != nullptr) {
//            if (head->key == key) {
//                return head->value;
//            }
//            head = head->next;
//        }
//        return -1; // Key not found
//    }
//
//public:
//    HashTable(int M = 10007) : M(M) {
//        for (int i = 0; i < M; ++i) {
//            table[i] = nullptr;  // Initialize all buckets to empty
//        }
//    }
//
//    // Custom hash function for strings (adapt as needed)
//    int hash(const std::string& key) {
//        int hash_value = 0;
//        for (char c : key) {
//            hash_value = (hash_value * 31 + (int)c) % M;  // Fowler-Noll-Vo hash
//        }
//        return hash_value;
//    }
//
//    // Pre-compute the hash value for a key
//    int precompute_hash(const std::string& key) {
//        return hash(key);
//    }
//
//    void insert(const std::string& key, int value) {
//        int hash_value = hash(key);
//        insert_at(hash_value, key, value);
//    }
//
//    int lookup(const std::string& key, int precomputed_hash = -1) {
//        int hash_value = precomputed_hash != -1 ? precomputed_hash : hash(key);
//        return lookup_at(hash_value, key);
//    }
//};
//
//#endif // !BUCKET_H
//
