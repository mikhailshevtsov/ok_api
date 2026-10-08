#ifndef OK_QUEUE_HPP
#define OK_QUEUE_HPP

#include <queue>
#include <mutex>
#include <condition_variable>

namespace ok
{

template <typename T>
class queue
{
public:
    void push(const T& value);
    void push(T&& value);
    
    template <typename... Args> requires std::is_constructible_v<T, Args...>
    void emplace(Args&&... args);

    T next();

private:
    std::queue<T> _queue;
    std::mutex _mtx;
    std::condition_variable _cv;
};

template <typename T>
void queue<T>::push(const T& value)
{
    std::lock_guard<std::mutex> lock(_mtx);
    _queue.push(value);
    _cv.notify_one();
}

template <typename T>
void queue<T>::push(T&& value)
{
    std::lock_guard<std::mutex> lock(_mtx);
    _queue.push(std::move(value));
    _cv.notify_one();
}

template <typename T>
template <typename... Args> requires std::is_constructible_v<T, Args...>
void queue<T>::emplace(Args&&... args) 
{
    std::lock_guard<std::mutex> lock(_mtx);
    _queue.emplace(std::forward<Args>(args)...);
    _cv.notify_one();
}

template <typename T>
T queue<T>::next()
{
    std::unique_lock<std::mutex> lock(_mtx);
    _cv.wait(lock, [this](){ return !_queue.empty(); });
    auto value = std::move(_queue.front());
    _queue.pop();
    return value;
}

}
#endif //OK_QUEUE_HPP