/*
std::shared_future in C++ is part of the <future> header and allows multiple threads to
access the result of an asynchronous operation. Unlike std::future,
which supports only single ownership, std::shared_future supports shared ownership of the result,
meaning multiple threads can call get() on the same std::shared_future object.

This makes std::shared_future ideal when you need to share the result of a function
among multiple threads without re-computing it.


(1) Multiple Ownership:
std::shared_future allows multiple threads or functions to access the result of the same asynchronous operation.

(2) Thread-Safe:
Multiple threads can safely call get() on the same std::shared_future.
Lazy Initialization:

Can be constructed from a std::future (the std::future becomes invalid after this).
Access to Results:

Provides access to the stored value or exception, just like std::future.

Differences Between std::future and std::shared_future
Feature        | std::future	        | std::shared_future
Ownership	   | Single thread	        | Multiple threads
get() behavior | Invalidates the future	| Can be called multiple times
Conversion	   | Cannot share result	| Can be constructed from std::future
Use Case	   | Single-threaded result	| Multi-threaded result sharing

*/

#include <iostream>
#include <future>
#include <thread>
#include <vector>

int compute_factorial(int n) {
    int result = 1;
    for (int i = 1; i <= n; ++i) {
        result *= i;
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate work
    }
    return result;
}

void consumer(int id, std::shared_future<int> shared_result) {
    if (shared_result.valid()) {
         std::cout << "Consumer (" << id << ") : Factorial = " << shared_result.get() << std::endl;
    } else {
        std::cout << "Shared_future is invalid" << std::endl;
    }
}

int main() {
    // Launch a computation asynchronously
    std::future<int> fut_res = std::async(std::launch::async, compute_factorial, 5);

    // Convert to shared_future so multiple threads can access the result
    std::shared_future<int> shared_result = fut_res.share();


    std::vector<std::thread> consumer_threads;
    for (int i = 1; i <= 3; ++i) {
        consumer_threads.emplace_back(consumer, i, shared_result);
    }

    // Wait for all threads to finish
    for (auto& t : consumer_threads) {
        t.join();
    }
    return 0;
}
