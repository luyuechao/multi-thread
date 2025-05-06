#include <iostream>
#include <thread>
#include <shared_mutex>
#include <vector>
#include <mutex>

std::shared_mutex gb_shared_mtx;
std::vector<int> gb_data {0};

void consumer(int id) {
     // shared lock for shared mutex
    std::shared_lock<std::shared_mutex> lock(gb_shared_mtx);
    std::cout << "consumer " << id << ": ";
    for (int val : gb_data) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}

void producer(int id, int value) {
    std::unique_lock<std::shared_mutex> lock(gb_shared_mtx); // Acquire exclusive lock
    std::cout << "producer " << id << ": Adding " << value << std::endl;
    gb_data.push_back(value);
}

int main() {
    std::thread t1(consumer, 1);
    std::thread t2(producer, 1, 42);
    std::thread t3(consumer, 2);
    std::thread t4(producer, 2, 7);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    std::thread t5(consumer, 3);
    t5.join();

    return 0;
}

