#pragma once

#include <stdexcept>
#include <utility>
#include <functional> 


template<typename T_k, typename T_v, int SIZE>
class ChainHashMap {
private:
    std::list<std::pair<T_k, T_v>> table[SIZE];

    inline int hashfun(const T_k& key) const {
        return std::hash<T_k>{}(key) % SIZE;
    }

public:
    void set(const T_k& key, const T_v& value);

    T_v get(const T_k& key) const ;

    void del(const T_k& key) ;

    void print() const;

};
