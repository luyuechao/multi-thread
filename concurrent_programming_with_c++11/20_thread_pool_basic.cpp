#include <iostream>
#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

class ThreadPool {
public:
    ThreadPool(uint32_t thread_num = std::thread::hardware_concurrency());
    void Enqueue(std::function<void()> task);
    ~ThreadPool();

private:
    bool stop_ { false };
    std::vector<std::thread> threads_;
    std::queue<std::function<void()>> task_q_;
    std::mutex mtx_;
    std::condition_variable cv_;
};

ThreadPool::ThreadPool(uint32_t thread_num) {
    for (int i = 0; i < thread_num; ++i) {
        threads_.emplace_back([this]{
            while (true) {
                std::function<void()> task;
                std::unique_lock<std::mutex> ulock(mtx_);
                // wait until there is a task or the thread pool has been stopped
                cv_.wait(ulock, [this] { return !task_q_.empty() || stop_;}); //  spurious wakeups
                if (task_q_.empty() && stop_) {
                    return;
                }
                task = std::move(task_q_.front());
                task_q_.pop();
                ulock.unlock();
                task();
            }
        });
    }
}


void ThreadPool::Enqueue(std::function<void()> task) {
   {
        std::lock_guard<std::mutex> lock(mtx_);
        task_q_.emplace(std::move(task));
   }
   cv_.notify_one();
}

ThreadPool::~ThreadPool() {

    { // lock the queue
        std::unique_lock<std::mutex> lock(mtx_);
        stop_ = true;
    }
    cv_.notify_all();
    for (auto& t : threads_) {
        if (t.joinable()) {
            t.join();
        } else {
            t.detach();
        }
    }
}

int main() {
    ThreadPool tPool(8);
    std::mutex term_mtx;
    for (int i = 0; i < 10; ++i) {
        tPool.Enqueue([&, i]{
            {
                std::lock_guard<std::mutex> lock(term_mtx);
                std::cout << "Task # " << i << " is running on thread ["
                << std::this_thread::get_id() << "]" << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        });
    }
    return 0;
}