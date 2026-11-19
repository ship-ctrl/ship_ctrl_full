#pragma once

#include <iostream>

#include <queue>
//#include <shared_mutex>
#include <mutex>
#include <condition_variable>



template<typename T>
class AsyncQueue {
public:

    AsyncQueue() = default;
    virtual ~AsyncQueue() = default;
    
    void push(T value) {
        //std::unique_lock<std::shared_mutex> lock(mutex_);
        std::unique_lock<std::mutex> lock(mutex_);
        queue_.push(std::move(value));
        //std::cout << "Value added" << std::string(value) << std::endl;
        lock.unlock();
        condition_.notify_one();
    }

    T pop() {
        //std::unique_lock<std::shared_mutex> lock(mutex_);
        std::unique_lock<std::mutex> lock(mutex_);
        while (queue_.empty()) {
        //condition_.wait(lock, [] { return !(queue_.empty()); });
        condition_.wait(lock);
        }
        T value = std::move(queue_.front());
        //std::cout << "Value picked" << value << std::endl;
        queue_.pop();
        return value;
    }

    inline bool empty() { return queue_.empty(); }

private:
    std::queue<T> queue_;
    //std::shared_mutex mutex_;
    std::mutex mutex_;
    std::condition_variable condition_;
};

