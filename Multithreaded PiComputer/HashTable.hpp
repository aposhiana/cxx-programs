#ifndef _HASHTABLE_HPP_
#define _HASHTABLE_HPP_

#include <unordered_map>
#include <mutex>

template<typename K, typename V>
class HashTable {
public:
    V& operator[](K key);
private:
    std::mutex m_mutex;
    std::unordered_map<K, V> m_map;
};

template<typename K, typename V>
V& HashTable<K, V>::operator[](K key) {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_map[key];
}

#endif