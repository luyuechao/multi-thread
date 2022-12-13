/*
we want to pass an object from one thread to another thread 
and get result
*/
#include<mutex>
#include<condition_variable>
#include<future>
#include<thread>
#include<iostream>

using namespace std;

mutex mu;
condition_variable cond;

void factorial(int N, int& x) {
    int res = 1;
    for (int i=N; i>1; i--) {
        res *= i;
    }
    cout << "Result is: " << res << endl;
    cond.notify_all();
    x += res;
}
/*
we want to get results from a certain thread
it's quite complicated with mutex, unique_lock and
condition variable 
*/

void test0() {
    // x is shared between main thread and t1,
    // so a mutex is used to protect x
    // condition variable cond is used to notify the finish
    int x = 2;
    thread t1(factorial, 4, ref(x));
    unique_lock<mutex> locker(mu);
    // cond.wait will put current thread into sleep until get notfiied
    cond.wait(locker);
    locker.unlock();
    t1.join();
    cout << "x = " << x << endl;
}


int factorial_async(int N){
    int res = 1;
    for (int i=N; i>1; i--) {
        res *= i;
    }
    cout <<"factorial_async(), thread ID = " << this_thread::get_id() << endl;
    return res;
}

void test1() {
    int x{0};
    // async is function and returns the future object 
    future<int> fu = async(factorial_async, 4);
    // equivelent to
    // future<int> fu = async(launch::async | launch::deferred, factorial_async, 4);    
    x = fu.get();

    cout << "test1(), thread ID =  " << this_thread::get_id() << endl;
    cout << "note that fu.get can only be called once." << endl;
    // x = fu.get(); // crash
}

// still the same thread, but
//  with std::launch::deferred, fu fatorial_async will only 
// be executed when fu.get() is called 
void test2() {
    int x{0};
    future<int> fu = async(launch::deferred, factorial_async, 4);
    cout << "test2(), thread ID =  " << this_thread::get_id() << endl;
    x = fu.get();
}

// force the compiler to create a new thread 
void test3() {
    int x{0};
    future<int> fu = async(launch::async, factorial_async, 4);
    cout << "test3(), thread ID =  " << this_thread::get_id() << endl;
    x = fu.get();
}


int main(){
    cout << "************************** test0() **********************" << endl;
    test0();
    cout << "too much trouble for global mutex and conditional variable. " << endl;
    cout << "************************** test1() **********************" << endl;
    cout << "call factorial in different threads. " << endl;
    test1();

    cout << "************************** test2() **********************" << endl;
    test2();

    cout << "************************** test3() **********************" << endl;
    test3();

}