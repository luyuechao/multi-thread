/*
promise<T>
The class template std::promise provides a facility to 
store a value or an exception that is later acquired asynchronously
 via a std::future object created by the std::promise object. 
 
 Note that the std::promise object is meant to be used only once.
*/
#include<mutex>
#include<condition_variable>
#include<future>
#include<thread>
#include<iostream>

using namespace std;

mutex mu;
condition_variable cond;


// pass an object to a thread in the future, (not the time when creating the thread)
// I'll send you the value in future
// I don't have the value yet, it's a promise
int factorial_promise(future<int>& f){
    int res{1};
    int N = f.get();
    for (int i=N; i>1; i--) {
        res *= i;
    }
    cout <<"factorial_promise(), thread ID = " << this_thread::get_id() << endl;
    return res;
}
// note that the promize and future are template class
// its class it int becuase the variable we want to pass is intger
void test0() {
    int x{0};
    // note that promise and future cannot be copied, can only be moved
    promise<int> p;
    future<int> f = p.get_future();
    future<int> fu = async(launch::async, factorial_promise, ref(f));
    cout << " do something else " << endl;
    this_thread::sleep_for(chrono::milliseconds(20));
    cout << " keep the promise " << endl;
    p.set_value(4);
    cout << "get the result " << endl;
    x = fu.get();
    cout << "x = " << x << endl;
}

// borken promise
void test1() {
    int x{0};
    promise<int> p;
    future<int> f = p.get_future();
    future<int> fu = async(launch::async, factorial_promise, ref(f));
    cout << " do something else " << endl;
    this_thread::sleep_for(chrono::milliseconds(20));
    cout << " forgot the promise!" << endl;
    cout << "get the result " << endl;
    //x = fu.get();
    cout << "x = " << x << endl;
}

void test2() {
    int x{0};
    promise<int> p;
    future<int> f = p.get_future();
    future<int> fu = async(launch::async, factorial_promise, ref(f));
    cout << " do something else " << endl;
    this_thread::sleep_for(chrono::milliseconds(20));
    p.set_exception(std::make_exception_ptr(std::runtime_error("To err is human")));
    //x = fu.get();
    cout << "x = " << x << endl;
}

void test3() {

    promise<int> p;
    promise<int> p2 = std::move(p);

}

int main(){
    cout << "************************** test0() **********************" << endl;
    test0();
    cout << "************************** test1() **********************" << endl; 
    test2();
}