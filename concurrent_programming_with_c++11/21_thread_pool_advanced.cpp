/*

tutorial: https://www.youtube.com/watch?v=6re5U82KwbY

A thread pool is a design pattern used to efficiently manage and execute multiple tasks concurrently
by reusing a fixed set of threads. Instead of creating and destroying threads for each individual task,
which incurs significant overhead, a thread pool maintains a pool of pre-created threads 
that are ready to execute task_q_.

The primary benefits of using a thread pool include:

Improved Performance: By reusing threads, the overhead associated with frequent thread creation and
destruction is minimized, especially for short-lived task_q_.

Efficient Resource Management: The number of threads in the pool can be controlled,
preventing resource exhaustion and ensuring that the system remains responsive
by limiting the number of concurrently executing threads.

Task Queueing: Tasks submitted to the thread pool are placed in a queue.
Threads in the pool fetch tasks from this queue and execute them as they become available,
achieving concurrent execution without creating excessive threads.

Concurrency and Scalability: A thread pool enables applications to
achieve a high degree of concurrency while optimizing the usage of available system resources,
such as CPU cores.

In essence, a thread pool separates task submission from task execution,
balancing workload and reducing the latency associated with thread lifecycle management.

https://github.com/progschj/ThreadPool

moded
https://github.com/progschj/ThreadPool/blob/234b1ab14bc5f9ab43df301941aebec03556211b/ThreadPool.h
*/
#include <iostream>
#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include <type_traits>

class ThreadPool {
public:
    explicit ThreadPool(size_t thread_count);
    ~ThreadPool();

    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args)
        -> std::future<typename std::invoke_result_t<F, Args...>>;
    void stop();

private:
    // Worker threads
    std::vector<std::thread> threads_;
    // Task queue
    std::queue<std::packaged_task<void()>> task_queue_;

    // Synchronization
    std::mutex queue_mutex_;
    std::condition_variable condition_;
    bool stop_ = false;
};

// Constructor: Launches threads
inline ThreadPool::ThreadPool(size_t thread_count) {
    for (size_t i = 0; i < thread_count; ++i) {
        threads_.emplace_back([this] {
            while (true) {
                std::packaged_task<void()> task;
                {
                    std::unique_lock<std::mutex> lock(queue_mutex_);
                    condition_.wait(lock, [this] {
                        return stop_ || !task_queue_.empty(); //  spurious wakeups
                    });
                    if (stop_ && task_queue_.empty()) {
                        return;
                    }
                    task = std::move(task_queue_.front());
                    task_queue_.pop();
                }
                task();  // Execute the task outside the lock
            }
        });
    }
}

// scott meyer morden c++, item 
void ThreadPool::stop() {
    {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        stop_ = true;
    }
    condition_.notify_all();
    for (auto& t : threads_) {
        if (t.joinable()) {
            t.join();
        } else {
            t.detach();
        }
    }
}

ThreadPool::~ThreadPool() {

}

// Enqueue a task into the thread pool
template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args)
    -> std::future<typename std::invoke_result_t<F, Args...>> 
{
    using return_type = typename std::invoke_result_t<F, Args...>;
    
    std::packaged_task<return_type()> pkg_task(std::bind(std::forward<F>(f),
                                                         std::forward<Args>(args)...));
    auto future_res(pkg_task.get_future());
    {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        if (stop_) {
            throw std::runtime_error("enqueue on stopped ThreadPool");
        }
        task_queue_.emplace(std::move(pkg_task));
    }
    condition_.notify_one();
    // return future_res; // implicitly moved.
    return std::move(future_res);
}

int main() {
    ThreadPool tp(4);
    auto fut_res = tp.enqueue([](int answer) { return answer; }, 42);

    // get result from future
    std::cout << fut_res.get() << std::endl;

    auto fut_res2 = tp.enqueue([](const std::string& answer) { return answer; }, "hello");
    std::cout << fut_res2.get() << std::endl;
    tp.stop();
}
