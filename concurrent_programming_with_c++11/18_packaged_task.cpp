/*
std::packaged_task is used to wrap a callable (like a function, lambda, or function object)
and associates it with a std::future object, allowing you to asynchronously execute
the callable and retrieve its result later.

Key Points:
(1) Purpose: It allows you to package a callable (e.g., a function) so that
    its execution can be deferred and the result can be accessed asynchronously via std::future.
(2) Associates with std::future: The result of the callable is stored in a std::future,
    which you can use to check the state of the task or retrieve its result.
(3) Thread-Safe: You can pass the std::packaged_task to another thread for execution.
*/

#include <iostream>
#include <thread>
#include <future>
#include <functional>

// Function to demonstrate a long-running task
int computeSum(int a, int b) {
    std::this_thread::sleep_for(std::chrono::seconds(2)); // Simulate a delay
    return a + b;
}

int main() {
    // Step 1: Wrap the function with std::packaged_task
    std::packaged_task<int(int, int)> pkg_task(computeSum);

    // Step 2: Get the future associated with the task
    std::future<int> future_result = pkg_task.get_future();

    // Step 3: Run the task asynchronously in a separate thread
    std::thread t(std::move(pkg_task), 5, 10);

    // Step 4: Do other work in the main thread (if needed)
    std::cout << "Main thread is doing other work...\n";

    // Step 5: Retrieve the result from the future
    std::cout << "Waiting for the result...\n";
    int sum = future_result.get(); // Blocks until the result is ready
    std::cout << "Result: " << sum << "\n";

    // Step 6: Join the thread
    t.join();

    return 0;
}
