#include <iostream>
#include <atomic>
#include <functional>
#include <thread>
#include <chrono>
#include <vector>
#include <deque>
#include <mutex>
#include <assert.h>
#include <condition_variable>
#include <future>

// Unbounded Blocking Multi-Producer/Multi-Consumer (MPMC) Queue

// 1) shared state
// 2) mutex
// 3) predicate(state)

template <typename T>
class UnboundedBlockingMPMCQueue{
public:
    // Thread role: producer
    void Put(T value){
        std::lock_guard<std::mutex> guard(mutex_);
        buffer_.push_back(std::move(value));
        not_empty_.notify_one();
    }
    
    // Thread role: consumer
    T Take(){
        std::unique_lock<std::mutex> lock(mutex_);
        while (buffer_.empty()){ // 
            // 1) Release mutex, 2) Wait 3) Reacquire mutex
            // Spurious wakeup
            not_empty_.wait(lock);
        }
        return TakeLocked();
    }
private:
    T TakeLocked(){
        assert(!buffer_.empty());
        T front = std::move(buffer_.front());
        buffer_.pop_front();
        return front;
    }

    std::deque<T> buffer_; // Guarded by mutex_
    std::mutex mutex_;
    std::condition_variable not_empty_;
};

class StaticThreadPool{
    using Task = std::function<void()>;
public:
    StaticThreadPool(size_t workers){
        StartWorkerThreads(workers);
    }
    ~StaticThreadPool() {
        assert(workers_.empty());
    }
 
    void Submit(Task task){
        tasks_.Put(std::move(task));
    }

    void Join(){
        for (auto& worker : workers_){
            tasks_.Put({}); // Poison pill
        }
        for (auto& worker : workers_){
            worker.join();
        }
        workers_.clear();
    }

private:
    void StartWorkerThreads(size_t count){
        for (size_t i = 0; i < count; ++i){
            workers_.emplace_back([this](){
                WorkerRoutine();
            });
        }
    }
    
    void WorkerRoutine(){
        while (true){
            auto task = tasks_.Take();
            if (!task){
                break;
            }
            task();
        }
    }

    std::vector<std::thread> workers_;
    UnboundedBlockingMPMCQueue<Task> tasks_;
};

template <typename F>
auto AsyncVia(StaticThreadPool& pool, F&& task){
    using T = decltype(task());
    
    std::promise<T> promise;
    auto future = promise.get_future();

    auto task_wrapper = [promise = std::move(promise), task]() mutable {
        try{
            T value = task();
            promise.set_value(std::move(value));
        } catch(...){
            promise.set_exception(std::current_exception());
        }
    };

    pool.Submit(std::move(task_wrapper));
    
    return future;
}

int main(){
    StaticThreadPool pool(4);

    auto future = AsyncVia(pool, []() -> int {return 42; });

    std::cout << "Async result: " << future.get() << std::endl;

    pool.Join();
    
    return 0;
}