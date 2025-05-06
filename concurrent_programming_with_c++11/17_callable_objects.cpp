/*
https://www.youtube.com/watch?v=nU18p75u1oQ&list=PL5jc9xFGsL8E12so1wlMS0r0hTQoJL74M&index=8

A callable object is something that can be called like a function, 
with the syntax object() or object(args); 
that is, a function pointer, or functor: an object of a class type 
that overloads operator().
The overload of operator() in your class makes it callable.

std::bind
https://en.cppreference.com/w/cpp/utility/functional/bind
std::bind creates a callable object by binding arguments to a function.
This is particularly useful when working with threads (std::thread),
as it allows you to pass functions with pre-bound arguments.



*/
#include<thread>
#include<iostream>
#include<future>
#include<functional> // for bind

using namespace std;

class A {
public:
    int operator()(int N) { return 0; }
};

void test0() {
    A a;
    // 
    std::thread t1(a, 6);
    //std::async(std::launch::async, a, 6);
    auto bd = std::bind(a, 6);
}

void printMessage(const std::string& message, int repeatCount) {
    for (int i = 0; i < repeatCount; ++i) {
        std::cout << "Thread says: " << message << " (Repeat: " << i + 1 << ")\n";
    }
}

int test1() {
    // Bind arguments to the function
    std::string myMessage = "Hello, Thread!";
    int repeats = 3;

    // Create a callable object with std::bind
    auto boundFunction = std::bind(printMessage, myMessage, repeats);

    // Pass the bound function to std::thread
    std::thread t(boundFunction);

    // Join the thread to wait for its completion
    t.join();
    return 0;
}

int main() {
    test0();
    test1();
}