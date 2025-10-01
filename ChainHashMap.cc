#include "ChainHashMap.h"
template<typename T_k, typename T_v, int SIZE>
void ChainHashMap<T_k, T_v,SIZE> ::set(const T_k& key, const T_v& value) {
    int index = hashfun(key);
    for (auto& kv : table[index]) {
        if (kv.first == key) {
            kv.second = value;
            return;
        }
    }
    table[index].emplace_front(key, value);
}
template<typename T_k, typename T_v, int SIZE>
T_v ChainHashMap<T_k, T_v,SIZE>::get(const T_k& key) const {
    int index = hashfun(key);
    for (const auto& kv : table[index]) {
        if (kv.first == key) {
            return kv.second;
        }
    }
    throw std::runtime_error("Key not found");
}
template<typename T_k, typename T_v, int SIZE>
void ChainHashMap<T_k, T_v,SIZE>::del(const T_k& key) {
    int index = hashfun(key);
    for (auto it = table[index].begin(); it != table[index].end(); ++it) {
        if (it->first == key) {
            table[index].erase(it);
            return;
        }
    }
    throw std::runtime_error("Key not found");
}
template<typename T_k, typename T_v, int SIZE>
void ChainHashMap<T_k, T_v,SIZE>::print() const {
    for (int i = 0; i < SIZE; i++) {
        std::cout << "Bucket[" << i << "]: ";
        for (const auto& kv : table[i]) {
            std::cout << "(" << kv.first << "," << kv.second << ") ";
        }
        std::cout << "\n";
    }
}