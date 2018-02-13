#ifndef _QUEUE_HPP_
#define _QUEUE_HPP_

#include <queue>
#include <mutex>

template<typename T>
class Queue {
public:
    void push(T item);
    bool pop(T& item);
private:
    std::mutex m_mutex;
    std::queue<T> m_queue;
};

template<typename T>
void Queue<T>::push(T item) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_queue.push(item);
}

template<typename T>
bool Queue<T>::pop(T& item) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!m_queue.empty()) {
        item = m_queue.front();
        m_queue.pop();
        return true;
    }
    else {
        return false;
    }
}
#endif