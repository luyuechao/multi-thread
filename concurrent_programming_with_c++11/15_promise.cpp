/*
senario 1:
we want to assign some job to another thread
and get result later.
But some parameter hasn't been decided yet,
Use promise for the parameter

senario 2: one shot inter-thread communcation
*/

#include<future>
#include<thread>
#include<iostream>

using namespace std;

int factorial_async(std::future<int>& f_param){
    int res { 1 };

    const int N = f_param.get(); // one get() of future can only be called once
    for (int i=N; i>1; i--) {
        res *= i;
    }
    cout <<"factorial_async(), thread ID = " << this_thread::get_id() << endl;
    return res;
}

void test0() {
    std::promise<int> pms;
    std::future<int> f_param = pms.get_future();
    std::future<int> fu = std::async(std::launch::async, factorial_async, std::ref(f_param));

    // do something else
    std::this_thread::sleep_for(std::chrono::seconds(1));
    pms.set_value(4);
    auto res = fu.get();
    std::cout << "Result from the other thread: " << res
    << ", this thread ID : " << this_thread::get_id() << std::endl;
}

void test_broken_promise() {

    std::promise<int> p;
    std::future<int> f_param = p.get_future();
    std::future<int> fu = std::async(std::launch::async, factorial_async, std::ref(f_param));

    // do something else
    std::this_thread::sleep_for(std::chrono::seconds(1));
    p.set_exception(std::make_exception_ptr(std::runtime_error("To error is human")));
    auto res = fu.get();
    std::cout << "Result from the other thread: " << res
              << ", this thread ID : " << this_thread::get_id() << std::endl;
}


int main(){
    cout << "************************** test0() **********************" << endl;
    test0();

    cout << "**************** test broken promise ************************* " << std::endl;
    test_broken_promise();
}