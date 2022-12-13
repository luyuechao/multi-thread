/*
https://www.youtube.com/watch?v=nU18p75u1oQ&list=PL5jc9xFGsL8E12so1wlMS0r0hTQoJL74M&index=8

A callable object is something that can be called like a function, 
with the syntax object() or object(args); 
that is, a function pointer, or functor: an object of a class type 
that overloads operator().
The overload of operator() in your class makes it callable.

std::bind
https://en.cppreference.com/w/cpp/utility/functional/bind
The function template bind generates a forwarding call wrapper for 
f. Calling this wrapper is equivalent to invoking f with some of its 
arguments bound to args.



*/
#include<thread>
#include<iostream>
#include<future>
#include<functional> // for bind

using namespace std;

class A {
public:
    void f(int x, char c) {}
    long g(double x) { return 0; }
    int operator()(int N) { return 0; }
};

int main() {
    A a;
    // 
    std::thread t1(a, 6);
    std::async(std::launch::async, a, 6);
    auto bd = std::bind(a, 6);
    
}