#pragma once
#include<list>
#include<unordered_map>
#include <cstddef>
#include <stdexcept>

namespace cache
{

    template<typename  K, typename V>
    class LRUCache
    {

    public:
        //constructor
        LRUCache(std::size_t size):size_(size){};
    
        ~LRUCache()=default;
    
        //Lru cashe: put method 

        void put(const K & key, const V &value){
            auto it = dict_.find(key); //dict iterator is (unordered_map<K,List_iterator>::iterator)
            list_.push_front(Entry(key,value));
            if (it != dict_.end()) //element found
            {
                dict_.erase(it);
                list_.erase(it->second);
            }
            dict_[key] = list_.begin();
            
            if (dict_.size()>size_) //case over size, pop the element from the queue, and remove it from the dictionary.
            {
                auto lst_element = list_.end();
                lst_element--;
                dict_.erase(lst_element->first);
                list_.pop_back(); 
            }
            
        }
        const V & get(const K& key){
            auto it = dict_.find(key);
            
            if(!contains(key)){
                throw std::range_error("Error no key found in the cache");
            }

            list_.splice(list_.begin(), list_, it->second);
            return it->second->second;
        
        }
        inline std::size_t size()const{return this->size_;}

        inline bool contains(const K&key){return dict_.find(key)!=dict_.end();}

    private:
        //utilities alias.
        using Entry = typename std::pair<K,V>;
        using List = std::list<Entry>;  
        using List_iterator = typename std::list<std::pair<K,V>>::iterator;
        using Dict = std::unordered_map<K,List_iterator>;

        //member
        std::size_t size_;
        List list_;
        Dict dict_;

    };


    
} // namespace cache

